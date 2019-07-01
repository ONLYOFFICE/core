QT       -= core
QT       -= gui

TARGET = docbuilder
CONFIG   += console
CONFIG   -= app_bundle

PRODUCT_VERSION=$$(PRODUCT_VERSION)
BUILD_NUMBER=$$(BUILD_NUMBER)

isEmpty(PRODUCT_VERSION) {
    BINARYVERSION = 0.0.0.0
}
else {
    BINARYVERSION = $$(PRODUCT_VERSION).$$(BUILD_NUMBER)
}

DEFINES += INTVER=$$BINARYVERSION

TEMPLATE = app

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/../../../../core
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

core_linux {
    QMAKE_LFLAGS += -Wl,--rpath=./:./system
}

core_win_64 {
    QMAKE_LFLAGS_CONSOLE  = /SUBSYSTEM:CONSOLE,5.02
}
core_win_32 {
    QMAKE_LFLAGS_CONSOLE  = /SUBSYSTEM:CONSOLE,5.01
}

core_windows {
    RC_FILE = version.rc
}

############### destination path ###############
DESTDIR = $$CORE_ROOT_DIR/build/bin/$$CORE_BUILDS_PLATFORM_PREFIX
################################################

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lkernel -lgraphics -lUnicodeConverter

build_xp {
    LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH/xp -ldoctrenderer
} else {
    LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -ldoctrenderer
}

core_linux {
    LIBS += -ldl
}

SOURCES += main.cpp
