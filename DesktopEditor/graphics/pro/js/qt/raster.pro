QT -= core
QT -= gui

VERSION = 0.0.0.1
TARGET = Raster
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin
CONFIG += core_static_link_libstd

DEFINES += RASTER_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

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

SOURCES += \
    ../../../../cximage/jpeg/wrtarga.c \
    ../../../../cximage/jpeg/wrrle.c \
    ../../../../cximage/jpeg/wrppm.c \
    ../../../../cximage/jpeg/wrjpgcom.c \
    ../../../../cximage/jpeg/wrgif.c \
    ../../../../cximage/jpeg/wrbmp.c \
    ../../../../cximage/jpeg/transupp.c \
    ../../../../cximage/jpeg/rdtarga.c \
    ../../../../cximage/jpeg/rdswitch.c \
    ../../../../cximage/jpeg/rdrle.c \
    ../../../../cximage/jpeg/rdppm.c \
    ../../../../cximage/jpeg/rdjpgcom.c \
    ../../../../cximage/jpeg/rdgif.c \
    ../../../../cximage/jpeg/rdcolmap.c \
    ../../../../cximage/jpeg/rdbmp.c \
    ../../../../cximage/jpeg/jutils.c \
    ../../../../cximage/jpeg/jpegtran.c \
    ../../../../cximage/jpeg/jquant1.c \
    ../../../../cximage/jpeg/jquant2.c \
    ../../../../cximage/jpeg/jdpostct.c \
    ../../../../cximage/jpeg/jdsample.c \
    ../../../../cximage/jpeg/jdtrans.c \
    ../../../../cximage/jpeg/jerror.c \
    ../../../../cximage/jpeg/jfdctflt.c \
    ../../../../cximage/jpeg/jfdctfst.c \
    ../../../../cximage/jpeg/jfdctint.c \
    ../../../../cximage/jpeg/jidctflt.c \
    ../../../../cximage/jpeg/jidctfst.c \
    ../../../../cximage/jpeg/jidctint.c \
    #../../../../cximage/jpeg/jmemansi.c \
    ../../../../cximage/jpeg/jmemmgr.c \
    #../../../../cximage/jpeg/jmemname.c \
    ../../../../cximage/jpeg/jmemnobs.c \
    ../../../../cximage/jpeg/jaricom.c \
    ../../../../cximage/jpeg/jcapimin.c \
    ../../../../cximage/jpeg/jcapistd.c \
    ../../../../cximage/jpeg/jcarith.c \
    ../../../../cximage/jpeg/jccoefct.c \
    ../../../../cximage/jpeg/jccolor.c \
    ../../../../cximage/jpeg/jcdctmgr.c \
    ../../../../cximage/jpeg/jchuff.c \
    ../../../../cximage/jpeg/jcinit.c \
    ../../../../cximage/jpeg/jcmainct.c \
    ../../../../cximage/jpeg/jcmarker.c \
    ../../../../cximage/jpeg/jcmaster.c \
    ../../../../cximage/jpeg/jcomapi.c \
    ../../../../cximage/jpeg/jcparam.c \
    ../../../../cximage/jpeg/jcprepct.c \
    ../../../../cximage/jpeg/jcsample.c \
    ../../../../cximage/jpeg/jctrans.c \
    ../../../../cximage/jpeg/jdapimin.c \
    ../../../../cximage/jpeg/jdapistd.c \
    ../../../../cximage/jpeg/jdarith.c \
    ../../../../cximage/jpeg/jdatadst.c \
    ../../../../cximage/jpeg/jdatasrc.c \
    ../../../../cximage/jpeg/jdcoefct.c \
    ../../../../cximage/jpeg/jdcolor.c \
    ../../../../cximage/jpeg/jddctmgr.c \
    ../../../../cximage/jpeg/jdhuff.c \
    ../../../../cximage/jpeg/jdinput.c \
    ../../../../cximage/jpeg/jdmainct.c \
    ../../../../cximage/jpeg/jdmarker.c \
    ../../../../cximage/jpeg/jdmaster.c \
    ../../../../cximage/jpeg/jdmerge.c \
    ../../../../cximage/jpeg/cdjpeg.c \
    ../../../../cximage/jpeg/cjpeg.c \
    ../../../../cximage/jpeg/ckconfig.c \
    ../../../../cximage/jpeg/djpeg.c \
    ../../../../cximage/jpeg/jdmainct.c

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

HEADERS += \
    ../../../../cximage/jpeg/cderror.h \
    ../../../../cximage/jpeg/cdjpeg.h \
    ../../../../cximage/jpeg/jconfig.h \
    ../../../../cximage/jpeg/jdct.h \
    ../../../../cximage/jpeg/jerror.h \
    ../../../../cximage/jpeg/jinclude.h \
    ../../../../cximage/jpeg/jmemsys.h \
    ../../../../cximage/jpeg/jmorecfg.h \
    ../../../../cximage/jpeg/jpegint.h \
    ../../../../cximage/jpeg/jpeglib.h \
    ../../../../cximage/jpeg/jversion.h \
    ../../../../cximage/jpeg/transupp.h
