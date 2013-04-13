/*
 * MotionDescriptor.cpp
 *
 *  Created on: Apr 3, 2013
 *      Author: zxwang
 */

#include "MotionDescriptor.h"

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
        cv::accumulateWeighted(m_flow_magnitude, m_motion_map, 0.1);
    }
	m_prev = gray.clone();
}

float MotionDescriptor::get_motion_score(const cv::Rect& bbox){
    if(m_motion_map.empty())
        return 0.f;
    cv::Mat sub_motion_map = m_motion_map(bbox);
    cv::Scalar sum = cv::sum(sub_motion_map);
    return static_cast<float>(sum[0]);
}
