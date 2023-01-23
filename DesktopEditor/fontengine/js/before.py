#!/usr/bin/env python

import sys
import os
sys.path.append("../../../../build_tools/scripts")
import base

base.configure_common_apps()

base.cmd_in_dir("./../../graphics/pro/js", "./before.py")
