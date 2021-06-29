QT       -= core gui

VERSION = 1.0.0.4
TARGET = DocxRenderer
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include(../Common/base.pri)

DEFINES += DOCXRENDERER_USE_DYNAMIC_LIBRARY

ADD_DEPENDENCY(UnicodeConverter, kernel, graphics)

core_windows {

LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

HEADERS += \
    src/resources/resources.h

HEADERS += \
    src/logic/Common.h \
    src/logic/Document.h \
    src/logic/ElementImage.h \
    src/logic/ElementParagraph.h \
    src/logic/ElementShape.h \
    src/logic/FontManager.h \
    src/logic/FontManagerBase.h \
    src/logic/Page.h

HEADERS += \
    DocxRenderer.h

SOURCES += \
    DocxRenderer.cpp
