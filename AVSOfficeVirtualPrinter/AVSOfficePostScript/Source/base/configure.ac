dnl  Copyright (C) 2001-2007 Artifex Software, Inc.
dnl  All Rights Reserved.

dnl  This software is provided AS-IS with no warranty, either express or
dnl  implied.

dnl  This software is distributed under license and may not be copied, modified
dnl  or distributed except as expressly authorized under the terms of that
dnl  license.  Refer to licensing information at http://www.artifex.com/
dnl  or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
dnl  San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.

dnl $Id: configure.ac 9394 2009-01-25 06:59:45Z giles $

dnl Process this file with autoconf to produce a configure script

dnl ------------------------------------------------
dnl Initialization and Versioning
dnl ------------------------------------------------

AC_INIT
AC_PREREQ(2.52)
AC_CONFIG_SRCDIR(psi/gs.c)

dnl Inherit compiler flags from the environment...
CFLAGS="${CFLAGS:=}"
CPPFLAGS="${CPPFLAGS:=}"
CXXFLAGS="${CXXFLAGS:=}"
LDFLAGS="${LDFLAGS:=}"

dnl --------------------------------------------------
dnl Local utilities
dnl --------------------------------------------------

dnl GS_SPLIT_LIBS( LIBS, LINKLINE )
dnl Split a unix-style link line into a list of 
dnl bare library names. For example, the line
dnl '-L/usr/X11R6/lib -lX11 -lXt' splits into
dnl LIB='X11 Xt'
dnl
AC_DEFUN([GS_SPLIT_LIBS], [
# the makefile wants a list of just the library names 
for gs_item in $2; do
  gs_stripped_item=`echo "$gs_item" | sed -e 's/^-l//'`
  if test "x$gs_stripped_item" != "x$gs_item"; then
    $1="$[$1] $gs_stripped_item"
  fi
done
])

dnl GS_SPLIT_LIBPATHS( LIBPATHS, LINKLINE )
dnl Split a unix-style link line into a list of
dnl bare search path entries. For example,
dnl '-L/usr/X11R6/lib -lX11 -L/opt/lib -lXt'
dnl splits to LIBPATHS='/usr/X11R6/lib /opt/lib'
dnl
AC_DEFUN([GS_SPLIT_LIBPATHS], [
for gs_item in $2; do
  gs_stripped_item=`echo "$gs_item" | sed -e 's/-L//'`
  if test "x$gs_stripped_item" != "x$gs_item"; then
    $1="$[$1] $gs_stripped_item"
  fi
done
])

dnl --------------------------------------------------  
dnl Check for programs
dnl --------------------------------------------------  

dnl AC_PROG_CC likes to add '-g -O2' to CFLAGS. however,
dnl we ignore those flags and construct our own.
save_cflags=$CFLAGS
AC_PROG_CC
AC_PROG_CPP
CFLAGS=$save_cflags

AC_PROG_RANLIB
#AC_PROG_INSTALL

dnl --------------------------------------------------  
dnl Allow excluding the contributed drivers
dnl --------------------------------------------------  

AC_ARG_ENABLE([contrib], AC_HELP_STRING([--disable-contrib],
    [Do not include contributed drivers]))
CONTRIBINCLUDE="include contrib/contrib.mak"
INSTALL_CONTRIB="install-contrib-extras"
if test x$enable_contrib = xno; then
    CONTRIBINCLUDE=""
    INSTALL_CONTRIB=""
    CFLAGS="$CFLAGS -DNOCONTRIB"
fi
AC_SUBST(CONTRIBINCLUDE)
AC_SUBST(INSTALL_CONTRIB)

dnl --------------------------------------------------
dnl Set build flags based on environment
dnl --------------------------------------------------

#AC_CANONICAL_HOST

if test $ac_cv_prog_gcc = yes; then
    cflags_to_try="-Wall -Wstrict-prototypes -Wundef \
-Wmissing-declarations -Wmissing-prototypes -Wwrite-strings \
-Wno-strict-aliasing -Wdeclaration-after-statement \
-fno-builtin -fno-common"
    optflags_to_try="-O2"
else
    cflags_to_try=
    optflags_to_try="-O"
fi

AC_ARG_ENABLE([debug], AC_HELP_STRING([--enable-debug],
    [turn on debugging]))
if test x$enable_debug = xyes; then
    optflags_to_try="-g"
    CFLAGS="-DDEBUG $CFLAGS"
fi

AC_MSG_CHECKING([supported compiler flags])
old_cflags=$CFLAGS
echo
for flag in $optflags_to_try; do
    CFLAGS="$CFLAGS $flag"
    AC_TRY_COMPILE(, [return 0;], [
	echo "   $flag"
	OPT_CFLAGS="$OPT_CFLAGS $flag"
    ])
    CFLAGS=$old_cflags
done
for flag in $cflags_to_try; do
	CFLAGS="$CFLAGS $flag"
	AC_TRY_COMPILE(, [return 0;], [
		echo "   $flag"
		GCFLAGS="$GCFLAGS $flag"
	])
	CFLAGS=$old_cflags
done
AC_MSG_RESULT([ ...done.])

dnl --------------------------------------------------
dnl Check for headers
dnl --------------------------------------------------

AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([errno.h fcntl.h limits.h malloc.h memory.h stdlib.h string.h strings.h sys/ioctl.h sys/param.h sys/time.h syslog.h unistd.h])

# for gdev3b1.c (AT&T terminal interface)
AC_CHECK_HEADER([sys/window.h])

dnl --------------------------------------------------
dnl Check for typedefs, structures, etc
dnl --------------------------------------------------

AC_C_CONST
AC_C_INLINE
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_STRUCT_ST_BLOCKS
AC_HEADER_TIME
AC_STRUCT_TM

