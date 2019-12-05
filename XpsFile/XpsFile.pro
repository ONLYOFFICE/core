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
ADD_DEPENDENCY(graphics, kernel, UnicodeConverter, PdfWriter)

core_windows {
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

HEADERS += \
    XpsFile.h \
    XpsLib/ContextState.h \
    XpsLib/Document.h \
    XpsLib/FontList.h \
    XpsLib/Page.h \
    XpsLib/StaticResources.h \
    XpsLib/Utils.h \
    XpsLib/WString.h

SOURCES += \
    XpsFile.cpp \
    XpsLib/ContextState.cpp \
    XpsLib/Document.cpp \
    XpsLib/Page.cpp \
    XpsLib/StaticResources.cpp \
    XpsLib/Utils.cpp \
    XpsLib/WString.cpp

