#!/bin/bash
SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

ICU_MAJOR_VER=58
ICU_MINOR_VER=2

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

if [[ -d "$SCRIPTPATH/$platform$arch" ]]
then
echo
else
mkdir "$SCRIPTPATH/$platform$arch"
fi

cd "$SCRIPTPATH/$platform$arch"

if [ -d "build" ]
then
 echo ""
else
 mkdir "build"
fi

if [ -d "./icu" ]
then
  echo "icu already exported"
else
  svn export https://github.com/unicode-org/icu/tags/release-$ICU_MAJOR_VER-$ICU_MINOR_VER/icu4c ./icu
fi

if [[ "$platform" == *"linux"* ]]
then
  # Workaround for building icu older than 60.0
  # on systems without xlocale.h (removed from glibc since 2.26)
  # See https://sourceware.org/glibc/wiki/Release/2.26#Removal_of_.27xlocale.h.27
  # See https://bugs.archlinux.org/task/55246
  sed -i 's/xlocale/locale/' ./icu/source/i18n/digitlst.cpp
fi

if [[ "$platform" == *"mac"* ]]
then
  sed -i -e 's/cmd\, \"%s %s -o %s%s %s %s%s %s %s\"\,/cmd\, \"%s %s -o %s%s %s %s %s %s %s\"\,/' ./icu/source/tools/pkgdata/pkgdata.cpp
fi

cd ./icu/source/

if [ ! -f "./Makefile" ]
then
 ./runConfigureICU $BUILD_PLATFORM
fi

make

if [ ! -d "$SCRIPTPATH/$platform$arch/usr/local" ]
then
  DESTDIR="$SCRIPTPATH/$platform$arch" make install
fi

cd ../../

cp "./icu/source/lib/libicudata$SHARED_LIB_EXT" "build/libicudata$SHARED_LIB_SHORT_EXT"
cp "./icu/source/lib/libicuuc$SHARED_LIB_EXT" "build/libicuuc$SHARED_LIB_SHORT_EXT"
