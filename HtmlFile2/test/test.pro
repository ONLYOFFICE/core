QT -= core
QT -= gui

TARGET = test
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

SOURCES += main.cpp

CORE_ROOT_DIR = $$PWD/../../../core
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lHtmlFile2

ADD_DEPENDENCY(kernel, UnicodeConverter)

DESTDIR = $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX
