#!/bin/bash
SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

export PATH=`pwd`/depot_tools:"$PATH"

if [ -d "$SCRIPTPATH/v8/third_party/binutils/Linux_x64/Release" ]; then
  rm -rf "$SCRIPTPATH/v8/third_party/binutils/Linux_x64/Release"
fi
if [ -d "$SCRIPTPATH/v8/third_party/binutils/Linux_ia32/Release" ]; then
  rm -rf "$SCRIPTPATH/v8/third_party/binutils/Linux_ia32/Release"
fi

cd "$SCRIPTPATH/v8"
gclient sync --no-history

if [ -d "$SCRIPTPATH/v8/third_party/binutils/Linux_x64/Release/bin" ]; then
cd "$SCRIPTPATH/v8/third_party/binutils/Linux_x64/Release/bin"
for file in *
do
 echo $file
 if [ $file != "ld.gold" ]
  then
   mv "$file" "old_${file}"
   ln -s /usr/bin/"$file" ./"$file"
 fi
done
fi

if [ -d "$SCRIPTPATH/v8/third_party/binutils/Linux_ia32/Release/bin" ]; then
cd "$SCRIPTPATH/v8/third_party/binutils/Linux_ia32/Release/bin"
for file in *
do
 echo $file
 if [ $file != "ld.gold" ]
  then
   mv "$file" "old_${file}"
   ln -s /usr/bin/"$file" ./"$file"
 fi
done
fi
