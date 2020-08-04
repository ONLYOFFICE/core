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
    base.extract("./build.zip", "./")
  if not base.is_dir("v8"):
    if not base.is_dir("depot_tools"):
      base.cmd("git", ["clone", "https://chromium.googlesource.com/chromium/tools/depot_tools.git"])
    old_cur = os.getcwd()
    os.environ["PATH"] = old_cur + "/depot_tools" + os.pathsep + os.environ["PATH"]
    base.cmd("./depot_tools/fetch", ["v8"], True)
    os.chdir(old_cur + "/v8")
    base.cmd("git", ["checkout", "-b", "6.0", "branch-heads/6.0"], True)
    os.chdir(old_cur)
    base.cmd("gclient", ["sync"])
