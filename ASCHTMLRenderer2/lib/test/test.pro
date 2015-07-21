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

INCLUDEPATH += \
    ../../../DesktopEditor/freetype-2.5.2/include \

TEMPLATE = app

LIBS += -L../../../../ASCOfficeUtils/ASCOfficeUtilsLib/Win/x64/Debug -lASCOfficeUtilsLib
LIBS += -L../../../../DesktopEditor/Qt_build/graphics/Debug/debug -lgraphics
LIBS += -L../../../../ASCHTMLRenderer2/lib/Debug/debug -laschtmlrenderer
LIBS += -L../../../../SDK/lib/win_64/DEBUG -llibxml
LIBS += -L../../../../PdfReader/Debug/debug -lPdfReader
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32

SOURCES += main.cpp
