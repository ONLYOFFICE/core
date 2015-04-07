#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T14:24:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = ASCOfficeUtilsLib
TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers

DEFINES += UNICODE \
           _UNICODE \
            LINUX \
           _LINUX_QT

#INCLUDEPATH += \
#    ../../ZLIB/zlib-1.2.3/contrib/minizip \
#    ../../ZLIB/zlib-1.2.3

SOURCES +=  \
    ../OfficeUtils.cpp \
    ../../ASCOfficeUtils/ZipUtilsCP.cpp \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/ioapi.c \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/miniunz.c \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/minizip.c \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/mztools.c \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/unzip.c \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/zip.c
	
	
HEADERS +=  \
    ../OfficeUtilsCommon.h \
    ../OfficeUtils.h \
    ../../ASCOfficeUtils/ASCOfficeCriticalSection.h \
    ../../ASCOfficeUtils/CSLocker.h \
    ../../ASCOfficeUtils/ZipUtilsCP.h \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/crypt.h \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/ioapi.h \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/mztools.h \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/unzip.h \
    ../../ZLIB/zlib-1.2.3/contrib/minizip/zip.h
	

unix {
    target.path = /usr/lib
    INSTALLS += target
}
