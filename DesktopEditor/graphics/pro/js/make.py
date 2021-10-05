import sys
sys.path.append("../../../../../build_tools/scripts")
import base
import os

if not base.is_file("raster.o"):
    base.cmd("python", ["raster_make.py"])
if not base.is_file("raster.o"):
    print("raster_make.py error")
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

if not base.is_dir("xml"):
   base.copy_dir("../../../xml", "./xml")
   base.replaceInFile("./xml/libxml2/libxml.h", "xmlNop(void)", "xmlNop(void* context, char* buffer, int len)")
   base.replaceInFile("./xml/libxml2/xmlIO.c", "xmlNop(void)", "xmlNop(void* context, char* buffer, int len)")
   base.replaceInFile("./xml/src/xmllight_private.h", "#include \"../../common/", "#include \"../../../../../common/")
   base.replaceInFile("./xml/include/xmlutils.h", "#include \"../../common/", "#include \"../../../../../common/")

# compile
compiler_flags = ["-O3",
                  # "-fno-rtti", cryptopp использует typeid
                  "-s WASM=1",
                  "-s ALLOW_MEMORY_GROWTH=1",
                  "-s FILESYSTEM=0",
                  "-s ENVIRONMENT='web'"]
                  #"-s LLD_REPORT_UNDEFINED"]

exported_functions = ["_malloc",
                      "_free",
                      "_GetType",
                      "_Open",
                      "_Close",
                      "_GetInfo",
                      "_GetPixmap",
                      "_GetGlyphs",
                      "_GetLinks",
                      "_GetStructure"]

libGraphics_src_path = "../../"
input_graphics_sources = ["GraphicsRenderer.cpp", "pro/pro_Graphics.cpp", "pro/pro_Fonts.cpp", "pro/pro_Image.cpp", "Graphics.cpp", "Brush.cpp", "BaseThread.cpp", "GraphicsPath.cpp", "Image.cpp", "Matrix.cpp", "Clip.cpp", "TemporaryCS.cpp"]

libFontEngine_src_path = "../../../fontengine/"
input_fontengine_sources = ["GlyphString.cpp", "FontManager.cpp", "FontFile.cpp", "FontPath.cpp", "ApplicationFonts.cpp"]

libAgg_src_path = "../../../agg-2.4/src/"
input_agg_sources = ["agg_arc.cpp", "agg_vcgen_stroke.cpp", "agg_vcgen_dash.cpp", "agg_trans_affine.cpp", "agg_curves.cpp"]

