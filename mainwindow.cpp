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
    QPixmap annotate_pix(":/new/my_resources/document-sign-2.png");
    QPixmap group_pix(":/new/my_resources/edit-group.png");
    QPixmap background_pix(":/new/my_resources/tab-detach.png");
    QPixmap optical_flow_pix(":/new/my_resources/tools-wizard-3.png");
    QPixmap walnut_pix(":/new/my_resources/food-kiwi.png");
    m_action_load_config = std::shared_ptr<QAction>(new QAction(QIcon(new_pix), "Load Config", this));
    m_action_load_config->setShortcut(tr("Ctrl+N"));
    connect(m_action_load_config.get(), SIGNAL(triggered()), this, SLOT(pick_config()));
    m_action_load_video = std::shared_ptr<QAction>(new QAction(QIcon(open_pix), "Load Video", this));
    m_action_load_video->setShortcut(tr("Ctrl+O"));
    connect(m_action_load_video.get(), SIGNAL(triggered()), this, SLOT(load_video()));
    m_action_annotate_video = std::shared_ptr<QAction>(new QAction(QIcon(annotate_pix), "Annotate Video", this));
    connect(m_action_annotate_video.get(), SIGNAL(triggered()), this, SLOT(pick_video()));
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
    m_action_cascade_detection = std::shared_ptr<QAction>(new QAction(QIcon(group_pix), "Cascade Detection", this));
    m_action_cascade_detection->setCheckable(true);
    m_action_cascade_detection->setDisabled(true);
    connect(m_action_cascade_detection.get(), SIGNAL(toggled(bool)), this, SLOT(cascade_detection(bool)));
    m_action_background_subtraction = std::shared_ptr<QAction>(new QAction(QIcon(background_pix), "Background Subtraction", this));
    m_action_background_subtraction->setCheckable(true);
    m_action_background_subtraction->setDisabled(true);
    connect(m_action_background_subtraction.get(), SIGNAL(toggled(bool)), this, SLOT(background_subtraction(bool)));
    m_action_optical_flow = std::shared_ptr<QAction>(new QAction(QIcon(optical_flow_pix), "Optical Flow", this));
    m_action_optical_flow->setCheckable(true);
    m_action_optical_flow->setDisabled(true);
    connect(m_action_optical_flow.get(), SIGNAL(toggled(bool)), this, SLOT(optical_flow(bool)));
    m_action_motion_map = std::shared_ptr<QAction>(new QAction("Motion Map", this));
    m_action_motion_map->setCheckable(true);
    m_action_motion_map->setDisabled(true);
    connect(m_action_motion_map.get(), SIGNAL(toggled(bool)), this, SLOT(motion_map(bool)));
    m_action_belief_map = std::shared_ptr<QAction>(new QAction("Belief Map", this));
    m_action_belief_map->setCheckable(true);
    m_action_belief_map->setDisabled(true);
    connect(m_action_belief_map.get(), SIGNAL(toggled(bool)), this, SLOT(belief_map(bool)));
    m_action_train_hog = std::shared_ptr<QAction>(new QAction("Train HoG", this));
    connect(m_action_train_hog.get(), SIGNAL(triggered()), this, SLOT(train_hog()));
    m_action_prepare_cascade = std::shared_ptr<QAction>(new QAction("Prepare Cascade", this));
    connect(m_action_prepare_cascade.get(), SIGNAL(triggered()), this, SLOT(prepare_cascade()));
    m_action_walnut_detection = std::shared_ptr<QAction>(new QAction(QIcon(walnut_pix), "Detection", this));
    m_action_walnut_detection->setCheckable(true);
    m_action_walnut_detection->setDisabled(true);
    connect(m_action_walnut_detection.get(), SIGNAL(toggled(bool)), this, SLOT(walnut_detection(bool)));
    m_action_about = std::shared_ptr<QAction>(new QAction("About", this));
    connect(m_action_about.get(), SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::init_menubar(){
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_action_load_config.get());
    fileMenu->addAction(m_action_load_video.get());
    fileMenu->addSeparator();
    fileMenu->addAction(m_action_annotate_video.get());
    fileMenu->addSeparator();
    fileMenu->addAction(m_action_exit.get());
    QMenu* detectMenu = menuBar()->addMenu(tr("&Detect"));
    detectMenu->addAction(m_action_cascade_detection.get());
    detectMenu->addAction(m_action_background_subtraction.get());
    detectMenu->addAction(m_action_optical_flow.get());
    detectMenu->addAction(m_action_motion_map.get());
    detectMenu->addAction(m_action_belief_map.get());
    detectMenu->addSeparator();
    detectMenu->addAction(m_action_walnut_detection.get());
    QMenu* trainMenu = menuBar()->addMenu(tr("&Train"));
    trainMenu->addAction(m_action_train_hog.get());
    trainMenu->addAction(m_action_prepare_cascade.get());
    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(m_action_about.get());
}

