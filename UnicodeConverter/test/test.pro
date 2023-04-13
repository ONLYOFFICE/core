QT       -= core
QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

DEFINES += UNICODECONVERTER_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD
include(../../Common/base.pri)

ADD_DEPENDENCY(kernel, UnicodeConverter)

SOURCES += main.cpp