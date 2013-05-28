#include "Layout.h"

Layout* Layout::p_instance = nullptr;


Layout* Layout::instance(){
    if(p_instance == nullptr){
        p_instance = new Layout;
    }
    return p_instance;
}

void Layout::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setOpacity(1.0);
    painter.drawImage(0, 0, QImage(":/new/my_resources/iroom_layout.png"));
    float level = (float)m_point_vector_list.size();
    float i = 0.f;
    for(auto &p_v : m_point_vector_list){
        ++i;
        painter.setOpacity(i / level);
        for(auto &p : p_v){
            cv::Point3f p3 = Fusion::instance()->convert_3d("171.67.83.73", p);
            qDebug() << p3.x << "\t" << p3.y << "\t" << p3.z;
            painter.drawImage(200 - 55 * p3.x -16, 280 + 55 * p3.y -16, QImage(":/new/my_resources/im-user.png"));
        }
    }
}

void Layout::set_rect_vector(const std::vector<cv::Rect>& rect_vector){
//    m_point_vector.clear();
//    m_point_vector.reserve(rect_vector.size());
//    for(auto &r: rect_vector){
//        m_point_vector.push_back(cv::Point2f(r.x + r.width / 2, r.y + r.height / 2));
//    }
    std::vector<cv::Point2f> point_vector;
    for(auto &r: rect_vector){
        point_vector.push_back(cv::Point2f(r.x + r.width / 2, r.y + r.height / 2));
    }
    if(m_point_vector_list.size() >= 3)
        m_point_vector_list.pop_front();
    m_point_vector_list.push_back(point_vector);
    repaint();
}
