CONFIG += ixwebsocket
core_mac:CONFIG += socketrocket
core_ios:CONFIG += socketrocket

HEADERS += \
	$$PWD/include/websocket.h \
	$$PWD/src/websocketbase.h

SOURCES += $$PWD/src/managerwebsocket.cpp

ixwebsocket {

	DEFINES += USE_IXWEBSOCKET
	include($$PWD/../../3dParty/ixwebsocket/ixwebsocket.pri)

	HEADERS += $$PWD/src/ixwebsocket/ixwebsocket_internal.h
	SOURCES += $$PWD/src/ixwebsocket/ixwebsocket_internal.cpp

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
