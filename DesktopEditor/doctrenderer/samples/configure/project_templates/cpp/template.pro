TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

TARGET = [TEST_NAME]
DESTDIR = $$PWD/build

BUILDER_DIR = "[BUILDER_DIR]"
ROOT_DIR = "[ROOT_DIR]"
INCLUDEPATH += $$BUILDER_DIR/include
INCLUDEPATH += $$ROOT_DIR
LIBS += -L$$BUILDER_DIR -ldoctrenderer

SOURCES += ../../../cpp/[TEST_NAME]/main.cpp
