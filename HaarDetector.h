#ifndef HAARDETECTOR_H
#define HAARDETECTOR_H
#include "PeopleDetector.h"

class HaarDetector : public PeopleDetector
{
public:
    HaarDetector(const std::string& classifier_file);
    virtual ~HaarDetector();
    void detect(const cv::Mat& image, std::vector<cv::Rect>& bbox);
private:
    cv::CascadeClassifier m_classifier;
};

#endif // HAARDETECTOR_H
