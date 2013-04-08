#include "CameraView.h"

CameraView::CameraView(const std::string& ip_address, const std::string& username, const std::string& password){
    m_camera = std::shared_ptr<Camera>(new Camera(ip_address, username, password));
    connect(m_camera.get(), SIGNAL(image_changed(QString)), this, SLOT(image_changed()));
    m_type = CAMERA;
    setFixedWidth(THUMBNAIL_WIDTH);
    setFixedHeight(THUMBNAIL_HEIGHT);
    init();
    m_camera->start();
}

CameraView::CameraView(const std::string& video_path){
    m_video_capture.open(video_path);
    m_type = VIDEO;
    setFixedWidth(FULL_WIDTH);
    setFixedHeight(FULL_HEIGHT);
    init();
}

CameraView::~CameraView(){

}

void CameraView::init(){
    m_haar = false;
}

void CameraView::set_haar_detection(bool run){
    m_haar = run;
    if(m_haar && !m_people_detector.get())
        m_people_detector = std::shared_ptr<PeopleDetector>(new HaarDetector("/usr/local/share/OpenCV/haarcascades/haarcascade_upperbody.xml"));
}

void CameraView::start_recording(const std::string& video_directory){
    m_camera->start_recording(video_directory);
}

void CameraView::stop_recording(){
    m_camera->stop_recording();
}

cv::Mat CameraView::process_image(cv::Mat& image){
    if(image.empty())
        return cv::Mat();
    if(m_haar){
        std::vector<cv::Rect> rect_vector;
        m_people_detector->detect(image, rect_vector);
        m_people_detector->draw(image, rect_vector);
    }
    cv::Mat preview_image;
    cv::resize(image, preview_image, cv::Size(width(), height()));
    return preview_image;
}

void CameraView::paintEvent(QPaintEvent*){
    QPainter painter(this);
    if(m_type == CAMERA){
        m_camera->fetch_frame(m_image_buffer);
        //painter.drawText(10, 20, m_camera->get_ip_address().c_str());
    }
    cv::Mat image = process_image(m_image_buffer);
    painter.drawImage(0, 0, Utility::mat2QImage(image));
    painter.drawRect(m_point_topleft.x(), m_point_topleft.y(), m_rect_length, m_rect_length);
    m_annotate.draw(&painter);
}

void CameraView::mousePressEvent(QMouseEvent* event){
    m_point_start = event->pos();
    //qDebug() << event->pos().x() << "\t" << event->pos().y();
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
    if(event->key() == Qt::Key_S){

    }
    repaint();
}

void CameraView::image_changed(){
    repaint();
}

void CameraView::haar_detection(bool run){
    if(run){
        m_people_detector = std::shared_ptr<PeopleDetector>(new HaarDetector("/usr/local/share/OpenCV/haarcascades/haarcascade_upperbody.xml"));
    }else{
        m_people_detector.reset();
    }
}
