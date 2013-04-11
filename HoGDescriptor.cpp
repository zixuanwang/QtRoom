/*
 * HoGDescriptor.cpp
 *
 *  Created on: Apr 3, 2013
 *      Author: zxwang
 */

#include "HoGDescriptor.h"

HoGDescriptor::HoGDescriptor(int num_bins, int row_cell_count, int column_cell_count) : m_num_bins(num_bins), m_row_cell_count(row_cell_count), m_column_cell_count(column_cell_count) {

}

HoGDescriptor::~HoGDescriptor() {

}

void HoGDescriptor::compute(const cv::Mat& image, const cv::Rect& bbox, std::vector<float>& histogram){
    if(!histogram.empty())
        histogram.clear();
    cv::Mat sub_image = image(bbox);
    cv::Mat sub_magnitude;
    cv::Mat sub_angle;
    compute_gradient(sub_image, sub_magnitude, sub_angle);
    int cell_height = bbox.height / m_row_cell_count;
    int cell_width = bbox.width / m_column_cell_count;
    for(int row = 0; row < m_row_cell_count; ++row){
        for(int col = 0; col < m_column_cell_count; ++col){
            cv::Rect rect(col * cell_width, row * cell_height, cell_width, cell_height);
            std::vector<float> sub_histogram;
            compute_hog(sub_magnitude, sub_angle, rect, sub_histogram);
            std::copy(sub_histogram.begin(), sub_histogram.end(), std::back_inserter(histogram));
        }
    }
    Utility::l2_normalize(histogram);
}

void HoGDescriptor::compute(const cv::Mat& magnitude, const cv::Mat& angle, const cv::Rect& bbox, std::vector<float>& histogram){
    int cell_height = bbox.height / m_row_cell_count;
    int cell_width = bbox.width / m_column_cell_count;
    for(int row = 0; row < m_row_cell_count; ++row){
        for(int col = 0; col < m_column_cell_count; ++col){
            cv::Rect rect(bbox.x + col * cell_width, bbox.y + row * cell_height, cell_width, cell_height);
            std::vector<float> sub_histogram;
            compute_hog(magnitude, angle, rect, sub_histogram);
            std::copy(sub_histogram.begin(), sub_histogram.end(), std::back_inserter(histogram));
        }
    }
    Utility::l2_normalize(histogram);
}

void HoGDescriptor::compute_gradient(const cv::Mat& image, cv::Mat& magnitude, cv::Mat& angle){
    magnitude = cv::Mat(image.size(), CV_32FC1, cv::Scalar(0.f));
    angle = cv::Mat(image.size(), CV_32FC1, cv::Scalar(0.f));
    cv::Mat gray = image;
    if(image.type() == CV_8UC3){
        cv::cvtColor(image, gray, CV_BGR2GRAY);
    }
    for(int row = 0; row < image.rows - 1; ++row){
        uchar* row_ptr = gray.ptr<uchar>(row);
        uchar* row_ptr_next = gray.ptr<uchar>(row + 1);
        float* mag_row_ptr = magnitude.ptr<float>(row);
        float* angle_row_ptr = angle.ptr<float>(row);
        for(int col = 0; col < image.cols - 1; ++col){
            float dx = static_cast<float>(row_ptr[col + 1]) - static_cast<float>(row_ptr[col]);
            float dy = static_cast<float>(row_ptr_next[col]) - static_cast<float>(row_ptr[col]);
            float mag = sqrtf(dx * dx + dy * dy);
            float ang = 0.f;
            if(dx != 0.f || dy != 0.f)
                ang = atanf(dy / dx);
            mag_row_ptr[col] = mag;
            angle_row_ptr[col] = ang;
        }
    }
}

void HoGDescriptor::compute_hog(const cv::Mat& magnitude, const cv::Mat& angle, const cv::Rect& bbox, std::vector<float>& histogram){
    if(magnitude.empty() || angle.empty()){
        std::cerr << "magnitude or angle empty" << std::endl;
        return;
    }
    histogram.assign(m_num_bins, 0.f);
    cv::Mat sub_magnitude = magnitude(bbox);
    cv::Mat sub_angle= angle(bbox);
    for(int row = 0; row < sub_magnitude.rows - 1; ++row){
        float* mag_row_ptr = sub_magnitude.ptr<float>(row);
        float* angle_row_ptr = sub_angle.ptr<float>(row);
        for(int col = 0; col < sub_magnitude.cols - 1; ++col){
            float mag = mag_row_ptr[col];
            float ang = angle_row_ptr[col];
            int bin_index = m_num_bins * (ang / PI +  0.5f);
            //std::cout << bin_index << std::endl;
            if(bin_index == m_num_bins){
                bin_index = m_num_bins - 1;
            }
            histogram[bin_index] += mag;
        }
    }
    Utility::l2_normalize(histogram);
}
