import sys
sys.path.append("../../../build_tools/scripts")
import base
import os

base.configure_common_apps()

# remove previous version
if base.is_dir("./deploy"):
    base.delete_dir("./deploy")
base.create_dir("./deploy")

# command
base.cmd_in_dir("./../../DesktopEditor/graphics/pro/js", "python", ["make.py"])

# finalize
if base.is_exist("./../../DesktopEditor/graphics/pro/js/deploy/drawingfile.wasm"):
    base.copy_file("./../../DesktopEditor/graphics/pro/js/deploy/drawingfile.js", "./deploy/drawingfile.js")
    base.copy_file("./../../DesktopEditor/graphics/pro/js/deploy/drawingfile.wasm", "./deploy/drawingfile.wasm")
    base.copy_file("./../../DesktopEditor/graphics/pro/js/deploy/drawingfile_ie.js", "./deploy/drawingfile_ie.js")
    base.copy_file("./../../DesktopEditor/graphics/pro/js/deploy/drawingfile.js.mem", "./deploy/drawingfile.js.mem")
else:
    print("make.py error")
    base.copy_dir("./all_files_test/xps_djvu", "./deploy")
    base.copy_dir("./all_files_test/pdf", "./deploy")

# write new version
base.copy_file("./all_files_test/index.html", "./deploy/index.html")
base.copy_file("./all_files_test/code.js", "./deploy/code.js")
base.copy_file("./all_files_test/file.js", "./deploy/file.js")
