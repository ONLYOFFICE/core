import sys
sys.path.append("../../../../build_tools/scripts")
import base
import codecs

base.configure_common_apps()

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

cmap_directory = "./../CMap/CMap/"

## GB1
#cmap_names = ["GB-EUC-H", "GB-EUC-V", "GB-H", "GB-V", "GBpc-EUC-H", "GBpc-EUC-V", "GBK-EUC-H", "GBK-EUC-V", "GBKp-EUC-H", "GBKp-EUC-V", "GBK2K-H", "GBK2K-V", "GBT-H", "GBT-V", "GBTpc-EUC-H", "GBTpc-EUC-V", "UniGB-UCS2-H", "UniGB-UCS2-V", "UniGB-UTF8-H", "UniGB-UTF8-V", "UniGB-UTF16-H", "UniGB-UTF16-V", "UniGB-UTF32-H", "UniGB-UTF32-V"]
## CNS1
#cmap_names += ["B5pc-H", "B5pc-V", "B5-H", "B5-V", "HKscs-B5-H", "HKscs-B5-V", "HKdla-B5-H", "HKdla-B5-V", "HKdlb-B5-H", "HKdlb-B5-V", "HKgccs-B5-H", "HKgccs-B5-V", "HKm314-B5-H", "HKm314-B5-V", "HKm471-B5-H", "HKm471-B5-V", "ETen-B5-H", "ETen-B5-V", "ETenms-B5-H", "ETenms-B5-V", "ETHK-B5-H", "ETHK-B5-V", "CNS-EUC-H", "CNS-EUC-V", "CNS1-H", "CNS1-V", "CNS2-H", "CNS2-V", "UniCNS-UCS2-H", "UniCNS-UCS2-V", "UniCNS-UTF8-H", "UniCNS-UTF8-V", "UniCNS-UTF16-H", "UniCNS-UTF16-V", "UniCNS-UTF32-H", "UniCNS-UTF32-V"]
## Japan1
#cmap_names += ["78-EUC-H", "78-EUC-V", "78-H", "78-V", "78-RKSJ-H", "78-RKSJ-V", "78ms-RKSJ-H", "78ms-RKSJ-V","83pv-RKSJ-H", "90ms-RKSJ-H", "90ms-RKSJ-V", "90msp-RKSJ-H", "90msp-RKSJ-V", "90pv-RKSJ-H", "90pv-RKSJ-V", "Add-H", "Add-V", "Add-RKSJ-H", "Add-RKSJ-V", "EUC-H", "EUC-V", "Ext-RKSJ-H", "Ext-RKSJ-V", "H", "V", "NWP-H", "NWP-V", "RKSJ-H", "RKSJ-V", "UniJIS-UCS2-H", "UniJIS-UCS2-V", "UniJIS-UCS2-HW-H", "UniJIS-UCS2-HW-V", "UniJIS-UTF8-H", "UniJIS-UTF8-V", "UniJIS-UTF16-H", "UniJIS-UTF16-V", "UniJIS-UTF32-H", "UniJIS-UTF32-V", "UniJIS2004-UTF8-H", "UniJIS2004-UTF8-V", "UniJIS2004-UTF16-H", "UniJIS2004-UTF16-V", "UniJIS2004-UTF32-H", "UniJIS2004-UTF32-V", "UniJISPro-UCS2-V", "UniJISPro-UCS2-HW-V", "UniJISPro-UTF8-V", "UniJISX0213-UTF32-H", "UniJISX0213-UTF32-V", "UniJISX02132004-UTF32-H", "UniJISX02132004-UTF32-V", "WP-Symbol", "Hankaku", "Hiragana", "Katakana", "Roman"]
## Korea1
#cmap_names += ["KSC-EUC-H", "KSC-EUC-V", "KSC-H", "KSC-V", "KSC-Johab-H", "KSC-Johab-V", "KSCms-UHC-H", "KSCms-UHC-V", "KSCms-UHC-HW-H", "KSCms-UHC-HW-V", "KSCpc-EUC-H", "KSCpc-EUC-V", "UniKS-UCS2-H", "UniKS-UCS2-V", "UniKS-UTF8-H", "UniKS-UTF8-V", "UniKS-UTF16-H", "UniKS-UTF16-V", "UniKS-UTF32-H", "UniKS-UTF32-V"]
## KR
#cmap_names += ["UniAKR-UTF8-H", "UniAKR-UTF16-H", "UniAKR-UTF32-H"]
#cmap_values = {}
#
#for item in cmap_names:
#  cmap_values[item] = {}
#  cmap_content = base.readFile(cmap_directory + item)
#  arr_str_cmap = cmap_content.splitlines()
#  arr_wc_str_cmap = []
#  for i in arr_str_cmap:
#    if i and not i.startswith("%"):
#      arr_wc_str_cmap.append(i + '\n')
#  arr_int_cmap = []
#  for i in arr_wc_str_cmap:
#    arr_int_cmap += list(map(ord, i))
#  cmap_values[item]["code"] = "const char c_arr" + item.replace("-", "_") + "[] = {" + ",".join(map(str, arr_int_cmap)) + "};"
#  cmap_values[item]["len"] = str(len(arr_int_cmap))

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

#for item in cmap_names:
#  content_cpp_file.append(cmap_values[item]["code"])
#
#content_cpp_file.append("struct TCMapData")
#content_cpp_file.append("{")
#content_cpp_file.append("  const char* Data;")
#content_cpp_file.append("  unsigned int Size;")
#content_cpp_file.append("};")

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

#content_cpp_file.append("std::map<std::string, TCMapData> g_memory_cmap;")
#content_cpp_file.append("bool PdfReader::GetMemoryCMap(const char* name, const char*& data, unsigned int& size)")
#content_cpp_file.append("{")
#content_cpp_file.append("  if (g_memory_cmap.empty())")
#content_cpp_file.append("  {")
#for item in cmap_names:
#  content_cpp_file.append("    g_memory_cmap.insert(std::pair<std::string, TCMapData>(\"" + item + "\", { c_arr" + item.replace("-", "_") + ", " + cmap_values[item]["len"] + " }));")
#content_cpp_file.append("  }")
#content_cpp_file.append("  std::map<std::string, TCMapData>::const_iterator iter = g_memory_cmap.find(name);")
#content_cpp_file.append("  if (iter != g_memory_cmap.end())")
#content_cpp_file.append("  {")
#content_cpp_file.append("    data = iter->second.Data;")
#content_cpp_file.append("    size = iter->second.Size;")
#content_cpp_file.append("    return true;")
#content_cpp_file.append("  }")
#content_cpp_file.append("  data = NULL;")
#content_cpp_file.append("  size = 0;")
#content_cpp_file.append("  return false;")
#content_cpp_file.append("}")
#content_cpp_file.append("")

base.writeFile("./cmap_memory.cpp", "\n".join(content_cpp_file))
