import sys
sys.path.append("../../../build_tools/scripts")
import base
import os

base.configure_common_apps()

# remove previous version
if base.is_dir("./deploy"):
    base.delete_dir("./deploy")
base.create_dir("./deploy")
base.create_dir("./deploy/djvu")

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
                  "-s ENVIRONMENT='web'",
                  "-s ERROR_ON_UNDEFINED_SYMBOLS=0"]

exported_functions = ["_malloc",
                      "_free",
                      "_DJVU_Load",
                      "_DJVU_Close",
                      "_DJVU_GetInfo",
                      "_DJVU_GetPixmap",
                      "_DJVU_Delete"]

libDjVu_src_path = "../libdjvu/"
input_djvu_sources = ["Arrays.cpp", "BSByteStream.cpp", "BSEncodeByteStream.cpp", "ByteStream.cpp", "DataPool.cpp", "debug.cpp", "DjVmDir.cpp", "DjVmDir0.cpp", "DjVmDoc.cpp", "DjVmNav.cpp", "DjVuAnno.cpp", "DjVuDocEditor.cpp", "DjVuDocument.cpp", "DjVuDumpHelper.cpp", "DjVuErrorList.cpp", "DjVuFile.cpp", "DjVuFileCache.cpp", "DjVuGlobal.cpp", "DjVuGlobalMemory.cpp", "DjVuImage.cpp", "DjVuInfo.cpp", "DjVuMessageLite.cpp", "DjVuNavDir.cpp", "DjVuPalette.cpp", "DjVuPort.cpp", "DjVuText.cpp", "DjVuToPS.cpp", "GBitmap.cpp", "GContainer.cpp", "GException.cpp", "GIFFManager.cpp", "GMapAreas.cpp", "GPixmap.cpp", "GRect.cpp", "GScaler.cpp", "GSmartPointer.cpp", "GString.cpp", "GThreads.cpp", "GUnicode.cpp", "IFFByteStream.cpp", "IW44EncodeCodec.cpp", "IW44Image.cpp", "JB2EncodeCodec.cpp", "JB2Image.cpp", "JPEGDecoder.cpp", "MMRDecoder.cpp", "MMX.cpp", "UnicodeByteStream.cpp", "XMLParser.cpp", "XMLTags.cpp", "ZPCodec.cpp"]

libWasmDjVu_src_path = "libdjvu/"
input_wasmdjvu_sources = ["atomic.cpp", "DjVuMessage.cpp", "GOS.cpp", "GURL.cpp"]

# sources
sources = []
for item in input_djvu_sources:
    sources.append(libDjVu_src_path + item)
for item in input_wasmdjvu_sources:
    sources.append(libWasmDjVu_src_path + item)
sources.append("djvu_common.cpp")

compiler_flags.append("-DWIN32 -DNDEBUG -D_LIB -D_CRT_SECURE_NO_WARNINGS -DWASM_MODE -Derrno=0 \"-DUINT=unsigned int\" -DTHREADMODEL=0 -DDEBUGLVL=0")

# arguments
arguments = ""
for item in compiler_flags:
    arguments += (item + " ")
arguments += "-s EXPORTED_FUNCTIONS=\"["
for item in exported_functions:
    arguments += ("'" + item + "',")
arguments = arguments[:-1]
arguments += "]\" "

for item in sources:
    arguments += (item + " ")

# command
windows_bat = []
if base.host_platform() == "windows":
    windows_bat.append("call emsdk/emsdk_env.bat")
    windows_bat.append("call emcc -o djvu.js " + arguments)
else:
    windows_bat.append("#!/bin/bash")
    windows_bat.append("source ./emsdk/emsdk_env.sh")
    windows_bat.append("emcc -o djvu.js " + arguments)
base.run_as_bat(windows_bat)

# finalize
base.replaceInFile("./djvu.js", "function getBinaryPromise(){", "function getBinaryPromise2(){")

djvu_js_content = base.readFile("./djvu.js")
engine_base_js_content = base.readFile("./djvu_base.js")
engine_js_content = engine_base_js_content.replace("//module", djvu_js_content)

# write new version
base.writeFile("./deploy/djvu/djvu.js", engine_js_content)
base.copy_file("./djvu.wasm", "./deploy/djvu/djvu.wasm")
base.copy_file("./all_files_test/index.html", "./deploy/index.html")
base.copy_file("./all_files_test/code.js", "./deploy/code.js")
base.copy_file("./all_files_test/file.js", "./deploy/file.js")
base.copy_dir("./all_files_test/pdf", "./deploy/pdf")
if base.is_dir("./../../DesktopEditor/graphics/pro/js/deploy"):
    base.create_dir("./deploy/xps")
    base.copy_file("./../../DesktopEditor/graphics/pro/js/deploy/xps.js", "./deploy/xps/xps.js")
    base.copy_file("./../../DesktopEditor/graphics/pro/js/deploy/xps.wasm", "./deploy/xps/xps.wasm")
else:
    print("xps_make.py not used. core/DesktopEditor/graphics/pro/js/xps_make.py")
    base.copy_dir("./all_files_test/xps", "./deploy/xps")

base.delete_file("djvu.js")
base.delete_file("djvu.wasm")
