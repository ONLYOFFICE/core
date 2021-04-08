QT -= core
QT -= gui

TARGET = zlib
TEMPLATE = lib

CONFIG += staticlib
CONFIG += build_all_zlib

DEFINES += ZLIB_USE_DYNAMIC_LIBRARY
DEFINES += BUILDING_WASM_MODULE

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

core_linux {
    QMAKE_CXXFLAGS += -Wall -Wno-ignored-qualifiers
}
core_mac {
    QMAKE_CXXFLAGS += -Wall -Wno-ignored-qualifiers
    DEFINES += unix
}

INCLUDEPATH += \
    $$PWD/../../src/zlib-1.2.11/contrib/minizip \
    $$PWD/../../src/zlib-1.2.11

SOURCES +=  \
    $$PWD/../../src/OfficeUtils.cpp \
    $$PWD/../../src/ZipUtilsCP.cpp \
    $$PWD/../../src/zlib-1.2.11/contrib/minizip/ioapi.c \
    $$PWD/../../src/zlib-1.2.11/contrib/minizip/miniunz.c \
    $$PWD/../../src/zlib-1.2.11/contrib/minizip/minizip.c \
    $$PWD/../../src/zlib-1.2.11/contrib/minizip/mztools.c \
    $$PWD/../../src/zlib-1.2.11/contrib/minizip/unzip.c \
    $$PWD/../../src/zlib-1.2.11/contrib/minizip/zip.c
core_windows {
SOURCES +=  \
    $$PWD/../../src/zlib-1.2.11/contrib/minizip/iowin32.c
}
build_all_zlib {
SOURCES += \
    $$PWD/../../src/zlib-1.2.11/adler32.c \
    $$PWD/../../src/zlib-1.2.11/compress.c \
    $$PWD/../../src/zlib-1.2.11/crc32.c \
    $$PWD/../../src/zlib-1.2.11/deflate.c \
    $$PWD/../../src/zlib-1.2.11/gzclose.c \
    $$PWD/../../src/zlib-1.2.11/gzlib.c \
    $$PWD/../../src/zlib-1.2.11/gzread.c \
    $$PWD/../../src/zlib-1.2.11/gzwrite.c \
    $$PWD/../../src/zlib-1.2.11/infback.c \
    $$PWD/../../src/zlib-1.2.11/inffast.c \
    $$PWD/../../src/zlib-1.2.11/inflate.c \
    $$PWD/../../src/zlib-1.2.11/inftrees.c \
    $$PWD/../../src/zlib-1.2.11/trees.c \
    $$PWD/../../src/zlib-1.2.11/uncompr.c \
    $$PWD/../../src/zlib-1.2.11/zutil.c
}

HEADERS +=  \
    $$PWD/../../src/OfficeUtilsCommon.h \
    $$PWD/../../src/OfficeUtils.h \
    $$PWD/../../src/ZipUtilsCP.h \
    $$PWD/../../src/zlib-1.2.11/contrib/minizip/crypt.h \
    $$PWD/../../src/zlib-1.2.11/contrib/minizip/ioapi.h \
    $$PWD/../../src/zlib-1.2.11/contrib/minizip/mztools.h \
    $$PWD/../../src/zlib-1.2.11/contrib/minizip/unzip.h \
    $$PWD/../../src/zlib-1.2.11/contrib/minizip/zip.h

core_ios {
OBJECTIVE_SOURCES += $$PWD/../../src/ZipUtilsCP_ios.mm
}

SOURCES += ../wasm/src/base.cpp \
    ../wasm/src/ioapibuf.c

HEADERS += ../wasm/src/base.h \
    ../wasm/src/ioapibuf.h
