#include "WalnutDetector.h"

WalnutDetector::WalnutDetector(const std::string& annotate_path) : m_count(0), m_cascade_detector(annotate_path){
    std::stringstream ss;
    ss << annotate_path << "/geometry.conf";
    set_geometry_config(ss.str());
    m_temporal_record = cv::Mat(480, 640, CV_16UC1, cv::Scalar(0));
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
        //qDebug() << i << " : " << score << "\t" << rect_vector[i].width;
    }
    geometry_filter(bbox);
    compute_belief(bbox);
    m_count = static_cast<int>(bbox.size());
}

cv::Mat WalnutDetector::debug(const cv::Mat& image){
    m_motion_descriptor.compute(image);
    cv::Mat map = m_motion_descriptor.get_motion_map();
    cv::Mat normalize_map;
    Utility::normalize_image(map, normalize_map);
    cv::imwrite("/home/zxwang/Desktop/1.jpg", normalize_map);
    return image;
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
        m_belief_map = cv::Mat(GlobalConfig::FULL_HEIGHT, GlobalConfig::FULL_WIDTH, CV_8UC1, cv::Scalar(0));
    }
    cv::Mat object_binary = cv::Mat(GlobalConfig::FULL_HEIGHT, GlobalConfig::FULL_WIDTH, CV_8UC1, cv::Scalar(0));
    for(size_t i = 0; i < bbox.size(); ++i){
        cv::Mat sub_object_binary = object_binary(bbox[i]);
        sub_object_binary = 1;
    }
    m_belief_map_list.push_back(object_binary);
    m_belief_map += object_binary;
    if(m_belief_map_list.size() > 10){
        m_belief_map -= m_belief_map_list.front();
        m_belief_map_list.pop_front();
    }
}
