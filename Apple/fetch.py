import sys
sys.path.append("../../build_tools/scripts")
import base
import os

base.cmd("git", ["clone", "https://github.com/g-truc/glm.git"])
base.cmd("git", ["clone", "https://github.com/kohei-us/mdds.git"])
base.cmd("git", ["clone", "https://github.com/Distrotech/librevenge.git"])
base.cmd("git", ["clone", "https://github.com/Distrotech/libodfgen.git"])
base.cmd("git", ["clone", "https://github.com/LibreOffice/libetonyek.git"])

stat_windows = ""
stat_windows += "#if !defined(S_ISREG) && defined(S_IFMT) && defined(S_IFREG)\n"
stat_windows += "#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)\n"
stat_windows += "#endif\n"
stat_windows += "#if !defined(S_ISDIR) && defined(S_IFMT) && defined(S_IFDIR)\n"
stat_windows += "#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)\n"
stat_windows += "#endif\n"

base.replaceInFile("./librevenge/src/lib/RVNGDirectoryStream.cpp", "#include <librevenge-stream/librevenge-stream.h>", 
    "#include <librevenge-stream/librevenge-stream.h>\n\n" + stat_windows)

base_gperf_args = ["--compare-strncmp", "--enum", "--null-strings", "--readonly-tables", "--language", "C++"]
base_gperf_files = ["IWORKToken.gperf", "KEY1Token.gperf", "KEY2Token.gperf", "NUM1Token.gperf", "PAG1Token.gperf"]

for file in base_gperf_files:
    base.cmd_in_dir("./libetonyek/src/lib", "gperf", base_gperf_args + [file, "--output-file", file[0:file.find(".")] + ".inc"])
