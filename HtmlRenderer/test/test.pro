#-------------------------------------------------
#
# Project created by QtCreator 2015-07-21T18:28:42
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

DEFINES += PDFREADER_USE_DYNAMIC_LIBRARY
DEFINES += XPS_USE_DYNAMIC_LIBRARY
DEFINES += DJVU_USE_DYNAMIC_LIBRARY
DEFINES += HTMLRENDERER_USE_DYNAMIC_LIBRARY

INCLUDEPATH += \
    ../../DesktopEditor/freetype-2.5.2/include

TEMPLATE = app

LIBS += -L../../../SDK/lib/win_64/DEBUG -lgraphics
LIBS += -L../../../SDK/lib/win_64/DEBUG -lHtmlRenderer
LIBS += -L../../../SDK/lib/win_64/DEBUG -lPdfReader
LIBS += -L../../../SDK/lib/win_64/DEBUG -lDjVuFile
LIBS += -L../../../SDK/lib/win_64/DEBUG -lXpsFile
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32

SOURCES += main.cpp
