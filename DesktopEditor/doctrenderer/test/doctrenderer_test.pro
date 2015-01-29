#-------------------------------------------------
#
# Project created by QtCreator 2015-01-19T14:14:33
#
#-------------------------------------------------

QT       -= core

QT       -= gui

TARGET = doctrenderer_test
CONFIG   += console
CONFIG   -= app_bundle

DEFINES += _UNICODE

TEMPLATE = app

CONFIG(debug, debug|release) {
    LIBS += -L../../Debug/debug -ldoctrenderer
    message(debug)
} else {
    LIBS += -L../../Release/release -ldoctrenderer
    message(release)
}

INCLUDEPATH += C:/Qt/Qt5.4.0/5.4/msvc2013_64_opengl/include

SOURCES += main.cpp
