#include "CameraCalibrator.h"


CameraCalibrator::CameraCalibrator(void):flag(0),mustInitUndistort(true)
{
}


CameraCalibrator::~CameraCalibrator(void)
{
}

int CameraCalibrator::addChessboardPoints(const std::vector<std::string>& filelist, cv::Size& boardSize){
	std::vector<cv::Point2f> imageCorners;
	std::vector<cv::Point3f> objectCorners;
	for(int i=0;i<boardSize.height;++i){
		for(int j=0;j<boardSize.width;++j){
			objectCorners.push_back(cv::Point3f((float)i,(float)j,0.0f));
		}
	}
	cv::Mat image;
	int successes=0;
	for(size_t i=0;i<filelist.size();++i){
		image=cv::imread(filelist[i],0);
		bool found=cv::findChessboardCorners(image,boardSize,imageCorners);
		cv::cornerSubPix(image,imageCorners,cv::Size(5,5),cv::Size(-1,-1),cv::TermCriteria(cv::TermCriteria::MAX_ITER+cv::TermCriteria::EPS,30,0.1));
		if((int)imageCorners.size()==boardSize.area()){
			addPoints(imageCorners,objectCorners);
			++successes;
		}
	}
	return successes;
}

void CameraCalibrator::addPoints(const std::vector<cv::Point2f>& imageCorners, const std::vector<cv::Point3f>& objectCorners){
	imagePoints.push_back(imageCorners);
	objectPoints.push_back(objectCorners);
}

double CameraCalibrator::calibrate(cv::Size& imageSize){
	mustInitUndistort=true;
	std::vector<cv::Mat> rvecs, tvecs;
	return cv::calibrateCamera(objectPoints,imagePoints,imageSize,cameraMatrix,distCoeffs,rvecs,tvecs,flag);
}

void CameraCalibrator::calibrate(const std::string& point_path, const std::string& filepath){
	std::ifstream in_stream;
	in_stream.open(point_path);
	if(in_stream.good()){
		std::vector<std::vector<cv::Point3f> > obj_point_vector(1);
		std::vector<std::vector<cv::Point2f> > img_point_vector(1);
		std::string line;
		getline(in_stream, line);
		int num_points = boost::lexical_cast<int>(line);
		for(int i = 0; i < num_points; ++i){
			getline(in_stream, line);
			std::vector<std::string> obj_strings;
			boost::split(obj_strings, line, boost::is_any_of(","));
			getline(in_stream, line);
			std::vector<std::string> img_strings;
			boost::split(img_strings, line, boost::is_any_of(","));
			obj_point_vector[0].push_back(cv::Point3f(boost::lexical_cast<float>(obj_strings[0]), boost::lexical_cast<float>(obj_strings[1]), boost::lexical_cast<float>(obj_strings[2])));
			img_point_vector[0].push_back(cv::Point2f(boost::lexical_cast<float>(img_strings[0]), boost::lexical_cast<float>(img_strings[1])));
		}
		cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << 6.28e+002, 0., 3.25e+002, 0., 6.30e+002, 1.99e+002, 0., 0., 1.);
		cv::Mat dist_coeff = (cv::Mat_<double>(1, 5) << -1.22e-001, 6.30e-001, -2.28e-002, 1.52e-003, -1.91e+000);
		std::vector<cv::Mat> R;
		std::vector<cv::Mat> t;
		cv::calibrateCamera(obj_point_vector, img_point_vector, cv::Size(640, 480), camera_matrix, dist_coeff, R, t, CV_CALIB_USE_INTRINSIC_GUESS);
		cv::FileStorage file(filepath, cv::FileStorage::WRITE);
		file << "camera" << camera_matrix;
		file << "dist" << dist_coeff;
		file << "r" << R[0];
		file << "t" << t[0];
		file.release();
		in_stream.close();
	}
}

cv::Mat CameraCalibrator::remap(const cv::Mat& image){
	cv::Mat undistorted;
	if(mustInitUndistort){
		cv::initUndistortRectifyMap(cameraMatrix,distCoeffs,cv::Mat(),cv::Mat(),image.size(),CV_32FC1,map1,map2);
		mustInitUndistort=false;
	}
	cv::remap(image,undistorted,map1,map2,cv::INTER_LINEAR);
	return undistorted;
}

cv::Mat CameraCalibrator::getCameraMatrix(){
	return cameraMatrix;
}

cv::Mat CameraCalibrator::getDistCoeffs(){
	return distCoeffs;
}

void CameraCalibrator::save(const std::string& filepath){
	cv::FileStorage file(filepath, cv::FileStorage::WRITE);
	file << "camera" << cameraMatrix;
	file << "dist" << distCoeffs;
	file.release();
}