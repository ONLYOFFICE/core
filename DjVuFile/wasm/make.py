import sys
sys.path.append("../../../build_tools/scripts")
import base
import os

base.configure_common_apps()

# remove previous version
if base.is_dir("./deploy"):
    base.delete_dir("./deploy")
base.create_dir("./deploy")
base.create_dir("./deploy/xps_djvu")

# command
base.cmd_in_dir("./../../DesktopEditor/graphics/pro/js", "python", ["xps_djvu_make.py"])

# finalize
if base.is_exist("./../../DesktopEditor/graphics/pro/js/deploy/xps_djvu.wasm"):
    base.copy_file("./../../DesktopEditor/graphics/pro/js/deploy/xps.js", "./deploy/xps_djvu/xps.js")
    base.copy_file("./../../DesktopEditor/graphics/pro/js/deploy/djvu.js", "./deploy/xps_djvu/djvu.js")
    base.copy_file("./../../DesktopEditor/graphics/pro/js/deploy/xps_djvu.wasm", "./deploy/xps_djvu/xps_djvu.wasm")
else:
    print("xps_djvu_make.py error")
    base.copy_dir("./all_files_test/xps_djvu", "./deploy/xps_djvu")

# write new version
base.copy_file("./all_files_test/index.html", "./deploy/index.html")
base.copy_file("./all_files_test/code.js", "./deploy/code.js")
base.copy_file("./all_files_test/file.js", "./deploy/file.js")
base.copy_dir("./all_files_test/pdf", "./deploy/pdf")