libFreetype_src_path = "../../../freetype-2.10.4/src/"
input_freetype_sources = ["base/ftinit.c", "base/ftlcdfil.c", "base/ftobjs.c", "base/ftglyph.c", "base/ftoutln.c", "base/ftutil.c", "base/ftgloadr.c", "base/ftfntfmt.c", "base/ftcalc.c", "base/ftbitmap.c", "base/ftstream.c", "base/fthash.c", "base/ftdebug.c", "base/fttrigon.c", "base/ftadvanc.c", "base/ftpsprop.c", "base/ftrfork.c", "bdf/bdfdrivr.c", "bdf/bdflib.c", "smooth/ftsmooth.c", "smooth/ftgrays.c", "../builds/unix/ftsystem.c", "autofit/afmodule.c", "autofit/afhints.c", "autofit/afloader.c", "autofit/afglobal.c", "autofit/afshaper.c", "autofit/afranges.c", "autofit/afdummy.c", "autofit/aflatin.c", "autofit/afcjk.c", "autofit/afindic.c", "autofit/afangles.c", "autofit/afblue.c", "autofit/afwarp.c", "truetype/ttdriver.c", "truetype/ttgload.c", "truetype/ttpload.c", "truetype/ttobjs.c", "truetype/ttgxvar.c", "truetype/ttinterp.c", "type1/t1driver.c", "type1/t1afm.c", "type1/t1load.c", "type1/t1gload.c", "type1/t1objs.c", "type1/t1parse.c", "cff/cffdrivr.c", "cff/cffgload.c", "cff/cffload.c", "cff/cffcmap.c", "cff/cffparse.c", "cff/cffobjs.c", "cid/cidriver.c", "cid/cidobjs.c", "cid/cidgload.c", "cid/cidload.c", "cid/cidparse.c", "pfr/pfrdrivr.c", "pfr/pfrobjs.c", "pfr/pfrload.c", "pfr/pfrgload.c", "pfr/pfrcmap.c", "pfr/pfrsbit.c", "type42/t42drivr.c", "type42/t42objs.c", "type42/t42parse.c", "winfonts/winfnt.c", "pcf/pcfdrivr.c", "pcf/pcfread.c", "pcf/pcfutil.c", "psaux/psauxmod.c", "psaux/psobjs.c", "psaux/t1decode.c", "psaux/psft.c", "psaux/afmparse.c", "psaux/t1cmap.c", "psaux/cffdecode.c", "psaux/psconv.c", "psaux/psfont.c", "psaux/psblues.c", "psaux/psintrp.c", "psaux/pserror.c", "psaux/psstack.c", "psaux/pshints.c", "psaux/psarrst.c", "psaux/psread.c", "psnames/psmodule.c", "pshinter/pshmod.c", "pshinter/pshrec.c", "pshinter/pshglob.c", "pshinter/pshalgo.c", "raster/ftrend1.c", "raster/ftraster.c", "sfnt/sfdriver.c", "sfnt/ttpost.c", "sfnt/sfobjs.c", "sfnt/ttload.c", "sfnt/ttbdf.c", "sfnt/ttmtx.c", "sfnt/ttkern.c", "sfnt/sfwoff.c", "sfnt/ttcmap.c", "sfnt/ttsbit.c", "sfnt/sfwoff2.c", "sfnt/ttcolr.c", "sfnt/woff2tags.c", "sfnt/ttcpal.c", "gzip/ftgzip.c", "lzw/ftlzw.c"]

libCommon_src_path = "../../../common/"
input_common_sources = ["File.cpp", "Directory.cpp", "ByteBuilder.cpp", "Base64.cpp", "StringExt.cpp"]

libIcu_src_path = "../../../../Common/3dParty/icu/icu/source/common/"
input_icu_sources = ["ucnv.c", "ustr_wcs.cpp", "ucnv_err.c", "ucnv_bld.cpp", "ustrtrns.cpp", "ucnv_cb.c", "udata.cpp", "ucnv_io.cpp", "uhash.c", "udatamem.c", "cmemory.c", "ustring.cpp", "umutex.cpp", "putil.cpp", "ustr_cnv.cpp", "ucnvmbcs.cpp", "ucnvlat1.c", "ucnv_u16.c", "ucnv_u8.c", "ucnv_u32.c", "ucnv_u7.c", "ucln_cmn.cpp", "ucnv2022.cpp", "ucnv_lmb.c", "ucnvhz.c", "ucnvscsu.c", "ucnvisci.c", "ucnvbocu.cpp", "ucnv_ct.c", "ucnv_cnv.c", "stringpiece.cpp", "charstr.cpp", "umapfile.c", "ucmndata.c", "ucnv_ext.cpp", "uobject.cpp", "umath.c", "ubidi_props.c", "uchar.c", "uinvchar.c", "usprep.cpp", "unistr.cpp", "uniset_props.cpp", "loadednormalizer2impl.cpp", "filterednormalizer2.cpp", "utrie2.cpp", "normalizer2.cpp", "normalizer2impl.cpp", "utrie.cpp", "ucase.cpp", "uniset.cpp", "ruleiter.cpp", "parsepos.cpp", "util.cpp", "uprops.cpp", "uvector.cpp", "unames.cpp", "propname.cpp", "utrie2_builder.cpp", "unifunct.cpp", "bmpset.cpp", "unisetspan.cpp", "unifilt.cpp", "patternprops.cpp", "utf_impl.c", "ustrcase.cpp", "cstring.c", "bytestrie.cpp"]

