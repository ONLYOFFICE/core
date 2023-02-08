#-------------------------------------------------
#
# Project created by QtCreator 2015-07-21T18:28:42
#
#-------------------------------------------------

QT       -= core

QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

CONFIG += c++11

DEFINES += PDFREADER_USE_DYNAMIC_LIBRARY
DEFINES += PDFWRITER_USE_DYNAMIC_LIBRARY
DEFINES += XPS_USE_DYNAMIC_LIBRARY
DEFINES += DJVU_USE_DYNAMIC_LIBRARY
DEFINES += HTMLRENDERER_USE_DYNAMIC_LIBRARY

DEFINES += KERNEL_USE_DYNAMIC_LIBRARY
DEFINES += GRAPHICS_USE_DYNAMIC_LIBRARY

TEMPLATE = app

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$PWD/../../Common/3dParty/icu/icu.pri)

ADD_DEPENDENCY(UnicodeConverter kernel graphics kernel_network HtmlRenderer PdfFile DjVuFile XpsFile)

win32 {
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

linux-g++ | linux-g++-64 | linux-g++-32 {
    LIBS += -lz
}

SOURCES += main.cpp

SOURCES += \
    ../src/ASCSVGWriter.cpp

HEADERS += ../include/ASCSVGWriter.h
