#!/usr/bin/env python

import sys
sys.path.append('../../../../build_tools/scripts')
import base

if not base.is_dir("libxml2"):
  base.cmd("git", ["clone", "https://github.com/GNOME/libxml2.git"])
  base.cmd_in_dir("libxml2", "git", ["checkout", "39b66eb2089f491eacf8e22a7965a235f7f5735e", "--quiet"])
  
  #fix parserInternals.h
  base.replaceInFileUtf8("libxml2/include/libxml/parserInternals.h", "#define XML_MAX_TEXT_LENGTH 10000000", "#define XML_MAX_TEXT_LENGTH 15000000")
  base.replaceInFileUtf8("libxml2/include/libxml/parserInternals.h", "#define XML_MAX_LOOKUP_LIMIT 10000000", "#define XML_MAX_LOOKUP_LIMIT 15000000")
  
  #Logging has been removed
  target = "void\nxmlGenericErrorDefaultFunc(void *ctx ATTRIBUTE_UNUSED, const char *msg, ...) {\n    va_list args;\n\n    if (xmlGenericErrorContext == NULL)\n	xmlGenericErrorContext = (void *) stderr;\n\n    va_start(args, msg);\n    vfprintf((FILE *)xmlGenericErrorContext, msg, args);\n    va_end(args);\n}"
  replacement = "#ifndef XML_ERROR_DISABLE_MODE\n" + target + "\n#else\nvoid\nxmlGenericErrorDefaultFunc(void *ctx ATTRIBUTE_UNUSED, const char *msg, ...) {\n	// NONE\n}\n#endif"
  base.replaceInFile("libxml2/error.c", target, replacement)
