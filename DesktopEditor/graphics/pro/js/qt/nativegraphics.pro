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
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

ADD_DEPENDENCY(kernel, graphics, UnicodeConverter, Raster)

INCLUDEPATH += \
    $$CORE_ROOT_DIR/DesktopEditor/agg-2.4/include \
    $$CORE_ROOT_DIR/DesktopEditor/cximage/jasper/include \
    $$CORE_ROOT_DIR/DesktopEditor/cximage/jpeg \
    $$CORE_ROOT_DIR/DesktopEditor/cximage/png \
    $$CORE_ROOT_DIR/DesktopEditor/freetype-2.10.4/include \
    $$CORE_ROOT_DIR/DesktopEditor/freetype-2.10.4/include/freetype

win32 {
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

HEADERS += ../wasm/src/graphics.h

SOURCES += ../wasm/src/graphics.cpp
