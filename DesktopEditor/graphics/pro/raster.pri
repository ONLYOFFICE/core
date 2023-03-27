LIB_GRAPHICS_PRI_PATH=$$PWD/../..

DEFINES -= UNICODE
DEFINES -= _UNICODE

DEFINES += \
    _QT \
    EXCLUDE_JPG_SUPPORT \
    MNG_SUPPORT_DISPLAY \
    MNG_SUPPORT_READ \
    MNG_SUPPORT_WRITE \
    MNG_ACCESS_CHUNKS \
    MNG_STORE_CHUNKS\
    MNG_ERROR_TELLTALE

core_linux {
    DEFINES += HAVE_UNISTD_H HAVE_FCNTL_H
	QMAKE_CXXFLAGS += -Wno-narrowing
}

core_mac {
    DEFINES += HAVE_UNISTD_H HAVE_FCNTL_H
}

core_windows {
    DEFINES += JAS_WIN_MSVC_BUILD NOMINMAX
	LIBS += -lUser32
}

INCLUDEPATH += \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jasper/include \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jpeg \
	$$LIB_GRAPHICS_PRI_PATH/cximage/png

HEADERS += \
    $$PWD/../../graphics/Image.h \
	$$PWD/../../raster/BgraFrame.h \
	$$PWD/../../raster/ImageFileFormatChecker.h

SOURCES += \
    $$PWD/../../graphics/Image.cpp \
    $$PWD/../../raster/BgraFrame.cpp \
	$$PWD/../../raster/ImageFileFormatChecker.cpp

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
    $$LIB_GRAPHICS_PRI_PATH/cximage/jpeg/djpeg.c

!disable_cximage_mng {
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
	$$LIB_GRAPHICS_PRI_PATH/cximage/mng/libmng_zlib.c
}

SOURCES += \
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

!disable_cximage_all {
SOURCES += \
    $$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/Encoder/jbig2arith.cpp \
    $$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/Encoder/jbig2enc.cpp \
    $$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/Encoder/jbig2sym.cpp

SOURCES += \
    $$LIB_GRAPHICS_PRI_PATH/cximage/jbig/jbig.c \
	$$LIB_GRAPHICS_PRI_PATH/cximage/jbig/jbig_tab.c

SOURCES += $$PWD/libpsd_pri.c
SOURCES += $$PWD/libpsd_pri2.c
SOURCES += $$PWD/libpsd_pri3.c

SOURCES += $$PWD/lepton_lib_all.cpp

SOURCES += $$LIB_GRAPHICS_PRI_PATH/cximage/raw/libdcr.c

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

SOURCES += \
    $$LIB_GRAPHICS_PRI_PATH/raster/Jp2/J2kFile.cpp \
    $$LIB_GRAPHICS_PRI_PATH/raster/Jp2/Reader.cpp \
    $$LIB_GRAPHICS_PRI_PATH/raster/JBig2/source/JBig2File.cpp

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
	$$LIB_GRAPHICS_PRI_PATH/cximage/jasper/ras/ras_enc.c
}

disable_cximage_mng:DEFINES += CXIMAGE_DISABLE_SUPPORT_MNG
disable_cximage_all:DEFINES += CXIMAGE_DISABLE_SUPPORT_ADDITIONS

!config_zlip_image {
    OFFICEUTILS_PATH = $$PWD/../../../OfficeUtils
	INCLUDEPATH += \
	    $$OFFICEUTILS_PATH/src/zlib-1.2.11 \
		$$OFFICEUTILS_PATH/src

    SOURCES += \
	    $$OFFICEUTILS_PATH/src/zlib-1.2.11/crc32.c \
		$$OFFICEUTILS_PATH/src/zlib-1.2.11/adler32.c \
		$$OFFICEUTILS_PATH/src/zlib-1.2.11/deflate.c \
		$$OFFICEUTILS_PATH/src/zlib-1.2.11/inffast.c \
		$$OFFICEUTILS_PATH/src/zlib-1.2.11/inflate.c \
		$$OFFICEUTILS_PATH/src/zlib-1.2.11/inftrees.c \
		$$OFFICEUTILS_PATH/src/zlib-1.2.11/trees.c \
		$$OFFICEUTILS_PATH/src/zlib-1.2.11/zutil.c \
		$$OFFICEUTILS_PATH/src/zlib-1.2.11/uncompr.c \
		$$OFFICEUTILS_PATH/src/zlib-1.2.11/compress.c \
		$$OFFICEUTILS_PATH/src/zlib_addon.c

    CONFIG += config_zlip_image
}
