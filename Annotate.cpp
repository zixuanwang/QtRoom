#include "Annotate.h"

Annotate::Annotate()
{
}

void Annotate::add_rect(const cv::Rect& rect){
    m_rect_vector.push_back(rect);
}

void Annotate::add_rect(int x, int y, int width, int height){
    cv::Rect rect(x, y, width, height);
    add_rect(rect);
}

void Annotate::draw(QPainter* painter){
    QPen pen(Qt::red);
    pen.setWidth(3);
    painter->setPen(pen);
    for(size_t i = 0; i < m_rect_vector.size(); ++i){
        painter->drawRect(QRect(m_rect_vector[i].x, m_rect_vector[i].y, m_rect_vector[i].width, m_rect_vector[i].height));
    }
}
