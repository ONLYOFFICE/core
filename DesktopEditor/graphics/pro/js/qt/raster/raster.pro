QT -= core
QT -= gui

VERSION = 0.0.0.1
TARGET = test

TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle

CORE_ROOT_DIR = $$PWD/../../../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

DEFINES += GRAPHICS_NO_USE_DYNAMIC_LIBRARY
DEFINES += KERNEL_NO_USE_DYNAMIC_LIBRARY
DEFINES -= UNICODE _UNICODE

#DEFINES += BUILDING_WASM_MODULE

CONFIG += disable_cximage_mng
CONFIG += disable_cximage_all
include(../../../raster.pri)

#CONFIG += graphics_disable_metafile
graphics_disable_metafile {
    DEFINES += GRAPHICS_DISABLE_METAFILE
} else {
    CONFIG += metafile_disable_svg
	CONFIG += metafile_disable_svm
	CONFIG += metafile_disable_wmf_emf_xml
	include(../../../metafile.pri)

    !metafile_disable_svg {
	    CONFIG += enable_libxml
	}
	!metafile_disable_svm {
	    CONFIG += enable_libxml
		SOURCES += $$CORE_ROOT_DIR/UnicodeConverter/UnicodeConverter_internal_empty.cpp
	}

    enable_libxml:include($$CORE_ROOT_DIR/DesktopEditor/xml/build/qt/libxml2.pri)

    !metafile_disable_wmf_emf {
	    SOURCES += $$CORE_ROOT_DIR/DesktopEditor/xml/src/xmlwriter.cpp
	}
}

CONFIG += graphics_disable_fonts
graphics_disable_fonts {
    SOURCES += $$PWD/pro_Fonts_empty.cpp
} else {
    include(../../../fontengine.pri)
}

GRAPHICS_PATH = $$CORE_ROOT_DIR/DesktopEditor/graphics
GRAPHICS_AGG_PATH = $$CORE_ROOT_DIR/DesktopEditor/agg-2.4
GRAPHICS_COMMON = $$CORE_ROOT_DIR/DesktopEditor/common
# matrix
HEADERS += \
    $$GRAPHICS_PATH/Matrix_private.h \
	$$GRAPHICS_PATH/Matrix.h

SOURCES += \
    $$GRAPHICS_PATH/Matrix.cpp

SOURCES += \
    $$GRAPHICS_AGG_PATH/src/agg_trans_affine.cpp

graphics_enable_path {
    # paths
	HEADERS += \
	    $$GRAPHICS_PATH/GraphicsPath_private.h \
		$$GRAPHICS_PATH/GraphicsPath.h

    SOURCES += \
	    $$GRAPHICS_PATH/GraphicsPath.cpp

    SOURCES += \
	    $$GRAPHICS_AGG_PATH/src/agg_arc.cpp \
		$$GRAPHICS_AGG_PATH/src/agg_bezier_arc.cpp \
		$$GRAPHICS_AGG_PATH/src/agg_curves.cpp \
		$$GRAPHICS_AGG_PATH/src/agg_bspline.cpp \
		$$GRAPHICS_AGG_PATH/src/agg_vcgen_bspline.cpp \
		$$GRAPHICS_AGG_PATH/src/agg_vcgen_stroke.cpp \
		$$GRAPHICS_AGG_PATH/src/agg_vcgen_contour.cpp
}

INCLUDEPATH += \
    $$GRAPHICS_AGG_PATH/include

SOURCES += \
    $$GRAPHICS_COMMON/File.cpp \
	$$GRAPHICS_COMMON/Base64.cpp \
	$$GRAPHICS_COMMON/StringBuilder.cpp \
	$$GRAPHICS_COMMON/StringExt.cpp

SOURCES += $$PWD/pro_Graphics_empty.cpp

SOURCES += ./raster.cpp
HEADERS += ./raster.h

SOURCES += ./main.cpp
