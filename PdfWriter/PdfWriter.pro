QT       -= core gui

VERSION = 1.0.0.4
TARGET = PdfWriter
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include(../Common/base.pri)

DEFINES += PDFWRITER_USE_DYNAMIC_LIBRARY

CONFIG += build_all_zlib build_zlib_as_sources
include(../OfficeUtils/OfficeUtils.pri)

CONFIG += build_cximage_zlib_disable
include(../DesktopEditor/Qt_build/graphics/project/graphics.pri)
include(../DesktopEditor/xml/build/qt/libxml2.pri)

core_windows {

#UnicodeConverter
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lUnicodeConverter

LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

HEADERS += \
    Src/Annotation.h \
    Src/Catalog.h \
    Src/Consts.h \
    Src/Destination.h \
    Src/Document.h \
    Src/Encrypt.h \
    Src/EncryptDictionary.h \
    Src/Font.h \
    Src/Font14.h \
    Src/FontCidTT.h \
    Src/FontTTWriter.h \
    Src/GState.h \
    Src/Image.h \
    Src/Info.h \
    Src/Objects.h \
    Src/Outline.h \
    Src/Pages.h \
    Src/Pattern.h \
    Src/Shading.h \
    Src/Streams.h \
    Src/Types.h \
    Src/Utils.h \
    OnlineOfficeBinToPdf.h \
    PdfRenderer.h

SOURCES += \
    Src/Annotation.cpp \
    Src/Catalog.cpp \
    Src/Destination.cpp \
    Src/Document.cpp \
    Src/Encrypt.cpp \
    Src/EncryptDictionary.cpp \
    Src/Font.cpp \
    Src/Font14.cpp \
    Src/FontCidTT.cpp \
    Src/FontTTWriter.cpp \
    Src/GState.cpp \
    Src/Image.cpp \
    Src/Info.cpp \
    Src/Objects.cpp \
    Src/Outline.cpp \
    Src/Pages.cpp \
    Src/Pattern.cpp \
    Src/Shading.cpp \
    Src/Streams.cpp \
    Src/Utils.cpp \
    OnlineOfficeBinToPdf.cpp \
    PdfRenderer.cpp
