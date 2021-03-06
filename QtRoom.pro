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
    CameraView.cpp \
    Annotate.cpp \
    MotionDescriptor.cpp \
    HoGDescriptor.cpp \
    FeatureDescriptor.cpp \
    ColorDescriptor.cpp \
    Classifier.cpp \
    Serializer.cpp \
    SVMClassifier.cpp \
    Trainer.cpp \
    WalnutDetector.cpp \
    CascadeDetector.cpp \
    GeometryConstraint.cpp \
    PeoplePlot.cpp \
    GlobalConfig.cpp \
    ParticleFilter.cpp \
    FusionLayer.cpp \
    Geometry.cpp \
    Graph.cpp \
    Fusion.cpp \
    Layout.cpp

HEADERS  += mainwindow.h \
    Camera.h \
    Utility.h \
    CameraView.h \
    Annotate.h \
    MotionDescriptor.h \
    HoGDescriptor.h \
    FeatureDescriptor.h \
    ColorDescriptor.h \
    Classifier.h \
    Serializer.h \
    SVMClassifier.h \
    Trainer.h \
    WalnutDetector.h \
    CascadeDetector.h \
    GeometryConstraint.h \
    PeoplePlot.h \
    GlobalConfig.h \
    ParticleFilter.h \
    FusionLayer.h \
    Geometry.h \
    Graph.h \
    Fusion.h \
    Layout.h

FORMS    += mainwindow.ui

# change the following

QMAKE_CXXFLAGS += -Wall -std=c++11
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lqwt -lcurl -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_calib3d -lopencv_video -lopencv_objdetect -lopencv_ml -lboost_system -lboost_filesystem

RESOURCES += Res.qrc
