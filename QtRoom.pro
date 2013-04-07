#-------------------------------------------------
#
# Project created by QtCreator 2013-04-04T11:25:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtRoom
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Camera.cpp \
    Utility.cpp \
    PeopleDetector.cpp \
    HaarDetector.cpp \
    Annotate.cpp \
    CameraWindow.cpp \
    CameraView.cpp

HEADERS  += mainwindow.h \
    Camera.h \
    Utility.h \
    PeopleDetector.h \
    HaarDetector.h \
    Annotate.h \
    CameraWindow.h \
    CameraView.h

FORMS    += mainwindow.ui \
    CameraWindow.ui

# change the following

# add console for debugging
CONFIG += console

QMAKE_CXXFLAGS += -Wall -std=c++11
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lcurl -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_video -lopencv_objdetect

RESOURCES += \
    Res.qrc
