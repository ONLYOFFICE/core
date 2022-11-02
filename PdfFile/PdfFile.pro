QT -= core
QT -= gui

VERSION = 0.0.0.1
TARGET = PdfFile
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin
CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

DEFINES += PDFFILE_USE_DYNAMIC_LIBRARY

ADD_DEPENDENCY(graphics, kernel, UnicodeConverter, kernel_network)

# PdfReader

core_windows {
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32 \
        -lOle32
}

core_android:DEFINES += ANDROID

PDFREADER_ROOT_DIR = $$PWD/../PdfReader

INCLUDEPATH += \
    $$PDFREADER_ROOT_DIR/lib/goo \
    $$PDFREADER_ROOT_DIR/lib/fofi \
    $$PDFREADER_ROOT_DIR/lib/splash \
    $$PDFREADER_ROOT_DIR/lib

HEADERS += $$files($$PDFREADER_ROOT_DIR/lib/*.h, true)
SOURCES += $$files($$PDFREADER_ROOT_DIR/lib/*.c, true)
SOURCES += $$files($$PDFREADER_ROOT_DIR/lib/*.cc, true)

SOURCES -= \
    $$PDFREADER_ROOT_DIR/lib/xpdf/HTMLGen.cc \
    $$PDFREADER_ROOT_DIR/lib/xpdf/pdftohtml.cc \
    $$PDFREADER_ROOT_DIR/lib/xpdf/pdftopng.cc \
    $$PDFREADER_ROOT_DIR/lib/xpdf/pdftoppm.cc \
    $$PDFREADER_ROOT_DIR/lib/xpdf/pdftops.cc \
    $$PDFREADER_ROOT_DIR/lib/xpdf/pdftotext.cc \
    $$PDFREADER_ROOT_DIR/lib/xpdf/pdfdetach.cc \
    $$PDFREADER_ROOT_DIR/lib/xpdf/pdffonts.cc \
    $$PDFREADER_ROOT_DIR/lib/xpdf/pdfimages.cc \
    $$PDFREADER_ROOT_DIR/lib/xpdf/pdfinfo.cc

SOURCES += \
    $$PDFREADER_ROOT_DIR/Src/RendererOutputDev.cpp \
    $$PDFREADER_ROOT_DIR/Src/Adaptors.cpp \
    $$PDFREADER_ROOT_DIR/Src/GfxClip.cpp

HEADERS += \
    $$PDFREADER_ROOT_DIR/Src/RendererOutputDev.h \
    $$PDFREADER_ROOT_DIR/Src/Adaptors.h \
    $$PDFREADER_ROOT_DIR/Src/MemoryUtils.h \
    $$PDFREADER_ROOT_DIR/Src/GfxClip.h

# Base fonts
HEADERS += \
    $$PDFREADER_ROOT_DIR/Resources/Fontd050000l.h \
    $$PDFREADER_ROOT_DIR/Resources/Fontn019003l.h \
    $$PDFREADER_ROOT_DIR/Resources/Fontn019004l.h \
    $$PDFREADER_ROOT_DIR/Resources/Fontn019023l.h \
    $$PDFREADER_ROOT_DIR/Resources/Fontn019024l.h \
    $$PDFREADER_ROOT_DIR/Resources/Fontn021003l.h \
    $$PDFREADER_ROOT_DIR/Resources/Fontn021004l.h \
    $$PDFREADER_ROOT_DIR/Resources/Fontn021023l.h \
    $$PDFREADER_ROOT_DIR/Resources/Fontn021024l.h \
    $$PDFREADER_ROOT_DIR/Resources/Fontn022003l.h \
    $$PDFREADER_ROOT_DIR/Resources/Fontn022004l.h \
    $$PDFREADER_ROOT_DIR/Resources/Fontn022023l.h \
    $$PDFREADER_ROOT_DIR/Resources/Fontn022024l.h \
    $$PDFREADER_ROOT_DIR/Resources/Fonts050000l.h \
    $$PDFREADER_ROOT_DIR/Resources/BaseFonts.h

SOURCES += \
    $$PDFREADER_ROOT_DIR/Resources/BaseFonts.cpp

CONFIG += use_external_jpeg2000
use_external_jpeg2000 {
    DEFINES += USE_EXTERNAL_JPEG2000
    CONFIG += use_openjpeg2000
    use_openjpeg2000 {
        include($$PWD/../DesktopEditor/raster/Jp2/openjpeg/openjpeg.pri)
    } else {
        DEFINES += USE_GRAPHICS_JPEG2000
    }
    HEADERS += $$PDFREADER_ROOT_DIR/Src/JPXStream2.h
    SOURCES += $$PDFREADER_ROOT_DIR/Src/JPXStream2.cpp
}

#CONFIG += build_viewer_module
build_viewer_module {
    DEFINES += BUILDING_WASM_MODULE \
               TEST_AS_EXECUTABLE

    HEADERS += $$CORE_ROOT_DIR/HtmlRenderer/include/HTMLRendererText.h
    SOURCES += $$CORE_ROOT_DIR/HtmlRenderer/src/HTMLRendererText.cpp
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

include($$PWD/../DesktopEditor/graphics/pro/freetype.pri)

PDFWRITER_ROOT_DIR = $$PWD/../PdfWriter

HEADERS += \
    $$PDFWRITER_ROOT_DIR/Src/AcroForm.h \
    $$PDFWRITER_ROOT_DIR/Src/Annotation.h \
    $$PDFWRITER_ROOT_DIR/Src/Catalog.h \
    $$PDFWRITER_ROOT_DIR/Src/Consts.h \
    $$PDFWRITER_ROOT_DIR/Src/Destination.h \
    $$PDFWRITER_ROOT_DIR/Src/Document.h \
    $$PDFWRITER_ROOT_DIR/Src/Encodings.h \
    $$PDFWRITER_ROOT_DIR/Src/Encrypt.h \
    $$PDFWRITER_ROOT_DIR/Src/EncryptDictionary.h \
    $$PDFWRITER_ROOT_DIR/Src/Field.h \
    $$PDFWRITER_ROOT_DIR/Src/Font.h \
    $$PDFWRITER_ROOT_DIR/Src/Font14.h \
    $$PDFWRITER_ROOT_DIR/Src/FontCidTT.h \
    $$PDFWRITER_ROOT_DIR/Src/FontTT.h \
    $$PDFWRITER_ROOT_DIR/Src/FontTTWriter.h \
    $$PDFWRITER_ROOT_DIR/Src/GState.h \
    $$PDFWRITER_ROOT_DIR/Src/Image.h \
    $$PDFWRITER_ROOT_DIR/Src/Info.h \
    $$PDFWRITER_ROOT_DIR/Src/Objects.h \
    $$PDFWRITER_ROOT_DIR/Src/Outline.h \
    $$PDFWRITER_ROOT_DIR/Src/Pages.h \
    $$PDFWRITER_ROOT_DIR/Src/Pattern.h \
    $$PDFWRITER_ROOT_DIR/Src/ResourcesDictionary.h \
    $$PDFWRITER_ROOT_DIR/Src/Shading.h \
    $$PDFWRITER_ROOT_DIR/Src/Streams.h \
    $$PDFWRITER_ROOT_DIR/Src/Types.h \
    $$PDFWRITER_ROOT_DIR/Src/Utils.h \
    $$PDFWRITER_ROOT_DIR/Src/Metadata.h \
    $$PDFWRITER_ROOT_DIR/Src/ICCProfile.h

SOURCES += \
    $$PDFWRITER_ROOT_DIR/Src/AcroForm.cpp \
    $$PDFWRITER_ROOT_DIR/Src/Annotation.cpp \
    $$PDFWRITER_ROOT_DIR/Src/Catalog.cpp \
    $$PDFWRITER_ROOT_DIR/Src/Destination.cpp \
    $$PDFWRITER_ROOT_DIR/Src/Document.cpp \
    $$PDFWRITER_ROOT_DIR/Src/Encrypt.cpp \
    $$PDFWRITER_ROOT_DIR/Src/EncryptDictionary.cpp \
    $$PDFWRITER_ROOT_DIR/Src/Field.cpp \
    $$PDFWRITER_ROOT_DIR/Src/Font.cpp \
    $$PDFWRITER_ROOT_DIR/Src/Font14.cpp \
    $$PDFWRITER_ROOT_DIR/Src/FontCidTT.cpp \
    $$PDFWRITER_ROOT_DIR/Src/FontTT.cpp \
    $$PDFWRITER_ROOT_DIR/Src/FontTTWriter.cpp \
    $$PDFWRITER_ROOT_DIR/Src/GState.cpp \
    $$PDFWRITER_ROOT_DIR/Src/Image.cpp \
    $$PDFWRITER_ROOT_DIR/Src/Info.cpp \
    $$PDFWRITER_ROOT_DIR/Src/Objects.cpp \
    $$PDFWRITER_ROOT_DIR/Src/Outline.cpp \
    $$PDFWRITER_ROOT_DIR/Src/Pages.cpp \
    $$PDFWRITER_ROOT_DIR/Src/Pattern.cpp \
    $$PDFWRITER_ROOT_DIR/Src/ResourcesDictionary.cpp \
    $$PDFWRITER_ROOT_DIR/Src/Shading.cpp \
    $$PDFWRITER_ROOT_DIR/Src/Streams.cpp \
    $$PDFWRITER_ROOT_DIR/Src/Utils.cpp \
    $$PDFWRITER_ROOT_DIR/Src/Metadata.cpp

# PdfFile

HEADERS += PdfFile.h \
           Src/PdfWriter.h \
           Src/PdfReader.h \
           Src/OnlineOfficeBinToPdf.h

SOURCES += PdfFile.cpp \
           Src/PdfWriter.cpp \
           Src/PdfReader.cpp \
           Src/OnlineOfficeBinToPdf.cpp
