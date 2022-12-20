import sys
sys.path.append("../../../../build_tools/scripts")
import base
import codecs

base.configure_common_apps()

# .cidToUnicode
cmap_directory = "./../CMap/"
cidToUnicode_names = ["Adobe-GB1", "Adobe-CNS1", "Adobe-Korea1", "Adobe-KR", "Adobe-Japan1"]
cidToUnicode_values = {}

for item in cidToUnicode_names:
  cidToUnicode_values[item] = {}
  cmap_content = base.readFile(cmap_directory + item + ".cidToUnicode")
  arr_str_cidToUnicode = cmap_content.splitlines()
  arr_int_cidToUnicode = [int(i, 16) for i in arr_str_cidToUnicode]
  cidToUnicode_values[item]["code"] = "const unsigned int c_arr" + item.replace("-", "_") + "[] = {" + ",".join(map(str, arr_int_cidToUnicode)) + "};"
  cidToUnicode_values[item]["len"] = str(len(arr_int_cidToUnicode))

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

base.writeFile("./cmap_memory.cpp", "\n".join(content_cpp_file))
