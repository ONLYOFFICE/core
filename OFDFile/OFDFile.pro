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

ADD_DEPENDENCY(graphics, kernel, UnicodeConverter)

HEADERS += \
    OFDFile.h \
    src/Base.h \
    src/Content/Content.h \
    src/Content/GraphicUnit.h \
    src/Content/IPageBlock.h \
    src/Content/Layer.h \
    src/Content/TextObject.h \
    src/Document.h \
    src/Page.h \
    src/PublicRes.h \
    src/Types/Color.h \
    src/Utils/Types.h \
    src/Utils/Utils.h \
    src/Utils/XmlReader.h

SOURCES += \
    OFDFile.cpp \
    src/Base.cpp \
    src/Content/Content.cpp \
    src/Content/GraphicUnit.cpp \
    src/Content/Layer.cpp \
    src/Content/TextObject.cpp \
    src/Document.cpp \
    src/Page.cpp \
    src/PublicRes.cpp \
    src/Types/Color.cpp \
    src/Utils/Types.cpp \
    src/Utils/XmlReader.cpp
