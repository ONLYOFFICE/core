import sys
sys.path.append("../../../../../build_tools/scripts")
import base
import os

if not base.is_file("raster.o"):
  print("Please use raste_make.py previously")
  exit(0)

base.configure_common_apps()

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
                  "-s ENVIRONMENT='web,worker'",
                  "-s LLD_REPORT_UNDEFINED"]

exported_functions = ["_malloc",
                      "_free",
                      "_Graphics_Malloc",
                      "_Graphics_Free",
                      "_Graphics_Create",
                      "_Graphics_Destroy",
                      "_Graphics_GetPagesCount",
                      "_Graphics_GetPageHeight",
                      "_Graphics_GetPageWidth",
                      "_Graphics_GetPage",
                      "_Graphics_TEST"]

libGraphics_src_path = "../../"
input_graphics_sources = ["GraphicsRenderer.cpp", "pro/pro_Graphics.cpp", "pro/pro_Fonts.cpp", "Graphics.cpp", "Brush.cpp", "GraphicsPath.cpp", "Image.cpp", "Matrix.cpp", "Clip.cpp"]

libFontEngine_src_path = "../../../fontengine/"
input_fontengine_sources = ["GlyphString.cpp", "FontManager.cpp", "FontFile.cpp", "FontPath.cpp", "ApplicationFonts.cpp"]

libAgg_src_path = "../../../agg-2.4/src/"
input_agg_sources = ["agg_arc.cpp", "agg_vcgen_stroke.cpp", "agg_vcgen_dash.cpp", "agg_trans_affine.cpp", "agg_curves.cpp"]

libFreetype_src_path = "../../../freetype-2.10.4/src/"
input_freetype_sources = ["base/ftinit.c", "base/ftlcdfil.c", "base/ftobjs.c", "base/ftglyph.c", "base/ftoutln.c", "base/ftutil.c", "base/ftgloadr.c", "base/ftfntfmt.c", "base/ftcalc.c", "base/ftbitmap.c", "base/ftstream.c", "base/fthash.c", "base/ftdebug.c", "base/fttrigon.c", "base/ftadvanc.c", "base/ftpsprop.c", "base/ftrfork.c", "bdf/bdfdrivr.c", "bdf/bdflib.c", "smooth/ftsmooth.c", "smooth/ftgrays.c", "../builds/unix/ftsystem.c", "autofit/afmodule.c", "autofit/afhints.c", "autofit/afloader.c", "autofit/afglobal.c", "autofit/afshaper.c", "autofit/afranges.c", "autofit/afdummy.c", "autofit/aflatin.c", "autofit/afcjk.c", "autofit/afindic.c", "autofit/afangles.c", "autofit/afblue.c", "autofit/afwarp.c", "truetype/ttdriver.c", "truetype/ttgload.c", "truetype/ttpload.c", "truetype/ttobjs.c", "truetype/ttgxvar.c", "truetype/ttinterp.c", "type1/t1driver.c", "type1/t1afm.c", "type1/t1load.c", "type1/t1gload.c", "type1/t1objs.c", "type1/t1parse.c", "cff/cffdrivr.c", "cff/cffgload.c", "cff/cffload.c", "cff/cffcmap.c", "cff/cffparse.c", "cff/cffobjs.c", "cid/cidriver.c", "cid/cidobjs.c", "cid/cidgload.c", "cid/cidload.c", "cid/cidparse.c", "pfr/pfrdrivr.c", "pfr/pfrobjs.c", "pfr/pfrload.c", "pfr/pfrgload.c", "pfr/pfrcmap.c", "pfr/pfrsbit.c", "type42/t42drivr.c", "type42/t42objs.c", "type42/t42parse.c", "winfonts/winfnt.c", "pcf/pcfdrivr.c", "pcf/pcfread.c", "pcf/pcfutil.c", "psaux/psauxmod.c", "psaux/psobjs.c", "psaux/t1decode.c", "psaux/psft.c", "psaux/afmparse.c", "psaux/t1cmap.c", "psaux/cffdecode.c", "psaux/psconv.c", "psaux/psfont.c", "psaux/psblues.c", "psaux/psintrp.c", "psaux/pserror.c", "psaux/psstack.c", "psaux/pshints.c", "psaux/psarrst.c", "psaux/psread.c", "psnames/psmodule.c", "pshinter/pshmod.c", "pshinter/pshrec.c", "pshinter/pshglob.c", "pshinter/pshalgo.c", "raster/ftrend1.c", "raster/ftraster.c", "sfnt/sfdriver.c", "sfnt/ttpost.c", "sfnt/sfobjs.c", "sfnt/ttload.c", "sfnt/ttbdf.c", "sfnt/ttmtx.c", "sfnt/ttkern.c", "sfnt/sfwoff.c", "sfnt/ttcmap.c", "sfnt/ttsbit.c", "sfnt/sfwoff2.c", "sfnt/ttcolr.c", "sfnt/woff2tags.c", "sfnt/ttcpal.c", "gzip/ftgzip.c", "lzw/ftlzw.c"]

libCommon_src_path = "../../../common/"
input_common_sources = ["File.cpp", "Directory.cpp", "ByteBuilder.cpp", "Base64.cpp"]

