import sys;
sys.path.append("../../../../../../../build_tools/scripts");
import base;

base.replaceInFile("./deploy/engine.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){window.cryptoJS.onLoad();}];");
base.replaceInFile("./deploy/engine.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){window.cryptoJS.onLoad();}];");
base.replaceInFile("./deploy/engine.js", "function getBinaryPromise()", "function getBinaryPromise2()");

base.replaceInFile("./deploy/engine_ie.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){window.cryptoJS.onLoad();}];");
base.replaceInFile("./deploy/engine_ie.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){window.cryptoJS.onLoad();}];");
base.replaceInFile("./deploy/engine_ie.js", "function getBinaryPromise()", "function getBinaryPromise2()");
