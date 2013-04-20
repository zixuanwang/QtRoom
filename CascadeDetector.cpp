#include "CascadeDetector.h"

CascadeDetector::CascadeDetector(const std::string& annotate_path){
    std::stringstream ss;
    ss << annotate_path << "/hog_data/cascade.xml";
    m_classifier.load(ss.str());
}

CascadeDetector::~CascadeDetector(){

}

void CascadeDetector::detect(const cv::Mat& image, std::vector<cv::Rect>& bbox){
    m_classifier.detectMultiScale(image, bbox, 1.1, 3, 0, cv::Size(30, 30), cv::Size(100, 100));
}

void CascadeDetector::draw(cv::Mat& image, const std::vector<cv::Rect>& bbox){
    std::for_each(bbox.begin(), bbox.end(), [&](const cv::Rect& rect){cv::rectangle(image, rect, cv::Scalar(40,40,200), 2, CV_AA);});
}
