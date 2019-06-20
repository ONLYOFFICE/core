#!/bin/bash
set -e

SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

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