libXps_src_path = "../../../../XpsFile/XpsLib/"
input_xps_sources = ["Document.cpp", "XpsPage.cpp", "StaticResources.cpp", "Utils.cpp", "WString.cpp", "ContextState.cpp"]

libDjVu_src_path = "../../../../DjVuFile/libdjvu/"
input_djvu_sources = ["Arrays.cpp", "BSByteStream.cpp", "BSEncodeByteStream.cpp", "ByteStream.cpp", "DataPool.cpp", "debug.cpp", "DjVmDir.cpp", "DjVmDir0.cpp", "DjVmDoc.cpp", "DjVmNav.cpp", "DjVuAnno.cpp", "DjVuDocEditor.cpp", "DjVuDocument.cpp", "DjVuDumpHelper.cpp", "DjVuErrorList.cpp", "DjVuFile.cpp", "DjVuFileCache.cpp", "DjVuGlobal.cpp", "DjVuGlobalMemory.cpp", "DjVuImage.cpp", "DjVuInfo.cpp", "DjVuMessageLite.cpp", "DjVuNavDir.cpp", "DjVuPalette.cpp", "DjVuPort.cpp", "DjVuText.cpp", "DjVuToPS.cpp", "GBitmap.cpp", "GContainer.cpp", "GException.cpp", "GIFFManager.cpp", "GMapAreas.cpp", "GPixmap.cpp", "GRect.cpp", "GScaler.cpp", "GSmartPointer.cpp", "DjVuGString.cpp", "GThreads.cpp", "GUnicode.cpp", "IFFByteStream.cpp", "IW44EncodeCodec.cpp", "IW44Image.cpp", "JB2EncodeCodec.cpp", "JB2Image.cpp", "JPEGDecoder.cpp", "MMRDecoder.cpp", "MMX.cpp", "UnicodeByteStream.cpp", "XMLParser.cpp", "XMLTags.cpp", "ZPCodec.cpp"]

libWasmDjVu_src_path = "../../../../DjVuFile/wasm/libdjvu/"
input_wasmdjvu_sources = ["atomic.cpp", "DjVuMessage.cpp", "GOS.cpp", "GURL.cpp"]

libOfficeUtils_src_path = "../../../../OfficeUtils/src/"
input_officeutils_sources = ["OfficeUtils.cpp", "ZipBuffer.cpp", "ZipUtilsCP.cpp"]

libMinizip_src_path = "../../../../OfficeUtils/src/zlib-1.2.11/contrib/minizip/"
input_minizip_sources = ["ioapi.c", "miniunz.c", "minizip.c", "mztools.c", "unzip.c", "zip.c", "ioapibuf.c"]

libZlib_src_path = "../../../../OfficeUtils/src/zlib-1.2.11/"
input_zlib_sources = ["adler32.c", "crc32.c", "deflate.c", "infback.c", "inffast.c", "inflate.c", "inftrees.c", "trees.c", "zutil.c", "compress.c"]

input_xml_sources = ["xml/src/xmllight.cpp", "xml/src/xmldom.cpp", "xml/build/qt/libxml2_all.c", "xml/build/qt/libxml2_all2.c"]

