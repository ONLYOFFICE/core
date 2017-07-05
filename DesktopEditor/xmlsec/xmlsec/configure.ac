dnl Process this file with autoconf to produce a configure script.
AC_INIT([xmlsec1],[1.2.23],[http://www.aleksey.com/xmlsec])

XMLSEC_PACKAGE=xmlsec1
XMLSEC_VERSION_MAJOR=1
XMLSEC_VERSION_MINOR=2
XMLSEC_VERSION_SUBMINOR=23
XMLSEC_VERSION="$XMLSEC_VERSION_MAJOR.$XMLSEC_VERSION_MINOR.$XMLSEC_VERSION_SUBMINOR"
XMLSEC_VERSION_INFO=`echo $XMLSEC_VERSION | awk -F. '{ printf "%d:%d:%d", $1+$2, $3, $2 }'`
XMLSEC_VERSION_SAFE=`echo $XMLSEC_VERSION | sed 's/\./_/g'`

AC_PREREQ([2.52g])
AC_CANONICAL_HOST

AC_SUBST(XMLSEC_VERSION)
AC_SUBST(XMLSEC_PACKAGE)
AC_SUBST(XMLSEC_VERSION_SAFE)
AC_SUBST(XMLSEC_VERSION_MAJOR)
AC_SUBST(XMLSEC_VERSION_MINOR)
AC_SUBST(XMLSEC_VERSION_SUBMINOR)
AC_SUBST(XMLSEC_VERSION_INFO)

AC_CONFIG_MACRO_DIR(m4)
AM_INIT_AUTOMAKE([1.7 tar-ustar subdir-objects -Wno-portability])
AC_CONFIG_HEADERS([config.h])
AM_MAINTAINER_MODE

# Support silent build rules, requires at least automake-1.11. Disable
# by either passing --disable-silent-rules to configure or passing V=1
# to make
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

dnl
dnl Check the environment
dnl
AC_PROG_CC
AC_PROG_INSTALL
AC_HEADER_STDC
AC_PROG_LIBTOOL

LT_INIT

dnl
dnl Find programs
dnl
if test "z$RM" == "z" ; then 
    AC_PATH_PROG(RM, rm, /bin/rm)
fi
AC_PATH_PROG(CP, cp, /bin/cp)
AC_PATH_PROG(MV, mv, /bin/mv)
AC_PATH_PROG(TAR, tar, /bin/tar)
AC_PATH_PROG(PERL, perl)


dnl Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([ctype.h])
AC_CHECK_HEADERS([errno.h])
AC_CHECK_HEADERS([ansidecl.h])
AC_CHECK_HEADERS([time.h])
AC_CHECK_FUNCS(strchr strrchr printf sprintf fprintf snprintf vfprintf vsprintf vsnprintf sscanf timegm)

XMLSEC_DEFINES=""

dnl ==========================================================================
dnl Specific setup for Aleksey's development environment: 
dnl	- with debug
dnl - with pedantic compiler flags
dnl - with docs build
dnl	- with static binaries
dnl	- without dynamic xmlsec-crypto loading
dnl ==========================================================================
AC_MSG_CHECKING(for development environment) 
AC_ARG_ENABLE(development,   [  --enable-development    enable development environment (no)])
if test "z$enable_development" = "zyes" ; then
    enable_debugging="yes"
    enable_pedantic="yes"
    enable_static_linking="yes"
    enable_manpages_build="yes"
    enable_docs_build="yes"
    enable_crypto_dl="no"
    AC_MSG_RESULT(yes)
else
    AC_MSG_RESULT(no)
fi

dnl ==========================================================================
dnl Do we want to build manpages?
dnl ==========================================================================
AC_MSG_CHECKING(for man pages build) 
AC_ARG_ENABLE(manpages_build,[  --enable-manpages-build man pages build (no)])
if test "z$enable_manpages_build" = "zyes" ; then
    AC_PATH_PROG(HELP2MAN, help2man)

    AC_MSG_RESULT(yes)
else
    AC_MSG_RESULT(no)
fi

AM_CONDITIONAL(HAS_HELP2MAN, [test x"$HELP2MAN" != x])
AM_CONDITIONAL(BUILD_MANPAGES, [test x"$HELP2MAN" != x])

dnl ==========================================================================
dnl Do we want to build docs?
dnl ==========================================================================
AC_MSG_CHECKING(for docs build) 
AC_ARG_ENABLE(docs_build,    [  --enable-docs-build     docs build (no)])
if test "z$enable_docs_build" = "zyes" ; then
    AC_PATH_PROG(XSLTPROC, xsltproc)
    AC_PATH_PROG(MAN2HTML, man2html)
    AC_PATH_PROG(GTKDOC_MKHTML, gtkdoc-mkhtml)
    AC_PATH_PROG(GTKDOC_MKTMPL, gtkdoc-mktmpl)
    AC_PATH_PROG(GTKDOC_MKDB,   gtkdoc-mkdb)
    AC_PATH_PROG(GTKDOC_SCAN,   gtkdoc-scan)

    AC_MSG_RESULT(yes)
else
    AC_MSG_RESULT(no)
fi

AM_CONDITIONAL(HAS_MAN2HTML, [test x"$MAN2HTML" != x])
AM_CONDITIONAL(HAS_XSLTPROC, [test x"$XSLTPROC" != x])

AM_CONDITIONAL(BUILD_MAN_DOCS, [test x"$PERL" != x && test x"$XSLTPROC" != x && test x"$MAN2HTML" != x])
AM_CONDITIONAL(BUILD_API_DOCS, [test x"$PERL" != x && test x"$XSLTPROC" != x && test x"$GTKDOC_MKHTML" != x && test x"$GTKDOC_MKTMPL" != x && test x"$GTKDOC_MKDB" != x && test x"$GTKDOC_SCAN" != x])

dnl ==========================================================================
dnl Perform host specific configuration
dnl ==========================================================================
XMLSEC_EXTRA_LDFLAGS=
XMLSEC_CRYPTO_EXTRA_LDFLAGS=
dnl should separate extra LDFLAGS for xmlsec and crypto libs ?
dnl should --enable-runtime-pseudo-reloc for mingw and cygwin ?

xmlsec_sharedlib_hack="no"
if test -n "$shrext"; then
  XMLSEC_SHLIBSFX="$shrext"
else
  dnl changed in libtool (between 1.5.2 and 1.5.14)
  XMLSEC_SHLIBSFX="$shrext_cmds"
fi
case "${host}" in
 *aix* )
	CFLAGS="${CFLAGS} -D_ALL_SOURCE"
	;;
 *-*-mingw*)
	dnl use libtool flags "-avoid-version" to create shared
	dnl libraries without version suffix, i.e. libxmlsec1.dll
	dnl instead libxmlsec1-NN.dll, where NN is a number.
	XMLSEC_EXTRA_LDFLAGS="-no-undefined -avoid-version"
	XMLSEC_CRYPTO_EXTRA_LDFLAGS="-no-undefined -avoid-version"
	XMLSEC_SHLIBSFX=".dll.a"
	xmlsec_sharedlib_hack="yes"
	;;
 *-*-cygwin*)
	XMLSEC_EXTRA_LDFLAGS="-no-undefined"
	XMLSEC_CRYPTO_EXTRA_LDFLAGS="-no-undefined"
	;;
 *-*-darwin*)
        XMLSEC_SHLIBSFX=".dylib"
        ;;
esac

# To avoid problem with loading of a shared library (dlopen or equivalent)
# at run time on some platforms we need to link crypto modules with extra
# source. It's work without hack on 9x and under emulation.
# On nt 5.x (w2k,xp) the error is 998("Invalid access to memory location").
AM_CONDITIONAL(SHAREDLIB_HACK, [test "z$xmlsec_sharedlib_hack" = "zyes"])

dnl ==========================================================================
dnl Hack for autoconf version mismatch
dnl ==========================================================================
if test "z$shrext" == "z" ; then 
    shrext=$shrext_cmds
fi

dnl ==========================================================================
dnl Check for __FUNCTION__ or __func__
dnl ==========================================================================
AC_MSG_CHECKING(for __FUNCTION__ or __func__)
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([],
	[[char *foo = __FUNCTION__;]])],
	[ac_function_exists=yes],
	[ac_function_exists=no])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([],
	[[char *foo = __func__;]])],
	[ac_func_exists=yes],
	[ac_func_exists=no])
if test "z$ac_func_exists" = "zyes" ; then 
	AC_MSG_RESULT(__func__)
	XMLSEC_DEFINES="$XMLSEC_DEFINES -D__XMLSEC_FUNCTION__=__func__"
elif test "z$ac_function_exists" = "zyes" ; then
    AC_MSG_RESULT(__FUNCTION__)
    XMLSEC_DEFINES="$XMLSEC_DEFINES -D__XMLSEC_FUNCTION__=__FUNCTION__"
else 
    AC_MSG_RESULT("no")
fi

dnl ==========================================================================
dnl check do we have size_t and its size, 
dnl TODO: will need to replace this and the xmlSecSize define with 
dnl typedef on next ABI refresh
dnl ==========================================================================
AC_CHECK_SIZEOF(size_t)
if test "$ac_cv_sizeof_size_t" -ne "4" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_SIZE_T"
fi

dnl ==========================================================================
dnl Check if pkg-config enabled and installed
dnl ==========================================================================
PKGCONFIG_FOUND="no"
AC_ARG_ENABLE(pkgconfig,   [  --enable-pkgconfig      enable pkgconfig for configuration (yes)])
if test "z$enable_pkgconfig" != "zno" ; then
    PKG_PROG_PKG_CONFIG
    PKGCONFIG_FOUND="yes"
else
    PKGCONFIG_FOUND="no"
fi

dnl ==========================================================================
dnl find libxml
dnl ==========================================================================
LIBXML_MIN_VERSION="2.7.4"
LIBXML_CONFIG="xml2-config"
LIBXML_CFLAGS=""
LIBXML_LIBS=""
LIBXML_FOUND="no"
AC_ARG_WITH(libxml, 
    [  --with-libxml=[PFX]       libxml2 location]
)
AC_ARG_WITH(libxml-src, 
    [  --with-libxml-src=[PFX]   not installed yet libxml2 location]
)

