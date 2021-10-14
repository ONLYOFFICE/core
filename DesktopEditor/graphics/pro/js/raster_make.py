import sys
sys.path.append("../../../../../build_tools/scripts")
import base
import os
import json

base.configure_common_apps()

# remove previous version
if base.is_dir("./o"):
    base.delete_dir("./o")
base.create_dir("./o")

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
                  # "-fno-rtti",
                  "-Wno-unused-command-line-argument",
                  "-s WASM=1",
                  "-s ALLOW_MEMORY_GROWTH=1",
                  "-s FILESYSTEM=0",
                  "-s ENVIRONMENT='web'",
                  "-s LLD_REPORT_UNDEFINED"]

compile_files_array = []

compile_files_array.append("r")
compile_files_array.append("../../../raster/")
compile_files_array.append(["BgraFrame.cpp", "ImageFileFormatChecker.cpp"])

compile_files_array.append("ci")
compile_files_array.append("../../../cximage/CxImage/")
compile_files_array.append(["ximaenc.cpp", "ximaexif.cpp", "ximage.cpp", "ximainfo.cpp", "ximajpg.cpp", "ximalpha.cpp", "ximapal.cpp", "ximasel.cpp", "xmemfile.cpp", "ximapng.cpp", "ximabmp.cpp", "ximatran.cpp", "ximatif.cpp", "tif_xfile.cpp", "ximajas.cpp", "ximagif.cpp", "ximaico.cpp", "ximatga.cpp", "ximapcx.cpp", "ximawbmp.cpp", "ximamng.cpp", "ximapsd.cpp", "ximaska.cpp", "ximaraw.cpp"])

compile_files_array.append("j")
compile_files_array.append("../../../cximage/jpeg/")
compile_files_array.append(["jerror.c", "jdmarker.c", "jdapimin.c", "jdmaster.c", "jdapistd.c", "jcomapi.c", "jutils.c", "jdinput.c", "jdmainct.c", "jmemmgr.c", "jquant1.c", "jquant2.c", "jdmerge.c", "jdcolor.c", "jdsample.c", "jdpostct.c", "jddctmgr.c", "jdarith.c", "jdhuff.c", "jdcoefct.c", "jmemnobs.c", "jidctint.c", "jidctfst.c", "jidctflt.c", "jaricom.c", "jcapimin.c", "jcparam.c", "jcapistd.c", "jcinit.c", "jcmaster.c", "jccolor.c", "jcmarker.c", "jcsample.c", "jcprepct.c", "jcdctmgr.c", "jcarith.c", "jchuff.c", "jccoefct.c", "jcmainct.c", "jfdctint.c", "jfdctfst.c", "jfdctflt.c"])

compile_files_array.append("p")
compile_files_array.append("../../../cximage/png/")
compile_files_array.append(["pngread.c", "pngmem.c", "pngerror.c", "png.c", "pngrio.c", "pngtrans.c", "pngget.c", "pngrutil.c", "pngrtran.c", "pngset.c", "pngwrite.c", "pngwio.c", "pngwutil.c", "pngwtran.c"])

compile_files_array.append("t")
compile_files_array.append("../../../cximage/tiff/")
compile_files_array.append(["tif_close.c", "tif_dir.c", "tif_aux.c", "tif_getimage.c", "tif_strip.c", "tif_open.c", "tif_tile.c", "tif_error.c", "tif_read.c", "tif_flush.c", "tif_dirinfo.c", "tif_compress.c", "tif_warning.c", "tif_swab.c", "tif_color.c", "tif_dirread.c", "tif_write.c", "tif_codec.c", "tif_luv.c", "tif_dirwrite.c", "tif_dumpmode.c", "tif_fax3.c", "tif_ojpeg.c", "tif_jpeg.c", "tif_next.c", "tif_thunder.c", "tif_packbits.c", "tif_lzw.c", "tif_zip.c", "tif_fax3sm.c", "tif_predict.c"])

