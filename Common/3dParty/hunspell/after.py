import sys
sys.path.append("../../../../build_tools/scripts")
import base
import os
import json

base.configure_common_apps()

# finalize
base.replaceInFile("./deploy/spell/spell.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];")
base.replaceInFile("./deploy/spell/spell.js", "function getBinaryPromise(){", "function getBinaryPromise2(){")

base.replaceInFile("./deploy/spell/spell_ie.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];")
base.replaceInFile("./deploy/spell/spell_ie.js", "function getBinaryPromise(){", "function getBinaryPromise2(){")

# write new version
base.copy_file("./wasm/js/code.js", "./deploy/spell.js")
base.copy_file("./wasm/js/index.html", "./deploy/index.html")