if test "z$with_libxml" = "zno" -o "z$with_libxml_src" = "zno"; then 
    AC_MSG_CHECKING(for libxml2 libraries >= $LIBXML_MIN_VERSION) 
    AC_MSG_ERROR(libxml2 >= $LIBXML_MIN_VERSION is required for $XMLSEC_PACKAGE)
elif test "z$with_libxml_src" != "z" ; then
    AC_MSG_CHECKING(for libxml2 libraries >= $LIBXML_MIN_VERSION) 
    CWD=`pwd`
    if cd "$with_libxml_src" ; then 
	SRC_DIR=`pwd`
	LIBXML_CONFIG=${SRC_DIR}/xml2-config
	LIBXML_LIBS="-L${SRC_DIR}/.libs -lxml2"
	LIBXML_CFLAGS="-I${SRC_DIR}/include"
	LIBXML_FOUND="yes"
	cd $CWD
	AC_MSG_RESULT([yes (source)])    
    else 
	AC_MSG_ERROR([libxml source dir not found (${with_libxml_src}), typo?])
    fi 	
elif test "z$with_libxml" = "z" -a "z$PKGCONFIG_FOUND" = "zyes" ; then
    PKG_CHECK_MODULES(LIBXML, libxml-2.0 >= $LIBXML_MIN_VERSION,
	[LIBXML_FOUND=yes],
	[LIBXML_FOUND=no])
fi
if test "z$LIBXML_FOUND" = "zno" ; then
    if test "z$with_libxml" != "zyes" ; then
        if test "z$with_libxml" != "z" ; then
	    AC_PATH_PROG([LIBXML_CONFIG], [$LIBXML_CONFIG], [],
		     [$with_libxml/bin:$PATH])
	else
	    AC_PATH_PROG([LIBXML_CONFIG], [$LIBXML_CONFIG], [],
		     [$PATH])
	fi
    fi
    AC_MSG_CHECKING([libxml2 $LIBXML_CONFIG ])
    if ! LIBXML_VERSION=`$LIBXML_CONFIG --version 2>/dev/null`; then
	AC_MSG_ERROR(Could not find libxml2 anywhere.)
    fi
    vers=`echo $LIBXML_VERSION | awk -F. '{ printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`
    minvers=`echo $LIBXML_MIN_VERSION | awk -F. '{ printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`
    if test "$vers" -ge "$minvers" ; then
        LIBXML_LIBS="`$LIBXML_CONFIG --libs`"
        LIBXML_CFLAGS="`$LIBXML_CONFIG --cflags`"
	LIBXML_FOUND="yes"
        AC_MSG_RESULT([yes ('$LIBXML_VERSION')])
    else
        AC_MSG_ERROR(You need at least libxml2 $LIBXML_MIN_VERSION for this version of $XMLSEC_PACKAGE)
    fi
fi

AC_SUBST(LIBXML_CFLAGS)
AC_SUBST(LIBXML_LIBS)
AC_SUBST(LIBXML_CONFIG)
AC_SUBST(LIBXML_MIN_VERSION)

dnl ==========================================================================
dnl find libxslt
dnl ==========================================================================
XMLSEC_NO_LIBXSLT="1"
LIBXSLT_MIN_VERSION=1.0.20
LIBXSLT_PC_FILE_COND="libxslt >= $LIBXSLT_MIN_VERSION"
LIBXSLT_CONFIG="xslt-config"
LIBXSLT_CFLAGS=""
LIBXSLT_LIBS=""
LIBXSLT_FOUND="no"
AC_ARG_WITH(libxslt, 
    [  --with-libxslt=[PFX]      libxslt location]
)
AC_ARG_WITH(libxslt-src, 
    [  --with-libxslt-src=[PFX]  not installed yet libxslt location]
)
if test "z$with_libxslt" = "zno" -o "z$with_libxslt_src" = "zno" ; then
    AC_MSG_CHECKING(for libxslt libraries >= $LIBXSLT_MIN_VERSION) 
    AC_MSG_RESULT(no)
    LIBXSLT_FOUND="without"
elif test "z$with_libxslt_src" != "z" ; then
    AC_MSG_CHECKING(for libxslt libraries >= $LIBXSLT_MIN_VERSION) 
    CWD=`pwd`
    if cd "$with_libxslt_src" ; then 
	SRC_DIR=`pwd`
	LIBXSLT_CONFIG=${SRC_DIR}/xslt-config
	LIBXSLT_LIBS="-L${SRC_DIR}/libxslt/.libs -lxslt"
	LIBXSLT_CFLAGS="-I${SRC_DIR}"
	LIBXSLT_FOUND="yes"
	cd $CWD
	AC_MSG_RESULT([yes (source)])    
    else 
	AC_MSG_ERROR([libxslt source dir not found (${with_libxslt_src}), typo?])
    fi 	
elif test "z$with_libxslt" = "z" -a  "z$PKGCONFIG_FOUND" = "zyes" ; then
    PKG_CHECK_MODULES(LIBXSLT, libxslt >= $LIBXSLT_MIN_VERSION,
	[LIBXSLT_FOUND=yes],
	[LIBXSLT_FOUND=no])
fi

if test "z$LIBXSLT_FOUND" = "zno" ; then
    if test "z$with_libxslt" != "zyes" ; then
        if test "z$with_libxslt" != "z" ; then
	    AC_PATH_PROG([LIBXSLT_CONFIG], [$LIBXSLT_CONFIG], [],
		     [$with_libxslt/bin:$PATH])
        else
	    AC_PATH_PROG([LIBXSLT_CONFIG], [$LIBXSLT_CONFIG], [],
		     [$PATH])
        fi
    fi
    AC_MSG_CHECKING(for libxslt libraries >= $LIBXSLT_MIN_VERSION) 
    if ! LIBXSLT_VERSION=`$LIBXSLT_CONFIG --version 2>/dev/null`; then
	if test "z$with_libxslt" != "zyes" ; then
	    AC_MSG_ERROR(Unable to find libxslt at '$with_libxslt')
	else 
	    AC_MSG_RESULT(no)
	fi
    else
        vers=`echo $LIBXSLT_VERSION | awk -F. '{ printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`
	minvers=`echo $LIBXSLT_MIN_VERSION | awk -F. '{ printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`
	if test "$vers" -ge "$minvers" ; then
    	    LIBXSLT_LIBS="`$LIBXSLT_CONFIG --libs`"
	    LIBXSLT_CFLAGS="`$LIBXSLT_CONFIG --cflags`"
	    LIBXSLT_FOUND="yes"
    	    AC_MSG_RESULT([yes ('$LIBXSLT_VERSION')])
	else
    	    AC_MSG_ERROR(You need at least libxslt $LIBXSLT_MIN_VERSION for this version of $XMLSEC_PACKAGE)
	fi	
    fi
fi

if test "z$LIBXSLT_FOUND" = "zyes" ; then
    XMLSEC_NO_LIBXSLT="0"
else
    LIBXSLT_PC_FILE_COND=""
    LIBXSLT_MIN_VERSION=""
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_XSLT=1"
fi

AC_SUBST(XMLSEC_NO_LIBXSLT)
AC_SUBST(LIBXSLT_CFLAGS)
AC_SUBST(LIBXSLT_LIBS)
AC_SUBST(LIBXSLT_CONFIG)
AC_SUBST(LIBXSLT_MIN_VERSION)
AC_SUBST(LIBXSLT_PC_FILE_COND)

dnl ==========================================================================
dnl See if we can find a crypto library
dnl ==========================================================================
XMLSEC_CRYPTO_LIST=""
XMLSEC_CRYPTO_DISABLED_LIST=""

dnl ==========================================================================
dnl OpenSSL
dnl ==========================================================================
ac_openssl_lib_dir="/usr/local/lib /usr/lib /usr/lib64 /usr/local /usr/local/ssl /usr/local/ssl/lib /usr/pkg"
ac_openssl_inc_dir="/usr/local/include /usr/include /usr/local /usr/local/ssl /usr/pkg /usr/local/ssl/include"

XMLSEC_NO_OPENSSL="1"
OPENSSL_MIN_VERSION="0.9.8"
OPENSSL_VERSION=""
OPENSSL_CFLAGS=""
OPENSSL_LIBS=""
OPENSSL_CRYPTO_LIB="$XMLSEC_PACKAGE-openssl"
OPENSSL_FOUND=no
AC_ARG_WITH(openssl, [  --with-openssl=[PFX]      openssl location])
if test "z$with_openssl" = "zno" ; then
    OPENSSL_FOUND=without
    AC_MSG_CHECKING(for openssl libraries >= $OPENSSL_MIN_VERSION) 
    AC_MSG_RESULT(no)
elif test "z$with_openssl" != "z" ; then
    case $host in
	*-*-mingw*) openssl_exlibs=;;
	*-*-osf5*) openssl_exlibs=;;
	*-*-openbsd*) openssl_exlibs=;;
	*-*-netbsd*) openssl_exlibs=;;
	#FIXME: check if lib "dl" is required
	*)          openssl_exlibs=-ldl;;
    esac
    OPENSSL_CFLAGS="$OPENSSL_CFLAGS -I$with_openssl/include"
    if test -f "$with_openssl/lib/libcrypto${XMLSEC_SHLIBSFX}" ; then
	OPENSSL_LIBS="-L$with_openssl/lib -lcrypto $openssl_exlibs"
    else
	OPENSSL_LIBS="$with_openssl/lib/libcrypto.a $openssl_exlibs"
    fi
    OPENSSL_FOUND="yes"
