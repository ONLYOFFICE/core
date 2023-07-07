QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtvlc
TEMPLATE = app

VLC_DIR = $$PWD/..

# cannot include base.pri cause of build errors with vs2019
# include($$VLC_DIR/../../base.pri)
CORE_BUILDS_PLATFORM_PREFIX = win_64
include($$VLC_DIR/libvlc.pri)

HEADERS += player.h

SOURCES += main.cpp \
           player.cpp
