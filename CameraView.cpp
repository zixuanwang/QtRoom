#include "CameraView.h"

CameraView::CameraView(const std::string& ip_address, const std::string& username, const std::string& password){
    m_camera = std::shared_ptr<Camera>(new Camera(ip_address, username, password));
    connect(m_camera.get(), SIGNAL(image_changed()), this, SLOT(image_changed()));
    m_type = CAMERA;
    m_id = ip_address;
    setFixedWidth(GlobalConfig::THUMBNAIL_WIDTH);
    setFixedHeight(GlobalConfig::THUMBNAIL_HEIGHT);
    init();
    m_camera->start();
}

CameraView::CameraView(const std::string& video_path, Type type){
    m_video_capture.open(video_path);
    m_timestamp_stream.open(Utility::get_stem_path(video_path) + ".timestamp");
    m_type = type;
    m_id = Utility::get_stem(video_path);
    if(m_type == ANNOTATE){
        setFixedWidth(GlobalConfig::FULL_WIDTH);
        setFixedHeight(GlobalConfig::FULL_HEIGHT);
    }
    if(m_type == VIDEO){
        setFixedWidth(GlobalConfig::THUMBNAIL_WIDTH);
        setFixedHeight(GlobalConfig::THUMBNAIL_HEIGHT);
    }
    init();
}

CameraView::~CameraView(){
    if(m_video_capture.isOpened()){
        m_video_capture.release();
    }
    if(m_timestamp_stream.is_open()){
        m_timestamp_stream.close();
    }
}

void CameraView::init(){
    QPixmap plot_pix(":/new/my_resources/draw-path.png");
    m_action_plot = std::shared_ptr<QAction>(new QAction(QIcon(plot_pix), "Show Plot", this));
    connect(m_action_plot.get(), SIGNAL(triggered()), this, SLOT(show_plot()));
    m_pick_rect_index = -1;
    m_mode = NORMAL; // set the initial mode.s
}

