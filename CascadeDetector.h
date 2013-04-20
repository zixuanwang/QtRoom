#ifndef CASCADEDETECTOR_H
#define CASCADEDETECTOR_H

#include "Utility.h"

class CascadeDetector
{
public:
    CascadeDetector(const std::string&);
    ~CascadeDetector();
    void detect(const cv::Mat& image, std::vector<cv::Rect>& bbox);
    void draw(cv::Mat& image, const std::vector<cv::Rect>& bbox);
private:
    cv::CascadeClassifier m_classifier;
};

#endif // CASCADEDETECTOR_H
