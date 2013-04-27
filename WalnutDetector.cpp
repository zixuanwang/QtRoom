#include "WalnutDetector.h"

WalnutDetector::WalnutDetector(const std::string& annotate_path) : m_count(0), m_cascade_detector(annotate_path){
    std::stringstream ss;
    ss << annotate_path << "/geometry.conf";
    set_geometry_config(ss.str());
    // create gaussian template
    cv::Mat kernel_x = cv::getGaussianKernel(128, 24, CV_32F);
    cv::Mat kernel_y = cv::getGaussianKernel(128, 24, CV_32F);
    cv::Mat kernel = kernel_x * kernel_y.t();
    Utility::normalize_image(kernel, m_gaussian_template);
}

WalnutDetector::~WalnutDetector(){

}

void WalnutDetector::detect(const cv::Mat& image, std::vector<cv::Rect>& bbox){
    if(!bbox.empty())
        bbox.clear();
    if(image.empty())
        return;
    std::vector<cv::Rect> rect_vector;
    m_cascade_detector.detect(image, rect_vector);
    m_motion_descriptor.compute(image);
    for(size_t i = 0; i < rect_vector.size(); ++i){
        float score = m_motion_descriptor.get_motion_score(rect_vector[i]) / (rect_vector[i].width * rect_vector[i].height);
        if(score > 1.f)
            bbox.push_back(rect_vector[i]);
    }
    geometry_filter(bbox);
    merge(bbox);
    compute_belief(bbox);
    if(m_count_list.size() > 20){
        m_count_list.pop_front();
    }
    m_count_list.push_back(static_cast<int>(bbox.size()));
    //m_count = static_cast<int>(bbox.size());
}

void WalnutDetector::set_geometry_config(const std::string& config_path){
    std::ifstream config_stream;
    config_stream.open(config_path);
    if(config_stream.is_open()){
        std::string line;
        getline(config_stream, line);
        float camera_height = boost::lexical_cast<float>(line);
        getline(config_stream, line);
        float object_height = boost::lexical_cast<float>(line);
        getline(config_stream, line);
        float view_angle = boost::lexical_cast<float>(line);
        getline(config_stream, line);
        float theta = boost::lexical_cast<float>(line);
        m_geometry_constraint = std::shared_ptr<GeometryConstraint>(new GeometryConstraint(camera_height, object_height, view_angle, theta));
        config_stream.close();
    }
}

void WalnutDetector::geometry_filter(std::vector<cv::Rect>& bbox){
    if(bbox.empty())
        return;
    if(m_geometry_constraint != nullptr){
        std::vector<cv::Rect> filter_box;
        for(size_t i = 0; i < bbox.size(); ++i){
            int bottom = bbox[i].y + bbox[i].height;
            int size = m_geometry_constraint->get_size(bottom);
            if(static_cast<float>(bbox[i].height) <= static_cast<float>(size) * 1.2f && static_cast<float>(bbox[i].height) >= static_cast<float>(size) * 0.8f){
                filter_box.push_back(bbox[i]);
            }
        }
        bbox.assign(filter_box.begin(), filter_box.end());
    }
}

void WalnutDetector::temporal_filter(std::vector<cv::Rect>& bbox){
    if(bbox.empty())
        return;
    if(m_temporal_record.empty())
        m_temporal_record = cv::Mat(480, 640, CV_16UC1, cv::Scalar(0));
    std::vector<cv::Rect> filter_box;
    for(size_t i = 0; i < bbox.size(); ++i){
        cv::Mat subfilter = m_temporal_record(bbox[i]);
        for(int row = 0; row < subfilter.rows; ++row){
            u_int16_t *row_ptr = subfilter.ptr<u_int16_t>(row);
            for(int col = 0; col < subfilter.cols; ++col){
                row_ptr[col] <<= 1;
            }
        }
        subfilter += 1;
    }
    cv::Mat binary_image(m_temporal_record.size(), CV_8UC1, cv::Scalar(0));
    for(int row = 0; row < m_temporal_record.rows; ++row){
        for(int col = 0; col < m_temporal_record.cols; ++col){
            std::bitset<16> bits(m_temporal_record.at<u_int16_t>(row, col));
            if(bits.count() >= 6){
                binary_image.at<uchar>(row, col) = 1;
            }
        }
    }
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(binary_image, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    for(size_t i = 0; i < contours.size(); ++i){
        filter_box.push_back(cv::boundingRect(contours[i]));
    }
    bbox.assign(filter_box.begin(), filter_box.end());
}

void WalnutDetector::compute_belief(std::vector<cv::Rect>& bbox){
    if(m_belief_map.empty()){
        m_belief_map = cv::Mat(GlobalConfig::FULL_HEIGHT, GlobalConfig::FULL_WIDTH, CV_32FC1, cv::Scalar(0));
    }
    cv::Mat object_binary = cv::Mat(GlobalConfig::FULL_HEIGHT, GlobalConfig::FULL_WIDTH, CV_32FC1, cv::Scalar(0));
    for(size_t i = 0; i < bbox.size(); ++i){
        cv::Mat sub_object_binary = object_binary(bbox[i]);
        cv::Mat gaussian;
        cv::resize(m_gaussian_template, gaussian, cv::Size(sub_object_binary.cols, sub_object_binary.rows));
        sub_object_binary += gaussian;
    }
    m_belief_map_list.push_back(object_binary);
    m_belief_map += object_binary;
    if(m_belief_map_list.size() > 20){
        m_belief_map -= m_belief_map_list.front();
        m_belief_map_list.pop_front();
    }
}

void WalnutDetector::draw(cv::Mat& image, const std::vector<cv::Rect>& bbox){
    m_cascade_detector.draw(image, bbox);
    //m_particle_filter.draw(image);
}

std::pair<int, float> WalnutDetector::get_count_belief(){
    if(m_count_list.empty())
        return std::pair<int, float>(0, 0.f);
    std::unordered_map<int, int> count_map;
    for(int count : m_count_list){
        count_map[count]++;
    }
    auto iter = std::max_element(count_map.begin(), count_map.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b){return a.second < b.second;});
    return std::pair<int, float>(iter->first, static_cast<float>(iter->second) / m_count_list.size());
}

void WalnutDetector::merge(std::vector<cv::Rect>& bbox){
    if(bbox.empty())
        return;
    // use mean shift to merge bbox
    std::vector<cv::Rect> merge_bbox;
    merge_bbox.reserve(bbox.size());
    for(auto box : bbox){
        cv::meanShift(m_belief_map, box, cv::TermCriteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 50, 1.0));
        merge_bbox.push_back(box);
    }
    std::vector<int> labels;
    int nclasses = cv::partition(merge_bbox, labels, SimilarRects(9.0));
    std::vector<cv::Rect> rrects(nclasses);
    std::vector<int> rweights(nclasses, 0);
    for(size_t i = 0; i < labels.size(); ++i){
        int cls = labels[i];
        rrects[cls].x += bbox[i].x;
        rrects[cls].y += bbox[i].y;
        rrects[cls].width += bbox[i].width;
        rrects[cls].height += bbox[i].height;
        rweights[cls]++;
    }
    for(int i = 0; i < nclasses; i++ ){
        cv::Rect r = rrects[i];
        float s = 1.f / rweights[i];
        rrects[i] = cv::Rect(r.x * s, r.y * s, r.width * s, r.height * s);
    }
    bbox.assign(rrects.begin(), rrects.end());
}
