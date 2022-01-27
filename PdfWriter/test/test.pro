QT -= core
QT -= gui

TARGET = test
TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
include($$CORE_ROOT_DIR/DesktopEditor/graphics/pro/freetype.pri)

ADD_DEPENDENCY(UnicodeConverter, kernel, graphics, PdfReader, PdfWriter)

INCLUDEPATH += \
    $$CORE_ROOT_DIR/DesktopEditor/agg-2.4/include \
    $$CORE_ROOT_DIR/DesktopEditor/cximage/jasper/include \
    $$CORE_ROOT_DIR/DesktopEditor/cximage/jpeg \
    $$CORE_ROOT_DIR/DesktopEditor/cximage/png \
    $$CORE_ROOT_DIR/DesktopEditor/freetype-2.10.4/include \
    $$CORE_ROOT_DIR/DesktopEditor/freetype-2.10.4/include/freetype

core_windows {
DEFINES += NOMINMAX
DEFINES -= UNICODE
DEFINES -= _UNICODE

LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32 \
        -lOle32
}

DEFINES += CRYPTOPP_DISABLE_ASM
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib

#PdfWriter

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

# PdfReader

PDF_ROOT_DIR = $$PWD/../../PdfReader

INCLUDEPATH += \
    $$PDF_ROOT_DIR/lib/goo \
    $$PDF_ROOT_DIR/lib/fofi \
    $$PDF_ROOT_DIR/lib/splash \
    $$PDF_ROOT_DIR/lib

HEADERS += \
    $$PDF_ROOT_DIR/lib/aconf.h \
    $$$files($$PDF_ROOT_DIR/lib/*.h)

SOURCES += $$files($$PDF_ROOT_DIR/lib/*.c, true)
SOURCES += $$files($$PDF_ROOT_DIR/lib/*.cc, true)

SOURCES -= \
    $$PDF_ROOT_DIR/lib/xpdf/HTMLGen.cc \
    $$PDF_ROOT_DIR/lib/xpdf/pdftohtml.cc \
    $$PDF_ROOT_DIR/lib/xpdf/pdftopng.cc \
    $$PDF_ROOT_DIR/lib/xpdf/pdftoppm.cc \
    $$PDF_ROOT_DIR/lib/xpdf/pdftops.cc \
    $$PDF_ROOT_DIR/lib/xpdf/pdftotext.cc \
    $$PDF_ROOT_DIR/lib/xpdf/pdfdetach.cc \
    $$PDF_ROOT_DIR/lib/xpdf/pdffonts.cc \
    $$PDF_ROOT_DIR/lib/xpdf/pdfimages.cc \
    $$PDF_ROOT_DIR/lib/xpdf/pdfinfo.cc

SOURCES += \
    $$PDF_ROOT_DIR/Src/RendererOutputDev.cpp \
    $$PDF_ROOT_DIR/Src/Adaptors.cpp \
    $$PDF_ROOT_DIR/Src/GfxClip.cpp \
    $$PDF_ROOT_DIR/Resources/BaseFonts.cpp \
    $$PDF_ROOT_DIR/PdfReader.cpp

HEADERS +=\
    $$PDF_ROOT_DIR/Resources/Fontd050000l.h \
    $$PDF_ROOT_DIR/Resources/Fontn019003l.h \
    $$PDF_ROOT_DIR/Resources/Fontn019004l.h \
    $$PDF_ROOT_DIR/Resources/Fontn019023l.h \
    $$PDF_ROOT_DIR/Resources/Fontn019024l.h \
    $$PDF_ROOT_DIR/Resources/Fontn021003l.h \
    $$PDF_ROOT_DIR/Resources/Fontn021004l.h \
    $$PDF_ROOT_DIR/Resources/Fontn021023l.h \
    $$PDF_ROOT_DIR/Resources/Fontn021024l.h \
    $$PDF_ROOT_DIR/Resources/Fontn022003l.h \
    $$PDF_ROOT_DIR/Resources/Fontn022004l.h \
    $$PDF_ROOT_DIR/Resources/Fontn022023l.h \
    $$PDF_ROOT_DIR/Resources/Fontn022024l.h \
    $$PDF_ROOT_DIR/Resources/Fonts050000l.h \
    $$PDF_ROOT_DIR/Resources/BaseFonts.h \
    $$PDF_ROOT_DIR/Src/RendererOutputDev.h \
    $$PDF_ROOT_DIR/Src/Adaptors.h \
    $$PDF_ROOT_DIR/Src/MemoryUtils.h \
    $$PDF_ROOT_DIR/Src/GfxClip.h \
    $$PDF_ROOT_DIR/PdfReader.h

SOURCES += test.cpp
