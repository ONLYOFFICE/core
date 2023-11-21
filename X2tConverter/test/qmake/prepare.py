#!/usr/bin/env python

import sys
sys.path.append('../../../../build_tools/scripts')
import config
import base
import build_js

platform_postfix = ""
platform = base.host_platform()

if platform == "windows":
  platform_postfix = "win_64"
elif platform == "mac":
  platform_postfix = "mac_64"
else:
  platform_postfix = "linux_64"

x2t_build_dir = "../../../build/bin/" + platform_postfix + "/debug"

config.parse()
build_js.build_sdk_builder("../../../../sdkjs/build")

if base.is_dir(x2t_build_dir + "/sdkjs"):
  base.delete_dir(x2t_build_dir + "/sdkjs")

base.copy_dir("../../../../sdkjs/deploy/sdkjs", x2t_build_dir + "/sdkjs")
base.create_dir(x2t_build_dir + "/sdkjs/vendor")
base.copy_dir("../../../../web-apps/vendor/jquery", x2t_build_dir + "/sdkjs/vendor/jquery")
base.copy_dir("../../../../web-apps/vendor/xregexp", x2t_build_dir + "/sdkjs/vendor/xregexp")

# for hyphen support in convertation to images/pdf
base.copy_dictionaries("../../../../dictionaries", x2t_build_dir + "/dictionaries", True, False)

# for pdf=>image/docx convertation
base.copy_file("../../../../sdkjs/pdf/src/engine/cmap.bin", x2t_build_dir + "/cmap.bin")

# DoctRenderer.config
base.generate_doctrenderer_config(x2t_build_dir + "/DoctRenderer.config", "./", "builder", "", "./dictionaries")
