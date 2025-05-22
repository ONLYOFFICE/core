#!/usr/bin/env python

import sys
sys.path.append('../../../../build_tools/scripts')
import config
import base
import os

base_directory = os.getcwd()

if not base.is_dir("md4c"):
  base.cmd("git", ["clone", "https://github.com/mity/md4c.git"])
  base.cmd_in_dir("md4c", "git", ["checkout", "481fbfbdf72daab2912380d62bb5f2187d438408"])