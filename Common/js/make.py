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
if not base.is_dir("emsdk"):
    base.cmd("git", ["clone", "https://github.com/emscripten-core/emsdk.git"])
    os.chdir("emsdk")
    base.cmd(command_prefix + "emsdk", ["install", "latest"])
    base.cmd(command_prefix + "emsdk", ["activate", "latest"])
    os.chdir("../")


def exec_wasm(data, work, compiler_flags, wasm):
    for include in data["include_path"]:
        compiler_flags.append("-I" + os.path.normpath(work + include).replace("\\", '/'))
    for define in data["define"]:
        compiler_flags.append("-D" + define)
  
    # arguments
    arguments = ""
    for item in compiler_flags:
        arguments += (item + " ")
  
    # command
    run_file = []
    prefix_call = ""
    if base.host_platform() == "windows":
        prefix_call = "call "
        run_file.append("call emsdk/emsdk_env.bat")
    else:
        run_file.append("#!/bin/bash")
        run_file.append("source ./emsdk/emsdk_env.sh")
  
    libs = ""
    for compile_files in data["compile_files_array"]:
        base.create_dir("./o/" + compile_files["name"])
        temp_libs = ""
        for item in compile_files["files"]:
            file_name = os.path.splitext(os.path.basename(item))[0]
            if not base.is_file("./o/" + compile_files["name"] + "/" + file_name + ".o"):
                run_file.append(prefix_call + "emcc -o o/" + compile_files["name"] + "/" + file_name + ".o -c " + arguments + os.path.normpath(work + os.path.join(compile_files["folder"] + item)).replace("\\", '/'))
            temp_libs += ("o/" + compile_files["name"] + "/" + file_name + ".o ")
        if len(compile_files["files"]) > 10:
            if not base.is_file("./o/" + compile_files["name"] + "/" + compile_files["name"] + ".o"):
                run_file.append(prefix_call + "emcc -o o/" + compile_files["name"] + "/" + compile_files["name"] + ".o -r " + arguments + temp_libs)
            libs += ("o/" + compile_files["name"] + "/" + compile_files["name"] + ".o ")
        else:
            libs += temp_libs
  
    arguments += "-s EXPORTED_FUNCTIONS=\"["
    for item in data["exported_functions"]:
        arguments += ("'" + item + "',")
    arguments = arguments[:-1]
    arguments += "]\" "
  
    for item in data["sources"]:
        arguments += (os.path.normpath(work + item).replace("\\", '/') + " ")
  
    run_file.append(prefix_call + "emcc -o " + data["name"] + ".js " + arguments + libs)
    print("run " + data["name"])
    base.run_as_bat(run_file)
  
    # finalize
    print("finalize " + data["name"])
    module_js_content = base.readFile("./" + data["name"] + ".js")
    engine_base_js_content = base.readFile(work + data["base_js_content"])
    string_utf8_content = base.readFile("./string_utf8.js")
    polyfill_js_content = base.readFile("./../3dParty/hunspell/wasm/js/polyfill.js")
    engine_js_content = engine_base_js_content.replace("//module", module_js_content)
    engine_js_content = engine_js_content.replace("//string_utf8", string_utf8_content)
    engine_js_content = engine_js_content.replace("//polyfill",    polyfill_js_content)
  
    # write new version
    base.writeFile(work + data["res_folder"] + "/" + data["name"] + ("" if wasm else "_ie") + ".js", engine_js_content)
    base.copy_file("./" + data["name"] + (".wasm" if wasm else ".js.mem"), work + data["res_folder"] + "/" + data["name"] + (".wasm" if wasm else ".js.mem"))
  
    # clear
    base.delete_file("./" + data["name"] + ".js")
    base.delete_file("./" + data["name"] + (".wasm" if wasm else ".js.mem"))


argv = sys.argv
argv.pop(0)
for param in argv:
    print(param)
    if not base.is_file(param):
        continue
    work_dir = os.path.dirname(param) + "/"
    json_data = json.loads(base.readFile(param))

    if json_data["run_before"] and base.is_file(work_dir + json_data["run_before"]):
        print("before " + json_data["run_before"])
        base.cmd_in_dir(work_dir, "python", [json_data["run_before"]])

    # remove previous version
    if base.is_dir("./o"):
        base.delete_dir("./o")
    base.create_dir("./o")
    base.create_dir(work_dir + json_data["res_folder"])
  
    # wasm or asm
    if json_data["wasm"]:
        print("wasm " + json_data["name"])
        flags = json_data["compiler_flags"].copy()
        flags.append("-s WASM=1")
        exec_wasm(json_data, work_dir, flags, True)
    if json_data["asm"]:
        print("asm " + json_data["name"])
        flags = json_data["compiler_flags"].copy()
        flags.append("-s WASM=0")
        exec_wasm(json_data, work_dir, flags, False)

    base.delete_dir("./o")
    if json_data["run_after"] and base.is_file(work_dir + json_data["run_after"]):
        print("after " + json_data["run_after"])
        base.cmd_in_dir(work_dir, "python", [json_data["run_after"]])
