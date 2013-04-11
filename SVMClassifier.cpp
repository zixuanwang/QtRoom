#include "SVMClassifier.h"

SVMClassifier::SVMClassifier(void) {
}

SVMClassifier::~SVMClassifier(void) {
}

void SVMClassifier::build() {
    if (m_mat.empty()) {
        m_mat = cv::Mat(m_data, false);
        m_mat = m_mat.reshape(0, static_cast<int>(m_label_vector.size()));
    }
    m_params.svm_type = CvSVM::C_SVC;
    m_params.kernel_type = CvSVM::RBF;
    m_params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
    std::vector<float> label_vector(m_label_vector.begin(), m_label_vector.end());
    cv::Mat label_mat(label_vector);
    m_SVM.train(m_mat, label_mat, cv::Mat(), cv::Mat(), m_params);
}

float SVMClassifier::query(const std::vector<float>& sample) {
    cv::Mat sample_mat(sample);
    sample_mat = sample_mat.reshape(0, 1);
    return m_SVM.predict(sample_mat);
}

void SVMClassifier::save(const std::string& filepath) {
    m_SVM.save(filepath.c_str());
}

void SVMClassifier::load(const std::string& filepath) {
    m_SVM.load(filepath.c_str());
}
