#ifndef ANNOTATE_H
#define ANNOTATE_H

#include <fstream>
#include <opencv2/opencv.hpp>
#include <QPainter>
#include <QPen>
#include <QPoint>

class Annotate
{
public:
    Annotate();
    void add_rect(const cv::Rect& rect);
    void add_rect(int x, int y, int width, int height);
    std::vector<cv::Rect> get_rect_vector(){return m_rect_vector;}
    void clear(){m_rect_vector.clear();}
    void draw(QPainter* painter);
private:
    std::vector<cv::Rect> m_rect_vector;
};

#endif // ANNOTATE_H
