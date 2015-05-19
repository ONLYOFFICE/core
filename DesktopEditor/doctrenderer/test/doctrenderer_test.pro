#-------------------------------------------------
#
# Project created by QtCreator 2015-01-19T14:14:33
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = doctrenderer_test
CONFIG   += console
CONFIG   -= app_bundle

DEFINES += UNICODE
DEFINES += _UNICODE
DEFINES += _USE_LIBXML2_READER_
DEFINES += LIBXML_READER_ENABLED

mac {
    DEFINES += \
    LINUX \
    _LINUX \
    _LINUX_QT \
    _MAC \
    QT_MAC
}

win32 {
    DEFINES += \
    WIN32
}

linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
    LINUX \
    _LINUX \
    _LINUX_QT \
}

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
