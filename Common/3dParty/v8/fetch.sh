#!/bin/bash
SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

cd "$SCRIPTPATH"

if [ ! -d "depot_tools" ]
then
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
fi

export PATH=`pwd`/depot_tools:"$PATH"
gclient

#detect gcc version
currentver="$(gcc -dumpversion)"
requiredver="6.0.0"
v8_version="6.0"

if [ "$(printf '%s\n' "$requiredver" "$currentver" | sort -V | head -n1)" = "$requiredver" ]
then
v8_version="7.0"
fi

echo "v8 version: $v8_version"

if [ ! -d "./v8" ]
then
fetch v8
cd v8
git checkout -b $v8_version -t branch-heads/$v8_version
else
cd v8
fi

gclient sync --no-history

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
