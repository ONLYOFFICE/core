QT       -= core
QT       -= gui

VERSION = 1.0.0.3
TARGET = doctrenderer
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += c++11

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD
include(../../Common/base.pri)

include(../Qt_build/graphics/project/graphics_fonts.pri)
include(../xml/build/qt/libxml2.pri)

CONFIG += build_all_zlib build_zlib_as_sources
include(../../OfficeUtils/OfficeUtils.pri)

include(../../Common/3dParty/v8/v8.pri)

SOURCES += \
    memorystream.cpp \
    nativecontrol.cpp \
    doctrenderer.cpp \
    docbuilder.cpp

SOURCES += \
    ../../Common/OfficeFileFormatChecker2.cpp \
    ../../Common/3dParty/pole/pole.cpp

HEADERS += doctrenderer.h \
    docbuilder.h \
    docbuilder_p.h \
    memorystream.h \
    nativecontrol.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
