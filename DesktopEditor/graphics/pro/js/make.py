#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
sys.path.append("../../../../../build_tools/scripts")
import base
import os
import json

def apply_patch(file, patch):
  file_content = base.readFile(file)
  patch_content = base.readFile(patch)
  index1 = patch_content.find("<<<<<<<")
  index2 = patch_content.find("=======")
  index3 = patch_content.find(">>>>>>>")
  file_content_old = patch_content[index1 + 7:index2]
  file_content_new = "\n#if 0" + file_content_old + "#else" + patch_content[index2 + 7:index3] + "#endif\n"
  base.replaceInFile(file, file_content_old, file_content_new)
  return

if not base.is_file("./raster.o"):
  base.cmd("python", ["raster_make.py"])
if not base.is_file("./raster.o"):
  print("raster_make.py error")
  exit(0)

base.configure_common_apps()

# remove previous version
if base.is_dir("./deploy"):
  base.delete_dir("./deploy")
base.create_dir("./deploy")
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

if not base.is_dir("xml"):
  base.copy_dir("../../../xml", "./xml")
  base.replaceInFile("./xml/libxml2/libxml.h", "xmlNop(void)", "xmlNop(void* context, char* buffer, int len)")
  base.replaceInFile("./xml/libxml2/xmlIO.c", "xmlNop(void)", "xmlNop(void* context, char* buffer, int len)")
  base.replaceInFile("./xml/src/xmllight_private.h", "#include \"../../common/", "#include \"../../../../../common/")
  base.replaceInFile("./xml/include/xmlutils.h", "#include \"../../common/", "#include \"../../../../../common/")
  
if not base.is_dir("freetype-2.10.4"):
  base.copy_dir("../../../freetype-2.10.4", "./freetype-2.10.4")
  # smooth
  base.copy_file("./freetype-2.10.4/src/smooth/ftgrays.c", "./freetype-2.10.4/src/smooth/ftgrays.cpp");
  apply_patch("./freetype-2.10.4/src/smooth/ftgrays.cpp", "./wasm/patches/ftgrays1.patch")
  apply_patch("./freetype-2.10.4/src/smooth/ftgrays.cpp", "./wasm/patches/ftgrays2.patch")
  apply_patch("./freetype-2.10.4/src/smooth/ftgrays.cpp", "./wasm/patches/ftgrays3.patch")
  base.copy_file("./freetype-2.10.4/src/smooth/smooth.c", "./freetype-2.10.4/src/smooth/smooth.cpp");
  apply_patch("./freetype-2.10.4/src/smooth/smooth.cpp", "./wasm/patches/smooth.patch")
  # ftobjs
  apply_patch("./freetype-2.10.4/src/base/ftobjs.c", "./wasm/patches/ftobjs1.patch")
  apply_patch("./freetype-2.10.4/src/base/ftobjs.c", "./wasm/patches/ftobjs2.patch")
  # ttcmap
  base.copy_file("./freetype-2.10.4/src/sfnt/ttcmap.c", "./freetype-2.10.4/src/sfnt/ttcmap.cpp");
  apply_patch("./freetype-2.10.4/src/sfnt/ttcmap.cpp", "./wasm/patches/ttcmap.patch")
  base.copy_file("./freetype-2.10.4/src/sfnt/sfnt.c", "./freetype-2.10.4/src/sfnt/sfnt.cpp");
  apply_patch("./freetype-2.10.4/src/sfnt/sfnt.cpp", "./wasm/patches/sfnt.patch")

# compile
compiler_flags = ["-O3",
                  # "-fno-rtti", cryptopp использует typeid
                  "-fexceptions",
                  "-Wno-unused-command-line-argument",
                  "-s WASM=1",
                  "-s ALLOW_MEMORY_GROWTH=1",
                  "-s FILESYSTEM=0",
                  "-s ENVIRONMENT='web'",
                  "-s LLD_REPORT_UNDEFINED"]

exported_functions = ["_malloc",
                      "_free",
                      "_GetType",
                      "_Open",
                      "_Close",
                      "_GetErrorCode",
                      "_GetInfo",
                      "_GetPixmap",
                      "_GetGlyphs",
                      "_GetLinks",
                      "_GetStructure",
                      "_InitializeFontsBin",
                      "_InitializeFontsBase64",
                      "_SetFontBinary",
                      "_IsFontBinaryExist"]

compile_files_array = []

compile_files_array.append("lib")
compile_files_array.append("./")
compile_files_array.append(["wasm/src/lib/wasm_jmp.cpp"])

