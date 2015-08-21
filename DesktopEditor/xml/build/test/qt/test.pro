#-------------------------------------------------
#
# Project created by QtCreator 2015-07-13T14:59:36
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

#LIBS += -L../../../../../SDK/lib/win_64/DEBUG -llibxml
LIB_XML_PRI_PATH = ../../../../../DesktopEditor/xml
include(../../../../../DesktopEditor/xml/build/qt/libxml2.pri)

SOURCES += main.cpp
