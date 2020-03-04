#!/bin/bash
SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

os=$(uname -s)
platform=""

case "$os" in
  Linux*)   platform="linux" ;;
  Darwin*)  platform="mac" ;; 
  *)        exit ;;
esac

architecture=$(uname -m)
arch=""

case "$architecture" in
  x86_64*)  arch="_64" ;;
  *)        arch="_32" ;;
esac

outputdir="$SCRIPTPATH/boost_1_58_0/build/$platform$arch"
echo $outputdir

cd "$SCRIPTPATH/boost_1_58_0"

folder="build/$platform$arch"
if [ ! -d $folder ]; then
  ./bootstrap.sh --with-libraries=filesystem,system,date_time,regex

   stage="stage"
   if [ -d $stage ]; then
     rm -R $stage
   fi

  mkdir -p "$folder"
  mkdir -p "$folder/static"
  mkdir -p "$folder/static_fpic"
  mkdir -p "$folder/shared"

  ./b2 --clean
  ./bjam link=static
  cp stage/lib/* "$folder/static/"

  ./b2 --clean
  ./bjam link=static cxxflags=-fPIC
  cp stage/lib/* "$folder/static_fpic/"

  ./b2 --clean
  ./bjam link=shared
  cp stage/lib/* "$folder/shared/"
fi

if [[ "$OO_PLATFORM" == *"ios"* ]]
then 
  if [ ! -d "$SCRIPTPATH/boost_1_58_0/build/ios" ]; then
    cd "$SCRIPTPATH"
    ./boost_ios.sh
  fi
fi


