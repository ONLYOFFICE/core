#-------------------------------------------------
#
# Project created by QtCreator 2015-01-19T10:22:14
#
#-------------------------------------------------

QT       -= core gui

TARGET = doctrenderer
TEMPLATE = lib
CONFIG += staticlib

linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
    LINUX \
    _LINUX \
    _LINUX_QT \
    UNICODE \
    _UNICODE
}

INCLUDEPATH += \
    ../../../../../../../v8_trunk \
    ../../../../../../../v8_trunk/include \
    ../../Common/DocxFormat/Source/XML/libxml2/XML/include \
    ../agg-2.4/include \
    ../freetype-2.5.2/include

SOURCES += doctrenderer.cpp

HEADERS += doctrenderer.h \
    memorystream.h \
    nativecontrol.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
