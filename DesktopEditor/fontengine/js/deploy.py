#!/usr/bin/env python

import sys
sys.path.append("../../../../build_tools/scripts")
import base
import os

base.configure_common_apps()

base.cmd_in_dir("./../../../../core/Common/js", "make.py", ["./../../DesktopEditor/fontengine/js/libfont.json"])

src_dir = "./deploy/"
dst_dir = "./../../../../sdkjs/common/libfont/engine/"

base.delete_file(dst_dir + "fonts.js")
base.delete_file(dst_dir + "fonts_ie.js")
base.delete_file(dst_dir + "fonts_native.js")
base.delete_file(dst_dir + "fonts.wasm")
base.delete_file(dst_dir + "../engine.js")

base.copy_file(src_dir + "fonts.js",                   dst_dir + "fonts.js")
base.copy_file(src_dir + "fonts_ie.js",                dst_dir + "fonts_ie.js")
base.copy_file(src_dir + "../engine/module_native.js", dst_dir + "fonts_native.js")
base.copy_file(src_dir + "fonts.wasm",                 dst_dir + "fonts.wasm")
base.copy_file(src_dir + "../module.js",               dst_dir + "../engine.js")
