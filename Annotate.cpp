#include "Annotate.h"

Annotate::Annotate() {
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

void Annotate::move_rect(int index, int x, int y){
    m_rect_vector[index].x = x;
    m_rect_vector[index].y = y;
}

void Annotate::delete_rect(int index){
    m_rect_vector.erase(m_rect_vector.begin() + index);
}

int Annotate::get_bound_rect(int x, int y){
    for(size_t i = 0; i < m_rect_vector.size(); ++i){
        if(in_rect(m_rect_vector[i], x, y))
            return static_cast<int>(i);
    }
    return -1;
}

bool Annotate::in_rect(const cv::Rect& rect, int x, int y){
    return x >= rect.x && x < rect.x + rect.width && y >=rect.y && y < rect.y + rect.height;
}

std::ostream& operator<<(std::ostream& os, const Annotate& annotate){
    std::for_each(annotate.m_rect_vector.begin(), annotate.m_rect_vector.end(), [&](const cv::Rect& rect){os << rect.x << "," << rect.y << "," << rect.width << "," << rect.height << std::endl;});
    return os;
}

void Annotate::save(const std::string& output_dir){
    if(!boost::filesystem::exists(output_dir))
        boost::filesystem::create_directories(output_dir);
    std::string prefix = output_dir + "/" + m_timestamp;
    cv::imwrite(prefix + ".jpg", m_image);
    std::ofstream out_stream(prefix + ".txt");
    out_stream << (*this);
    out_stream.close();
}
