QT       -= core gui

TARGET = test
TEMPLATE = app

CONFIG   += console
CONFIG   -= app_bundle

DEFINES += UNICODE

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

#DEFINES += XMLSEC_CRYPTO_DYNAMIC_LOADING
DEFINES += XMLSEC_CRYPTO_MSCRYPTO

INCLUDEPATH += \
    $$CORE_ROOT_DIR/DesktopEditor/xml/libxml2/include \
    $$CORE_ROOT_DIR/DesktopEditor/xml/libxml2/include/libxml \
    \
    $$CORE_ROOT_DIR/DesktopEditor/xmlsec/xmlsec/include

DEFINES += \
    LIBXML_READER_ENABLED \
    LIBXML_PUSH_ENABLED \
    LIBXML_HTML_ENABLED \
    LIBXML_XPATH_ENABLED \
    LIBXML_OUTPUT_ENABLED \
    LIBXML_C14N_ENABLED \
    LIBXML_SAX1_ENABLED \
    LIBXML_TREE_ENABLED \
    LIBXML_XPTR_ENABLED

DEFINES += XMLSEC_NO_XSLT

DEFINES += XMLSEC_STATIC

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -llibxmlsec

LIBS += -lcrypt32
LIBS += -lcryptui
LIBS += -lAdvapi32

SOURCES += main.cpp
