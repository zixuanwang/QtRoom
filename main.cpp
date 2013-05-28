#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Fusion::instance()->init();
	MainWindow w;
	w.show();
	return app.exec();
}