dnl see if we're on a system that puts the *int*_t types 
dnl from stdint.h in sys/types.h
if test "x$ac_cv_header_stdint_h" != xyes; then
    AC_CHECK_TYPES([int8_t, int16_t, int32_t, uint8_t, uint16_t, uint32_t],,,[#include <sys/types.h>])
    if test "$ac_cv_type_uint8_t" = yes; then
        AC_DEFINE([SYS_TYPES_HAS_STDINT_TYPES])
	GCFLAGS="$GCFLAGS -DSYS_TYPES_HAS_STDINT_TYPES"
    fi
fi

dnl we aren't interested in all of DEFS, so manually insert
dnl the flags we care about
if test "$ac_cv_c_const" != yes; then
	GCFLAGS="$GCFLAGS -Dconst="
fi
if test "x$ac_cv_header_stdint_h" = xyes; then
	GCFLAGS="$GCFLAGS -DHAVE_STDINT_H"
fi

dnl try to find a 64 bit type for devicen color index
uint64_type="none"
 AC_CHECK_SIZEOF(unsigned long int)
 if test $ac_cv_sizeof_unsigned_long_int = 8; then
	uint64_type="unsigned long int"
 else
  AC_CHECK_SIZEOF(unsigned long long)
  if test $ac_cv_sizeof_unsigned_long_long = 8; then
	uint64_type="unsigned long long"
  else
   AC_CHECK_SIZEOF(unsigned __int64)
   if test $ac_cv_sizeof_unsigned___int64 = 8; then
	uint64_type="unsigned __int64"
   else
    AC_CHECK_SIZEOF(u_int64_t)
    if test $ac_cv_sizeof_u_int64_t = 8; then
	uint64_type="u_int64_t"
    fi
   fi
  fi
 fi
dnl we don't need to do anything if a 64-bit type wasn't found
dnl the code falls back to a (probably 32-bit) default
if test "$uint64_type" != "none"; then
	GCFLAGS="$GCFLAGS -DGX_COLOR_INDEX_TYPE=\"$uint64_type\""
fi

dnl pkg-config is used for several tests now...
AC_PATH_PROG(PKGCONFIG, pkg-config)

dnl Fontconfig support
HAVE_FONTCONFIG=""
FONTCONFIG_CFLAGS=""
FONTCONFIG_LIBS=""
AC_ARG_ENABLE([fontconfig], AC_HELP_STRING([--disable-fontconfig],
    [Don't use fontconfig to list system fonts]))
if test "$enable_fontconfig" != "no"; then
	# We MUST NOT use PKG_CHECK_MODULES since it is a) not a standard
	# autoconf macro and b) requires pkg-config on the system, which is
	# NOT standard on ANY OS, including Linux!
	if test "x$PKGCONFIG" != x; then
		AC_MSG_CHECKING(for fontconfig with pkg-config)
		if $PKGCONFIG --exists fontconfig; then
			AC_MSG_RESULT(yes)
			FONTCONFIG_CFLAGS="$CFLAGS `$PKGCONFIG --cflags fontconfig`"
			FONTCONFIG_LIBS="`$PKGCONFIG --libs fontconfig`"
			HAVE_FONTCONFIG=-DHAVE_FONTCONFIG
		else
			AC_MSG_RESULT(no)
		fi
	fi
	if test -z "$HAVE_FONTCONFIG"; then
		AC_CHECK_LIB([fontconfig], [FcInitLoadConfigAndFonts], [
		  AC_CHECK_HEADER([fontconfig/fontconfig.h], [
		    FONTCONFIG_LIBS="-lfontconfig"
		    HAVE_FONTCONFIG="-DHAVE_FONTCONFIG"
		  ])
		])
	fi
fi
AC_SUBST(HAVE_FONTCONFIG)
AC_SUBST(FONTCONFIG_CFLAGS)
AC_SUBST(FONTCONFIG_LIBS)

dnl --------------------------------------------------
dnl Check for libraries
dnl --------------------------------------------------

AC_CHECK_LIB(m, cos)

SYNC="nosync"
PTHREAD_LIBS=""
AC_CHECK_LIB(pthread, pthread_create, [
  SYNC=posync;
  PTHREAD_LIBS="-lpthread"
])
AC_SUBST(SYNC)
AC_SUBST(PTHREAD_LIBS)

dnl Tests for iconv (Needed for OpenPrinting Vector, "opvp" output device)
AC_ARG_WITH(libiconv,
            [AC_HELP_STRING([--with-libiconv=@<:@no/gnu/native@:>@],
                            [use the libiconv library])],,
	    [with_libiconv=maybe])
found_iconv=no
case $with_libiconv in
  maybe)
    # Check in the C library first
    AC_CHECK_FUNC(iconv_open, [with_libiconv=no; found_iconv=yes])
    # Check if we have GNU libiconv
    if test $found_iconv = "no"; then
      AC_CHECK_LIB(iconv, libiconv_open, [with_libiconv=gnu; found_iconv=yes])
    fi
    # Check if we have a iconv in -liconv, possibly from vendor
    if test $found_iconv = "no"; then
      AC_CHECK_LIB(iconv, iconv_open, [with_libiconv=native; found_iconv=yes])
    fi
    ;;
  no)
    AC_CHECK_FUNC(iconv_open, [with_libiconv=no; found_iconv=yes])
    ;;
  gnu|yes)
    AC_CHECK_LIB(iconv, libiconv_open, [with_libiconv=gnu; found_iconv=yes])
    ;;
  native)
    AC_CHECK_LIB(iconv, iconv_open, [with_libiconv=native; found_iconv=yes])
    ;;
esac
if test x$found_iconv != xno -a x$with_libiconv != xno ; then
  LIBS="$LIBS -liconv"
fi

case $with_libiconv in
  gnu)
    AC_DEFINE(USE_LIBICONV_GNU, 1, [Using GNU libiconv])
    CFLAGS="$CFLAGS -DUSE_LIBICONV_GNU"
    ;;
  native)
    AC_DEFINE(USE_LIBICONV_NATIVE, 1, [Using a native implementation of iconv in a separate library])
    ;;
