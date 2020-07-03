QT -= core
QT -= gui

TARGET = test
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

DEFINES += FB2FILE_USE_DYNAMIC_LIBRARY

SOURCES += main.cpp

CORE_ROOT_DIR = $$PWD/../../../../core
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lkernel -lFb2File
