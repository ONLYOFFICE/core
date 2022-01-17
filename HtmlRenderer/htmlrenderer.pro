QT       -= core
QT       -= gui

VERSION = 1.0.0.3
TARGET = HtmlRenderer
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += core_static_link_libstd

DEFINES += HTMLRENDERER_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include(../Common/base.pri)

ADD_DEPENDENCY(graphics, kernel, UnicodeConverter)

core_windows {
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

SOURCES += \
    src/HTMLRenderer3.cpp

HEADERS += \
    src/CanvasWriter.h \
    src/Common.h \
    src/Document.h \
    src/FontManager.h \
    src/FontManagerBase.h \
    src/SVGWriter.h \
    src/SVGWriter2.h \
    src/Text.h \
    src/VectorGraphicsWriter.h \
    src/VectorGraphicsWriter2.h \
    src/VMLWriter.h \
    src/Writer.h \
    include/HTMLRenderer3.h \
    src/Common2.h \
    src/Meta.h

#HEADERS += $$PWD/include/HTMLRendererText.h
#SOURCES += $$PWD/src/HTMLRendererText.cpp
