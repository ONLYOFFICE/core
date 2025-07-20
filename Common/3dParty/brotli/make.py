#!/usr/bin/env python

import sys
import os
sys.path.append("../../../../build_tools/scripts")
import base

def clear_module():
  if base.is_dir("brotli"):
    base.delete_dir_with_access_error("brotli")
  return

base.check_module_version("1", clear_module)

# fetch harfbuzz
if not base.is_dir("brotli"):
  base.cmd("git", ["clone", "https://github.com/google/brotli.git"])
  os.chdir("brotli")
  base.cmd("git", ["checkout", "a47d7475063eb223c87632eed806c0070e70da29"])
  os.chdir("../")
