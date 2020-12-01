#!/usr/bin/env python

import sys
sys.path.append('./../../../build_tools/scripts')
import base

def make():
    base.configure_common_apps("../../../build_tools/scripts/base.py")

    with open("template.docx", "rb") as f:
      binary_content = bytearray(open("template.docx", "rb").read())

    text_indent = "    " # 4 spaces
    len_content = len(binary_content)
    cpp_content = "// [START]"
    cpp_content += "\n" + text_indent
    cpp_content += "int template_binary_len = " + str(len_content) + ";"
    cpp_content += "\n" + text_indent
    cpp_content += "BYTE template_binary[" + str(len_content) + "] = {"
    for byte in binary_content:
      cpp_content += str(byte)
      cpp_content += ","
    cpp_content = cpp_content[0:-1]
    cpp_content += "};"
    cpp_content += "\n" + text_indent
    cpp_content += "// [END]"

    template_path = "./template.h"
    header_content = base.readFile(template_path)
    index_start = header_content.find("// [START]")
    index_end = header_content.find("// [END]") + 8

    header_content_new = header_content[0:index_start] + cpp_content + header_content[index_end:]

    base.delete_file(template_path)
    with open(base.get_path(template_path), "w") as file:
      file.write(header_content_new)

    return

make()
