QT -= core
QT -= gui

TARGET = test
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

DEFINES += HWPFILE_USE_DYNAMIC_LIBRARY

SOURCES += main.cpp

CORE_ROOT_DIR = $$PWD/../../
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

ADD_DEPENDENCY(kernel, UnicodeConverter, HWPFile)

DESTDIR = $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX
