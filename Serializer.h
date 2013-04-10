#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <fstream>
#include <string>

class Serializer
{
public:
    Serializer();

    static void load(cv::Mat& mat, const std::string& path) {
        mat = cv::Mat();
        std::ifstream inStream;
        inStream.open(path.c_str(), std::ios::binary);
        if (inStream.good()) {
            int rows;
            int cols;
            int type;
            inStream.read((char*) &rows, sizeof(rows));
            inStream.read((char*) &cols, sizeof(cols));
            inStream.read((char*) &type, sizeof(type));
            mat = cv::Mat(rows, cols, type);
            size_t length = (size_t) rows * mat.step;
            inStream.read((char*) mat.data, length);
            inStream.close();
        }
    }

    static void save(const cv::Mat& mat, const std::string& path) {
        if (mat.empty()) {
            return;
        }
        std::ofstream outStream;
        outStream.open(path.c_str(), std::ios::binary);
        if (outStream.good()) {
            int rows = mat.rows;
            int cols = mat.cols;
            int type = mat.type();
            outStream.write((char*) &rows, sizeof(rows));
            outStream.write((char*) &cols, sizeof(cols));
            outStream.write((char*) &type, sizeof(type));
            size_t length = (size_t) rows * mat.step;
            outStream.write((char*) mat.data, length);
            outStream.close();
        }
    }

    template<class T> static void load(std::vector<T>& array,
                const std::string& path) {
            array.clear();
            std::ifstream inStream;
            inStream.open(path.c_str(), std::ios::binary);
            if (inStream.good()) {
                size_t size;
                inStream.read((char*) &size, sizeof(size));
                array.assign(size, 0);
                inStream.read((char*) &array[0], sizeof(T) * size);
                inStream.close();
            }
        }
    template<class T> static void save(const std::vector<T>& array,
            const std::string& path) {
        if (array.empty()) {
            return;
        }
        std::ofstream outStream;
        outStream.open(path.c_str(), std::ios::binary);
        if (outStream.good()) {
            size_t size = array.size();
            outStream.write((char*) &size, sizeof(size));
            outStream.write((char*) &array[0], sizeof(T) * size);
            outStream.close();
        }
    }
};

#endif // SERIALIZER_H
