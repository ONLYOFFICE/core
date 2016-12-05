#!/bin/bash

SCRIPT=$(readlink -f "$0")
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
if [[ -f "cef_binary.7z" ]]
then 
echo "cef_binary already downloaded"
else
wget http://d2ettrnqo7v976.cloudfront.net/cef/2454/$platform$arch/cef_binary.7z
fi
if [ -d cef_binary ]
then
echo "cef_binary already extracted"
else

7z x -y cef_binary.7z
fi
cp -r -t build/ ./cef_binary/Release/*  ./cef_binary/Resources/*
chmod a+xr build/locales
fi
