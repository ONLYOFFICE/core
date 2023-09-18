QT       -= core

QT       -= gui

TARGET = metafile
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

CONFIG += c++14 cmdline

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(kernel, graphics, UnicodeConverter)

core_linux:include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
core_windows:LIBS += -lgdi32 -ladvapi32 -luser32 -lshell32
core_linux:LIBS += -lz

SOURCES += \
        main.cpp

DESTDIR = $$PWD_ROOT_DIR/build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX

