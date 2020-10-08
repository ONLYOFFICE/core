QT -= core
QT -= gui

TARGET = V8CanvasContext_Test
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

DEFINES += V8CANVASCONTEXT_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/../../../
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/v8/v8.pri)

ADD_DEPENDENCY(kernel, UnicodeConverter, graphics)

HEADERS += V8CanvasContext.h \
    $$CORE_ROOT_DIR/DesktopEditor/doctrenderer/jsgraphics/graphics.h \
    $$CORE_ROOT_DIR/DesktopEditor/doctrenderer/jsgraphics/graphics_wrapper.h

SOURCES += main.cpp \
    V8CanvasContext.cpp \
    $$CORE_ROOT_DIR/DesktopEditor/doctrenderer/jsgraphics/graphics.cpp \
    $$CORE_ROOT_DIR/DesktopEditor/doctrenderer/jsgraphics/graphics_wrapper.cpp \

DESTDIR = $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX
