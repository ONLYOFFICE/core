#!/bin/bash

SCRIPT=$(readlink -f "$0" || grealpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

cd "$SCRIPTPATH"/openssl

os=$(uname -s)
platform=""

case "$os" in
  Linux*)   platform="linux" ;;
  Darwin*)  platform="darwin64-x86_64-cc" ;; 
  *)        exit ;;
esac

platformLinux=""

architecture=$(uname -m)
arch=""

if [[ "$platform" == "linux" ]]
then 
case "$architecture" in
  x86_64*) arch="-64" ;;
  *) arch="-32" ;;
esac 
fi 

echo "$platform$arch"

if [ ! -f Makefile ]; then
  perl ./Configure $platform$arch
  ./config
fi

make
