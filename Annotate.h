#ifndef ANNOTATE_H
#define ANNOTATE_H

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
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
    void move_rect(int index, int x, int y);
    void delete_rect(int index);
    cv::Rect get_rect(int index){return m_rect_vector[index];}
    int get_bound_rect(int x, int y); // get the index of the bounding rect. if no bounding rect is found, -1 is returned.
    bool in_rect(const cv::Rect& rect, int x, int y); // check the point whether in the rect.
    void set_image(const cv::Mat& image){m_image = image;}
    void save(const std::string& output_dir);
    // overload the output function.
    friend std::ostream& operator<<(std::ostream& os, const Annotate& annotate);
private:
    int m_index;
    std::vector<cv::Rect> m_rect_vector;
    cv::Mat m_image;
};

#endif // ANNOTATE_H
