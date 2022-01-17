HEADERS += \
    $$PWD/include/FileTransporter.h \
    $$PWD/src/FileTransporter_private.h \
    $$PWD/src/transport_external.h

SOURCES += $$PWD/src/FileTransporter.cpp

core_windows {
    SOURCES += $$PWD/src/FileTransporter_win.cpp

    LIBS += -lAdvapi32
    LIBS += -lurlmon
    LIBS += -lRpcrt4
    LIBS += -lShell32
}

core_linux:CONFIG += use_external_transport
core_mac:CONFIG += use_external_transport

use_external_transport:DEFINES += USE_EXTERNAL_TRANSPORT

core_linux {
    !use_external_transport:include($$PWD/../../3dParty/curl/curl.pri)
    SOURCES += $$PWD/src/FileTransporter_curl.cpp
}
core_mac {
    OBJECTIVE_SOURCES += $$PWD/src/FileTransporter_mac.mm
    LIBS += -framework AppKit
}

core_ios {
    OBJECTIVE_SOURCES += \
        $$PWD/src/FileTransporter_mac.mm

    LIBS += -framework Foundation
}

core_android {
    DEFINES += USE_FILE32API
    SOURCES += $$PWD/src/FileTransporter_curl.cpp

    !use_external_transport:include($$PWD/../../3dParty/curl/curl.pri)
}
