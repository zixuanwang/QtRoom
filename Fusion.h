#pragma once

#include "WalnutDetector.h"

class WalnutDetector;

class Fusion
{
public:
	static Fusion* instance();
	void init();
	void spatial_propagation();
    cv::Point3f convert_3d(const std::string& ip, const cv::Point2f& point, float height = 1.f);
	cv::Point2f compute_correspondence(const std::string& ip1, const std::string& ip2, const cv::Point2f& point);
	void set_walnut_detector(const std::string& ip, const std::shared_ptr<WalnutDetector>& detector);
    void merge(boost::unordered_map<std::pair<int, int>, float>& confidence); // merge the confidence points in one view.
private:
	static Fusion* p_instance;
	Fusion();
	Fusion(const Fusion&){}
	std::vector<std::string> m_ip_vector;
	std::unordered_map<std::string, std::vector<cv::Mat> > m_mat_map; // key is the ip of the camera, value is R, t, A.
	std::unordered_map<std::string, std::shared_ptr<WalnutDetector> > m_detector_map;
};

