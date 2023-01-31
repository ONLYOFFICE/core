import sys;
sys.path.append("../../../../../../build_tools/scripts");
import base;

base.replaceInFile("./deploy/openssl.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];");
base.replaceInFile("./deploy/openssl.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];");
base.replaceInFile("./deploy/openssl.js", "function getBinaryPromise()", "function getBinaryPromise2()");

base.replaceInFile("./deploy/openssl_ie.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];");
base.replaceInFile("./deploy/openssl_ie.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];");
base.replaceInFile("./deploy/openssl_ie.js", "function getBinaryPromise()", "function getBinaryPromise2()");