libCryptoPP_src_path = "../../../../Common/3dParty/cryptopp/"
input_cryptopp_sources = ["cryptlib.cpp", #"cpu.cpp", 
"integer.cpp", #"3way.cpp", "adler32.cpp", "algebra.cpp", 
"algparam.cpp", "arc4.cpp", #"aria-simd.cpp", "aria.cpp", "ariatab.cpp", 
"asn.cpp", #"authenc.cpp", "base32.cpp", "base64.cpp", "basecode.cpp", "bfinit.cpp", "blake2-simd.cpp", "blake2.cpp", "blowfish.cpp", "blumshub.cpp", "camellia.cpp", "cast.cpp", "casts.cpp", "cbcmac.cpp", "ccm.cpp", "chacha.cpp", "channels.cpp", "cmac.cpp", "crc-simd.cpp", "crc.cpp", "default.cpp", "des.cpp", "dessp.cpp", "dh.cpp", "dh2.cpp", "dll.cpp", "dsa.cpp", "eax.cpp", "ec2n.cpp", "eccrypto.cpp", "ecp.cpp", "elgamal.cpp", "emsa2.cpp", "eprecomp.cpp", "esign.cpp", "files.cpp", 
"filters.cpp", "fips140.cpp", #"fipstest.cpp", "gcm-simd.cpp", "gcm.cpp", "gf256.cpp", "gf2_32.cpp", "gf2n.cpp", "gfpcrypt.cpp", "gost.cpp", "gzip.cpp", "hex.cpp", "hmac.cpp", "hrtimer.cpp", "ida.cpp", "idea.cpp", 
"iterhash.cpp", #"kalyna.cpp", "kalynatab.cpp", "keccak.cpp", "luc.cpp", "mars.cpp", "marss.cpp", "md2.cpp", "md4.cpp", 
"md5.cpp", "misc.cpp", "modes.cpp", "mqueue.cpp", #"mqv.cpp", "nbtheory.cpp", "neon-simd.cpp", "network.cpp", "oaep.cpp", "osrng.cpp", "panama.cpp", "pkcspad.cpp", "poly1305.cpp", "polynomi.cpp", "pssr.cpp", "pubkey.cpp", 
"queue.cpp", #"rabin.cpp", "randpool.cpp", "rc2.cpp", "rc5.cpp", "rc6.cpp", "rdrand.cpp", 
"rdtables.cpp", "rijndael.cpp", #"ripemd.cpp", "rng.cpp", "rsa.cpp", "rw.cpp", "safer.cpp", "salsa.cpp", "seal.cpp", "seed.cpp", "serpent.cpp", "sha-simd.cpp", 
"sha.cpp"] #"sha3.cpp", "shacal2-simd.cpp", "shacal2.cpp", "shark.cpp", "sharkbox.cpp", "skipjack.cpp", "socketft.cpp", "sosemanuk.cpp", "square.cpp", "squaretb.cpp", "sse-simd.cpp", "strciphr.cpp", "tea.cpp", "tftables.cpp", "threefish.cpp", "tiger.cpp", "tigertab.cpp", "trdlocal.cpp", "ttmac.cpp", "twofish.cpp", "vmac.cpp", "wait.cpp", "wake.cpp", "whrlpool.cpp", "xtr.cpp", "xtrcrypt.cpp", "zdeflate.cpp", "zinflate.cpp", "zlib.cpp"]

