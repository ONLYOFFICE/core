import sys
sys.path.append('../../../../build_tools/scripts')
import base
import os

if not base.is_dir("glm"):
    base.cmd("git", ["clone", "https://github.com/g-truc/glm.git"])
    base.cmd_in_dir("glm", "git", ["checkout", "33b4a621a697a305bc3a7610d290677b96beb181", "--quiet"])

if not base.is_dir("mdds"):
    base.cmd("git", ["clone", "https://github.com/kohei-us/mdds.git"])
    base.cmd_in_dir("mdds", "git", ["checkout", "0783158939c6ce4b0b1b89e345ab983ccb0f0ad0"], "--quiet")

    #the linux code uses an implementation for c++ 17, so we just put this implementation
    if ("linux" == base.host_platform()):
        linux_cpp_version  = "#if __cplusplus < 201402L\n"
        linux_cpp_version += "namespace std {\n"
        linux_cpp_version += "  template<bool __v>\n"
        linux_cpp_version += "    using bool_constant = integral_constant<bool, __v>;\n\n"
        linux_cpp_version += "  template <class... _Types>\n"
        linux_cpp_version += "    using void_t = void;\n"
        linux_cpp_version += "}\n #endif\n\n"
        linux_cpp_version += "namespace mdds {"

        base.replaceInFile("./mdds/include/mdds/global.hpp", "namespace mdds {", linux_cpp_version)

if not base.is_dir("librevenge"):
    base.cmd("git", ["clone", "https://github.com/Distrotech/librevenge.git"])
    base.cmd_in_dir("librevenge", "git", ["checkout", "becd044b519ab83893ad6398e3cbb499a7f0aaf4", "--quiet"])

    stat_windows = ""
    stat_windows += "#if !defined(S_ISREG) && defined(S_IFMT) && defined(S_IFREG)\n"
    stat_windows += "#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)\n"
    stat_windows += "#endif\n"
    stat_windows += "#if !defined(S_ISDIR) && defined(S_IFMT) && defined(S_IFDIR)\n"
    stat_windows += "#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)\n"
    stat_windows += "#endif\n"

    base.replaceInFile("./librevenge/src/lib/RVNGDirectoryStream.cpp", "#include <librevenge-stream/librevenge-stream.h>", 
                       "#include <librevenge-stream/librevenge-stream.h>\n\n" + stat_windows)

if not base.is_dir("libodfgen"):
    base.cmd("git", ["clone", "https://github.com/Distrotech/libodfgen.git"])
    base.cmd_in_dir("libodfgen", "git", ["checkout", "8ef8c171ebe3c5daebdce80ee422cf7bb96aa3bc", "--quiet"])

if not base.is_dir("libetonyek"):
    base.cmd("git", ["clone", "https://github.com/LibreOffice/libetonyek.git"])
    base.cmd_in_dir("libetonyek", "git", ["checkout", "cb396b4a9453a457469b62a740d8fb933c9442c3", "--quiet"])

    base.replaceInFile("./libetonyek/src/lib/IWORKTable.cpp", "is_tree_valid", "valid_tree")

cmd_args  = sys.argv[1:]
use_gperf = False

for arg in cmd_args:
    if '--gperf' == arg:
        use_gperf = True

if use_gperf:
    base_gperf_args = ["--compare-strncmp", "--enum", "--null-strings", "--readonly-tables", "--language", "C++"]
    base_gperf_files = ["IWORKToken.gperf", "KEY1Token.gperf", "KEY2Token.gperf", "NUM1Token.gperf", "PAG1Token.gperf"]

    for file in base_gperf_files:
        base.cmd_in_dir("./libetonyek/src/lib", "gperf", base_gperf_args + [file, "--output-file", file[0:file.find(".")] + ".inc"])
else:
    base.copy_dir_content("./headers", "./libetonyek/src/lib")



