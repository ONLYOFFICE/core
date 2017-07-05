#-------------------------------------------------
#
# Project created by QtCreator 2017-06-05T12:59:43
#
#-------------------------------------------------

QT       += core gui

QT += widgets

TARGET = OpenSSL_gui_test
TEMPLATE = app

DEFINES -= \
    UNICODE \
    _UNICODE

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

core_linux {
    QMAKE_LFLAGS += -Wl,--rpath=./
    QMAKE_LFLAGS += -static-libstdc++ -static-libgcc
}

DEFINES += XMLSEC_OPENSSL_110
#DEFINES += "OPENSSL_API_COMPAT=\"0x10100000\""

INCLUDEPATH += $$CORE_ROOT_DIR/Common/3dParty/openssl/openssl/include

LIBS += -L$$CORE_ROOT_DIR/Common/3dParty/openssl/openssl -lssl
LIBS += -L$$CORE_ROOT_DIR/Common/3dParty/openssl/openssl -lcrypto

LIBS += -ldl

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

HEADERS += opensslwrapper.h
SOURCES += opensslwrapper.cpp
