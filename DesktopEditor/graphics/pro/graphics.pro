QT       -= core gui

TARGET = graphics
TEMPLATE = lib

CONFIG += graphics_dynamic_library

DEFINES += _QT
graphics_dynamic_library {
    CONFIG += shared
    CONFIG += plugin

    DEFINES += GRAPHICS_USE_DYNAMIC_LIBRARY_BUILDING
} else {
    DEFINES += GRAPHICS_NO_USE_DYNAMIC_LIBRARY
    CONFIG += static
}

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD
include(../../../Common/base.pri)

ADD_DEPENDENCY(UnicodeConverter, kernel)

core_windows {
    LIBS += -lAdvapi32
	LIBS += -lShell32
}

HEADERS += ./../config.h

GRAPHICS_AGG_PATH = $$PWD/../../agg-2.4

INCLUDEPATH += \
    $$GRAPHICS_AGG_PATH/include

# matrix
HEADERS += \
    ./../Matrix_private.h \
	./../Matrix.h

SOURCES += \
    ./../Matrix.cpp

SOURCES += \
    $$GRAPHICS_AGG_PATH/src/agg_trans_affine.cpp

# paths
HEADERS += \
    ./../GraphicsPath_private.h \
	./../GraphicsPath.h

SOURCES += \
    ./../GraphicsPath.cpp

SOURCES += \
    $$GRAPHICS_AGG_PATH/src/agg_arc.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_bezier_arc.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_curves.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_bspline.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_vcgen_bspline.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_vcgen_stroke.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_vcgen_contour.cpp

include(raster.pri)

#CONFIG += graphics_disable_metafile
graphics_disable_metafile {
    DEFINES += GRAPHICS_DISABLE_METAFILE
} else {
    include(metafile.pri)
}

CONFIG += support_font_converter
include(fontengine.pri)

# drawing file
HEADERS += ./officedrawingfile.h
SOURCES += ./officedrawingfile.cpp

# graphics
SOURCES += \
    $$GRAPHICS_AGG_PATH/src/agg_arrowhead.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_image_filters.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_line_aa_basics.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_line_profile_aa.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_vcgen_dash.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_vcgen_markers_term.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_vcgen_smooth_poly1.cpp

HEADERS += \
    ./../ArrowHead.h \
	./../Brush.h \
	./../Clip.h \
	./../Color.h \
	./../Defines.h \
	./../Graphics.h \
	./../ImageFilesCache.h \
	./../MetafileToRenderer.h \
	./../MetafileToRendererCheck.h \
	./../MetafileToGraphicsRenderer.h \
	./../FormField.h \
	./../structures.h \
	./../Graphics.h \
	./../GraphicsRenderer.h \
	\
	./Graphics.h \
	./Image.h \

SOURCES += \
    ./../ArrowHead.cpp \
	./../Brush.cpp \
	./../Clip.cpp \
	./../Graphics.cpp \
	./../GraphicsRenderer.cpp \
	./../MetafileToRenderer.cpp \
	./../MetafileToGraphicsRenderer.cpp \
	./../FormField.cpp \
	\
	./pro_Image.cpp \
	./pro_Graphics.cpp
