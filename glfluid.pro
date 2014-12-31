#-------------------------------------------------
#
# Project created by QtCreator 2014-12-24T21:15:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = glfluid
TEMPLATE = app


SOURCES += main.cpp\
    myopenglwindow.cpp \
    math/tmat3.cpp \
    math/tmat4.cpp \
    math/tplane.cpp \
    math/tquat.cpp \
    math/tvec3.cpp \
    math/tvec4.cpp \
    fluid.cpp \
    camera.cpp

HEADERS  += \
    myopenglwindow.h \
    array2d.h \
    math/tmat3.h \
    math/tmat4.h \
    math/tplane.h \
    math/tquat.h \
    math/tvec2.h \
    math/tvec3.h \
    math/tvec4.h \
    fluid.h \
    camera.h

DISTFILES += \
    simple.vert \
    simple.frag
