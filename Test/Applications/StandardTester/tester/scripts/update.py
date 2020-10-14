#!/usr/bin/env python

import os
import platform
import shutil
import codecs

work_dir = os.path.dirname(os.path.realpath(__file__)) + "/.."
git_dir = work_dir + "/../../../../.."
exe_addon = ".exe" if "windows" == platform.system().lower() else ""

# 1) Remove all old version
print("-----------------------------------------------")
print("- remove old version --------------------------")
for r, d, f in os.walk(work_dir):
  for file in f:
    os.remove(os.path.join(work_dir, file))
  break
	
if os.path.isdir(work_dir + "/sdkjs"):
  shutil.rmtree(work_dir + "/sdkjs")
if os.path.isdir(work_dir + "/HtmlFileInternal"):
  shutil.rmtree(work_dir + "/HtmlFileInternal")
print("-----------------------------------------------")

# 2) create new portable version
print("- create new version --------------------------")

base_core = git_dir + "/server/FileConverter/bin/core"
for r, d, f in os.walk(base_core):
  for file in f:
    shutil.copy2(os.path.join(base_core, file), work_dir + "/" + os.path.basename(file))
  for dir in d:
    shutil.copytree(os.path.join(base_core, dir), work_dir + "/" + os.path.basename(dir))
  break
  
if os.path.isfile(work_dir + "/allfontsgen" + exe_addon):
  os.remove(work_dir + "/allfontsgen" + exe_addon)
if os.path.isfile(work_dir + "/allthemesgen" + exe_addon):
  os.remove(work_dir + "/allthemesgen" + exe_addon)

config = "<Settings>\n"
config += "<file>./sdkjs/common/Native/native.js</file>\n"
config += "<file>./sdkjs/common/Native/jquery_native.js</file>\n"
config += "<file>./sdkjs/vendor/xregexp/xregexp-all-min.js</file>\n"
config += "<file>./fonts/AllFonts.js</file>\n"
config += "<htmlfile>./sdkjs/vendor/jquery/jquery.min.js</htmlfile>\n"
config += "<DoctSdk>\n"
config += "<file>./sdkjs/word/sdk-all-min.js</file>\n"
config += "<file>./sdkjs/common/libfont/js/fonts.js</file>\n"
config += "<file>./sdkjs/word/sdk-all.js</file>\n"
config += "</DoctSdk>\n"
config += "<PpttSdk>\n"
config += "<file>./sdkjs/slide/sdk-all-min.js</file>\n"
config += "<file>./sdkjs/common/libfont/js/fonts.js</file>\n"
config += "<file>./sdkjs/slide/sdk-all.js</file>\n"
config += "</PpttSdk>\n"
config += "<XlstSdk>\n"
config += "<file>./sdkjs/cell/sdk-all-min.js</file>\n"
config += "<file>./sdkjs/common/libfont/js/fonts.js</file>\n"
config += "<file>./sdkjs/cell/sdk-all.js</file>\n"
config += "</XlstSdk>\n"
config += "</Settings>"

file = codecs.open(work_dir + "/DoctRenderer.config", "w", "utf-8")
file.write(config)
file.close()
print("-----------------------------------------------")