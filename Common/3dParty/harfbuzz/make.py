#!/usr/bin/env python

import sys
import os
sys.path.append("../../../../build_tools/scripts")
import base

def apply_patch(file, patch):
  file_content = base.readFile(file)
  patch_content = base.readFile(patch)
  index1 = patch_content.find("<<<<<<<")
  index2 = patch_content.find("=======")
  index3 = patch_content.find(">>>>>>>")
  file_content_old = patch_content[index1 + 7:index2]
  file_content_new = "\n#if 0" + file_content_old + "#else" + patch_content[index2 + 7:index3] + "#endif\n"
  base.replaceInFile(file, file_content_old, file_content_new)
  return

def read_files(folder, addon=""):
  ret_data = os.listdir(folder)
  for i in range(len(ret_data)):
    ret_data[i] = addon + ret_data[i]
  return ret_data

def clear_module():
  if base.is_dir("harfbuzz"):
    base.delete_dir_with_access_error("harfbuzz")
  return

base.check_module_version("1", clear_module)

# fetch harfbuzz
if not base.is_dir("harfbuzz"):
  base.cmd("git", ["clone", "https://github.com/harfbuzz/harfbuzz.git"])
  os.chdir("harfbuzz")
  base.cmd("git", ["checkout", "894a1f72ee93a1fd8dc1d9218cb3fd8f048be29a"])
  os.chdir("../")

  apply_patch("./harfbuzz/src/hb-ft.cc", "./patch/hb-ft.cc.patch")

  qmake_content_lines = []
  
  qmake_content_lines.append("SRC_DIR = $$PWD/harfbuzz/src")
  qmake_content_lines.append("")
  qmake_content_lines.append("DEFINES += \\")
  qmake_content_lines.append("  HAVE_FREETYPE \\")
  qmake_content_lines.append("  HB_NO_VISIBILITY")
  qmake_content_lines.append("")
  qmake_content_lines.append("INCLUDEPATH += \\")
  qmake_content_lines.append("  $$SRC_DIR \\")
  qmake_content_lines.append("")

  all_files = read_files("./harfbuzz/src")
  all_files += read_files("./harfbuzz/src/graph", "graph/")

  headers_files = []
  sources_files = []

  headers_exclude = []

  sources_exclude = []
  sources_exclude.append("main.cc")
  sources_exclude.append("failing-alloc.c")

  sources_exclude_filter = []
  sources_exclude_filter.append("test")
  sources_exclude_filter.append("harfbuzz")

  for item in all_files:
    arr_split = os.path.splitext(item)
    if len(arr_split) == 0:
      continue
    ext = arr_split[-1]
    if (ext == ".h") or (ext == ".hh") or (ext == ".c") or (ext == ".cc"):
      test_file_name = os.path.basename(item)
      for extest in sources_exclude_filter:
        if (0 == test_file_name.find(extest)):
          sources_exclude.append(item)
          break

  for item in all_files:
    arr_split = os.path.splitext(item)
    if len(arr_split) == 0:
      continue
    ext = arr_split[-1]
    if (ext == ".h") or (ext == ".hh"):
      headers_files.append(item)
    if (ext == ".c") or (ext == ".cc"):
      sources_files.append(item)

  qmake_content_lines.append("HEADERS += \\")
  for item in headers_files:
    if item in headers_exclude:
      continue
    qmake_content_lines.append("  $$SRC_DIR/" + item + " \\")
  qmake_content_lines[-1] = qmake_content_lines[-1].replace(" \\", "")

  qmake_content_lines.append("")
  qmake_content_lines.append("SOURCES += \\")
  for item in sources_files:
    if item in sources_exclude:
      continue
    qmake_content_lines.append("  $$SRC_DIR/" + item + " \\")
  qmake_content_lines[-1] = qmake_content_lines[-1].replace(" \\", "")

  qmake_content_lines.append("")

  if len(headers_exclude) != 0:
    qmake_content_lines.append("#HEADERS += \\")
    for item in headers_exclude:
      qmake_content_lines.append("#  $$SRC_DIR/" + item + " \\")
    qmake_content_lines[-1] = qmake_content_lines[-1].replace(" \\", "")
    qmake_content_lines.append("")

  if len(sources_exclude) != 0:
    qmake_content_lines.append("#SOURCES += \\")
    for item in sources_exclude:
      qmake_content_lines.append("#  $$SRC_DIR/" + item + " \\")
    qmake_content_lines[-1] = qmake_content_lines[-1].replace(" \\", "")
    qmake_content_lines.append("")
  else:
    qmake_content_lines.append("")

  if (base.is_file("./harfbuzz.pri")):
    base.delete_file("./harfbuzz.pri")

  with open("./harfbuzz.pri", "w") as file:
    file.write("\n".join(qmake_content_lines))

  #base.delete_file("./harfbuzz/src/hb-ft.cc")
  #base.copy_file("./patch/hb-ft.cc", "./harfbuzz/src/hb-ft.cc")
