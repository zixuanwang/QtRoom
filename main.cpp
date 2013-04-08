#include "CameraView.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
//    CameraView camera;
//    camera.show();
    return app.exec();
}