esac

dnl Tests for libpaper (to determine system default paper size)
AC_ARG_WITH([libpaper],
            AC_HELP_STRING([--without-libpaper],
                           [disable libpaper support]))
if test x$with_libpaper != xno; then
    AC_CHECK_LIB(paper, systempapername, [with_libpaper=yes],
    [
	AC_MSG_WARN([disabling support for libpaper])
        with_libpaper=no
    ])
fi
if test x$with_libpaper != xno; then
    LIBS="$LIBS -lpaper"
    AC_DEFINE(USE_LIBPAPER, 1, [Using libpaper])
    CFLAGS="$CFLAGS -DUSE_LIBPAPER"
fi

AC_CHECK_LIB(dl, dlopen)

AC_MSG_CHECKING([for local jpeg library source])
dnl At present, we give the local source priority over the shared
dnl build, so that the D_MAX_BLOCKS_IN_MCU patch will be applied.
dnl A more sophisticated approach would be to test the shared lib
dnl to see whether it has already been patched.
LIBJPEGDIR=src
if test -f jpeg/jpeglib.h; then
	AC_MSG_RESULT([jpeg])
	SHARE_LIBJPEG=0
	LIBJPEGDIR=jpeg
elif test -f jpeg-6b/jpeglib.h; then
	AC_MSG_RESULT([jpeg-6b])
	SHARE_LIBJPEG=0
	LIBJPEGDIR=jpeg-6b
else
	AC_MSG_RESULT([no])
	AC_CHECK_LIB(jpeg, jpeg_set_defaults, [
	  AC_CHECK_HEADERS([jpeglib.h], [SHARE_LIBJPEG=1])
	])
fi
if test -z "$SHARE_LIBJPEG"; then
  AC_MSG_ERROR([I wasn't able to find a copy
  of the jpeg library. This is required for compiling
  ghostscript. Please download a copy of the source,
  e.g. from http://www.ijg.org/, unpack it at the
  top level of the gs source tree, and rename
  the directory to 'jpeg'.
  ])
fi
AC_SUBST(SHARE_LIBJPEG)
AC_SUBST(LIBJPEGDIR)
dnl check for the internal jpeg memory header
AC_MSG_CHECKING([for jmemsys.h])
if test -r $LIBJPEGDIR/jmemsys.h; then
  AC_MSG_RESULT([yes])
