QT -= core
QT -= gui

TARGET = raster
TEMPLATE = lib

CONFIG += raster_dynamic_library

graphics_dynamic_library {
    CONFIG += shared
    CONFIG += plugin
} else {
    CONFIG += static
}

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

LIB_GRAPHICS_PRI_PATH = $$PWD/../../../..

HEADERS += ../wasm/src/base.h \
#    ../cimage/CxImage/stdint.h \
    ../cimage/CxImage/xfile.h \
#    ../cimage/CxImage/ximabmp.h \
    ../cimage/CxImage/ximacfg.h \
    ../cimage/CxImage/ximadef.h \
    ../cimage/CxImage/ximage.h \
#    ../cimage/CxImage/ximagif.h \
#    ../cimage/CxImage/ximaico.h \
    ../cimage/CxImage/ximaiter.h \
#    ../cimage/CxImage/ximajas.h \
#    ../cimage/CxImage/ximajbg.h \
    ../cimage/CxImage/ximajpg.h \
#    ../cimage/CxImage/ximamng.h \
#    ../cimage/CxImage/ximapcx.h \
#    ../cimage/CxImage/ximapng.h \
#    ../cimage/CxImage/ximapsd.h \
#    ../cimage/CxImage/ximaraw.h \
#    ../cimage/CxImage/ximaska.h \
#    ../cimage/CxImage/ximatga.h \
#    ../cimage/CxImage/ximath.h \
#    ../cimage/CxImage/ximatif.h \
#    ../cimage/CxImage/ximawbmp.h \
#    ../cimage/CxImage/ximawmf.h \
#    ../cimage/CxImage/xiofile.h \
    ../cimage/CxImage/xmemfile.h

SOURCES += ../wasm/src/base.cpp \
#    ../cimage/CxImage/tif_xfile.cpp \
#    ../cimage/CxImage/ximabmp.cpp \
#    ../cimage/CxImage/ximadsp.cpp \
    ../cimage/CxImage/ximaenc.cpp \
    ../cimage/CxImage/ximaexif.cpp \
    ../cimage/CxImage/ximage.cpp \
#    ../cimage/CxImage/ximagif.cpp \
#    ../cimage/CxImage/ximahist.cpp \
#    ../cimage/CxImage/ximaico.cpp \
    ../cimage/CxImage/ximainfo.cpp \
#    ../cimage/CxImage/ximaint.cpp \
#    ../cimage/CxImage/ximajas.cpp \
#    ../cimage/CxImage/ximajbg.cpp \
    ../cimage/CxImage/ximajpg.cpp \
    ../cimage/CxImage/ximalpha.cpp \
#    ../cimage/CxImage/ximalyr.cpp \
#    ../cimage/CxImage/ximamng.cpp \
    ../cimage/CxImage/ximapal.cpp \
#    ../cimage/CxImage/ximapcx.cpp \
#    ../cimage/CxImage/ximapng.cpp \
#    ../cimage/CxImage/ximapsd.cpp \
#    ../cimage/CxImage/ximaraw.cpp \
    ../cimage/CxImage/ximasel.cpp \
#    ../cimage/CxImage/ximaska.cpp \
#    ../cimage/CxImage/ximatga.cpp \
#    ../cimage/CxImage/ximath.cpp \
#    ../cimage/CxImage/ximatif.cpp \
#    ../cimage/CxImage/ximatran.cpp \
#    ../cimage/CxImage/ximawbmp.cpp \
#    ../cimage/CxImage/ximawmf.cpp \
#    ../cimage/CxImage/ximawnd.cpp \
    ../cimage/CxImage/xmemfile.cpp

HEADERS += \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/cderror.h \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/cdjpeg.h \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jconfig.h \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jdct.h \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jerror.h \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jinclude.h \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jmemsys.h \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jmorecfg.h \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jpegint.h \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jpeglib.h \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jversion.h \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/transupp.h

SOURCES += \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/wrtarga.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/wrrle.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/wrppm.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/wrjpgcom.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/wrgif.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/wrbmp.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/transupp.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/rdtarga.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/rdswitch.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/rdrle.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/rdppm.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/rdjpgcom.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/rdgif.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/rdcolmap.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/rdbmp.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jutils.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jpegtran.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jquant1.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jquant2.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jdpostct.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jdsample.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jdtrans.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jerror.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jfdctflt.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jfdctfst.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jfdctint.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jidctflt.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jidctfst.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jidctint.c \
    #$$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jmemansi.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jmemmgr.c \
    #$$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jmemname.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jmemnobs.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jaricom.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jcapimin.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jcapistd.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jcarith.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jccoefct.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jccolor.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jcdctmgr.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jchuff.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jcinit.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jcmainct.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jcmarker.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jcmaster.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jcomapi.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jcparam.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jcprepct.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jcsample.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jctrans.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jdapimin.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jdapistd.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jdarith.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jdatadst.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jdatasrc.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jdcoefct.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jdcolor.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jddctmgr.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jdhuff.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jdinput.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jdmainct.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jdmarker.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jdmaster.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jdmerge.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/cdjpeg.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/cjpeg.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/ckconfig.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/djpeg.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/jdmainct.c
