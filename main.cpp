#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    cv::Rect rect;
    qDebug() << rect.x <<  rect.y << rect.width << rect.height;
    return app.exec();
}
