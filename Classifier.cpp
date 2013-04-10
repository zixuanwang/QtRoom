#include "Classifier.h"

Classifier::Classifier(void) {
}

Classifier::~Classifier(void) {
}

void Classifier::add_sample(const std::vector<float>& sample, int label) {
    if (sample.empty()) {
        return;
    }
    std::copy(sample.begin(), sample.end(), std::back_inserter(m_data));
    m_label_vector.push_back(label);
}

void Classifier::save(const std::string& filepath) {
    Serializer::save(m_mat, filepath + ".matrix");
    Serializer::save(m_label_vector, filepath + ".label");
}

void Classifier::load(const std::string& filepath) {
    Serializer::load(m_mat, filepath + ".matrix");
    Serializer::load(m_label_vector, filepath + ".label");
}