libUnicodeConverter_src_path = "../../../../UnicodeConverter/"
input_unicodeconverter_sources = ["UnicodeConverter.cpp"]

libIcu_src_path = "../../../../Common/3dParty/icu/icu/source/common/"
input_icu_sources = ["ucnv.c", "ustr_wcs.cpp", "ucnv_err.c", "ucnv_bld.cpp", "ustrtrns.cpp", "ucnv_cb.c", "udata.cpp", "ucnv_io.cpp", "uhash.c", "udatamem.c", "cmemory.c", "ustring.cpp", "umutex.cpp", "putil.cpp", "ustr_cnv.cpp", "ucnvmbcs.cpp", "ucnvlat1.c", "ucnv_u16.c", "ucnv_u8.c", "ucnv_u32.c", "ucnv_u7.c", "ucln_cmn.cpp", "ucnv2022.cpp", "ucnv_lmb.c", "ucnvhz.c", "ucnvscsu.c", "ucnvisci.c", "ucnvbocu.cpp", "ucnv_ct.c", "ucnv_cnv.c", "stringpiece.cpp", "charstr.cpp", "umapfile.c", "ucmndata.c", "ucnv_ext.cpp", "uobject.cpp", "umath.c"]

# sources
sources = []
for item in input_graphics_sources:
    sources.append(libGraphics_src_path + item)
for item in input_fontengine_sources:
    sources.append(libFontEngine_src_path + item)
for item in input_agg_sources:
    sources.append(libAgg_src_path + item)
for item in input_freetype_sources:
    sources.append(libFreetype_src_path + item)
for item in input_common_sources:
    sources.append(libCommon_src_path + item)
for item in input_unicodeconverter_sources:
    sources.append(libUnicodeConverter_src_path + item)
# icu
sources.append("raster.o")
sources.append("wasm/src/graphics.cpp")

compiler_flags.append("-I../../../agg-2.4/include -I../../../cximage/jasper/include -I../../../cximage/jpeg -I../../../cximage/png -I../../../freetype-2.10.4/include -I../../../freetype-2.10.4/include/freetype -I../../../../OfficeUtils/src/zlib-1.2.11 -I../../../../Common/3dParty/icu/icu/include")
compiler_flags.append("-D__linux__ -D_LINUX -DFT2_BUILD_LIBRARY -DHAVE_FCNTL_H -DFT_CONFIG_OPTION_USE_ZLIB -DFT_CONFIG_OPTION_SYSTEM_ZLIB -DBUILDING_WASM_MODULE -DU_COMMON_IMPLEMENTATION -DMAP_IMPLEMENTATION=MAP_390DLL")

# arguments
arguments = ""
for item in compiler_flags:
    arguments += (item + " ")



# command
windows_bat = []
if base.host_platform() == "windows":
    windows_bat.append("call emsdk/emsdk_env.bat") 

    icu = ""
    for item in input_icu_sources:
        windows_bat.append("call emcc -o temp/" + item + ".o -c " + arguments + libIcu_src_path + item) 
        icu += ("temp/" + item + ".o ")

    arguments += "-s EXPORTED_FUNCTIONS=\"["
    for item in exported_functions:
        arguments += ("'" + item + "',")
    arguments = arguments[:-1]
    arguments += "]\" "

    for item in sources:
        arguments += (item + " ")

    windows_bat.append("call emcc -o graphics.js " + arguments + icu)
else:
    windows_bat.append("#!/bin/bash")
    windows_bat.append("source ./emsdk/emsdk_env.sh")

    icu = ""
    for item in input_icu_sources:
        windows_bat.append("emcc -o temp/" + item + ".o -c " + arguments + libIcu_src_path + item) 
        icu += ("temp/" + item + ".o ")

    arguments += "-s EXPORTED_FUNCTIONS=\"["
    for item in exported_functions:
        arguments += ("'" + item + "',")
    arguments = arguments[:-1]
    arguments += "]\" "

    for item in sources:
        arguments += (item + " ")
    
    windows_bat.append("emcc -o graphics.js " + arguments + icu)
base.run_as_bat(windows_bat)

# finalize
base.replaceInFile("./graphics.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){self.onEngineGraphicsInit();}];")
base.replaceInFile("./graphics.js", "function getBinaryPromise(){", "function getBinaryPromise2(){")

graphics_js_content = base.readFile("./graphics.js")
desktop_fetch_content = base.readFile("./../../../../Common/js/desktop_fetch.js")
string_utf8_content   = base.readFile("./../../../../Common/js/string_utf8.js")
engine_base_js_content = base.readFile("./wasm/js/graphics.js")
engine_js_content = engine_base_js_content.replace("//desktop_fetch", desktop_fetch_content)
engine_js_content = engine_js_content.replace("//string_utf8", string_utf8_content)
engine_js_content = engine_js_content.replace("//module", graphics_js_content)

# write new version
base.writeFile("./deploy/graphics.js", engine_js_content)
base.copy_file("./graphics.wasm", "./deploy/graphics.wasm")
base.copy_file("./wasm/js/index.html", "./deploy/index.html")
base.copy_file("./wasm/js/code_graphics.js", "./deploy/code_graphics.js")

base.delete_file("graphics.js")
base.delete_file("graphics.wasm")
base.delete_dir("./temp")
# base.delete_file("raster.o")
