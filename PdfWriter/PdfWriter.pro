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

ADD_DEPENDENCY(graphics, kernel, UnicodeConverter)

DEFINES += CRYPTOPP_DISABLE_ASM
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib

DEFINES += NOMINMAX

core_linux {
    QMAKE_CXXFLAGS += -Wno-narrowing
}

core_windows {
    DEFINES -= UNICODE
    DEFINES -= _UNICODE

    LIBS += -lAdvapi32
    LIBS += -lShell32
    LIBS += -lUser32
}

# FREETYPE ------------------------------------
DEFINES += FT2_BUILD_LIBRARY

INCLUDEPATH += \
    ../DesktopEditor/freetype-2.5.2/include

SOURCES += \
    ../DesktopEditor/freetype-2.5.2/src/base/ftbbox.c \
    ../DesktopEditor/freetype-2.5.2/src/base/ftgxval.c \
    ../DesktopEditor/freetype-2.5.2/src/base/ftlcdfil.c \
    ../DesktopEditor/freetype-2.5.2/src/base/ftmm.c \
    ../DesktopEditor/freetype-2.5.2/src/base/ftotval.c \
    ../DesktopEditor/freetype-2.5.2/src/base/ftpatent.c \
    ../DesktopEditor/freetype-2.5.2/src/base/ftpfr.c \
    ../DesktopEditor/freetype-2.5.2/src/base/ftsynth.c \
    ../DesktopEditor/freetype-2.5.2/src/base/fttype1.c \
    ../DesktopEditor/freetype-2.5.2/src/base/ftwinfnt.c \
    ../DesktopEditor/freetype-2.5.2/src/base/ftxf86.c \
    ../DesktopEditor/freetype-2.5.2/src/pcf/pcf.c \
    ../DesktopEditor/freetype-2.5.2/src/pfr/pfr.c \
    ../DesktopEditor/freetype-2.5.2/src/psaux/psaux.c \
    ../DesktopEditor/freetype-2.5.2/src/pshinter/pshinter.c \
    ../DesktopEditor/freetype-2.5.2/src/psnames/psmodule.c \
    ../DesktopEditor/freetype-2.5.2/src/raster/raster.c \
    ../DesktopEditor/freetype-2.5.2/src/sfnt/sfnt.c \
    ../DesktopEditor/freetype-2.5.2/src/truetype/truetype.c \
    ../DesktopEditor/freetype-2.5.2/src/type1/type1.c \
    ../DesktopEditor/freetype-2.5.2/src/cid/type1cid.c \
    ../DesktopEditor/freetype-2.5.2/src/type42/type42.c \
    ../DesktopEditor/freetype-2.5.2/src/winfonts/winfnt.c \
    \
    ../DesktopEditor/freetype-2.5.2/src/autofit/autofit.c \
    ../DesktopEditor/freetype-2.5.2/src/bdf/bdf.c \
    ../DesktopEditor/freetype-2.5.2/src/cff/cff.c \
    ../DesktopEditor/freetype-2.5.2/src/base/ftbase.c \
    ../DesktopEditor/freetype-2.5.2/src/base/ftbitmap.c \
    ../DesktopEditor/freetype-2.5.2/src/cache/ftcache.c \
    ../DesktopEditor/freetype-2.5.2/src/base/ftfstype.c \
    ../DesktopEditor/freetype-2.5.2/src/base/ftgasp.c \
    ../DesktopEditor/freetype-2.5.2/src/base/ftglyph.c \
    ../DesktopEditor/freetype-2.5.2/src/gzip/ftgzip.c \
    ../DesktopEditor/freetype-2.5.2/src/base/ftinit.c \
    ../DesktopEditor/freetype-2.5.2/src/lzw/ftlzw.c \
    ../DesktopEditor/freetype-2.5.2/src/base/ftstroke.c \
    ../DesktopEditor/freetype-2.5.2/src/base/ftsystem.c \
    ../DesktopEditor/freetype-2.5.2/src/smooth/smooth.c

# ---------------------------------------------

core_windows {

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
    PdfRenderer.h \
    Src/Metadata.h \
    Src/ICCProfile.h

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
    Src/Metadata.cpp \
    OnlineOfficeBinToPdf.cpp \
    PdfRenderer.cpp
