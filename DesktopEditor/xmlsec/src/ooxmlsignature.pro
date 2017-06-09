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

CONFIG += core_static_link_xml_full
include($$CORE_ROOT_DIR/DesktopEditor/xml/build/qt/libxml2.pri)

DEFINES -= UNICODE

HEADERS += \
    include/XmlCertificate.h \
    include/OOXMLSigner.h \
    include/OOXMLVerifier.h

HEADERS += \
    src/XmlCanonicalizator.h \
    src/XmlRels.h \    
    src/XmlTransform.h \
    src/XmlSigner_mscrypto.h \
    src/XmlSigner_openssl.h

SOURCES += \
    src/XmlTransform.cpp \
    src/XmlCertificate.cpp \
    src/OOXMLSigner.cpp \
    src/OOXMLVerifier.cpp

core_windows {

    LIBS += -lcrypt32
    LIBS += -lcryptui
    LIBS += -lAdvapi32

}

core_linux {

DEFINES += XMLSEC_OPENSSL_110
INCLUDEPATH += $$CORE_ROOT_DIR/Common/3dParty/openssl/openssl/include

LIBS += -L$$CORE_ROOT_DIR/Common/3dParty/openssl/openssl -lssl
LIBS += -L$$CORE_ROOT_DIR/Common/3dParty/openssl/openssl -lcrypto

}
