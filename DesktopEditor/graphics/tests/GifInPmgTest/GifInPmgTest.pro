#CONFIG += c++11 cmdline

#SOURCES += \
QT -= core

QT -= gui

TARGET = test
CONFIG += console
TEMPLATE = app

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

ADD_DEPENDENCY(kernel, graphics, UnicodeConverter)

GRAPHICS_AGG_PATH = $$PWD/../../../agg-2.4

INCLUDEPATH += \
    $$GRAPHICS_AGG_PATH/include \
    C:/Users/mrkir/core/DesktopEditor/cximage/zlib \
    C:/Users/mrkir/core/OfficeUtils/src

SOURCES += main.cpp

DESTDIR = $$PWD_ROOT_DIR/build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX

DISTFILES += \
    ../../../../build/lib/win_64/debug/kernel.dll \
    ../../../../build/lib/win_64/debug/kernel.lib \
    image1.png \
    test.txt

SUBDIRS += \
    ../../pro/graphics.pro

