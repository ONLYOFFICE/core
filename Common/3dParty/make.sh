#!/bin/bash

SCRIPT=$(readlink -f "$0" || grealpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

cd "$SCRIPTPATH"

for d in */ ; do

echo "$d"
cd "$SCRIPTPATH/$d"

if [ -f "fetch.sh" ]
then 
bash "fetch.sh"
fi
if [ -f "build.sh" ]
then 
bash "build.sh"
fi

cd "$SCRIPTPATH"
done
