#-------------------------------------------------
#
# Project created by QtCreator 2015-07-21T18:28:42
#
#-------------------------------------------------

QT -= core gui

TARGET = dictionariestester
CONFIG += console
CONFIG -= app_bundle

DEFINES += KERNEL_USE_DYNAMIC_LIBRARY

TEMPLATE = app

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

ADD_DEPENDENCY(UnicodeConverter kernel hunspell)

core_windows:LIBS += -lgdi32 -ladvapi32 -luser32 -lshell32

SOURCES += main.cpp

DESTDIR = $$CORE_BUILDS_BINARY_PATH
