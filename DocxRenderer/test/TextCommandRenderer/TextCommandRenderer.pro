QT       -= core gui

VERSION = 1.0.0
TARGET = TextCommandRenderer
TEMPLATE = lib

CONFIG += c++11
CONFIG += shared
CONFIG += plugin

CORE_ROOT_DIR = $$PWD/../../../
PWD_ROOT_DIR = $$PWD
include(../../../Common/base.pri)

DEFINES += TEXTCOMMANDRENDERER_USE_DYNAMIC_LIBRARY

ADD_DEPENDENCY(UnicodeConverter, kernel, graphics)

core_windows {
LIBS += \
    -lgdi32 \
	-ladvapi32 \
	-luser32 \
	-lshell32
}

HEADERS += \
    TextCommandRenderer.h \
    Utils.h

SOURCES += \
    TextCommandRenderer.cpp
