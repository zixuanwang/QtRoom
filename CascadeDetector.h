#ifndef CASCADEDETECTOR_H
#define CASCADEDETECTOR_H

#include "PeopleDetector.h"

class CascadeDetector : public PeopleDetector
{
public:
    CascadeDetector(const std::string&);
    ~CascadeDetector();
    void detect(const cv::Mat& image, std::vector<cv::Rect>& bbox);
    cv::Mat debug(const cv::Mat& image);
    int get_count();
private:
    cv::CascadeClassifier m_classifier;
};

#endif // CASCADEDETECTOR_H
