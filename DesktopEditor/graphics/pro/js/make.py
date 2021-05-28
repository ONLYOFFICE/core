import sys
sys.path.append("../../../../../build_tools/scripts")
import base
import os

base.configure_common_apps()

# remove previous version
if base.is_dir("./deploy"):
    base.delete_dir("./deploy")
base.create_dir("./deploy")
if base.is_dir("./temp"):
    base.delete_dir("./temp")
base.create_dir("./temp")

# fetch emsdk
command_prefix = "" if ("windows" == base.host_platform()) else "./"
if not base.is_dir("emsdk"):
    base.cmd("git", ["clone", "https://github.com/emscripten-core/emsdk.git"])
    os.chdir("emsdk")
    base.cmd(command_prefix + "emsdk", ["install", "latest"])
    base.cmd(command_prefix + "emsdk", ["activate", "latest"])
    os.chdir("../")

# compile
compiler_flags = ["-O3",
                  "-fno-rtti",
                  "-s WASM=1",
                  "-s ALLOW_MEMORY_GROWTH=1",
                  "-s FILESYSTEM=0",
                  "-s ENVIRONMENT='web,worker'"]
                  #"-s LLD_REPORT_UNDEFINED"]

exported_functions = ["_malloc",
                      "_free",
                      "_Raster_Malloc",
                      "_Raster_Free",
                      "_Raster_Create",
                      "_Raster_Load",
                      "_Raster_Destroy",
                      "_Raster_GetHeight",
                      "_Raster_GetWidth",
                      "_Raster_Decode",
                      "_Raster_GetRGBA"]

libRaster_src_path = "../../../raster"
input_raster_sources = ["BgraFrame.cpp", "ImageFileFormatChecker.cpp"]

libZlib_src_path = "../../../../OfficeUtils/src/zlib-1.2.11"
input_zlib_sources = ["inflate.c", "zutil.c", "crc32.c", "adler32.c", "inftrees.c", "inffast.c",
                      "deflate.c", "trees.c"]

libCxImage_src_path = "../../../cximage/CxImage"
input_cximage_sources = ["ximaenc.cpp", "ximaexif.cpp", "ximage.cpp", "ximainfo.cpp",
                         "ximajpg.cpp", "ximalpha.cpp", "ximapal.cpp", "ximasel.cpp",
                         "xmemfile.cpp", "ximapng.cpp", "ximabmp.cpp", "ximatran.cpp",
                         "ximatif.cpp", "tif_xfile.cpp", "ximajas.cpp", "ximagif.cpp",
                         "ximaico.cpp", "ximatga.cpp", "ximapcx.cpp", "ximawbmp.cpp",
                         "ximamng.cpp", "ximapsd.cpp", "ximaska.cpp", "ximaraw.cpp"]

libJpeg_src_path = "../../../cximage/jpeg"
input_jpeg_sources = ["jerror.c", "jdmarker.c", "jdapimin.c", "jdmaster.c", "jdapistd.c",
                      "jcomapi.c", "jutils.c", "jdinput.c", "jdmainct.c", "jmemmgr.c",
                      "jquant1.c", "jquant2.c", "jdmerge.c", "jdcolor.c", "jdsample.c",
                      "jdpostct.c", "jddctmgr.c", "jdarith.c", "jdhuff.c", "jdcoefct.c",
                      "jmemnobs.c", "jidctint.c", "jidctfst.c", "jidctflt.c", "jaricom.c",
                      "jcapimin.c", "jcparam.c", "jcapistd.c", "jcinit.c", "jcmaster.c",
                      "jccolor.c", "jcmarker.c", "jcsample.c", "jcprepct.c", "jcdctmgr.c",
                      "jcarith.c", "jchuff.c", "jccoefct.c", "jcmainct.c", "jfdctint.c",
                      "jfdctfst.c", "jfdctflt.c"]

libPng_src_path = "../../../cximage/png"
input_png_sources = ["pngread.c", "pngmem.c", "pngerror.c", "png.c", "pngrio.c", "pngtrans.c",
                     "pngget.c", "pngrutil.c", "pngrtran.c", "pngset.c"]

