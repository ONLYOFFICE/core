QT       -= core
QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CORE_ROOT_DIR = $$PWD/../../../../../core
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)

INCLUDEPATH += $$PWD_ROOT_DIR/../hyphen

DEFINES += PRO_DIR=\\\"$$PWD/\\\"

HEADERS += $$PWD_ROOT_DIR/../hyphen/hyphen.h
HEADERS += $$PWD_ROOT_DIR/../hyphen/hnjalloc.h

#SOURCES += $$PWD_ROOT_DIR/../hyphen/hyphen.c
SOURCES += $$PWD_ROOT_DIR/../hyphen/hnjalloc.c

SOURCES += \
    ../js/src/ExportedFunctions.cpp \
	../js/src/HyphenApplication.cpp

SOURCES += main.cpp

DESTDIR = $$PWD/build
