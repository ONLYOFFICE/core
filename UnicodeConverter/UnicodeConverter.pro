QT       -= core gui

VERSION = 1.0.0.6
TARGET = UnicodeConverter
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin
CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include(../Common/base.pri)

DEFINES += UNICODECONVERTER_USE_DYNAMIC_LIBRARY

core_linux {    
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
    QMAKE_LFLAGS += -Wl,--disable-new-dtags
}

core_ios {
    CONFIG += bundle_dylibs
    OBJECTIVE_SOURCES += UnicodeConverter_internal_ios.mm

    # SASLprepToUtf8 TODO: remove this dependence
    CONFIG += core_disable_icu
    include(../Common/3dParty/icu/icu.pri)

    LIBS += -framework Foundation
}

!core_disable_icu {
    include(../Common/3dParty/icu/icu.pri)

SOURCES += \
    UnicodeConverter.cpp

HEADERS +=\
    UnicodeConverter.h
}
