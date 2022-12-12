QT       -= core
QT       -= gui

VERSION = 1.0.0.1
TARGET = ooxmlsignature
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += c++11

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(kernel)

DEFINES -= UNICODE
HEADERS += \
    include/Certificate.h \
	include/CertificateCommon.h \
    include/OOXMLSigner.h \
	include/OOXMLVerifier.h

HEADERS += \
    src/XmlCanonicalizator.h \
    src/XmlRels.h \    
    src/XmlTransform.h

SOURCES += \
    src/XmlTransform.cpp \
	src/CertificateCommon.cpp \
    src/OOXMLSigner.cpp \
	src/OOXMLVerifier.cpp

core_windows {
    DEFINES += SUPPORT_MS_CRYPTO
    HEADERS += src/Certificate_mscrypto.h

    LIBS += -lcrypt32
    LIBS += -lcryptui
    LIBS += -lAdvapi32
    LIBS += -lws2_32
    LIBS += -lUser32
}

DEFINES += SUPPORT_OPENSSL
HEADERS += src/Certificate_openssl.h

support_oform {
    DEFINES += SUPPORT_OFORM
	HEADERS += src/Certificate_oform.h
}

CONFIG += open_ssl_common
include(../../../Common/3dParty/openssl/openssl.pri)
build_xp:DESTDIR=$$DESTDIR/xp
