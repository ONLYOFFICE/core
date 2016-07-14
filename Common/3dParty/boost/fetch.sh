#!/bin/bash

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

if [[ -f "$SCRIPTPATH/boost_1_58_0.7z" ]]
then
echo "boost already downloaded"
else
wget http://freefr.dl.sourceforge.net/project/boost/boost/1.58.0/boost_1_58_0.7z
fi

if [ -d "$SCRIPTPATH/boost" ]; then
echo "boost already extracted"
else
7z x "$SCRIPTPATH/boost_1_58_0.7z" -o"$SCRIPTPATH/"
fi
