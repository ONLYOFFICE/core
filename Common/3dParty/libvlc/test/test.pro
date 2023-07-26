QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtvlc
TEMPLATE = app

PWD_ROOT_DIR = $$PWD
CORE_ROOT_DIR = $$PWD/../../../..

VLC_DIR = $$PWD/..

#CONFIG += apple_silicon
include($$VLC_DIR/../../base.pri)
include($$VLC_DIR/libvlc.pri)

HEADERS += player.h

SOURCES += main.cpp \
           player.cpp
