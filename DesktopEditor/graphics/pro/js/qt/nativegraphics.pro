QT -= core
QT -= gui

TARGET = wasmgraphics
TEMPLATE = app

CONFIG += console
CONFIG += object_parallel_to_source
CONFIG -= app_bundle

DEFINES += TEST_CPP_BINARY \
	GRAPHICS_NO_USE_DYNAMIC_LIBRARY \
	BUILDING_WASM_MODULE \
	_QT

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
include(../../freetype.pri)

ADD_DEPENDENCY(UnicodeConverter, kernel)

INCLUDEPATH += \
	$$CORE_ROOT_DIR/DesktopEditor/agg-2.4/include \
	$$CORE_ROOT_DIR/DesktopEditor/cximage/jasper/include \
	$$CORE_ROOT_DIR/DesktopEditor/cximage/jpeg \
	$$CORE_ROOT_DIR/DesktopEditor/cximage/png \
	$$CORE_ROOT_DIR/DesktopEditor/freetype-2.10.4/include \
	$$CORE_ROOT_DIR/DesktopEditor/freetype-2.10.4/include/freetype \
	$$CORE_ROOT_DIR/Common/3dParty/openssl/openssl/include

win32 {
DEFINES += \
	JAS_WIN_MSVC_BUILD \
	NOMINMAX

DEFINES -= UNICODE
DEFINES -= _UNICODE

LIBS += -lgdi32 \
		-ladvapi32 \
		-luser32 \
		-lshell32 \
		-lOle32
}

# graphics

HEADERS += \
	../../../config.h \
	\
	../../../Matrix.h \
	../../../Matrix_private.h \
	../../../GraphicsPath.h \
	../../../BooleanOperations.h \
	../../../boolean_operations_math.h \
	../../../GraphicsPath_private.h \
	../../../AlphaMask.h \
	\
	../../../../raster/BgraFrame.h \
	../../../../raster/ImageFileFormatChecker.h \
	../../../../raster/Metafile/Metafile.h \
	../../../../raster/Metafile/Common/MetaFile.h \
	../../../../raster/Metafile/Common/IOutputDevice.h \
	../../../../raster/Metafile/Common/MetaFileTypes.h \
	../../../../raster/Metafile/Common/MetaFileClip.h \
	../../../../raster/Metafile/Common/MetaFileObjects.h \
	../../../../raster/Metafile/Common/MetaFileRenderer.h \
	../../../../raster/Metafile/Common/MetaFileUtils.h \
	../../../../raster/PICT/PICTFile.h \
	\
	../../../ArrowHead.h \
	../../../Brush.h \
	../../../Clip.h \
	../../../Color.h \
	../../../Defines.h \
	../../../Graphics.h \
	../../../Image.h \
	../../../ImageFilesCache.h \
	../../../structures.h \
	../../../shading_info.h \
	../../../GraphicsRenderer.h \
	../../../GraphicsLayer.h \
	\
	../../../../fontengine/ApplicationFonts.h \
	../../../../fontengine/FontFile.h \
	../../../../fontengine/FontPath.h \
	../../../../fontengine/GlyphString.h \
	../../../../fontengine/FontManager.h \
	../../../../fontengine/FontConverter.h \
	\
	../../Fonts.h \
	../../Graphics.h \
	../../Image.h \
	../../../../raster/Metafile/svg/SVGFramework.h \
	../../../../raster/Metafile/svg/SVGTransformer.h \
	\
	../../../MetafileToRenderer.h \
	../../../MetafileToRendererCheck.h \
	../../../MetafileToRendererReader.h \
	../../../MetafileToGraphicsRenderer.h \
	../../../commands/FormField.h \
	../../../commands/AnnotField.h \
	../../../commands/DocInfo.h

