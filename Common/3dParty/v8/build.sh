#!/bin/bash

export PATH=`pwd`/depot_tools:"$PATH"

SCRIPT=$(readlink -f "$0" || grealpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

os=$(uname -s)
platform=""

case "$os" in
  Linux*)   platform="linux" ;;
  Darwin*)  platform="mac" ;; 
  *)        exit ;;
esac

architecture=$(uname -m)
arch=""

case "$architecture" in
  x86_64*)  arch="_64" ;;
  *)        arch="_32" ;;
esac

if [[ -d "$SCRIPTPATH/$platform$arch" ]]
then
echo
else
mkdir "$SCRIPTPATH/$platform$arch"
fi

cd "$SCRIPTPATH/v8"

if [[ "$platform" == *"linux"* ]]
then
CFLAGS="-fPIC" CXXFLAGS="-fPIC" make native -j $(grep -c ^processor /proc/cpuinfo) GYPFLAGS=-Dclang=0

cp "./out/native/obj.target/tools/gyp/libv8_base.a" "./../$platform$arch/"
cp "./out/native/obj.target/tools/gyp/libv8_libbase.a" "./../$platform$arch/"
cp "./out/native/obj.target/tools/gyp/libv8_libplatform.a" "./../$platform$arch/"
cp "./out/native/obj.target/tools/gyp/libv8_nosnapshot.a" "./../$platform$arch/"
cp "./out/native/obj.target/tools/gyp/libv8_external_snapshot.a" "./../$platform$arch/"

cp "./out/native/obj.target/third_party/icu/libicui18n.a" "./../$platform$arch/"
cp "./out/native/obj.target/third_party/icu/libicuuc.a" "./../$platform$arch/"
cp "./out/native/obj.target/third_party/icu/libicudata.a" "./../$platform$arch/"

cp "./third_party/icu/source/data/in/icudtl.dat" "./../$platform$arch/"
fi

if [[ "$platform" == *"mac"* ]]
then
CFLAGS="-fPIC" CXXFLAGS="-fPIC -stdlib=libc++" LDFLAGS="-stdlib=libc++"  make native -j $(sysctl -n hw.ncpu)

cp "./out/native/obj.target/tools/gyp/libv8_base.a" "./../$platform$arch/"
cp "./out/native/obj.target/tools/gyp/libv8_libbase.a" "./../$platform$arch/"
cp "./out/native/obj.target/tools/gyp/libv8_libplatform.a" "./../$platform$arch/"
cp "./out/native/obj.target/tools/gyp/libv8_nosnapshot.a" "./../$platform$arch/"
cp "./out/native/obj.target/tools/gyp/libv8_external_snapshot.a" "./../$platform$arch/"

cp "./out/native/obj.target/third_party/icu/libicui18n.a" "./../$platform$arch/"
cp "./out/native/obj.target/third_party/icu/libicuuc.a" "./../$platform$arch/"
cp "./out/native/obj.target/third_party/icu/libicudata.a" "./../$platform$arch/"

cp "./third_party/icu/source/data/in/icudtl.dat" "./../$platform$arch/"
fi
