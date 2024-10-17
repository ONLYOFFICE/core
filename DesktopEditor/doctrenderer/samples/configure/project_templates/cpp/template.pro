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

linux: QMAKE_LFLAGS += -Wl,--unresolved-symbols=ignore-in-shared-libs

SOURCES += ../../../cpp/[TEST_NAME]/main.cpp
