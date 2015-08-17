#-------------------------------------------------
#
# Project created by QtCreator 2014-12-16T18:25:28
#
#-------------------------------------------------

QT       -= core

QT       -= gui

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers

DEFINES += UNICODE \
    _UNICODE \
     _LINUX_QT \
     LINUX 

TARGET = ASCXlsConverter

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    /usr/include/libxml2

SOURCES += \
    XlsFileConverter.cpp


linux-g++{

    message(64 bit)

    unix:!macx: LIBS += -L$$PWD/../../SDK/lib/linux_64/ -lXlsFormatLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../SDK/lib/linux_64/libXlsFormatLib.a

    unix:!macx: LIBS += -L$$PWD/../../SDK/lib/linux_64/ -lgraphics
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../SDK/lib/linux_64/libgraphics.a
  }

else{

    message(32 bit)

    unix:!macx: LIBS += -L$$PWD/../../SDK/lib/linux_32/ -lXlsFormatLib
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../SDK/lib/linux_32/libXlsFormatLib.a

   unix:!macx: LIBS += -L$$PWD/../../SDK/lib/linux_32/ -lgraphics
    unix:!macx: PRE_TARGETDEPS += $$PWD/../../SDK/lib/linux_32/libgraphics.a
}


