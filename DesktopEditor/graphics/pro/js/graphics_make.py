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
compiler_flags = ["-o graphics.js",
                  "-O3",
                  "-fno-rtti",
                  "-s WASM=1",
                  "-s ALLOW_MEMORY_GROWTH=1",
                  "-s FILESYSTEM=0",
                  "-s ENVIRONMENT='web,worker'"]
                  #"-s LLD_REPORT_UNDEFINED"]

exported_functions = ["_malloc",
                      "_free",
                      "_Graphics_Malloc",
                      "_Graphics_Free",
                      "_Graphics_Create",
                      "_Graphics_Destroy",
                      "_Graphics_CreateFromBgraFrame",
                      "_Graphics_SetFontManager",
                      "_Graphics_drawHorLine"]

# sources
sources = []
sources.append("wasm/src/raster.cpp")

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
base.replaceInFile("./graphics.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];")
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
base.copy_file("./wasm/js/code.js", "./deploy/code.js")

base.delete_file("graphics.js")
base.delete_file("graphics.wasm")