SOURCES += \
	../../../Matrix.cpp \
	../../../GraphicsPath.cpp \
	../../../BooleanOperations.cpp \
	../../../AlphaMask.cpp \
	../../../../raster/BgraFrame.cpp \
	../../../../raster/ImageFileFormatChecker.cpp \
	../../../../raster/Metafile/MetaFile.cpp \
	../../../../raster/PICT/PICFile.cpp \
	../../../../raster/PICT/pic.cpp \
	\
	../../../ArrowHead.cpp \
	../../../Brush.cpp \
	../../../Clip.cpp \
	../../../Graphics.cpp \
	../../../GraphicsRenderer.cpp \
	../../../Image.cpp \
	../../../GraphicsLayer.cpp \
	\
	../../../../fontengine/ApplicationFonts.cpp \
	../../../../fontengine/FontFile.cpp \
	../../../../fontengine/FontManager.cpp \
	../../../../fontengine/FontPath.cpp \
	../../../../fontengine/GlyphString.cpp \
	\
	../../pro_Fonts.cpp \
	../../pro_Image.cpp \
	../../pro_Graphics.cpp \
	../../../../raster/Metafile/svg/SVGFramework.cpp \
	../../../../raster/Metafile/svg/SVGTransformer.cpp \
	\
	../../../MetafileToRenderer.cpp \
	../../../MetafileToRendererReader.cpp \
	../../../MetafileToGraphicsRenderer.cpp \
	../../../commands/FormField.cpp \
	../../../commands/AnnotField.cpp \
	../../../commands/DocInfo.cpp

SOURCES += \
	../../../../agg-2.4/src/agg_arc.cpp \
	../../../../agg-2.4/src/agg_bezier_arc.cpp \
	../../../../agg-2.4/src/agg_arrowhead.cpp \
	../../../../agg-2.4/src/ctrl/agg_cbox_ctrl.cpp \
	../../../../agg-2.4/src/agg_curves.cpp \
	../../../../agg-2.4/src/agg_gsv_text.cpp \
	../../../../agg-2.4/src/agg_image_filters.cpp \
	../../../../agg-2.4/src/agg_line_aa_basics.cpp \
	../../../../agg-2.4/src/agg_line_profile_aa.cpp \
	../../../../agg-2.4/src/agg_rounded_rect.cpp \
	../../../../agg-2.4/src/agg_sqrt_tables.cpp \
	../../../../agg-2.4/src/agg_trans_affine.cpp \
	../../../../agg-2.4/src/agg_bspline.cpp \
	../../../../agg-2.4/src/agg_vcgen_bspline.cpp \
	../../../../agg-2.4/src/agg_vcgen_contour.cpp \
	../../../../agg-2.4/src/agg_vcgen_dash.cpp \
	../../../../agg-2.4/src/agg_vcgen_markers_term.cpp \
	../../../../agg-2.4/src/agg_vcgen_smooth_poly1.cpp \
	../../../../agg-2.4/src/agg_vcgen_stroke.cpp \
	\
	../../../../raster/Jp2/J2kFile.cpp \
	../../../../raster/Jp2/Reader.cpp \
	\
	../../../../raster/Metafile/Common/MetaFileTypes.cpp \
	../../../../raster/Metafile/Common/MetaFileUtils.cpp \
	\
	../../../../raster/JBig2/source/JBig2File.cpp \
	\
	../../../../raster/Metafile/StarView/SvmFile.cpp \
	../../../../raster/Metafile/StarView/SvmObjects.cpp \
	../../../../raster/Metafile/StarView/SvmPlayer.cpp

LIB_GRAPHICS_PRI_PATH = $$PWD/../../../..
SOURCES += \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/base/jas_cm.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/base/jas_debug.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/base/jas_getopt.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/base/jas_icc.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/base/jas_iccdata.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/base/jas_image.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/base/jas_init.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/base/jas_malloc.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/base/jas_stream.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/base/jas_seq.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/base/jas_string.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/base/jas_tvp.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/base/jas_version.c \
	\
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/bmp/bmp_cod.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/bmp/bmp_dec.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/bmp/bmp_enc.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jp2/jp2_cod.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jp2/jp2_dec.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jp2/jp2_enc.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_bs.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_cs.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_dec.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_enc.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_math.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_mct.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_mqcod.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_mqdec.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_mqenc.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_qmfb.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_t1cod.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_t1dec.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_t1enc.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_t2cod.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_t2dec.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_t2enc.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_tagtree.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_tsfb.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpc/jpc_util.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpg/jpg_dummy.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/jpg/jpg_val.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/mif/mif_cod.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/pgx/pgx_cod.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/pgx/pgx_dec.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/pgx/pgx_enc.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/pnm/pnm_cod.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/pnm/pnm_dec.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/pnm/pnm_enc.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/ras/ras_cod.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/ras/ras_dec.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/ras/ras_enc.c \
	\
	$$LIB_GRAPHICS_PRI_PATH/cximage/jbig/jbig.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jbig/jbig_tab.c \
	\
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
	$$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/djpeg.c

