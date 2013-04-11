#include "CascadeDetector.h"

CascadeDetector::CascadeDetector(const std::string& classifier_file){
    m_classifier.load(classifier_file);
}

CascadeDetector::~CascadeDetector(){

}

cv::Mat CascadeDetector::debug(const cv::Mat& image){
    return image;
}

void CascadeDetector::detect(const cv::Mat& image, std::vector<cv::Rect>& bbox){
    m_classifier.detectMultiScale(image, bbox);
}
