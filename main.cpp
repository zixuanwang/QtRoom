#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    cv::Mat kernel_x = cv::getGaussianKernel(128, 16, CV_32F);
    cv::Mat kernel_y = cv::getGaussianKernel(128, 16, CV_32F);
    cv::Mat kernel = kernel_x * kernel_y.t();
    cv::Mat normalize_kernel;
    Utility::normalize_image(kernel, normalize_kernel);
    cv::namedWindow("test");
    cv::imshow("test", normalize_kernel);
    cv::waitKey(0);
    return app.exec();
}
