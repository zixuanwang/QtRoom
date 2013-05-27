#ifndef WALNUTDETECTOR_H
#define WALNUTDETECTOR_H

#include <bitset>
#include "CascadeDetector.h"
#include "ColorDescriptor.h"
#include "Fusion.h"
#include "GeometryConstraint.h"
#include "HoGDescriptor.h"
#include "MotionDescriptor.h"
#include "ParticleFilter.h"
#include "SVMClassifier.h"
#include "Utility.h"

/// this class implements more accurate people detector in the iroom.

class WalnutDetector
{
public:
	friend class Fusion;
    WalnutDetector(const std::string&);
    ~WalnutDetector();
    void detect(const cv::Mat& image, std::vector<cv::Rect>& bbox);
    void set_geometry_config(const std::string& config_path);
    void geometry_filter(std::vector<cv::Rect>& bbox); // use the geometry of the camera to filter bounding boxes which are too large or too small.
    void temporal_propagation(const std::vector<cv::Rect>& bbox); // this function will modify the confidence map.
    void compute_confidence(std::vector<cv::Rect>& bbox); // clustering using minshift.
    cv::Mat get_confidence_map(){return m_confidence_map;}
    void draw(cv::Mat& image, const std::vector<cv::Rect>& bbox);
    std::pair<int, float> get_count_belief();
    void merge(std::vector<cv::Rect>& bbox);

	/******* obsolete functions *******/
	void temporal_filter(std::vector<cv::Rect>& bbox); // use the temporal information to smooth the detection result.
private:
    int m_count;
    ParticleFilter m_particle_filter;
    CascadeDetector m_cascade_detector;
    std::shared_ptr<GeometryConstraint> m_geometry_constraint;
    ColorDescriptor m_color_descriptor;
    HoGDescriptor m_hog_descriptor;
    MotionDescriptor m_motion_descriptor;
    SVMClassifier m_svm_classifier;
	boost::unordered_map<std::pair<int, int>, float> m_confidence;

    cv::Mat m_confidence_map;
    std::list<cv::Mat> m_belief_map_list;
    cv::Mat m_temporal_record; // each pixel records the count of this pixel classified as people in recent 16 frames.
    cv::Mat m_gaussian_template;
    std::list<int> m_count_list;
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
