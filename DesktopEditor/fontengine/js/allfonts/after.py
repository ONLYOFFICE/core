#!/usr/bin/env python

import sys
sys.path.append("../../../../../build_tools/scripts")
import base
import os

base.configure_common_apps()
base.copy_file("./index.html", "./deploy/index.html")

#base.cmd_in_dir("../../../../Common/js", "python", ["./min.py", "./../../DesktopEditor/fontengine/js/allfonts/deploy/allfonts.js", "WHITESPACE_ONLY"])
