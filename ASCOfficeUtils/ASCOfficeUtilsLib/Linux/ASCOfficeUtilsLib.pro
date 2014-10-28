#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T14:24:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = ASCOfficeUtilsLib
TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers -g
DEFINES += UNICODE \
    _UNICODE \
    LINUX \
    _LINUX_QT

INCLUDEPATH += \
    ../../ZLIB/zlib-1.2.3/contrib/minizip \
    ../../ZLIB/zlib-1.2.3

SOURCES +=  \
    ../OfficeUtils.cpp \
    ../../ZLIB/zlib-1.2.3/adler32.c \
    ../../ZLIB/zlib-1.2.3/compress.c \
    ../../ZLIB/zlib-1.2.3/crc32.c \
    ../../ZLIB/zlib-1.2.3/deflate.c \
    ../../ZLIB/zlib-1.2.3/example.c \
    ../../ZLIB/zlib-1.2.3/gzio.c \
    ../../ZLIB/zlib-1.2.3/infback.c \
    ../../ZLIB/zlib-1.2.3/inffast.c \
    ../../ZLIB/zlib-1.2.3/inflate.c \
    ../../ZLIB/zlib-1.2.3/inftrees.c \
    ../../ZLIB/zlib-1.2.3/minigzip.c \
    ../../ZLIB/zlib-1.2.3/trees.c \
    ../../ZLIB/zlib-1.2.3/uncompr.c \
    ../../ZLIB/zlib-1.2.3/zutil.c \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/zip.c \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/unzip.c \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/mztools.c \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/minizip.c \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/miniunz.c \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/ioapi.c \
    ../../ASCOfficeUtils/ZipUtilsCP.cpp
	
	
HEADERS +=  \
    ../OfficeUtilsCommon.h \
    ../OfficeUtils.h \
    ../../ZLIB/zlib-1.2.3/zutil.h \
    ../../ZLIB/zlib-1.2.3/zlib.h \
    ../../ZLIB/zlib-1.2.3/zconf.in.h \
    ../../ZLIB/zlib-1.2.3/zconf.h \
    ../../ZLIB/zlib-1.2.3/trees.h \
    ../../ZLIB/zlib-1.2.3/inftrees.h \
    ../../ZLIB/zlib-1.2.3/inflate.h \
    ../../ZLIB/zlib-1.2.3/inffixed.h \
    ../../ZLIB/zlib-1.2.3/inffast.h \
    ../../ZLIB/zlib-1.2.3/deflate.h \
    ../../ZLIB/zlib-1.2.3/crc32.h \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/crypt.h \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/ioapi.h \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/mztools.h \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/unzip.h \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/zip.h \
    ../../ASCOfficeUtils/ASCOfficeCriticalSection.h \
	../../ASCOfficeUtils/CSLocker.h \
    ../../ASCOfficeUtils/ZipUtilsCP.h
	

unix {
    target.path = /usr/lib
    INSTALLS += target
}
