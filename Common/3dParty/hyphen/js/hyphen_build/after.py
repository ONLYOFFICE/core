import sys
sys.path.append("../../../../../../build_tools/scripts")
import base

base.configure_common_apps()
base.replaceInFile("../deploy/engine/hyphen.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[onLoadModule];")
base.replaceInFile("../deploy/engine/hyphen_ie.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[onLoadModule];")

base.copy_file("../library.js", "../deploy/hyphen.js")