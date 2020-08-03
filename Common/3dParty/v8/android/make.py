import sys
sys.path.append('./../../../../../build_tools/scripts')
import base
import os

if ("linux" == base.host_platform()):
  if not base.is_dir("./build"):
    base.bash("./build")

if ("mac" == base.host_platform()):
  if not base.is_file("./build.zip"):
    base.bash("./run_docker")
  if not base.is_dir("./build"):
    base.configure_common_apps("./../../../../../build_tools/scripts/base.py")
    base.extract("./build.zip", "./build")