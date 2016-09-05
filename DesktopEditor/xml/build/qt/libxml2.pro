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

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD
include(../../../../Common/base.pri)
include($$PWD/libxml2.pri)