SOURCES += ../../libpsd_pri.c
SOURCES += ../../libpsd_pri2.c
SOURCES += ../../libpsd_pri3.c

SOURCES += \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_callback_xs.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_chunk_descr.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_chunk_io.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_chunk_prc.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_chunk_xs.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_cms.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_display.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_dither.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_error.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_filter.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_hlapi.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_jpeg.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_object_prc.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_pixels.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_prop_xs.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_read.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_trace.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_write.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_zlib.c \
	\
	$$LIB_GRAPHICS_PRI_PATH/cximage/png/png.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/png/pngerror.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/png/pngget.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/png/pngmem.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/png/pngpread.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/png/pngread.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/png/pngrio.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/png/pngrtran.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/png/pngrutil.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/png/pngset.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/png/pngtrans.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/png/pngwio.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/png/pngwrite.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/png/pngwtran.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/png/pngwutil.c \
	\
	$$LIB_GRAPHICS_PRI_PATH/cximage/raw/libdcr.c \
	\
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_stream.cxx \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_aux.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_close.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_codec.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_color.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_compress.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_dir.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_dirinfo.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_dirread.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_dirwrite.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_dumpmode.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_error.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_extension.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_fax3.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_fax3sm.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_flush.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_getimage.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_jbig.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_jpeg.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_luv.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_lzw.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_next.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_ojpeg.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_open.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_packbits.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_pixarlog.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_predict.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_print.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_read.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_strip.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_swab.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_thunder.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_tile.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_unix.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_version.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_warning.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_write.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/tiff/tif_zip.c \
	\
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/tif_xfile.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximabmp.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximadsp.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximaenc.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximaexif.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximage.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximagif.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximahist.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximaico.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximainfo.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximaint.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximajas.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximajbg.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximajpg.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximalpha.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximalyr.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximamng.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximapal.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximapcx.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximapng.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximapsd.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximaraw.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximasel.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximaska.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximatga.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximath.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximatif.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximatran.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximawbmp.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximawmf.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/ximawnd.cpp \
	$$LIB_GRAPHICS_PRI_PATH/cximage/CxImage/xmemfile.cpp

SOURCES += \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/Encoder/jbig2arith.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/Encoder/jbig2enc.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/Encoder/jbig2sym.cpp

SOURCES += ../../lepton_lib_all.cpp

SOURCES += \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/boxbasic.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/ccbord.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/dwacomb.2.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/dwacomblow.2.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/fhmtgen.1.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/fliphmtgen.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/fmorphauto.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/fmorphgen.1.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/numabasic.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/pix5.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/pixabasic.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/pixafunc1.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/pixcomp.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/ptabasic.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/ptra.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/ropiplow.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/roplow.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/rotateam.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/rotateshear.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/sarray.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/sel1.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/sel2.cpp \
	$$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/LeptonLib/skew.cpp

HEADERS += \
	../../../../fontengine/ApplicationFontsWorker.h \
	../../../../fontengine/FontsAssistant.h \
	../../officedrawingfile.h


SOURCES += \
	../../../../fontengine/ApplicationFontsWorker.cpp \
	../../../../fontengine/FontsAssistant.cpp \
	../../officedrawingfile.cpp

# XpsFile

XPS_ROOT_DIR = $$CORE_ROOT_DIR/XpsFile

