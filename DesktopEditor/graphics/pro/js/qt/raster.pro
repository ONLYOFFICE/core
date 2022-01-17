QT -= core
QT -= gui

VERSION = 0.0.0.1
TARGET = Raster
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin
CONFIG += core_static_link_libstd

DEFINES += RASTER_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(graphics)

SOURCES += ../wasm/src/raster.cpp

HEADERS += ../wasm/src/raster.h
