#ifndef UTILITY_H
#define UTILITY_H

#include <algorithm>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <QImage>

class Utility
{
public:
    Utility();
    static QImage mat2QImage(const cv::Mat3b& src); // image is of type CV_8UC3.
    static float l1_norm(const std::vector<float>& v);
    static float l2_norm(const std::vector<float>& v);
    static void l1_normalize(std::vector<float>& v);
    static void l2_normalize(std::vector<float>& v);
    static void normalize_image(cv::Mat& image, cv::Mat& output);
    static void get_files(std::vector<std::string>& file_vector, const std::string& directory, bool recursive = false, const std::string& extension = "");
};

#endif // UTILITY_H