libTiff_src_path = "../../../cximage/tiff"
input_tiff_sources = ["tif_close.c", "tif_dir.c", "tif_aux.c", "tif_getimage.c", "tif_strip.c",
                      "tif_open.c", "tif_tile.c", "tif_error.c", "tif_read.c", "tif_flush.c",
                      "tif_dirinfo.c", "tif_compress.c", "tif_warning.c", "tif_swab.c",
                      "tif_color.c", "tif_dirread.c", "tif_write.c", "tif_codec.c", "tif_luv.c",
                      "tif_dirwrite.c", "tif_dumpmode.c", "tif_fax3.c", "tif_ojpeg.c",
                      "tif_jpeg.c", "tif_next.c", "tif_thunder.c", "tif_packbits.c", "tif_lzw.c",
                      "tif_zip.c", "tif_fax3sm.c", "tif_predict.c"]

libJasper_src_path = "../../../cximage/jasper"
input_jasper_sources = ["base/jas_init.c", "base/jas_stream.c", "base/jas_malloc.c", 
                        "base/jas_image.c", "base/jas_cm.c", "base/jas_seq.c", 
                        "base/jas_string.c", "base/jas_icc.c", "base/jas_debug.c", 
                        "base/jas_iccdata.c", "base/jas_tvp.c", "base/jas_version.c",
                        "mif/mif_cod.c", "pnm/pnm_dec.c", "pnm/pnm_enc.c", "pnm/pnm_cod.c",
                        "bmp/bmp_dec.c", "bmp/bmp_enc.c", "bmp/bmp_cod.c", "ras/ras_dec.c",
                        "ras/ras_enc.c", "jp2/jp2_dec.c", "jp2/jp2_enc.c", "jp2/jp2_cod.c",
                        "jpc/jpc_cs.c", "jpc/jpc_enc.c", "jpc/jpc_dec.c", "jpc/jpc_t1cod.c",
                        "jpc/jpc_math.c", "jpc/jpc_util.c", "jpc/jpc_tsfb.c", "jpc/jpc_mct.c",
                        "jpc/jpc_t1enc.c", "jpc/jpc_t1dec.c", "jpc/jpc_bs.c", "jpc/jpc_t2cod.c",
                        "jpc/jpc_t2enc.c", "jpc/jpc_t2dec.c", "jpc/jpc_tagtree.c",
                        "jpc/jpc_mqenc.c", "jpc/jpc_mqdec.c", "jpc/jpc_mqcod.c",
                        "jpc/jpc_qmfb.c", "jpg/jpg_val.c", "jpg/jpg_dummy.c", "pgx/pgx_dec.c",
                        "pgx/pgx_enc.c"]

libJ2kFile_src_path = "../../../raster/Jp2"
input_j2k_sources = ["J2kFile.cpp", "Reader.cpp"]

libMng_src_path = "../../../cximage/mng"
input_mng_sources = ["libmng_hlapi.c", "libmng_callback_xs.c", "libmng_prop_xs.c",
                     "libmng_object_prc.c", "libmng_zlib.c", "libmng_jpeg.c", "libmng_pixels.c",
                     "libmng_read.c", "libmng_error.c", "libmng_display.c", "libmng_write.c",
                     "libmng_chunk_io.c", "libmng_cms.c", "libmng_filter.c",
                     "libmng_chunk_prc.c"]

libPsd_src_path = "../../../cximage/libpsd"
input_psd_sources = ["psd.c", "file_header.c", "color_mode.c", "image_resource.c", "blend.c",
                     "layer_mask.c", "image_data.c", "stream.c", "psd_system.c", "color.c",
                     "pattern_fill.c", "color_balance.c", "channel_image.c", "gradient_fill.c",
                     "invert.c", "posterize.c", "brightness_contrast.c", "solid_color.c",
                     "threshold.c", "effects.c", "selective_color.c", "channel_mixer.c",
                     "photo_filter.c", "type_tool.c", "gradient_map.c", "hue_saturation.c",
                     "levels.c", "curves.c", "pattern.c", "psd_zip.c", "descriptor.c",
                     "drop_shadow.c", "inner_shadow.c", "color_overlay.c", "outer_glow.c",
                     "inner_glow.c", "bevel_emboss.c", "satin.c", "gradient_overlay.c",
                     "stroke.c", "pattern_overlay.c"]

