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

#CONFIG += disable_fonts_engine
#CONFIG += disable_graphics_renderer
#CONFIG += disable_metafile_svg
#CONFIG += disable_metafile_wmf_emf
#CONFIG += disable_metafile_svm
#CONFIG += disable_raster

disable_fonts_engine:CONFIG += disable_graphics_renderer

!disable_raster {
    include(./raster_module.pri)
}

!disable_fonts_engine {
    include(./fonts_module.pri)
}

LIB_GRAPHICS_PRI_PATH = $$PWD/../../../../..

INCLUDEPATH += \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/include

include($$CORE_ROOT_DIR/DesktopEditor/xml/build/qt/libxml2.pri)

SOURCES += \
    $$CORE_ROOT_DIR/DesktopEditor/common/Directory.cpp \
	$$CORE_ROOT_DIR/DesktopEditor/common/Base64.cpp \
	$$CORE_ROOT_DIR/DesktopEditor/common/StringBuilder.cpp \
	$$CORE_ROOT_DIR/DesktopEditor/common/StringExt.cpp \
	$$CORE_ROOT_DIR/DesktopEditor/common/Path.cpp

SOURCES += \
    $$LIB_GRAPHICS_PRI_PATH/graphics/Matrix.cpp \
	$$LIB_GRAPHICS_PRI_PATH/graphics/GraphicsPath.cpp

HEADERS += \
    $$LIB_GRAPHICS_PRI_PATH/raster/Metafile/CXmlOutput.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Common/CPathConverter.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Common/IOutputDevice.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Common/MetaFile.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Common/MetaFileClip.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Common/MetaFileObjects.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Common/MetaFileRenderer.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Common/MetaFileTypes.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Common/MetaFileUtils.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfInterpretator/CEmfInterpretator.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfInterpretator/CEmfInterpretatorArray.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfInterpretator/CEmfInterpretatorBase.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfInterpretator/CEmfInterpretatorRender.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfInterpretator/CEmfInterpretatorSvg.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfInterpretator/CEmfInterpretatorXml.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfParser/CEmfParser.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfParser/CEmfParserBase.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfParser/CEmfPlusParser.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfParser/CEmfxParser.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Wmf/WmfInterpretator/CWmfInterpretatorSvg.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Wmf/WmfParser/CWmfParser.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Wmf/WmfParser/CWmfParserBase.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Wmf/WmfInterpretator/CWmfInterpretatorBase.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Wmf/WmfInterpretator/CWmfInterpretatorRender.h \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Wmf/WmfInterpretator/CWmfInterpretator.h

SOURCES += \
    $$LIB_GRAPHICS_PRI_PATH/raster/Metafile/CXmlOutput.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Common/CPathConverter.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Common/MetaFileTypes.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Common/MetaFileUtils.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfInterpretator/CEmfInterpretator.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfInterpretator/CEmfInterpretatorArray.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfInterpretator/CEmfInterpretatorRender.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfInterpretator/CEmfInterpretatorSvg.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfInterpretator/CEmfInterpretatorXml.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfParser/CEmfParser.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfParser/CEmfParserBase.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfParser/CEmfPlusParser.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfParser/CEmfxParser.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Wmf/WmfInterpretator/CWmfInterpretatorSvg.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Wmf/WmfParser/CWmfParser.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Wmf/WmfParser/CWmfParserBase.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Wmf/WmfInterpretator/CWmfInterpretatorRender.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Wmf/WmfInterpretator/CWmfInterpretator.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/MetaFile.cpp \
	\
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfClip.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfObjects.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfPath.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfPlayer.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Emf/EmfFile.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Wmf/WmfClip.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Wmf/WmfObjects.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/Wmf/WmfPlayer.cpp \
	\
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/StarView/SvmClip.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/StarView/SvmFile.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/StarView/SvmObjects.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/StarView/SvmPlayer.cpp \
	\
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/svg/SVGFramework.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/Metafile/svg/SVGTransformer.cpp

SOURCES += \
    $$CORE_ROOT_DIR/UnicodeConverter/UnicodeConverter_internal_empty.cpp

SOURCES += \
    $$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/agg_arc.cpp \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/agg_bezier_arc.cpp \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/agg_arrowhead.cpp \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/ctrl/agg_cbox_ctrl.cpp \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/agg_curves.cpp \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/agg_gsv_text.cpp \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/agg_image_filters.cpp \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/agg_line_aa_basics.cpp \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/agg_line_profile_aa.cpp \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/agg_rounded_rect.cpp \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/agg_sqrt_tables.cpp \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/agg_trans_affine.cpp \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/agg_bspline.cpp \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/agg_vcgen_bspline.cpp \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/agg_vcgen_contour.cpp \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/agg_vcgen_dash.cpp \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/agg_vcgen_markers_term.cpp \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/agg_vcgen_smooth_poly1.cpp \
	$$LIB_GRAPHICS_PRI_PATH/agg-2.4/src/agg_vcgen_stroke.cpp

SOURCES += \
    $$CORE_ROOT_DIR/OfficeUtils/src/OfficeUtils.cpp \
	$$CORE_ROOT_DIR/OfficeUtils/src/ZipBuffer.cpp \
	$$CORE_ROOT_DIR/OfficeUtils/src/ZipUtilsCP.cpp

SOURCES += \
    ../../../pro_base.cpp \
	../../../pro_Fonts_empty.cpp \
	../../../pro_Graphics_empty.cpp

SOURCES += ./raster.cpp
HEADERS += ./raster.h

core_windows {
    LIBS += -lRpcrt4
	LIBS += -lShell32
}
