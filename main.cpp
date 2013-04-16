#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    cv::Mat mat(3, 3, CV_8UC1, cv::Scalar(0));
    mat -= 1;
//    mat.at<uchar>(0,0) <<= 1;
//    std::bitset<10> b(mat.at<uchar>(0,0));
//    qDebug() << b.count();
    qDebug() << (int)mat.at<uchar>(0, 0);
    return app.exec();
}
