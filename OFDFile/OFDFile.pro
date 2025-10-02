QT       -= core gui

VERSION = 1.0.0.1
TARGET = OFDFile
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)

DEFINES += OFD_USE_DYNAMIC_LIBRARY

ADD_DEPENDENCY(graphics, kernel, UnicodeConverter, PdfFile)

core_windows {
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

INCLUDEPATH += \
	$$CORE_ROOT_DIR/DesktopEditor/freetype-2.10.4/include \
	$$CORE_ROOT_DIR/DesktopEditor/freetype-2.10.4/include/freetype

HEADERS += \
	OFDFile.h \
	src/Annotation.h \
	src/Content/ImageObject.h \
	src/Content/PageBlock.h \
	src/Content/PathObject.h \
	src/IOFDElement.h \
	src/OFDFile_Private.h \
	src/Base.h \
	src/Content/Content.h \
	src/Content/GraphicUnit.h \
	src/Content/IPageBlock.h \
	src/Content/Layer.h \
	src/Content/TextObject.h \
	src/Document.h \
	src/Page.h \
	src/Res.h \
	src/Types/Color.h \
	src/Types/ColorSpace.h \
	src/Types/CommonData.h \
	src/Types/CompositeGraphicUnit.h \
	src/Types/DrawParam.h \
	src/Types/Font.h \
	src/Types/MultiMedia.h \
	src/Types/PageArea.h \
	src/Types/PenSettings.h \
	src/Types/Signature.h \
	src/Types/TemplatePage.h \
	src/Utils/Types.h \
	src/Utils/Utils.h \
	src/Utils/XmlReader.h

SOURCES += \
	OFDFile.cpp \
	src/Annotation.cpp \
	src/Content/ImageObject.cpp \
	src/Content/PageBlock.cpp \
	src/Content/PathObject.cpp \
	src/OFDFile_Private.cpp \
	src/Base.cpp \
	src/Content/Content.cpp \
	src/Content/GraphicUnit.cpp \
	src/Content/Layer.cpp \
	src/Content/TextObject.cpp \
	src/Document.cpp \
	src/Page.cpp \
	src/Res.cpp \
	src/Types/Color.cpp \
	src/Types/ColorSpace.cpp \
	src/Types/CommonData.cpp \
	src/Types/CompositeGraphicUnit.cpp \
	src/Types/DrawParam.cpp \
	src/Types/Font.cpp \
	src/Types/MultiMedia.cpp \
	src/Types/PageArea.cpp \
	src/Types/PenSettings.cpp \
	src/Types/Signature.cpp \
	src/Types/TemplatePage.cpp \
	src/Utils/Types.cpp \
	src/Utils/XmlReader.cpp
