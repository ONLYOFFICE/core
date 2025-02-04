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

    fix_cpp_version  = "#if __cplusplus < 201402L\n"
    fix_cpp_version += "#ifndef _MSC_VER\n"
    fix_cpp_version += "namespace std {\n"
    fix_cpp_version += "  template<bool __v>\n"
    fix_cpp_version += "  using bool_constant = integral_constant<bool, __v>;\n\n"
    fix_cpp_version += "  template <class... _Types>\n"
    fix_cpp_version += "  using void_t = void;\n"
    fix_cpp_version += "}\n#endif\n"
    fix_cpp_version += "#endif\n\n"
    fix_cpp_version += "namespace mdds {"

    base.replaceInFile("./mdds/include/mdds/global.hpp", "namespace mdds {", fix_cpp_version)

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

    fix_RVNG_H  = "explicit RVNGFileStream(const char *filename);\n"
    fix_RVNG_H += "	#if defined(_WIN32) || defined(_WIN64)\n"
    fix_RVNG_H += "	explicit RVNGFileStream(const wchar_t *filename);\n"
    fix_RVNG_H += "	#endif\n"

    base.replaceInFile("./librevenge/inc/librevenge-stream/RVNGStreamImplementation.h", "explicit RVNGFileStream(const char *filename);", fix_RVNG_H)
    
    fix_RVNG_CPP_include  = "#if defined(_WIN32) || defined(_WIN64)\n"
    fix_RVNG_CPP_include += "#include <sys/stat.h>\n\n"
    fix_RVNG_CPP_include += "static __inline int wstat(wchar_t const* const _FileName, struct stat* const _Stat)\n"
    fix_RVNG_CPP_include += "{\n"
    fix_RVNG_CPP_include += "	_STATIC_ASSERT(sizeof(struct stat) == sizeof(struct _stat64i32));\n";
    fix_RVNG_CPP_include += "	return _wstat64i32(_FileName, (struct _stat64i32*)_Stat);\n";
    fix_RVNG_CPP_include += "}\n"
    fix_RVNG_CPP_include += "#endif\n\n"
    fix_RVNG_CPP_include += "namespace librevenge"

    base.replaceInFile("./librevenge/src/lib/RVNGStreamImplementation.cpp", "namespace librevenge", fix_RVNG_CPP_include)

    fix_RVNG_CPP  = "#if defined(_WIN32) || defined(_WIN64)\n"
    fix_RVNG_CPP += "RVNGFileStream::RVNGFileStream(const wchar_t *filename) :\n"
    fix_RVNG_CPP += "	RVNGInputStream(),\n"
    fix_RVNG_CPP += "	d(new RVNGFileStreamPrivate())\n"
    fix_RVNG_CPP += "{\n"
    fix_RVNG_CPP += "	d->file = _wfopen(filename, L\"rb\");\n"
    fix_RVNG_CPP += "	if (!d->file || ferror(d->file))\n"
    fix_RVNG_CPP += "	{\n"
    fix_RVNG_CPP += "		delete d;\n"
    fix_RVNG_CPP += "		d = 0;\n"
    fix_RVNG_CPP += "		return;\n"
    fix_RVNG_CPP += "	}\n\n"
    fix_RVNG_CPP += "	struct stat status;\n"
    fix_RVNG_CPP += "	const int retval = wstat(filename, &status);\n"
    fix_RVNG_CPP += "	if ((0 != retval) || !S_ISREG(status.st_mode))\n"
    fix_RVNG_CPP += "	{\n"
    fix_RVNG_CPP += "		delete d;\n"
    fix_RVNG_CPP += "		d = 0;\n"
    fix_RVNG_CPP += "		return;\n"
    fix_RVNG_CPP += "	}\n\n"
    fix_RVNG_CPP += "	fseek(d->file, 0, SEEK_END);\n\n"
    fix_RVNG_CPP += "	d->streamSize = (unsigned long) ftell(d->file);\n"
    fix_RVNG_CPP += "	if (d->streamSize == (unsigned long)-1)\n"
    fix_RVNG_CPP += "		d->streamSize = 0;\n"
    fix_RVNG_CPP += "	if (d->streamSize > (std::numeric_limits<unsigned long>::max)() / 2)\n"
    fix_RVNG_CPP += "		d->streamSize = (std::numeric_limits<unsigned long>::max)() / 2;\n"
    fix_RVNG_CPP += "	fseek(d->file, 0, SEEK_SET);\n"
    fix_RVNG_CPP += "}\n"
    fix_RVNG_CPP += "#endif\n\n"
    fix_RVNG_CPP += "RVNGFileStream::~RVNGFileStream()"
    
    base.replaceInFile("./librevenge/src/lib/RVNGStreamImplementation.cpp", "RVNGFileStream::~RVNGFileStream()", fix_RVNG_CPP)

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



