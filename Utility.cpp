#include "Utility.h"

Utility::Utility()
{
}

QImage Utility::mat2QImage(const cv::Mat3b& src){
    QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
    for(int y = 0; y < src.rows; ++y){
        const cv::Vec3b* src_row = src[y];
        QRgb* dest_row = (QRgb*)dest.scanLine(y);
        for(int x = 0; x < src.cols; ++x){
            dest_row[x] = qRgba(src_row[x][2], src_row[x][1], src_row[x][0], 255);
        }
    }
    return dest;
}

float Utility::l1_norm(const std::vector<float>& v){
    float sum = 0.f;
    std::for_each(v.begin(), v.end(), [&sum](float v){sum += v;});
    return sum;
}

float Utility::l2_norm(const std::vector<float>& v){
    float sum = 0.f;
    std::for_each(v.begin(), v.end(), [&sum](float v){sum += v * v;});
    return sqrtf(sum);
}

void Utility::l1_normalize(std::vector<float>& v){
    float norm = l1_norm(v);
    if(norm == 0.f){
        std::cerr << "0 norm" << std::endl;
    }else{
        std::for_each(v.begin(), v.end(), [&](float& v){v /= norm;});
    }
}

void Utility::l2_normalize(std::vector<float>& v){
    float norm = l2_norm(v);
    if(norm == 0.f){
        std::cerr << "0 norm" << std::endl;
    }else{
        std::for_each(v.begin(), v.end(), [&](float& v){v /= norm;});
    }
}

void Utility::normalize_image(cv::Mat& image, cv::Mat& output){
    double min_value;
    double max_value;
    cv::minMaxLoc(image, &min_value, &max_value);
    image.convertTo(output, CV_32FC1, 1.0 / max_value);
}