libPdfReader_src_path = "../../../../PdfReader/"
input_pdfreader_sources = ["Src/Adaptors.cpp", "Src/GfxClip.cpp", "Src/RendererOutputDev.cpp", "lib/fofi/FofiBase.cc", "lib/fofi/FofiEncodings.cc", "lib/fofi/FofiIdentifier.cc", "lib/fofi/FofiTrueType.cc", "lib/fofi/FofiType1.cc", "lib/fofi/FofiType1C.cc", "lib/goo/FixedPoint.cc", "lib/goo/gfile.cc", "lib/goo/GHash.cc", "lib/goo/GList.cc", "lib/goo/gmem.cc", "lib/goo/gmempp.cc", "lib/goo/GString.cc", "lib/goo/parseargs.c", "lib/goo/Trace.cc", "lib/splash/Splash.cc", "lib/splash/SplashBitmap.cc", "lib/splash/SplashClip.cc", "lib/splash/SplashFont.cc", "lib/splash/SplashFontEngine.cc", "lib/splash/SplashFontFile.cc", "lib/splash/SplashFontFileID.cc", "lib/splash/SplashFTFont.cc", "lib/splash/SplashFTFontEngine.cc", "lib/splash/SplashFTFontFile.cc", "lib/splash/SplashPath.cc", "lib/splash/SplashPattern.cc", "lib/splash/SplashScreen.cc", "lib/splash/SplashState.cc", "lib/splash/SplashXPath.cc", "lib/splash/SplashXPathScanner.cc", "lib/xpdf/AcroForm.cc", "lib/xpdf/Annot.cc", "lib/xpdf/Array.cc", "lib/xpdf/BuiltinFont.cc", "lib/xpdf/BuiltinFontTables.cc", "lib/xpdf/Catalog.cc", "lib/xpdf/CharCodeToUnicode.cc", "lib/xpdf/CMap.cc", "lib/xpdf/Decrypt.cc", "lib/xpdf/Dict.cc", "lib/xpdf/DisplayState.cc", "lib/xpdf/Error.cc", "lib/xpdf/FontEncodingTables.cc", "lib/xpdf/Function.cc", "lib/xpdf/Gfx.cc", "lib/xpdf/GfxFont.cc", "lib/xpdf/GfxState.cc", "lib/xpdf/GlobalParams.cc", "lib/xpdf/ImageOutputDev.cc", "lib/xpdf/JArithmeticDecoder.cc", "lib/xpdf/JBIG2Stream.cc", "lib/xpdf/JPXStream.cc", "lib/xpdf/Lexer.cc", "lib/xpdf/Link.cc", "lib/xpdf/NameToCharCode.cc", "lib/xpdf/Object.cc", "lib/xpdf/OptionalContent.cc", "lib/xpdf/Outline.cc", "lib/xpdf/OutputDev.cc", "lib/xpdf/Page.cc", "lib/xpdf/Parser.cc", "lib/xpdf/PDF417Barcode.cc", "lib/xpdf/PDFCore.cc", "lib/xpdf/PDFDoc.cc", "lib/xpdf/PDFDocEncoding.cc", "lib/xpdf/PreScanOutputDev.cc", "lib/xpdf/PSOutputDev.cc", "lib/xpdf/PSTokenizer.cc", "lib/xpdf/SecurityHandler.cc", "lib/xpdf/ShadingImage.cc", "lib/xpdf/SplashOutputDev.cc", "lib/xpdf/Stream.cc", "lib/xpdf/TextOutputDev.cc", "lib/xpdf/TextString.cc", "lib/xpdf/TileCache.cc", "lib/xpdf/TileCompositor.cc", "lib/xpdf/TileMap.cc", "lib/xpdf/UnicodeMap.cc", "lib/xpdf/UnicodeRemapping.cc", "lib/xpdf/UnicodeTypeTable.cc", "lib/xpdf/UTF8.cc", "lib/xpdf/WebFont.cc", "lib/xpdf/XFAScanner.cc", "lib/xpdf/XRef.cc", "lib/xpdf/Zoox.cc"]

input_libs_sources = ["../../../../UnicodeConverter/UnicodeConverter.cpp", "../../../../XpsFile/XpsFile.cpp", "../../../../DjVuFile/DjVu.cpp", "../../../../DjVuFile/DjVuFileImplementation.cpp", "../../../../PdfWriter/PdfRenderer.cpp", "../../../../HtmlRenderer/src/HTMLRenderer3.cpp", "../../../../PdfReader/PdfReader.cpp"]

# sources
sources = []
# graphics
# fontengine
# agg
# freetype
# common
# icu
# xps
# djvu
# wasm djvu
# officeutils
# minizip
# zlib
# xml
# cryptopp
# pdfReader
# libs
sources.append("raster.o")
sources.append("wasm/src/drawingfile.cpp")

