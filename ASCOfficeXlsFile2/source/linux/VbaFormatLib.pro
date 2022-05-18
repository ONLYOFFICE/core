#-------------------------------------------------
#
# Project created by QtCreator 2014-12-16T18:28:23
#
#-------------------------------------------------

QT       -= core gui

TARGET = VbaFormatLib
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

SOURCES +=  \
    ../VbaFormat/Records.cpp \
    ../VbaFormat/StreamObjects.cpp \
    ../VbaFormat/VbaBinary.cpp \
    ../VbaFormat/VbaReader.cpp 


HEADERS +=  \
    ../VbaFormat/Records.h \
    ../VbaFormat/StreamObjects.h \
    ../VbaFormat/VbaBinary.h \
    ../VbaFormat/VbaReader.h \
    ../VbaFormat/VbaRecordType.h
 
