#!/usr/bin/env python

import sys
sys.path.append('../../../../../../build_tools/scripts')
import base
import os

def install_deps():
  if base.is_file("./.deps_complete"):
    return

  # packages
  packages = ["automake",
              "autopoint",
              "pkg-config",
              "flex",
              "bison",
              "lua5.2",
              "ant",
              "libssl-dev",
              "wget"]

  base.cmd("sudo", ["apt-get", "install", "-y"] + packages)

  # build-dep vlc packages
  # NOTE: this command needs enabled source packages downloading (can be enabled in `software-properties-gtk`)
  base.cmd("sudo", ["apt-get", "build-dep", "-y", "vlc"])

  # prepare directory for sources
  base.cmd("mkdir", ["-p", "src"])
  src_dir = os.getcwd() + "/src"
  
  # cmake
  os.chdir(src_dir)
  cmake_version = "3.16.3"
  cmake_version_short = "3.16"
  base.cmd("wget", ["-q", "https://cmake.org/files/v" + cmake_version_short + "/cmake-" + cmake_version + ".tar.gz"])
  base.cmd("tar", ["-xzf", "cmake-" + cmake_version + ".tar.gz"])
  os.chdir(src_dir + "/cmake-" + cmake_version)
  base.cmd("./bootstrap")
  base.cmd("make", ["-j$(nproc)"])
  base.cmd("sudo", ["make", "install"])

  # nasm
  os.chdir(src_dir)
  nasm_version = "2.14"
  base.cmd("wget", ["--no-check-certificate", "-q", "https://nasm.us/pub/nasm/releasebuilds/" + nasm_version + "/nasm-" + nasm_version + ".tar.gz"])
  base.cmd("tar", ["-xzf", "nasm-" + nasm_version + ".tar.gz"])
  os.chdir(src_dir + "/nasm-" + nasm_version)
  base.cmd("./autogen.sh")
  base.cmd("./configure")
  base.cmd("make", ["-j$(nproc)"])
  base.cmd("sudo", ["make", "install"])

  # python 3.7
  os.chdir(src_dir)
  python_version = "3.7.9"
  base.cmd("wget", ["-q", "https://python.org/ftp/python/" + python_version + "/Python-" + python_version + ".tgz"])
  base.cmd("tar", ["-xzf", "Python-" + python_version + ".tgz"])
  os.chdir(src_dir + "/Python-" + python_version)
  base.cmd("./configure", ["--enable-optimizations"])
  base.cmd("sudo", ["make", "-j$(nproc)", "altinstall"])

  # meson & ninja
  base.cmd("sudo", ["pip3.7", "install", "meson", "ninja"])

  # protobuf
  os.chdir(src_dir)
  protobuf_version = "3.4.1"
  base.cmd("wget", ["-q", "https://github.com/google/protobuf/releases/download/v" + protobuf_version + "/protobuf-cpp-" + protobuf_version + ".tar.gz"])
  base.cmd("tar", ["-xzf", "protobuf-cpp-" + protobuf_version + ".tar.gz"])
  os.chdir(src_dir + "/protobuf-" + protobuf_version)
  base.cmd("mkdir", ["-p", "build"])
  base.run_command("sed -i.orig \'s,#ifdef _MSC_VER,#if 1,\' \"src/google/protobuf/repeated_field.h\"")
  base.cmd("cmake", ["-S", "cmake", "-B", "build", "-DBUILD_SHARED_LIBS=OFF", "-Dprotobuf_BUILD_TESTS=OFF", "-Dprotobuf_BUILD_EXAMPLES=OFF"])
  base.cmd("cmake", ["--build", "build", "--parallel", "$(nproc)"])
  base.cmd("sudo", ["cmake", "--install", "build"])

  # TODO: remove tarballs ?
  base.writeFile("./.deps_complete", "complete")
  return

if __name__ == "__main__":
  install_deps()
