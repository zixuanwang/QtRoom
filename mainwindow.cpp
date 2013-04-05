#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_pdetector(NULL){
    ui->setupUi(this);
    init_menubar();
    init_toolbar();
    init_statusbar();
    // set the timer to update the status bar.
    m_status_timer = new QTimer(this);
    m_status_timer->setInterval(1000);
    connect(m_status_timer, SIGNAL(timeout()), this, SLOT(status_timer_timeout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_menubar(){
    connect(ui->action_config, SIGNAL(triggered()), this, SLOT(pick_config()));
    connect(ui->action_haar, SIGNAL(triggered()), this, SLOT(run_haar_detection()));
}

void MainWindow::init_toolbar(){
    QPixmap start_recording_pix(":/new/my_resources/media-record.png");
    QPixmap stop_recording_pix(":/new/my_resources/media-playback-stop.png");
    QPixmap new_pix(":/new/my_resources/document-new-5.png");
    QToolBar *toolbar = addToolBar("main_toolbar");
    toolbar->setIconSize(QSize(32,32));
    QAction* action_load_config = new QAction(QIcon(new_pix), "Load Config", this);
    action_load_config->setShortcut(tr("Ctrl+N"));
    connect(action_load_config, SIGNAL(triggered()), this, SLOT(pick_config()));
    m_action_start_recording = new QAction(QIcon(start_recording_pix), "Start Recording", this);
    m_action_start_recording->setShortcut(tr("Ctrl+S"));
    m_action_start_recording->setDisabled(true);
    connect(m_action_start_recording, SIGNAL(triggered()), this, SLOT(start_recording()));
    m_action_stop_recording = new QAction(QIcon(stop_recording_pix), "Stop Recording", this);
    m_action_stop_recording->setDisabled(true);
    connect(m_action_stop_recording, SIGNAL(triggered()), this, SLOT(stop_recording()));
    toolbar->addAction(action_load_config);
    toolbar->addAction(m_action_start_recording);
    toolbar->addAction(m_action_stop_recording);
}

void MainWindow::init_statusbar(){

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
        int line_count = 1;
        while(getline(in_stream, ip_address)){
            std::shared_ptr<Camera> p_camera(new Camera(ip_address, username, password));
            m_camera_map[ip_address] = p_camera;
            connect(p_camera.get(), SIGNAL(image_changed(QString)), this, SLOT(on_image_changed(QString)));
            m_label_map[ip_address] = findChildren<QLabel*>("label_" + QString::number(line_count++))[0];
            p_camera->start();
        }
        in_stream.close();
    }
    m_action_start_recording->setDisabled(false);// enable start_recording
}

void MainWindow::on_image_changed(QString ip_address){
    std::shared_ptr<Camera> p_camera = m_camera_map[ip_address.toStdString()];
    cv::Mat image;
    p_camera->fetch_frame(image);
    cv::Mat preview_image = process_image(image);
    m_label_map[ip_address.toStdString()]->setPixmap(QPixmap::fromImage(Utility::mat2QImage(preview_image)));
}

void MainWindow::status_timer_timeout(){

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
