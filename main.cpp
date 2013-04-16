#include "CameraView.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    GeometryConstraint c(3.5f, 0.8f, 32.f, 45.f);
    qDebug() << c.get_size(480);
    qDebug() << c.get_size(380);
    qDebug() << c.get_size(280);
    qDebug() << c.get_size(180);
    qDebug() << c.get_size(80);
//    CameraView camera;
//    camera.show();
    return app.exec();
}