compile_files_array.append("ja")
compile_files_array.append("../../../cximage/jasper/")
compile_files_array.append(["base/jas_init.c", "base/jas_stream.c", "base/jas_malloc.c", "base/jas_image.c", "base/jas_cm.c", "base/jas_seq.c", "base/jas_string.c", "base/jas_icc.c", "base/jas_debug.c", "base/jas_iccdata.c", "base/jas_tvp.c", "base/jas_version.c", "mif/mif_cod.c", "pnm/pnm_dec.c", "pnm/pnm_enc.c", "pnm/pnm_cod.c", "bmp/bmp_dec.c", "bmp/bmp_enc.c", "bmp/bmp_cod.c", "ras/ras_dec.c", "ras/ras_enc.c", "jp2/jp2_dec.c", "jp2/jp2_enc.c", "jp2/jp2_cod.c", "jpc/jpc_cs.c", "jpc/jpc_enc.c", "jpc/jpc_dec.c", "jpc/jpc_t1cod.c", "jpc/jpc_math.c", "jpc/jpc_util.c", "jpc/jpc_tsfb.c", "jpc/jpc_mct.c", "jpc/jpc_t1enc.c", "jpc/jpc_t1dec.c", "jpc/jpc_bs.c", "jpc/jpc_t2cod.c", "jpc/jpc_t2enc.c", "jpc/jpc_t2dec.c", "jpc/jpc_tagtree.c", "jpc/jpc_mqenc.c", "jpc/jpc_mqdec.c", "jpc/jpc_mqcod.c", "jpc/jpc_qmfb.c", "jpg/jpg_val.c", "jpg/jpg_dummy.c", "pgx/pgx_dec.c", "pgx/pgx_enc.c"])

compile_files_array.append("jp")
compile_files_array.append("../../../raster/Jp2/")
compile_files_array.append(["J2kFile.cpp", "Reader.cpp"])

compile_files_array.append("m")
compile_files_array.append("../../../cximage/mng/")
compile_files_array.append(["libmng_hlapi.c", "libmng_callback_xs.c", "libmng_prop_xs.c", "libmng_object_prc.c", "libmng_zlib.c", "libmng_jpeg.c", "libmng_pixels.c", "libmng_read.c", "libmng_error.c", "libmng_display.c", "libmng_write.c", "libmng_chunk_io.c", "libmng_cms.c", "libmng_filter.c", "libmng_chunk_prc.c", "libmng_chunk_xs.c"])

compile_files_array.append("lp")
compile_files_array.append("../../../cximage/libpsd/")
compile_files_array.append(["psd.c", "file_header.c", "color_mode.c", "image_resource.c", "blend.c", "layer_mask.c", "image_data.c", "stream.c", "psd_system.c", "color.c", "pattern_fill.c", "color_balance.c", "channel_image.c", "gradient_fill.c", "invert.c", "posterize.c", "brightness_contrast.c", "solid_color.c", "threshold.c", "effects.c", "selective_color.c", "channel_mixer.c", "photo_filter.c", "type_tool.c", "gradient_map.c", "hue_saturation.c", "levels.c", "curves.c", "pattern.c", "psd_zip.c", "descriptor.c", "drop_shadow.c", "inner_shadow.c", "color_overlay.c", "outer_glow.c", "inner_glow.c", "bevel_emboss.c", "satin.c", "gradient_overlay.c", "stroke.c", "pattern_overlay.c"])

compile_files_array.append("ra")
compile_files_array.append("../../../cximage/raw/")
compile_files_array.append(["libdcr.c"])