elif test "z$PKGCONFIG_FOUND" = "zyes" ; then
    if test "z$OPENSSL_VERSION" = "z" ; then
        PKG_CHECK_MODULES(OPENSSL, openssl >= 1.1.0,
            [OPENSSL_VERSION="1.1.0"],
            [OPENSSL_VERSION=""])
    fi

    if test "z$OPENSSL_VERSION" = "z" ; then
        PKG_CHECK_MODULES(OPENSSL, openssl >= 1.0.0,
            [OPENSSL_VERSION="1.0.0"],
            [OPENSSL_VERSION=""])
    fi

    if test "z$OPENSSL_VERSION" = "z" ; then
        PKG_CHECK_MODULES(OPENSSL, openssl >= 0.9.8,
    	    [OPENSSL_VERSION="0.9.8"],
	    [OPENSSL_VERSION=""])
    fi

    if test "z$OPENSSL_VERSION" = "z" ; then
        PKG_CHECK_MODULES(OPENSSL, openssl >= $OPENSSL_MIN_VERSION,
	    [OPENSSL_VERSION="$OPENSSL_MIN_VERSION"],
	    [OPENSSL_VERSION=""])
    fi

    if test "z$OPENSSL_VERSION" != "z" ; then
        OPENSSL_FOUND="yes"
    fi
fi

if test "z$OPENSSL_FOUND" = "zno" ; then
    OPENSSL_INCLUDES_FOUND="no"
    OPENSSL_LIBS_FOUND="no"
    
    for dir in $ac_openssl_inc_dir ; do
	if test -f $dir/openssl/ssl.h ; then
	    dnl do not add -I/usr/include because compiler does it anyway
	    if test "z$dir" = "z/usr/include" ; then
		OPENSSL_CFLAGS="$OPENSSL_CFLAGS "
	    else
		OPENSSL_CFLAGS="$OPENSSL_CFLAGS -I$dir"
	    fi
	    OPENSSL_INCLUDES_FOUND="yes"
	    break
	fi
    done

    for dir in $ac_openssl_lib_dir ; do
	if test -f $dir/libcrypto.a ; then
	    dnl do not add -L/usr/lib because compiler does it anyway
	    if test "z$dir" = "z/usr/lib" ; then
		OPENSSL_LIBS="-lcrypto -ldl"
	    else
		OPENSSL_LIBS="-L$dir -lcrypto -ldl"
	    fi
	    OPENSSL_LIBS_FOUND="yes"
	    ac_found_openssl_lib_dir=$dir
	    break;
	fi
    done
    
    if test "z$OPENSSL_INCLUDES_FOUND" = "zyes" -a "z$OPENSSL_LIBS_FOUND" = "zyes" ; then
	OPENSSL_FOUND="yes"
    fi
fi

if test "z$OPENSSL_FOUND" = "zyes" -a "z$OPENSSL_VERSION" = "z" ; then
    AC_MSG_CHECKING(for openssl libraries >= $OPENSSL_MIN_VERSION) 

    dnl Check the OpenSSL version    
    OLD_CPPFLAGS=$CPPFLAGS
    CPPFLAGS="$OPENSSL_CFLAGS"

    if test "z$OPENSSL_VERSION" = "z" ; then
        AC_EGREP_CPP(yes,[
            #include <openssl/opensslv.h>
        #if OPENSSL_VERSION_NUMBER >= 0x10100000L && !defined(LIBRESSL_VERSION_NUMBER)
        yes
        #endif
    ],[
        OPENSSL_VERSION="1.1.0"
    ],[
        OPENSSL_VERSION=""
    ])
    fi

    if test "z$OPENSSL_VERSION" = "z" ; then
        AC_EGREP_CPP(yes,[
    	    #include <openssl/opensslv.h>
	    #if OPENSSL_VERSION_NUMBER >= 0x10000000L
		yes
	    #endif
	],[
	    OPENSSL_VERSION="1.0.0"
	],[
	    OPENSSL_VERSION=""
	])
    fi

    if test "z$OPENSSL_VERSION" = "z" ; then
        AC_EGREP_CPP(yes,[
    	    #include <openssl/opensslv.h>
	    #if OPENSSL_VERSION_NUMBER >= 0x00908000L
		yes
	    #endif
	],[
	    OPENSSL_VERSION="0.9.8"
	],[
	    OPENSSL_VERSION=""
	])
    fi

    if test "z$OPENSSL_VERSION" = "z" ; then
        AC_MSG_RESULT(no)
    else
        AC_MSG_RESULT([yes ('$OPENSSL_VERSION')])
    fi
    
    CPPFLAGS=$OLD_CPPFLAGS
fi    

if test "z$OPENSSL_FOUND" = "zyes" ; then
    dnl Check availability of ECDSA
    OLD_CPPFLAGS=$CPPFLAGS
    CPPFLAGS="$OPENSSL_CFLAGS"

    AC_EGREP_CPP(yes,[
	#include <openssl/opensslconf.h>
	#ifdef OPENSSL_NO_ECDSA
	    yes
	#endif
    ],[
	OPENSSL_CFLAGS="$OPENSSL_CFLAGS -DXMLSEC_NO_ECDSA=1"
    ]) 
    CPPFLAGS=$OLD_CPPFLAGS
fi

if test "z$OPENSSL_FOUND" = "zyes" ; then
    XMLSEC_NO_OPENSSL="0"
    if test "z$OPENSSL_VERSION" = "z0.9.8" ; then
	OPENSSL_CFLAGS="$OPENSSL_CFLAGS -DXMLSEC_OPENSSL_098=1 -DXMLSEC_NO_ECDSA=1 -DXMLSEC_NO_SHA256=1"
    fi
    if test "z$OPENSSL_VERSION" = "z1.0.0" ; then
	OPENSSL_CFLAGS="$OPENSSL_CFLAGS -DXMLSEC_OPENSSL_100=1"
    fi    
    if test "z$OPENSSL_VERSION" = "z1.1.0" ; then
    	OPENSSL_CFLAGS="$OPENSSL_CFLAGS -DXMLSEC_OPENSSL_110=1"
    fi    
    OPENSSL_CFLAGS="$OPENSSL_CFLAGS -DXMLSEC_CRYPTO_OPENSSL=1"
    XMLSEC_CRYPTO_LIST="$XMLSEC_CRYPTO_LIST openssl"
else
    XMLSEC_CRYPTO_DISABLED_LIST="$XMLSEC_CRYPTO_DISABLED_LIST openssl"
fi

AM_CONDITIONAL(XMLSEC_NO_OPENSSL, test "z$XMLSEC_NO_OPENSSL" == "z1")
AC_SUBST(XMLSEC_NO_OPENSSL)
AC_SUBST(OPENSSL_CFLAGS)
AC_SUBST(OPENSSL_LIBS)
AC_SUBST(OPENSSL_CRYPTO_LIB)
AC_SUBST(OPENSSL_MIN_VERSION)

dnl ==========================================================================
dnl See if we can find NSS and NSPR
dnl Allow the use of:
dnl   1.  Mozilla style distribution where all mozilla components (NSS, NSPR
dnl       are just 2 of over 100 components) are under one directory.
dnl       options: [--with-mozilla-ver=<VERSION>]
dnl
dnl       Using the version number, various directories are searched
dnl       for NSS & NSPR
dnl
dnl                      AND / OR
dnl
dnl   2.  separate NSS & NSPR distributions
dnl       options: [--with-nss=<PFX>] [--with-nspr=<PFX>]
dnl   
dnl   2 overrides 1
dnl
dnl ==========================================================================
XMLSEC_NO_NSS="1"
SEAMONKEY_MIN_VERSION="1.0"
MOZILLA_MIN_VERSION="1.4"
NSS_MIN_VERSION="3.9"
NSPR_MIN_VERSION="4.4.1"
NSS_CFLAGS=""
NSS_LIBS=""
NSS_LIBS_LIST="-lnss3 -lsmime3"
NSPR_LIBS_LIST="-lnspr4 -lplds4 -lplc4"
NSS_CRYPTO_LIB="$XMLSEC_PACKAGE-nss"
NSS_FOUND="no"
NSPR_PACKAGE=mozilla-nspr
NSS_PACKAGE=mozilla-nss


dnl if nss is disabled, bail out early
AC_ARG_WITH(nss,     [  --with-nss=[PFX]          nss location])
AC_ARG_WITH(nspr,    [  --with-nspr=[PFX]         nspr location (needed for NSS)])
AC_ARG_WITH(seamonkey_ver, [  --with-seamonkey-ver=[VER]  mozilla version (alt to --with-nss, --with-nspr)])
AC_ARG_WITH(mozilla_ver, [  --with-mozilla-ver=[VER]  mozilla version (alt to --with-nss, --with-nspr)])
if test "z$with_nss" = "zno" -o "z$with_nspr" = "zno" ; then
    AC_MSG_CHECKING(for NSS libraries) 
    AC_MSG_RESULT(no)
    NSS_FOUND="without"
elif test "z$with_nss" = "z" -a "z$with_nspr" = "z" -a "z$with_mozilla_ver" = "z" -a "z$with_seamonkey_ver" = "z" -a "z$PKGCONFIG_FOUND" = "zyes" ; then
    dnl
    dnl Mozilla's NSS/NSPR are distributed under different names
    dnl in different distribution:
    dnl         seamonkey-nspr and seamonkey-nss
    dnl 	mozilla-nspr and mozilla-nss
    dnl		xulrunner-nspr and xulrunner-nss
    dnl		nspr and nss
    dnl We are going to try all options
    dnl
    if test "z$NSS_FOUND" = "zno" ; then
        PKG_CHECK_MODULES(NSS, seamonkey-nspr >= $NSPR_MIN_VERSION seamonkey-nss >= $SEAMONKEY_MIN_VERSION,
    	    [NSS_FOUND=yes NSPR_PACKAGE=seamonkey-nspr NSS_PACKAGE=seamonkey-nss],
	    [NSS_FOUND=no])
    fi
    if test "z$NSS_FOUND" = "zno" ; then
        PKG_CHECK_MODULES(NSS, mozilla-nspr >= $MOZILLA_MIN_VERSION mozilla-nss >= $MOZILLA_MIN_VERSION,
    	    [NSS_FOUND=yes NSPR_PACKAGE=mozilla-nspr NSS_PACKAGE=mozilla-nss],
	    [NSS_FOUND=no])
    fi
    if test "z$NSS_FOUND" = "zno" ; then
        PKG_CHECK_MODULES(NSS, xulrunner-nspr >= $NSPR_MIN_VERSION xulrunner-nss >= $NSS_MIN_VERSION,
    	    [NSS_FOUND=yes NSPR_PACKAGE=xulrunner-nspr NSS_PACKAGE=xulrunner-nss],
	    [NSS_FOUND=no])
    fi
    if test "z$NSS_FOUND" = "zno" ; then
        PKG_CHECK_MODULES(NSS, nspr >= $NSPR_MIN_VERSION nss >= $NSS_MIN_VERSION,
    	    [NSS_FOUND=yes NSPR_PACKAGE=nspr NSS_PACKAGE=nss],
	    [NSS_FOUND=no])
    fi
