#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include "Annotate.h"
#include "Camera.h"
#include "GlobalConfig.h"
#include "PeoplePlot.h"
#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <QtGui>
#include <QToolTip>
#include <QVBoxLayout>
#include <QWidget>
#include "Utility.h"
#include "WalnutDetector.h"

// this class creates the ui for the camera.
class CameraView : public QWidget
{
    Q_OBJECT
public:
    enum Type{CAMERA, VIDEO, ANNOTATE};
    enum MODE{NORMAL, CASCADE, BACKGROUND, OPTICAL_FLOW, MOTION_MAP, WALNUT};
    CameraView(const std::string& ip_address, const std::string& username, const std::string& password);
    CameraView(const std::string& video_path, Type type);
    void init();
    void set_mode(MODE mode);
    void start_recording(const std::string& video_directory);
    void stop_recording();
    cv::Mat process_image(cv::Mat& image); // the major function to analyse the image.
    virtual ~CameraView();
protected:
    bool event(QEvent *);
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
    void contextMenuEvent(QContextMenuEvent*);
private:
    std::shared_ptr<Camera> m_camera;
    cv::VideoCapture m_video_capture;
    std::ifstream m_timestamp_stream;
    std::string m_timestamp; // the timestamp of the current frame.
    cv::Mat m_image_buffer;
    QPoint m_point_start;
    QPoint m_point_topleft;
    int m_rect_length;
    Type m_type;
    MODE m_mode;
    std::string m_id; // ip address of the camera.
    Annotate m_annotate;
    int m_pick_rect_index; // the index of the picked rect.
    std::shared_ptr<PeopleDetector> m_people_detector;
    ColorDescriptor m_color_descriptor;
    HoGDescriptor m_hog_descriptor;
    MotionDescriptor m_motion_descriptor;
    // show plot
    std::shared_ptr<QAction> m_action_plot;
    std::shared_ptr<QWidget> m_plot_widget;
    std::shared_ptr<PeoplePlot> m_plot;
    std::shared_ptr<QVBoxLayout> m_plot_layout;


signals:

public slots:
    void image_changed();
    void read_image();
    void show_plot();
};

#endif // CAMERAVIEW_H
