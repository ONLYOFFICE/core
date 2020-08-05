QT -= core
QT -= gui

VERSION = 1.0.0.3
TARGET = gumbo
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += core_static_link_libstd

DEFINES += HTMLTOXHTML_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(kernel, UnicodeConverter)

core_windows:INCLUDEPATH += $$PWD/gumbo-parser/visualc/include

HEADERS += $$files($$PWD/gumbo-parser/src/*.h, true) \
    htmltoxhtml.h
SOURCES += $$files($$PWD/gumbo-parser/src/*.c, true)
