#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
    //Geometry geo;
    //geo.annotate("/home/zxwang/Dropbox/data/iroom/annotate/171.67.83.73/fusion/1367015926522.jpg", "/home/zxwang/Dropbox/data/iroom/annotate/171.67.83.75/fusion/1367015928174.jpg");
    //geo.show();
	//std::string imageDirectory="c:/users/zixuan/desktop/calibrate";
	//std::vector<std::string> filelist;
	//Utility::get_files(filelist, imageDirectory);
	//CameraCalibrator calibrator;
	//cv::Size size1(7,5);
	//cv::Size size2(640,480);
	//calibrator.addChessboardPoints(filelist,size1);
	//calibrator.calibrate(size2);
	//calibrator.save("c:/users/zixuan/desktop/iroom_intrinsic.xml");
	//CameraCalibrator calibrator;
	//calibrator.calibrate("c:/users/zixuan/dropbox/data/iroom/calibrate/171.67.83.76/points.txt", "c:/users/zixuan/dropbox/data/iroom/calibrate/171.67.83.76/camera.xml");
	Geometry geo;
	geo.init_geometry();
	geo.show();
	MainWindow w;
	w.show();
	return app.exec();
}
