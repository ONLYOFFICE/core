#!/bin/bash

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

cd "$SCRIPTPATH/$platform$arch"

if [ -d "build" ]
then
echo ""
else
mkdir "build"
fi

if [[ "$platform" == *"linux"* ]]
then
if [[ -f "./icu.zip" ]]
then 
echo "icu already downloaded"
else
if [[ "$arch" == *"_64"* ]]
then
wget -O icu.zip http://download.icu-project.org/files/icu4c/55.1/icu4c-55_1-RHEL6-x64.tgz
else
wget -O icu.zip http://download.icu-project.org/files/icu4c/55.1/icu4c-55_1-RHEL6-i386.tgz
fi
fi
if [ -d "./icu" ]
then
echo "icu already extracted"
else
7z x -so "./icu.zip" | tar xf -
fi
cp "./usr/local/lib/libicudata.so.55.1" "build/libicudata.so.55"
cp "./usr/local/lib/libicuuc.so.55.1" "build/libicuuc.so.55"
fi

if [[ "$platform" == *"mac"* ]]
then
if [ -d "./icu" ]
then
echo "icu already extracted"
else
svn export http://source.icu-project.org/repos/icu/icu/tags/release-55-1 ./icu
fi
cd ./icu/source/
./runConfigureICU MacOSX
make
cd ../../
cp "./icu/source/lib/libicudata.55.1.dylib" "build/libicudata.55.1.dylib"
cp "./icu/source/lib/libicuuc.55.1.dylib" "build/libicuuc.55.1.dylib"
fi
