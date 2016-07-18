#!/bin/bash

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

cd "$SCRIPTPATH"

if [ -d "depot_tools" ]
then
echo ""
else
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
fi

export PATH=`pwd`/depot_tools:"$PATH"
gclient

if [ -d "./v8" ]
then
gclient sync
else
fetch v8
fi
