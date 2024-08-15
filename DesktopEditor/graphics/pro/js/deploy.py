#!/usr/bin/env python

import sys
sys.path.append("../../../../../build_tools/scripts")
import base
import os

base.configure_common_apps()

base.cmd_in_dir("./../../../../../core/Common/js", "make.py", ["./../../DesktopEditor/graphics/pro/js/drawingfile.json"])

src_dir = "./deploy/"
dst_dir = "./../../../../../sdkjs/pdf/src/engine/"

base.delete_file(dst_dir + "drawingfile.js")
base.delete_file(dst_dir + "drawingfile.wasm")
base.delete_file(dst_dir + "drawingfile_ie.js")

base.copy_file(src_dir + "drawingfile.js",    dst_dir + "drawingfile.js")
base.copy_file(src_dir + "drawingfile.wasm",  dst_dir + "drawingfile.wasm")
base.copy_file(src_dir + "drawingfile_ie.js", dst_dir + "drawingfile_ie.js")
base.copy_file(src_dir + "drawingfile_native.js", dst_dir + "drawingfile_native.js")
