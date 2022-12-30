import sys
sys.path.append("../../../../../../build_tools/scripts")
import base

base.cmd_in_dir("./../3rdParty", "python", ["openssl.py"])

if not base.is_dir("./../3rdPartyxml"):
  base.print_info("Copy xml...")
  base.copy_dir("./../../../../xml", "./../3rdParty/xml")
  base.replaceInFile("./../3rdParty/xml/libxml2/libxml.h", "xmlNop(void)", "xmlNop(void* context, char* buffer, int len)")
  base.replaceInFile("./../3rdParty/xml/libxml2/xmlIO.c", "xmlNop(void)", "xmlNop(void* context, char* buffer, int len)")
  base.replaceInFile("./../3rdParty/xml/src/xmllight_private.h", "#include \"../../common/", "#include \"../../../../../../common/")
  base.replaceInFile("./../3rdParty/xml/include/xmlutils.h", "#include \"../../common/", "#include \"../../../../../../common/")
  base.replaceInFile("./../3rdParty/xml/include/xmlwriter.h", "#include \"../../common/", "#include \"../../../../../../common/")
