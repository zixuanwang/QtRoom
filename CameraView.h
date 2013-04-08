#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include "Annotate.h"
#include "Camera.h"
#include <QMouseEvent>
#include <QtGui>
#include <QWidget>
#include "Utility.h"

// this class creates the ui for the camera.
class CameraView : public QWidget
{
    Q_OBJECT
public:
    enum Type{CAMERA, VIDEO};
    CameraView();
    virtual ~CameraView();
protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);
private:
    std::shared_ptr<Camera> m_camera;
    cv::VideoCapture m_video_capture;
    cv::Mat m_image_buffer;
    QPoint m_point_start;
    QPoint m_point_topleft;
    int m_rect_length;
    Type m_type;
    Annotate m_annotate;
signals:

public slots:
    void image_changed();
};

#endif // CAMERAVIEW_H
