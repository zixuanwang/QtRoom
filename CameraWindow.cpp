#include "CameraWindow.h"
#include "ui_CameraWindow.h"

CameraWindow::CameraWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CameraWindow)
{
    ui->setupUi(this);
}

CameraWindow::~CameraWindow()
{
    delete ui;
}
