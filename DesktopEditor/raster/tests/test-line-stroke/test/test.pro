QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(kernel, graphics, UnicodeConverter)

SOURCES += main.cpp

DESTDIR = $$PWD_ROOT_DIR/build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX
