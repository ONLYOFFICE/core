import sys
sys.path.append("../../../../../../build_tools/scripts")
import base

# finalize
base.replaceInFile("./deploy/openssl.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];")
base.replaceInFile("./deploy/openssl.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];")
base.replaceInFile("./deploy/openssl.js", "function getBinaryPromise()", "function getBinaryPromise2()")

# write new version
base.copy_file("./index.html", "./deploy/index.html")
base.copy_file("./code.js",    "./deploy/code.js")
