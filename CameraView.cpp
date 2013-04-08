#include "CameraView.h"

CameraView::CameraView(){
    // test
    //m_camera = std::shared_ptr<Camera>(new Camera("171.67.88.36", "WSNLCAM", "y2e2cam"));
    //connect(m_camera.get(), SIGNAL(image_changed(QString)), this, SLOT(image_changed()));
    //m_camera->start();
    m_video_capture.open("/home/zxwang/Dropbox/data/iroom/video1/171.67.83.73.avi");
    m_type = VIDEO;
}

CameraView::~CameraView(){

}

void CameraView::paintEvent(QPaintEvent*){
    QPainter painter(this);
    if(m_type == CAMERA){
        m_camera->fetch_frame(m_image_buffer);
        painter.drawImage(0, 0, Utility::mat2QImage(m_image_buffer));
        painter.drawText(10, 20, m_camera->get_ip_address().c_str());
    }
    if(m_type == VIDEO){
        painter.drawImage(0, 0, Utility::mat2QImage(m_image_buffer));
    }
    painter.drawRect(m_point_topleft.x(), m_point_topleft.y(), m_rect_length, m_rect_length);
    m_annotate.draw(&painter);
}

void CameraView::mousePressEvent(QMouseEvent* event){
    m_point_start = event->pos();
    qDebug() << event->pos().x() << "\t" << event->pos().y();
}
void CameraView::mouseReleaseEvent(QMouseEvent*){
    m_annotate.add_rect(m_point_topleft.x(), m_point_topleft.y(), m_rect_length, m_rect_length);
    repaint();
}
void CameraView::mouseMoveEvent(QMouseEvent *event){
    m_rect_length = std::max(abs(event->pos().x() - m_point_start.x()), abs(event->pos().y() - m_point_start.y()));
    int x = event->pos().x() > m_point_start.x() ? m_point_start.x() : m_point_start.x() - m_rect_length;
    int y = event->pos().y() > m_point_start.y() ? m_point_start.y() : m_point_start.y() - m_rect_length;
    m_point_topleft = QPoint(x, y);
    repaint();
}
void CameraView::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_P){
        m_video_capture >> m_image_buffer;
    }
    if(event->key() == Qt::Key_C){
        m_annotate.clear();
    }
    repaint();
}

void CameraView::image_changed(){
    repaint();
}
