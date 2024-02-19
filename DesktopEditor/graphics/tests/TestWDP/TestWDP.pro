#CONFIG += c++11 cmdline

#SOURCES += \
QT       -= core

QT       -= gui

TARGET = test
CONFIG   += console
TEMPLATE = app

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

ADD_DEPENDENCY(kernel, graphics, UnicodeConverter)

GRAPHICS_AGG_PATH = $$PWD/../../../agg-2.4

INCLUDEPATH += \
    $$GRAPHICS_AGG_PATH/include

SOURCES += main.cpp \
    ../../../raster/JpegXR/systems/tools/wmpgluelib/WMPGlue.c \
	../../../raster/JpegXR/systems/tools/wmpgluelib/WMPGlueBmp.c \
	../../../raster/JpegXR/systems/tools/wmpgluelib/WMPGlueHdr.c \
	../../../raster/JpegXR/systems/tools/wmpgluelib/WMPGluePFC.c \
	../../../raster/JpegXR/systems/tools/wmpgluelib/WMPGluePnm.c \
	../../../raster/JpegXR/systems/tools/wmpgluelib/WMPGlueTif.c \
	../../../raster/JpegXR/systems/tools/wmpgluelib/WMPGlueWmp.c \
	../../../raster/JpegXR/systems/tools/wmpgluelib/WMPGlueYUV.c \
#	../../../raster/JpegXR/systems/tools/samplecodenew/WMPDecApp.c \
#	../../../raster/JpegXR/systems/tools/samplecodenew/WMPEncApp.c \
    ../../../raster/JpegXR/systems/tools/WMPEncDLL/stdafx.cpp \
	../../../raster/JpegXR/systems/tools/wmpmetalib/WMPMeta.c \
	../../../raster/JpegXR/image/decode/decode.c \
	../../../raster/JpegXR/image/decode/postprocess.c \
	../../../raster/JpegXR/image/decode/segdec.c \
	../../../raster/JpegXR/image/decode/strInvTransform.c \
	../../../raster/JpegXR/image/decode/strPredQuantDec.c \
	../../../raster/JpegXR/image/decode/strdec.c \
	../../../raster/JpegXR/image/decode/huffman.c \
	../../../raster/JpegXR/image/decode/WMPTranscode.c \
	../../../raster/JpegXR/image/encode/encode.c \
	../../../raster/JpegXR/image/encode/segenc.c \
	../../../raster/JpegXR/image/encode/strFwdTransform.c \
	../../../raster/JpegXR/image/encode/strPredQuantEnc.c \
	../../../raster/JpegXR/image/encode/strenc.c \
	../../../raster/JpegXR/image/sys/adapthuff.c \
	../../../raster/JpegXR/image/sys/image.c \
	../../../raster/JpegXR/image/sys/perfTimerANSI.c \
	../../../raster/JpegXR/image/sys/strPredQuant.c \
	../../../raster/JpegXR/image/sys/strTransform.c \
	../../../raster/JpegXR/image/sys/strcodec.c \
	../../../raster/JpegXR/image/sys/perfTimerWin32.c \
	../../../raster/JpegXR/image/x86/strenc_x86.c \


DESTDIR = $$PWD_ROOT_DIR/build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX

DISTFILES +=

HEADERS += \
    ../../../raster/JpegXR/systems/tools/wmpgluelib/WMPGlue.h \
	../../../raster/JpegXR/systems/tools/wmpmetalib/WMPMeta.h \
	../../../raster/JpegXR/systems/tools/WMPEncDLL/stdafx.h \
	../../../raster/JpegXR/systems/tools/WMPEncDLL/targetver.h \
	../../../raster/JpegXR/systems/tools/WMPEncDLL/WMPEncDLL.h \
	../../../raster/JpegXR/image/decode/decode.h \
	../../../raster/JpegXR/image/encode/encode.h \
	../../../raster/JpegXR/image/sys/ansi.h \
	../../../raster/JpegXR/image/sys/common.h \
	../../../raster/JpegXR/image/sys/perfTimer.h \
	../../../raster/JpegXR/image/sys/strTransform.h \
	../../../raster/JpegXR/image/sys/strcodec.h \
	../../../raster/JpegXR/image/sys/windowsmediaphoto.h \
	../../../raster/JpegXR/image/sys/xplatform_image.h \
	../../../raster/JpegXR/common/include/guiddef.h \
	../../../raster/JpegXR/common/include/wmsal.h \
	../../../raster/JpegXR/common/include/wmspecstring.h \
	../../../raster/JpegXR/common/include/wmspecstrings_adt.h \
	../../../raster/JpegXR/common/include/wmspecstrings_strict.h \
	../../../raster/JpegXR/image/x86/x86.h \
