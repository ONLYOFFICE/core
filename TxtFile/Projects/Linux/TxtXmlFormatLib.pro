#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       -= core gui

TARGET = TxtXmlFormatLib
TEMPLATE = lib
CONFIG += staticlib

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD

CONFIG += core_x2t
include(../../../Common/base.pri)

DEFINES += UNICODE \
           _UNICODE \
            DONT_WRITE_EMBEDDED_FONTS

#BOOST
include($$PWD/../../../Common/3dParty/boost/boost.pri)

HEADERS += \
	../../Source/TxtFormat/File.h \
	../../Source/TxtFormat/TxtFile.h \
	../../Source/TxtXmlFile.h \
	../../Source/ConvertDocx2Txt.h \
	../../Source/ConvertTxt2Docx.h

SOURCES += \
	../../Source/TxtFormat/File.cpp \
	../../Source/TxtFormat/TxtFile.cpp \
	../../Source/TxtXmlFile.cpp \
	../../Source/ConvertDocx2Txt.cpp \
	../../Source/ConvertTxt2Docx.cpp
