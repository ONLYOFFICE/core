#!/bin/bash

SCRIPT=$(readlink -f "$0" || grealpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

ICU_MAJOR_VER=60
ICU_MINOR_VER=2

SHARED_LIB_VER=$ICU_MAJOR_VER.$ICU_MINOR_VER

os=$(uname -s)
platform=""

case "$os" in
  Linux*)
    platform="linux"
    BUILD_PLATFORM=Linux
    SHARED_LIB_EXT=.so.$SHARED_LIB_VER
    ;;
  Darwin*)
    platform="mac"
    BUILD_PLATFORM=MacOSX
    SHARED_LIB_EXT=.$SHARED_LIB_VER.dylib
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
  svn export http://source.icu-project.org/repos/icu/tags/release-$ICU_MAJOR_VER-$ICU_MINOR_VER/icu4c ./icu
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

cp "./icu/source/lib/libicudata$SHARED_LIB_EXT" "build/libicudata$SHARED_LIB_EXT"
cp "./icu/source/lib/libicuuc$SHARED_LIB_EXT" "build/libicuuc$SHARED_LIB_EXT"
