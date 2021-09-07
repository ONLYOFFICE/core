#!/usr/bin/env python

import sys
sys.path.append('../../../../../../build_tools/scripts')
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

if ("windows" == base.host_platform()):
  print("Windows system not supported. Please use Linux or MacOS")
  exit(0)

base.configure_common_apps()
base_dir = os.getcwd()

# fetch emsdk
if not base.is_dir("emsdk"):
  base.print_info("Fetching emsdk...")  
  base.cmd("git", ["clone", "https://github.com/emscripten-core/emsdk.git"])
  os.chdir(base_dir + "/emsdk")
  base.cmd("./emsdk", ["install", "latest"])
  base.cmd("./emsdk", ["activate", "latest"])
  os.chdir(base_dir)

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
if not base.is_file(base_dir + "/openssl/libcrypto.a"):  
  base.print_info("Compile openssl...")
  os.chdir(base_dir + "/openssl")
  #run_as_bash("./compile_openssl.sh", ["./config no-shared no-asm no-ssl2 no-ssl3", "source ./../emsdk/emsdk_env.sh", "export CC=emcc", "export CXX=emcc", "make"])
  run_as_bash("./compile_openssl.sh", ["source ./../emsdk/emsdk_env.sh", "emconfigure ./config no-shared no-asm no-threads", "sed -i 's|^CROSS_COMPILE.*$|CROSS_COMPILE=|g' Makefile", "emmake make build_generated libssl.a libcrypto.a"])
  os.chdir(base_dir)

# compile wasm module with bindings
compiler_flags = ["-o openssl.js",
                  "-O3",
                  #"-fno-exceptions", because xml/src/xmldom.cpp:691:17
                  "-fno-rtti",
                  "-s WASM=1",
                  "-s ALLOW_MEMORY_GROWTH=1",
                  "-s FILESYSTEM=0",
                  "-s ENVIRONMENT='web'"]
                  #"-s LLD_REPORT_UNDEFINED"]

exported_functions = ["_malloc",
                      "_free",
                      "_XmlSignature_Malloc",
                      "_XmlSignature_Free",
                      "_XmlSignature_CreateCertificate",
                      "_XmlSignature_LoadCert",
                      "_XmlSignature_LoadKey",
                      "_XmlSignature_DestroyCertificate",
                      "_XmlSignature_CreateFile",
                      "_XmlSignature_DestroyFile",
                      "_XmlSignature_GetFileSize",
                      "_XmlSignature_GetFileRaw",
                      "_XmlSignature_Sign"]

# XML
compiler_flags.append("-DHAVE_VA_COPY -DLIBXML_READER_ENABLED -DLIBXML_PUSH_ENABLED -DLIBXML_HTML_ENABLED -DLIBXML_XPATH_ENABLED -DLIBXML_OUTPUT_ENABLED -DLIBXML_C14N_ENABLED -DLIBXML_SAX1_ENABLED -DLIBXML_TREE_ENABLED -DLIBXML_XPTR_ENABLED -DIN_LIBXML -DLIBXML_STATIC")

compiler_flags.append("-I./xml/libxml2/include -I./xml/libxml2/include/libxml -I./xml/build/qt")

input_xml_sources = ["./xml/build/qt/libxml2_all.c", "./xml/build/qt/libxml2_all2.c", "./xml/src/xmllight.cpp", "./xml/src/xmldom.cpp"]

# KERNEL
libKernel_src_parh = "../../../../common/"
input_kernel_sources = ["StringBuilder.cpp", "Base64.cpp", "Path.cpp", "File.cpp", "Directory.cpp"]

# ZIP
compiler_flags.append("-DBUILD_ZLIB_AS_SOURCES")

compiler_flags.append("-I../../../../../OfficeUtils/src/zlib-1.2.11/contrib/minizip -I../../../../../OfficeUtils/src/zlib-1.2.11")

