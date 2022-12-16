QT -= core gui

TARGET = wasm
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

DEFINES += TEST_AS_EXECUTABLE

CONFIG += oform_only

oform_only {
    DEFINES += DISABLE_XMLSEC
}

CORE_ROOT_DIR = $$PWD/../../../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

DEFINES += KERNEL_NO_USE_DYNAMIC_LIBRARY
DEFINES += COMMON_OPENSSL_BUILDING_INTERNAL

SOURCES += \
    $$CORE_ROOT_DIR/DesktopEditor/common/File.cpp \
    $$CORE_ROOT_DIR/DesktopEditor/common/Base64.cpp

oform_only {
    HEADERS += \
        $$CORE_ROOT_DIR/DesktopEditor/xmlsec/src/include/Certificate.h \
        $$CORE_ROOT_DIR/DesktopEditor/xmlsec/src/include/CertificateCommon.h

    SOURCES += \
        $$CORE_ROOT_DIR/DesktopEditor/xmlsec/src/src/CertificateCommon.cpp

    DEFINES += SUPPORT_OFORM
    HEADERS += $$CORE_ROOT_DIR/DesktopEditor/xmlsec/src/src/Certificate_oform.h
}

# OPENSSL
CONFIG += open_ssl_common
include($$CORE_ROOT_DIR/Common/3dParty/openssl/openssl.pri)

core_windows {
    LIBS += -lcrypt32
    LIBS += -lcryptui
    LIBS += -lAdvapi32
    LIBS += -lws2_32
    LIBS += -lUser32
}

core_linux {
    LIBS += -ldl
}

# WASM EXPORT
SOURCES += main.cpp
