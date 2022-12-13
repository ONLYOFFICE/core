import sys;
sys.path.append("../../../../../../../build_tools/scripts");
import base;

base.replaceInFile("./deploy/engine.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){window.cryptoJS.onLoad();}];");
base.replaceInFile("./deploy/engine.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){window.cryptoJS.onLoad();}];");
base.replaceInFile("./deploy/engine.js", "function getBinaryPromise()", "function getBinaryPromise2()");

base.copy_file("./index.html", "./deploy/index.html");
base.copy_file("./code.js", "./deploy/code.js")