compiler_flags.append("-I../../../agg-2.4/include -I../../../cximage/jasper/include -I../../../cximage/jpeg -I../../../cximage/png -I../../../freetype-2.10.4/include -I../../../freetype-2.10.4/include/freetype -I../../../../OfficeUtils/src/zlib-1.2.11 -I../../../../Common/3dParty/icu/icu/source/common -I../../../xml/libxml2/include -I../../../xml/build/qt -I../../../../OfficeUtils/src/zlib-1.2.11/contrib/minizip -I../../../../PdfReader/lib/goo -I../../../../PdfReader/lib/fofi -I../../../../PdfReader/lib/splash -I../../../../PdfReader/lib")
compiler_flags.append("-D__linux__ -D_LINUX -DUNIX -DFT2_BUILD_LIBRARY -DHAVE_FCNTL_H -DFT_CONFIG_OPTION_SYSTEM_ZLIB -DBUILDING_WASM_MODULE -DU_COMMON_IMPLEMENTATION")
compiler_flags.append("-DWASM_MODE -Derrno=0 -DTHREADMODEL=0 -DDEBUGLVL=0 -DHAVE_MBSTATE_T -DHAVE_STDINCLUDES -DHAS_WCHAR")
compiler_flags.append("-DHAVE_VA_COPY -DLIBXML_READER_ENABLED -DLIBXML_PUSH_ENABLED -DLIBXML_HTML_ENABLED -DLIBXML_XPATH_ENABLED -DLIBXML_OUTPUT_ENABLED -DLIBXML_C14N_ENABLED -DLIBXML_SAX1_ENABLED -DLIBXML_TREE_ENABLED -DLIBXML_XPTR_ENABLED -DIN_LIBXML -DLIBXML_STATIC -DBUILD_ZLIB_AS_SOURCES -DCRYPTOPP_DISABLE_ASM")

# arguments
arguments = ""
for item in compiler_flags:
    arguments += (item + " ")

# command
windows_bat = []
if base.host_platform() == "windows":
    windows_bat.append("call emsdk/emsdk_env.bat") 

    libs = ""
    for item in input_graphics_sources:
        windows_bat.append("call emcc -o temp/" + os.path.splitext(os.path.basename(item))[0] + ".o -c " + arguments + libGraphics_src_path + item)
        libs += ("temp/" + os.path.splitext(os.path.basename(item))[0] + ".o ")
    
    for item in input_fontengine_sources:
        windows_bat.append("call emcc -o temp/" + os.path.splitext(item)[0] + ".o -c " + arguments + libFontEngine_src_path + item)
        libs += ("temp/" + os.path.splitext(item)[0] + ".o ")
    
    for item in input_agg_sources:
        windows_bat.append("call emcc -o temp/" + os.path.splitext(item)[0] + ".o -c " + arguments + libAgg_src_path + item)
        libs += ("temp/" + os.path.splitext(item)[0] + ".o ")
    
    for item in input_freetype_sources:
        windows_bat.append("call emcc -o temp/" + os.path.splitext(os.path.basename(item))[0] + ".o -c " + arguments + libFreetype_src_path + item)
        libs += ("temp/" + os.path.splitext(os.path.basename(item))[0] + ".o ")
    
    for item in input_common_sources:
        windows_bat.append("call emcc -o temp/" + os.path.splitext(item)[0] + ".o -c " + arguments + libCommon_src_path + item)
        libs += ("temp/" + os.path.splitext(item)[0] + ".o ")
    
    for item in input_icu_sources:
        windows_bat.append("call emcc -o temp/" + os.path.splitext(item)[0] + ".o -c " + arguments + libIcu_src_path + item)
        libs += ("temp/" + os.path.splitext(item)[0] + ".o ")
    
    for item in input_xps_sources:
        windows_bat.append("call emcc -o temp/" + os.path.splitext(item)[0] + ".o -c " + arguments + libXps_src_path + item)
        libs += ("temp/" + os.path.splitext(item)[0] + ".o ")
    
    for item in input_djvu_sources:
        windows_bat.append("call emcc -o temp/" + os.path.splitext(item)[0] + ".o -c " + arguments + libDjVu_src_path + item)
        libs += ("temp/" + os.path.splitext(item)[0] + ".o ")
    
    for item in input_wasmdjvu_sources:
        windows_bat.append("call emcc -o temp/" + os.path.splitext(item)[0] + ".o -c " + arguments + libWasmDjVu_src_path + item)
        libs += ("temp/" + os.path.splitext(item)[0] + ".o ")
    
    for item in input_officeutils_sources:
        windows_bat.append("call emcc -o temp/" + os.path.splitext(item)[0] + ".o -c " + arguments + libOfficeUtils_src_path + item)
        libs += ("temp/" + os.path.splitext(item)[0] + ".o ")
    
    for item in input_minizip_sources:
        windows_bat.append("call emcc -o temp/" + os.path.splitext(item)[0] + ".o -c " + arguments + libMinizip_src_path + item)
        libs += ("temp/" + os.path.splitext(item)[0] + ".o ")
    
    for item in input_zlib_sources:
        windows_bat.append("call emcc -o temp/" + os.path.splitext(item)[0] + ".o -c " + arguments + libZlib_src_path + item)
        libs += ("temp/" + os.path.splitext(item)[0] + ".o ")
    
    for item in input_xml_sources:
        windows_bat.append("call emcc -o temp/" + os.path.splitext(os.path.basename(item))[0] + ".o -c " + arguments + item)
        libs += ("temp/" + os.path.splitext(os.path.basename(item))[0] + ".o ")
    
    for item in input_cryptopp_sources:
        windows_bat.append("call emcc -o temp/" + os.path.splitext(item)[0] + ".o -c " + arguments + libCryptoPP_src_path + item)
        libs += ("temp/" + os.path.splitext(item)[0] + ".o ")
    
    for item in input_pdfreader_sources:
        windows_bat.append("call emcc -o temp/" + os.path.splitext(os.path.basename(item))[0] + ".o -c " + arguments + libPdfReader_src_path + item)
        libs += ("temp/" + os.path.splitext(os.path.basename(item))[0] + ".o ")

    for item in input_libs_sources:
        windows_bat.append("call emcc -o temp/" + os.path.splitext(os.path.basename(item))[0] + ".o -c " + arguments + item)
        libs += ("temp/" + os.path.splitext(os.path.basename(item))[0] + ".o ")

    arguments += "-s EXPORTED_FUNCTIONS=\"["
    for item in exported_functions:
        arguments += ("'" + item + "',")
    arguments = arguments[:-1]
    arguments += "]\" "

    for item in sources:
        arguments += (item + " ")

    windows_bat.append("call emcc -o drawingfile.js " + arguments + libs)
