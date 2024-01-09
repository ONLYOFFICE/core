import sys;
sys.path.append("../../../../../../../build_tools/scripts");
import base;

base.replaceInFile("./deploy/engine.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){window.cryptoJS.onLoad();}];");
base.replaceInFile("./deploy/engine.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){window.cryptoJS.onLoad();}];");
base.replaceInFile("./deploy/engine.js", "function getBinaryPromise(", "function getBinaryPromise2(");

base.replaceInFile("./deploy/engine_ie.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){window.cryptoJS.onLoad();}];");
base.replaceInFile("./deploy/engine_ie.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){window.cryptoJS.onLoad();}];");
base.replaceInFile("./deploy/engine_ie.js", "function getBinaryPromise(", "function getBinaryPromise2(");

base.cmd_in_dir("../../../../../../Common/js", "python", ["./min.py", "./../../DesktopEditor/xmlsec/src/wasm/oform_plugin/module/deploy/engine.js", "WHITESPACE_ONLY"])
base.cmd_in_dir("../../../../../../Common/js", "python", ["./min.py", "./../../DesktopEditor/xmlsec/src/wasm/oform_plugin/module/deploy/engine_ie.js", "WHITESPACE_ONLY"])