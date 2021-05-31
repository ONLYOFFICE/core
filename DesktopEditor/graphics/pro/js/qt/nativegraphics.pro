QT -= core
QT -= gui

TARGET = wasmgraphics
TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

DEFINES += TEST_AS_EXECUTABLE

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(graphics)

SOURCES += ../wasm/src/graphics.cpp
