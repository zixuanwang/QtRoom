/*
 * HoGDescriptor.h
 *
 *  Created on: Apr 3, 2013
 *      Author: zxwang
 */

#ifndef HOGDESCRIPTOR_H_
#define HOGDESCRIPTOR_H_

#include "FeatureDescriptor.h"

class HoGDescriptor : public FeatureDescriptor {
public:
    HoGDescriptor(int num_bins = 9, int row_cell_count = 2, int column_cell_count = 2);
    virtual ~HoGDescriptor();
    void compute(const cv::Mat& image, const cv::Rect& bbox, std::vector<float>& histogram);
    void compute(const cv::Mat& magnitude, const cv::Mat& angle, const cv::Rect& bbox, std::vector<float>& histogram);
    void compute_gradient(const cv::Mat& image, cv::Mat& magnitude, cv::Mat& angle);
    void compute_hog(const cv::Mat& magnitude, const cv::Mat& angle, const cv::Rect& bbox, std::vector<float>& histogram);
private:
	int m_num_bins;
    int m_row_cell_count;
    int m_column_cell_count;
	cv::Mat m_magnitude; // save the magnitude of the gradient.
	cv::Mat m_angle; // save the angle of the gradient.
};

#endif /* HOGDESCRIPTOR_H_ */
