TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

DESTDIR = $$PWD/../../build/bin/AllFontsGen

TARGET = $$CORE_BUILDS_PLATFORM_PREFIX

DEFINES += KERNEL_USE_DYNAMIC_LIBRARY
DEFINES += GRAPHICS_USE_DYNAMIC_LIBRARY
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lgraphics -lkernel -lUnicodeConverter

core_windows {
    DEFINES += \
    JAS_WIN_MSVC_BUILD \
    WIN32

    DEFINES -= UNICODE
    DEFINES -= _DEBUG

    message(windows)

    LIBS += -lAdvapi32
    LIBS += -lShell32
    LIBS += -lGdi32
    LIBS += -lUser32
}

core_linux {
    LIBS += -lz -pthread
}

SOURCES += main.cpp
