import sys
sys.path.append("../../../build_tools/scripts")
import base

base.configure_common_apps()

# finalize
base.replaceInFile("./deploy/zlib.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];")
base.replaceInFile("./deploy/zlib.js", "function getBinaryPromise(){", "function getBinaryPromise2(){")

base.copy_file("./wasm/js/index.html", "./deploy/index.html")
base.copy_file("./wasm/js/code.js", "./deploy/code.js")
