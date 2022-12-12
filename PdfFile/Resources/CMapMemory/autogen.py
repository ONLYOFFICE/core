import sys
sys.path.append("../../../../build_tools/scripts")
import base
import codecs

base.configure_common_apps()

cmap_directory = "./../CMap/"
cidToUnicode_names = ["Adobe-GB1", "Adobe-Korea1", "Adobe-KR", "Adobe-Japan1"]
cidToUnicode_values = {}

for item in cidToUnicode_names:
  cidToUnicode_values[item] = {}
  cmap_content = base.readFile(cmap_directory + item + ".cidToUnicode")
  arr_str_cidToUnicode = cmap_content.splitlines()
  arr_int_cidToUnicode = [int(i, 16) for i in arr_str_cidToUnicode]
  cidToUnicode_values[item]["code"] = "const unsigned int c_arr" + item.replace("-", "_") + "[] = {" + ",".join(map(str, arr_int_cidToUnicode)) + "};"
  cidToUnicode_values[item]["len"] = str(len(arr_int_cidToUnicode))

cmap_directory = "./../CMap/CMap/"

cmap_names = ["90ms-RKSJ-H", "90ms-RKSJ-V", "GBK-EUC-H"]
cmap_values = {}

for item in cmap_names:
  cmap_values[item] = {}
  cmap_content = base.readFile(cmap_directory + item)
  arr_str_cmap = cmap_content.splitlines()
  arr_wc_str_cmap = []
  for i in arr_str_cmap:
    if i and not i.startswith("%"):
      arr_wc_str_cmap.append(i + '\n')
  arr_int_cmap = []
  for i in arr_wc_str_cmap:
    arr_int_cmap += list(map(ord, i))
  cmap_values[item]["code"] = "const char c_arr" + item.replace("-", "_") + "[] = {" + ",".join(map(str, arr_int_cmap)) + "};"
  cmap_values[item]["len"] = str(len(arr_int_cmap))

content_cpp_file = []
content_cpp_file.append("#include <map>")
content_cpp_file.append("#include \"./cmap_memory.h\"")
content_cpp_file.append("")
content_cpp_file.append("// This file was generated and should not edited by hand")
content_cpp_file.append("")

for item in cidToUnicode_names:
  content_cpp_file.append(cidToUnicode_values[item]["code"])

content_cpp_file.append("struct TCidToUnicodeData")
content_cpp_file.append("{")
content_cpp_file.append("  const unsigned int* Data;")
content_cpp_file.append("  unsigned int Size;")
content_cpp_file.append("};")

for item in cmap_names:
  content_cpp_file.append(cmap_values[item]["code"])

content_cpp_file.append("struct TCMapData")
content_cpp_file.append("{")
content_cpp_file.append("  const char* Data;")
content_cpp_file.append("  unsigned int Size;")
content_cpp_file.append("};")

content_cpp_file.append("")

content_cpp_file.append("std::map<std::string, TCidToUnicodeData> g_memory_cid_to_unicode;")
content_cpp_file.append("bool PdfReader::GetCidToUnicodeMemoryMap(const char* name, const unsigned int*& data, unsigned int& size)")
content_cpp_file.append("{")
content_cpp_file.append("  if (g_memory_cid_to_unicode.empty())")
content_cpp_file.append("  {")
for item in cidToUnicode_names:
  content_cpp_file.append("    g_memory_cid_to_unicode.insert(std::pair<std::string, TCidToUnicodeData>(\"" + item + "\", { c_arr" + item.replace("-", "_") + ", " + cidToUnicode_values[item]["len"] + " }));")
content_cpp_file.append("  }")
content_cpp_file.append("  std::map<std::string, TCidToUnicodeData>::const_iterator iter = g_memory_cid_to_unicode.find(name);")
content_cpp_file.append("  if (iter != g_memory_cid_to_unicode.end())")
content_cpp_file.append("  {")
content_cpp_file.append("    data = iter->second.Data;")
content_cpp_file.append("    size = iter->second.Size;")
content_cpp_file.append("    return true;")
content_cpp_file.append("  }")
content_cpp_file.append("  data = NULL;")
content_cpp_file.append("  size = 0;")
content_cpp_file.append("  return false;")
content_cpp_file.append("}")
content_cpp_file.append("")

content_cpp_file.append("std::map<std::string, TCMapData> g_memory_cmap;")
content_cpp_file.append("bool PdfReader::GetMemoryCMap(const char* name, const char*& data, unsigned int& size)")
content_cpp_file.append("{")
content_cpp_file.append("  if (g_memory_cmap.empty())")
content_cpp_file.append("  {")
for item in cmap_names:
  content_cpp_file.append("    g_memory_cmap.insert(std::pair<std::string, TCMapData>(\"" + item + "\", { c_arr" + item.replace("-", "_") + ", " + cmap_values[item]["len"] + " }));")
content_cpp_file.append("  }")
content_cpp_file.append("  std::map<std::string, TCMapData>::const_iterator iter = g_memory_cmap.find(name);")
content_cpp_file.append("  if (iter != g_memory_cmap.end())")
content_cpp_file.append("  {")
content_cpp_file.append("    data = iter->second.Data;")
content_cpp_file.append("    size = iter->second.Size;")
content_cpp_file.append("    return true;")
content_cpp_file.append("  }")
content_cpp_file.append("  data = NULL;")
content_cpp_file.append("  size = 0;")
content_cpp_file.append("  return false;")
content_cpp_file.append("}")
content_cpp_file.append("")

base.writeFile("./cmap_memory.cpp", "\n".join(content_cpp_file))
