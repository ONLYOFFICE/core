#!/bin/sh -f
# $Id: pv.sh 8104 2007-07-05 10:41:52Z till $
#
# pv - preview a specified page of a dvi file in a Ghostscript window
# usage: pv page file
#
# pv converts the given page to PostScript and displays it
# in a Ghostscript window.
#
if [ $# -lt 2 ] ;then
  echo usage: $0 'page_number file_name[.dvi]'
  exit 1
fi
#
# The following line used to appear here:
#
#RESOLUTION=100
#
# But according to Peter Dyballa
# <pete@lovelace.informatik.uni-frankfurt.de>, "Modern versions of dvips are
# taught to read configuration files which tell them the paths to PK, TFM,
# VF and other files for example PostScript font programmes. These files
# tell #dvips too which default resolution is used and therefore which
# series of PK files (based on 300 DPI or 400 DPI or 600 DPI or even more)
# are held on the system."  So we have deleted this line, and also removed
# the -D switch from the call of dvips below.
#

# This definition is changed on install to match the
# executable name set in the makefile
GS_EXECUTABLE=gs

TEMPDIR=.
PAGE=$1
shift
FILE="$1"
shift
if test -z "$TEMPDIR"; then
	TEMPDIR=/tmp
fi
if which mktemp >/dev/null 2>/dev/null; then
	tmpfile="`mktemp $TEMPDIR/\"$FILE\".pv.XXXXXX`"
else
	tmpfile="$TEMPDIR/$FILE.$$.pv"
fi
trap "rm -rf $tmpfile" 0 1 2 15
#dvips -D$RESOLUTION -p $PAGE -n 1 "$FILE" "$@" -o $tmpfile
dvips -p $PAGE -n 1 "$FILE" "$@" -o $tmpfile
$GS_EXECUTABLE $tmpfile
exit 0
