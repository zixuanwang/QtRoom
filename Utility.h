#ifndef UTILITY_H
#define UTILITY_H

#include <opencv2/opencv.hpp>
#include <QImage>

class Utility
{
public:
    Utility();
    static QImage mat2QImage(const cv::Mat3b& src); // image is of type CV_8UC3.
};

#endif // UTILITY_H
