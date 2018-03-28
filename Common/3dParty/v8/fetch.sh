#!/bin/bash

SCRIPT=$(readlink -f "$0" || grealpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

cd "$SCRIPTPATH"

if [ ! -d "depot_tools" ]
then
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
fi

export PATH=`pwd`/depot_tools:"$PATH"
gclient

if [ ! -d "./v8" ]
then
fetch v8
cd v8
git checkout -b 6.0 -t branch-heads/6.0
else
cd v8
fi

gclient sync

os=$(uname -s)
platform=""
case "$os" in
  Linux*)   platform="linux" ;;  
  *)        exit ;;
esac

cd "$SCRIPTPATH"

if [[ "$platform" == "linux" ]]
then
./fetch_linux_correct.sh
fi