else:
    windows_bat.append("#!/bin/bash")
    windows_bat.append("source ./emsdk/emsdk_env.sh")

    libs = ""
    for item in input_graphics_sources:
        windows_bat.append("emcc -o temp/" + os.path.basename(item) + ".o -c " + arguments + libGraphics_src_path + item)
        libs += ("temp/" + os.path.basename(item) + ".o ")
    
    for item in input_fontengine_sources:
        windows_bat.append("emcc -o temp/" + item + ".o -c " + arguments + libFontEngine_src_path + item)
        libs += ("temp/" + item + ".o ")
    
    for item in input_agg_sources:
        windows_bat.append("emcc -o temp/" + item + ".o -c " + arguments + libAgg_src_path + item)
        libs += ("temp/" + item + ".o ")
    
    for item in input_freetype_sources:
        windows_bat.append("emcc -o temp/" + os.path.basename(item) + ".o -c " + arguments + libFreetype_src_path + item)
        libs += ("temp/" + os.path.basename(item) + ".o ")
    
    for item in input_common_sources:
        windows_bat.append("emcc -o temp/" + item + ".o -c " + arguments + libCommon_src_path + item)
        libs += ("temp/" + item + ".o ")
    
    for item in input_icu_sources:
        windows_bat.append("emcc -o temp/" + item + ".o -c " + arguments + libIcu_src_path + item)
        libs += ("temp/" + item + ".o ")
    
    for item in input_xps_sources:
        windows_bat.append("emcc -o temp/" + item + ".o -c " + arguments + libXps_src_path + item)
        libs += ("temp/" + item + ".o ")
    
    for item in input_djvu_sources:
        windows_bat.append("emcc -o temp/" + item + ".o -c " + arguments + libDjVu_src_path + item)
        libs += ("temp/" + item + ".o ")
    
    for item in input_wasmdjvu_sources:
        windows_bat.append("emcc -o temp/" + item + ".o -c " + arguments + libWasmDjVu_src_path + item)
        libs += ("temp/" + item + ".o ")
    
    for item in input_officeutils_sources:
        windows_bat.append("emcc -o temp/" + item + ".o -c " + arguments + libOfficeUtils_src_path + item)
        libs += ("temp/" + item + ".o ")
    
    for item in input_minizip_sources:
        windows_bat.append("emcc -o temp/" + item + ".o -c " + arguments + libMinizip_src_path + item)
        libs += ("temp/" + item + ".o ")
    
    for item in input_zlib_sources:
        windows_bat.append("emcc -o temp/" + item + ".o -c " + arguments + libZlib_src_path + item)
        libs += ("temp/" + item + ".o ")
    
    for item in input_xml_sources:
        windows_bat.append("emcc -o temp/" + os.path.basename(item) + ".o -c " + arguments + item)
        libs += ("temp/" + os.path.basename(item) + ".o ")
    
    for item in input_cryptopp_sources:
        windows_bat.append("emcc -o temp/" + item + ".o -c " + arguments + libCryptoPP_src_path + item)
        libs += ("temp/" + item + ".o ")
    
    for item in input_pdfreader_sources:
        windows_bat.append("emcc -o temp/" + os.path.basename(item) + ".o -c " + arguments + libPdfReader_src_path + item)
        libs += ("temp/" + os.path.basename(item) + ".o ")
    
    for item in input_libs_sources:
        windows_bat.append("call emcc -o temp/" + os.path.basename(item) + ".o -c " + arguments + item)
        libs += ("temp/" + os.path.basename(item) + ".o ")
    
    arguments += "-s EXPORTED_FUNCTIONS=\"["
    for item in exported_functions:
        arguments += ("'" + item + "',")
    arguments = arguments[:-1]
    arguments += "]\" "

    for item in sources:
        arguments += (item + " ")
    
    windows_bat.append("emcc -o drawingfile.js " + arguments + libs)
