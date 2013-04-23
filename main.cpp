#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    ParticleFilter filter;
    cv::Mat image = cv::imread("/home/zxwang/Downloads/handle_example/image202.jpg");
    std::vector<float> model;
    filter.get_color_model(image, model);
    return app.exec();
}
