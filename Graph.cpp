#include "Graph.h"

Graph* Graph::p_instance = nullptr;

Graph::Graph(){

}

Graph* Graph::instance(){
    if(p_instance == nullptr){
        p_instance = new Graph;
    }
    return p_instance;
}

void Graph::page_rank(cv::Mat& r){
    double rows = static_cast<double>(r.rows);
    for(int i = 0; i < 10; ++i){
        r = 0.85 * m_M * r + 0.15 / rows;
    }
}
