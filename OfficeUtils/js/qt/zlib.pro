QT -= core
QT -= gui

TARGET = zlib
TEMPLATE = lib

CONFIG += staticlib
CONFIG += build_all_zlib

DEFINES += ZLIB_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

INCLUDEPATH += \
    $$PWD/../../src/zlib-1.2.11/contrib/minizip \
    $$PWD/../../src/zlib-1.2.11

ADD_DEPENDENCY(kernel)

SOURCES += ../wasm/src/base.cpp
HEADERS += ../wasm/src/base.h
