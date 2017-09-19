QT       -= core gui

TARGET = freetype
TEMPLATE = lib
CONFIG += staticlib

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD
include(../../../../Common/base.pri)

FREETYPE_LIB_PATH=$$PWD/../../freetype-2.5.3

INCLUDEPATH += \
    $$FREETYPE_LIB_PATH/include

DEFINES += \
    FT2_BUILD_LIBRARY

DESTDIR=$$PWD/../build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX

SOURCES += \
    $$FREETYPE_LIB_PATH/src/base/ftbbox.c \
    $$FREETYPE_LIB_PATH/src/base/ftgxval.c \
    $$FREETYPE_LIB_PATH/src/base/ftlcdfil.c \
    $$FREETYPE_LIB_PATH/src/base/ftmm.c \
    $$FREETYPE_LIB_PATH/src/base/ftotval.c \
    $$FREETYPE_LIB_PATH/src/base/ftpatent.c \
    $$FREETYPE_LIB_PATH/src/base/ftpfr.c \
    $$FREETYPE_LIB_PATH/src/base/ftsynth.c \
    $$FREETYPE_LIB_PATH/src/base/fttype1.c \
    $$FREETYPE_LIB_PATH/src/base/ftwinfnt.c \
    $$FREETYPE_LIB_PATH/src/base/ftxf86.c \
    $$FREETYPE_LIB_PATH/src/pcf/pcf.c \
    $$FREETYPE_LIB_PATH/src/pfr/pfr.c \
    $$FREETYPE_LIB_PATH/src/psaux/psaux.c \
    $$FREETYPE_LIB_PATH/src/pshinter/pshinter.c \
    $$FREETYPE_LIB_PATH/src/psnames/psmodule.c \
    $$FREETYPE_LIB_PATH/src/raster/raster.c \
    $$FREETYPE_LIB_PATH/src/sfnt/sfnt.c \
    $$FREETYPE_LIB_PATH/src/truetype/truetype.c \
    $$FREETYPE_LIB_PATH/src/type1/type1.c \
    $$FREETYPE_LIB_PATH/src/cid/type1cid.c \
    $$FREETYPE_LIB_PATH/src/type42/type42.c \
    $$FREETYPE_LIB_PATH/src/winfonts/winfnt.c \
    \
    $$FREETYPE_LIB_PATH/src/autofit/autofit.c \
    $$FREETYPE_LIB_PATH/src/bdf/bdf.c \
    $$FREETYPE_LIB_PATH/src/cff/cff.c \
    $$FREETYPE_LIB_PATH/src/base/ftbase.c \
    $$FREETYPE_LIB_PATH/src/base/ftbitmap.c \
    $$FREETYPE_LIB_PATH/src/cache/ftcache.c \
    $$FREETYPE_LIB_PATH/src/base/ftfstype.c \
    $$FREETYPE_LIB_PATH/src/base/ftgasp.c \
    $$FREETYPE_LIB_PATH/src/base/ftglyph.c \
    $$FREETYPE_LIB_PATH/src/gzip/ftgzip.c \
    $$FREETYPE_LIB_PATH/src/base/ftinit.c \
    $$FREETYPE_LIB_PATH/src/lzw/ftlzw.c \
    $$FREETYPE_LIB_PATH/src/base/ftstroke.c \
    $$FREETYPE_LIB_PATH/src/base/ftsystem.c \
    $$FREETYPE_LIB_PATH/src/smooth/smooth.c
