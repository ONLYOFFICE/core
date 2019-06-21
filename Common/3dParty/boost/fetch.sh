#!/bin/bash
SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

os=$(uname -s)
platform=""

case "$os" in
  Linux*)   platform="linux" ;;
  Darwin*)  platform="mac" ;; 
  *)        exit ;;
esac

if [[ -f "$SCRIPTPATH/boost_1_58_0.7z" ]]
then
echo "boost already downloaded"
else
wget http://freefr.dl.sourceforge.net/project/boost/boost/1.58.0/boost_1_58_0.7z || curl -O http://freefr.dl.sourceforge.net/project/boost/boost/1.58.0/boost_1_58_0.7z
fi

if [ -d "$SCRIPTPATH/boost_1_58_0" ]; then
echo "boost already extracted"
else
if [[ "$platform" == *"linux"* ]]
then
7z x -y "$SCRIPTPATH/boost_1_58_0.7z" -o"$SCRIPTPATH/"
else
7za x "$SCRIPTPATH/boost_1_58_0.7z" -o"$SCRIPTPATH/"
fi
fi
