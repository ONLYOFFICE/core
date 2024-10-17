QT       -= core gui

VERSION = 1.0.0.4
TARGET = DocxRenderer
TEMPLATE = lib

CONFIG += c++11
CONFIG += shared
CONFIG += plugin

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include(../Common/base.pri)

DEFINES += DOCXRENDERER_USE_DYNAMIC_LIBRARY

ADD_DEPENDENCY(UnicodeConverter, kernel, graphics)

# Flag for disable full document creation. Enabled in pdf editor
#CONFIG += disable_full_document_creation

core_windows {
LIBS += \
	-lgdi32 \
	-ladvapi32 \
	-luser32 \
	-lshell32
}

HEADERS += \
	src/logic/elements/BaseItem.h \
	src/logic/elements/ContText.h \
	src/logic/elements/DropCap.h \
	src/logic/elements/Paragraph.h \
	src/logic/elements/Shape.h \
	src/logic/elements/TextLine.h \
	src/logic/managers/ExternalImageStorage.h \
	src/logic/managers/FontStyleManager.h \
	src/logic/managers/ImageManager.h \
	src/logic/managers/FontManager.h \
	src/logic/managers/ParagraphStyleManager.h \
	src/logic/styles/FontStyle.h \
	src/logic/styles/ParagraphStyle.h \
	src/resources/ColorTable.h \
	src/resources/Constants.h \
	src/resources/ImageInfo.h \
	src/resources/LinesTable.h \
	src/resources/VectorGraphics.h \
	src/resources/resources.h \
	src/resources/utils.h \
	src/logic/Page.h \
	src/logic/Document.h \
	DocxRenderer.h

SOURCES += \
	src/logic/elements/BaseItem.cpp \
	src/logic/elements/ContText.cpp \
	src/logic/elements/DropCap.cpp \
	src/logic/elements/Paragraph.cpp \
	src/logic/elements/Shape.cpp \
	src/logic/elements/TextLine.cpp \
	src/logic/managers/FontManager.cpp \
	src/logic/managers/FontStyleManager.cpp \
	src/logic/managers/ImageManager.cpp \
	src/logic/managers/ParagraphStyleManager.cpp \
	src/logic/styles/FontStyle.cpp \
	src/logic/Page.cpp \
	src/logic/Document.cpp \
	src/logic/styles/ParagraphStyle.cpp \
	src/resources/VectorGraphics.cpp \
	DocxRenderer.cpp

disable_full_document_creation {
	DEFINES += DISABLE_FULL_DOCUMENT_CREATION
} else {
	SOURCES += \
		src/resources/resources.cpp
}

DISTFILES += \
	readme.md
