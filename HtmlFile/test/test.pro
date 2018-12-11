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

CONFIG -= debug_and_release debug_and_release_target

DEFINES += HTMLFILE_USE_DYNAMIC_LIBRARY
DEFINES += UNICODECONVERTER_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/Debug
} else {
    DESTDIR = $$PWD/Release
}

CONFIG += c++11
TEMPLATE = app

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lUnicodeConverter -lkernel -lgraphics -lHtmlFile

linux-g++ | linux-g++-64 | linux-g++-32 {
    QMAKE_LFLAGS += -Wl,--rpath=./
    message(linux)
}

win32 {
LIBS += -ladvapi32 \
        -luser32 \
        -lshell32
}

SOURCES += main.cpp