base.replaceInFile("../../../../Common/3dParty/icu/icu/source/common/udata.cpp", "\n{\n    UDataMemory tData;", "\n{\n#ifdef BUILDING_WASM_MODULE\nreturn NULL;\n#endif\n    UDataMemory tData;")
base.run_as_bat(windows_bat)
base.replaceInFile("../../../../Common/3dParty/icu/icu/source/common/udata.cpp", "\n{\n#ifdef BUILDING_WASM_MODULE\nreturn NULL;\n#endif\n    UDataMemory tData;", "\n{\n    UDataMemory tData;")

# finalize
base.replaceInFile("./drawingfile.js", "function getBinaryPromise(){", "function getBinaryPromise2(){")
graphics_js_content = base.readFile("./drawingfile.js")
engine_base_js_content = base.readFile("./wasm/js/xps_base.js")
string_utf8_content    = base.readFile("./../../../../Common/js/string_utf8.js")
engine_js_content = engine_base_js_content.replace("//module", graphics_js_content)
engine_js_content = engine_js_content.replace("//string_utf8", string_utf8_content)
engine_base_js_content = base.readFile("./../../../../DjVuFile/wasm/djvu_base.js")
djvu_js_content = engine_base_js_content.replace("//module", graphics_js_content)
djvu_js_content = djvu_js_content.replace("//string_utf8", string_utf8_content)
engine_base_js_content = base.readFile("./wasm/js/pdf_base.js")
pdf_js_content = engine_base_js_content.replace("//module", graphics_js_content)
pdf_js_content = pdf_js_content.replace("//string_utf8", string_utf8_content)

# write new version
base.writeFile("./deploy/xps.js", engine_js_content)
base.writeFile("./deploy/djvu.js", djvu_js_content)
base.writeFile("./deploy/pdf.js", pdf_js_content)
base.copy_file("./drawingfile.wasm", "./deploy/drawingfile.wasm")

base.delete_file("drawingfile.js")
base.delete_file("drawingfile.wasm")
base.delete_dir("./temp")
base.delete_dir("./xml")
# base.delete_file("raster.o")
