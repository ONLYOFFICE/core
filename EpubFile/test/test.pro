QT       -= core
QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

CONFIG -= debug_and_release debug_and_release_target

DEFINES += EPUBFORMAT_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)


CONFIG(debug, debug|release) {
    DESTDIR = $$PWD_ROOT_DIR/Debug/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX
} else {
    DESTDIR = $$PWD_ROOT_DIR/Release/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX
}

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH  -lkernel -lEpubFile

TEMPLATE = app


SOURCES += main.cpp
