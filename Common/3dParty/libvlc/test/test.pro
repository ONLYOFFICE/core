QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtvlc
TEMPLATE = app

VLC_DIR = $$PWD/..

include($$VLC_DIR/../../base.pri)
# CORE_BUILDS_PLATFORM_PREFIX = win_32
include($$VLC_DIR/libvlc.pri)

HEADERS += player.h

SOURCES += main.cpp \
           player.cpp