fi

if test "z$NSS_FOUND" = "zno" ; then 
    dnl process the mozilla options if any
    if test "z$with_seamonkey_ver" != "z" ; then
        ac_mozilla_name=seamonkey-$with_seamonkey_ver
    elif test "z$with_mozilla_ver" != "z" ; then
        ac_mozilla_name=mozilla-$with_mozilla_ver
    else
        ac_mozilla_name=mozilla-$MOZILLA_MIN_VERSION
    fi

    ac_nss_lib_dir="/usr/lib /usr/lib64 /usr/local/lib /usr/lib/$ac_mozilla_name /usr/local/lib/$ac_mozilla_name"
    ac_nss_inc_dir="/usr/include /usr/include/mozilla /usr/local/include /usr/local/include/mozilla /usr/include/$ac_mozilla_name /usr/local/include/$ac_mozilla_name"

    AC_MSG_CHECKING(for nspr libraries >= $NSPR_MIN_VERSION)
    NSPR_INCLUDES_FOUND="no"
    NSPR_LIBS_FOUND="no"
    NSPR_FOUND="no"
    NSPR_PRINIT_H=""

    if test "z$with_nspr" != "z" ; then
	NSPR_PREFIX="$with_nspr"
	NSPR_CFLAGS="-I$with_nspr/include -I$with_nspr/include/nspr"
	if test "z$with_gnu_ld" = "zyes" ; then
	    NSPR_LIBS="-Wl,-rpath-link -Wl,$with_nspr/lib -L$with_nspr/lib $NSPR_LIBS_LIST"
	else
	    NSPR_LIBS="-L$with_nspr/lib $NSPR_LIBS_LIST"
	fi
	NSPR_INCLUDES_FOUND="yes"
	NSPR_LIBS_FOUND="yes"
	NSPR_PRINIT_H="$with_nspr/include/prinit.h"
    else
	for dir in $ac_nss_inc_dir ; do
    	    if test -f $dir/nspr/prinit.h ; then
    		dnl do not add -I/usr/include because compiler does it anyway
        	if test "z$dir" = "z/usr/include" ; then
    		    NSPR_CFLAGS=""
    		else
    		    NSPR_CFLAGS="-I$dir/nspr"
    		fi
		NSPR_INCLUDES_FOUND="yes"
		NSPR_PRINIT_H="$dir/nspr/prinit.h"
    		break
    	    fi
	done
	
	for dir in $ac_nss_lib_dir ; do
    	    if test -f $dir/libnspr4$shrext ; then
		dnl do not add -L/usr/lib because compiler does it anyway
        	if test "z$dir" = "z/usr/lib" ; then
            	    NSPR_LIBS="$NSPR_LIBS_LIST"
    		else
    		    if test "z$with_gnu_ld" = "zyes" ; then
    			NSPR_LIBS="-Wl,-rpath-link -Wl,$dir -L$dir $NSPR_LIBS_LIST"
    		    else
    			NSPR_LIBS="-L$dir $NSPR_LIBS_LIST"
		    fi
		fi
		NSPR_LIBS_FOUND="yes"
		break
	    fi
	done
    fi

    if test "z$NSPR_INCLUDES_FOUND" = "zyes" -a "z$NSPR_LIBS_FOUND" = "zyes" ; then
	OLD_CPPFLAGS=$CPPFLAGS
	CPPFLAGS="$NSPR_CFLAGS"
	AC_EGREP_CPP(yes,[
    	    #include <prinit.h>
            #if PR_VMAJOR >= 4
               yes
            #endif
	],[
	    NSPR_FOUND=yes
    	],[
	    NSPR_FOUND=no
        ]) 
	CPPFLAGS="$OLD_CPPFLAGS"
    fi
    
    if test "z$NSPR_FOUND" = "zyes" ; then    
	AC_MSG_RESULT(yes)	
    else
	AC_MSG_RESULT(no)
    fi

    dnl look for nss
    AC_MSG_CHECKING(for nss libraries >= $NSS_MIN_VERSION)
    NSS_INCLUDES_FOUND="no"
    NSS_LIBS_FOUND="no"
    NSS_NSS_H=""

    if test "z$with_nss" != "z" ; then
	NSS_CFLAGS="$NSS_CFLAGS -I$with_nss/include -I$with_nss/include/nss"
	if test "z$with_gnu_ld" = "zyes" ; then
	    NSS_LIBS="$NSS_LIBS -Wl,-rpath-link -Wl,$with_nss/lib -L$with_nss/lib $NSS_LIBS_LIST"
        else
	    NSS_LIBS="$NSS_LIBS -L$with_nss/lib $NSS_LIBS_LIST"
        fi
	NSS_INCLUDES_FOUND="yes"
	NSS_LIBS_FOUND="yes"
	NSS_NSS_H="$with_nss/include/nss.h"
    else
	for dir in $ac_nss_inc_dir ; do
	    if test -f $dir/nss/nss.h ; then
        	dnl do not add -I/usr/include because compiler does it anyway
    		if test "z$dir" = "z/usr/include" ; then
        	    NSS_CFLAGS="$NSS_CFLAGS"
		else
            	    NSS_CFLAGS="$NSS_CFLAGS -I$dir/nss"
		fi
    		NSS_INCLUDES_FOUND="yes"
    		NSS_NSS_H="$dir/nss/nss.h"
    		break
    	    fi
        done
       
        for dir in $ac_nss_lib_dir ; do
    	    if test -f $dir/libnss3$shrext ; then
        	dnl do not add -L/usr/lib because compiler does it anyway
    		if test "z$dir" = "z/usr/lib" ; then
        	    NSS_LIBS="$NSS_LIBS_LIST"
                else
            	    if test "z$with_gnu_ld" = "zyes" ; then
    			NSS_LIBS="-Wl,-rpath-link -Wl,$dir -L$dir $NSS_LIBS_LIST"
            	    else
        		NSS_LIBS="-L$dir $NSS_LIBS_LIST"
		    fi
		fi
    		NSS_LIBS_FOUND="yes"
    		break
	    fi
	done
    fi

    if test "z$NSS_INCLUDES_FOUND" = "zyes" -a "z$NSS_LIBS_FOUND" = "zyes" ; then
       OLD_CPPFLAGS=$CPPFLAGS
       CPPFLAGS="$NSPR_CFLAGS $NSS_CFLAGS"
       AC_EGREP_CPP(yes,[
          #include <nss.h>
          #if NSS_VMAJOR >= 3 && NSS_VMINOR >= 2
             yes
          #endif
	],[
	    NSS_FOUND=yes
    	],[
	    NSS_FOUND=no
        ]) 
	CPPFLAGS="$OLD_CPPFLAGS"
    fi
    
    if test "z$NSS_FOUND" = "zyes" ; then    
	AC_MSG_RESULT(yes)	
    else
	AC_MSG_RESULT(no)
    fi

    dnl check that we have found both NSS and NSPR
    if test "z$NSPR_FOUND" = "zyes" -a "z$NSS_FOUND" = "zyes" ; then
	NSS_CFLAGS="$NSS_CFLAGS $NSPR_CFLAGS"
	NSS_LIBS="$NSS_LIBS $NSPR_LIBS"
    elif test "z$NSPR_FOUND" != "zyes" -a "z$NSS_FOUND" = "zyes" ; then    
	NSS_FOUND="no"
	AC_MSG_RESULT(NSPR library is required for NSS, ignoring NSS library we have found)
    fi
fi

if test "z$NSS_FOUND" = "zyes" ; then
    XMLSEC_NO_NSS="0"
    NSS_CFLAGS="$NSS_CFLAGS -DXMLSEC_CRYPTO_NSS=1"
    XMLSEC_CRYPTO_LIST="$XMLSEC_CRYPTO_LIST nss"
else
    XMLSEC_CRYPTO_DISABLED_LIST="$XMLSEC_CRYPTO_DISABLED_LIST nss"
fi

AM_CONDITIONAL(XMLSEC_NO_NSS, test "z$XMLSEC_NO_NSS" = "z1")
AC_SUBST(XMLSEC_NO_NSS)
AC_SUBST(NSPR_PACKAGE)
AC_SUBST(NSS_PACKAGE)
AC_SUBST(NSS_CFLAGS)
AC_SUBST(NSS_LIBS)
AC_SUBST(NSS_CRYPTO_LIB)
AC_SUBST(NSS_MIN_VERSION)
AC_SUBST(NSPR_MIN_VERSION)
AC_SUBST(MOZILLA_MIN_VERSION)


dnl ==========================================================================
dnl See if we can find GCrypt
dnl ==========================================================================
GCRYPT_CONFIG="libgcrypt-config"
XMLSEC_NO_GCRYPT="1"
GCRYPT_MIN_VERSION="1.4.0"
GCRYPT_VERSION=""
GCRYPT_CFLAGS=""
GCRYPT_LIBS=""
GCRYPT_CRYPTO_LIB="$XMLSEC_PACKAGE-gcrypt"
GCRYPT_FOUND="no"
AC_ARG_WITH(gcrypt, [  --with-gcrypt=[PFX]       gcrypt location])
if test "z$with_gcrypt" = "zno" ; then
    GCRYPT_FOUND="without"
elif test "z$with_gcrypt" = "z" -a "z$PKGCONFIG_FOUND" = "zyes" ; then
    PKG_CHECK_MODULES(GCRYPT, gcrypt >= $GCRYPT_MIN_VERSION,
	[GCRYPT_FOUND=yes],
	[GCRYPT_FOUND=no])

    if test "z$GCRYPT_FOUND" = "zno" ; then
        PKG_CHECK_MODULES(GCRYPT, libgcrypt >= $GCRYPT_MIN_VERSION,
	    [GCRYPT_FOUND=yes],
	    [GCRYPT_FOUND=no])
    fi

    if test "z$GCRYPT_FOUND" = "zno" ; then
        PKG_CHECK_MODULES(GCRYPT, libgcrypt11 >= $GCRYPT_MIN_VERSION,
	    [GCRYPT_FOUND=yes],
	    [GCRYPT_FOUND=no])
    fi