void MainWindow::init_toolbar(){
    QToolBar* toolbar = addToolBar("main_toolbar");
    toolbar->setIconSize(QSize(32,32));
    toolbar->addAction(m_action_load_config.get());
    toolbar->addAction(m_action_load_video.get());
    toolbar->addAction(m_action_annotate_video.get());
    toolbar->addAction(m_action_start_recording.get());
    toolbar->addAction(m_action_stop_recording.get());
    toolbar->addAction(m_action_cascade_detection.get());
    toolbar->addAction(m_action_background_subtraction.get());
    toolbar->addAction(m_action_optical_flow.get());
    toolbar->addAction(m_action_walnut_detection.get());
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
    // set the timer to update the status bar.
    m_status_timer = std::shared_ptr<QTimer>(new QTimer(this));
    m_status_timer->setInterval(1000);
    connect(m_status_timer.get(), SIGNAL(timeout()), this, SLOT(status_timer_timeout()));
    m_status_timer->start();
}

void MainWindow::enable_buttons(){
    m_action_start_recording->setDisabled(false);
    m_action_cascade_detection->setDisabled(false);
    m_action_background_subtraction->setDisabled(false);
    m_action_optical_flow->setDisabled(false);
    m_action_motion_map->setDisabled(false);
    m_action_belief_map->setDisabled(false);
    m_action_walnut_detection->setDisabled(false);
}

void MainWindow::pick_config(){
    QString filepath = QFileDialog::getOpenFileName(this, tr("Open Configuration"), QDir::homePath(), tr("Configuration Files (*.conf)"));
    if(filepath.size() != 0){
        load_config(filepath.toStdString());
    }
}

void MainWindow::load_video(){
    m_camera_view_vector.clear();
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(dir.size() != 0){
        std::vector<std::string> video_path_vector;
        Utility::get_files(video_path_vector, dir.toStdString(), false, ".avi");
        for(size_t i = 0; i < video_path_vector.size(); ++i){
            std::shared_ptr<CameraView> camera_view(new CameraView(video_path_vector[i], CameraView::VIDEO));
            m_camera_view_vector.push_back(camera_view);
        }
        // set the timer to update the video frame.
        m_video_timer = std::shared_ptr<QTimer>(new QTimer(this));
        m_video_timer->setInterval(1000);
        connect(m_video_timer.get(), SIGNAL(timeout()), this, SLOT(video_timer_timeout()));
        m_video_timer->start();
        init_widgets();
        enable_buttons();
    }
}

void MainWindow::pick_video(){
    QString filepath = QFileDialog::getOpenFileName(this, tr("Open Video"), QDir::homePath(), tr("Video Files (*.avi)"));
    if(filepath.size() != 0){
        m_video_view = std::shared_ptr<CameraView>(new CameraView(filepath.toStdString(), CameraView::ANNOTATE));
        m_video_view->show();
    }
}

void MainWindow::cascade_detection(bool run){
    for(size_t i = 0; i < m_camera_view_vector.size(); ++i){
        m_camera_view_vector[i]->set_mode(run ? CameraView::CASCADE : CameraView::NORMAL);
    }
}

void MainWindow::background_subtraction(bool run){
    for(size_t i = 0; i < m_camera_view_vector.size(); ++i){
        m_camera_view_vector[i]->set_mode(run ? CameraView::BACKGROUND : CameraView::NORMAL);
    }
}

void MainWindow::optical_flow(bool run){
    for(size_t i = 0; i < m_camera_view_vector.size(); ++i){
        m_camera_view_vector[i]->set_mode(run ? CameraView::OPTICAL_FLOW : CameraView::NORMAL);
    }
}

void MainWindow::motion_map(bool run){
    for(size_t i = 0; i < m_camera_view_vector.size(); ++i){
        m_camera_view_vector[i]->set_mode(run ? CameraView::MOTION_MAP : CameraView::NORMAL);
    }
}

void MainWindow::belief_map(bool run){
    for(size_t i = 0; i < m_camera_view_vector.size(); ++i){
        m_camera_view_vector[i]->set_mode(run ? CameraView::BELIEF_MAP : CameraView::NORMAL);
    }
}

void MainWindow::walnut_detection(bool run){
    for(size_t i = 0; i < m_camera_view_vector.size(); ++i){
        m_camera_view_vector[i]->set_mode(run ? CameraView::WALNUT : CameraView::NORMAL);
    }
}

void MainWindow::load_config(const std::string& config_path){
    m_camera_view_vector.clear();
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
    enable_buttons();
}

void MainWindow::status_timer_timeout(){
    m_status_label->setText(QDate::currentDate().toString() + " " + QTime::currentTime().toString());
}

void MainWindow::video_timer_timeout(){
    for(size_t i = 0; i < m_camera_view_vector.size(); ++i){
        m_camera_view_vector[i]->read_image();
    }
}

void MainWindow::start_recording(){
    if(m_video_directory.empty()){
        QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(dir.size() == 0)
            return;
        m_video_directory = dir.toStdString();
    }
    std::string video_directory = m_video_directory + "/" + boost::lexical_cast<std::string>(Utility::get_timestamp());
    boost::filesystem::create_directories(video_directory);
    for(size_t i = 0; i < m_camera_view_vector.size(); ++i){
        m_camera_view_vector[i]->start_recording(video_directory);
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

void MainWindow::train_hog(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(dir.size() != 0){
        Trainer::train_hog(dir.toStdString());
    }
}

void MainWindow::prepare_cascade(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(dir.size() != 0){
        Trainer::train_cascade(dir.toStdString());
        QMessageBox::information(this, "Preparation Finished", "Please run opencv_createsamples and opencv_traincascade");
    }
}

void MainWindow::about(){
    QMessageBox::about(this, "About iRoom", "Version 0.01");
}
