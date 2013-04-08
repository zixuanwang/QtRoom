#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_pdetector(NULL){
    ui->setupUi(this);
    init_actions();
    init_menubar();
    init_toolbar();
    init_widgets();
    init_statusbar();
    init_timers();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_actions(){
    QPixmap start_recording_pix(":/new/my_resources/media-record.png");
    QPixmap stop_recording_pix(":/new/my_resources/media-playback-stop.png");
    QPixmap new_pix(":/new/my_resources/document-new-5.png");
    m_action_load_config = std::shared_ptr<QAction>(new QAction(QIcon(new_pix), "Load Config", this));
    m_action_load_config->setShortcut(tr("Ctrl+N"));
    connect(m_action_load_config.get(), SIGNAL(triggered()), this, SLOT(pick_config()));
    m_action_start_recording = std::shared_ptr<QAction>(new QAction(QIcon(start_recording_pix), "Start Recording", this));
    m_action_start_recording->setShortcut(tr("Ctrl+S"));
    m_action_start_recording->setDisabled(true);
    connect(m_action_start_recording.get(), SIGNAL(triggered()), this, SLOT(start_recording()));
    m_action_stop_recording = std::shared_ptr<QAction>(new QAction(QIcon(stop_recording_pix), "Stop Recording", this));
    m_action_stop_recording->setDisabled(true);
    connect(m_action_stop_recording.get(), SIGNAL(triggered()), this, SLOT(stop_recording()));
    m_action_exit = std::shared_ptr<QAction>(new QAction(tr("E&xit"), this));
    m_action_exit->setStatusTip(tr("Exit the application"));
    connect(m_action_exit.get(), SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::init_menubar(){
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_action_load_config.get());
    fileMenu->addSeparator();
    fileMenu->addAction(m_action_exit.get());
    //connect(ui->action_haar, SIGNAL(triggered()), this, SLOT(run_haar_detection()));
}

void MainWindow::init_toolbar(){
    QToolBar* toolbar = addToolBar("main_toolbar");
    toolbar->setIconSize(QSize(32,32));
    toolbar->addAction(m_action_load_config.get());
    toolbar->addAction(m_action_start_recording.get());
    toolbar->addAction(m_action_stop_recording.get());
}

void MainWindow::init_widgets(){
    m_gridlayout = std::shared_ptr<QGridLayout>(new QGridLayout);
    int camera_count = static_cast<int>(m_camera_map.size());
    int row_count = 3; // 3 thumbnails per row.
    m_label_vector.clear();
    m_label_vector.reserve(camera_count);
    for(int i = 0; i < camera_count; ++i){
        std::shared_ptr<QLabel> label(new QLabel);
        label->setFixedWidth(THUMBNAIL_WIDTH);
        label->setFixedHeight(THUMBNAIL_HEIGHT);
        QPixmap pix(":/new/my_resources/placeholder.png");
        label->setPixmap(pix);
        m_gridlayout->addWidget(label.get(), i / row_count, i % row_count);
        m_label_vector.push_back(label);
    }
    centralWidget()->setLayout(m_gridlayout.get());
}

void MainWindow::init_statusbar(){
    m_status_label = std::shared_ptr<QLabel>(new QLabel);

    statusBar()->addWidget(m_status_label.get());
}

void MainWindow::init_timers(){
    // set the timer to update the status bar.
    m_status_timer = std::shared_ptr<QTimer>(new QTimer(this));
    m_status_timer->setInterval(1000);
    connect(m_status_timer.get(), SIGNAL(timeout()), this, SLOT(status_timer_timeout()));
    m_status_timer->start();
}

void MainWindow::pick_config(){
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    int result = dialog.exec();
    if(result){
        QString file = dialog.selectedFiles()[0];
        load_config(file.toStdString());
    }
}

void MainWindow::run_haar_detection(){
    m_pdetector = std::shared_ptr<PeopleDetector>(new HaarDetector("/usr/local/share/OpenCV/haarcascades/haarcascade_upperbody.xml"));
}

void MainWindow::load_config(const std::string& config_path){
    std::ifstream in_stream;
    in_stream.open(config_path);
    if(in_stream.good()){
        std::string username;
        std::string password;
        // read username and password
        getline(in_stream, username);
        getline(in_stream, password);
        // read ip_address
        std::string ip_address;
        while(getline(in_stream, ip_address)){
            std::shared_ptr<Camera> p_camera(new Camera(ip_address, username, password));
            m_camera_map[ip_address] = p_camera;
            connect(p_camera.get(), SIGNAL(image_changed(QString)), this, SLOT(image_changed(QString)));
        }
        // create labels, and start each camera thread.
        init_widgets();
        int i = 0;
        for(auto iter = m_camera_map.begin(); iter != m_camera_map.end(); ++iter, ++i){
            m_label_map[iter->first] = m_label_vector[i];
            iter->second->start();
        }
        in_stream.close();
    }
    m_action_start_recording->setDisabled(false);
}

void MainWindow::image_changed(QString ip_address){
    std::shared_ptr<Camera> p_camera = m_camera_map[ip_address.toStdString()];
    cv::Mat image;
    p_camera->fetch_frame(image);
    cv::Mat preview_image = process_image(image);
    m_label_map[ip_address.toStdString()]->setPixmap(QPixmap::fromImage(Utility::mat2QImage(preview_image)));
}

void MainWindow::status_timer_timeout(){
    m_status_label->setText(QDate::currentDate().toString() + " " + QTime::currentTime().toString());
}

cv::Mat MainWindow::process_image(cv::Mat& image){
    cv::Mat preview_image;
    if(m_pdetector.get()){
        std::vector<cv::Rect> rect_vector;
        m_pdetector->detect(image, rect_vector);
        m_pdetector->draw(image, rect_vector);
    }
    cv::resize(image, preview_image, cv::Size(), 0.5, 0.5);
    return preview_image;
}

void MainWindow::start_recording(){
    if(m_video_directory.empty()){
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::Directory);
        int result = dialog.exec();
        if(result){
            m_video_directory = dialog.selectedFiles()[0].toStdString();
        }
    }
    for(auto iter = m_camera_map.begin(); iter != m_camera_map.end(); ++iter){
        iter->second->start_recording(m_video_directory);
    }
    m_action_start_recording->setDisabled(true);
    m_action_stop_recording->setDisabled(false);
}

void MainWindow::stop_recording(){
    for(auto iter = m_camera_map.begin(); iter != m_camera_map.end(); ++iter){
        iter->second->stop_recording();
    }
    m_action_start_recording->setDisabled(false);
    m_action_stop_recording->setDisabled(true);
}
