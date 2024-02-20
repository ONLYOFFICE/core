#!/usr/bin/env python

import sys
sys.path.append('../../../build_tools/scripts')
import base
import os

params = sys.argv[1:]

file_path = params[0]
file_path_min = file_path + ".min.js"

compilation_level = "WHITESPACE_ONLY"
#compilation_level = "SIMPLE_OPTIMIZATIONS"
if (len(params) > 1):
  compilation_level = params[1]

base.cmd("java", ["-jar", "../../../sdkjs/build/node_modules/google-closure-compiler-java/compiler.jar", 
                  "--compilation_level", compilation_level,
                  "--language_out", "ECMASCRIPT5_STRICT",
                  "--js_output_file", file_path_min,
                  "--js", file_path])

min_content = base.readFile("../license/header.license") + base.readFile(file_path_min)

base.delete_file(file_path_min)
base.delete_file(file_path)

base.writeFile(file_path, min_content)
