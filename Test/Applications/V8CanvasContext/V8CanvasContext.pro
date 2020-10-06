QT -= core
QT -= gui

VERSION = 0.0.0.1
TARGET = V8CanvasContext
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin
CONFIG += core_static_link_libstd

DEFINES += V8CANVASCONTEXT_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/../../../
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/v8/v8.pri)

ADD_DEPENDENCY(kernel, UnicodeConverter, graphics)

SOURCES += V8CanvasContext.cpp

HEADERS += V8CanvasContext.h
