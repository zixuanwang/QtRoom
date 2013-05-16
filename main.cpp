#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
    Geometry geo;
    geo.annotate("/home/zxwang/Dropbox/data/iroom/annotate/171.67.83.73/fusion/1367015926522.jpg", "/home/zxwang/Dropbox/data/iroom/annotate/171.67.83.75/fusion/1367015928174.jpg");
    geo.show();
	MainWindow w;
	w.show();
	return app.exec();
}
