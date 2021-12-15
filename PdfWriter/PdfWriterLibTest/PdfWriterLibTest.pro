QT -= core
QT -= gui

TARGET = PdfWriterLibTest
TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
include($$CORE_ROOT_DIR/DesktopEditor/graphics/pro/freetype.pri)

ADD_DEPENDENCY(UnicodeConverter, kernel, graphics, PdfWriter)

INCLUDEPATH += \
    $$CORE_ROOT_DIR/DesktopEditor/agg-2.4/include \
    $$CORE_ROOT_DIR/DesktopEditor/freetype-2.10.4/include \
    $$CORE_ROOT_DIR/DesktopEditor/freetype-2.10.4/include/freetype

DEFINES += NOMINMAX \
           CRYPTOPP_DISABLE_ASM

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib

core_windows {
DEFINES -= UNICODE
DEFINES -= _UNICODE

LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

HEADERS += \
    ../Src/AcroForm.h \
    ../Src/Annotation.h \
    ../Src/Catalog.h \
    ../Src/Consts.h \
    ../Src/Destination.h \
    ../Src/Document.h \
    ../Src/Encodings.h \
    ../Src/Encrypt.h \
    ../Src/EncryptDictionary.h \
    ../Src/Field.h \
    ../Src/Font.h \
    ../Src/Font14.h \
    ../Src/FontCidTT.h \
    ../Src/FontTT.h \
    ../Src/FontTTWriter.h \
    ../Src/GState.h \
    ../Src/Image.h \
    ../Src/Info.h \
    ../Src/Objects.h \
    ../Src/Outline.h \
    ../Src/Pages.h \
    ../Src/Pattern.h \
    ../Src/ResourcesDictionary.h \
    ../Src/Shading.h \
    ../Src/Streams.h \
    ../Src/Types.h \
    ../Src/Utils.h \
    ../OnlineOfficeBinToPdf.h \
    ../PdfRenderer.h \
    ../Src/Metadata.h \
    ../Src/ICCProfile.h \
    stdafx.h \
    targetver.h

SOURCES += \
    ../Src/AcroForm.cpp \
    ../Src/Annotation.cpp \
    ../Src/Catalog.cpp \
    ../Src/Destination.cpp \
    ../Src/Document.cpp \
    ../Src/Encrypt.cpp \
    ../Src/EncryptDictionary.cpp \
    ../Src/Field.cpp \
    ../Src/Font.cpp \
    ../Src/Font14.cpp \
    ../Src/FontCidTT.cpp \
    ../Src/FontTT.cpp \
    ../Src/FontTTWriter.cpp \
    ../Src/GState.cpp \
    ../Src/Image.cpp \
    ../Src/Info.cpp \
    ../Src/Objects.cpp \
    ../Src/Outline.cpp \
    ../Src/Pages.cpp \
    ../Src/Pattern.cpp \
    ../Src/ResourcesDictionary.cpp \
    ../Src/Shading.cpp \
    ../Src/Streams.cpp \
    ../Src/Utils.cpp \
    ../Src/Metadata.cpp \
    ../OnlineOfficeBinToPdf.cpp \
    ../PdfRenderer.cpp

SOURCES += PdfWriterLibTest.cpp
