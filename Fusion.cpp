#include "Fusion.h"

Fusion* Fusion::p_instance = nullptr;

Fusion::Fusion(){
	m_ip_vector.push_back("171.67.83.73");
	m_ip_vector.push_back("171.67.83.75");
	m_ip_vector.push_back("171.67.83.76");
}

Fusion* Fusion::instance(){
	if(p_instance == nullptr)
		p_instance = new Fusion;
	return p_instance;
}

void Fusion::init(){
	for(auto &ip : m_ip_vector)
	for(size_t i = 0; i < m_ip_vector.size(); ++i){
		std::stringstream ss;
		ss << "C:/Users/Zixuan/Dropbox/data/iroom/calibrate/" << ip << "/camera.xml";
		cv::FileStorage f(ss.str(), cv::FileStorage::READ);
		cv::Mat R,r,t,A;
		f["camera"] >> A;
		f["r"] >> r;
		f["t"] >> t;
		cv::Rodrigues(r, R);
		m_mat_map[ip].push_back(R);
		m_mat_map[ip].push_back(t);
		m_mat_map[ip].push_back(A);
		f.release();
	}
}

void Fusion::spatial_propagation(){

}

cv::Point2f Fusion::compute_correspondence(const std::string& ip1, const std::string& ip2, const cv::Point2f& point){
	cv::Mat n1 = m_mat_map[ip1][2].inv();
	cv::Mat q1 = m_mat_map[ip1][0].inv();
	std::stringstream ss;
	ss << q1 << std::endl;
	qDebug() << ss.str().c_str();
	double* n1_ptr = (double*)n1.data;
	double* q1_ptr = (double*)q1.data;
	double* t1_ptr = (double*)m_mat_map[ip1][1].data;
	double coeff = q1_ptr[6] * (n1_ptr[0] * point.x + n1_ptr[1] * point.y + n1_ptr[2]);
	coeff += q1_ptr[7] * (n1_ptr[3] * point.x + n1_ptr[4] * point.y + n1_ptr[5]);
	coeff += q1_ptr[8] * (n1_ptr[6] * point.x + n1_ptr[7] * point.y + n1_ptr[8]);
	double h = 1.0;
	double b = h + q1_ptr[6] * t1_ptr[0] + q1_ptr[7] * t1_ptr[1] + q1_ptr[8] * t1_ptr[2];
	double s = b / coeff;
	cv::Mat p1 = (cv::Mat_<double>(3, 1) << s * point.x, s * point.y , s);
	cv::Mat P = q1 * (n1 * p1 - m_mat_map[ip1][1]);
	cv::Mat p2 = m_mat_map[ip2][2] * (m_mat_map[ip2][0] * P + m_mat_map[ip2][1]);
	double* p2_ptr = (double*)p2.data;
	double s2 = p2_ptr[2];
	cv::Point2f point2(p2_ptr[0] / s2, p2_ptr[1] / s2);
	return point2;
}

void Fusion::set_walnut_detector(const std::string& ip, const std::shared_ptr<WalnutDetector>& detector){
	m_detector_map[ip] = detector;
}