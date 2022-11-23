QT       -= core gui

VERSION = 1.0.0.3
TARGET = XpsFile
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include(../Common/base.pri)

DEFINES += XPS_USE_DYNAMIC_LIBRARY
ADD_DEPENDENCY(graphics, kernel, UnicodeConverter, PdfFile)

core_windows {
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

INCLUDEPATH += \
    $$CORE_ROOT_DIR/DesktopEditor/freetype-2.10.4/include \
    $$CORE_ROOT_DIR/DesktopEditor/freetype-2.10.4/include/freetype

HEADERS += \
    XpsFile.h \
    XpsLib/ContextState.h \
    XpsLib/Document.h \
    XpsLib/FontList.h \
    XpsLib/XpsPage.h \
    XpsLib/StaticResources.h \
    XpsLib/Utils.h \
    XpsLib/WString.h

SOURCES += \
    XpsFile.cpp \
    XpsLib/ContextState.cpp \
    XpsLib/Document.cpp \
    XpsLib/XpsPage.cpp \
    XpsLib/StaticResources.cpp \
    XpsLib/Utils.cpp \
    XpsLib/WString.cpp

#CONFIG += build_viewer_module
build_viewer_module {
    DEFINES += BUILDING_WASM_MODULE

    HEADERS += $$CORE_ROOT_DIR/HtmlRenderer/include/HTMLRendererText.h
    SOURCES += $$CORE_ROOT_DIR/HtmlRenderer/src/HTMLRendererText.cpp
}
