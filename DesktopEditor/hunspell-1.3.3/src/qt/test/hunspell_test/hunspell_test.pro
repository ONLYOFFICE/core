#-------------------------------------------------
#
# Project created by QtCreator 2015-03-26T09:54:00
#
#-------------------------------------------------

QT       += core

QT       += gui

TARGET = hunspell_test
CONFIG   += console
CONFIG   -= app_bundle

DEFINES += _UNICODE
DEFINES += WIN32
DEFINES += _WIN32
DEFINES += HUNSPELL_STATIC

TEMPLATE = app


CONFIG(debug, debug|release) {
    LIBS += -L../../../Debug/debug -lhunspell
    message(debug)
} else {
    LIBS += -L../../../Release/release -lhunspell
    message(release)
}


SOURCES += main.cpp
