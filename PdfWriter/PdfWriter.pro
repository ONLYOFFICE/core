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
    DEFINES += HAVE_UNISTD_H HAVE_FCNTL_H
    QMAKE_CXXFLAGS += -Wno-narrowing
}
core_mac {
    DEFINES += HAVE_UNISTD_H HAVE_FCNTL_H
}
core_android {
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
FREETYPE_VERSION=2.10.4

INCLUDEPATH += \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/include \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/include/freetype

SOURCES += \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/autofit/autofit.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftbase.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftbbox.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftbdf.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftbitmap.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftcid.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftfstype.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftgasp.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftglyph.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftgxval.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftinit.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftmm.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftotval.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftpatent.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftpfr.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftstroke.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftsynth.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/fttype1.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftwinfnt.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/bdf/bdf.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/bzip2/ftbzip2.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/cache/ftcache.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/cff/cff.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/cid/type1cid.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/gzip/ftgzip.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/lzw/ftlzw.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/pcf/pcf.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/pfr/pfr.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/psaux/psaux.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/pshinter/pshinter.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/psnames/psnames.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/raster/raster.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/sfnt/sfnt.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/smooth/smooth.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/truetype/truetype.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/type1/type1.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/type42/type42.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/winfonts/winfnt.c \
    ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftdebug.c

    core_windows {
        SOURCES += ../DesktopEditor/freetype-$$FREETYPE_VERSION/src/base/ftsystem.c
    } else {
        SOURCES += ../DesktopEditor/freetype-$$FREETYPE_VERSION/builds/unix/ftsystem.c
    }
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