else
  AC_MSG_RESULT([no])
  AC_DEFINE([DONT_HAVE_JMEMSYS_H], 1, 
    [define if the libjpeg memory system prototypes aren't available])
fi

dnl these are technically optional

AC_MSG_CHECKING([for local zlib source])
dnl zlib is needed for language level 3, and libpng
# we must define ZLIBDIR regardless because libpng.mak does a -I$(ZLIBDIR)
# this seems a harmless default
ZLIBDIR=src
if test -d zlib; then
	AC_MSG_RESULT([yes])
	SHARE_ZLIB=0
	ZLIBDIR=zlib
else
	AC_MSG_RESULT([no])
	AC_CHECK_LIB(z, deflate, [
	  AC_CHECK_HEADERS(zlib.h, [SHARE_ZLIB=1])
	])
fi
if test -z "$SHARE_ZLIB"; then
  AC_MSG_ERROR([I did not find a copy of zlib on your system.
  Please either install it, or unpack a copy of the source in a
  local directory named 'zlib'. See http://www.gzip.org/zlib/
  for more information.
  ])
fi
AC_SUBST(SHARE_ZLIB)
AC_SUBST(ZLIBDIR)

dnl png for the png output device; it also requires zlib
LIBPNGDIR=src
PNGDEVS=''
PNGDEVS_ALL='$(DD)png48.dev $(DD)png16m.dev $(DD)pnggray.dev $(DD)pngmono.dev $(DD)png256.dev $(DD)png16.dev $(DD)pngalpha.dev'
AC_MSG_CHECKING([for local png library source])
if test -f libpng/pngread.c; then
	AC_MSG_RESULT([yes])
	SHARE_LIBPNG=0
	LIBPNGDIR=libpng
	PNGDEVS="$PNGDEVS_ALL"
else
	AC_MSG_RESULT([no])
	AC_CHECK_LIB(png, png_check_sig, [
	  AC_CHECK_HEADERS(png.h, [
		SHARE_LIBPNG=1
		PNGDEVS="$PNGDEVS_ALL"
	  ], [SHARE_LIBPNG=0])
	], [SHARE_LIBPNG=0], [-lz])
fi
if test -z "$PNGDEVS"; then
  AC_MSG_NOTICE([disabling png output devices])
fi
AC_SUBST(SHARE_LIBPNG)
AC_SUBST(LIBPNGDIR)
AC_SUBST(PNGDEVS)

dnl look for CUPS...
AC_ARG_ENABLE([cups], AC_HELP_STRING([--disable-cups],
    [Don't include CUPS support]))

CUPSDEV=""
CUPSINCLUDE=""
CUPSCFLAGS=""
CUPSLIBS=""
CUPSLIBDIRS=""
CUPSCONFIG="${CUPSCONFIG:=}"
CUPSSERVERBIN=""
CUPSSERVERROOT=""
CUPSDATA=""

if ( test -d cups ); then
    if test x$enable_cups != xno; then
	AC_PATH_PROG(CUPSCONFIG,cups-config)
	dnl AC_CHECK_HEADER([cups/raster.h],[],[CUPSCONFIG=""])
	if test "x$CUPSCONFIG" != x; then
	    dnl Use values from CUPS config...
	    CUPSCFLAGS="`$CUPSCONFIG --cflags` $CFLAGS"
#	    CUPSLINK="`$CUPSCONFIG --ldflags` `$CUPSCONFIG --static --image --libs | sed -e '1,$s/-lssl//'` $LIBS"
	    CUPSLINK="`$CUPSCONFIG --ldflags` `$CUPSCONFIG --image --libs`"
	    GS_SPLIT_LIBS([CUPSLIBS], [$CUPSLINK])
	    GS_SPLIT_LIBPATHS([CUPSLIBDIRS],[$CUPSLINK])
	    CUPSSERVERROOT="`$CUPSCONFIG --serverroot`"
	    CUPSSERVERBIN="`$CUPSCONFIG --serverbin`"
	    CUPSDATA="`$CUPSCONFIG --datadir`"
	    CUPSINCLUDE="include cups/cups.mak"
	    CUPSDEV="\$(DD)cups.dev"
	fi
    fi
fi

AC_SUBST(CUPSDEV)
AC_SUBST(CUPSCFLAGS)
AC_SUBST(CUPSLIBS)
AC_SUBST(CUPSLIBDIRS)
AC_SUBST(CUPSINCLUDE)
AC_SUBST(CUPSSERVERBIN)
AC_SUBST(CUPSSERVERROOT)
AC_SUBST(CUPSDATA)


dnl look for IJS implementation
AC_ARG_WITH([ijs], AC_HELP_STRING([--without-ijs],
    [disable IJS driver support]))
dnl set safe defaults
    IJSDIR=src
    IJSDEVS=''
if test x$with_ijs != xno; then
    AC_MSG_CHECKING([for local ijs library source])
    if test -d ijs; then
        AC_MSG_RESULT([yes])
        IJSDIR=ijs
        IJSDEVS='$(DD)ijs.dev'
    else
        AC_MSG_RESULT([no])
    fi
fi
AC_SUBST(IJSDIR)
AC_SUBST(IJSDEVS)

dnl look for jbig2dec
AC_ARG_WITH([jbig2dec], AC_HELP_STRING([--without-jbig2dec],
    [disable JBIG2 decode support]))
JBIG2DIR=src
SHARE_JBIG2=0
JBIG2DEVS=''
if test x$with_jbig2dec != xno; then
  AC_MSG_CHECKING([for local jbig2dec library source])
  for d in jbig2dec jbig2dec-0.2 jbig2dec-0.3; do
    test -d "$d" && JBIG2DIR=$d && break
  done
  if test "x$JBIG2DIR" != xsrc; then
    AC_MSG_RESULT([$JBIG2DIR])
  else
    AC_MSG_RESULT([no])
    AC_CHECK_LIB([jbig2dec], [jbig2_page_out], [
	SHARE_JBIG2=1
    ], [
	AC_MSG_WARN([disabling support for JBIG2 files])
        with_jbig2dec=no
    ])
  fi
fi
if test x$with_jbig2dec != xno; then
  if test x$ac_cv_header_stdint_h != xyes; then
    AC_MSG_WARN([JBIG2 support requires stdint types which do not seem to be available.])
  else
    JBIG2DEVS='$(PSD)jbig2.dev'
  fi
fi

AC_SUBST(JBIG2DIR)
AC_SUBST(SHARE_JBIG2)
AC_SUBST(JBIG2DEVS)

dnl look for the jasper JPEG 2000 library
AC_ARG_WITH([jasper], AC_HELP_STRING([--without-jasper],
  [don't use the JasPer library for JPEG 2000]))
JASPERDIR=src
SHARE_JASPER=0
JPXDEVS=''
if test x$with_jasper != xno; then
  AC_MSG_CHECKING([for local jasper library source])
  for d in jasper jasper-1.7*; do
    test -d "$d" && JASPERDIR=$d && break
  done
  if test "x$JASPERDIR" != xsrc; then
    AC_MSG_RESULT([$JASPERDIR])
    AC_MSG_CHECKING([for local jasper configure script])
    if test -x $JASPERDIR/configure; then
      AC_MSG_RESULT([yes])
      echo
      echo "Running jasper configure script..."
      olddir=`pwd`
      cd $JASPERDIR && ./configure
      status=$?
      if test "$status" -ne 0 ; then
        AC_MSG_ERROR([jasper configure script failed], $status)
      fi
      cd $olddir
      echo
      echo "Continuing with Ghostscript configuration..."
    else
      AC_MSG_RESULT([no])
      AC_MSG_CHECKING([for local jasper autogen.sh script])
      if test -x $JASPERDIR/autogen.sh; then
        AC_MSG_RESULT([yes])
        echo
        echo "Running jasper autogen script..."
        olddir=`pwd`
        cd $JASPERDIR && ./autogen.sh
        status=$?
        if test "$status" -ne 0 ; then
          AC_MSG_ERROR([jasper autogen script failed], $status)
        fi
        cd $olddir
        echo
        echo "Continuing with Ghostscript configuration..."
      else
        AC_MSG_ERROR([
Unable to find $JASPERDIR/src/libjasper/include/jas_config.h,
or generate generate such a file for this system. You will
have to build one by hand, or configure, build and install
the jasper library separately.

You can also pass --without-jasper to configure to disable
JPEG 2000 PDF image support entirely.
])
      fi
    fi
  else
    AC_MSG_RESULT([no])
    AC_CHECK_LIB([jasper], [jas_image_create], [
	SHARE_JASPER=1
    ], [
	AC_MSG_WARN([disabling support for JPEG 2000 images])
        with_jasper=no
    ])
  fi
fi
if test x$with_jasper != xno; then
  JPXDEVS='$(PSD)jpx.dev'
fi

AC_SUBST(JASPERDIR)
AC_SUBST(SHARE_JASPER)
AC_SUBST(JPXDEVS)

dnl check if we can/should build the gtk loader
AC_ARG_ENABLE([gtk], AC_HELP_STRING([--disable-gtk],
    [Don't build the gtk loader]))
SOC_CFLAGS=""
SOC_LIBS=""
SOC_LOADER="dxmainc.c"
if test "x$enable_gtk" != "xno"; then
    # Try GTK+ 2.x first...
    if test "x$PKGCONFIG" != x; then
	AC_MSG_CHECKING(for GTK+ 2.x)
	if $PKGCONFIG --exists gtk+-2.0; then
	    SOC_LOADER="dxmain.c"
	    SOC_CFLAGS="`$PKGCONFIG gtk+-2.0 --cflags`"
	    SOC_LIBS="`$PKGCONFIG gtk+-2.0 --libs`"
	    AC_MSG_RESULT([yes])
	else
	    AC_MSG_RESULT([no])
	fi
    fi

    # Then fall back on GTK+ 1.x...
    if test "x$SOC_LOADER" = x; then
	AC_PATH_PROG(GTKCONFIG, gtk-config)
	if test "x$GTKCONFIG" != x; then
	    SOC_LOADER="dxmain.c"
	    SOC_CFLAGS="`$GTKCONFIG --cflags`"
	    SOC_LIBS="`$GTKCONFIG --libs`"
	fi
    fi
fi
AC_SUBST(SOC_CFLAGS)
AC_SUBST(SOC_LIBS)
AC_SUBST(SOC_LOADER)

dnl enable the cairo device if the library is available
AC_ARG_ENABLE([cairo], AC_HELP_STRING([--disable-cairo],
    [Don't include the cairo output device]))
CAIRO_DEVS=""
CAIRO_CFLAGS=""
CAIRO_LIBS=""
if test "x$enable_cairo" != xno; then
  AC_PATH_PROG(PKGCONFIG, pkg-config)
  if test "x$PKGCONFIG" != x; then
    AC_MSG_CHECKING([for cairo])
    if $PKGCONFIG --exists 'cairo >= 1.2.0'; then
      CAIRO_CFLAGS=`$PKGCONFIG --cflags cairo`
      CAIRO_LIBS=`$PKGCONFIG --libs cairo`
      CAIRO_DEVS="cairo"
      AC_MSG_RESULT([yes])
    else
      AC_MSG_RESULT([no])
    fi
  fi
fi

AC_SUBST(CAIRO_CFLAGS)
AC_SUBST(CAIRO_LIBS)

dnl look for omni implementation
AC_ARG_WITH([omni], AC_HELP_STRING([--without-omni],
	[disable the omni driver]))
dnl set safe defaults
OMNIDEVS=''
INCLUDEOMNI=yes
if ( ! test -z "$CONTRIBINCLUDE" ) && ( test x$with_omni != xno ); then
	OMNIDEVS='$(DD)omni.dev'

	if test -n "$GCC"; then
		LIBS="$LIBS -lstdc++"
	fi
fi
AC_SUBST(OMNIDEVS)

dnl optional X11 for display devices
AC_PATH_XTRA

X_LDFLAGS=""
X_CFLAGS=""
X_DEVS=""
X_LIBS=""

if test x$no_x != xyes; then
	if test "$x_libraries" = "/usr/lib"; then
		echo "Ignoring X library directory \"$x_libraries\" requested by configure."
		x_libraries="NONE"
	fi
	if test ! "$x_libraries" = "NONE" -a ! "$x_libraries" = ""; then
		X_LDFLAGS="-L$x_libraries"
		if test "$uname" = "SunOS"; then
			X_LDFLAGS="$X_LDFLAGS -R$x_libraries"
		fi
	fi

	if test "$x_includes" = "/usr/include"; then
		echo "Ignoring X include directory \"$x_includes\" requested by configure."
		x_includes="NONE"
	fi
	if test ! "$x_includes" = "NONE" -a ! "$x_includes" = ""; then
		X_CFLAGS="-I$x_includes"
	fi

	SAVELIBS="$LIBS"
	SAVELDFLAGS="$LDFLAGS"
	LDFLAGS="$LDFLAGS $X_LDFLAGS"

	AC_CHECK_LIB(X11,XOpenDisplay)
	AC_CHECK_LIB(Xext,XdbeQueryExtension)
	AC_CHECK_LIB(Xt,XtAppCreateShell)

	LDFLAGS="$SAVELDFLAGS"
	LIBS="$SAVELIBS"

	if test "$ac_cv_lib_Xt_XtAppCreateShell" = yes; then
		X11DEVS="\$(DD)x11.dev \$(DD)x11alpha.dev \$(DD)x11cmyk.dev \$(DD)x11mono.dev \$(DD)x11_.dev \$(DD)x11alt_.dev \$(DD)x11cmyk2.dev \$(DD)x11cmyk4.dev \$(DD)x11cmyk8.dev \$(DD)x11rg16x.dev \$(DD)x11rg32x.dev \$(DD)x11gray2.dev \$(DD)x11gray4.dev"
		X_DEVS=$X11DEVS
		# the makefile wants a list of just the library names in X_LIBS
		GS_SPLIT_LIBS([X_LIBS], 
			[-lXt $X_PRE_LIBS -lXext -lX11 $X_EXTRA_LIBS])
	fi
fi

AC_SUBST(X_LDFLAGS)
AC_SUBST(X_CFLAGS)
AC_SUBST(X_LIBS)
AC_SUBST(X_DEVS)
AC_SUBST(X11DEVS)
AC_SUBST(XLIBS)

dnl executible name
AC_ARG_WITH([gs], AC_HELP_STRING([--with-gs=NAME],
	[name of the Ghostscript executible [[gs]]]),
	[GS="$with_gs"],[GS='gs'])
AC_SUBST(GS)

dnl do we compile the postscript initialization files into Ghostscript?
COMPILE_INITS="1"
AC_ARG_ENABLE([compile-inits], AC_HELP_STRING([--disable-compile-inits],
       [Don't compile in initialization files]),[
               if test "x$enable_compile_inits" = xno; then
                       COMPILE_INITS="0"
               fi])
AC_SUBST(COMPILE_INITS)

dnl look for drivers to compile...
AC_ARG_WITH(drivers,
[  --with-drivers=LIST     Drivers to support, separated by commas.
                          Either list the drivers or use aliases:
                          ALL      = all drivers
                          FILES    = all file format drivers
                          PRINTERS = all printer drivers
                          Printers:
                          APPLE    = all Apple printers
                          BROTHER  = all Brother printers
                          CANON    = all Canon printers
                          EPSON    = all Epson printers
                          HP       = all HP printers
                          IBM      = all IBM printers
                          JAPAN    = older japanese printers
                          LEXMARK  = all Lexmark printers
                          OKI      = all OKI printers
			  PCLXL    = all PCL XL/6 printers
                          File formats:
                          BMP      = Output to bmp files
                          FAX      = Output to fax files
                          JPEG     = Output to JPEG files
                          PBM      = Output to PBM/PNM
                          PCX      = Output to PCX
			  PNG      = Output to PNG
                          PS       = Output to PostScript/PDF
                          TIFF     = Output to TIFF
                          CAIRO    = Output using libcairo
			  WTS      = WTS Halftoning devices
			  ETS      = ETS Halftoning devices
                          You can mix both variants, e.g.
                          --with-drivers=HP,stcolor would build HP drivers and
                          the Epson stcolor driver.
                          Aliases must be uppercase (a 3rd party driver might
                          have the same name).
                          Default: ALL],drivers="$withval",drivers="ALL")

AC_ARG_WITH(driversfile,
[  --with-driversfile=FILE Drivers to support from file, separated by newlines.],
driversfile="$withval",driversfile="")

if test "x$driversfile" != x; then
	# Add drivers from file...
	drivers="`tr '\n' ',' <$driversfile`"
fi

dnl Check which drivers we'd like to support.
P_DEVS=""
F_DEVS=""

dnl Known printers
HP_DEVS="cdj500 djet500 djet500c dnj650c cljet5pr deskjet laserjet ljetplus ljet2p ljet3 ljet3d ljet4 ljet4d lj4dith lj5mono lj5gray cdeskjet cdjcolor cdjmono cdj550 pj pjxl pjxl300 lp2563 paintjet pjetxl cljet5 cljet5c pxlmono pxlcolor cdj670 cdj850 cdj880 cdj890 cdj970 cdj1600 chp2200 pcl3 hpdjplus hpdjportable hpdj310 hpdj320 hpdj340 hpdj400 hpdj500 hpdj500c hpdj510 hpdj520 hpdj540 hpdj550c hpdj560c hpdj600 hpdj660c hpdj670c hpdj680c hpdj690c hpdj850c hpdj855c hpdj870c hpdj890c hpdj1120c lj3100sw"
PCLXL_DEVS="pxlmono pxlcolor"
EPSON_DEVS="eps9high eps9mid epson epsonc escp lp8000 lq850 photoex st800 stcolor alc1900 alc2000 alc4000 alc4100 alc8500 alc8600 alc9100 lp3000c lp8000c lp8200c lp8300c lp8500c lp8800c lp9000c lp9200c lp9500c lp9800c lps6500 epl2050 epl2050p epl2120 epl2500 epl2750 epl5800 epl5900 epl6100 epl6200 lp1800 lp1900 lp2200 lp2400 lp2500 lp7500 lp7700 lp7900 lp8100 lp8300f lp8400f lp8600 lp8600f lp8700 lp8900 lp9000b lp9100 lp9200b lp9300 lp9400 lp9600 lp9600s lps4500"
CANON_DEVS="bj10e bj200 bjc600 bjc800 lbp8 lips3 bjcmono bjcgray bjccmyk bjccolor"
LEXMARK_DEVS="lxm5700m lx5000 lxm3200 lex2050 lex3200 lex5700 lex7000"
BROTHER_DEVS="hl7x0 hl1240 hl1250"
APPLE_DEVS="appledmp iwhi iwlo iwlq"
IBM_DEVS="ibmpro jetp3852"
OKI_DEVS="oki182 okiibm oki4w"
JAPAN_DEVS="lips4 lips4v ljet4pjl lj4dithp dj505j picty180 lips2p bjc880j pr201 pr150 pr1000 pr1000_4 jj100 bj10v bj10vh mj700v2c mj500c mj6000c mj8000c fmpr fmlbp ml600 lbp310 lbp320 md50Mono md50Eco md1xMono escpage lp2000 npdl rpdl"
MISC_PDEVS="uniprint ap3250 atx23 atx24 atx38 coslw2p coslwxl cp50 declj250 fs600 imagen lj250 m8510 necp6 oce9050 r4081 sj48 tek4696 t4693d2 t4693d4 t4693d8 dl2100 la50 la70 la75 la75plus ln03 xes md2k md5k gdi samsunggdi"
OPVP_DEVS="opvp oprp"
ETS_HALFTONING_DEVS="rinkj"

dnl Known file formats
BMP_DEVS="bmpmono bmpgray bmpsep1 bmpsep8 bmp16 bmp256 bmp16m bmp32b"
FAX_DEVS="cfax dfaxlow dfaxhigh fax tfax tiffg3 tiffg32d tiffg4 faxg3 faxg32d faxg4"
JPEG_DEVS="jpeg jpeggray jpegcmyk"
PNG_DEVS="png16 png16m png256 pngalpha pnggray pngmono"
TIFF_DEVS="tiffs tiff12nc tiff24nc tiff32nc tiffcrle tifflzw tiffpack tiffgray tiffsep"
PCX_DEVS="pcxmono pcxgray pcx16 pcx256 pcx24b pcxcmyk pcx2up"
PBM_DEVS="pbm pbmraw pgm pgmraw pgnm pgnmraw pnm pnmraw ppm ppmraw pkm pkmraw pksm pksmraw pam"
PS_DEVS="psdf psdcmyk psdrgb pdfwrite pswrite ps2write epswrite psgray psmono psrgb bbox"
# CAIRO_DEVS is defined conditionally above
WTS_HALFTONING_DEVS="imdi simdi wtsimdi wtscmyk"
MISC_FDEVS="ccr cgm24 cgm8 cgmmono cif inferno mag16 mag256 mgr4 mgr8 mgrgray2 mgrgray4 mgrgray8 mgrmono miff24 plan9bm sgirgb sunhmono bit bitrgb bitrgbtags bitcmyk devicen spotcmyk xcf"

while test -n "$drivers"; do
	if echo $drivers |grep "," >/dev/null; then
		THIS="`echo $drivers |sed -e 's/,.*//'`"
		drivers="`echo $drivers |sed -e \"s/$THIS,//\"`"
	else
		THIS=$drivers
		drivers=""
	fi
	case "$THIS" in
	ALL)
		# ALL = PRINTERS + FILES...
		if test -z "$drivers"; then
			drivers="PRINTERS,FILES"
		else
			drivers="$drivers,PRINTERS,FILES"
		fi
		;;
	PRINTERS)
		P_DEVS="$P_DEVS $CANON_DEVS $EPSON_DEVS $HP_DEVS $LEXMARK_DEVS $BROTHER_DEVS $APPLE_DEVS $IBM_DEVS $OKI_DEVS $JAPAN_DEVS $MISC_PDEVS $ETS_HALFTONING_DEVS $OPVP_DEVS"
		;;
	FILES)
		F_DEVS="$F_DEVS $BMP_DEVS $FAX_DEVS $JPEG_DEVS $PNG_DEVS $TIFF_DEVS $PCX_DEVS $PBM_DEVS $PS_DEVS $CAIRO_DEVS $WTS_HALFTONING_DEVS $MISC_FDEVS"
		;;
	APPLE)
		# All Apple printers
		P_DEVS="$P_DEVS $APPLE_DEVS"
		;;
	BMP)
		# BMP file format
		F_DEVS="$F_DEVS $BMP_DEVS"
		;;
	CANON)
		# All Canon printers
		P_DEVS="$P_DEVS $CANON_DEVS"
		;;
	EPSON)
		# All Epson printers
		P_DEVS="$P_DEVS $EPSON_DEVS"
		;;
	FAX)
		# Fax file formats
		F_DEVS="$F_DEVS $FAX_DEVS"
		;;
	JPEG)
		# Jpeg file formats
		F_DEVS="$F_DEVS $JPEG_DEVS"
		;;
	PNG)
		# PNG file formats
		F_DEVS="$F_DEVS $PNG_DEVS"
		;;
	TIFF)
		# TIFF file formats
		F_DEVS="$F_DEVS $TIFF_DEVS"
		;;
	PCLXL)
		# PCL XL/PCL 6 drivers
		F_DEVS="$F_DEVS $PCLXL_DEVS"
		;;
	PCX)
		# PCX file formats
		F_DEVS="$F_DEVS $PCX_DEVS"
		;;
	PBM)
		# PBM file formats
		F_DEVS="$F_DEVS $PBM_DEVS"
		;;
	HP)
		# All HP printers
		P_DEVS="$P_DEVS $HP_DEVS"
		;;
	LEXMARK)
		# All Lexmark printers
		P_DEVS="$P_DEVS $LEXMARK_DEVS"
		;;
	BROTHER)
		# All Brother printers
		P_DEVS="$P_DEVS $BROTHER_DEVS"
		;;
	OKI)
		# All OKI printers
		P_DEVS="$P_DEVS $OKI_DEVS"
		;;
	IBM)
		# All IBM printers
		P_DEVS="$P_DEVS $IBM_DEVS"
		;;
	JAPAN)
		# All old japanese printers
		P_DEVS="$P_DEVS $JAPAN_DEVS"
		;;
	PS)
		# PostScript/PDF writing
		F_DEVS="$F_DEVS $PS_DEVS"
		;;
	CAIRO)
		$ cairo output device
		F_DEVS="$F_DEVS $CAIRO_DEVS"
		;;
	WTS)
		# WTS Halftoning devices
		F_DEVS="$F_DEVS $WTS_HALFTONING_DEVS"
		;;
	ETS)
		# ETS Halftoning devices
		F_DEVS="$F_DEVS $ETS_HALFTONING_DEVS"
		;;
        opvp)
		# Open Vector Printing driver...
		if test x$ac_cv_lib_dl_dlopen != xno -a x$found_iconv != xno; then
			P_DEVS="$P_DEVS $OPVP_DEVS"
		else
			AC_MSG_WARN(Unable to include opvp/oprp driver due to missing prerequisites...)
		fi
		;;
	*)
		# It's a driver name (or a user messup)
		P_DEVS="$P_DEVS `echo $THIS |sed -e 's,\.dev$,,'`"
		;;
	esac
