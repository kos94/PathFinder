#-------------------------------------------------
#
# Project created by QtCreator 2013-12-18T12:53:15
#
#-------------------------------------------------

QT       += core gui opengl network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3Deditor
TEMPLATE = app

SOURCES += main.cpp\
    scene3D.cpp \
    pathCalculator.cpp \
    setting.cpp \
    server.cpp \
    vector3d.cpp \
    proectionview.cpp \
    proection.cpp

INCLUDEPATH += $$PWD/../../../usr/local/include
DEPENDPATH += $$PWD/../../../usr/local/include

INCLUDEPATH += $$PWD/../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../usr/include
HEADERS  += \
    scene3D.h \
    PathCalculator.h \
    setting.h \
    server.h \
    vector3d.h \
    proection.h \
    proectionview.h

QMAKE_CXXFLAGS += -std=c++11

unix:!mac {
    message("*Settings for linux")
    INCLUDEPATH +=/usr/local/include/opencv

    LIBS += -L/usr/local/lib/ \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_objdetect \
        -lopencv_imgproc
}

OTHER_FILES += \
    triangle.xml \
    circle.xml \
    8.xml
