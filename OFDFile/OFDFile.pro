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
    src/Document.h \
    src/Page.h \
    src/PublicRes.h

SOURCES += \
    OFDFile.cpp \
    src/Base.cpp \
    src/Document.cpp \
    src/Page.cpp \
    src/PublicRes.cpp

HEADERS += $$CORE_ROOT_DIR/OOXML/Base/Unit.h
SOURCES += $$CORE_ROOT_DIR/OOXML/Base/Unit.cpp
