QT -= core
QT -= gui

TARGET = test
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

DEFINES += OFD_USE_DYNAMIC_LIBRARY

SOURCES += main.cpp

CORE_ROOT_DIR = $$PWD/../../
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(kernel, UnicodeConverter, OFDFile)

DESTDIR = $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX
