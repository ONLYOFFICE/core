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
    src/logic/elements/BaseItem.h \
    src/logic/elements/ContText.h \
    src/logic/elements/Image.h \
    src/logic/elements/OldShape.h \
    src/logic/elements/Paragraph.h \
    src/logic/elements/Shape.h \
    src/logic/elements/TextLine.h \
    src/logic/managers/ImageManager.h \
    src/logic/managers/FontManager.h \
    src/logic/managers/FontManagerBase.h \
    src/logic/managers/StyleManager.h \
    src/logic/styles/BaseStyle.h \
    src/logic/styles/FontStyle.h \
    src/resources/ColorTable.h \
    src/resources/Constants.h \
    src/resources/ImageInfo.h \
    src/resources/LinesTable.h \
    src/resources/SingletonTemplate.h \
    src/resources/SortElements.h \
    src/resources/VectorGraphics.h \
    src/resources/resources.h \
    src/resources/utils.h \
    src/logic/Page.h \
    src/logic/Document.h \
    DocxRenderer.h

SOURCES += \
    src/logic/elements/BaseItem.cpp \
    src/logic/elements/ContText.cpp \
    src/logic/elements/Image.cpp \
    src/logic/elements/OldShape.cpp \
    src/logic/elements/Paragraph.cpp \
    src/logic/elements/Shape.cpp \
    src/logic/elements/TextLine.cpp \
    src/logic/managers/FontManager.cpp \
    src/logic/managers/FontManagerBase.cpp \
    src/logic/managers/ImageManager.cpp \
    src/logic/managers/StyleManager.cpp \
    src/logic/styles/FontStyle.cpp \
    src/logic/Page.cpp \
    src/logic/Document.cpp \
    src/resources/VectorGraphics.cpp \
    src/resources/resources.cpp \
    DocxRenderer.cpp

DISTFILES += \
    readme.md
