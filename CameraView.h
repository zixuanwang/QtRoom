#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include "Camera.h"
#include <QtGui>
#include <QWidget>
#include "Utility.h"

// this class creates the ui for the camera.
class CameraView : public QWidget
{
    Q_OBJECT
public:
    CameraView();
    virtual ~CameraView();
protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent *);
private:
    std::shared_ptr<Camera> m_camera;
    cv::Mat m_image_buffer;
    QPoint m_point_start;
    QPoint m_point_current;
signals:

public slots:
    void on_image_changed();
};

#endif // CAMERAVIEW_H
