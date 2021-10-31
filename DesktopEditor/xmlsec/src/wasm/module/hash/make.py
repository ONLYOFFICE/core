import sys
sys.path.append('../../../../../../../build_tools/scripts')
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
compiler_flags = ["-o hash.js",
                  "-O3",
                  "-fno-exceptions",
                  "-fno-rtti",
                  "-s WASM=0",
                  "-s ALLOW_MEMORY_GROWTH=1",
                  "-s FILESYSTEM=0",
                  "-s ENVIRONMENT='web'",
                  "-s TOTAL_STACK=1MB",
                  "-s TOTAL_MEMORY=2MB",
                  "--memory-init-file 0"]

exported_functions = ["_malloc",
                      "_free",
                      "_hash",
					  "_hash2"]

sources = []
sources.append("./../openssl/crypto/sha/keccak1600.c")
sources.append("./../openssl/crypto/sha/sha1_one.c")
sources.append("./../openssl/crypto/sha/sha1dgst.c")
sources.append("./../openssl/crypto/sha/sha256.c")
sources.append("./../openssl/crypto/sha/sha512.c")

sources.append("./../openssl/crypto/md2/md2_dgst.c")
sources.append("./../openssl/crypto/md2/md2_one.c")

sources.append("./../openssl/crypto/md4/md4_dgst.c")
sources.append("./../openssl/crypto/md4/md4_one.c")

sources.append("./../openssl/crypto/md5/md5_dgst.c")
sources.append("./../openssl/crypto/md5/md5_one.c")

sources.append("./../openssl/crypto/ripemd/rmd_dgst.c")
sources.append("./../openssl/crypto/ripemd/rmd_one.c")

sources.append("./../openssl/crypto/whrlpool/wp_block.c")
sources.append("./../openssl/crypto/whrlpool/wp_dgst.c")

sources.append("./../openssl/crypto/mem_clr.c")

sources.append("./main.cpp")

compiler_flags.append("-I./")
compiler_flags.append("-I./../openssl/include")
compiler_flags.append("-DWIN32 -DNDEBUG")

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
if (base.host_platform() == "windows"):
  windows_bat.append("call emsdk/emsdk_env.bat")
  windows_bat.append("call emcc " + arguments)  
  #windows_bat.append("emcc --help")
else:
  windows_bat.append("#!/bin/bash")
  windows_bat.append("source ./emsdk/emsdk_env.sh")
  windows_bat.append("emcc " + arguments)  
base.run_as_bat(windows_bat)

module_js_content = base.readFile("./hash.js")
engine_base_js_content = base.readFile("./hash_base.js")
string_utf8_content    = base.readFile("./../../../../../../Common/js/string_utf8.js")
polyfill_js_content    = base.readFile("./../../../../../../Common/3dParty/hunspell/wasm/js/polyfill.js")
engine_js_content = engine_base_js_content.replace("//module", module_js_content)
engine_js_content = engine_js_content.replace("//string_utf8", string_utf8_content)
engine_js_content = engine_js_content.replace("//polyfill",    polyfill_js_content)

base.writeFile("./deploy/hash.js", engine_js_content)
