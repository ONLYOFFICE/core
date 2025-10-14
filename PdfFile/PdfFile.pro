QT -= core
QT -= gui

VERSION = 0.0.0.1
TARGET = PdfFile
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

DEFINES += PDFFILE_USE_DYNAMIC_LIBRARY

ADD_DEPENDENCY(graphics, kernel, UnicodeConverter, kernel_network)

#CONFIG += use_openssl_hash
use_openssl_hash {
    DEFINES += USE_OPENSSL_HASH
    INCLUDEPATH += $$PWD/../Common/3dParty/openssl/openssl/include
}

# PdfReader

core_windows {
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32 \
        -lOle32
}

core_android:DEFINES += ANDROID

INCLUDEPATH += \
    lib/goo \
    lib/fofi \
    lib/splash \
    lib

HEADERS += $$files(lib/*.h, true)
SOURCES += $$files(lib/*.c, true)
SOURCES += $$files(lib/*.cpp, true)
SOURCES += $$files(lib/*.cc, true)

SOURCES -= \
    lib/xpdf/HTMLGen.cc \
    lib/xpdf/pdftohtml.cc \
    lib/xpdf/pdftopng.cc \
    lib/xpdf/pdftoppm.cc \
    lib/xpdf/pdftops.cc \
    lib/xpdf/pdftotext.cc \
    lib/xpdf/pdfdetach.cc \
    lib/xpdf/pdffonts.cc \
    lib/xpdf/pdfimages.cc \
    lib/xpdf/pdfinfo.cc

SOURCES += \
	SrcReader/RendererOutputDev.cpp \
    SrcReader/Adaptors.cpp \
    SrcReader/PdfAnnot.cpp \
    SrcReader/GfxClip.cpp

HEADERS += \
	SrcReader/RendererOutputDev.h \
    SrcReader/Adaptors.h \
    SrcReader/MemoryUtils.h \
    SrcReader/PdfAnnot.h \
    SrcReader/GfxClip.h

# Base fonts
HEADERS += \
    Resources/Fontd050000l.h \
    Resources/Fontn019003l.h \
    Resources/Fontn019004l.h \
    Resources/Fontn019023l.h \
    Resources/Fontn019024l.h \
    Resources/Fontn021003l.h \
    Resources/Fontn021004l.h \
    Resources/Fontn021023l.h \
    Resources/Fontn021024l.h \
    Resources/Fontn022003l.h \
    Resources/Fontn022004l.h \
    Resources/Fontn022023l.h \
    Resources/Fontn022024l.h \
    Resources/Fonts050000l.h \
    Resources/CMapMemory/cmap_memory.h \
    Resources/BaseFonts.h

SOURCES += \
    Resources/CMapMemory/cmap_memory.cpp \
    Resources/BaseFonts.cpp

CONFIG += use_external_jpeg2000
use_external_jpeg2000 {
    DEFINES += USE_EXTERNAL_JPEG2000
    CONFIG += use_openjpeg2000
    use_openjpeg2000 {
        include($$PWD/../DesktopEditor/raster/Jp2/openjpeg/openjpeg.pri)
    } else {
        DEFINES += USE_GRAPHICS_JPEG2000
    }
    HEADERS += SrcReader/JPXStream2.h
    SOURCES += SrcReader/JPXStream2.cpp
}

# PdfWriter
DEFINES += CRYPTOPP_DISABLE_ASM \
           NOMINMAX
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib

core_linux {
    DEFINES += HAVE_UNISTD_H \
               HAVE_FCNTL_H
    QMAKE_CXXFLAGS += -Wno-narrowing
}
core_mac {
    DEFINES += HAVE_UNISTD_H \
               HAVE_FCNTL_H
}
core_android {
    QMAKE_CXXFLAGS += -Wno-narrowing
}

core_windows {
    DEFINES -= UNICODE
    DEFINES -= _UNICODE
}

include($$PWD/../Common/3dParty/brotli/brotli.pri)
include($$PWD/../DesktopEditor/graphics/pro/freetype.pri)

HEADERS += \
    SrcWriter/AcroForm.h \
    SrcWriter/Annotation.h \
    SrcWriter/Catalog.h \
    SrcWriter/Consts.h \
    SrcWriter/Destination.h \
    SrcWriter/Document.h \
    SrcWriter/Encodings.h \
    SrcWriter/Encrypt.h \
    SrcWriter/EncryptDictionary.h \
    SrcWriter/Field.h \
    SrcWriter/Font.h \
    SrcWriter/Font14.h \
    SrcWriter/FontCidTT.h \
    SrcWriter/FontTT.h \
    SrcWriter/FontTTWriter.h \
    SrcWriter/GState.h \
    SrcWriter/Image.h \
    SrcWriter/Info.h \
    SrcWriter/Objects.h \
    SrcWriter/Outline.h \
    SrcWriter/Pages.h \
    SrcWriter/Pattern.h \
    SrcWriter/ResourcesDictionary.h \
    SrcWriter/Shading.h \
    SrcWriter/Streams.h \
    SrcWriter/Types.h \
    SrcWriter/Utils.h \
    SrcWriter/Metadata.h \
    SrcWriter/ICCProfile.h \
	SrcWriter/States.h \
	SrcWriter/RedactOutputDev.h

SOURCES += \
    SrcWriter/AcroForm.cpp \
    SrcWriter/Annotation.cpp \
    SrcWriter/Catalog.cpp \
    SrcWriter/Destination.cpp \
    SrcWriter/Document.cpp \
    SrcWriter/Encrypt.cpp \
    SrcWriter/EncryptDictionary.cpp \
    SrcWriter/Field.cpp \
    SrcWriter/Font.cpp \
    SrcWriter/Font14.cpp \
    SrcWriter/FontCidTT.cpp \
    SrcWriter/FontTT.cpp \
    SrcWriter/FontTTWriter.cpp \
    SrcWriter/FontOTWriter.cpp \
    SrcWriter/GState.cpp \
    SrcWriter/Image.cpp \
    SrcWriter/Info.cpp \
    SrcWriter/Objects.cpp \
    SrcWriter/Outline.cpp \
    SrcWriter/Pages.cpp \
    SrcWriter/Pattern.cpp \
    SrcWriter/ResourcesDictionary.cpp \
    SrcWriter/Shading.cpp \
    SrcWriter/Streams.cpp \
    SrcWriter/Utils.cpp \
    SrcWriter/Metadata.cpp \
	SrcWriter/States.cpp \
	SrcWriter/RedactOutputDev.cpp

# PdfFile

HEADERS += PdfFile.h \
           PdfWriter.h \
           PdfReader.h \
           PdfEditor.h \
           OnlineOfficeBinToPdf.h

SOURCES += PdfFile.cpp \
           PdfWriter.cpp \
           PdfReader.cpp \
           PdfEditor.cpp \
           OnlineOfficeBinToPdf.cpp
