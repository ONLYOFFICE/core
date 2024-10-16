TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

TARGET = [TEST_NAME]
DESTDIR = $$PWD/build

BUILDER_DIR = "[BUILDER_DIR]"
INCLUDEPATH += $$BUILDER_DIR/include
LIBS += -L$$BUILDER_DIR -ldoctrenderer

SOURCES += ../../../cpp/[TEST_NAME]/main.cpp