done
# No need to include opvp/oprp driver without iconv/libiconv.
if test -n "$P_DEVS"; then
	if test x$found_iconv = xno ; then
		P_DEVS=`echo $P_DEVS | sed -e 's|opvp||' -e 's|oprp||'` 
		AC_MSG_WARN(Unable to include opvp/oprp driver due to missing iconv/libiconv...)
	fi

# Remove contributed drivers if requested and make sure we don't have any 
# duplicates in there, add $(DD)foo.dev constructs
	noncontribmakefiles=`find . -name '*.mak' -print | grep -v '^\./contrib/'`
	PRINTERS=`(for i in $P_DEVS; do d='$(DD)'${i}.dev; if ( grep '^'$d $noncontribmakefiles 2>&1 >/dev/null ) || ( ! test -z "$CONTRIBINCLUDE" ); then echo $d; fi; done) | sort | uniq | tr '\012' ' '`
fi
if test -n "$F_DEVS"; then
	FILES=`(for i in $F_DEVS; do d='$(DD)'${i}.dev; if ( grep '^'$d $noncontribmakefiles  2>&1 >/dev/null) || ( ! test -z "$CONTRIBINCLUDE" ); then echo $d; fi; done) | sort | uniq | tr '\012' ' '`
fi
AC_SUBST(PRINTERS)
AC_SUBST(FILES)

