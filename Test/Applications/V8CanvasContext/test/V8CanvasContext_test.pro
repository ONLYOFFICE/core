QT -= core
QT -= gui

TARGET = V8CanvasContext_Test
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

DEFINES += V8CanvasContext_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/../../../../
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lV8CanvasContext
ADD_DEPENDENCY(kernel, UnicodeConverter, graphics)

SOURCES += main.cpp

DESTDIR = $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX
