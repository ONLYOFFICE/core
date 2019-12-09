#!/usr/bin/env python

import os
import platform
import subprocess

def make(src_args):
  #--input="input-standard-files-dir"
  #--output="output-dir"
  #--standard // generate standarts
  #--use-system-fonts="0/1/false/true"
  #--font-dirs="C:\\Windows\\Fonts;/usr/share/fonts;"
  #--cores=4
  work_dir = os.path.dirname(os.path.realpath(__file__)) + "/.."
  os.chdir(work_dir)
  host = platform.system().lower()
  
  args = []
  args.append("standardtester.exe" if "windows" == host else "standardtester")
  for arg in src_args:
    arg_valid = arg.replace("'", "\"")
    arg_valid = arg_valid.replace("./", work_dir + "/")
    args.append(arg_valid)
	
  # env
  env_dir = os.environ
  if ("linux" == host):
    env_dir["LD_LIBRARY_PATH"] = "./:" + env_dir["LD_LIBRARY_PATH"]
  elif ("darwin" == host):
    env_dir["DYLD_LIBRARY_PATH"] = "./:" + env_dir["DYLD_LIBRARY_PATH"]

  ret = 0
  if ("windows" == platform):
    process = subprocess.Popen(args, stderr=subprocess.STDOUT, shell=True, env=env_dir)
    ret = process.wait()
  else:
    command = ""
    for arg in args:
      command += (" \"" + arg + "\"")
    process = subprocess.Popen(command, stderr=subprocess.STDOUT, shell=True, env=env_dir)
    ret = process.wait()
  return ret