import sys
sys.path.append('../../../../../../build_tools/scripts')
import base
import os

base.configure_common_apps()

contentHeader = base.readFile("./deploy/engine.js")
headerLast = contentHeader.find("*/")
contentHeader = contentHeader[:headerLast+2]

base.replaceInFile("./deploy/engine.js", contentHeader, "")
base.replaceInFile("./deploy/engine_ie.js", contentHeader, "")

base.replaceInFile("./deploy/engine.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];")
base.replaceInFile("./deploy/engine.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];")
base.replaceInFile("./deploy/engine.js", "function getBinaryPromise()", "function getBinaryPromise2()")

base64_js_content      = base.readFile("./../../../../../../../sdkjs/common/stringserialize.js")
base64_js_content      = base64_js_content.replace("(window);", "(self);")

base.replaceInFile("./deploy/engine.js", "&&window===this?this", "?window")
base.replaceInFile("./deploy/engine.js", "//base64", base64_js_content)

base.replaceInFile("./deploy/engine_ie.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];")
base.replaceInFile("./deploy/engine_ie.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];")
base.replaceInFile("./deploy/engine_ie.js", "function getBinaryPromise()", "function getBinaryPromise2()")

base.replaceInFile("./deploy/engine_ie.js", "&&window===this?this", "?window")
base.replaceInFile("./deploy/engine_ie.js", "//base64", base64_js_content)
