#ifndef UTILITY_H
#define UTILITY_H

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/unordered_map.hpp>
#include <chrono>
#include <fstream>
#include "GlobalConfig.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QImage>
#include <random>
#include <unordered_map>
#include <unordered_set>

class Utility
{
public:
    Utility();
    static QImage mat2QImage(const cv::Mat3b& src); // image is of type CV_8UC3.
    static float l1_norm(const std::vector<float>& v);
    static float l2_norm(const std::vector<float>& v);
    static void l1_normalize(std::vector<float>& v);
    static void l2_normalize(std::vector<float>& v);
    static float l1_norm(const std::unordered_map<int, float>& v);
    static float l2_norm(const std::unordered_map<int, float>& v);
    static void l1_normalize(std::unordered_map<int, float>& v);
    static void l2_normalize(std::unordered_map<int, float>& v);
    static void normalize_image(const cv::Mat& image, cv::Mat& output);
    static void get_files(std::vector<std::string>& file_vector, const std::string& directory, bool recursive = false, const std::string& extension = "");
    static std::string get_parent_path(const std::string& filepath); // /usr/bin/foo.txt returns /usr/bin
    static std::string get_parent_name(const std::string& filepath); // /usr/bin/foo.txt returns bin
    static std::string get_stem(const std::string& filepath); // /usr/bin/foo.txt returns foo
    static std::string get_extension(const std::string& filepath); // /usr/bin/foo.txt returns .txt
    static std::string get_stem_path(const std::string& filepath); // /usr/bin/foo.txt returns /usr/bin/foo
    static std::string get_name(const std::string& filepath); // /usr/bin/foo.txt returns foo.txt
    static int64_t get_timestamp();
};

#endif // UTILITY_H
