#!/bin/bash
SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

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
  #perl ./Configure $platform$arch
  ./config no-shared no-asm
fi

make build_libs
