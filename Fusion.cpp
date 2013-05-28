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
        ss << GlobalConfig::CALIBRATE_PATH << "/" << ip << "/camera.xml";
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
    // propagate 75 and 76 to 73.

}

cv::Point3f Fusion::convert_3d(const std::string& ip, const cv::Point2f& point, float height){
    cv::Mat n1 = m_mat_map[ip][2].inv();
    cv::Mat q1 = m_mat_map[ip][0].inv();
    double* n1_ptr = (double*)n1.data;
    double* q1_ptr = (double*)q1.data;
    double* t1_ptr = (double*)m_mat_map[ip][1].data;
    double coeff = q1_ptr[6] * (n1_ptr[0] * point.x + n1_ptr[1] * point.y + n1_ptr[2]);
    coeff += q1_ptr[7] * (n1_ptr[3] * point.x + n1_ptr[4] * point.y + n1_ptr[5]);
    coeff += q1_ptr[8] * (n1_ptr[6] * point.x + n1_ptr[7] * point.y + n1_ptr[8]);
    double h = (double) height;
    double b = h + q1_ptr[6] * t1_ptr[0] + q1_ptr[7] * t1_ptr[1] + q1_ptr[8] * t1_ptr[2];
    double s = b / coeff;
    cv::Mat p1 = (cv::Mat_<double>(3, 1) << s * point.x, s * point.y , s);
    cv::Mat P = q1 * (n1 * p1 - m_mat_map[ip][1]);
    double* P_ptr = P.ptr<double>(0);
    cv::Point3f ret(P_ptr[0], P_ptr[1], P_ptr[2]);
    return ret;
}

cv::Point2f Fusion::compute_correspondence(const std::string& ip1, const std::string& ip2, const cv::Point2f& point){
	cv::Mat n1 = m_mat_map[ip1][2].inv();
	cv::Mat q1 = m_mat_map[ip1][0].inv();
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

void merge(boost::unordered_map<std::pair<int, int>, float>& confidence){
    if(confidence.size() < 2){
        return;
    }
    float min_distance = FLT_MAX;
    boost::unordered_map<std::pair<int, int>, float>::iterator iter_i, iter_j;
    for(auto i = confidence.begin(); i != confidence.end(); ++i){
        auto j = i;
        ++j;
        for(; j != confidence.end(); ++j){
            float dx = (float)i->first.first - j->first.first;
            float dy = (float)i->first.second - j->first.second;
            float distance = dx * dx + dy * dy;
            if(distance < min_distance){
                min_distance = distance;
                iter_i = i;
                iter_j = j;
            }
        }
    }
    if(min_distance < 30.f){
        std::pair<int, int> p;
        p.first = (iter_i->first.first + iter_j->first.first) / 2;
        p.second = (iter_i->first.second + iter_j->first.second) / 2;
        confidence[p] = std::max(iter_i->second, iter_j->second); // max aggregation.
        confidence.erase(iter_i->first);
        confidence.erase(iter_j->first);
        merge(confidence);
    }
}
