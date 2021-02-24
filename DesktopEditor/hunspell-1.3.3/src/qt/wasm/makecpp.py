#!/usr/bin/env python

import sys
sys.path.append('../../../../../../build_tools/scripts')
import base
import os

base.configure_common_apps()



# compile
compiler_flags = ["-o test"]

version_hunspell = 17
libhunspell_src_path = "./../../hunspell"

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
sources.append("./src/main.cpp")

for item in input_sources:
  if base.is_file(libhunspell_src_path + "/" + item):
    sources.append(libhunspell_src_path + "/" + item)



sources.append("./src/base.cpp")
sources.append("./src/filemgr_wrapper_new.cxx")


compiler_flags.append("-I" + libhunspell_src_path)
compiler_flags.append("-I" + libhunspell_src_path + "/../win_api")
compiler_flags.append("-DWIN32 -DNDEBUG -DHUNSPELL_STATIC -DBUILDING_LIBHUNSPELL -DHUNSPELL_WASM_MODULE")

# arguments
arguments = ""
for item in compiler_flags:
  arguments += (item + " ")

for item in sources:
  arguments += (item + " ")

# command
windows_bat = []

windows_bat.append("#!/bin/bash")
windows_bat.append("g++ " + arguments)  

base.run_as_bat(windows_bat)
