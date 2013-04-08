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
    CameraView.cpp \
    Annotate.cpp \
    MotionDescriptor.cpp \
    HoGDescriptor.cpp \
    FeatureDescriptor.cpp \
    ColorDescriptor.cpp

HEADERS  += mainwindow.h \
    Camera.h \
    Utility.h \
    PeopleDetector.h \
    HaarDetector.h \
    CameraView.h \
    Annotate.h \
    MotionDescriptor.h \
    HoGDescriptor.h \
    FeatureDescriptor.h \
    ColorDescriptor.h

FORMS    += mainwindow.ui

# change the following

QMAKE_CXXFLAGS += -Wall -std=c++11
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lcurl -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_video -lopencv_objdetect -lboost_system -lboost_filesystem

RESOURCES += \
    Res.qrc
