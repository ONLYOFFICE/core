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

core_linux {
    CONFIG += signature_openssl
}
core_mac {
    CONFIG += signature_openssl
}

HEADERS += \
    include/XmlCertificate.h \
    include/OOXMLSigner.h \
    include/OOXMLVerifier.h

HEADERS += \
    src/XmlCanonicalizator.h \
    src/XmlRels.h \    
    src/XmlTransform.h

SOURCES += \
    src/XmlTransform.cpp \
    src/XmlCertificate.cpp \
    src/OOXMLSigner.cpp \
    src/OOXMLVerifier.cpp

core_windows {

HEADERS += \
    src/XmlSigner_mscrypto.h

    LIBS += -lcrypt32
    LIBS += -lcryptui
    LIBS += -lAdvapi32

}

signature_openssl {

HEADERS += \
    src/XmlSigner_openssl.h

SOURCES += \
    src/XmlSigner_openssl.cpp

}

signature_openssl {

include(../../../Common/3dParty/openssl/openssl.pri)

}
