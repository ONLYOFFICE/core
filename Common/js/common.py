#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
sys.path.append("./../../../build_tools/scripts")
import base
import os

def apply_patch(file, patch):
  patch_content = base.readFile(patch)
  index1 = patch_content.find("<<<<<<<")
  index2 = patch_content.find("=======")
  index3 = patch_content.find(">>>>>>>")
  file_content_old = patch_content[index1 + 7:index2].strip()
  file_content_new = patch_content[index2 + 7:index3].strip()
  #file_content_new = "\n#if 0" + file_content_old + "#else" + file_content_new + "#endif\n"
  base.replaceInFile(file, file_content_old, file_content_new)
  return

def clear_dir(dir):
  if base.is_dir(dir):
    base.delete_dir(dir)
  base.create_dir(dir)
  return
