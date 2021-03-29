QT -= core
QT -= gui

VERSION = 0.0.0.1
TARGET = zlib
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin
CONFIG += core_static_link_libstd

DEFINES += ZLIB_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(OfficeUtils)

SOURCES += ../wasm/src/base.cpp

HEADERS += ../wasm/src/base.h