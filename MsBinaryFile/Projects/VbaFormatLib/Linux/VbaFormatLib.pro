#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       -= core gui

TARGET = VbaFormatLib
TEMPLATE = lib
CONFIG += staticlib

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD

CONFIG += core_x2t
include(../../../../Common/base.pri)

DEFINES += UNICODE \
           _UNICODE \
            DONT_WRITE_EMBEDDED_FONTS

#BOOST
include($$PWD/../../../../Common/3dParty/boost/boost.pri)

SOURCES +=  \
	../../../Common/Vba/Records.cpp \
	../../../Common/Vba/StreamObjects.cpp \
	../../../Common/Vba/VbaBinary.cpp \
	../../../Common/Vba/VbaReader.cpp

HEADERS +=  \
	../../../Common/Vba/Records.h \
	../../../Common/Vba/StreamObjects.h \
	../../../Common/Vba/VbaBinary.h \
	../../../Common/Vba/VbaReader.h \
	../../../Common/Vba/VbaRecordType.h
 
