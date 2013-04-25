#ifndef WALNUTDETECTOR_H
#define WALNUTDETECTOR_H

#include <bitset>
#include "CascadeDetector.h"
#include "ColorDescriptor.h"
#include "GeometryConstraint.h"
#include "HoGDescriptor.h"
#include "MotionDescriptor.h"
#include "ParticleFilter.h"
#include "SVMClassifier.h"
#include "Utility.h"

#define HAAR_UPPER_BODY_PATH "/usr/local/share/OpenCV/haarcascades/haarcascade_upperbody.xml"

/// this class implements more accurate people detector in the iroom.

class WalnutDetector
{
public:
    WalnutDetector(const std::string&);
    ~WalnutDetector();
    void detect(const cv::Mat& image, std::vector<cv::Rect>& bbox);
    void set_geometry_config(const std::string& config_path);
    void geometry_filter(std::vector<cv::Rect>& bbox); // use the geometry of the camera to filter bounding boxes which are too large or too small.
    void temporal_filter(std::vector<cv::Rect>& bbox); // use the temporal information to smooth the detection result.
    void compute_belief(std::vector<cv::Rect>& bbox); // clustering using minshift.
    cv::Mat get_belief_map(){return m_belief_map;}
    void draw(cv::Mat& image, const std::vector<cv::Rect>& bbox);
    int get_count(){return m_count;}
    void merge(std::vector<cv::Rect>& bbox);
private:
    int m_count;
    ParticleFilter m_particle_filter;
    CascadeDetector m_cascade_detector;
    std::shared_ptr<GeometryConstraint> m_geometry_constraint;
    ColorDescriptor m_color_descriptor;
    HoGDescriptor m_hog_descriptor;
    MotionDescriptor m_motion_descriptor;
    SVMClassifier m_svm_classifier;
    cv::Mat m_belief_map;
    std::list<cv::Mat> m_belief_map_list;
    cv::Mat m_temporal_record; // each pixel records the count of this pixel classified as people in recent 16 frames.
    cv::Mat m_gaussian_template;
};

class SimilarRects
{
public:
    SimilarRects(double _eps) : eps(_eps) {}
    inline bool operator()(const cv::Rect& r1, const cv::Rect& r2) const
    {
        double diff_x = r1.x + r1.width / 2 - r2.x - r2.width / 2;
        double diff_y = r1.y + r1.height / 2 - r2.y - r2.height / 2;
        double distance = diff_x * diff_x + diff_y * diff_y;
        return distance < eps * eps;
    }
    double eps;
};

#endif // WALNUTDETECTOR_H
