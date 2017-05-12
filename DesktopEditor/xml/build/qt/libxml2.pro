#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T14:24:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = libxml
TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS += -Wall -g

DEFINES += \
    LIBXML_READER_ENABLED \
    LIBXML_PUSH_ENABLED \
    LIBXML_HTML_ENABLED \
    LIBXML_XPATH_ENABLED \
    LIBXML_OUTPUT_ENABLED \
    LIBXML_C14N_ENABLED \
    LIBXML_SAX1_ENABLED \
    LIBXML_TREE_ENABLED \
    LIBXML_XPTR_ENABLED \
    IN_LIBXML \
    LIBXML_STATIC

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD
include(../../../../Common/base.pri)
include($$PWD/libxml2.pri)
