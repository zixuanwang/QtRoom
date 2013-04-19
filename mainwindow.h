#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CameraView.h"
#include "HoGDescriptor.h"
#include <memory>
#include <QDate>
#include <QFileDialog>
#include <QGridLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QLabel>
#include <QTime>
#include <QTimer>
#include <QToolBar>
#include "SVMClassifier.h"
#include "Trainer.h"
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
    void load_video();
    void pick_video();
    void cascade_detection(bool);
    void background_subtraction(bool);
    void optical_flow(bool);
    void motion_map(bool);
    void belief_map(bool);
    void walnut_detection(bool);
    void load_config(const std::string& config_path);
    void status_timer_timeout();
    void video_timer_timeout();
    void start_recording();
    void stop_recording();
    void train_hog();
    void prepare_cascade();
    void about();
private:
    void init_actions();
    void init_menubar();
    void init_toolbar();
    void init_widgets();
    void init_statusbar();
    void enable_buttons();
    Ui::MainWindow *ui;
    std::string m_video_directory;
    // UI widgets
    std::shared_ptr<QGridLayout> m_gridlayout;
    std::vector<std::shared_ptr<CameraView> > m_camera_view_vector;
    std::shared_ptr<CameraView> m_video_view;
    std::shared_ptr<QAction> m_action_load_config;
    std::shared_ptr<QAction> m_action_load_video;
    std::shared_ptr<QAction> m_action_annotate_video;
    std::shared_ptr<QAction> m_action_start_recording;
    std::shared_ptr<QAction> m_action_stop_recording;
    std::shared_ptr<QAction> m_action_cascade_detection;
    std::shared_ptr<QAction> m_action_background_subtraction;
    std::shared_ptr<QAction> m_action_optical_flow;
    std::shared_ptr<QAction> m_action_motion_map;
    std::shared_ptr<QAction> m_action_belief_map;
    std::shared_ptr<QAction> m_action_train_hog;
    std::shared_ptr<QAction> m_action_prepare_cascade;
    std::shared_ptr<QAction> m_action_walnut_detection;
    std::shared_ptr<QAction> m_action_exit;
    std::shared_ptr<QAction> m_action_about;
    std::shared_ptr<QTimer> m_status_timer;
    std::shared_ptr<QLabel> m_status_label;
    std::shared_ptr<QTimer> m_video_timer;
};

#endif // MAINWINDOW_H