fi

if test "z$GCRYPT_FOUND" = "zno" ; then
    if test "z$with_gcrypt" != "z" ; then
        GCRYPT_CONFIG=$with_gcrypt/bin/$GCRYPT_CONFIG
    fi
    if ! $GCRYPT_CONFIG --version > /dev/null 2>&1 ; then
	if test "z$with_gcrypt" != "z" ; then
	    AC_MSG_ERROR(Unable to find gcrypt at '$with_gcrypt')
	else
	    AC_MSG_RESULT(no)
	fi
    else
        vers=`$GCRYPT_CONFIG --version | awk -F. '{ printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`
	minvers=`echo $GCRYPT_MIN_VERSION | awk -F. '{ printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`
	if test "$vers" -ge "$minvers" ; then
	    GCRYPT_CFLAGS="$GCRYPT_CFLAGS `$GCRYPT_CONFIG --cflags`"
	    GCRYPT_LIBS="$GCRYPT_LIBS `$GCRYPT_CONFIG --libs`"
	    GCRYPT_FOUND=yes
	else
	    AC_MSG_ERROR(You need at least gcrypt $GCRYPT_MIN_VERSION for this version of $XMLSEC_PACKAGE)
	fi	
    fi
fi

AC_MSG_CHECKING(for gcrypt libraries >= $GCRYPT_MIN_VERSION) 
if test "z$GCRYPT_FOUND" = "zyes" ; then 
    XMLSEC_NO_GCRYPT="0"
    GCRYPT_CFLAGS="$GCRYPT_CFLAGS -DXMLSEC_CRYPTO_GCRYPT=1"
    XMLSEC_CRYPTO_LIST="$XMLSEC_CRYPTO_LIST gcrypt"
    AC_MSG_RESULT(yes)
else
    XMLSEC_CRYPTO_DISABLED_LIST="$XMLSEC_CRYPTO_DISABLED_LIST gcrypt"
    AC_MSG_RESULT(no)
fi

AM_CONDITIONAL(XMLSEC_NO_GCRYPT, test "z$XMLSEC_NO_GCRYPT" = "z1")
AC_SUBST(XMLSEC_NO_GCRYPT)
AC_SUBST(GCRYPT_CFLAGS)
AC_SUBST(GCRYPT_LIBS)
AC_SUBST(GCRYPT_CRYPTO_LIB)
AC_SUBST(GCRYPT_MIN_VERSION)


dnl ==========================================================================
dnl See if we can find GnuTLS
dnl ==========================================================================
XMLSEC_NO_GNUTLS="1"
GNUTLS_MIN_VERSION="2.8.0"
GNUTLS_VERSION=""
GNUTLS_CFLAGS=""
GNUTLS_LIBS=""
GNUTLS_CRYPTO_LIB="$XMLSEC_PACKAGE-gnutls"
GNUTLS_FOUND="no"
AC_ARG_WITH(gnutls, [  --with-gnutls=[PFX]       gnutls location])
if test "z$with_gnutls" = "zno" ; then
    GNUTLS_FOUND="without"
elif test "z$with_gnutls" = "z" -a "z$PKGCONFIG_FOUND" = "zyes" ; then
    PKG_CHECK_MODULES(GNUTLS, gnutls >= $GNUTLS_MIN_VERSION,
	[GNUTLS_FOUND=yes],
	[GNUTLS_FOUND=no])
    if test "z$GNUTLS_FOUND" = "zno" ; then 
        PKG_CHECK_MODULES(GNUTLS, libgnutls >= $GNUTLS_MIN_VERSION,
	    [GNUTLS_FOUND=yes],
	    [GNUTLS_FOUND=no])
    fi
fi

if test "z$GNUTLS_FOUND" = "zno" ; then
    SAVE_CFLAGS="$CFLAGS"
    SAVE_LDFLAGS="$LDFLAGS"
    if test "z$with_gnutls" != "z" -a "z$with_gnutls" != "zno" ; then
        GNUTLS_CFLAGS="-I$with_gnutls/include"
        GNUTLS_LIBS="-L$with_gnutls/lib -lgnutls"
        CFLAGS="$GNUTLS_CFLAGS $CFLAGS"
        LDFLAGS="$GNUTLS_LIBS $LDFLAGS"
    fi
    AC_CHECK_HEADERS([gnutls/gnutls.h],[
        AC_CHECK_LIB([gnutls], [gnutls_check_version], [ GNUTLS_FOUND=yes ])
    ], [], [AC_INCLUDES_DEFAULT])
    if test "z$with_gnutls" != "z" -a "z$with_gnutls" != "zno" -a "z$GNUTLS_FOUND" != "zyes" ; then
        AC_MSG_ERROR([can't find usable libgnutls at specified location "$with_gnutls"])
    fi
    CFLAGS="$SAVE_CFLAGS"
    LDFLAGS="$SAVE_LDFLAGS"
fi

AC_MSG_CHECKING(for gnutls libraries >= $GNUTLS_MIN_VERSION)
if test "z$GNUTLS_FOUND" = "zyes" ; then 
    XMLSEC_NO_GNUTLS="0"
    GNUTLS_CFLAGS="$GNUTLS_CFLAGS -DXMLSEC_CRYPTO_GNUTLS=1"
    XMLSEC_CRYPTO_LIST="$XMLSEC_CRYPTO_LIST gnutls"

    dnl xmlsec-gnutls is using xmlsec-gcrypt
    if test "z$GCRYPT_FOUND" != "zyes" ; then
        AC_MSG_ERROR(xmlsec-gnutls library requires xmlsec-gcrypt library which is disabled or missing)
    fi
    AC_MSG_RESULT(yes)
else
    XMLSEC_CRYPTO_DISABLED_LIST="$XMLSEC_CRYPTO_DISABLED_LIST gnutls"
    AC_MSG_RESULT(no)
fi

AM_CONDITIONAL(XMLSEC_NO_GNUTLS, test "z$XMLSEC_NO_GNUTLS" = "z1")
AC_SUBST(XMLSEC_NO_GNUTLS)
AC_SUBST(GNUTLS_CFLAGS)
AC_SUBST(GNUTLS_LIBS)
AC_SUBST(GNUTLS_CRYPTO_LIB)
AC_SUBST(GNUTLS_MIN_VERSION)


dnl ==========================================================================
dnl See if we can find MSCrypto
dnl ==========================================================================
XMLSEC_NO_MSCRYPTO="1"
MSCRYPTO_CFLAGS=""
MSCRYPTO_LIBS=""
MSCRYPTO_CRYPTO_LIB="$XMLSEC_PACKAGE-mscrypto"
MSCRYPTO_ENABLE='none'
AC_ARG_ENABLE(mscrypto,
	[  --enable-mscrypto       enable mscrypto (no)],
	[MSCRYPTO_ENABLE=$enableval])
if test "z$MSCRYPTO_ENABLE" != "zyes" ; then
    AC_MSG_CHECKING(for mscrypto libraries)
    AC_MSG_RESULT([$MSCRYPTO_ENABLE])
else
dnl cannot detect __stdcall functions
dnl    AC_CHECK_LIB(crypt32, CertOpenStore, ....
    LIBS_SAVE="$LIBS"
    LIBS="$LIBS -lcrypt32"
    AC_MSG_CHECKING(for mscrypto libraries)
    AC_LINK_IFELSE([AC_LANG_PROGRAM([
	#include <windows.h>
	#include <wincrypt.h>],[
	int main () { CertOpenStore(0,0,0,0,0);; return(0); }
	])],
	[],
	[MSCRYPTO_ENABLE="no"])
    AC_MSG_RESULT([$MSCRYPTO_ENABLE])
    LIBS="$LIBS_SAVE"
fi

if test "z$MSCRYPTO_ENABLE" = "zyes" ; then
    XMLSEC_NO_MSCRYPTO="0"

    MSCRYPTO_CFLAGS="$MSCRYPTO_CFLAGS -DXMLSEC_CRYPTO_MSCRYPTO=1"
    case $host in
	*-*-mingw*)
		dnl since mingw crypt32 library is limited
		dnl we use own def-file
		MSCRYPTO_LIBS='-Wl,$(srcdir)/mingw-crypt32.def';;
	*)
		MSCRYPTO_LIBS="-lcrypt32";;
    esac

    XMLSEC_CRYPTO_LIST="$XMLSEC_CRYPTO_LIST mscrypto"
else
    XMLSEC_CRYPTO_DISABLED_LIST="$XMLSEC_CRYPTO_DISABLED_LIST mscrypto"
fi

AM_CONDITIONAL(XMLSEC_NO_MSCRYPTO, [test "z$XMLSEC_NO_MSCRYPTO" = "z1"])
AC_SUBST(XMLSEC_NO_MSCRYPTO)
AC_SUBST(MSCRYPTO_CFLAGS)
AC_SUBST(MSCRYPTO_LIBS)
AC_SUBST(MSCRYPTO_CRYPTO_LIB)


dnl ==========================================================================
dnl Figure out the default crypt - the first crypto library wins
dnl ==========================================================================
XMLSEC_DEFAULT_CRYPTO=""
XMLSEC_CRYPTO_LIB=""
XMLSEC_CRYPTO_CFLAGS=""
XMLSEC_CRYPTO_LIBS=""
AC_MSG_CHECKING(for default crypto library)
AC_ARG_WITH(default_crypto, [  --with-default-crypto=name default crypto name])

# check the argument
case "z$with_default_crypto" in
    'zmscrypto')
        if test "z$XMLSEC_NO_MSCRYPTO" != "z1" ; then
            XMLSEC_DEFAULT_CRYPTO="mscrypto"
        else
            AC_MSG_ERROR('$with_default_crypto' is specified as default crypto library but it is not configured or found)
        fi
        ;;
    'zopenssl')
        if test "z$XMLSEC_NO_OPENSSL" != "z1" ; then
            XMLSEC_DEFAULT_CRYPTO="openssl"
        else
            AC_MSG_ERROR('$with_default_crypto' is specified as default crypto library but it is not configured or found)
        fi
        ;;
    'znss')
        if test "z$XMLSEC_NO_NSS" != "z1" ; then
            XMLSEC_DEFAULT_CRYPTO="nss"
        else
            AC_MSG_ERROR('$with_default_crypto' is specified as default crypto library but it is not configured or found)
        fi
        ;;
    'zgnutls')
        if test "z$XMLSEC_NO_GNUTLS" != "z1" ; then
            XMLSEC_DEFAULT_CRYPTO="gnutls"
        else
            AC_MSG_ERROR('$with_default_crypto' is specified as default crypto library but it is not configured or found)
        fi
        ;;
    'zgcrypt')
        if test "z$XMLSEC_NO_GCRYPT" != "z1" ; then
            XMLSEC_DEFAULT_CRYPTO="gcrypt"
        else
            AC_MSG_ERROR('$with_default_crypto' is specified as default crypto library but it is not configured or found)
        fi
        ;;
    'z')
        dnl The first crypto library wins
        if test "z$XMLSEC_NO_MSCRYPTO" != "z1" ; then
            XMLSEC_DEFAULT_CRYPTO="mscrypto"
        elif test "z$XMLSEC_NO_OPENSSL" != "z1" ; then
            XMLSEC_DEFAULT_CRYPTO="openssl"
        elif test "z$XMLSEC_NO_NSS" != "z1" ; then
            XMLSEC_DEFAULT_CRYPTO="nss"
        elif test "z$XMLSEC_NO_GNUTLS" != "z1" ; then
            XMLSEC_DEFAULT_CRYPTO="gnutls"
        elif test "z$XMLSEC_NO_GCRYPT" != "z1" ; then
            XMLSEC_DEFAULT_CRYPTO="gcrypt"
        else
            AC_MSG_ERROR(At least one crypto library should exist for $XMLSEC_PACKAGE)
        fi
        ;;
    *)
        AC_MSG_ERROR(The value '$with_default_crypto' is not a recongnized crypto library name)
        ;;
