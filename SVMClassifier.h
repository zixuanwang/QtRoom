#ifndef SVMCLASSIFIER_H
#define SVMCLASSIFIER_H

#include "Classifier.h"

class SVMClassifier : public Classifier
{
public:
    SVMClassifier(void);
    ~SVMClassifier(void);
    // build the classifier structure. called after adding samples.
    void build();
    // query
    float query(const std::vector<float>& sample);
    // serialize the data structures.
    void save(const std::string& filepath);
    void load(const std::string& filepath);
private:
    CvSVM m_SVM;
    CvSVMParams m_params;
};

#endif // SVMCLASSIFIER_H
