#include "PeopleDetector.h"

PeopleDetector::PeopleDetector(){

}

PeopleDetector::~PeopleDetector(){

}

void PeopleDetector::draw(cv::Mat& image, const std::vector<cv::Rect>& bbox){
    std::for_each(bbox.begin(), bbox.end(), [&](const cv::Rect& rect){cv::rectangle(image, rect, cv::Scalar(40,40,200), 2, CV_AA);});
}
