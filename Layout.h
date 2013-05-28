#ifndef LAYOUT_H
#define LAYOUT_H

#include "Fusion.h"
#include <QPainter>
#include <QWidget>
#include "Utility.h"
class Layout : public QWidget
{
    Q_OBJECT
public:
    static Layout* instance();
    void set_rect_vector(const std::vector<cv::Rect>& rect_vector);
protected:
    void paintEvent(QPaintEvent *);
private:
    std::vector<cv::Point2f> m_point_vector;
    static Layout* p_instance;
    std::list<std::vector<cv::Point2f> > m_point_vector_list;
};

#endif // LAYOUT_H
