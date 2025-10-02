CONFIG -= qt
QT -= core gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(UnicodeConverter, kernel, kernel_network, graphics, PdfFile)

core_linux:include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
core_windows:LIBS += -lgdi32 -ladvapi32 -luser32 -lshell32
core_linux:LIBS += -lz

SOURCES += main.cpp

DESTDIR = $$PWD_ROOT_DIR/build
