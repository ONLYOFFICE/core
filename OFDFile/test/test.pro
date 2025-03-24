QT -= core
QT -= gui

TARGET = test
TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle
s
CORE_ROOT_DIR = $$PWD/../../
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

ADD_DEPENDENCY(kernel, graphics, UnicodeConverter, OFDFile)

win32 {
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}
linux-g++ | linux-g++-64 | linux-g++-32 {
    LIBS += -lz
}

SOURCES += main.cpp

DESTDIR = $$PWD_ROOT_DIR/build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX
