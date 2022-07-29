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
    src/logic/BaseItem.h \
    src/logic/ElementContText.h \
    src/logic/ElementOldShape.h \
    src/logic/ElementShape.h \
    src/logic/ElementTextLine.h \
    src/logic/ImageManager.h \
    src/resources/ColorTable.h \
    src/resources/Constants.h \
    src/resources/LinesTable.h \
    src/resources/SingletonTemplate.h \
    src/resources/SortElements.h \
    src/resources/VectorGraphics.h \
    src/resources/resources.h \
    \
    src/logic/Page.h \
    src/logic/Document.h \
    src/logic/ElementImage.h \
    src/logic/ElementParagraph.h \
    src/logic/FontManager.h \
    src/logic/FontManagerBase.h \
    \
    DocxRenderer.h \
    src/resources/utils.h

SOURCES += \
    src/logic/BaseItem.cpp \
    src/logic/Document.cpp \
    src/logic/ElementContText.cpp \
    src/logic/ElementImage.cpp \
    src/logic/ElementOldShape.cpp \
    src/logic/ElementParagraph.cpp \
    src/logic/ElementShape.cpp \
    src/logic/ElementTextLine.cpp \
    src/logic/FontManager.cpp \
    src/logic/FontManagerBase.cpp \
    src/logic/ImageManager.cpp \
    src/logic/Page.cpp \
    src/resources/VectorGraphics.cpp \
    src/resources/resources.cpp \
    \
    DocxRenderer.cpp
