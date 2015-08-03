#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T14:24:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = ASCOfficeUtilsLib
TEMPLATE = lib
CONFIG += staticlib
win32 {
    QMAKE_CXXFLAGS += -std=c++11
} else {
    QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers
}
############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../../SDK/lib

# WINDOWS
win32:contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64
}
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32
}
}

linux-g++:contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_64
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_32
}
############### destination path ###############

DEFINES += UNICODE \
           _UNICODE

#################### WINDOWS #####################
win32 {
INCLUDEPATH += \
    ../../ZLIB/zlib-1.2.3
}
#################### WINDOWS #####################

#################### LINUX ########################
linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
        LINUX \
        _LINUX \
        _LINUX_QT
}
#################### LINUX ########################

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

#SOURCES += \
#    ../../ZLIB/zlib-1.2.3/adler32.c \
#    ../../ZLIB/zlib-1.2.3/compress.c \
#    ../../ZLIB/zlib-1.2.3/crc32.c \
#    ../../ZLIB/zlib-1.2.3/deflate.c \
#    ../../ZLIB/zlib-1.2.3/example.c \
#    ../../ZLIB/zlib-1.2.3/gzio.c \
#    ../../ZLIB/zlib-1.2.3/infback.c \
#    ../../ZLIB/zlib-1.2.3/inffast.c \
#    ../../ZLIB/zlib-1.2.3/inflate.c \
#    ../../ZLIB/zlib-1.2.3/inftrees.c \
#    ../../ZLIB/zlib-1.2.3/minigzip.c \
#    ../../ZLIB/zlib-1.2.3/trees.c \
#    ../../ZLIB/zlib-1.2.3/uncompr.c \
#    ../../ZLIB/zlib-1.2.3/zutil.c
	
	
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
