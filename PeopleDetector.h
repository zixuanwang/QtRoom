#ifndef PEOPLEDETECTOR_H
#define PEOPLEDETECTOR_H

#include <algorithm>
#include <opencv2/opencv.hpp>

class PeopleDetector
{
public:
    PeopleDetector();
    virtual ~PeopleDetector();
    virtual void detect(const cv::Mat& image, std::vector<cv::Rect>& bbox) = 0;
    void draw(cv::Mat& image, const std::vector<cv::Rect>& bbox);
};

#endif // PEOPLEDETECTOR_H