compile_files_array.append("f")
compile_files_array.append("freetype-2.10.4/src/")
compile_files_array.append(["base/ftdebug.c","autofit/autofit.c","bdf/bdf.c","cff/cff.c","base/ftbase.c","base/ftbitmap.c","base/ftfstype.c","base/ftgasp.c","cache/ftcache.c","base/ftglyph.c","gzip/ftgzip.c","base/ftinit.c","lzw/ftlzw.c","base/ftstroke.c","base/ftsystem.c","smooth/smooth.cpp","base/ftbbox.c","base/ftbdf.c","base/ftcid.c","base/ftmm.c","base/ftpfr.c","base/ftsynth.c","base/fttype1.c","base/ftwinfnt.c","base/ftgxval.c","base/ftotval.c","base/ftpatent.c","pcf/pcf.c","pfr/pfr.c","psaux/psaux.c","pshinter/pshinter.c","psnames/psmodule.c","raster/raster.c","sfnt/sfnt.cpp","truetype/truetype.c","type1/type1.c","cid/type1cid.c","type42/type42.c","winfonts/winfnt.c"])

compile_files_array.append("g")
compile_files_array.append("../../")
compile_files_array.append(["GraphicsRenderer.cpp", "pro/pro_Graphics.cpp", "pro/pro_Fonts.cpp", "pro/pro_Image.cpp", "Graphics.cpp", "Brush.cpp", "BaseThread.cpp", "GraphicsPath.cpp", "Image.cpp", "Matrix.cpp", "Clip.cpp", "TemporaryCS.cpp"])

compile_files_array.append("fe")
compile_files_array.append("../../../fontengine/")
compile_files_array.append(["GlyphString.cpp", "FontManager.cpp", "FontFile.cpp", "FontPath.cpp", "ApplicationFonts.cpp"])

compile_files_array.append("a")
compile_files_array.append("../../../agg-2.4/src/")
compile_files_array.append(["agg_arc.cpp", "agg_vcgen_stroke.cpp", "agg_vcgen_dash.cpp", "agg_trans_affine.cpp", "agg_curves.cpp"])

compile_files_array.append("c")
compile_files_array.append("../../../common/")
compile_files_array.append(["File.cpp", "Directory.cpp", "ByteBuilder.cpp", "Base64.cpp", "StringExt.cpp"])

compile_files_array.append("i")
compile_files_array.append("../../../../Common/3dParty/icu/icu/source/common/")
compile_files_array.append(["ucnv.c", "ustr_wcs.cpp", "ucnv_err.c", "ucnv_bld.cpp", "ustrtrns.cpp", "ucnv_cb.c", "udata.cpp", "ucnv_io.cpp", "uhash.c", "udatamem.c", "cmemory.c", "ustring.cpp", "umutex.cpp", "putil.cpp", "ustr_cnv.cpp", "ucnvmbcs.cpp", "ucnvlat1.c", "ucnv_u16.c", "ucnv_u8.c", "ucnv_u32.c", "ucnv_u7.c", "ucln_cmn.cpp", "ucnv2022.cpp", "ucnv_lmb.c", "ucnvhz.c", "ucnvscsu.c", "ucnvisci.c", "ucnvbocu.cpp", "ucnv_ct.c", "ucnv_cnv.c", "stringpiece.cpp", "charstr.cpp", "umapfile.c", "ucmndata.c", "ucnv_ext.cpp", "uobject.cpp", "umath.c", "ubidi_props.c", "uchar.c", "uinvchar.c", "usprep.cpp", "unistr.cpp", "uniset_props.cpp", "loadednormalizer2impl.cpp", "filterednormalizer2.cpp", "utrie2.cpp", "normalizer2.cpp", "normalizer2impl.cpp", "utrie.cpp", "ucase.cpp", "uniset.cpp", "ruleiter.cpp", "parsepos.cpp", "util.cpp", "uprops.cpp", "uvector.cpp", "unames.cpp", "propname.cpp", "utrie2_builder.cpp", "unifunct.cpp", "bmpset.cpp", "unisetspan.cpp", "unifilt.cpp", "patternprops.cpp", "utf_impl.c", "ustrcase.cpp", "cstring.c", "bytestrie.cpp"])

compile_files_array.append("o")
compile_files_array.append("../../../../OfficeUtils/src/")
compile_files_array.append(["OfficeUtils.cpp", "ZipBuffer.cpp", "ZipUtilsCP.cpp"])

compile_files_array.append("m")
compile_files_array.append("../../../../OfficeUtils/src/zlib-1.2.11/contrib/minizip/")
compile_files_array.append(["ioapi.c", "miniunz.c", "minizip.c", "mztools.c", "unzip.c", "zip.c", "ioapibuf.c"])

