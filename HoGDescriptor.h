/*
 * HoGDescriptor.h
 *
 *  Created on: Apr 3, 2013
 *      Author: zxwang
 */

#ifndef HOGDESCRIPTOR_H_
#define HOGDESCRIPTOR_H_

#include "FeatureDescriptor.h"

#define PI 3.1415926

class HoGDescriptor : public FeatureDescriptor {
public:
    HoGDescriptor(int num_bins = 9, int row_cell_count = 2, int column_cell_count = 2);
    virtual ~HoGDescriptor();
    void compute(const cv::Mat& image, const cv::Rect& bbox, std::vector<float>& histogram);
    void compute_hog(const cv::Mat& image, std::vector<float>& histogram);
private:
	int m_num_bins;
    int m_row_cell_count;
    int m_column_cell_count;
	cv::HOGDescriptor m_hog;
	cv::Mat m_magnitude; // save the magnitude of the gradient.
	cv::Mat m_angle; // save the angle of the gradient.
};

#endif /* HOGDESCRIPTOR_H_ */
