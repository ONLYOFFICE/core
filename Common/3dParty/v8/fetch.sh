#!/bin/bash

SCRIPT=$(readlink -f "$0" || grealpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

cd "$SCRIPTPATH"

if [ ! -d "depot_tools" ]
then
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
fi

export PATH=`pwd`/depot_tools:"$PATH"

if [ ! -d "./v8" ]
then
fetch v8
fi

gclient sync -r 4.10.253