compile_files_array.append("z")
compile_files_array.append("../../../../OfficeUtils/src/zlib-1.2.11/")
compile_files_array.append(["adler32.c", "crc32.c", "deflate.c", "infback.c", "inffast.c", "inflate.c", "inftrees.c", "trees.c", "zutil.c", "compress.c"])

compile_files_array.append("x")
compile_files_array.append("./")
compile_files_array.append(["xml/src/xmllight.cpp", "xml/src/xmldom.cpp", "xml/build/qt/libxml2_all.c", "xml/build/qt/libxml2_all2.c"])

compile_files_array.append("p")
compile_files_array.append("../../../../PdfReader/")
compile_files_array.append(["PdfReader.cpp", "Src/Adaptors.cpp", "Src/GfxClip.cpp", "Src/RendererOutputDev.cpp", "lib/fofi/FofiBase.cc", "lib/fofi/FofiEncodings.cc", "lib/fofi/FofiIdentifier.cc", "lib/fofi/FofiTrueType.cc", "lib/fofi/FofiType1.cc", "lib/fofi/FofiType1C.cc", "lib/goo/FixedPoint.cc", "lib/goo/gfile.cc", "lib/goo/GHash.cc", "lib/goo/GList.cc", "lib/goo/gmem.cc", "lib/goo/gmempp.cc", "lib/goo/GString.cc", "lib/goo/parseargs.c", "lib/goo/Trace.cc", "lib/splash/Splash.cc", "lib/splash/SplashBitmap.cc", "lib/splash/SplashClip.cc", "lib/splash/SplashFont.cc", "lib/splash/SplashFontEngine.cc", "lib/splash/SplashFontFile.cc", "lib/splash/SplashFontFileID.cc", "lib/splash/SplashFTFont.cc", "lib/splash/SplashFTFontEngine.cc", "lib/splash/SplashFTFontFile.cc", "lib/splash/SplashPath.cc", "lib/splash/SplashPattern.cc", "lib/splash/SplashScreen.cc", "lib/splash/SplashState.cc", "lib/splash/SplashXPath.cc", "lib/splash/SplashXPathScanner.cc", "lib/xpdf/AcroForm.cc", "lib/xpdf/Annot.cc", "lib/xpdf/Array.cc", "lib/xpdf/BuiltinFont.cc", "lib/xpdf/BuiltinFontTables.cc", "lib/xpdf/Catalog.cc", "lib/xpdf/CharCodeToUnicode.cc", "lib/xpdf/CMap.cc", "lib/xpdf/Decrypt.cc", "lib/xpdf/Dict.cc", "lib/xpdf/DisplayState.cc", "lib/xpdf/Error.cc", "lib/xpdf/FontEncodingTables.cc", "lib/xpdf/Function.cc", "lib/xpdf/Gfx.cc", "lib/xpdf/GfxFont.cc", "lib/xpdf/GfxState.cc", "lib/xpdf/GlobalParams.cc", "lib/xpdf/ImageOutputDev.cc", "lib/xpdf/JArithmeticDecoder.cc", "lib/xpdf/JBIG2Stream.cc", "lib/xpdf/JPXStream.cc", "lib/xpdf/Lexer.cc", "lib/xpdf/Link.cc", "lib/xpdf/NameToCharCode.cc", "lib/xpdf/Object.cc", "lib/xpdf/OptionalContent.cc", "lib/xpdf/Outline.cc", "lib/xpdf/OutputDev.cc", "lib/xpdf/Page.cc", "lib/xpdf/Parser.cc", "lib/xpdf/PDF417Barcode.cc", "lib/xpdf/PDFCore.cc", "lib/xpdf/PDFDoc.cc", "lib/xpdf/PDFDocEncoding.cc", "lib/xpdf/PreScanOutputDev.cc", "lib/xpdf/PSOutputDev.cc", "lib/xpdf/PSTokenizer.cc", "lib/xpdf/SecurityHandler.cc", "lib/xpdf/ShadingImage.cc", "lib/xpdf/SplashOutputDev.cc", "lib/xpdf/Stream.cc", "lib/xpdf/TextOutputDev.cc", "lib/xpdf/TextString.cc", "lib/xpdf/TileCache.cc", "lib/xpdf/TileCompositor.cc", "lib/xpdf/TileMap.cc", "lib/xpdf/UnicodeMap.cc", "lib/xpdf/UnicodeRemapping.cc", "lib/xpdf/UnicodeTypeTable.cc", "lib/xpdf/UTF8.cc", "lib/xpdf/WebFont.cc", "lib/xpdf/XFAScanner.cc", "lib/xpdf/XRef.cc", "lib/xpdf/Zoox.cc"])

