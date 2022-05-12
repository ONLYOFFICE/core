QT       -= core gui

VERSION = 1.0.0.4
TARGET = PdfReader
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin
CONFIG += warn_off

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include(../Common/base.pri)

DEFINES += PDFREADER_USE_DYNAMIC_LIBRARY

ADD_DEPENDENCY(graphics, kernel, UnicodeConverter)

core_windows {
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

core_android:DEFINES += ANDROID

INCLUDEPATH += \
    $$PWD/lib/goo \
    $$PWD/lib/fofi \
    $$PWD/lib/splash \
    $$PWD/lib

HEADERS += \
    $$PWD/lib/aconf.h \
    $$$files($$PWD/lib/*.h)

SOURCES += $$files($$PWD/lib/*.c, true)
SOURCES += $$files($$PWD/lib/*.cc, true)

SOURCES -= \
    $$PWD/lib/xpdf/HTMLGen.cc \
    $$PWD/lib/xpdf/pdftohtml.cc \
    $$PWD/lib/xpdf/pdftopng.cc \
    $$PWD/lib/xpdf/pdftoppm.cc \
    $$PWD/lib/xpdf/pdftops.cc \
    $$PWD/lib/xpdf/pdftotext.cc \
    $$PWD/lib/xpdf/pdfdetach.cc \
    $$PWD/lib/xpdf/pdffonts.cc \
    $$PWD/lib/xpdf/pdfimages.cc \
    $$PWD/lib/xpdf/pdfinfo.cc

SOURCES += \
    Src/RendererOutputDev.cpp \
    Src/Adaptors.cpp \
    Src/GfxClip.cpp \
	PdfReader.cpp

HEADERS += \
    Src/RendererOutputDev.h \
    Src/Adaptors.h \
    Src/MemoryUtils.h \
    Src/GfxClip.h \
    PdfReader.h

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
    Resources/BaseFonts.h

SOURCES += \
    Resources/BaseFonts.cpp

core_windows:LIBS += -lOle32

CONFIG += use_external_jpeg2000
use_external_jpeg2000 {
    DEFINES += USE_EXTERNAL_JPEG2000
	CONFIG += use_openjpeg2000
	use_openjpeg2000 {
	    include($$PWD/../DesktopEditor/raster/Jp2/openjpeg/openjpeg.pri)
	} else {
	    DEFINES += USE_GRAPHICS_JPEG2000
	}
	HEADERS += Src/JPXStream2.h
	SOURCES += Src/JPXStream2.cpp
}

#CONFIG += build_viewer_module
build_viewer_module {
    DEFINES += BUILDING_WASM_MODULE
    DEFINES += TEST_AS_EXECUTABLE

    HEADERS += $$CORE_ROOT_DIR/HtmlRenderer/include/HTMLRendererText.h
    SOURCES += $$CORE_ROOT_DIR/HtmlRenderer/src/HTMLRendererText.cpp
}
