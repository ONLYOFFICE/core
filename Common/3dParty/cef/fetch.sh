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
cef_binary=cef_binary
cef_arch=$cef_binary.7z
cef_url=http://d2ettrnqo7v976.cloudfront.net/cef/3163/$platform$arch/$cef_arch

if [[ "$platform" == *"linux"* ]]
then

  if [[ -f $cef_arch ]]
  then
    cef_mod_time=$(date -d"$(curl -sI $cef_url | awk '/Last-Modified/ {print ($3, $4, $5, $6, $7, $8)}')" +"%s")
    local_mod_time=$(stat -c %Y $cef_arch)
    if [[ $cef_mod_time -eq $local_mod_time ]]
    then
      echo "cef_binary already downloaded"
    else
      wget $cef_url -O $cef_arch
      rm -fr $cef_binary/
    fi
  else
    wget $cef_url
    rm -fr $cef_binary/
  fi

  if [ -d $cef_binary ]
  then
    echo "cef_binary already extracted"
  else
    7z x -y $cef_arch
  fi

  cp -r -t build/ ./$cef_binary/Release/*  ./$cef_binary/Resources/*
  chmod a+xr build/locales
fi