HEADERS += \
	$$XPS_ROOT_DIR/XpsFile.h \
	$$XPS_ROOT_DIR/XpsLib/ContextState.h \
	$$XPS_ROOT_DIR/XpsLib/Document.h \
	$$XPS_ROOT_DIR/XpsLib/FontList.h \
	$$XPS_ROOT_DIR/XpsLib/XpsPage.h \
	$$XPS_ROOT_DIR/XpsLib/StaticResources.h \
	$$XPS_ROOT_DIR/XpsLib/Utils.h \
	$$XPS_ROOT_DIR/XpsLib/WString.h

SOURCES += \
	$$XPS_ROOT_DIR/XpsFile.cpp \
	$$XPS_ROOT_DIR/XpsLib/ContextState.cpp \
	$$XPS_ROOT_DIR/XpsLib/Document.cpp \
	$$XPS_ROOT_DIR/XpsLib/XpsPage.cpp \
	$$XPS_ROOT_DIR/XpsLib/StaticResources.cpp \
	$$XPS_ROOT_DIR/XpsLib/Utils.cpp \
	$$XPS_ROOT_DIR/XpsLib/WString.cpp

# DjVuFile

DEFINES += \
	THREADMODEL=0 \
	DEBUGLVL=0

DJVU_ROOT_DIR = $$CORE_ROOT_DIR/DjVuFile
DJVU_WRAPPER  = $$CORE_ROOT_DIR/DjVuFile/wasm/libdjvu

HEADERS += \
	$$DJVU_ROOT_DIR/DjVu.h \
	$$DJVU_ROOT_DIR/DjVuFileImplementation.h \
	$$DJVU_ROOT_DIR/libdjvu/Arrays.h \
	$$DJVU_ROOT_DIR/libdjvu/atomic.h \
	$$DJVU_ROOT_DIR/libdjvu/BSByteStream.h \
	$$DJVU_ROOT_DIR/libdjvu/ByteStream.h \
	$$DJVU_ROOT_DIR/libdjvu/DataPool.h \
	$$DJVU_ROOT_DIR/libdjvu/debug.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVmDir.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVmDir0.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVmDoc.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVmNav.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVuAnno.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVuDocEditor.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVuDocument.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVuDumpHelper.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVuErrorList.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVuFile.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVuFileCache.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVuGlobal.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVuImage.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVuInfo.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVuMessage.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVuMessageLite.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVuNavDir.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVuPalette.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVuPort.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVuText.h \
	$$DJVU_ROOT_DIR/libdjvu/DjVuToPS.h \
	$$DJVU_ROOT_DIR/libdjvu/GBitmap.h \
	$$DJVU_ROOT_DIR/libdjvu/GContainer.h \
	$$DJVU_ROOT_DIR/libdjvu/GException.h \
	$$DJVU_ROOT_DIR/libdjvu/GIFFManager.h \
	$$DJVU_ROOT_DIR/libdjvu/GMapAreas.h \
	$$DJVU_ROOT_DIR/libdjvu/GOS.h \
	$$DJVU_ROOT_DIR/libdjvu/GPixmap.h \
	$$DJVU_ROOT_DIR/libdjvu/GRect.h \
	$$DJVU_ROOT_DIR/libdjvu/GScaler.h \
	$$DJVU_ROOT_DIR/libdjvu/GSmartPointer.h \
	$$DJVU_ROOT_DIR/libdjvu/GString.h \
	$$DJVU_ROOT_DIR/libdjvu/GThreads.h \
	$$DJVU_ROOT_DIR/libdjvu/GURL.h \
	$$DJVU_ROOT_DIR/libdjvu/IFFByteStream.h \
	$$DJVU_ROOT_DIR/libdjvu/IW44Image.h \
	$$DJVU_ROOT_DIR/libdjvu/JB2Image.h \
	$$DJVU_ROOT_DIR/libdjvu/JPEGDecoder.h \
	$$DJVU_ROOT_DIR/libdjvu/MMRDecoder.h \
	$$DJVU_ROOT_DIR/libdjvu/MMX.h \
	$$DJVU_ROOT_DIR/libdjvu/UnicodeByteStream.h \
	$$DJVU_ROOT_DIR/libdjvu/XMLParser.h \
	$$DJVU_ROOT_DIR/libdjvu/XMLTags.h \
	$$DJVU_ROOT_DIR/libdjvu/ZPCodec.h

