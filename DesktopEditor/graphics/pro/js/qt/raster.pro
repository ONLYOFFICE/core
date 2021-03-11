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
    ../cimage/CxImage/stdint.h \
    ../cimage/CxImage/xfile.h \
    ../cimage/CxImage/ximabmp.h \
    ../cimage/CxImage/ximacfg.h \
    ../cimage/CxImage/ximadef.h \
    ../cimage/CxImage/ximage.h \
    ../cimage/CxImage/ximagif.h \
    ../cimage/CxImage/ximaico.h \
    ../cimage/CxImage/ximaiter.h \
    ../cimage/CxImage/ximajas.h \
    ../cimage/CxImage/ximajbg.h \
    ../cimage/CxImage/ximajpg.h \
    ../cimage/CxImage/ximamng.h \
    ../cimage/CxImage/ximapcx.h \
    ../cimage/CxImage/ximapng.h \
    ../cimage/CxImage/ximapsd.h \
    ../cimage/CxImage/ximaraw.h \
    ../cimage/CxImage/ximaska.h \
    ../cimage/CxImage/ximatga.h \
    ../cimage/CxImage/ximath.h \
    ../cimage/CxImage/ximatif.h \
    ../cimage/CxImage/ximawbmp.h \
    ../cimage/CxImage/ximawmf.h \
    ../cimage/CxImage/xiofile.h \
    ../cimage/CxImage/xmemfile.h
SOURCES += ../wasm/src/base.cpp \
    ../cimage/CxImage/tif_xfile.cpp \
    ../cimage/CxImage/ximabmp.cpp \
    ../cimage/CxImage/ximadsp.cpp \
    ../cimage/CxImage/ximaenc.cpp \
    ../cimage/CxImage/ximaexif.cpp \
    ../cimage/CxImage/ximage.cpp \
    ../cimage/CxImage/ximagif.cpp \
    ../cimage/CxImage/ximahist.cpp \
    ../cimage/CxImage/ximaico.cpp \
    ../cimage/CxImage/ximainfo.cpp \
    ../cimage/CxImage/ximaint.cpp \
    ../cimage/CxImage/ximajas.cpp \
    ../cimage/CxImage/ximajbg.cpp \
    ../cimage/CxImage/ximajpg.cpp \
    ../cimage/CxImage/ximalpha.cpp \
    ../cimage/CxImage/ximalyr.cpp \
    ../cimage/CxImage/ximamng.cpp \
    ../cimage/CxImage/ximapal.cpp \
    ../cimage/CxImage/ximapcx.cpp \
    ../cimage/CxImage/ximapng.cpp \
    ../cimage/CxImage/ximapsd.cpp \
    ../cimage/CxImage/ximaraw.cpp \
    ../cimage/CxImage/ximasel.cpp \
    ../cimage/CxImage/ximaska.cpp \
    ../cimage/CxImage/ximatga.cpp \
    ../cimage/CxImage/ximath.cpp \
    ../cimage/CxImage/ximatif.cpp \
    ../cimage/CxImage/ximatran.cpp \
    ../cimage/CxImage/ximawbmp.cpp \
    ../cimage/CxImage/ximawmf.cpp \
    ../cimage/CxImage/ximawnd.cpp \
    ../cimage/CxImage/xmemfile.cpp