compile_files_array.append("x")
compile_files_array.append("../../../../XpsFile/")
compile_files_array.append(["XpsFile.cpp", "XpsLib/Document.cpp", "XpsLib/XpsPage.cpp", "XpsLib/StaticResources.cpp", "XpsLib/Utils.cpp", "XpsLib/WString.cpp", "XpsLib/ContextState.cpp"])

compile_files_array.append("d")
compile_files_array.append("../../../../DjVuFile/libdjvu/")
compile_files_array.append(["../DjVu.cpp", "../DjVuFileImplementation.cpp", "Arrays.cpp", "BSByteStream.cpp", "BSEncodeByteStream.cpp", "ByteStream.cpp", "DataPool.cpp", "debug.cpp", "DjVmDir.cpp", "DjVmDir0.cpp", "DjVmDoc.cpp", "DjVmNav.cpp", "DjVuAnno.cpp", "DjVuDocEditor.cpp", "DjVuDocument.cpp", "DjVuDumpHelper.cpp", "DjVuErrorList.cpp", "DjVuFile.cpp", "DjVuFileCache.cpp", "DjVuGlobal.cpp", "DjVuGlobalMemory.cpp", "DjVuImage.cpp", "DjVuInfo.cpp", "DjVuMessageLite.cpp", "DjVuNavDir.cpp", "DjVuPalette.cpp", "DjVuPort.cpp", "DjVuText.cpp", "DjVuToPS.cpp", "GBitmap.cpp", "GContainer.cpp", "GException.cpp", "GIFFManager.cpp", "GMapAreas.cpp", "GPixmap.cpp", "GRect.cpp", "GScaler.cpp", "GSmartPointer.cpp", "DjVuGString.cpp", "GThreads.cpp", "GUnicode.cpp", "IFFByteStream.cpp", "IW44EncodeCodec.cpp", "IW44Image.cpp", "JB2EncodeCodec.cpp", "JB2Image.cpp", "JPEGDecoder.cpp", "MMRDecoder.cpp", "MMX.cpp", "UnicodeByteStream.cpp", "XMLParser.cpp", "XMLTags.cpp", "ZPCodec.cpp"])

compile_files_array.append("d")
compile_files_array.append("../../../../DjVuFile/wasm/libdjvu/")
compile_files_array.append(["atomic.cpp", "DjVuMessage.cpp", "GOS.cpp", "GURL.cpp"])

compile_files_array.append("u")
compile_files_array.append("../../../../UnicodeConverter/")
compile_files_array.append(["UnicodeConverter.cpp"])

compiler_flags.append("-Iwasm/src/lib -I../../../agg-2.4/include -I../../../cximage/jasper/include -I../../../cximage/jpeg -I../../../cximage/png -Ifreetype-2.10.4/include -Ifreetype-2.10.4/include/freetype -I../../../../OfficeUtils/src/zlib-1.2.11 -I../../../../Common/3dParty/icu/icu/source/common -I../../../xml/libxml2/include -I../../../xml/build/qt -I../../../../OfficeUtils/src/zlib-1.2.11/contrib/minizip -I../../../../PdfReader/lib/goo -I../../../../PdfReader/lib/fofi -I../../../../PdfReader/lib/splash -I../../../../PdfReader/lib")
compiler_flags.append("-D__linux__ -D_LINUX -DUNIX -DFT2_BUILD_LIBRARY -DHAVE_FCNTL_H -DFT_CONFIG_OPTION_SYSTEM_ZLIB -DBUILDING_WASM_MODULE -DU_COMMON_IMPLEMENTATION")
compiler_flags.append("-Derrno=0 -DTHREADMODEL=0 -DDEBUGLVL=0 -DHAVE_MBSTATE_T -DHAVE_STDINCLUDES -DHAS_WCHAR")
compiler_flags.append("-DHAVE_VA_COPY -DLIBXML_READER_ENABLED -DLIBXML_PUSH_ENABLED -DLIBXML_HTML_ENABLED -DLIBXML_XPATH_ENABLED -DLIBXML_OUTPUT_ENABLED -DLIBXML_C14N_ENABLED -DLIBXML_SAX1_ENABLED -DLIBXML_TREE_ENABLED -DLIBXML_XPTR_ENABLED -DIN_LIBXML -DLIBXML_STATIC -DBUILD_ZLIB_AS_SOURCES -DDISABLE_PDF_CONVERTATION -D_ARM_ALIGN_")

