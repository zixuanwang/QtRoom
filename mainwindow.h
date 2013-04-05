#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Camera.h"
#include "HaarDetector.h"
#include <memory>
#include <QMainWindow>
#include <QLabel>
#include <QtWidgets/QFileDialog>
#include <QTimer>
#include <QToolBar>
#include <unordered_map>
#include "Utility.h"

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
    void init_statusbar();
    cv::Mat process_image(cv::Mat& image);
    Ui::MainWindow *ui;
    std::unordered_map<std::string, std::shared_ptr<Camera> > m_camera_map;
    std::unordered_map<std::string, QLabel*> m_label_map;
    std::shared_ptr<PeopleDetector> m_pdetector;
    std::string m_video_directory;
    QTimer *m_status_timer;
    QAction* m_action_start_recording;
    QAction* m_action_stop_recording;
};

#endif // MAINWINDOW_H
