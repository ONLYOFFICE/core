#!/usr/bin/env python

import sys
sys.path.append("../../../../build_tools/scripts")
import base
import os

base.configure_common_apps()
base.replaceInFile("./deploy/fonts.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){window[\"AscFonts\"].onLoadModule();}];")
base.replaceInFile("./deploy/fonts.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){window[\"AscFonts\"].onLoadModule();}];")
base.replaceInFile("./deploy/fonts.js", "function getBinaryPromise(", "function getBinaryPromise2(")

base.replaceInFile("./deploy/fonts_ie.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){window[\"AscFonts\"].onLoadModule();}];")
base.replaceInFile("./deploy/fonts_ie.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){window[\"AscFonts\"].onLoadModule();}];")
base.replaceInFile("./deploy/fonts_ie.js", "function getBinaryPromise(", "function getBinaryPromise2(")

base.cmd_in_dir("../../../Common/js", "python", ["./min.py", "./../../DesktopEditor/fontengine/js/deploy/fonts.js", "WHITESPACE_ONLY"])
base.cmd_in_dir("../../../Common/js", "python", ["./min.py", "./../../DesktopEditor/fontengine/js/deploy/fonts_ie.js", "WHITESPACE_ONLY"])
