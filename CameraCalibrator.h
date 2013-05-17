#pragma once

#include "Utility.h"

class CameraCalibrator
{
public:
	CameraCalibrator(void);
	~CameraCalibrator(void);
	int addChessboardPoints(const std::vector<std::string>& filelist, cv::Size& boardSize);
	void addPoints(const std::vector<cv::Point2f>& imageCorners, const std::vector<cv::Point3f>& objectCorners);
	double calibrate(cv::Size& imageSize);
	void calibrate(const std::string& point_path, const std::string& filepath);
	cv::Mat remap(const cv::Mat& image);
	cv::Mat getCameraMatrix();
	cv::Mat getDistCoeffs();
	void save(const std::string& filepath);
private:
	std::vector<std::vector<cv::Point3f> > objectPoints;
	std::vector<std::vector<cv::Point2f> > imagePoints;
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;
	int flag;
	cv::Mat map1;
	cv::Mat map2;
	bool mustInitUndistort;
};