dnl Dynamic device support.
DYNAMIC_CFLAGS=""
DYNAMIC_DEVS=""
DYNAMIC_FLAGS=""
DYNAMIC_LDFLAGS=""
DYNAMIC_LIBS=""
INSTALL_SHARED=""

AC_ARG_ENABLE([dynamic], AC_HELP_STRING([--enable-dynamic],
    [Enable dynamically loaded drivers]),
[	case `uname` in
		Linux*|GNU*)
		INSTALL_SHARED="install-shared"
		DYNAMIC_CFLAGS="-fPIC"
		if test "x$X_DEVS" != x; then
			DYNAMIC_DEVS="\$(GLOBJDIR)/X11.so"
		else
			DYNAMIC_DEVS=""
		fi
		DYNAMIC_FLAGS="-DGS_DEVS_SHARED -DGS_DEVS_SHARED_DIR=\\\"\$(gssharedir)\\\""
		DYNAMIC_LDFLAGS="-fPIC -shared"
		DYNAMIC_LIBS="-rdynamic -ldl"
		X_DEVS=""
		;;
		*BSD)
		DYNAMIC_CFLAGS="-fPIC"
		DYNAMIC_DEVS="\$(GLOBJDIR)/X11.so"
		DYNAMIC_FLAGS="-DGS_DEVS_SHARED -DGS_DEVS_SHARED_DIR=\\\"\$(gssharedir)\\\""
		DYNAMIC_LDFLAGS="-fPIC -shared"
		DYNAMIC_LIBS=""
		X_DEVS=""
		;;
		Darwin*)
		INSTALL_SHARED="install-shared"
		DYNAMIC_FLAGS="-DGS_DEVS_SHARED -DGS_DEVS_SHARED_DIR=\\\"\$(gssharedir)\\\""
		DYNAMIC_LDFLAGS="-dynamiclib"
		DYNAMIC_LIBS=""
		X_DEVS=""
		;;
		SunOS)
		DYNAMIC_CFLAGS="-KPIC"
		DYNAMIC_DEVS="\$(GLOBJDIR)/X11.so"
		DYNAMIC_FLAGS="-DGS_DEVS_SHARED -DGS_DEVS_SHARED_DIR=\\\"\$(gssharedir)\\\""
		DYNAMIC_LDFLAGS="-G"
		DYNAMIC_LIBS=""
		X_DEVS=""
		;;
		*)
		AC_MSG_ERROR([Sorry, dynamic driver support not available on this platform!])
		;;
	esac
])

