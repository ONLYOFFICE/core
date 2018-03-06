DEFINES += \
    _QT \
    FT2_BUILD_LIBRARY \
    EXCLUDE_JPG_SUPPORT \
    MNG_SUPPORT_DISPLAY \
    MNG_SUPPORT_READ \
    MNG_SUPPORT_WRITE \
    MNG_ACCESS_CHUNKS \
    MNG_STORE_CHUNKS\
    MNG_ERROR_TELLTALE

core_linux {
    DEFINES += \
    HAVE_UNISTD_H
    QMAKE_CXXFLAGS += -Wno-narrowing
}

core_mac {
    DEFINES += \
    HAVE_UNISTD_H
}

core_windows {
    DEFINES += \
    JAS_WIN_MSVC_BUILD \
    NOMINMAX

    DEFINES -= UNICODE
    DEFINES -= _UNICODE
}

LIB_GRAPHICS_PRI_PATH = $$PWD/../../..

INCLUDEPATH += \
    $$LIB_GRAPHICS_PRI_PATH/agg-2.4/include \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/include \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jasper/include \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg \
    $$LIB_GRAPHICS_PRI_PATH/cximage/png \
    $$LIB_GRAPHICS_PRI_PATH/cximage/zlib

SOURCES += $$PWD/graphics_pri.cpp

SOURCES += $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/ftbbox.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/ftgxval.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/ftlcdfil.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/ftmm.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/ftotval.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/ftpatent.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/ftpfr.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/ftsynth.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/fttype1.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/ftwinfnt.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/ftxf86.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/pcf/pcf.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/pfr/pfr.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/psaux/psaux.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/pshinter/pshinter.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/psnames/psmodule.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/raster/raster.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/sfnt/sfnt.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/truetype/truetype.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/type1/type1.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/cid/type1cid.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/type42/type42.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/winfonts/winfnt.c \
    \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/autofit/autofit.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/bdf/bdf.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/cff/cff.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/ftbase.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/ftbitmap.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/cache/ftcache.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/ftfstype.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/ftgasp.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/ftglyph.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/gzip/ftgzip.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/ftinit.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/lzw/ftlzw.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/ftstroke.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/base/ftsystem.c \
    $$LIB_GRAPHICS_PRI_PATH/freetype-2.5.2/src/smooth/smooth.c

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

SOURCES += $$LIB_GRAPHICS_PRI_PATH/Qt_build/graphics/project/libpsd_pri.c
SOURCES += $$LIB_GRAPHICS_PRI_PATH/Qt_build/graphics/project/libpsd_pri2.c
SOURCES += $$LIB_GRAPHICS_PRI_PATH/Qt_build/graphics/project/libpsd_pri3.c

!build_cximage_zlib_disable {
SOURCES += \
    $$LIB_GRAPHICS_PRI_PATH/cximage/zlib/crc32.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/zlib/adler32.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/zlib/deflate.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/zlib/inffast.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/zlib/inflate.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/zlib/inftrees.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/zlib/trees.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/zlib/zutil.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/zlib/uncompr.c \
    $$LIB_GRAPHICS_PRI_PATH/cximage/zlib/compress.c \
}

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

SOURCES += $$LIB_GRAPHICS_PRI_PATH/Qt_build/graphics/project/lepton_lib_all.cpp

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
