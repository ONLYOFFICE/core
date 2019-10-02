#!/bin/bash
SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

ICU_MAJOR_VER=62
ICU_MINOR_VER=1

SHARED_LIB_VER=$ICU_MAJOR_VER.$ICU_MINOR_VER

os=$(uname -s)
platform=""

case "$os" in
  Linux*)
    platform="linux"
    BUILD_PLATFORM=Linux
    SHARED_LIB_EXT=.so.$SHARED_LIB_VER
    SHARED_LIB_SHORT_EXT=.so.$ICU_MAJOR_VER
    ;;
  Darwin*)
    platform="mac"
    BUILD_PLATFORM=MacOSX
    SHARED_LIB_EXT=.$SHARED_LIB_VER.dylib
    SHARED_LIB_SHORT_EXT=.$ICU_MAJOR_VER.dylib
    ;;
  *)        exit ;;
esac


architecture=$(uname -m)
arch=""

case "$architecture" in
  x86_64*)  arch="_64" ;;
  *)        arch="_32" ;;
esac

rm -rf "$SCRIPTPATH/$platform$arch"

if [[ -d "$SCRIPTPATH/$platform$arch" ]]
then
echo
else
mkdir "$SCRIPTPATH/$platform$arch"
fi

cd "$SCRIPTPATH/$platform$arch"

svn export https://github.com/unicode-org/icu/tags/release-$ICU_MAJOR_VER-$ICU_MINOR_VER/icu4c ./icu
