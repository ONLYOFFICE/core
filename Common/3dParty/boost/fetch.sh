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

if [[ "$platform" == *"mac"* ]]
then
if [[ -f "$SCRIPTPATH/7zX_1.7.1.dmg" ]]
then
echo "7z already downloaded"
else
wget http://static.updatestar.net/dl/7zX/7zX_1.7.1.dmg
fi
fi

if [[ -f "$SCRIPTPATH/boost_1_58_0.7z" ]]
then
echo "boost already downloaded"
else
wget http://freefr.dl.sourceforge.net/project/boost/boost/1.58.0/boost_1_58_0.7z
fi

if [ -d "$SCRIPTPATH/boost_1_58_0" ]; then
echo "boost already extracted"
else
if [[ "$platform" == *"linux"* ]]
then
7z x -y "$SCRIPTPATH/boost_1_58_0.7z" -o"$SCRIPTPATH/"
else
hdiutil mount "$SCRIPTPATH/7zX_1.7.1.dmg"
/Volumes/7zX/7zX.app/Contents/Resources/7za x "$SCRIPTPATH/boost_1_58_0.7z" -o"$SCRIPTPATH/"
hdiutil unmount /Volumes/7zX
fi
fi
