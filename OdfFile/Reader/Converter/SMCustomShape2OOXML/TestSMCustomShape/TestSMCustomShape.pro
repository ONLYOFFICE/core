QT -= core gui

TARGET = test
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/OdfFile/Reader/Converter/SMCustomShape2OOXML/SMCustomShape2OOXML.pri)
include($$CORE_ROOT_DIR/Common/3dParty/googletest/googletest.pri)



SOURCES += main.cpp

DESTDIR = $$PWD/build