AC_SUBST(DYNAMIC_CFLAGS)
AC_SUBST(DYNAMIC_DEVS)
AC_SUBST(DYNAMIC_FLAGS)
AC_SUBST(DYNAMIC_LDFLAGS)
AC_SUBST(DYNAMIC_LIBS)
AC_SUBST(INSTALL_SHARED)

dnl look for default font path...
AC_ARG_WITH([fontpath],  AC_HELP_STRING([--with-fontpath],
    [set font search path for gs]), fontpath="$withval", fontpath="")

dnl Fix "prefix" variable...
if test "x$prefix" = xNONE; then
	prefix=/usr/local
fi

dnl Fix "datadir" variable...
if test "x$datadir" = 'x${prefix}/share'; then
	datadir="$prefix/share"
fi

dnl Fix "fontpath" variable...
if test "x$fontpath" = "x"; then
	# These font directories are used by various Linux distributions...
	fontpath="$datadir/fonts/default/ghostscript"
	fontpath="${fontpath}:$datadir/fonts/default/Type1"
	fontpath="${fontpath}:$datadir/fonts/default/TrueType"

	# These font directories are used by IRIX...
	fontpath="${fontpath}:/usr/lib/DPS/outline/base"

	# These font directories are used by Solaris...
	fontpath="${fontpath}:/usr/openwin/lib/X11/fonts/Type1"
	fontpath="${fontpath}:/usr/openwin/lib/X11/fonts/TrueType"

	# This font directory is used by CUPS...
	if test "x$CUPSCONFIG" != x; then
	  fontpath="${fontpath}:`$CUPSCONFIG --datadir`/fonts"
	fi
