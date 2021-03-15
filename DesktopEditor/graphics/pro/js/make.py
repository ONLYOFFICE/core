import sys
sys.path.append("../../../../../build_tools/scripts")
import base
import os

base.configure_common_apps()

# remove previous version
if base.is_dir("./deploy"):
    base.delete_dir("./deploy")
base.create_dir("./deploy")
base.create_dir("./deploy/raster")

# fetch emsdk
command_prefix = "" if ("windows" == base.host_platform()) else "./"
if not base.is_dir("emsdk"):
    base.cmd("git", ["clone", "https://github.com/emscripten-core/emsdk.git"])
    os.chdir("emsdk")
    base.cmd(command_prefix + "emsdk", ["install", "latest"])
    base.cmd(command_prefix + "emsdk", ["activate", "latest"])
    os.chdir("../")

# compile
compiler_flags = ["-o raster.js",
                  "-O3",
                  "-fno-exceptions",
                  "-fno-rtti",
                  "-s WASM=1",
                  "-s ALLOW_MEMORY_GROWTH=1",
                  "-s FILESYSTEM=0",
                  "-s ENVIRONMENT='web,worker'"]
                  #"-s LLD_REPORT_UNDEFINED"]

exported_functions = ["_malloc",
                      "_free",
                      "_CxImage_Malloc",
                      "_CxImage_Free",
                      "_CxImage_Create",
                      "_CxImage_Destroy",
                      "_CxImage_GetHeight",
                      "_CxImage_GetWidth",
                      "_CxImage_Decode",
                      "_CxImage_Encode",
                      "_CxImage_Encode2RGBA"]

libRaster_src_path = "./cimage/CxImage"
input_sources = ["/ximaenc.cpp",
                 "/ximaexif.cpp",
                 "/ximage.cpp",
                 "/ximainfo.cpp",
                 "/ximajpg.cpp",
                 "/ximalpha.cpp",
                 "/ximapal.cpp",
                 "/ximasel.cpp",
                 "/xmemfile.cpp"]

libJpeg_src_path = "./../../../cximage/jpeg"
input_jpeg_sources = ["/jerror.c", "jdmarker.c", "jdapimin.c", "jdmaster.c", "jdapistd.c",
                      "jcomapi.c", "jutils.c", "jdinput.c", "jdmainct.c", "jmemmgr.c",
                      "jquant1.c", "jquant2.c", "jdmerge.c", "jdcolor.c", "jdsample.c",
                      "jdpostct.c", "jddctmgr.c", "jdarith.c", "jdhuff.c", "jdcoefct.c",
                      "jmemnobs.c", "jidctint.c", "jidctfst.c", "jidctflt.c", "jaricom.c",
                      "jcapimin.c", "jcparam.c", "jcapistd.c", "jcinit.c", "jcmaster.c",
                      "jccolor.c", "jcmarker.c", "jcsample.c", "jcprepct.c", "jcdctmgr.c",
                      "jcarith.c", "jchuff.c", "jccoefct.c", "jcmainct.c", "jfdctint.c",
                      "jfdctfst.c", "jfdctflt.c"]

sources = []
for item in input_sources:
    sources.append(libRaster_src_path + '/' + item)
for item in input_jpeg_sources:
    sources.append(libJpeg_src_path + '/' + item)
sources.append("./wasm/src/base.cpp")

compiler_flags.append("-I" + libRaster_src_path)
compiler_flags.append("-I" + libJpeg_src_path)
compiler_flags.append("-DWIN32")

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
    windows_bat.append("call emcc " + arguments)
else:
    windows_bat.append("#!/bin/bash")
    windows_bat.append("source ./emsdk/emsdk_env.sh")
    windows_bat.append("emcc " + arguments)
base.run_as_bat(windows_bat)

# finalize
base.replaceInFile("./raster.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];")
base.replaceInFile("./raster.js", "function getBinaryPromise(){", "function getBinaryPromise2(){")

raster_js_content = base.readFile("./raster.js")
engine_base_js_content = base.readFile("./wasm/js/raster.js")
engine_js_content = engine_base_js_content.replace("//module", raster_js_content)

# write new version
base.writeFile("./deploy/raster/raster.js", engine_js_content)
base.copy_file("raster.wasm", "./deploy/raster/raster.wasm")
base.copy_file("./wasm/js/index.html", "./deploy/index.html")

base.delete_file("raster.js")
base.delete_file("raster.wasm")
