TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

DESTDIR = $$CORE_BUILDS_BINARY_PATH

TARGET = pluginsmanager

DEFINES += KERNEL_USE_DYNAMIC_LIBRARY

ADD_DEPENDENCY(kernel, kernel_network, UnicodeConverter)

core_windows {
    DEFINES += \
    WIN32

    DEFINES -= UNICODE
    DEFINES -= _DEBUG

    message(windows)

    LIBS += -lAdvapi32
    LIBS += -lShell32
    LIBS += -lGdi32
    LIBS += -lUser32
}

SOURCES += main.cpp
HEADERS += help.h
