#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    cv::Mat image = cv::imread("/home/zxwang/Downloads/handle_example/image202.jpg");
    cv::Mat sub_image = image(cv::Rect(-1, -1 , 10, 10) & cv::Rect(200, 200, 10, 10));
    if(sub_image.empty())
        qDebug() << "image is empty";
    return app.exec();
}
