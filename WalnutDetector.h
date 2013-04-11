#ifndef WALNUTDETECTOR_H
#define WALNUTDETECTOR_H

#include "CascadeDetector.h"
#include "ColorDescriptor.h"
#include "HoGDescriptor.h"
#include "MotionDescriptor.h"
#include "PeopleDetector.h"
#include "SVMClassifier.h"
#include "Utility.h"

#define HAAR_UPPER_BODY_PATH "/usr/local/share/OpenCV/haarcascades/haarcascade_upperbody.xml"
#define LBP_UPPER_BODY_PATH "/home/zxwang/Dropbox/data/iroom/annotate/171.67.83.73/lbp_data/cascade.xml"
#define HOG_UPPER_BODY_PATH "/home/zxwang/Dropbox/data/iroom/annotate/171.67.83.73/hog_data/cascade.xml"

class WalnutDetector : public PeopleDetector
{
public:
    WalnutDetector();
    ~WalnutDetector();
    void detect(const cv::Mat& image, std::vector<cv::Rect>& bbox);
    cv::Mat debug(const cv::Mat& image);
private:
    CascadeDetector m_cascade_detector;
    ColorDescriptor m_color_descriptor;
    HoGDescriptor m_hog_descriptor;
    MotionDescriptor m_motion_descriptor;
    SVMClassifier m_svm_classifier;
};

#endif // WALNUTDETECTOR_H
