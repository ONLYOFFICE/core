QT -= core
QT -= gui

TARGET = raster
TEMPLATE = lib

CONFIG += raster_dynamic_library

graphics_dynamic_library {
    CONFIG += shared
    CONFIG += plugin
} else {
    CONFIG += static
}

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

LIB_GRAPHICS_PRI_PATH = $$PWD/../../../..

SOURCES += \
    $$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximajpg.cpp
