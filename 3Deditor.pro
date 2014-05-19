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
    AxisWay.cpp \
    setting.cpp \
    server.cpp

INCLUDEPATH += $$PWD/../../../usr/local/include
DEPENDPATH += $$PWD/../../../usr/local/include

INCLUDEPATH += $$PWD/../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../usr/include
HEADERS  += \
    scene3D.h \
    AxisWay.h \
    PathCalculator.h \
    setting.h \
    server.h

FORMS    +=