libOfficeUtils_src_parh = "../../../../../OfficeUtils/src/"
input_officeutils_sources = ["OfficeUtils.cpp", "ZipBuffer.cpp", "ZipUtilsCP.cpp"]

libMinizip_src_parh = "../../../../../OfficeUtils/src/zlib-1.2.11/contrib/minizip/"
input_minizip_sources = ["ioapi.c", "miniunz.c", "minizip.c", "mztools.c", "unzip.c", "zip.c", "ioapibuf.c"]

libZlib_src_parh = "../../../../../OfficeUtils/src/zlib-1.2.11/"
input_zlib_sources = ["adler32.c", "crc32.c", "deflate.c", "infback.c", "inffast.c", "inflate.c", "inftrees.c", "trees.c", "zutil.c"]

# SIGN
libSign_src_parh = "../../../../xmlsec/src/src/"
input_sign_sources = ["XmlTransform.cpp", "XmlCertificate.cpp", "OOXMLSigner.cpp", "OOXMLVerifier.cpp", "XmlSigner_openssl.cpp"]

# OPENSSL
compiler_flags.append("-Iopenssl/include -Iopenssl -Iemsdk/node/14.15.5_64bit/include/node/openssl/archs/linux-x86_64/no-asm/include")

compiler_flags.append("-D__linux__ -D_LINUX")
# sources
sources = []
for item in input_xml_sources:
  sources.append(item)
for item in input_kernel_sources:
  sources.append(libKernel_src_parh + item)
for item in input_officeutils_sources:
  sources.append(libOfficeUtils_src_parh + item)
for item in input_minizip_sources:
  sources.append(libMinizip_src_parh + item)
for item in input_zlib_sources:
  sources.append(libZlib_src_parh + item)
for item in input_sign_sources:
  sources.append(libSign_src_parh + item)
sources.append("./openssl/libcrypto.a ./openssl/apps/openssl.c")
sources.append("../main.cpp")

# arguments
arguments = ""
for item in compiler_flags:
  arguments += (item + " ")

arguments += "-s EXPORTED_FUNCTIONS=\"["
for item in exported_functions:
  arguments += ("'" + item + "',")
arguments = arguments[:-1]
arguments += "]\" "

for item in sources:
  arguments += (item + " ")

run_as_bash("./compile_module.sh", ["source ./emsdk/emsdk_env.sh", "emcc " + arguments])

# finalize
base.replaceInFile("./openssl.js", "__ATPOSTRUN__=[];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];")
base.replaceInFile("./openssl.js", "__ATPOSTRUN__ = [];", "__ATPOSTRUN__=[function(){self.onEngineInit();}];")
base.replaceInFile("./openssl.js", "function getBinaryPromise()", "function getBinaryPromise2()")

openssl_js_content     = base.readFile("./openssl.js")
engine_base_js_content = base.readFile("./engine.js")
desktop_fetch_content  = base.readFile("./../../../../../Common/js/desktop_fetch.js")
string_utf8_content    = base.readFile("./../../../../../Common/js/string_utf8.js")
engine_js_content = engine_base_js_content.replace("//desktop_fetch", desktop_fetch_content)
engine_js_content = engine_js_content.replace("//string_utf8", string_utf8_content)
engine_js_content = engine_js_content.replace("//module", openssl_js_content)

if not base.is_dir("./deploy"):
  base.create_dir("./deploy")

# remove previous version
if base.is_file("./deploy/openssl.js"):
  base.delete_file("./deploy/openssl.js")
if base.is_file("./deploy/openssl.wasm"):
  base.delete_file("./deploy/openssl.wasm")

# write new version
base.writeFile("./deploy/openssl.js", engine_js_content)
base.copy_file("./index.html",   "./deploy/index.html")
base.copy_file("./code.js",      "./deploy/code.js")
base.copy_file("./openssl.wasm", "./deploy/openssl.wasm")

base.delete_file("./openssl.js")
base.delete_file("./openssl.wasm")