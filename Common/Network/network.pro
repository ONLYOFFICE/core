TARGET = network
TEMPLATE = lib

# Исключаем зависимость от QT
CONFIG -= qt
QT -= core gui
# Вместо shared используем plugin, чтобы к целевому файлу не добавлялась информция о версии(вместо libnetwork.so.1.0.0 получаем libnetwork.so)
CONFIG += plugin warn_on

CORE_ROOT_DIR = $$PWD/../../../core
PWD_ROOT_DIR = $$PWD
include(../base.pri)

# WEBSOCKET
include(./WebSocket/websocket.pri)

ixwebsocket {

    DEFINES += USE_IXWEBSOCKET    
	
        include(../3dParty/ixwebsocket/ixwebsocket.pri)
	
    HEADERS += \
        ./WebSocket/include/websocket.h \
        ./WebSocket/src/base/WebWorkerBase.h \
        ./WebSocket/src/ixwebsocket/ixwebsocket_internal.h \

    SOURCES += \
        ./WebSocket/src/ixwebsocket/ixwebsocket_internal.cpp \
        ./WebSocket/src/manager/managerWebSocket.cpp \
        
}

socketrocket {

    DEFINES += USE_SOCKETROCKET

    include(../3dParty/socketrocket/socketrocket.pri)

    HEADERS += \
        ./WebSocket/include/websocket.h \
        ./WebSocket/src/base/WebWorkerBase.h \
        ./WebSocket/src/socketrocket/socketRocket_objc.h \
        ./WebSocket/src/socketrocket/socketRocket_internal.h \

    OBJECTIVE_SOURCES += \
        ./WebSocket/src/socketrocket/socketRocket_objc.mm \
        ./WebSocket/src/socketrocket/socketRocket_internal.mm \
        ./WebSocket/src/manager/managerWebSocket.cpp \

}

# FileTransporter
HEADERS += \
    ./FileTransporter/include/FileTransporter.h \
    ./FileTransporter/src/FileTransporter_private.h \
    ./FileTransporter/src/transport_external.h

SOURCES += ./FileTransporter/src/FileTransporter.cpp

core_windows {
    SOURCES += \
        ./FileTransporter/src/FileTransporter_win.cpp

    LIBS += -lAdvapi32
    LIBS += -lurlmon
    LIBS += -lRpcrt4
    LIBS += -lShell32
}
core_linux {
    CONFIG += use_external_transport

    use_external_transport {
        DEFINES += USE_EXTERNAL_TRANSPORT
    } else {
        include(../3dParty/curl/curl.pri)
    }

    SOURCES += \
        ./FileTransporter/src/FileTransporter_curl.cpp
}
core_mac {
    DEFINES += USE_EXTERNAL_TRANSPORT
    OBJECTIVE_SOURCES += \
        ./FileTransporter/src/FileTransporter_mac.mm

    LIBS += -framework AppKit
}

core_ios {
    OBJECTIVE_SOURCES += \
        ./FileTransporter/src/FileTransporter_mac.mm \
        ../../DesktopEditor/common/File_ios.mm

    LIBS += -framework Foundation

}

core_android {
    DEFINES += USE_FILE32API
    SOURCES += ./FileTransporter/src/FileTransporter_curl.cpp

    USE_EXTERNAL_TRANSPORT {
        DEFINES += USE_EXTERNAL_TRANSPORT
    } else {
        include(../3dParty/curl/curl.pri)
    }

    DEFINES += NOT_USE_PTHREAD_CANCEL
}

use_mac_extensions {
    INCLUDEPATH += $$CORE_ROOT_DIR/DesktopEditor/common/Mac
    OBJECTIVE_SOURCES += \
        $$CORE_ROOT_DIR/DesktopEditor/common/Mac/NSString+StringUtils.mm \
        $$CORE_ROOT_DIR/DesktopEditor/common/Mac/NSFileManager+Utils.m
}

