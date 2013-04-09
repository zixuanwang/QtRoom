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
    m_pick_index = -1;
}

void CameraView::set_mode(MODE mode){
    m_mode = mode;
    if(m_mode == HAAR){
        if(!m_people_detector.get()){
            m_people_detector = std::shared_ptr<PeopleDetector>(new HaarDetector(HAAR_UPPER_BODY_PATH));
        }
    }
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
    if(m_mode == HAAR){
        std::vector<cv::Rect> rect_vector;
        m_people_detector->detect(image, rect_vector);
        m_people_detector->draw(image, rect_vector);
    }
    if(m_mode == BACKGROUND){
        m_color_descriptor.update_background(image);
        m_color_descriptor.get_background(image);
    }
    if(m_mode == OPTICAL_FLOW){
        cv::Mat flow;
        m_motion_descriptor.compute(image, flow);
        m_motion_descriptor.draw_optical_flow(flow, image, 16);
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
    if(m_type == VIDEO && m_pick_index == -1)
        painter.drawRect(m_point_topleft.x(), m_point_topleft.y(), m_rect_length, m_rect_length);
    m_annotate.draw(&painter);
}

void CameraView::mousePressEvent(QMouseEvent* event){
    if(m_type == VIDEO){
        m_pick_index = m_annotate.get_bound_rect(event->pos().x(), event->pos().y());
        if(m_pick_index != -1){
            cv::Rect rect = m_annotate.get_rect(m_pick_index);
            m_point_topleft = QPoint(rect.x, rect.y);
        }
        m_point_start = event->pos();
    }

    //qDebug() << event->pos().x() << "\t" << event->pos().y();
}
void CameraView::mouseReleaseEvent(QMouseEvent*){
    if(m_type == VIDEO){
        if(m_pick_index == -1)
            m_annotate.add_rect(m_point_topleft.x(), m_point_topleft.y(), m_rect_length, m_rect_length);
    }
    repaint();
}
void CameraView::mouseMoveEvent(QMouseEvent *event){
    if(m_type == VIDEO){
        if(m_pick_index != -1){
            m_annotate.move_rect(m_pick_index, event->pos().x() - m_point_start.x() + m_point_topleft.x(), event->pos().y() - m_point_start.y() + m_point_topleft.y());
        }else{
            m_rect_length = std::max(abs(event->pos().x() - m_point_start.x()), abs(event->pos().y() - m_point_start.y()));
            int x = event->pos().x() > m_point_start.x() ? m_point_start.x() : m_point_start.x() - m_rect_length;
            int y = event->pos().y() > m_point_start.y() ? m_point_start.y() : m_point_start.y() - m_rect_length;
            m_point_topleft = QPoint(x, y);
        }
    }
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
        // save image and bounding box
        m_annotate.set_image(m_image_buffer);
        m_annotate.save(ANNOTATE_PATH);
    }
    if(event->key() == Qt::Key_D){
        if(m_pick_index != -1)
            m_annotate.delete_rect(m_pick_index);
    }
    repaint();
}

void CameraView::image_changed(){
    repaint();
}
