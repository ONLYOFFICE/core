import sys
sys.path.append('./../../../../../build_tools/scripts')
import base
import os

def build_func():
  if ("linux" == base.host_platform()):
    if not base.is_dir("./build"):
      base.cmd("python", ["./build.py"])

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
      base.cmd("./depot_tools/gclient", ["sync", "-r", "remotes/branch-heads/8.9"], True)
      base.cmd("gclient", ["sync", "--force"], True)
  return

# build function. for speed we just download the build archive
# archive built with this func
if False:
  build_func()
else:
  if not base.is_file("./build.7z"):
    base.download("http://d2ettrnqo7v976.cloudfront.net/v8/android/build.7z", "./build.7z")
    base.extract("./build.7z", "./")
    base.copy_dir("./build/v8", "./v8")
    base.delete_dir("./build/v8")