fi

AC_SUBST(fontpath)

dnl --------------------------------------------------
dnl Check for library functions
dnl --------------------------------------------------

AC_CHECK_FUNCS([mkstemp], [HAVE_MKSTEMP=-DHAVE_MKSTEMP])
AC_SUBST(HAVE_MKSTEMP)

AC_CHECK_FUNCS([hypot], [HAVE_HYPOT=-DHAVE_HYPOT])
AC_SUBST(HAVE_HYPOT)

AC_CHECK_FUNCS([fopen64], [HAVE_FILE64=-DHAVE_FILE64])
AC_SUBST(HAVE_FILE64)

AC_CHECK_FUNCS([mkstemp64], [HAVE_MKSTEMP64=-DHAVE_MKSTEMP64])
AC_SUBST(HAVE_MKSTEMP64)

AC_PROG_GCC_TRADITIONAL

dnl NB: We don't actually provide autoconf-switched fallbacks for any
dnl     of these functions, so the checks are purely informational.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([bzero dup2 floor gettimeofday memchr memmove memset mkdir mkfifo modf pow putenv rint setenv sqrt strchr strerror strrchr strspn strstr])

dnl --------------------------------------------------
dnl Do substitutions
dnl --------------------------------------------------

AC_SUBST(OPT_CFLAGS)
AC_SUBST(GCFLAGS)
AC_OUTPUT(Makefile cups/pstopxl cups/pstoraster)

chmod +x cups/pstopxl cups/pstoraster
