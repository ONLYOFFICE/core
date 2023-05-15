core_mac:CONFIG += apple_platform
core_ios:CONFIG += apple_platform

# since 7.3: ONLY socketio
#apple_platform:CONFIG += socketrocket
#!apple_platform:CONFIG += ixwebsocket

CONFIG += libsocketio

libsocketio:CONFIG += use_openssl
ixwebsocket:CONFIG += use_openssl

use_openssl {
    include($$PWD/../../3dParty/ixwebsocket/openssl.pri)
}

libsocketio {
    SOCKET_IO_LIB=$$PWD/../../3dParty/socketio/socket.io-client-cpp

    INCLUDEPATH += \
        $$SOCKET_IO_LIB/lib/websocketpp \
        $$SOCKET_IO_LIB/lib/rapidjson/include \
        $$SOCKET_IO_LIB/lib/asio/asio/include

    HEADERS += \
        $$SOCKET_IO_LIB/src/internal/sio_client_impl.h \
        $$SOCKET_IO_LIB/src/internal/sio_packet.h \
        $$SOCKET_IO_LIB/src/sio_message.h \
        $$SOCKET_IO_LIB/src/sio_socket.h \
        $$SOCKET_IO_LIB/src/sio_client.h

    SOURCES += \
        $$SOCKET_IO_LIB/src/internal/sio_client_impl.cpp \
        $$SOCKET_IO_LIB/src/internal/sio_packet.cpp \
        $$SOCKET_IO_LIB/src/sio_socket.cpp \
        $$SOCKET_IO_LIB/src/sio_client.cpp

    HEADERS += \
        $$SOCKET_IO_LIB/src_no_tls/internal/sio_client_impl.h \
        $$SOCKET_IO_LIB/src_no_tls/internal/sio_packet.h \
        $$SOCKET_IO_LIB/src_no_tls/sio_message.h \
        $$SOCKET_IO_LIB/src_no_tls/sio_socket.h \
        $$SOCKET_IO_LIB/src_no_tls/sio_client.h

    SOURCES += \
        $$SOCKET_IO_LIB/src_no_tls/internal/sio_client_impl.cpp \
        $$SOCKET_IO_LIB/src_no_tls/internal/sio_packet.cpp \
        $$SOCKET_IO_LIB/src_no_tls/sio_socket.cpp \
        $$SOCKET_IO_LIB/src_no_tls/sio_client.cpp

    DEFINES += \
        BOOST_DATE_TIME_NO_LIB \
        BOOST_REGEX_NO_LIB \
        ASIO_STANDALONE \
        \
        _WEBSOCKETPP_CPP11_STL_ \
        _WEBSOCKETPP_CPP11_FUNCTIONAL_ \
        _WEBSOCKETPP_CPP11_TYPE_TRAITS_ \
        _WEBSOCKETPP_CPP11_CHRONO_ \
        \
        "SIO_TLS=1" \
        "SIO_TLS_NO=0" \
        "PING_TIMEOUT_INTERVAL=20000"

    include($$PWD/../../3dParty/boost/boost.pri)

    DEFINES += USE_IOWEBSOCKET

    HEADERS += \
        $$PWD/src/socketio/socketio_internal.h \
        $$PWD/src/socketio/socketio_internal_private.h \
        $$PWD/src/socketio/socketio_internal_private_no_tls.h

    SOURCES += $$PWD/src/socketio/socketio_internal.cpp
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
