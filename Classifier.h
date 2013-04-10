#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <opencv2/opencv.hpp>
#include "Serializer.h"

class Classifier
{
public:
    Classifier();
    virtual ~Classifier();
    // add a training sample to the classifier.
    void add_sample(const std::vector<float>& sample, int label);
    // build the classifier structure. called after adding samples.
    virtual void build() = 0;
    // query
    virtual float query(const std::vector<float>& sample) = 0;
    // serialize the data structures.
    virtual void save(const std::string& filepath);
    virtual void load(const std::string& filepath);
protected:
    std::vector<float> m_data;
    cv::Mat m_mat;
    std::vector<int> m_label_vector;
};

#endif // CLASSIFIER_H
