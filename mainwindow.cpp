#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
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
    QPixmap open_pix(":/new/my_resources/document-open-7.png");
    QPixmap group_pix(":/new/my_resources/edit-group.png");
    m_action_load_config = std::shared_ptr<QAction>(new QAction(QIcon(new_pix), "Load Config", this));
    m_action_load_config->setShortcut(tr("Ctrl+N"));
    connect(m_action_load_config.get(), SIGNAL(triggered()), this, SLOT(pick_config()));
    m_action_load_video = std::shared_ptr<QAction>(new QAction(QIcon(open_pix), "Load Video", this));
    m_action_load_video->setShortcut(tr("Ctrl+O"));
    connect(m_action_load_video.get(), SIGNAL(triggered()), this, SLOT(pick_video()));
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
    m_action_haar_detection = std::shared_ptr<QAction>(new QAction(QIcon(group_pix), "Haar Detection", this));
    m_action_haar_detection->setCheckable(true);
    connect(m_action_haar_detection.get(), SIGNAL(toggled(bool)), this, SLOT(haar_detection(bool)));
}

void MainWindow::init_menubar(){
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_action_load_config.get());
    fileMenu->addAction(m_action_load_video.get());
    fileMenu->addSeparator();
    fileMenu->addAction(m_action_exit.get());
    QMenu* detectMenu = menuBar()->addMenu(tr("&Detect"));
    detectMenu->addAction(m_action_haar_detection.get());
}

void MainWindow::init_toolbar(){
    QToolBar* toolbar = addToolBar("main_toolbar");
    toolbar->setIconSize(QSize(32,32));
    toolbar->addAction(m_action_load_config.get());
    toolbar->addAction(m_action_load_video.get());
    toolbar->addAction(m_action_start_recording.get());
    toolbar->addAction(m_action_stop_recording.get());
    toolbar->addAction(m_action_haar_detection.get());
}

void MainWindow::init_widgets(){
    m_gridlayout = std::shared_ptr<QGridLayout>(new QGridLayout);
    int camera_count = static_cast<int>(m_camera_view_vector.size());
    int row_count = 3; // 3 thumbnails per row.
    for(int i = 0; i < camera_count; ++i){
        m_gridlayout->addWidget(m_camera_view_vector[i].get(), i / row_count, i % row_count);
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
    m_camera_view_vector.clear();
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    int result = dialog.exec();
    if(result){
        QString file = dialog.selectedFiles()[0];
        load_config(file.toStdString());
    }
}

void MainWindow::pick_video(){
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    int result = dialog.exec();
    if(result){
        QString file = dialog.selectedFiles()[0];
        m_video_view = std::shared_ptr<CameraView>(new CameraView(file.toStdString()));
        m_video_view->show();
    }
}

void MainWindow::haar_detection(bool run){
    for(size_t i = 0; i < m_camera_view_vector.size(); ++i){
        m_camera_view_vector[i]->set_haar_detection(run);
    }
}

void MainWindow::load_config(const std::string& config_path){
    std::ifstream in_stream;
    in_stream.open(config_path);
    if(in_stream.good()){
        std::string username;
        std::string password;
        getline(in_stream, username);
        getline(in_stream, password);
        std::string ip_address;
        while(getline(in_stream, ip_address)){
            std::shared_ptr<CameraView> camera_view(new CameraView(ip_address, username, password));
            m_camera_view_vector.push_back(camera_view);
        }
        init_widgets();
        in_stream.close();
    }
    m_action_start_recording->setDisabled(false);
}

void MainWindow::status_timer_timeout(){
    m_status_label->setText(QDate::currentDate().toString() + " " + QTime::currentTime().toString());
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
    for(size_t i = 0; i < m_camera_view_vector.size(); ++i){
        m_camera_view_vector[i]->start_recording(m_video_directory);
    }
    m_action_start_recording->setDisabled(true);
    m_action_stop_recording->setDisabled(false);
}

void MainWindow::stop_recording(){
    for(size_t i = 0; i < m_camera_view_vector.size(); ++i){
        m_camera_view_vector[i]->stop_recording();
    }
    m_action_start_recording->setDisabled(false);
    m_action_stop_recording->setDisabled(true);
}
