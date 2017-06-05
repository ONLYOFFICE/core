QT       -= core gui

TARGET = test
TEMPLATE = app

CONFIG   += console
CONFIG   -= app_bundle

CORE_ROOT_DIR = $$PWD/../../../../
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

SOURCES += main.cpp

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -looxmlsignature


