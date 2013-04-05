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
