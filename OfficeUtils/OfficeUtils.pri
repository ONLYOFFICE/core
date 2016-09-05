core_linux {
    QMAKE_CXXFLAGS += -Wall -Wno-ignored-qualifiers
}
core_mac {
    QMAKE_CXXFLAGS += -Wall -Wno-ignored-qualifiers
    DEFINES += unix
}

build_zlib_as_sources {
    DEFINES += BUILD_ZLIB_AS_SOURCES
}

INCLUDEPATH += \
    $$PWD/src/zlib-1.2.3/contrib/minizip \
    $$PWD/src/zlib-1.2.3

SOURCES +=  \
    $$PWD/src/OfficeUtils.cpp \
    $$PWD/src/ZipUtilsCP.cpp \
    $$PWD/src/zlib-1.2.3/contrib/minizip/ioapi.c \
    $$PWD/src/zlib-1.2.3/contrib/minizip/miniunz.c \
    $$PWD/src/zlib-1.2.3/contrib/minizip/minizip.c \
    $$PWD/src/zlib-1.2.3/contrib/minizip/mztools.c \
    $$PWD/src/zlib-1.2.3/contrib/minizip/unzip.c \
    $$PWD/src/zlib-1.2.3/contrib/minizip/zip.c

build_all_zlib {
SOURCES += \
    $$PWD/src/zlib-1.2.3/adler32.c \
    $$PWD/src/zlib-1.2.3/compress.c \
    $$PWD/src/zlib-1.2.3/crc32.c \
    $$PWD/src/zlib-1.2.3/deflate.c \
    $$PWD/src/zlib-1.2.3/example.c \
    $$PWD/src/zlib-1.2.3/gzio.c \
    $$PWD/src/zlib-1.2.3/infback.c \
    $$PWD/src/zlib-1.2.3/inffast.c \
    $$PWD/src/zlib-1.2.3/inflate.c \
    $$PWD/src/zlib-1.2.3/inftrees.c \
    $$PWD/src/zlib-1.2.3/minigzip.c \
    $$PWD/src/zlib-1.2.3/trees.c \
    $$PWD/src/zlib-1.2.3/uncompr.c \
    $$PWD/src/zlib-1.2.3/zutil.c
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
