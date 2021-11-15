#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
sys.path.append("./../../../build_tools/scripts")
import base
import os
import json

base.configure_common_apps()

# fetch emsdk
command_prefix = "" if ("windows" == base.host_platform()) else "./"
core_path = "./../../"
if not base.is_dir("emsdk"):
    base.cmd("git", ["clone", "https://github.com/emscripten-core/emsdk.git"])
    os.chdir("emsdk")
    base.cmd(command_prefix + "emsdk", ["install", "latest"])
    base.cmd(command_prefix + "emsdk", ["activate", "latest"])
    os.chdir("../")


def exec_wasm(data, compiler_flags, wasm):
    for include in data.include_path:
        compiler_flags.append("-I" + include)
    for define in data.define:
        compiler_flags.append("-D" + define)
  
    # arguments
    arguments = ""
    for item in compiler_flags:
        arguments += (item + " ")
  
    # command
    external_file = []
    prefix_call = ""
    if base.host_platform() == "windows":
        prefix_call = "call "
        external_file.append("call emsdk/emsdk_env.bat")
    else:
        external_file.append("#!/bin/bash")
        external_file.append("source ./emsdk/emsdk_env.sh")
  
    libs = ""
    for compile_files in data.compile_files_array:
        base.create_dir("./o/" + compile_files.name)
        for item in compile_files.files:
            file_name = os.path.splitext(os.path.basename(item))[0]
            if not base.is_file("./o/" + compile_files.name + "/" + file_name + ".o"):
                external_file.append(prefix_call + "emcc -o o/" + compile_files.name + "/" + file_name + ".o -c " + arguments + compile_files.folder + item)
            libs += ("o/" + compile_files.name + "/" + file_name + ".o ")
  
    arguments += "-s EXPORTED_FUNCTIONS=\"["
    for item in data.exported_functions:
        arguments += ("'" + item + "',")
    arguments = arguments[:-1]
    arguments += "]\" "
  
    for item in data.sources:
        arguments += (item + " ")
  
    external_file.append(prefix_call + "emcc -o " + data.name + ".js " + arguments + libs)
    print("run " + data.name)
    cur_dir = os.getcwd()
    os.chdir(data.work_folder)
    base.run_as_bat(external_file)
    os.chdir(cur_dir)
  
    # finalize
    print("finalize " + data.name)
    module_js_content = base.readFile(command_prefix + data.name + ".js")
    engine_base_js_content = base.readFile("data.base_js_content")
    string_utf8_content = base.readFile("./string_utf8.js")
    polyfill_js_content = base.readFile("./../3dParty/hunspell/wasm/js/polyfill.js")
    engine_js_content = engine_base_js_content.replace("//module", module_js_content)
    engine_js_content = engine_js_content.replace("//string_utf8", string_utf8_content)
    engine_js_content = engine_js_content.replace("//polyfill",    polyfill_js_content)
  
    # write new version
    base.writeFile(core_path + data.res_folder + "/" + data.name + ("" if wasm else "_ie") + ".js", engine_js_content)
    base.copy_file("./" + data.name + (".wasm" if wasm else ".js.mem"), core_path + data.res_folder + "/" + data.name + (".wasm" if wasm else ".js.mem"))
  
    # clear
    base.delete_file("./" + data.name + ".js")
    base.delete_file("./" + data.name + ".wasm")
    base.delete_file("./" + data.name + ".js.mem")


argv = sys.argv
argv.pop(0)
for param in argv:
    print(param)
    with open(param, "r") as json_file:
        json_data = json.load(json_file)
        json_file.close()
  
    print("before " + json_data.run_before)
    base.cmd_in_dir(core_path + json_data.work_folder, "python", [json_data.run_before])
  
    print(json_data.name)
    # remove previous version
    if base.is_dir("./deploy"):
        base.delete_dir("./deploy")
    base.create_dir("./deploy")
    if base.is_dir("./o"):
        base.delete_dir("./o")
    base.create_dir("./o")
    if base.is_dir(core_path + json_data.res_folder):
        base.delete_dir(core_path + json_data.res_folder)
    base.create_dir(core_path + json_data.res_folder)
  
    # wasm or asm
    if json_data.wasm:
        flags = json_data.compiler_flags
        flags.append("-s WASM=1")
        exec_wasm(json_data, flags, True)
    if json_data.asm:
        flags = json_data.compiler_flags
        flags.append("-s WASM=0")
        exec_wasm(json_data, flags, False)

    print("after " + json_data.run_after)
    base.cmd_in_dir(core_path + json_data.work_folder, "python", [json_data.run_after])
