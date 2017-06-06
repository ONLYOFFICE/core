QT       -= core gui

TARGET = test
TEMPLATE = app

CONFIG   += console
CONFIG   -= app_bundle

DEFINES -= \
    UNICODE \
    _UNICODE

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

DEFINES += XMLSEC_OPENSSL_110

INCLUDEPATH += $$PWD/../../openssl/include

LIBS += -L$$PWD/../../openssl -lssl -lcrypto
LIBS += -ldl

SOURCES += main.cpp
