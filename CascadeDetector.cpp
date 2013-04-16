#include "CascadeDetector.h"

CascadeDetector::CascadeDetector(const std::string& annotate_path){
    std::stringstream ss;
    ss << annotate_path << "/hog_data/cascade.xml";
    m_classifier.load(ss.str());
}

CascadeDetector::~CascadeDetector(){

}

cv::Mat CascadeDetector::debug(const cv::Mat& image){
    return image;
}

void CascadeDetector::detect(const cv::Mat& image, std::vector<cv::Rect>& bbox){
    m_classifier.detectMultiScale(image, bbox);
}

int CascadeDetector::get_count(){
    return 0;
}
