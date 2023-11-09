import sys
sys.path.append("../../../../../build_tools/scripts")
import base

base.configure_common_apps()

base.replaceInFile("../../../../Common/3dParty/icu/icu/source/common/udata.cpp", "\n{\n#ifdef BUILDING_WASM_MODULE\nreturn NULL;\n#endif\n    UDataMemory tData;", "\n{\n    UDataMemory tData;")

# finalize
base.replaceInFile("./deploy/drawingfile.js", "function getBinaryPromise()", "function getBinaryPromise2()")
base.replaceInFile("./deploy/drawingfile.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){window[\"AscViewer\"] && window[\"AscViewer\"][\"onLoadModule\"] && window[\"AscViewer\"][\"onLoadModule\"]();}];")
base.replaceInFile("./deploy/drawingfile.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){window[\"AscViewer\"] && window[\"AscViewer\"][\"onLoadModule\"] && window[\"AscViewer\"][\"onLoadModule\"]();}];")
base.replaceInFile("./deploy/drawingfile.js", "\"drawingfile.js.mem\"", "getMemoryPathIE(\"drawingfile.js.mem\")")

base.replaceInFile("./deploy/drawingfile_ie.js", "function getBinaryPromise()", "function getBinaryPromise2()")
base.replaceInFile("./deploy/drawingfile_ie.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){window[\"AscViewer\"] && window[\"AscViewer\"][\"onLoadModule\"] && window[\"AscViewer\"][\"onLoadModule\"]();}];")
base.replaceInFile("./deploy/drawingfile_ie.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){window[\"AscViewer\"] && window[\"AscViewer\"][\"onLoadModule\"] && window[\"AscViewer\"][\"onLoadModule\"]();}];")
base.replaceInFile("./deploy/drawingfile_ie.js", "\"drawingfile.js.mem\"", "getMemoryPathIE(\"drawingfile.js.mem\")")

base.replaceInFile("./deploy/drawingfile.js", "{credentials:\"same-origin\"}", "{credentials:\"same-origin\",mode:\"no-cors\"}")
base.replaceInFile("./deploy/drawingfile_ie.js", "{credentials:\"same-origin\"}", "{credentials:\"same-origin\",mode:\"no-cors\"}")

base.cmd_in_dir("../../../../Common/js", "python", ["./min.py", "./../../DesktopEditor/graphics/pro/js/deploy/drawingfile.js", "WHITESPACE_ONLY"])
base.cmd_in_dir("../../../../Common/js", "python", ["./min.py", "./../../DesktopEditor/graphics/pro/js/deploy/drawingfile_ie.js", "WHITESPACE_ONLY"])

# base.delete_dir("./xml")
# base.delete_dir("./freetype-2.10.4")