SOURCES += \
	$$DJVU_ROOT_DIR/DjVu.cpp \
	$$DJVU_ROOT_DIR/DjVuFileImplementation.cpp \
	$$DJVU_ROOT_DIR/libdjvu/Arrays.cpp \
	#$$DJVU_ROOT_DIR/libdjvu/atomic.cpp \
	$$DJVU_ROOT_DIR/libdjvu/BSByteStream.cpp \
	$$DJVU_ROOT_DIR/libdjvu/BSEncodeByteStream.cpp \
	$$DJVU_ROOT_DIR/libdjvu/ByteStream.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DataPool.cpp \
	$$DJVU_ROOT_DIR/libdjvu/debug.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVmDir.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVmDir0.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVmDoc.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVmNav.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVuAnno.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVuDocEditor.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVuDocument.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVuDumpHelper.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVuErrorList.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVuFile.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVuFileCache.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVuGlobal.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVuGlobalMemory.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVuImage.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVuInfo.cpp \
	#$$DJVU_ROOT_DIR/libdjvu/DjVuMessage.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVuMessageLite.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVuNavDir.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVuPalette.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVuPort.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVuText.cpp \
	$$DJVU_ROOT_DIR/libdjvu/DjVuToPS.cpp \
	$$DJVU_ROOT_DIR/libdjvu/GBitmap.cpp \
	$$DJVU_ROOT_DIR/libdjvu/GContainer.cpp \
	$$DJVU_ROOT_DIR/libdjvu/GException.cpp \
	$$DJVU_ROOT_DIR/libdjvu/GIFFManager.cpp \
	$$DJVU_ROOT_DIR/libdjvu/GMapAreas.cpp \
	#$$DJVU_ROOT_DIR/libdjvu/GOS.cpp \
	$$DJVU_ROOT_DIR/libdjvu/GPixmap.cpp \
	$$DJVU_ROOT_DIR/libdjvu/GRect.cpp \
	$$DJVU_ROOT_DIR/libdjvu/GScaler.cpp \
	$$DJVU_ROOT_DIR/libdjvu/GSmartPointer.cpp \
	$$DJVU_ROOT_DIR/libdjvu/GString.cpp \
	$$DJVU_ROOT_DIR/libdjvu/GThreads.cpp \
	$$DJVU_ROOT_DIR/libdjvu/GUnicode.cpp \
	#$$DJVU_ROOT_DIR/libdjvu/GURL.cpp \
	$$DJVU_ROOT_DIR/libdjvu/IFFByteStream.cpp \
	$$DJVU_ROOT_DIR/libdjvu/IW44EncodeCodec.cpp \
	$$DJVU_ROOT_DIR/libdjvu/IW44Image.cpp \
	$$DJVU_ROOT_DIR/libdjvu/JB2EncodeCodec.cpp \
	$$DJVU_ROOT_DIR/libdjvu/JB2Image.cpp \
	$$DJVU_ROOT_DIR/libdjvu/JPEGDecoder.cpp \
	$$DJVU_ROOT_DIR/libdjvu/MMRDecoder.cpp \
	$$DJVU_ROOT_DIR/libdjvu/MMX.cpp \
	$$DJVU_ROOT_DIR/libdjvu/UnicodeByteStream.cpp \
	$$DJVU_ROOT_DIR/libdjvu/XMLParser.cpp \
	$$DJVU_ROOT_DIR/libdjvu/XMLTags.cpp \
	$$DJVU_ROOT_DIR/libdjvu/ZPCodec.cpp

SOURCES += \
	$$DJVU_WRAPPER/atomic.cpp \
	$$DJVU_WRAPPER/DjVuMessage.cpp \
	$$DJVU_WRAPPER/GOS.cpp \
	$$DJVU_WRAPPER/GURL.cpp

