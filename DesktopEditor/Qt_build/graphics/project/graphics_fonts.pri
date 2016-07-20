DESKTOPEDITOR_PATH =  $$PWD/../../..
DEFINES += \
    _QT \
    FT2_BUILD_LIBRARY

core_linux {
    DEFINES += \
    HAVE_UNISTD_H
}

core_mac {
    DEFINES += \
    HAVE_UNISTD_H
}

INCLUDEPATH += \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/include

SOURCES += \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/ftbbox.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/ftgxval.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/ftlcdfil.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/ftmm.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/ftotval.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/ftpatent.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/ftpfr.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/ftsynth.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/fttype1.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/ftwinfnt.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/ftxf86.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/pcf/pcf.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/pfr/pfr.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/psaux/psaux.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/pshinter/pshinter.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/psnames/psmodule.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/raster/raster.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/sfnt/sfnt.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/truetype/truetype.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/type1/type1.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/cid/type1cid.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/type42/type42.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/winfonts/winfnt.c \
    \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/autofit/autofit.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/bdf/bdf.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/cff/cff.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/ftbase.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/ftbitmap.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/cache/ftcache.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/ftfstype.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/ftgasp.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/ftglyph.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/gzip/ftgzip.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/ftinit.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/lzw/ftlzw.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/ftstroke.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/base/ftsystem.c \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/src/smooth/smooth.c \
    \
    $$DESKTOPEDITOR_PATH/fontengine/ApplicationFonts.cpp \
    $$DESKTOPEDITOR_PATH/fontengine/FontFile.cpp \
    $$DESKTOPEDITOR_PATH/fontengine/FontManager.cpp \
    $$DESKTOPEDITOR_PATH/fontengine/FontPath.cpp \
    $$DESKTOPEDITOR_PATH/fontengine/GlyphString.cpp    

HEADERS += \    
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/include/config/ftconfig.h \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/include/config/ftheader.h \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/include/config/ftmodule.h \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/include/config/ftoption.h \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/include/config/ftstdlib.h \
    $$DESKTOPEDITOR_PATH/freetype-2.5.2/include/ft2build.h \
    \
    $$DESKTOPEDITOR_PATH/fontengine/ApplicationFonts.h \
    $$DESKTOPEDITOR_PATH/fontengine/FontFile.h \
    $$DESKTOPEDITOR_PATH/fontengine/FontManager.h \
    $$DESKTOPEDITOR_PATH/fontengine/FontPath.h \
    $$DESKTOPEDITOR_PATH/fontengine/GlyphString.h 
    
