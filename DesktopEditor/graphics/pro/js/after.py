import sys
sys.path.append("../../../../../build_tools/scripts")
import base

base.configure_common_apps()

base.replaceInFile("../../../../Common/3dParty/icu/icu/source/common/udata.cpp", "\n{\n#ifdef BUILDING_WASM_MODULE\nreturn NULL;\n#endif\n    UDataMemory tData;", "\n{\n    UDataMemory tData;")
base.replaceInFile("../../../../DesktopEditor/cximage/png/pnglibconf.h", "//#define PNG_CONSOLE_IO_SUPPORTED", "#define PNG_CONSOLE_IO_SUPPORTED")

base.replaceInFile("../../../../Common/3dParty/openssl/openssl/crypto/sha/sha512.c", "const SHA_LONG64 *W = (const SHA_LONG64*)in;", "const SHA_LONG64 *W = in;")

# finalize
base.replaceInFile("./deploy/drawingfile.js", "function getBinaryPromise(", "function getBinaryPromise2(")
base.replaceInFile("./deploy/drawingfile.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){window[\"AscViewer\"] && window[\"AscViewer\"][\"onLoadModule\"] && window[\"AscViewer\"][\"onLoadModule\"]();}];")
base.replaceInFile("./deploy/drawingfile.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){window[\"AscViewer\"] && window[\"AscViewer\"][\"onLoadModule\"] && window[\"AscViewer\"][\"onLoadModule\"]();}];")
base.replaceInFile("./deploy/drawingfile.js", "\"drawingfile.js.mem\"", "getMemoryPathIE(\"drawingfile.js.mem\")")

base.replaceInFile("./deploy/drawingfile_ie.js", "function getBinaryPromise(", "function getBinaryPromise2(")
base.replaceInFile("./deploy/drawingfile_ie.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){window[\"AscViewer\"] && window[\"AscViewer\"][\"onLoadModule\"] && window[\"AscViewer\"][\"onLoadModule\"]();}];")
base.replaceInFile("./deploy/drawingfile_ie.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){window[\"AscViewer\"] && window[\"AscViewer\"][\"onLoadModule\"] && window[\"AscViewer\"][\"onLoadModule\"]();}];")
base.replaceInFile("./deploy/drawingfile_ie.js", "\"drawingfile.js.mem\"", "getMemoryPathIE(\"drawingfile.js.mem\")")

base.cmd_in_dir("../../../../Common/js", "python", ["./min.py", "./../../DesktopEditor/graphics/pro/js/deploy/drawingfile.js", "WHITESPACE_ONLY"])
base.cmd_in_dir("../../../../Common/js", "python", ["./min.py", "./../../DesktopEditor/graphics/pro/js/deploy/drawingfile_ie.js", "WHITESPACE_ONLY"])

content_native_files = [
	"./wasm/js/stream.js",
	"./wasm/js/drawingfile.js"
]

content_native = "(function(window, undefined) {\n"
for item in content_native_files:
	content_native += base.readFile(item)
content_native += "\n})(window, undefined);"
content_native = content_native.replace("//file_internal", base.readFile("./wasm/js/drawingfile_native.js"))
content_native = content_native.replace("//string_utf8", base.readFile("./../../../../Common/js/string_utf8.js"))
base.writeFile("./deploy/drawingfile_native.js", content_native)
base.cmd_in_dir("../../../../Common/js", "python", ["./min.py", "./../../DesktopEditor/graphics/pro/js/deploy/drawingfile_native.js", "WHITESPACE_ONLY"])

# base.delete_dir("./xml")
# base.delete_dir("./freetype-2.10.4")
