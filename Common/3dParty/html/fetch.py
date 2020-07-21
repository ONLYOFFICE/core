#!/usr/bin/env python

import sys
sys.path.append('../../../../build_tools/scripts')
import config
import base
import os
import build

if not base.is_dir("gumbo-parser"):
  base.cmd("git", ["clone", "https://github.com/google/gumbo-parser.git"])

if not base.is_dir("katana-parser"):
  base.cmd("git", ["clone", "https://github.com/hackers-painters/katana-parser.git"])