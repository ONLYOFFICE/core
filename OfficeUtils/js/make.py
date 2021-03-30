import sys
sys.path.append("../../../build_tools/scripts")
import base
import os

base.configure_common_apps()

# remove previous version
if base.is_dir("./deploy"):
    base.delete_dir("./deploy")
base.create_dir("./deploy")

# fetch emsdk
command_prefix = "" if ("windows" == base.host_platform()) else "./"
if not base.is_dir("emsdk"):
    base.cmd("git", ["clone", "https://github.com/emscripten-core/emsdk.git"])
    os.chdir("emsdk")
    base.cmd(command_prefix + "emsdk", ["install", "latest"])
    base.cmd(command_prefix + "emsdk", ["activate", "latest"])
    os.chdir("../")


# compile
compiler_flags = ["-o zlib.js",
                  "-O3",
                  "-fno-rtti",
                  "-s WASM=1",
                  "-s ALLOW_MEMORY_GROWTH=1",
                  "-s FILESYSTEM=0",
                  "-s ENVIRONMENT='web,worker'"]
                  #"-s LLD_REPORT_UNDEFINED"]

exported_functions = ["_malloc",
                      "_free",
                      "_Zlib_Malloc",
                      "_Zlib_Free",
                      "_Zlib_Create",
                      "_Zlib_Load",
                      "_Zlib_Destroy",
                      "_Zlib_GetPaths",
                      "_Zlib_GetFile"]