void CameraView::set_mode(MODE mode){
    m_mode = mode;
    std::stringstream ss;
    ss << GlobalConfig::ANNOTATE_PATH << "/" << m_id;
    if(m_mode == CASCADE){
        if(m_cascade_detector == nullptr)
            m_cascade_detector = std::shared_ptr<CascadeDetector>(new CascadeDetector(ss.str()));
    }
    if(m_mode == WALNUT || m_mode == BELIEF_MAP){
        if(m_walnut_detector == nullptr)
            m_walnut_detector = std::shared_ptr<WalnutDetector>(new WalnutDetector(ss.str()));
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
    if(m_mode == CASCADE){
        std::vector<cv::Rect> rect_vector;
        m_cascade_detector->detect(image, rect_vector);
        m_cascade_detector->draw(image, rect_vector);
    }
    if(m_mode == WALNUT){
        std::vector<cv::Rect> rect_vector;
        m_walnut_detector->detect(image, rect_vector);
        m_walnut_detector->draw(image, rect_vector);
    }
    if(m_mode == BACKGROUND){
        cv::Mat foreground;
        m_color_descriptor.update_background(image, foreground);
        if(!foreground.empty())
            cv::cvtColor(foreground, image, CV_GRAY2BGR);
        //m_color_descriptor.get_background(image);
    }
    if(m_mode == OPTICAL_FLOW){
        m_motion_descriptor.compute(image);
        cv::Mat flow = m_motion_descriptor.get_flow();
        m_motion_descriptor.draw_optical_flow(flow, image, 16);
    }
    if(m_mode == MOTION_MAP){
        m_motion_descriptor.compute(image);
        cv::Mat motion_map = m_motion_descriptor.get_motion_map();
        cv::Mat normalize_motion_map;
        Utility::normalize_image(motion_map, normalize_motion_map);
        cv::Mat gray_motion_map;
        normalize_motion_map.convertTo(gray_motion_map, CV_8UC1, 255.0);
        cv::cvtColor(gray_motion_map, image, CV_GRAY2BGR);
    }
    if(m_mode == BELIEF_MAP){
        std::vector<cv::Rect> rect_vector;
        m_walnut_detector->detect(image, rect_vector);
        m_walnut_detector->draw(image, rect_vector);
        cv::Mat belief_map = m_walnut_detector->get_belief_map();
        cv::Mat normalize_belief_map;
        belief_map.convertTo(normalize_belief_map, CV_8UC1, 10.0);
        cv::cvtColor(normalize_belief_map, image, CV_GRAY2BGR);
    }
    cv::Mat preview_image;
    cv::resize(image, preview_image, cv::Size(width(), height()));
    return preview_image;
}

bool CameraView::event(QEvent* event){
    if(event->type() == QEvent::ToolTip){
        QHelpEvent* help_event = static_cast<QHelpEvent*>(event);
        QToolTip::showText(help_event->globalPos(), m_id.c_str());
        return true;
    }
    return QWidget::event(event);
}

void CameraView::paintEvent(QPaintEvent*){
    QPainter painter(this);
    if(m_type == CAMERA){
        m_camera->fetch_frame(m_image_buffer);
        //painter.drawText(10, 20, m_camera->get_ip_address().c_str());
    }
    cv::Mat image = process_image(m_image_buffer);
    painter.drawImage(0, 0, Utility::mat2QImage(image));
    if(m_type == ANNOTATE && m_pick_rect_index == -1)
        painter.drawRect(m_point_topleft.x(), m_point_topleft.y(), m_rect_length, m_rect_length);
    m_annotate.draw(&painter);
}

void CameraView::mousePressEvent(QMouseEvent* event){
    if(m_type == ANNOTATE){
        m_pick_rect_index = m_annotate.get_bound_rect(event->pos().x(), event->pos().y());
        if(m_pick_rect_index != -1){
            cv::Rect rect = m_annotate.get_rect(m_pick_rect_index);
            m_point_topleft = QPoint(rect.x, rect.y);
        }
        m_point_start = event->pos();
    }
    //qDebug() << event->pos().x() << "\t" << event->pos().y();
}
void CameraView::mouseReleaseEvent(QMouseEvent*){
    if(m_type == ANNOTATE){
        if(m_pick_rect_index == -1)
            m_annotate.add_rect(m_point_topleft.x(), m_point_topleft.y(), m_rect_length, m_rect_length);
    }
    repaint();
}
void CameraView::mouseMoveEvent(QMouseEvent *event){
    if(m_type == ANNOTATE){
        if(m_pick_rect_index != -1){
            m_annotate.move_rect(m_pick_rect_index, event->pos().x() - m_point_start.x() + m_point_topleft.x(), event->pos().y() - m_point_start.y() + m_point_topleft.y());
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
        getline(m_timestamp_stream, m_timestamp);
    }
    if(event->key() == Qt::Key_C){
        m_annotate.clear();
    }
    if(event->key() == Qt::Key_S){
        // save image and bounding box
        m_annotate.set_image(m_image_buffer, m_timestamp);
        std::stringstream ss;
        ss << GlobalConfig::ANNOTATE_PATH << "/" << m_id;
        m_annotate.save(ss.str());
    }
    if(event->key() == Qt::Key_D){
        if(m_pick_rect_index != -1)
            m_annotate.delete_rect(m_pick_rect_index);
    }
    repaint();
}

void CameraView::contextMenuEvent(QContextMenuEvent* event){
    QMenu menu(this);
    menu.addAction(m_action_plot.get());
    menu.exec(event->globalPos());
}

void CameraView::image_changed(){
    repaint();
}

void CameraView::read_image(){
    if(m_video_capture.isOpened()){
        m_video_capture >> m_image_buffer;
        repaint();
    }
}

void CameraView::show_plot(){
    m_plot_widget = std::shared_ptr<QWidget>(new QWidget);
    m_plot_widget->setWindowTitle("People Plot");
    m_plot = std::shared_ptr<PeoplePlot>(new PeoplePlot(m_plot_widget.get()));
    m_plot->setTitle(m_id.c_str());
    m_plot->set_walnut_detector(m_walnut_detector);
    const int margin = 5;
    m_plot->setContentsMargins( margin, margin, margin, margin );
    m_plot_layout = std::shared_ptr<QVBoxLayout>(new QVBoxLayout(m_plot_widget.get()));
    m_plot_layout->addWidget(m_plot.get());
    m_plot_widget->resize(600, 400);
    m_plot_widget->show();
}
