#ifndef GRAPH_H
#define GRAPH_H

#include "Utility.h"

class Graph
{
public:
    static Graph* instance();
    void set_M(const cv::Mat& M){m_M = M;}
    void page_rank(cv::Mat& r);
private:
    Graph();
    cv::Mat m_M;
    static Graph* p_instance;
};

#endif // GRAPH_H
