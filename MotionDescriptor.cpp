/*
 * MotionDescriptor.cpp
 *
 *  Created on: Apr 3, 2013
 *      Author: zxwang
 */

#include "MotionDescriptor.h"

const int MotionDescriptor::ARRAY_SIZE = 30;

MotionDescriptor::MotionDescriptor() {

}

MotionDescriptor::~MotionDescriptor() {

}

void MotionDescriptor::draw_optical_flow(const cv::Mat& flow,
		cv::Mat& cflow_map, int step) {
	for (int y = 0; y < flow.rows; y += step) {
		for (int x = 0; x < flow.cols; x += step) {
			const cv::Point2f& fxy = flow.at<cv::Point2f>(y, x);
			line(cflow_map, cv::Point(x, y),
					cv::Point(cvRound(x + fxy.x), cvRound(y + fxy.y)), cv::Scalar(0, 0, 255));
            cv::circle(cflow_map, cv::Point(x, y), 2, cv::Scalar(100, 220, 100), -1);
		}
	}
}

void MotionDescriptor::compute(const cv::Mat& image){
	cv::Mat gray = image;
	if(image.type() == CV_8UC3){
		cv::cvtColor(image, gray, CV_BGR2GRAY);
	}
    if(m_flow_magnitude.empty()){
        m_flow_magnitude = cv::Mat(gray.size(), CV_32FC1, cv::Scalar(0));
    }
    if(m_motion_map.empty()){
        m_motion_map = cv::Mat(gray.size(), CV_32FC1, cv::Scalar(0));
    }
    if(!m_prev.empty()){
        cv::calcOpticalFlowFarneback(m_prev, gray, m_flow, 0.5, 3, 15, 3, 5, 1.2, 0); // compute optical flow.
        for (int y = 0; y < m_flow.rows; ++y) {
            for (int x = 0; x < m_flow.cols; ++x) {
                const cv::Point2f& fxy = m_flow.at<cv::Point2f>(y, x);
                m_flow_magnitude.at<float>(y, x) = sqrtf(fxy.x * fxy.x + fxy.y * fxy.y);
            }
        }
        //average_motion_map();
        max_motion_map();
        //nth_motion_map(3); // 100 times slower than max_motion_map.
    }
	m_prev = gray.clone();
}

void MotionDescriptor::average_motion_map(){
    cv::accumulateWeighted(m_flow_magnitude, m_motion_map, 0.1);
}

void MotionDescriptor::nth_motion_map(int n){
    if(m_flow_magnitude_array.empty()){
        m_flow_magnitude_array = boost::multi_array<float, 3>(boost::extents[GlobalConfig::FULL_HEIGHT][GlobalConfig::FULL_WIDTH][ARRAY_SIZE]);
        std::fill(m_flow_magnitude_array.data(), m_flow_magnitude_array.data() + m_flow_magnitude_array.num_elements(), 0.f);
        m_array_pos = 0;
    }
    std::greater<float> greater;
    float array_buffer[ARRAY_SIZE];
    for(int row = 0; row < m_flow_magnitude.rows; ++row){
        float* flow_row_ptr = m_flow_magnitude.ptr<float>(row);
        float* motion_row_ptr = m_motion_map.ptr<float>(row);
        for(int col = 0; col < m_flow_magnitude.cols; ++col){
            m_flow_magnitude_array[row][col][m_array_pos] = flow_row_ptr[col];
            std::copy(m_flow_magnitude_array[row][col].begin(), m_flow_magnitude_array[row][col].end(), array_buffer);
            std::nth_element(array_buffer, array_buffer + n, array_buffer + ARRAY_SIZE, greater); // find the nth largest magnitude.
            motion_row_ptr[col] = array_buffer[n];
        }
    }
    ++m_array_pos;
    if(m_array_pos >= ARRAY_SIZE)
        m_array_pos = 0;
}

void MotionDescriptor::max_motion_map(){
    if(m_flow_magnitude_list.size() > 30)
        m_flow_magnitude_list.pop_front();
    m_flow_magnitude_list.push_back(m_flow_magnitude.clone());
    cv::Mat max_mat(m_flow_magnitude.size(), CV_32FC1, cv::Scalar(0));
    for(auto iter = m_flow_magnitude_list.begin(); iter != m_flow_magnitude_list.end(); ++iter){
        max_mat = cv::max(max_mat, *iter);
    }
    m_motion_map = max_mat;
}

float MotionDescriptor::get_motion_score(const cv::Rect& bbox){
    if(m_motion_map.empty())
        return 0.f;
    cv::Mat sub_motion_map = m_motion_map(bbox);
    cv::Scalar sum = cv::sum(sub_motion_map);
    return static_cast<float>(sum[0]);
}
