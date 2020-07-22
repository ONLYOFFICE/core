QT       -= core
QT       -= gui

TARGET = test
TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle

DEFINES += EPUBFORMAT_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

core_windows:INCLUDEPATH += $$PWD/../gumbo-parser/visualc/include
core_windows:INCLUDEPATH += $$PWD/../katana-parser/visualc/include

DESTDIR = $$PWD_ROOT_DIR/build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX

ADD_DEPENDENCY(gumbo, katana)

SOURCES += main.cpp \
    CGetData.cpp

HEADERS += \
    CGetData.h
