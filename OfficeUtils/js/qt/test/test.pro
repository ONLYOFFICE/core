QT -= core
QT -= gui

TARGET = test
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

DEFINES += ZLIB_USE_DYNAMIC_LIBRARY

SOURCES += main.cpp

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

INCLUDEPATH += \
    $$PWD/../../../src/zlib-1.2.11/contrib/minizip \
    $$PWD/../../../src/zlib-1.2.11

ADD_DEPENDENCY(kernel, OfficeUtils, zlib)
