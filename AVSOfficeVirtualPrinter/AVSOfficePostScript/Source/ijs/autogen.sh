#!/bin/sh
# Run this to generate all the initial makefiles, etc.
# bits borrowed from all over

package="ijs"

olddir=`pwd`
srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

cd "$srcdir"
DIE=0

for CONFIG_AC in "configure.ac" "configure.in" nope; do
  test -r $CONFIG_AC && break
done
if test "x$CONFIG_AC" = xnope; then
  echo "No configure script found."
  exit 1
fi

echo "checking for autoconf..."
(autoconf --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "You must have autoconf installed to compile $package."
  echo "Download the appropriate package for your system,"    
  echo "or get the source from one of the GNU ftp sites"    
  echo "listed in http://www.gnu.org/order/ftp.html"
  DIE=1
}

VERSIONGREP="sed -e s/.*[^0-9\.]\([0-9]\.[0-9]\).*/\1/"

# do we need automake?
(grep "^AM_INIT_AUTOMAKE" "$CONFIG_AC" >/dev/null) && {
  AM_NEEDED=`fgrep AUTOMAKE_OPTIONS Makefile.am | $VERSIONGREP`
  AM_NMAJOR=`echo $AM_NEEDED |cut -d. -f1`
  AM_NMINOR=`echo $AM_NEEDED |cut -d. -f2`
  AM_NPATCHLEVEL=`echo $AM_NEEDED |cut -d. -f3`
  AM_NVERSION=`expr $AM_NMAJOR '*' 10000 + $AM_NMINOR '*' 100 + 0$AM_NPATCHLEVEL`
  if test -z $AM_NEEDED; then
    echo -n "checking for automake... "
    AUTOMAKE=automake
    ACLOCAL=aclocal
    if ($AUTOMAKE --version < /dev/null > /dev/null 2>&1); then
      echo "no"
      AUTOMAKE=
    else
      echo "yes"
    fi
  echo
  else
    echo -n "checking for automake $AM_NEEDED or later... "
    for am in automake-$AM_NEEDED automake$AM_NEEDED automake; do
      ($am --version < /dev/null > /dev/null 2>&1) || continue
      AM_MAJOR=`echo $AM_NEEDED |cut -d. -f1`
      AM_MINOR=`echo $AM_NEEDED |cut -d. -f2`
      AM_PATCHLEVEL=`echo $AM_NEEDED |cut -d. -f3`
      AM_VERSION=`expr $AM_NMAJOR '*' 10000 + $AM_NMINOR '*' 100 + 0$AM_NPATCHLEVEL`
      if test $AM_VERSION -ge $AM_NVERSION; then
        AUTOMAKE=$am
        echo $AUTOMAKE
        break
      fi
    done
    test -z $AUTOMAKE &&  echo "no"
    echo -n "checking for aclocal $AM_NEEDED or later... "
    for ac in aclocal-$AM_NEEDED aclocal$AM_NEEDED aclocal; do
      ($ac --version < /dev/null > /dev/null 2>&1) || continue
      AM_MAJOR=`echo $AM_NEEDED |cut -d. -f1`
      AM_MINOR=`echo $AM_NEEDED |cut -d. -f2`
      AM_PATCHLEVEL=`echo $AM_NEEDED |cut -d. -f3`
      AM_VERSION=`expr $AM_NMAJOR '*' 10000 + $AM_NMINOR '*' 100 + 0$AM_NPATCHLEVEL`
      if test $AM_VERSION -ge $AM_NVERSION; then
        ACLOCAL=$ac
        echo $ACLOCAL
        break
      fi
    done
  test -z $ACLOCAL && echo "no"
  fi
  test -z $AUTOMAKE || test -z $ACLOCAL && {
        echo
        echo "You must have automake installed to compile $package."
        echo "Download the appropriate package for your distribution,"
        echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/"
        exit 1
  }
}

(grep "^AM_PROG_LIBTOOL" "$CONFIG_AC" >/dev/null) && {
  echo -n "checking for libtoolize... "
  # libtoolize is shipped at glibtoolize on MacOS X
  for LIBTOOLIZE in libtoolize glibtoolize nope; do
    ($LIBTOOLIZE --version) < /dev/null > /dev/null 2>&1 && break
  done
  if test x$LIBTOOLIZE = xnope; then
    echo "nope."
    LIBTOOLIZE=libtoolize
  else
    echo $LIBTOOLIZE
  fi
  ($LIBTOOLIZE --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "You must have libtool installed to compile $package."
    echo "Download the appropriate package for your system,"
    echo "or get the source from one of the GNU ftp sites"
    echo "listed in http://www.gnu.org/order/ftp.html"
    DIE=1
  }
}

if test "$DIE" -eq 1; then
  exit 1
fi

echo "Creating configuration files for $package..."

echo "  $ACLOCAL $ACLOCAL_FLAGS"
$ACLOCAL $ACLOCAL_FLAGS

#echo "  autoheader"
#autoheader

echo "  $LIBTOOLIZE --automake"
$LIBTOOLIZE --automake

echo "  $AUTOMAKE --add-missing $AUTOMAKE_OPTIONS"
$AUTOMAKE --add-missing $AUTOMAKE_OPTIONS

echo "  autoconf"
autoconf

cd "$olddir"

conf_flags="--enable-maintainer-mode --enable-compile-warnings" #--enable-iso-c

echo Running $srcdir/configure $conf_flags "$@" ...
$srcdir/configure $conf_flags "$@" \
  && echo Now type \`make\' to compile $package || exit 1

