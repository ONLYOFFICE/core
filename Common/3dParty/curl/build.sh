SCRIPT=$(readlink -f "$0" || grealpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

os=$(uname -s)
platform=""

case "$os" in
  Linux*)
    platform="linux"
    BUILD_PLATFORM=Linux
    ;;
  Darwin*)
    platform="mac"
    BUILD_PLATFORM=MacOSX
    ;;
  *)        exit ;;
esac


architecture=$(uname -m)
arch=""

case "$architecture" in
  x86_64*)  arch="_64" ;;
  *)        arch="_32" ;;
esac

if [[ -d "$SCRIPTPATH/$platform$arch" ]]
then
  echo
else
  mkdir "$SCRIPTPATH/$platform$arch"
fi

CURL_FOLDER=curl
cd ${CURL_FOLDER}

if [[ ! -f configure ]] 
then
  ./buildconf
fi

if [[ ! -f Makefile ]]
then 
  ./configure \
    --disable-shared \
    --enable-static \
    --disable-ldap \
    --disable-sspi \
    --without-librtmp \
    --disable-ftp \
    --disable-file \
    --disable-dict \
    --disable-telnet \
    --disable-tftp \
    --disable-rtsp \
    --disable-pop3 \
    --disable-imap \
    --disable-smtp \
    --disable-gopher \
    --disable-smb \
    --without-libidn
fi

make

if [ ! -d "$SCRIPTPATH/$platform$arch/build" ]
then
  DESTDIR="$SCRIPTPATH/$platform$arch" make install
  mkdir -p "$SCRIPTPATH/$platform$arch/build"  
  cp "$SCRIPTPATH/$platform$arch/usr/local/lib/libcurl.a" "$SCRIPTPATH/$platform$arch/build/"  
fi

