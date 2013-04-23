#include "ParticleFilter.h"

ParticleFilter::ParticleFilter()
{
}

void ParticleFilter::init(const cv::Mat& image, const cv::Rect& bbox){
    get_color_model(image(bbox), m_color_template);
    m_box_size = bbox.width;
    m_std = static_cast<float>(bbox.width) * 0.05f;
    // initialize particles
    int center_x = bbox.x + bbox.width / 2;
    int center_y = bbox.y + bbox.height / 2;
    for(int i = 0; i < 100; ++i){
        m_point_vector.push_back(cv::Point(center_x + m_normal(m_generator) * m_std, center_y + m_normal(m_generator) * m_std));
        m_weight_vector.push_back(1.f);
    }
}

void ParticleFilter::get_color_model(const cv::Mat& patch, std::vector<float>& model){
    if(!model.empty())
        model.clear();
    cv::Mat hsv;
    cv::cvtColor(patch, hsv, CV_BGR2HSV);
    std::vector<cv::Mat> hsv_vector;
    cv::split(hsv, hsv_vector);
    cv::Mat s_mask = hsv_vector[1] > 25;
    cv::Mat v_mask = hsv_vector[2] > 50;
    cv::Mat mask = s_mask & v_mask;
    int hbins = 10;
    int sbins = 10;
    int vbins = 10;
    int hs_size[] = {hbins, sbins};
    float hranges[] = {0.f, 180.f};
    float sranges[] = {25.f, 255.f};
    const float* hs_ranges[] = {hranges, sranges};
    int hs_channels[] = {0, 1};
    cv::MatND hs_hist;
    cv::calcHist(&patch, 1, hs_channels, mask, hs_hist, 2, hs_size, hs_ranges);
    int v_size[] = {vbins};
    float vranges[] = {0.f, 255.f};
    const float* v_ranges[] = {vranges};
    int v_channels[] = {2};
    cv::MatND v_hist;
    cv::calcHist(&patch, 1, v_channels, cv::Mat(), v_hist, 1, v_size, v_ranges);
    model.reserve(hbins * sbins + vbins);
    for(int i = 0; i < hbins; ++i){
        for(int j = 0; j < sbins; ++j){
            model.push_back(hs_hist.at<float>(i, j));
        }
    }
    for(int i = 0; i < vbins; ++i){
        model.push_back(v_hist.at<float>(i));
    }
    Utility::l1_normalize(model);
}

float ParticleFilter::similarity(const std::vector<float>& v1, const std::vector<float>& v2){
    if(v1.empty() || v2.empty() || v1.size() != v2.size())
        return 0.f;
    float sum = 0.f;
    for(size_t i = 0; i < v1.size(); ++i){
        sum += sqrtf(v1[i] * v2[i]);
    }
    return sqrtf(1 - sum);
}

float ParticleFilter::likelihood(const cv::Mat& patch){
    std::vector<float> current_model;
    get_color_model(patch, current_model);
    float distance = similarity(current_model, m_color_template);
    return expf(-20.f * distance * distance);
}

void ParticleFilter::transition(const cv::Mat& image){
    for(size_t i = 0; i < m_point_vector.size(); ++i){
        m_point_vector[i].x += m_normal(m_generator) * m_std;
        m_point_vector[i].y += m_normal(m_generator) * m_std;
        cv::Point& pos = m_point_vector[i];
        cv::Rect rect(pos.x - m_box_size / 2, pos.y - m_box_size / 2, m_box_size, m_box_size);
        m_weight_vector[i] = likelihood(image(rect));
    }
    Utility::l1_normalize(m_weight_vector);
}

void ParticleFilter::resample(){

}

void ParticleFilter::draw(cv::Mat& image){
    for(size_t i = 0; i < m_point_vector.size(); ++i){
        cv::circle(image, m_point_vector[i], 2, cv::Scalar(220, 50, 40), 2, CV_AA);
    }
}
