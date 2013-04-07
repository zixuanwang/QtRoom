#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Camera.h"
#include "HaarDetector.h"
#include <memory>
#include <QDate>
#include <QGridLayout>
#include <QMainWindow>
#include <QLabel>
#include <QtWidgets/QFileDialog>
#include <QTime>
#include <QTimer>
#include <QToolBar>
#include <unordered_map>
#include "Utility.h"

#define THUMBNAIL_WIDTH 320
#define THUMBNAIL_HEIGHT 240

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void pick_config();
    void run_haar_detection();
    void load_config(const std::string& config_path);
    void on_image_changed(QString ip_address);
    void status_timer_timeout();
    void start_recording();
    void stop_recording();
private:
    void init_menubar();
    void init_toolbar();
    void init_widgets();
    void init_statusbar();
    void init_timers();
    cv::Mat process_image(cv::Mat& image);
    Ui::MainWindow *ui;
    std::unordered_map<std::string, std::shared_ptr<Camera> > m_camera_map;
    std::unordered_map<std::string, std::shared_ptr<QLabel> > m_label_map;
    std::shared_ptr<PeopleDetector> m_pdetector;
    std::string m_video_directory;


    // UI widgets
    std::shared_ptr<QGridLayout> m_gridlayout;
    std::vector<std::shared_ptr<QLabel> > m_label_vector;
    std::shared_ptr<QAction> m_action_load_config;
    std::shared_ptr<QAction> m_action_start_recording;
    std::shared_ptr<QAction> m_action_stop_recording;
    std::shared_ptr<QTimer> m_status_timer;
    std::shared_ptr<QLabel> m_status_label;
};

#endif // MAINWINDOW_H
