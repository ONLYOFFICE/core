#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
sys.path.append("./../../../build_tools/scripts")
import base
import os
import json
import common

base.configure_common_apps()

# fetch emsdk
command_prefix = "" if ("windows" == base.host_platform()) else "./"
if not base.is_dir("emsdk"):
  base.cmd("git", ["clone", "https://github.com/emscripten-core/emsdk.git"])
  os.chdir("emsdk")
  base.cmd(command_prefix + "emsdk", ["install", "latest"])
  base.cmd(command_prefix + "emsdk", ["activate", "latest"])
  os.chdir("../")

def exec_wasm(data, work, compiler_flags, wasm):
  cur_dir = os.getcwd()
  os.chdir(work)

  for include in data["include_path"]:
    compiler_flags.append("-I" + include)
  for define in data["define"]:
    compiler_flags.append("-D" + define)
    
  if not wasm:
    compiler_flags.append("-DBUILDING_ASMJS_MODULE")
    compiler_flags.append("-D_ARM_ALIGN_")

  compiler_flags.append("-Wno-deprecated-register")

  # arguments
  arguments = ""
  for item in compiler_flags:
    arguments += (item + " ")

  # command
  run_file = []
  prefix_call = ""
  if base.host_platform() == "windows":
    prefix_call = "call "
    run_file.append("call " + cur_dir + "/emsdk/emsdk_env.bat")
  else:
    run_file.append("#!/bin/bash")
    run_file.append("source " + cur_dir + "/emsdk/emsdk_env.sh")

  libs = ""
  for compile_files in data["compile_files_array"]:
    base.create_dir("./o/" + compile_files["name"])

    temp_arguments = ""
    if "include_path" in compile_files and compile_files["include_path"]:
      for include in compile_files["include_path"]:
        temp_arguments += ("-I" + include + " ")
    if "define" in compile_files and compile_files["define"]:
      for define in compile_files["define"]:
        temp_arguments += ("-D" + define + " ")

    temp_libs = ""
    for item in compile_files["files"]:
      file_name = os.path.splitext(os.path.basename(item))[0]
      if not base.is_file("./o/" + compile_files["name"] + "/" + file_name + ".o"):
        run_file.append(prefix_call + "emcc -o o/" + compile_files["name"] + "/" + file_name + ".o -c " + arguments + temp_arguments + os.path.join(compile_files["folder"], item))
      temp_libs += ("o/" + compile_files["name"] + "/" + file_name + ".o ")

    if len(compile_files["files"]) > 10:
      if not base.is_file("./o/" + compile_files["name"] + "/" + compile_files["name"] + ".o"):
        run_file.append(prefix_call + "emcc -o o/" + compile_files["name"] + "/" + compile_files["name"] + ".o -r " + arguments + temp_arguments + temp_libs)
      libs += ("o/" + compile_files["name"] + "/" + compile_files["name"] + ".o ")
    else:
      libs += temp_libs

  arguments += "-s EXPORTED_FUNCTIONS=\"["
  for item in data["exported_functions"]:
    arguments += ("'" + item + "',")
  arguments = arguments[:-1]
  arguments += "]\" "

  if "sources" in data and data["sources"]:
    for item in data["sources"]:
      arguments += (item + " ")

  run_file.append(prefix_call + "emcc -o " + data["name"] + ".js " + arguments + libs)
  base.print_info("run " + ("wasm " if wasm else "asm ") + data["name"])
  base.run_as_bat(run_file)

  # finalize
  base.print_info("end " + ("wasm " if wasm else "asm ") + data["name"])
  module_js_content = base.readFile("./" + data["name"] + ".js")
  engine_base_js_content = base.readFile(data["base_js_content"])
  string_utf8_content = base.readFile(cur_dir + "/string_utf8.js")
  desktop_fetch_content = base.readFile(cur_dir + "/desktop_fetch.js")
  polyfill_js_content = base.readFile(cur_dir + "/polyfill.js")
  engine_js_content = engine_base_js_content.replace("//module", module_js_content)
  engine_js_content = engine_js_content.replace("//string_utf8", string_utf8_content)
  engine_js_content = engine_js_content.replace("//desktop_fetch", desktop_fetch_content)
  if not wasm:
    engine_js_content = engine_js_content.replace("//polyfill", polyfill_js_content)

  # write new version
  base.writeFile(data["res_folder"] + "/" + data["name"] + ("" if wasm else "_ie") + ".js", engine_js_content)
  base.copy_file("./" + data["name"] + (".wasm" if wasm else ".js.mem"), data["res_folder"] + "/" + data["name"] + (".wasm" if wasm else ".js.mem"))

  # clear
  base.delete_file("./" + data["name"] + ".js")
  base.delete_file("./" + data["name"] + (".wasm" if wasm else ".js.mem"))

  os.chdir(cur_dir)
  return

argv = sys.argv
argv.pop(0)
for param in argv:
  base.print_info(param)
  if not base.is_file(param):
    continue
  work_dir = os.path.dirname(param) + "/"
  json_data = json.loads(base.readFile(param))

  if json_data["run_before"]:
    base.print_info("before")
    if base.is_file(work_dir + json_data["run_before"]):
      base.cmd_in_dir(work_dir, "python", [json_data["run_before"]])
    else:
      base.cmd_in_dir(work_dir, "python", ["-c", json_data["run_before"]])

  # remove previous version
  common.clear_dir(work_dir + "/o")
  base.create_dir(work_dir + json_data["res_folder"])

  # wasm or asm
  if json_data["wasm"]:
    flags = json_data["compiler_flags"][:]
    flags.append("-s WASM=1")
    exec_wasm(json_data, work_dir, flags, True)
    base.delete_dir(work_dir + "/o")
  if json_data["asm"]:
    flags = json_data["compiler_flags"][:]
    flags.append("-s WASM=0")
    flags.append("-s MIN_IE_VERSION=11")
    if "embed_mem_file" in json_data and (json_data["embed_mem_file"]):
      flags.append("--memory-init-file 0")
    exec_wasm(json_data, work_dir, flags, False)
    base.delete_dir(work_dir + "/o")
  if json_data["run_after"]:
    base.print_info("after")
    if base.is_file(work_dir + json_data["run_after"]):
      base.cmd_in_dir(work_dir, "python", [json_data["run_after"]])
    else:
      base.cmd_in_dir(work_dir, "python", ["-c", json_data["run_after"]])