esac

dnl Set the flags for default crypto lib
case "$XMLSEC_DEFAULT_CRYPTO" in
    'mscrypto')
        XMLSEC_CRYPTO_LIB="$MSCRYPTO_CRYPTO_LIB"
        XMLSEC_CRYPTO_CFLAGS="$MSCRYPTO_CFLAGS"
        XMLSEC_CRYPTO_LIBS="$MSCRYPTO_LIBS"
        ;;
    'openssl')
        XMLSEC_CRYPTO_LIB="$OPENSSL_CRYPTO_LIB"
        XMLSEC_CRYPTO_CFLAGS="$OPENSSL_CFLAGS"
        XMLSEC_CRYPTO_LIBS="$OPENSSL_LIBS"
        ;;
    'nss')
        XMLSEC_CRYPTO_LIB="$NSS_CRYPTO_LIB"
        XMLSEC_CRYPTO_CFLAGS="$NSS_CFLAGS"
        XMLSEC_CRYPTO_LIBS="$NSS_LIBS"
        ;;
    'gnutls')
        XMLSEC_CRYPTO_LIB="$GNUTLS_CRYPTO_LIB"
        XMLSEC_CRYPTO_CFLAGS="$GNUTLS_CFLAGS"
        XMLSEC_CRYPTO_LIBS="$GNUTLS_LIBS"
        ;;
    'gcrypt')
        XMLSEC_CRYPTO_LIB="$GCRYPT_CRYPTO_LIB"
        XMLSEC_CRYPTO_CFLAGS="$GCRYPT_CFLAGS"
        XMLSEC_CRYPTO_LIBS="$GCRYPT_LIBS"
        ;;
    *)
        AC_MSG_ERROR(The value \"$XMLSEC_DEFAULT_CRYPTO\" is not a recongnized crypto library name)
        ;;
esac
AC_MSG_RESULT(yes ('$XMLSEC_DEFAULT_CRYPTO'))

dnl ==========================================================================
dnl See do we need MD5 support
dnl ==========================================================================
AC_MSG_CHECKING(for MD5 support) 
AC_ARG_ENABLE(md5,   [  --enable-md5            enable MD5 support (yes)])
if test "z$enable_md5" = "zno" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_MD5=1"
    XMLSEC_NO_MD5="1"
    AC_MSG_RESULT(no)
else
    XMLSEC_NO_MD5="0"
    AC_MSG_RESULT(yes)
fi    
AM_CONDITIONAL(XMLSEC_NO_MD5, test "z$XMLSEC_NO_MD5" = "z1")
AC_SUBST(XMLSEC_NO_MD5)

dnl ==========================================================================
dnl See do we need RIPEMD-160 support
dnl ==========================================================================
AC_MSG_CHECKING(for RIPEMD-160 support) 
AC_ARG_ENABLE(ripemd160,   [  --enable-ripemd160      enable RIPEMD-160 support (yes)])
if test "z$enable_ripemd160" = "zno" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_RIPEMD160=1"
    XMLSEC_NO_RIPEMD160="1"
    AC_MSG_RESULT(no)
else
    XMLSEC_NO_RIPEMD160="0"
    AC_MSG_RESULT(yes)
fi    
AM_CONDITIONAL(XMLSEC_NO_RIPEMD160, test "z$XMLSEC_NO_RIPEMD160" = "z1")
AC_SUBST(XMLSEC_NO_RIPEMD160)

dnl ==========================================================================
dnl See do we need SHA1 support
dnl ==========================================================================
AC_MSG_CHECKING(for SHA1 support) 
AC_ARG_ENABLE(sha1,   [  --enable-sha1           enable SHA1 support (yes)])
if test "z$enable_sha1" = "zno" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_SHA1=1"
    XMLSEC_NO_SHA1="1"
    AC_MSG_RESULT(no)
else
    XMLSEC_NO_SHA1="0"
    AC_MSG_RESULT(yes)
fi    
AM_CONDITIONAL(XMLSEC_NO_SHA1, test "z$XMLSEC_NO_SHA1" = "z1")
AC_SUBST(XMLSEC_NO_SHA1)

dnl ==========================================================================
dnl See do we need SHA224 support
dnl ==========================================================================
AC_MSG_CHECKING(for SHA224 support) 
AC_ARG_ENABLE(sha224,   [  --enable-sha224         enable SHA224 support (yes)])
if test "z$enable_sha224" = "zno" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_SHA224=1"
    XMLSEC_NO_SHA224="1"
    AC_MSG_RESULT(no)
else
    XMLSEC_NO_SHA224="0"
    AC_MSG_RESULT(yes)
fi    
AM_CONDITIONAL(XMLSEC_NO_SHA224, test "z$XMLSEC_NO_SHA224" = "z1")
AC_SUBST(XMLSEC_NO_SHA224)

dnl ==========================================================================
dnl See do we need SHA256 support
dnl ==========================================================================
AC_MSG_CHECKING(for SHA256 support) 
AC_ARG_ENABLE(sha256,   [  --enable-sha256         enable SHA256 support (yes)])
if test "z$enable_sha256" = "zno" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_SHA256=1"
    XMLSEC_NO_SHA256="1"
    AC_MSG_RESULT(no)
else
    XMLSEC_NO_SHA256="0"
    AC_MSG_RESULT(yes)
fi    
AM_CONDITIONAL(XMLSEC_NO_SHA256, test "z$XMLSEC_NO_SHA256" = "z1")
AC_SUBST(XMLSEC_NO_SHA256)

dnl ==========================================================================
dnl See do we need SHA384 support
dnl ==========================================================================
AC_MSG_CHECKING(for SHA384 support) 
AC_ARG_ENABLE(sha384,   [  --enable-sha384         enable SHA384 support (yes)])
if test "z$enable_sha384" = "zno" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_SHA384=1"
    XMLSEC_NO_SHA384="1"
    AC_MSG_RESULT(no)
else
    XMLSEC_NO_SHA384="0"
    AC_MSG_RESULT(yes)
fi    
AM_CONDITIONAL(XMLSEC_NO_SHA384, test "z$XMLSEC_NO_SHA384" = "z1")
AC_SUBST(XMLSEC_NO_SHA384)

dnl ==========================================================================
dnl See do we need SHA512 support
dnl ==========================================================================
AC_MSG_CHECKING(for SHA512 support) 
AC_ARG_ENABLE(sha512,   [  --enable-sha512         enable SHA512 support (yes)])
if test "z$enable_sha512" = "zno" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_SHA512=1"
    XMLSEC_NO_SHA512="1"
    AC_MSG_RESULT(no)
else
    XMLSEC_NO_SHA512="0"
    AC_MSG_RESULT(yes)
fi    
AM_CONDITIONAL(XMLSEC_NO_SHA512, test "z$XMLSEC_NO_SHA512" = "z1")
AC_SUBST(XMLSEC_NO_SHA512)


dnl ==========================================================================
dnl See do we need HMAC support
dnl ==========================================================================
AC_MSG_CHECKING(for HMAC support) 
AC_ARG_ENABLE(hmac,   [  --enable-hmac           enable HMAC support (yes)])
if test "z$enable_hmac" = "zno" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_HMAC=1"
    XMLSEC_NO_HMAC="1"
    AC_MSG_RESULT(no)
else
    XMLSEC_NO_HMAC="0"
    AC_MSG_RESULT(yes)
fi    
AM_CONDITIONAL(XMLSEC_NO_HMAC, test "z$XMLSEC_NO_HMAC" = "z1")
AC_SUBST(XMLSEC_NO_HMAC)

dnl ==========================================================================
dnl See do we need DSA support
dnl ==========================================================================
AC_MSG_CHECKING(for DSA support) 
AC_ARG_ENABLE(dsa,   [  --enable-dsa            enable DSA support (yes)])
if test "z$enable_dsa" = "zno" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_DSA=1"
    XMLSEC_NO_DSA="1"
    AC_MSG_RESULT(no)
else
    XMLSEC_NO_DSA="0"
    AC_MSG_RESULT(yes)
fi    
AM_CONDITIONAL(XMLSEC_NO_DSA, test "z$XMLSEC_NO_DSA" = "z1")
AC_SUBST(XMLSEC_NO_DSA)

dnl ==========================================================================
dnl See do we need RSA support
dnl ==========================================================================
AC_MSG_CHECKING(for RSA support) 
AC_ARG_ENABLE(rsa,   [  --enable-rsa            enable RSA support (yes)])
if test "z$enable_rsa" = "zno" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_RSA=1"
    XMLSEC_NO_RSA="1"
    AC_MSG_RESULT(no)
else
    XMLSEC_NO_RSA="0"
    AC_MSG_RESULT(yes)
fi    
AM_CONDITIONAL(XMLSEC_NO_RSA, test "z$XMLSEC_NO_RSA" = "z1")
AC_SUBST(XMLSEC_NO_RSA)

dnl ==========================================================================
dnl See do we need x509 support
dnl ==========================================================================
AC_MSG_CHECKING(for x509 support) 
AC_ARG_ENABLE(x509,   [  --enable-x509           enable x509 support (yes)])
if test "z$enable_x509" = "zno" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_X509=1"
    XMLSEC_NO_X509="1"
    AC_MSG_RESULT(no)
else
    XMLSEC_NO_X509="0"
    AC_MSG_RESULT(yes)
fi    
AM_CONDITIONAL(XMLSEC_NO_X509, test "z$XMLSEC_NO_X509" = "z1")
AC_SUBST(XMLSEC_NO_X509)

dnl ==========================================================================
dnl See do we need DES support
dnl ==========================================================================
AC_MSG_CHECKING(for DES support) 
AC_ARG_ENABLE(des,   [  --enable-des            enable DES support (yes)])
if test "z$enable_des" = "zno" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_DES=1"
    XMLSEC_NO_DES="1"
    AC_MSG_RESULT(no)
else
    XMLSEC_NO_DES="0"
    AC_MSG_RESULT(yes)
fi    
AM_CONDITIONAL(XMLSEC_NO_DES, test "z$XMLSEC_NO_DES" = "z1")
AC_SUBST(XMLSEC_NO_DES)

dnl ==========================================================================
dnl See do we need AES support
dnl ==========================================================================
AC_MSG_CHECKING(for AES support) 
AC_ARG_ENABLE(aes,   [  --enable-aes            enable AES support])
if test "z$enable_aes" = "zno" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_AES=1"
    XMLSEC_NO_AES="1"
    AC_MSG_RESULT(no)
else
    XMLSEC_NO_AES="0"
    AC_MSG_RESULT(yes)
fi    
AM_CONDITIONAL(XMLSEC_NO_AES, test "z$XMLSEC_NO_AES" = "z1")
AC_SUBST(XMLSEC_NO_AES)

dnl ==========================================================================
dnl See do we need GOST support
dnl ==========================================================================
AC_MSG_CHECKING(for GOST support)
AC_ARG_ENABLE(gost,   [  --enable-gost           enable GOST support (no)])
if test "z$enable_gost" != "zyes" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_GOST=1"
    XMLSEC_NO_GOST="1"
    AC_MSG_RESULT(no)
else
    XMLSEC_NO_GOST="0"
    AC_MSG_RESULT(yes)
fi
AM_CONDITIONAL(XMLSEC_NO_GOST, test "z$XMLSEC_NO_GOST" = "z1")
AC_SUBST(XMLSEC_NO_GOST)


dnl ==========================================================================
dnl See do we need GOST2012 support
dnl ==========================================================================
AC_MSG_CHECKING(for EXPERMIENTAL GOST2012 support)
AC_ARG_ENABLE(gost,   [  --enable-gost2012       enable EXPERMIENTAL GOST2012 support (no)])
if test "z$enable_gost2012" != "zyes" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_GOST2012=1"
    XMLSEC_NO_GOST2012="1"
    AC_MSG_RESULT(no)
else
    XMLSEC_NO_GOST2012="0"
    AC_MSG_RESULT(yes)
fi
AM_CONDITIONAL(XMLSEC_NO_GOST2012, test "z$XMLSEC_NO_GOST2012" = "z1")
AC_SUBST(XMLSEC_NO_GOST2012)


dnl ==========================================================================
dnl See do we need XMLDSig support
dnl ==========================================================================
AC_MSG_CHECKING(for XMLDSig support) 
AC_ARG_ENABLE(xmldsig,   [  --enable-xmldsig        enable XMLDSig support (yes)])
if test "z$enable_xmldsig" = "zno" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_XMLDSIG=1"
    XMLSEC_NO_XMLDSIG="1"
    AC_MSG_RESULT(no)
else
    XMLSEC_NO_XMLDSIG="0"
    AC_MSG_RESULT(yes)
fi    
AM_CONDITIONAL(XMLSEC_NO_XMLDSIG, test "z$XMLSEC_NO_XMLDSIG" = "z1")
AC_SUBST(XMLSEC_NO_XMLDSIG)

dnl ==========================================================================
dnl See do we need XMLEnc support
dnl ==========================================================================
AC_MSG_CHECKING(for XMLEnc support) 
AC_ARG_ENABLE(xmlenc,   [  --enable-xmlenc         enable XMLEnc support (yes)])
if test "z$enable_xmlenc" = "zno" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_XMLENC=1"
    XMLSEC_NO_XMLENC="1"
    AC_MSG_RESULT(no)
else
    XMLSEC_NO_XMLENC="0"
    AC_MSG_RESULT(yes)
fi    
AM_CONDITIONAL(XMLSEC_NO_XMLENC, test "z$XMLSEC_NO_XMLENC" = "z1")
AC_SUBST(XMLSEC_NO_XMLENC)

dnl ==========================================================================
dnl check if we need dynamic loading support
dnl ==========================================================================
XMLSEC_DL_INCLUDES=""
XMLSEC_DL_LIBS=""
AC_MSG_CHECKING(for xmlsec-crypto dynamic loading support) 
AC_ARG_ENABLE(crypto_dl,   [  --enable-crypto-dl      enable dynamic loading support for xmlsec-crypto libraries (yes)])
if test "z$enable_crypto_dl" = "zno" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_CRYPTO_DYNAMIC_LOADING=1"
    XMLSEC_NO_CRYPTO_DYNAMIC_LOADING="1"
    AC_MSG_RESULT(no)
else
    AC_CHECK_HEADER([ltdl.h], 
	[
	XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_DL_LIBLTDL=1"
	AC_CHECK_LIB(
	    [ltdl],
	    [lt_dlopenext],
	    [LIBLTDL=-lltdl],
	    [LIBLTDL=]
	)],
	[LIBLTDL=]
    )

    if test "z$LIBLTDL" != "z" ; then
        XMLSEC_DL_INCLUDES="$INCLTDL"
        XMLSEC_DL_LIBS="$LIBLTDL"
        XMLSEC_NO_CRYPTO_DYNAMIC_LOADING="0"
	AC_MSG_RESULT(yes)
    else
	AC_MSG_ERROR([libltdl is required for xmlsec-crypto dynamic loading])
    fi
fi
AM_CONDITIONAL(XMLSEC_NO_CRYPTO_DYNAMIC_LOADING, test "z$XMLSEC_NO_CRYPTO_DYNAMIC_LOADING" = "z1")
AC_SUBST(XMLSEC_NO_CRYPTO_DYNAMIC_LOADING)
AC_SUBST(XMLSEC_DL_INCLUDES)
AC_SUBST(XMLSEC_DL_LIBS)

dnl ==========================================================================
dnl check if we need dynamic loading in the xmlsec apps
dnl ==========================================================================
AC_MSG_CHECKING(for xmlsec-crypto dynamic loading support in command line tool) 
AC_ARG_ENABLE(apps_crypto_dl,   [  --enable-apps-crypto-dl  enable dynamic loading support for xmlsec-crypto libraries in xmlsec command line tool (yes)])
if test "z$enable_apps_crypto_dl" = "z" ; then
    enable_apps_crypto_dl="$enable_crypto_dl"
fi
if test "z$enable_apps_crypto_dl" = "zno" ; then
    XMLSEC_NO_APPS_CRYPTO_DYNAMIC_LOADING="1"
    AC_MSG_RESULT(no)
elif test "z$enable_crypto_dl" = "zno" ; then
    AC_MSG_RESULT(no)
    AC_MSG_ERROR(xmlsec-crypto libraries dynamic loading support in xmlsec command line tool is requested but no dynamic loading in xmlsec itself is disabled)
else
    XMLSEC_APP_DEFINES="$XMLSEC_APP_DEFINES -DXMLSEC_CRYPTO_DYNAMIC_LOADING=1"
    XMLSEC_NO_APPS_CRYPTO_DYNAMIC_LOADING="0"
    AC_MSG_RESULT(yes)
fi    
AM_CONDITIONAL(XMLSEC_NO_APPS_CRYPTO_DYNAMIC_LOADING, 
    test "z$XMLSEC_NO_APPS_CRYPTO_DYNAMIC_LOADING" = "z1")
AC_SUBST(XMLSEC_NO_APPS_CRYPTO_DYNAMIC_LOADING)

dnl ==========================================================================
dnl Where do we want to install docs
dnl ==========================================================================
AC_MSG_CHECKING(for docs folder) 
AC_ARG_WITH(html-dir,           [  --with-html-dir=PATH    path to installed docs ])
if test "z$with_html_dir" != "z" ; then
    XMLSEC_DOCDIR=$with_html_dir
else
    XMLSEC_DOCDIR='$(datadir)/doc/xmlsec1'
fi

AC_MSG_RESULT($XMLSEC_DOCDIR)
AC_SUBST(XMLSEC_DOCDIR)

dnl ==========================================================================
dnl See do we need Simple Keys Manager 
dnl ==========================================================================
AC_MSG_CHECKING(for Simple Keys Manager testing) 
AC_ARG_ENABLE(skm,   [  --enable-skm            enable Simple Keys Manager testing (yes)])
if test "z$enable_skm" = "zno" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_SKM_TEST=1"
    AC_MSG_RESULT(no)
else
    AC_MSG_RESULT(yes)
fi

dnl ==========================================================================
dnl See do we need templates tests
dnl ==========================================================================
AC_MSG_CHECKING(for templates testing) 
AC_ARG_ENABLE(tmpl_tests,   [  --enable-tmpl-tests     enable templates testing in xmlsec utility (yes)])
if test "z$enable_tmpl_tests" = "zyes" ; then
    XMLSEC_DEFINES="$XMLSEC_DEFINES -DXMLSEC_NO_TMPL_TEST=1"
    AC_MSG_RESULT(no)
else
    AC_MSG_RESULT(yes)
fi    

dnl ==========================================================================
dnl Debug 
dnl ==========================================================================
AC_MSG_CHECKING(for debugging)
AC_ARG_ENABLE(debugging,   [  --enable-debugging      enable debugging compilation flags (no)])
if test "z$enable_debugging" = "zyes" ; then
    CFLAGS="$CFLAGS -g"
    AC_MSG_RESULT(yes)
else
    AC_MSG_RESULT(no)
fi    

dnl ==========================================================================
dnl Profiling 
dnl ==========================================================================
AC_MSG_CHECKING(for profiling) 
AC_ARG_ENABLE(profiling,   [  --enable-profiling      enable profiling compilation flags (no)])
if test "z$enable_profiling" = "zyes" ; then
    CFLAGS="$CFLAGS -pg"
    AC_MSG_RESULT(yes)
else
    AC_MSG_RESULT(no)
fi 

dnl ==========================================================================
dnl Pedantic compilation
dnl ==========================================================================
AC_MSG_CHECKING(for pedantic) 
AC_ARG_ENABLE(pedantic,   [  --enable-pedantic       enable pedantic compilation flags (no)])
if test "z$enable_pedantic" = "zyes" ; then
    dnl CFLAGS="$CFLAGS -pedantic -Wall -std=c99 -W -Wunused -Wimplicit -Wreturn-type -Wswitch -Wcomment -Wtrigraphs -Wformat -Wchar-subscripts -Wuninitialized -Wparentheses -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wnested-externs -Wredundant-decls"
    CFLAGS="$CFLAGS -O -pedantic -Wall -std=c99 -fno-inline -W -Wunused -Wimplicit -Wreturn-type -Wswitch -Wcomment -Wtrigraphs -Wformat -Wchar-subscripts -Wuninitialized -Wparentheses -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Waggregate-return -Wmissing-prototypes -Wnested-externs -Wredundant-decls"
    AC_MSG_RESULT(yes)
else
    AC_MSG_RESULT(no)
fi    

dnl ==========================================================================
dnl Static linking
dnl ==========================================================================
XMLSEC_STATIC_BINARIES=""
AC_MSG_CHECKING(for static linking) 
AC_ARG_ENABLE(static_linking,   [  --enable-static-linking enable static linking (no)])
if test "z$enable_static_linking" = "zyes" ; then
    XMLSEC_STATIC_BINARIES="-static"
    AC_MSG_RESULT(yes)
else
    AC_MSG_RESULT(no)
fi    
AC_SUBST(XMLSEC_STATIC_BINARIES)

dnl ==========================================================================
dnl Final steps: xmlsec config
dnl ==========================================================================
XMLSEC_CORE_CFLAGS="$XMLSEC_DEFINES -I${includedir}/xmlsec1  $XMLSEC_DL_INCLUDES"
XMLSEC_CORE_LIBS="-lxmlsec1 $XMLSEC_DL_LIBS "
AC_SUBST(XMLSEC_CORE_CFLAGS)
AC_SUBST(XMLSEC_CORE_LIBS)

dnl used in xmlsecConf.sh*
XMLSEC_LIBDIR="${libdir}"
AC_SUBST(XMLSEC_LIBDIR)

XMLSEC_OPENSSL_CFLAGS="$XMLSEC_CORE_CFLAGS $OPENSSL_CFLAGS"
XMLSEC_OPENSSL_LIBS="-L${libdir} -l$OPENSSL_CRYPTO_LIB $XMLSEC_CORE_LIBS $OPENSSL_LIBS"
AC_SUBST(XMLSEC_OPENSSL_CFLAGS)
AC_SUBST(XMLSEC_OPENSSL_LIBS)

XMLSEC_GCRYPT_CFLAGS="$XMLSEC_CORE_CFLAGS $GCRYPT_CFLAGS"
XMLSEC_GCRYPT_LIBS="-L${libdir} -l$GCRYPT_CRYPTO_LIB $XMLSEC_CORE_LIBS $GCRYPT_LIBS"
AC_SUBST(XMLSEC_GCRYPT_CFLAGS)
AC_SUBST(XMLSEC_GCRYPT_LIBS)

# xmlsec-gnutls is using xmlsec-gcrypt
XMLSEC_GNUTLS_CFLAGS="$XMLSEC_CORE_CFLAGS $GNUTLS_CFLAGS"
XMLSEC_GNUTLS_LIBS="-L${libdir} -l$GNUTLS_CRYPTO_LIB -l$GCRYPT_CRYPTO_LIB $XMLSEC_CORE_LIBS $GNUTLS_LIBS"
AC_SUBST(XMLSEC_GNUTLS_CFLAGS)
AC_SUBST(XMLSEC_GNUTLS_LIBS)

XMLSEC_NSS_CFLAGS="$XMLSEC_CORE_CFLAGS $NSS_CFLAGS"
XMLSEC_NSS_LIBS="-L${libdir} -l$NSS_CRYPTO_LIB $XMLSEC_CORE_LIBS $NSS_LIBS"
AC_SUBST(XMLSEC_NSS_CFLAGS)
AC_SUBST(XMLSEC_NSS_LIBS)

dnl No we will not generate pkg-config file for mscrypto !
dnl XMLSEC_MSCRYPTO_CFLAGS="$XMLSEC_CORE_CFLAGS $MSCRYPTO_CFLAGS"
dnl XMLSEC_MSCRYPTO_LIBS="-L${libdir} -l$MSCRYPTO_CRYPTO_LIB $XMLSEC_CORE_LIBS $MSCRYPTO_LIBS";;
dnl AC_SUBST(XMLSEC_MSCRYPTO_CFLAGS)
dnl AC_SUBST(XMLSEC_MSCRYPTO_LIBS)

XMLSEC_CFLAGS="$XMLSEC_CORE_CFLAGS $LIBXML_CFLAGS $LIBXSLT_CFLAGS $XMLSEC_CRYPTO_CFLAGS"
XMLSEC_LIBS="-L${libdir} -l$XMLSEC_CRYPTO_LIB $XMLSEC_CORE_LIBS $LIBXML_LIBS $LIBXSLT_LIBS $XMLSEC_CRYPTO_LIBS"
AC_SUBST(XMLSEC_CFLAGS)
AC_SUBST(XMLSEC_LIBS)

AC_SUBST(XMLSEC_DEFINES)
AC_SUBST(XMLSEC_APP_DEFINES)
AC_SUBST(CFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(XMLSEC_EXTRA_LDFLAGS)
AC_SUBST(XMLSEC_CRYPTO_EXTRA_LDFLAGS)

AC_SUBST(XMLSEC_DEFAULT_CRYPTO)
AC_SUBST(XMLSEC_CRYPTO_LIST)
AC_SUBST(XMLSEC_CRYPTO_DISABLED_LIST)
AC_SUBST(XMLSEC_CRYPTO_LIB)
AC_SUBST(XMLSEC_CRYPTO_CFLAGS)
AC_SUBST(XMLSEC_CRYPTO_LIBS)

XMLSEC_CRYPTO_PC_FILES_LIST=""
for i in $XMLSEC_CRYPTO_LIST ; do
    dnl skip pkg-config file for mscrypto
    test x$i = xmscrypto && continue
    XMLSEC_CRYPTO_PC_FILES_LIST="$XMLSEC_CRYPTO_PC_FILES_LIST $XMLSEC_PACKAGE-$i.pc"
done
AC_SUBST(XMLSEC_CRYPTO_PC_FILES_LIST)

dnl ==========================================================================
dnl Writing result files
dnl ==========================================================================
if test "z$XMLSEC_NO_OPENSSL" != "z1" ; then
AC_CONFIG_FILES([include/xmlsec/openssl/Makefile src/openssl/Makefile])
fi

if test "z$XMLSEC_NO_GNUTLS" != "z1" ; then
AC_CONFIG_FILES([include/xmlsec/gnutls/Makefile src/gnutls/Makefile])
fi

if test "z$XMLSEC_NO_GCRYPT" != "z1" ; then
AC_CONFIG_FILES([include/xmlsec/gcrypt/Makefile src/gcrypt/Makefile])
fi

if test "z$XMLSEC_NO_NSS" != "z1" ; then
AC_CONFIG_FILES([include/xmlsec/nss/Makefile src/nss/Makefile])
fi

if test "z$XMLSEC_NO_MSCRYPTO" != "z1" ; then
AC_CONFIG_FILES([include/xmlsec/mscrypto/Makefile src/mscrypto/Makefile])
fi

if test "z$XMLSEC_NO_CRYPTO_DYNAMIC_LOADING" = "z1" ; then
(rm -f xmlsec1.pc && $LN_S xmlsec1-$XMLSEC_DEFAULT_CRYPTO.pc xmlsec1.pc)
else
AC_CONFIG_FILES([xmlsec1.pc:xmlsec.pc.in])
fi

AC_CONFIG_FILES([
include/xmlsec/version.h
Makefile
include/Makefile
include/xmlsec/Makefile
include/xmlsec/private/Makefile
src/Makefile
apps/Makefile
docs/Makefile
docs/api/Makefile
man/Makefile

xmlsec1-openssl.pc:xmlsec-openssl.pc.in
xmlsec1-gnutls.pc:xmlsec-gnutls.pc.in
xmlsec1-gcrypt.pc:xmlsec-gcrypt.pc.in
xmlsec1-nss.pc:xmlsec-nss.pc.in
xmlsec1.spec:xmlsec.spec.in
])
AC_CONFIG_FILES([xmlsec1Conf.sh:xmlsecConf.sh.in], [chmod +x xmlsec1Conf.sh])
AC_CONFIG_FILES([xmlsec1-config:xmlsec-config.in], [chmod +x xmlsec1-config])
AC_OUTPUT

