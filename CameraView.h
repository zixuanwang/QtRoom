#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include "Annotate.h"
#include "Camera.h"
#include "CascadeDetector.h"
#include "ColorDescriptor.h"
#include "HoGDescriptor.h"
#include "MotionDescriptor.h"
#include <QMouseEvent>
#include <QtGui>
#include <QWidget>
#include "Utility.h"
#include "WalnutDetector.h"

#define THUMBNAIL_WIDTH 320
#define THUMBNAIL_HEIGHT 240
#define FULL_WIDTH 640
#define FULL_HEIGHT 480

#define ANNOTATE_PATH "/home/zxwang/Dropbox/data/iroom/annotate"

// this class creates the ui for the camera.
class CameraView : public QWidget
{
    Q_OBJECT
public:
    enum Type{CAMERA, VIDEO, ANNOTATE};
    enum MODE{NORMAL, CASCADE, BACKGROUND, EDGE, OPTICAL_FLOW, WALNUT};
    CameraView(const std::string& ip_address, const std::string& username, const std::string& password);
    CameraView(const std::string& video_path, Type type);
    void init();
    void set_mode(MODE mode);
    void start_recording(const std::string& video_directory);
    void stop_recording();
    cv::Mat process_image(cv::Mat& image); // the major function to analyse the image.
    virtual ~CameraView();
protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
private:
    std::shared_ptr<Camera> m_camera;
    cv::VideoCapture m_video_capture;
    cv::Mat m_image_buffer;
    QPoint m_point_start;
    QPoint m_point_topleft;
    int m_rect_length;
    Type m_type;
    MODE m_mode;
    Annotate m_annotate;
    int m_pick_rect_index; // the index of the picked rect.
    std::shared_ptr<PeopleDetector> m_people_detector;
    ColorDescriptor m_color_descriptor;
    HoGDescriptor m_hog_descriptor;
    MotionDescriptor m_motion_descriptor;
signals:

public slots:
    void image_changed();
    void read_image();
};

#endif // CAMERAVIEW_H
