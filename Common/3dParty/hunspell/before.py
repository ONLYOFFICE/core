import sys
sys.path.append("../../../../build_tools/scripts")
import base
import os


def get_hunspell(stable_commit):
    repo_path = "https://github.com/hunspell/hunspell.git"
    base.cmd("git", ["clone", repo_path])
    os.chdir("hunspell")
    base.cmd("git", ["checkout", stable_commit])
    base.replaceInFile("./src/hunspell/filemgr.hxx", "FileMgr& operator=(const FileMgr&);", "FileMgr& operator=(const FileMgr&);\n" + "#ifdef HUNSPELL_WASM_MODULE\nstring_buffer_stream memin;\n#endif")  # custom filemgr support watch filemgr_wrapper_new.cxx
    base.replaceInFile("./src/hunspell/filemgr.hxx", "#include <fstream>", "#include <fstream>\n#ifdef HUNSPELL_WASM_MODULE\n#include \"string_buffer_stream.h\"\n#endif\n")
    base.replaceInFile("./src/hunspell/csutil.cxx", "void free_utf_tbl() {", "void free_utf_tbl() { \n return;\n")
    # bug fix, we need to keep this utf table
    # free_utf_tbl doesnt delete anything so we can destroy hunspell object
    os.chdir("../")


base.configure_common_apps()

# fetch hunspell
HEAD = open("HEAD", "r")
last_stable_commit = HEAD.read().split('\n')[0]  # workaround to delete \n in the end of the line
HEAD.close()

if not base.is_dir("hunspell"):
    get_hunspell(last_stable_commit)

# version check
git_head = open("hunspell/.git/HEAD", "r")
current_commit = git_head.read().split('\n')[0]
git_head.close()

if current_commit != last_stable_commit:
    base.delete_dir("hunspell")
    get_hunspell(last_stable_commit)
