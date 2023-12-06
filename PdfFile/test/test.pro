QT -= core
QT -= gui

TARGET = test
TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/googletest/googletest.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

ADD_DEPENDENCY(UnicodeConverter, kernel, graphics, PdfFile, ooxmlsignature)

SOURCES += test.cpp

DESTDIR = $$PWD/build
