QT       -= core
QT       -= gui

DESTDIR = $$PWD

TARGET = test
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(kernel)

SOURCES += main.cpp \
    cepubformat.cpp

HEADERS += \
    cepubformat.h
