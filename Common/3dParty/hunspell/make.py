import sys
sys.path.append('../../../../build_tools/scripts')
import base
import os

def get_hunspell(last_stable_commit):
    repo_path = "https://github.com/hunspell/hunspell.git"
    base.cmd("git", ["clone", repo_path])
    os.chdir("hunspell")
    base.cmd("git", ["checkout", last_stable_commit])
    base.replaceInFile("./src/hunspell/filemgr.hxx", "FileMgr& operator=(const FileMgr&);", "FileMgr& operator=(const FileMgr&);\n" 
         +"#ifdef HUNSPELL_WASM_MODULE\nstring_buffer_stream memin;\n#endif") #custon filemgr support watch filemgr_wrapper_new.cxx
    base.replaceInFile("./src/hunspell/filemgr.hxx", "#include <fstream>", "#include <fstream>\n#ifdef HUNSPELL_WASM_MODULE\n#include \"string_buffer_stream.h\"\n#endif\n")
    base.replaceInFile("./src/hunspell/csutil.cxx", "void free_utf_tbl() {", "void free_utf_tbl() { \n return;\n")
	# bug fix, we need to keep this utf table 
    # free_utf_tbl doesnt delete anything so we can destroy huspell object
    os.chdir("../")


base.configure_common_apps()

# remove previous version
if base.is_dir("./deploy"):
  base.delete_dir("./deploy")
base.create_dir("./deploy")
base.create_dir("./deploy/spell")

# fetch emsdk
command_prefix = "" if ("windows" == base.host_platform()) else "./"
if not base.is_dir("emsdk"):
  base.cmd("git", ["clone", "https://github.com/emscripten-core/emsdk.git"])
  os.chdir("emsdk")
  base.cmd(command_prefix + "emsdk", ["install", "latest"])
  base.cmd(command_prefix + "emsdk", ["activate", "latest"])
  os.chdir("../")


#fetch libhunspell
HEAD = open("HEAD", "r")
last_stable_commit = HEAD.read().split('\n')[0] # workaround to delete \n in the end of the line
HEAD.close()

if not base.is_dir("hunspell"):
  get_hunspell(last_stable_commit)

# version check

git_head = open("hunspell/.git/HEAD", "r")
current_commit = git_head.read().split('\n')[0]
git_head.close()

if current_commit != last_stable_commit:
  base.delete_dir("hunspell")
  get_hunspell(last_stable_commit)

# compile
compiler_flags = ["-o spell.js",
                  "-O3",
                  "-fno-exceptions",
                  "-fno-rtti",
                  "-s WASM=1",
                  "-s ALLOW_MEMORY_GROWTH=1",
                  "-s FILESYSTEM=0",
                  "-s ENVIRONMENT='web,worker'"]

exported_functions = ["_malloc",
                      "_free",
                      "_Spellchecker_Malloc",
                      "_Spellchecker_Free",
                      "_Spellchecker_Create",
                      "_Spellchecker_Destroy",
                      "_Spellchecker_AddDictionary",
                      "_Spellchecker_RemoveDicrionary",
                      "_Spellchecker_Load",
                      "_Spellchecker_Spell",
                      "_Spellchecker_Suggest",
                      "_Spellchecker_RemoveEngine",
                      "_Spellchecker_TotalAllocatedMemory"]

version_hunspell = 17
libhunspell_src_path = "./hunspell/src/hunspell"

input_sources = ["affentry.cxx",
                 "affixmgr.cxx",
                 "csutil.cxx",
                 "dictmgr.cxx",
                 "hashmgr.cxx",
                 "hunspell.cxx",
                 "hunzip.cxx",
                 "phonet.cxx",
                 "replist.cxx",
                 "suggestmgr.cxx"]

sources = []
for item in input_sources:
  if base.is_file(libhunspell_src_path + "/" + item):
    sources.append(libhunspell_src_path + "/" + item)

if (13 == version_hunspell):
  sources.append("./wasm/src/filemgr_wrapper.cxx")
else:
  sources.append("./wasm/src/filemgr_wrapper_new.cxx")

sources.append("./wasm/src/base.cpp")

compiler_flags.append("-I" + libhunspell_src_path)
compiler_flags.append("-I./wasm/src")
compiler_flags.append("-DWIN32 -DNDEBUG -DHUNSPELL_STATIC -DBUILDING_LIBHUNSPELL -DHUNSPELL_WASM_MODULE")

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
else:
  windows_bat.append("#!/bin/bash")
  windows_bat.append("source ./emsdk/emsdk_env.sh")
  windows_bat.append("emcc " + arguments)  
base.run_as_bat(windows_bat)

# finalize
base.replaceInFile("./spell.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];")
base.replaceInFile("./spell.js", "function getBinaryPromise(){", "function getBinaryPromise2(){")

spell_js_content = base.readFile("./spell.js")
engine_base_js_content = base.readFile("./wasm/js/spell.js")
engine_js_content = base.readFile("./../../license/header.license") + "\n" + engine_base_js_content.replace("//module", spell_js_content)

# write new version
base.writeFile("./deploy/spell/spell.js", engine_js_content)
base.copy_file("spell.wasm", "./deploy/spell/spell.wasm")

# ie asm version
arguments = arguments.replace("WASM=1", "WASM=0")

# command
windows_bat = []
if (base.host_platform() == "windows"):
  windows_bat.append("call emsdk/emsdk_env.bat")
  windows_bat.append("call emcc " + arguments)  
else:
  windows_bat.append("#!/bin/bash")
  windows_bat.append("source ./emsdk/emsdk_env.sh")
  windows_bat.append("emcc " + arguments)  
base.run_as_bat(windows_bat)

# finalize
base.replaceInFile("./spell.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];")
base.replaceInFile("./spell.js", "function getBinaryPromise(){", "function getBinaryPromise2(){")

spell_js_content = base.readFile("./spell.js")
engine_base_js_content = base.readFile("./wasm/js/spell.js")
engine_base_js_polyfill = base.readFile("./wasm/js/polyfill.js")
engine_js_content = base.readFile("./../../license/header.license") + "\n" + engine_base_js_polyfill + "\n\n" + engine_base_js_content.replace("//module", spell_js_content)

# write new version
base.writeFile("./deploy/spell/spell_ie.js", engine_js_content)
base.copy_file("spell.js.mem", "./deploy/spell/spell.js.mem")

base.copy_file("./wasm/js/code.js", "./deploy/spell.js")
base.copy_file("./wasm/js/index.html", "./deploy/index.html")

base.delete_file("spell.js")
base.delete_file("spell.js.mem")