compile_files_array.append("jb")
compile_files_array.append("../../../raster/JBig2/source/")
compile_files_array.append(["JBig2File.cpp", "Encoder/jbig2enc.cpp", "Encoder/jbig2arith.cpp", "Encoder/jbig2sym.cpp", "LeptonLib/pixconv.cpp", "LeptonLib/writefile.cpp", "LeptonLib/scale.cpp", "LeptonLib/pix1.cpp", "LeptonLib/pix2.cpp", "LeptonLib/pix3.cpp", "LeptonLib/pix4.cpp", "LeptonLib/pix5.cpp", "LeptonLib/grayquant.cpp", "LeptonLib/grayquantlow.cpp", "LeptonLib/seedfill.cpp", "LeptonLib/jbclass.cpp", "LeptonLib/pixabasic.cpp", "LeptonLib/numabasic.cpp", "LeptonLib/morphseq.cpp", "LeptonLib/binexpandlow.cpp", "LeptonLib/ptabasic.cpp", "LeptonLib/rop.cpp", "LeptonLib/colormap.cpp", "LeptonLib/pngiostub.cpp", "LeptonLib/lepton_utils.cpp", "LeptonLib/scalelow.cpp", "LeptonLib/enhance.cpp", "LeptonLib/jpegio.cpp", "LeptonLib/jpegiostub.cpp", "LeptonLib/spixio.cpp", "LeptonLib/webpio.cpp", "LeptonLib/webpiostub.cpp", "LeptonLib/psio2.cpp", "LeptonLib/gifio.cpp", "LeptonLib/gifiostub.cpp", "LeptonLib/pnmio.cpp", "LeptonLib/tiffio.cpp", "LeptonLib/tiffiostub.cpp", "LeptonLib/bmpio.cpp", "LeptonLib/binexpand.cpp", "LeptonLib/compare.cpp", "LeptonLib/boxbasic.cpp", "LeptonLib/conncomp.cpp", "LeptonLib/pixafunc1.cpp", "LeptonLib/boxfunc1.cpp", "LeptonLib/ptafunc1.cpp", "LeptonLib/binreduce.cpp", "LeptonLib/seedfilllow.cpp", "LeptonLib/sel1.cpp", "LeptonLib/morphapp.cpp", "LeptonLib/correlscore.cpp", "LeptonLib/sarray.cpp", "LeptonLib/morph.cpp", "LeptonLib/roplow.cpp", "LeptonLib/fpix1.cpp", "LeptonLib/stack.cpp", "LeptonLib/pixacc.cpp", "LeptonLib/pixarith.cpp", "LeptonLib/convolve.cpp", "LeptonLib/binreducelow.cpp", "LeptonLib/convolvelow.cpp", "LeptonLib/arithlow.cpp"])

compiler_flags.append("-I../../../../OfficeUtils/src/zlib-1.2.11 -I../../../cximage/jasper/include")
compiler_flags.append("-D__linux__ -DBUILDING_WASM_MODULE -D_tcsnicmp=strncmp -D_lseek=lseek -D_getcwd=getcwd -DNO_CONSOLE_IO")

# arguments
arguments = ""
for item in compiler_flags:
    arguments += (item + " ")

# command
compile_files_array_len = len(compile_files_array)
external_file = []

prefix_call = ""
if base.host_platform() == "windows":
  prefix_call = "call "
  external_file.append("call emsdk/emsdk_env.bat")
else:
  external_file.append("#!/bin/bash")
  external_file.append("source ./emsdk/emsdk_env.sh")

file_index = 0
libs = ""
while file_index < compile_files_array_len:
  objects_dir = compile_files_array[file_index]
  base_dir = compile_files_array[file_index + 1]
  files = compile_files_array[file_index + 2]
  file_index += 3
  base.create_dir("./o/" + objects_dir)
  for item in files:
    file_name = os.path.splitext(os.path.basename(item))[0]
    external_file.append(prefix_call + "emcc -o o/" + objects_dir + "/" + file_name + ".o -c " + arguments + base_dir + item)
    libs += ("o/" + objects_dir + "/" + file_name + ".o ")

external_file.append(prefix_call + "emcc -r -o raster.o " + arguments + "wasm/src/raster.cpp " + libs)
base.run_as_bat(external_file)

# clear
base.delete_dir("./o")
