import sys
sys.path.append("../../../../../../build_tools/scripts")
import base
import os
import codecs


def apply_patch(file, patch):
    file_content = base.readFile(file)
    patch_content = base.readFile(patch)
    index1 = patch_content.find("<<<<<<<")
    index2 = patch_content.find("=======")
    index3 = patch_content.find(">>>>>>>")
    file_content_old = patch_content[index1 + 7:index2]
    file_content_new = "#if 0" + file_content_old + "#else" + patch_content[index2 + 7:index3] + "#endif"
    base.replaceInFile(file, file_content_old, file_content_new)
    return


def run_as_bash(file, commands):
    if base.is_file(file):
        base.delete_file(file)
    file_bash = codecs.open(file, "w", "utf-8")
    file_bash.write("#!/bin/bash\n")
    file_bash.write("\n".join(commands))
    file_bash.close()
    base.cmd("chmod", ["+x", file])
    base.cmd(file)
    return


if "windows" == base.host_platform():
    print("Windows system not supported. Please use Linux or MacOS")
    exit(0)

# fetch openssl
if not base.is_dir("openssl"):
    base.print_info("Fetching openssl...")
    base.cmd("git", ["clone",  "--depth=1", "--branch", "OpenSSL_1_1_1f", "https://github.com/openssl/openssl.git"])
    # correct for wasm builds
    apply_patch("./openssl/crypto/rand/rand_lib.c", "./patches/openssl1.patch")

if not base.is_dir("xml"):
    base.print_info("Copy xml...")
    base.copy_dir("./../../../../xml", "./xml")
    base.replaceInFile("./xml/libxml2/libxml.h", "xmlNop(void)", "xmlNop(void* context, char* buffer, int len)")
    base.replaceInFile("./xml/libxml2/xmlIO.c", "xmlNop(void)", "xmlNop(void* context, char* buffer, int len)")
    base.replaceInFile("./xml/src/xmllight_private.h", "#include \"../../common/", "#include \"../../../../../../common/")
    base.replaceInFile("./xml/include/xmlutils.h", "#include \"../../common/", "#include \"../../../../../../common/")

# compile openssl
if not base.is_file("./openssl/libcrypto.a"):  
    base.print_info("Compile openssl...")
    os.chdir("./openssl")
    #run_as_bash("./compile_openssl.sh", ["./config no-shared no-asm no-ssl2 no-ssl3", "source ./../emsdk/emsdk_env.sh", "export CC=emcc", "export CXX=emcc", "make"])
    run_as_bash("./compile_openssl.sh", ["source ./../emsdk/emsdk_env.sh", "emconfigure ./config no-shared no-asm no-threads", "sed -i 's|^CROSS_COMPILE.*$|CROSS_COMPILE=|g' Makefile", "emmake make build_generated libssl.a libcrypto.a"])
    os.chdir("../")
