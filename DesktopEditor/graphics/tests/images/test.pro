CONFIG += c++11

QMAKE_CXXFLAGS += /permissive-

QT += core gui widgets

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

ADD_DEPENDENCY(kernel, graphics, UnicodeConverter)

GRAPHICS_AGG_PATH = $$PWD/../../../agg-2.4

INCLUDEPATH += \
    $$GRAPHICS_AGG_PATH/include

SOURCES += main.cpp \
	mainwindow.cpp

DESTDIR = $$PWD_ROOT_DIR/build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX

FORMS += \
	mainwindow.ui

HEADERS += \
	mainwindow.h
