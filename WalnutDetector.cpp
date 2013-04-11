#include "WalnutDetector.h"

WalnutDetector::WalnutDetector() : m_cascade_detector(HOG_UPPER_BODY_PATH){
}

WalnutDetector::~WalnutDetector(){

}

void WalnutDetector::detect(const cv::Mat& image, std::vector<cv::Rect>& bbox){
    std::vector<cv::Rect> rect_vector;
    m_cascade_detector.detect(image, rect_vector);
    m_motion_descriptor.compute(image);
    for(size_t i = 0; i < rect_vector.size(); ++i){
        float score = m_motion_descriptor.get_motion_score(rect_vector[i]) / (rect_vector[i].width * rect_vector[i].height);
        if(score > 0.5f)
            bbox.push_back(rect_vector[i]);
        qDebug() << i << " : " << score;
    }
}

cv::Mat WalnutDetector::debug(const cv::Mat& image){
    m_motion_descriptor.compute(image);
    cv::Mat map = m_motion_descriptor.get_motion_map();
    cv::Mat normalize_map;
    Utility::normalize_image(map, normalize_map);
    cv::imwrite("/home/zxwang/Desktop/1.jpg", normalize_map);
    return image;
}
