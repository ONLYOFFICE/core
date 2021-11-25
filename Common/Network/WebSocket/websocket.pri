core_mac:CONFIG += apple_platform
core_ios:CONFIG += apple_platform

apple_platform {
	CONFIG += socketrocket
} else {
	CONFIG += ixwebsocket
}

HEADERS += \
	$$PWD/include/websocket.h \
	$$PWD/src/websocketbase.h

SOURCES += $$PWD/src/managerwebsocket.cpp

ixwebsocket {

    DEFINES += USE_IXWEBSOCKET
    include($$PWD/../../3dParty/ixwebsocket/ixwebsocket.pri)

    HEADERS += $$PWD/src/ixwebsocket/ixwebsocket_internal.h
    SOURCES += $$PWD/src/ixwebsocket/ixwebsocket_internal.cpp

    CONFIG += use_zlib

}

use_zlib {
    ZLIB_SOURCES = $$PWD/../../../OfficeUtils/src/zlib-1.2.11
    INCLUDEPATH += \
        $$ZLIB_SOURCES \
        $$ZLIB_SOURCES/..
    SOURCES += \
        $$ZLIB_SOURCES/adler32.c \
        $$ZLIB_SOURCES/compress.c \
        $$ZLIB_SOURCES/crc32.c \
        $$ZLIB_SOURCES/deflate.c \
        $$ZLIB_SOURCES/gzclose.c \
        $$ZLIB_SOURCES/gzlib.c \
        $$ZLIB_SOURCES/gzread.c \
        $$ZLIB_SOURCES/gzwrite.c \
        $$ZLIB_SOURCES/infback.c \
        $$ZLIB_SOURCES/inffast.c \
        $$ZLIB_SOURCES/inflate.c \
        $$ZLIB_SOURCES/inftrees.c \
        $$ZLIB_SOURCES/trees.c \
        $$ZLIB_SOURCES/uncompr.c \
        $$ZLIB_SOURCES/zutil.c \
        $$ZLIB_SOURCES/../zlib_addon.c
}

socketrocket {

    DEFINES += USE_SOCKETROCKET
    include($$PWD/../../3dParty/socketrocket/socketrocket.pri)

    HEADERS += \
        $$PWD/src/socketrocket/socketRocket_objc.h \
        $$PWD/src/socketrocket/socketRocket_internal.h \

    OBJECTIVE_SOURCES += \
        $$PWD/src/socketrocket/socketRocket_objc.mm \
        $$PWD/src/socketrocket/socketRocket_internal.mm

}
