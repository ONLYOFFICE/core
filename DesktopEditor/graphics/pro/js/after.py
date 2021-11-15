import sys
sys.path.append("../../../../../build_tools/scripts")
import base
import os
import json

base.configure_common_apps()

base.replaceInFile("../../../../Common/3dParty/icu/icu/source/common/udata.cpp", "\n{\n#ifdef BUILDING_WASM_MODULE\nreturn NULL;\n#endif\n    UDataMemory tData;", "\n{\n    UDataMemory tData;")

# finalize
base.replaceInFile("./drawingfile.js", "function getBinaryPromise()", "function getBinaryPromise2()")
base.replaceInFile("./drawingfile.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){window[\"AscViewer\"] && window[\"AscViewer\"][\"onLoadModule\"] && window[\"AscViewer\"][\"onLoadModule\"]();}];")
base.replaceInFile("./drawingfile.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){window[\"AscViewer\"] && window[\"AscViewer\"][\"onLoadModule\"] && window[\"AscViewer\"][\"onLoadModule\"]();}];")
base.replaceInFile("./drawingfile.js", "\"drawingfile.js.mem\"", "getMemoryPathIE(\"drawingfile.js.mem\")")

base.delete_dir("./xml")
base.delete_dir("./freetype-2.10.4")
