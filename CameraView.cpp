#include "CameraView.h"

CameraView::CameraView(){
    // test
    m_camera = std::shared_ptr<Camera>(new Camera("171.67.88.36", "WSNLCAM", "y2e2cam"));
    connect(m_camera.get(), SIGNAL(image_changed(QString)), this, SLOT(on_image_changed()));
    m_camera->start();
}

CameraView::~CameraView(){

}

void CameraView::paintEvent(QPaintEvent*){
    QPainter painter(this);
    m_camera->fetch_frame(m_image_buffer);
    painter.drawImage(0, 0, Utility::mat2QImage(m_image_buffer));
    painter.setPen(Qt::blue);
    painter.drawText(10, 20, m_camera->get_ip_address().c_str());
    painter.drawRect(QRect(m_point_start, m_point_current));
}

void CameraView::mousePressEvent(QMouseEvent* event){
    m_point_start = event->pos();
}
void CameraView::mouseReleaseEvent(QMouseEvent* event){
    m_point_current = event->pos();
    repaint();
}
void CameraView::mouseMoveEvent(QMouseEvent *event){
    m_point_current = event->pos();
    repaint();
}

void CameraView::on_image_changed(){
    repaint();
}
