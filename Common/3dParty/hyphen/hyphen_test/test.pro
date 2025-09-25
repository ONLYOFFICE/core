QT       -= core
QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CORE_ROOT_DIR = $$PWD/../../../../../core
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
ADD_DEPENDENCY(UnicodeConverter, kernel, graphics)

SOURCES += main.cpp

DESTDIR = $$PWD/build
