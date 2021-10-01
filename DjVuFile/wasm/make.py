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
if base.is_exist("./../../DesktopEditor/graphics/pro/js/deploy/xps_djvu_pdf.wasm"):
    base.copy_file("./../../DesktopEditor/graphics/pro/js/deploy/xps.js", "./deploy/xps.js")
    base.copy_file("./../../DesktopEditor/graphics/pro/js/deploy/djvu.js", "./deploy/djvu.js")
    base.copy_file("./../../DesktopEditor/graphics/pro/js/deploy/pdf.js", "./deploy/pdf.js")
    base.copy_file("./../../DesktopEditor/graphics/pro/js/deploy/xps_djvu_pdf.wasm", "./deploy/xps_djvu_pdf.wasm")
else:
    print("make.py error")
    base.copy_dir("./all_files_test/xps_djvu", "./deploy")
    base.copy_dir("./all_files_test/pdf", "./deploy")

# write new version
base.copy_file("./all_files_test/index.html", "./deploy/index.html")
base.copy_file("./all_files_test/code.js", "./deploy/code.js")
base.copy_file("./all_files_test/file.js", "./deploy/file.js")
base.copy_dir("./all_files_test/pdf", "./deploy/pdf")
