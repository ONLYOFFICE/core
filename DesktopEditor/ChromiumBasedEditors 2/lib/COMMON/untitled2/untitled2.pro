#-------------------------------------------------
#
# Project created by QtCreator 2015-08-28T18:01:18
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lX11

TARGET = untitled2
TEMPLATE = app
CONFIG += app_bundle

SOURCES += main.cpp\
        mainwindow.cpp \
    XKeyboard.cpp

HEADERS  += mainwindow.h \
    X11Exception.h \
    XKeyboard.h

FORMS    += mainwindow.ui
