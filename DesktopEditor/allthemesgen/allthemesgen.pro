TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

DESTDIR = $$PWD/../../build/bin/$$CORE_BUILDS_PLATFORM_PREFIX

TARGET = allthemesgen

DEFINES += KERNEL_USE_DYNAMIC_LIBRARY
DEFINES += GRAPHICS_USE_DYNAMIC_LIBRARY
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lgraphics -lkernel -lUnicodeConverter

build_xp {
    LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH/xp -ldoctrenderer
} else {
    LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -ldoctrenderer
}

core_windows {
    DEFINES -= UNICODE
    DEFINES -= _UNICODE

    LIBS += -lAdvapi32
    LIBS += -lShell32
    LIBS += -lGdi32
    LIBS += -lUser32
}

core_linux {
    LIBS += -lz -pthread

    QMAKE_LFLAGS += -Wl,--rpath=./
}

SOURCES += main.cpp
