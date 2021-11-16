import sys
sys.path.append("../../../../../build_tools/scripts")
import base
import os
import json

base.configure_common_apps()


def apply_patch(file, patch):
    patch_content = base.readFile(patch)
    index1 = patch_content.find("<<<<<<<")
    index2 = patch_content.find("=======")
    index3 = patch_content.find(">>>>>>>")
    file_content_old = patch_content[index1 + 7:index2]
    file_content_new = "\n#if 0" + file_content_old + "#else" + patch_content[index2 + 7:index3] + "#endif\n"
    base.replaceInFile(file, file_content_old, file_content_new)
    return


if not base.is_dir("xml"):
    base.copy_dir("../../../xml", "./xml")
    base.replaceInFile("./xml/libxml2/libxml.h", "xmlNop(void)", "xmlNop(void* context, char* buffer, int len)")
    base.replaceInFile("./xml/libxml2/xmlIO.c", "xmlNop(void)", "xmlNop(void* context, char* buffer, int len)")
    base.replaceInFile("./xml/src/xmllight_private.h", "#include \"../../common/", "#include \"../../../../../common/")
    base.replaceInFile("./xml/include/xmlutils.h", "#include \"../../common/", "#include \"../../../../../common/")

if not base.is_dir("freetype-2.10.4"):
    base.copy_dir("../../../freetype-2.10.4", "./freetype-2.10.4")
    # smooth
    base.copy_file("./freetype-2.10.4/src/smooth/ftgrays.c", "./freetype-2.10.4/src/smooth/ftgrays.cpp")
    apply_patch("./freetype-2.10.4/src/smooth/ftgrays.cpp", "./wasm/patches/ftgrays1.patch")
    apply_patch("./freetype-2.10.4/src/smooth/ftgrays.cpp", "./wasm/patches/ftgrays2.patch")
    apply_patch("./freetype-2.10.4/src/smooth/ftgrays.cpp", "./wasm/patches/ftgrays3.patch")
    base.copy_file("./freetype-2.10.4/src/smooth/smooth.c", "./freetype-2.10.4/src/smooth/smooth.cpp")
    apply_patch("./freetype-2.10.4/src/smooth/smooth.cpp", "./wasm/patches/smooth.patch")
    # ftobjs
    apply_patch("./freetype-2.10.4/src/base/ftobjs.c", "./wasm/patches/ftobjs1.patch")
    apply_patch("./freetype-2.10.4/src/base/ftobjs.c", "./wasm/patches/ftobjs2.patch")
    # ttcmap
    base.copy_file("./freetype-2.10.4/src/sfnt/ttcmap.c", "./freetype-2.10.4/src/sfnt/ttcmap.cpp")
    apply_patch("./freetype-2.10.4/src/sfnt/ttcmap.cpp", "./wasm/patches/ttcmap.patch")
    base.copy_file("./freetype-2.10.4/src/sfnt/sfnt.c", "./freetype-2.10.4/src/sfnt/sfnt.cpp")
    apply_patch("./freetype-2.10.4/src/sfnt/sfnt.cpp", "./wasm/patches/sfnt.patch")

base.replaceInFile("../../../../Common/3dParty/icu/icu/source/common/udata.cpp", "\n{\n    UDataMemory tData;", "\n{\n#ifdef BUILDING_WASM_MODULE\nreturn NULL;\n#endif\n    UDataMemory tData;")