libRaw_src_path = "../../../cximage/raw"
input_raw_sources = ["libdcr.c"]

sources = []
for item in input_raster_sources:
    sources.append(libRaster_src_path + '/' + item)
for item in input_zlib_sources:
    sources.append(libZlib_src_path + '/' + item)
for item in input_cximage_sources:
    sources.append(libCxImage_src_path + '/' + item)
# jpeg
for item in input_png_sources:
    sources.append(libPng_src_path + '/' + item)
# tiff
for item in input_jasper_sources:
    sources.append(libJasper_src_path + '/' + item)
for item in input_j2k_sources:
    sources.append(libJ2kFile_src_path + '/' + item)
for item in input_mng_sources:
    sources.append(libMng_src_path + '/' + item)
for item in input_psd_sources:
    sources.append(libPsd_src_path + '/' + item)
for item in input_raw_sources:
    sources.append(libRaw_src_path + '/' + item)
sources.append("wasm/src/base.cpp")

compiler_flags.append("-I../../../../OfficeUtils/src/zlib-1.2.11")
compiler_flags.append("-I" + libJasper_src_path + "/include")
compiler_flags.append("-D__linux__ -DBUILDING_WASM_MODULE -D_tcsnicmp=strncmp -D_lseek=lseek -D_getcwd=getcwd")

# arguments
arguments = ""
for item in compiler_flags:
    arguments += (item + " ")

# command
windows_bat = []
if base.host_platform() == "windows":
    windows_bat.append("call emsdk/emsdk_env.bat")
    
    jpeg = ""
    for item in input_jpeg_sources:
        windows_bat.append("call emcc -o temp/" + item + ".o -c " + arguments + libJpeg_src_path + '/' + item)
        jpeg += ("temp/" + item + ".o ")
    
    tiff = ""
    for item in input_tiff_sources:
        windows_bat.append("call emcc -o temp/" + item + ".o -c " + arguments + libTiff_src_path + '/' + item)
        tiff += ("temp/" + item + ".o ")
    
    arguments += "-s EXPORTED_FUNCTIONS=\"["
    for item in exported_functions:
        arguments += ("'" + item + "',")
    arguments = arguments[:-1]
    arguments += "]\" "
    for item in sources:
        arguments += (item + " ")
    
    windows_bat.append("call emcc -o raster.js " + arguments + jpeg + tiff)
else:
    windows_bat.append("#!/bin/bash")
    windows_bat.append("source ./emsdk/emsdk_env.sh")
    
    jpeg = ""
    for item in input_jpeg_sources:
        windows_bat.append("emcc -o temp/" + item + ".o -c " + arguments + libJpeg_src_path + '/' + item)
        jpeg += ("temp/" + item + ".o ")
    
    tiff = ""
    for item in input_tiff_sources:
        windows_bat.append("emcc -o temp/" + item + ".o -c " + arguments + libTiff_src_path + '/' + item)
        tiff += ("temp/" + item + ".o ")
    
    arguments += "-s EXPORTED_FUNCTIONS=\"["
    for item in exported_functions:
        arguments += ("'" + item + "',")
    arguments = arguments[:-1]
    arguments += "]\" "
    for item in sources:
        arguments += (item + " ")
    
    windows_bat.append("emcc -o raster.js " + arguments + jpeg + tiff)
base.run_as_bat(windows_bat)

# finalize
base.replaceInFile("./raster.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];")
base.replaceInFile("./raster.js", "function getBinaryPromise(){", "function getBinaryPromise2(){")

raster_js_content = base.readFile("./raster.js")
engine_base_js_content = base.readFile("./wasm/js/raster.js")
engine_js_content = engine_base_js_content.replace("//module", raster_js_content)

# write new version
base.writeFile("./deploy/raster.js", engine_js_content)
base.copy_file("./raster.wasm", "./deploy/raster.wasm")
base.copy_file("./wasm/js/index.html", "./deploy/index.html")
base.copy_file("./wasm/js/code.js", "./deploy/code.js")

base.delete_file("raster.js")
base.delete_file("raster.wasm")
base.delete_dir("./temp")
