QT       -= core gui

TARGET = test
TEMPLATE = app

CONFIG   += console
CONFIG   -= app_bundle

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(kernel, ooxmlsignature)

SOURCES += main.cpp

DESTDIR = $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX
