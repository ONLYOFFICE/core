#!/usr/bin/env python

import sys
import platform
import os
import subprocess
import glob
import shutil
import codecs

# BASE --------------------------------------------
def host_platform():
  ret = platform.system().lower()
  if (ret == "darwin"):
    return "mac"
  return ret

def get_path(path):
  if "windows" == host_platform():
    return path.replace("/", "\\")
  return path

def is_file(path):
  return os.path.isfile(get_path(path))

def is_dir(path):
  return os.path.isdir(get_path(path))

def copy_file(src, dst):
  if is_file(dst):
    delete_file(dst)
  if not is_file(src):
    print("copy warning [file not exist]: " + src)
    return
  return shutil.copy2(get_path(src), get_path(dst))

def delete_file(path):
  if not is_file(path):
    print("delete warning [file not exist]: " + path)
    return
  return os.remove(get_path(path))

def create_dir(path):
  path2 = get_path(path)
  if not os.path.exists(path2):
    os.makedirs(path2)
  return

def copy_dir(src, dst):
  if is_dir(dst):
    delete_dir(dst)
  try:
    shutil.copytree(get_path(src), get_path(dst))    
  except OSError as e:
    print('Directory not copied. Error: %s' % e)
  return

def delete_dir(path):
  if not is_dir(path):
    print("delete warning [folder not exist]: " + path)
    return
  shutil.rmtree(get_path(path), ignore_errors=True)
  return

def cmd(prog, args=[], is_no_errors=False):  
  ret = 0
  command = prog
  for arg in args:
    command += (" \"" + arg + "\"")
  ret = subprocess.call(command, stderr=subprocess.STDOUT, shell=True)
  if ret != 0 and True != is_no_errors:
    sys.exit("Error (" + prog + "): " + str(ret))
  return ret

def cmd2(prog, args=[], is_no_errors=False):  
  ret = 0
  command = get_path(prog)
  for arg in args:
    command += (" " + arg)
  print(command)
  ret = subprocess.call(command, stderr=subprocess.STDOUT, shell=True)
  if ret != 0 and True != is_no_errors:
    sys.exit("Error (" + prog + "): " + str(ret))
  return ret

def get_script_dir():
  test_file = __file__
  scriptPath = os.path.realpath(test_file)
  scriptDir = os.path.dirname(scriptPath)
  return scriptDir

def get_env(name):
  return os.getenv(name, "")

def set_env(name, value):
  os.environ[name] = value
  return

def replaceInFile(path, text, textReplace):
  if not is_file(path):
    print("[replaceInFile] file not exist: " + path)
    return
  filedata = ""
  with open(get_path(path), "r") as file:
    filedata = file.read()
  filedata = filedata.replace(text, textReplace)
  delete_file(path)
  with open(get_path(path), "w") as file:
    file.write(filedata)
  return

# -------------------------------------------------

def get_android_args(platform, sdk_ver=21):
  gn_args = ["is_component_build=false",
             "is_clang=true",
             "is_debug=false",
             "use_sysroot=true",
             "target_os=\\\"android\\\"",
             "target_cpu=\\\"" + platform + "\\\"",
             "v8_target_cpu=\\\"" + platform + "\\\"",
             "v8_enable_i18n_support=true",
             "v8_static_library=true",
             "v8_monolithic=true",
             "use_custom_libcxx=false",
             "android_ndk_version=\\\"26.2.11394342\\\"",
             "android_sdk_version=\\\"" + str(sdk_ver) + "\\\"",
             "clang_use_chrome_plugins=false",
             "v8_use_external_startup_data=false",
             "treat_warnings_as_errors=false"]
  return gn_args

def deploy_libs():
  if is_dir("../build"):
    delete_dir("../build")
  create_dir("../build")

  platforms = ["arm64", "arm", "x86_64", "x86"]
  dirs = ["arm64-v8a", "armeabi-v7a", "x86_64", "x86"]

  for index, item in enumerate(platforms):
    input_dir = "./out.gn/android/" + platforms[index]
    output_dir = "../build/" + dirs[index]
    create_dir(output_dir)
    copy_file(input_dir + "/icudtl.dat", output_dir + "/icudtl.dat")
    copy_file(input_dir + "/icudtl_extra.dat", output_dir + "/icudtl_extra.dat")
    copy_file(input_dir + "/obj/libv8_monolith.a", output_dir + "/libv8_monolith.a")

  create_dir("../build/v8")
  copy_dir("./include", "../build/v8/include")
  copy_dir("./src", "../build/v8/src")
  return

def make():
  old_env = dict(os.environ)
  old_cur = os.getcwd()

  base_dir = get_script_dir()

  os.chdir(base_dir)
  if not is_dir("depot_tools"):
    cmd("git", ["clone", "https://chromium.googlesource.com/chromium/tools/depot_tools.git"])

  os.environ["PATH"] = base_dir + "/depot_tools" + os.pathsep + os.environ["PATH"]

  if not is_dir("v8"):
    cmd("./depot_tools/fetch", ["v8"], True)
    copy_dir("./v8/third_party", "./v8/third_party_new")
    v8_branch_version = "remotes/branch-heads/8.9"
    cmd("./depot_tools/gclient", ["sync", "-r", v8_branch_version], True)
    replaceInFile("./.gclient", "]", "];target_os=['android'];")
    cmd("gclient", ["sync", "--force"], True)
    copy_dir("./v8/third_party_new/ninja", "./v8/third_party/ninja")

  if not is_file("v8/third_party/jinja2/tests.py.bak"):
    copy_file("v8/third_party/jinja2/tests.py", "v8/third_party/jinja2/tests.py.bak")
    replaceInFile("v8/third_party/jinja2/tests.py", "from collections import Mapping", "try:\n    from collections.abc import Mapping\nexcept ImportError:\n    from collections import Mapping")

  os.chdir("v8")
  
  cmd2("gn", ["gen", "out.gn/android/arm64", "--args=\"" + " ".join(get_android_args("arm64")) + "\""])
  cmd("ninja", ["-C", "out.gn/android/arm64"])

  cmd2("gn", ["gen", "out.gn/android/arm", "--args=\"" + " ".join(get_android_args("arm")) + "\""])
  cmd("ninja", ["-C", "out.gn/android/arm"])

  cmd2("gn", ["gen", "out.gn/android/x86_64", "--args=\"" + " ".join(get_android_args("x64", 16)) + "\""])
  cmd("ninja", ["-C", "out.gn/android/x86_64"])

  cmd2("gn", ["gen", "out.gn/android/x86", "--args=\"" + " ".join(get_android_args("x86", 16)) + "\""])
  cmd("ninja", ["-C", "out.gn/android/x86"])

  deploy_libs()

  os.chdir(old_cur)
  os.environ.clear()
  os.environ.update(old_env)

make()
