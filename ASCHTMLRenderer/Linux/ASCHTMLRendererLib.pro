#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T14:24:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = ASCHTMLRendererLib
TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers

DEFINES +=  UNICODE \
            _UNICODE \
            _LINUX_QT \
            NODOCX \
            DESKTOP_EDITOR_GRAPHICS

INCLUDEPATH += \
    ../../DesktopEditor/freetype-2.5.2/include \
    ../../DesktopEditor/agg-2.4/include
	
SOURCES += ../ASCSVGWriter.cpp \
    ../CASCSVGRenderer.cpp \
    ../CMetafile.cpp \
    ../CASCImage.cpp

HEADERS += ../ASCSVGWriter.h \
    ../CASCImage.h \
    ../CASCSVGRenderer.h \
    ../CMetafile.h \
    ../Writer/Common.h \
    ../Writer/Const.h \
    ../IBaseMatrixUpdater.h \
    ../Writer/TextItem.h \
    ../Writer/StringWriter.h \
    ../Writer/SVGWriter.h \
    ../Writer/VectorGraphicsWriter2.h \
    ../Writer/VMLWriter.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

