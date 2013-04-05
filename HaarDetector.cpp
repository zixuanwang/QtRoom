#include "HaarDetector.h"

HaarDetector::HaarDetector(const std::string& classifier_file){
    m_classifier.load(classifier_file);
}

HaarDetector::~HaarDetector(){

}

void HaarDetector::detect(const cv::Mat& image, std::vector<cv::Rect>& bbox){
    m_classifier.detectMultiScale(image, bbox);
}
