CONFIG += c++11
win32 {
    QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings
} else {
    QMAKE_CXXFLAGS += -Wall -Wno-ignored-qualifiers
}

SOURCES_UTILS = $$PWD/src

#################### WINDOWS #####################
win32 {
DEFINES += WIN32
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

build_zlib_as_sources {
    DEFINES += BUILD_ZLIB_AS_SOURCES
}

INCLUDEPATH += \
    $$SOURCES_UTILS/zlib-1.2.3/contrib/minizip \
    $$SOURCES_UTILS/zlib-1.2.3

SOURCES +=  \
    $$SOURCES_UTILS/OfficeUtils.cpp \
    $$SOURCES_UTILS/ZipUtilsCP.cpp \
    $$SOURCES_UTILS/zlib-1.2.3/contrib/minizip/ioapi.c \
    $$SOURCES_UTILS/zlib-1.2.3/contrib/minizip/miniunz.c \
    $$SOURCES_UTILS/zlib-1.2.3/contrib/minizip/minizip.c \
    $$SOURCES_UTILS/zlib-1.2.3/contrib/minizip/mztools.c \
    $$SOURCES_UTILS/zlib-1.2.3/contrib/minizip/unzip.c \
    $$SOURCES_UTILS/zlib-1.2.3/contrib/minizip/zip.c

build_all_zlib {
SOURCES += \
    $$SOURCES_UTILS/zlib-1.2.3/adler32.c \
    $$SOURCES_UTILS/zlib-1.2.3/compress.c \
    $$SOURCES_UTILS/zlib-1.2.3/crc32.c \
    $$SOURCES_UTILS/zlib-1.2.3/deflate.c \
    $$SOURCES_UTILS/zlib-1.2.3/example.c \
    $$SOURCES_UTILS/zlib-1.2.3/gzio.c \
    $$SOURCES_UTILS/zlib-1.2.3/infback.c \
    $$SOURCES_UTILS/zlib-1.2.3/inffast.c \
    $$SOURCES_UTILS/zlib-1.2.3/inflate.c \
    $$SOURCES_UTILS/zlib-1.2.3/inftrees.c \
    $$SOURCES_UTILS/zlib-1.2.3/minigzip.c \
    $$SOURCES_UTILS/zlib-1.2.3/trees.c \
    $$SOURCES_UTILS/zlib-1.2.3/uncompr.c \
    $$SOURCES_UTILS/zlib-1.2.3/zutil.c
}	
	
HEADERS +=  \
    $$SOURCES_UTILS/OfficeUtilsCommon.h \
    $$SOURCES_UTILS/OfficeUtils.h \
    $$SOURCES_UTILS/ASCOfficeCriticalSection.h \
    $$SOURCES_UTILS/CSLocker.h \
    $$SOURCES_UTILS/ZipUtilsCP.h \
    $$SOURCES_UTILS/zlib-1.2.3/contrib/minizip/crypt.h \
    $$SOURCES_UTILS/zlib-1.2.3/contrib/minizip/ioapi.h \
    $$SOURCES_UTILS/zlib-1.2.3/contrib/minizip/mztools.h \
    $$SOURCES_UTILS/zlib-1.2.3/contrib/minizip/unzip.h \
    $$SOURCES_UTILS/zlib-1.2.3/contrib/minizip/zip.h
