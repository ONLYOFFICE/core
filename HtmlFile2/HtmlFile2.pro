QT -= core
QT -= gui

VERSION = 0.0.0.1
TARGET = HtmlFile2
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin
CONFIG += core_static_link_libstd

DEFINES += HTMLFILE2_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/../../core
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/html/gumbo.pri)

ADD_DEPENDENCY(kernel, UnicodeConverter, graphics)

SOURCES += htmlfile2.cpp

HEADERS += htmlfile2.h