# PdfReader
PDF_ROOT_DIR = $$CORE_ROOT_DIR/PdfFile

INCLUDEPATH += \
	$$PDF_ROOT_DIR/lib/goo \
	$$PDF_ROOT_DIR/lib/fofi \
	$$PDF_ROOT_DIR/lib/splash \
	$$PDF_ROOT_DIR/lib

HEADERS += $$files($$PDF_ROOT_DIR/lib/*.h, true)
SOURCES += $$files($$PDF_ROOT_DIR/lib/*.c, true)
SOURCES += $$files($$PDF_ROOT_DIR/lib/*.cc, true)

SOURCES -= \
	$$PDF_ROOT_DIR/lib/xpdf/HTMLGen.cc \
	$$PDF_ROOT_DIR/lib/xpdf/pdftohtml.cc \
	$$PDF_ROOT_DIR/lib/xpdf/pdftopng.cc \
	$$PDF_ROOT_DIR/lib/xpdf/pdftoppm.cc \
	$$PDF_ROOT_DIR/lib/xpdf/pdftops.cc \
	$$PDF_ROOT_DIR/lib/xpdf/pdftotext.cc \
	$$PDF_ROOT_DIR/lib/xpdf/pdfdetach.cc \
	$$PDF_ROOT_DIR/lib/xpdf/pdffonts.cc \
	$$PDF_ROOT_DIR/lib/xpdf/pdfimages.cc \
	$$PDF_ROOT_DIR/lib/xpdf/pdfinfo.cc

SOURCES += \
	$$PDF_ROOT_DIR/SrcReader/RendererOutputDev.cpp \
	$$PDF_ROOT_DIR/SrcReader/Adaptors.cpp \
	$$PDF_ROOT_DIR/SrcReader/GfxClip.cpp \
	$$PDF_ROOT_DIR/SrcReader/PdfAnnot.cpp \
	$$PDF_ROOT_DIR/SrcReader/RedactOutputDev.cpp \
	$$PDF_ROOT_DIR/Resources/BaseFonts.cpp \
	$$PDF_ROOT_DIR/Resources/CMapMemory/cmap_memory.cpp

HEADERS +=\
	$$PDF_ROOT_DIR/Resources/Fontd050000l.h \
	$$PDF_ROOT_DIR/Resources/Fontn019003l.h \
	$$PDF_ROOT_DIR/Resources/Fontn019004l.h \
	$$PDF_ROOT_DIR/Resources/Fontn019023l.h \
	$$PDF_ROOT_DIR/Resources/Fontn019024l.h \
	$$PDF_ROOT_DIR/Resources/Fontn021003l.h \
	$$PDF_ROOT_DIR/Resources/Fontn021004l.h \
	$$PDF_ROOT_DIR/Resources/Fontn021023l.h \
	$$PDF_ROOT_DIR/Resources/Fontn021024l.h \
	$$PDF_ROOT_DIR/Resources/Fontn022003l.h \
	$$PDF_ROOT_DIR/Resources/Fontn022004l.h \
	$$PDF_ROOT_DIR/Resources/Fontn022023l.h \
	$$PDF_ROOT_DIR/Resources/Fontn022024l.h \
	$$PDF_ROOT_DIR/Resources/Fonts050000l.h \
	$$PDF_ROOT_DIR/Resources/BaseFonts.h \
	$$PDF_ROOT_DIR/Resources/CMapMemory/cmap_memory.h \
	$$PDF_ROOT_DIR/SrcReader/RendererOutputDev.h \
	$$PDF_ROOT_DIR/SrcReader/Adaptors.h \
	$$PDF_ROOT_DIR/SrcReader/MemoryUtils.h \
	$$PDF_ROOT_DIR/SrcReader/GfxClip.h \
	$$PDF_ROOT_DIR/SrcReader/FontsWasm.h \
	$$PDF_ROOT_DIR/SrcReader/PdfAnnot.h \
	$$PDF_ROOT_DIR/SrcReader/RedactOutputDev.h

DEFINES += CRYPTOPP_DISABLE_ASM
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib

# PdfWriter
HEADERS += \
	$$PDF_ROOT_DIR/SrcWriter/AcroForm.h \
	$$PDF_ROOT_DIR/SrcWriter/Annotation.h \
	$$PDF_ROOT_DIR/SrcWriter/Catalog.h \
	$$PDF_ROOT_DIR/SrcWriter/Consts.h \
	$$PDF_ROOT_DIR/SrcWriter/Destination.h \
	$$PDF_ROOT_DIR/SrcWriter/Document.h \
	$$PDF_ROOT_DIR/SrcWriter/Encodings.h \
	$$PDF_ROOT_DIR/SrcWriter/Encrypt.h \
	$$PDF_ROOT_DIR/SrcWriter/EncryptDictionary.h \
	$$PDF_ROOT_DIR/SrcWriter/Field.h \
	$$PDF_ROOT_DIR/SrcWriter/Font.h \
	$$PDF_ROOT_DIR/SrcWriter/Font14.h \
	$$PDF_ROOT_DIR/SrcWriter/FontCidTT.h \
	$$PDF_ROOT_DIR/SrcWriter/FontTT.h \
	$$PDF_ROOT_DIR/SrcWriter/FontTTWriter.h \
	$$PDF_ROOT_DIR/SrcWriter/GState.h \
	$$PDF_ROOT_DIR/SrcWriter/Image.h \
	$$PDF_ROOT_DIR/SrcWriter/Info.h \
	$$PDF_ROOT_DIR/SrcWriter/Objects.h \
	$$PDF_ROOT_DIR/SrcWriter/Outline.h \
	$$PDF_ROOT_DIR/SrcWriter/Pages.h \
	$$PDF_ROOT_DIR/SrcWriter/Pattern.h \
	$$PDF_ROOT_DIR/SrcWriter/ResourcesDictionary.h \
	$$PDF_ROOT_DIR/SrcWriter/Shading.h \
	$$PDF_ROOT_DIR/SrcWriter/Streams.h \
	$$PDF_ROOT_DIR/SrcWriter/Types.h \
	$$PDF_ROOT_DIR/SrcWriter/Utils.h \
	$$PDF_ROOT_DIR/SrcWriter/Metadata.h \
	$$PDF_ROOT_DIR/SrcWriter/ICCProfile.h \
	$$PDF_ROOT_DIR/SrcWriter/States.h

SOURCES += \
	$$PDF_ROOT_DIR/SrcWriter/AcroForm.cpp \
	$$PDF_ROOT_DIR/SrcWriter/Annotation.cpp \
	$$PDF_ROOT_DIR/SrcWriter/Catalog.cpp \
	$$PDF_ROOT_DIR/SrcWriter/Destination.cpp \
	$$PDF_ROOT_DIR/SrcWriter/Document.cpp \
	$$PDF_ROOT_DIR/SrcWriter/Encrypt.cpp \
	$$PDF_ROOT_DIR/SrcWriter/EncryptDictionary.cpp \
	$$PDF_ROOT_DIR/SrcWriter/Field.cpp \
	$$PDF_ROOT_DIR/SrcWriter/Font.cpp \
	$$PDF_ROOT_DIR/SrcWriter/Font14.cpp \
	$$PDF_ROOT_DIR/SrcWriter/FontCidTT.cpp \
	$$PDF_ROOT_DIR/SrcWriter/FontTT.cpp \
	$$PDF_ROOT_DIR/SrcWriter/FontTTWriter.cpp \
	$$PDF_ROOT_DIR/SrcWriter/FontOTWriter.cpp \
	$$PDF_ROOT_DIR/SrcWriter/GState.cpp \
	$$PDF_ROOT_DIR/SrcWriter/Image.cpp \
	$$PDF_ROOT_DIR/SrcWriter/Info.cpp \
	$$PDF_ROOT_DIR/SrcWriter/Objects.cpp \
	$$PDF_ROOT_DIR/SrcWriter/Outline.cpp \
	$$PDF_ROOT_DIR/SrcWriter/Pages.cpp \
	$$PDF_ROOT_DIR/SrcWriter/Pattern.cpp \
	$$PDF_ROOT_DIR/SrcWriter/ResourcesDictionary.cpp \
	$$PDF_ROOT_DIR/SrcWriter/Shading.cpp \
	$$PDF_ROOT_DIR/SrcWriter/Streams.cpp \
	$$PDF_ROOT_DIR/SrcWriter/Utils.cpp \
	$$PDF_ROOT_DIR/SrcWriter/Metadata.cpp \
	$$PDF_ROOT_DIR/SrcWriter/States.cpp

# PdfFile

HEADERS += \
	$$PDF_ROOT_DIR/PdfFile.h \
	$$PDF_ROOT_DIR/PdfWriter.h \
	$$PDF_ROOT_DIR/PdfReader.h \
	$$PDF_ROOT_DIR/PdfEditor.h \
	$$PDF_ROOT_DIR/OnlineOfficeBinToPdf.h

SOURCES += \
	$$PDF_ROOT_DIR/PdfFile.cpp \
	$$PDF_ROOT_DIR/PdfWriter.cpp \
	$$PDF_ROOT_DIR/PdfReader.cpp \
	$$PDF_ROOT_DIR/PdfEditor.cpp \
	$$PDF_ROOT_DIR/OnlineOfficeBinToPdf.cpp

# DocxRenderer
DOCX_RENDERER_ROOT_DIR = $$CORE_ROOT_DIR/DocxRenderer
HEADERS += \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/elements/BaseItem.h \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/elements/ContText.h \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/elements/DropCap.h \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/elements/Paragraph.h \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/elements/Shape.h \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/elements/Table.h \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/elements/TextLine.h \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/managers/ExternalImageStorage.h \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/managers/FontStyleManager.h \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/managers/ImageManager.h \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/managers/FontManager.h \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/managers/ParagraphStyleManager.h \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/styles/FontStyle.h \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/styles/ParagraphStyle.h \
	$$DOCX_RENDERER_ROOT_DIR/src/resources/ColorTable.h \
	$$DOCX_RENDERER_ROOT_DIR/src/resources/Constants.h \
	$$DOCX_RENDERER_ROOT_DIR/src/resources/ImageInfo.h \
	$$DOCX_RENDERER_ROOT_DIR/src/resources/LinesTable.h \
	$$DOCX_RENDERER_ROOT_DIR/src/resources/VectorGraphics.h \
	$$DOCX_RENDERER_ROOT_DIR/src/resources/resources.h \
	$$DOCX_RENDERER_ROOT_DIR/src/resources/utils.h \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/Page.h \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/Document.h \
	$$DOCX_RENDERER_ROOT_DIR/DocxRenderer.h

SOURCES += \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/elements/BaseItem.cpp \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/elements/ContText.cpp \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/elements/Paragraph.cpp \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/elements/Shape.cpp \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/elements/Table.cpp \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/elements/TextLine.cpp \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/managers/FontManager.cpp \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/managers/FontStyleManager.cpp \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/managers/ImageManager.cpp \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/managers/ParagraphStyleManager.cpp \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/styles/FontStyle.cpp \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/Page.cpp \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/Document.cpp \
	$$DOCX_RENDERER_ROOT_DIR/src/logic/styles/ParagraphStyle.cpp \
	$$DOCX_RENDERER_ROOT_DIR/src/resources/VectorGraphics.cpp \
	$$DOCX_RENDERER_ROOT_DIR/DocxRenderer.cpp \
	$$DOCX_RENDERER_ROOT_DIR/src/resources/resources.cpp

HEADERS += $$CORE_ROOT_DIR/DesktopEditor/doctrenderer/drawingfile.h

HEADERS += \
	../wasm/src/serialize.h \
	../wasm/src/HTMLRendererText.h \
	../wasm/src/Text.h

SOURCES += \
	../wasm/src/HTMLRendererText.cpp \
	../wasm/src/drawingfile.cpp \
	../wasm/src/drawingfile_test.cpp
