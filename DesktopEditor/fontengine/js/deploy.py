#!/usr/bin/env python

import sys
sys.path.append("../../../../build_tools/scripts")
import base
import os

base.configure_common_apps()

module_base_engine = base.readFile("./module.js")

# js
module_js = base.readFile("./module_js.js")
module_js = module_js[module_js.find("*/") + 2:] #licence

# native
module_native = base.readFile("./module_native.js")
module_native = module_native[module_native.find("*/") + 2:] #licence

fonts_base_engine = module_base_engine.replace("//module", module_js)
base.writeFile("./fonts_base.js", fonts_base_engine)

base.cmd_in_dir("./../../../../core/Common/js", "make.py", ["./../../DesktopEditor/fontengine/js/libfont.json"])

module_native_engine = module_base_engine.replace("//module", module_native)
base.delete_file("./deploy/fonts_native.js")
base.writeFile("./deploy/fonts_native.js", module_native_engine)

src_dir = "./deploy/"
dst_dir = "./../../../../sdkjs/common/libfont/engine/"

base.delete_file("./fonts_base.js")

base.delete_file(dst_dir + "engine.js")
base.delete_file(dst_dir + "engine_ie.js")
base.delete_file(dst_dir + "engine_native.js")
base.delete_file(dst_dir + "fonts.wasm")

base.copy_file(src_dir + "fonts.js", dst_dir + "engine.js")
base.copy_file(src_dir + "fonts_ie.js", dst_dir + "engine_ie.js")
base.copy_file(src_dir + "fonts_native.js", dst_dir + "engine_native.js")
base.copy_file(src_dir + "fonts.wasm", dst_dir + "fonts.wasm")
