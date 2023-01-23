DEFINES += FT_CONFIG_OPTION_SYSTEM_ZLIB
DEFINES += FT2_BUILD_LIBRARY
FREETYPE_VERSION=2.10.4

FREETYPE_PATH = $$PWD/../../freetype-$$FREETYPE_VERSION

INCLUDEPATH += \
	$$FREETYPE_PATH/include \
    $$FREETYPE_PATH/include/freetype

!config_zlip_image {
    OFFICEUTILS_PATH = $$PWD/../../../OfficeUtils
	INCLUDEPATH += \
	    $$OFFICEUTILS_PATH/src/zlib-1.2.11 \
		$$OFFICEUTILS_PATH/src

    SOURCES += \
	    $$OFFICEUTILS_PATH/src/zlib-1.2.11/crc32.c \
		$$OFFICEUTILS_PATH/src/zlib-1.2.11/adler32.c \
		$$OFFICEUTILS_PATH/src/zlib-1.2.11/deflate.c \
		$$OFFICEUTILS_PATH/src/zlib-1.2.11/inffast.c \
		$$OFFICEUTILS_PATH/src/zlib-1.2.11/inflate.c \
		$$OFFICEUTILS_PATH/src/zlib-1.2.11/inftrees.c \
		$$OFFICEUTILS_PATH/src/zlib-1.2.11/trees.c \
		$$OFFICEUTILS_PATH/src/zlib-1.2.11/zutil.c \
		$$OFFICEUTILS_PATH/src/zlib-1.2.11/uncompr.c \
		$$OFFICEUTILS_PATH/src/zlib-1.2.11/compress.c \
		$$OFFICEUTILS_PATH/src/zlib_addon.c

    CONFIG += config_zlip_image
}

SOURCES += \
    $$FREETYPE_PATH/src/autofit/autofit.c \
    $$FREETYPE_PATH/src/base/ftbase.c \
    $$FREETYPE_PATH/src/base/ftbbox.c \
    $$FREETYPE_PATH/src/base/ftbdf.c \
    $$FREETYPE_PATH/src/base/ftbitmap.c \
    $$FREETYPE_PATH/src/base/ftcid.c \
    $$FREETYPE_PATH/src/base/ftfstype.c \
    $$FREETYPE_PATH/src/base/ftgasp.c \
    $$FREETYPE_PATH/src/base/ftglyph.c \
    $$FREETYPE_PATH/src/base/ftgxval.c \
    $$FREETYPE_PATH/src/base/ftinit.c \
    $$FREETYPE_PATH/src/base/ftmm.c \
    $$FREETYPE_PATH/src/base/ftotval.c \
    $$FREETYPE_PATH/src/base/ftpatent.c \
    $$FREETYPE_PATH/src/base/ftpfr.c \
    $$FREETYPE_PATH/src/base/ftstroke.c \
    $$FREETYPE_PATH/src/base/ftsynth.c \
    $$FREETYPE_PATH/src/base/fttype1.c \
    $$FREETYPE_PATH/src/base/ftwinfnt.c \
    $$FREETYPE_PATH/src/bdf/bdf.c \
    $$FREETYPE_PATH/src/bzip2/ftbzip2.c \
    $$FREETYPE_PATH/src/cache/ftcache.c \
    $$FREETYPE_PATH/src/cff/cff.c \
    $$FREETYPE_PATH/src/cid/type1cid.c \
    $$FREETYPE_PATH/src/gzip/ftgzip.c \
    $$FREETYPE_PATH/src/lzw/ftlzw.c \
    $$FREETYPE_PATH/src/pcf/pcf.c \
    $$FREETYPE_PATH/src/pfr/pfr.c \
    $$FREETYPE_PATH/src/psaux/psaux.c \
    $$FREETYPE_PATH/src/pshinter/pshinter.c \
    $$FREETYPE_PATH/src/psnames/psnames.c \
    $$FREETYPE_PATH/src/raster/raster.c \
    $$FREETYPE_PATH/src/sfnt/sfnt.c \
    $$FREETYPE_PATH/src/smooth/smooth.c \
    $$FREETYPE_PATH/src/truetype/truetype.c \
    $$FREETYPE_PATH/src/type1/type1.c \
    $$FREETYPE_PATH/src/type42/type42.c \
    $$FREETYPE_PATH/src/winfonts/winfnt.c \
    $$FREETYPE_PATH/src/base/ftdebug.c

core_windows {
	SOURCES += $$FREETYPE_PATH/src/base/ftsystem.c
} else {
	SOURCES += $$FREETYPE_PATH/builds/unix/ftsystem.c
}
