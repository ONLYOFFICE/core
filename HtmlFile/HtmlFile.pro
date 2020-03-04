QT       -= core gui

VERSION = 1.0.0.3
TARGET = HtmlFile
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += core_static_link_libstd

DEFINES += HTMLFILE_USE_DYNAMIC_LIBRARY
DEFINES += UNICODECONVERTER_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include(../Common/base.pri)

ADD_DEPENDENCY(kernel, UnicodeConverter)

core_linux {
    DEFINES += asc_static_link_libstd
}

core_windows {
    DEFINES += NOMINMAX
}

SOURCES += HtmlFile.cpp
HEADERS += HtmlFile.h
