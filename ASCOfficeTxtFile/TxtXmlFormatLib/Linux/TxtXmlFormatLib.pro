#-------------------------------------------------
#
# Project created by QtCreator 2015-04-29T13:41:59
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
    ../Source/TxtXmlEvent.h \
    ../Source/TxtXmlFile.h \
    ../Source/Common/Encoding.h \
    ../Source/Common/precompiled_utility.h \
    ../Source/Common/StlUtils.h \
    ../Source/Common/ToString.h \
    ../Source/Common/Utility.h \
    ../Source/TxtFormat/File.h \
    ../Source/TxtFormat/TxtFile.h \
    ../Source/TxtFormat/TxtFormat.h \
    ../Source/ConvertDocx2Txt.h \
    ../Source/ConvertTxt2Docx.h

SOURCES += \
    ../Source/Common/Encoding.cpp \
    ../Source/Common/ToString.cpp \
    ../Source/TxtFormat/File.cpp \
    ../Source/TxtFormat/TxtFile.cpp \
    ../Source/TxtXmlFile.cpp \
    ../Source/ConvertDocx2Txt.cpp \
    ../Source/ConvertTxt2Docx.cpp