# arguments
arguments = ""
for item in compiler_flags:
  arguments += (item + " ")

# sources
sources = ["raster.o", "wasm/src/drawingfile.cpp", "wasm/src/metafile.cpp"]

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

arguments += "-s EXPORTED_FUNCTIONS=\"["
for item in exported_functions:
  arguments += ("'" + item + "',")
arguments = arguments[:-1]
arguments += "]\" "

for item in sources:
  arguments += (item + " ")

external_file.append(prefix_call + "emcc -o drawingfile.js " + arguments + libs)

base.replaceInFile("../../../../Common/3dParty/icu/icu/source/common/udata.cpp", "\n{\n    UDataMemory tData;", "\n{\n#ifdef BUILDING_WASM_MODULE\nreturn NULL;\n#endif\n    UDataMemory tData;")
base.run_as_bat(external_file)
base.replaceInFile("../../../../Common/3dParty/icu/icu/source/common/udata.cpp", "\n{\n#ifdef BUILDING_WASM_MODULE\nreturn NULL;\n#endif\n    UDataMemory tData;", "\n{\n    UDataMemory tData;")

# finalize
base.replaceInFile("./drawingfile.js", "function getBinaryPromise()", "function getBinaryPromise2()")
base.replaceInFile("./drawingfile.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){window[\"AscViewer\"] && window[\"AscViewer\"][\"onLoadModule\"] && window[\"AscViewer\"][\"onLoadModule\"]();}];")
base.replaceInFile("./drawingfile.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){window[\"AscViewer\"] && window[\"AscViewer\"][\"onLoadModule\"] && window[\"AscViewer\"][\"onLoadModule\"]();}];")
base.replaceInFile("./drawingfile.js", "\"drawingfile.js.mem\"", "getMemoryPathIE(\"drawingfile.js.mem\")")

module_js_content = base.readFile("./drawingfile.js")
engine_base_js_content = base.readFile("./wasm/js/drawingfile_base.js")
string_utf8_content    = base.readFile("./../../../../Common/js/string_utf8.js")
engine_js_content = engine_base_js_content.replace("//module", module_js_content)
engine_js_content = engine_js_content.replace("//string_utf8", string_utf8_content)

# write new version
base.writeFile("./deploy/drawingfile.js", engine_js_content)
base.copy_file("./drawingfile.wasm", "./deploy/drawingfile.wasm")

# clear
base.delete_file("drawingfile.js")
base.delete_file("drawingfile.wasm")

# ie asm version
arguments = arguments.replace("WASM=1", "WASM=0")

# command
external_file = []

if base.host_platform() == "windows":
  external_file.append("call emsdk/emsdk_env.bat")
else:
  external_file.append("#!/bin/bash")
  external_file.append("source ./emsdk/emsdk_env.sh")

external_file.append(prefix_call + "emcc -o drawingfile.js " + arguments + libs)
base.run_as_bat(external_file)

# finalize
base.replaceInFile("./drawingfile.js", "function getBinaryPromise()", "function getBinaryPromise2()")
base.replaceInFile("./drawingfile.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){window[\"AscViewer\"] && window[\"AscViewer\"][\"onLoadModule\"] && window[\"AscViewer\"][\"onLoadModule\"]();}];")
base.replaceInFile("./drawingfile.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){window[\"AscViewer\"] && window[\"AscViewer\"][\"onLoadModule\"] && window[\"AscViewer\"][\"onLoadModule\"]();}];")
base.replaceInFile("./drawingfile.js", "\"drawingfile.js.mem\"", "getMemoryPathIE(\"drawingfile.js.mem\")")

module_js_content = base.readFile("./drawingfile.js")
engine_base_js_content = base.readFile("./wasm/js/drawingfile_base.js")
string_utf8_content    = base.readFile("./../../../../Common/js/string_utf8.js")
polyfill_js_content    = base.readFile("./../../../../Common/js/polyfill.js")
engine_js_content = engine_base_js_content.replace("//module", module_js_content)
engine_js_content = engine_js_content.replace("//string_utf8", string_utf8_content)
engine_js_content = engine_js_content.replace("//polyfill",    polyfill_js_content)

# write new version
base.writeFile("./deploy/drawingfile_ie.js", engine_js_content)
base.copy_file("./drawingfile.js.mem", "./deploy/drawingfile.js.mem")

# clear
base.delete_file("drawingfile.js")
base.delete_file("drawingfile.js.mem")
# base.delete_dir("./o")
# base.delete_dir("./xml")
# base.delete_dir("./freetype-2.10.4")
# base.delete_file("./raster.o")
