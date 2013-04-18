/*
 * MotionDescriptor.h
 *
 *  Created on: Apr 3, 2013
 *      Author: zxwang
 */

#ifndef MOTIONDESCRIPTOR_H_
#define MOTIONDESCRIPTOR_H_
#include "FeatureDescriptor.h"
#include <list>

class MotionDescriptor : public FeatureDescriptor {
public:
	MotionDescriptor();
	virtual ~MotionDescriptor();
	void draw_optical_flow(const cv::Mat& flow, cv::Mat& cflow_map, int step);
    void compute(const cv::Mat& image);
    void average_motion_map(); // running average the motion map.
    void max_motion_map(); // max the motion map.
    cv::Mat get_motion_map(){return m_motion_map;}
    cv::Mat get_flow(){return m_flow;}
    float get_motion_score(const cv::Rect& bbox); // get the motion score within the bounding box.
private:
	cv::Mat m_prev;
    cv::Mat m_flow;
    cv::Mat m_flow_magnitude;
    cv::Mat m_motion_map;
    std::list<cv::Mat> m_flow_magnitude_list;
};

#endif /* MOTIONDESCRIPTOR_H_ */
