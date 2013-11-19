dnl Copyright (C) 2001-2004 Artifex Software, Inc.
dnl
dnl Permission is hereby granted, free of charge, to any person
dnl obtaining a copy of this software and associated documentation
dnl files (the "Software"), to deal in the Software without
dnl restriction, including without limitation the rights to use, copy,
dnl modify, merge, publish, distribute, sublicense, and/or sell copies
dnl of the Software, and to permit persons to whom the Software is
dnl furnished to do so, subject to the following conditions:
dnl
dnl The above copyright notice and this permission notice shall be
dnl included in all copies or substantial portions of the Software.
dnl
dnl THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
dnl EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
dnl MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
dnl NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
dnl BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
dnl ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
dnl CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
dnl SOFTWARE.

dnl $Id: configure.ac 8022 2007-06-05 22:23:38Z giles $

dnl Process this file with autoconf to produce a configure script.

dnl MAKING RELEASES (a step-by-step guide!)
dnl ===============
dnl
dnl Since the last release:
dnl 1. if only source code (not the interface) has changed, set
dnl      IJS_MICRO_VERSION += 1;
dnl      IJS_INTERFACE_AGE += 1;
dnl 2. if any functions have been added, removed, or changed, set
dnl      IJS_INTERFACE_AGE = 0;
dnl      IJS_CURRENT_INTERFACE += 1;
dnl 3. if interfaces have been added, set
dnl      IJS_BINARY_AGE += 1;
dnl 4. if interfaces have been removed, set
dnl      IJS_BINARY_AGE = 0;
dnl
dnl For more detailed information, see the libtool info documentation.
dnl
pushdef([IJS_NAME],              [ijs])
pushdef([IJS_MAJOR_VERSION],     [0])
pushdef([IJS_MINOR_VERSION],     [35])
pushdef([IJS_EXTRA_VERSION],     [])
pushdef([IJS_CURRENT_INTERFACE], [1])
pushdef([IJS_INTERFACE_AGE],     [1])
pushdef([IJS_BINARY_AGE],        [0])
pushdef([IJS_VERSION], IJS_MAJOR_VERSION.IJS_MINOR_VERSION[]IJS_EXTRA_VERSION)

AC_INIT(IJS_NAME, IJS_VERSION, [inkjet-list@linuxprinting.org])
AC_PREREQ(2.53)
AC_CONFIG_SRCDIR(ijs.c)
AC_REVISION($Revision: 5783 $)

dnl In the following script, there are a the following variants
dnl of ijs cflags and libs variables
dnl
dnl IJS_CFLAGS:  cflags for compiling libraries and example progs
dnl IJS_LIBS:    libraries for linking programs. ONLY to be used
dnl                    to generate ${ijs_libs}
dnl IJS_DEPLIBS: libraries for linking libraries against
dnl ijs_cflags:  cflags to store in ijs-config
dnl ijs_libs:    libs to store in ijs-config
dnl LIBIJS_LIBS  libs to link programs IN THIS PACKAGE ONLY against

dnl initial default values
IJS_LIBS="-lijs"

[IJS_MAJOR_VERSION]=IJS_MAJOR_VERSION
[IJS_MINOR_VERSION]=IJS_MINOR_VERSION
[IJS_EXTRA_VERSION]=IJS_EXTRA_VERSION

[IJS_CURRENT_INTERFACE]=IJS_CURRENT_INTERFACE
[IJS_INTERFACE_AGE]=IJS_INTERFACE_AGE
[IJS_BINARY_AGE]=IJS_BINARY_AGE
[IJS_VERSION]=IJS_VERSION
popdef([IJS_MAJOR_VERSION])
popdef([IJS_MINOR_VERSION])
popdef([IJS_EXTRA_VERSION])
popdef([IJS_CURRENT_INTERFACE])
popdef([IJS_INTERFACE_AGE])
popdef([IJS_BINARY_AGE])

dnl Initialize automake stuff.
AM_INIT_AUTOMAKE(IJS_NAME, IJS_VERSION)
popdef([IJS_NAME])
popdef([IJS_VERSION])

AC_SUBST(IJS_MAJOR_VERSION)
AC_SUBST(IJS_MINOR_VERSION)
AC_SUBST(IJS_CURRENT_INTERFACE)
AC_SUBST(IJS_INTERFACE_AGE)
AC_SUBST(IJS_BINARY_AGE)
AC_SUBST(IJS_VERSION)

dnl libtool versioning
LT_RELEASE=$IJS_MAJOR_VERSION.$IJS_MINOR_VERSION
LT_CURRENT=$IJS_CURRENT_INTERFACE
LT_REVISION=$IJS_INTERFACE_AGE
LT_AGE=$IJS_BINARY_AGE
AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

dnl Specify a configuration file.
dnl AM_CONFIG_HEADER(config.h)

dnl Set up libtool scripts.
dnl Disable shared library building to speed up the build
AC_DISABLE_SHARED
AM_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

dnl Checks for programs

AC_PROG_CC
AM_PROG_CC_STDC
AM_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)
AC_ISC_POSIX
AC_PROG_INSTALL

AC_PATH_PROG(DB2PS, db2ps)
if test x${DB2PS} = x ; then
  AC_MSG_WARN([PostScript documentation cannot be generated!])
fi
AC_SUBST(DB2PS)
AC_PATH_PROG(PS2PDF, ps2pdf)
if test x${PS2PDF} = x ; then
  AC_MSG_WARN([PDF documentation cannot be generated!])
fi
AC_SUBST(PS2PDF)


dnl conditional building

case ${host_os} in
  cygwin | mingw32 | pw32 | interix3 | interix | uwin) libijs_sysdeps="windows";;
  *) libijs_sysdeps="unix";;
esac
AM_CONDITIONAL(SYSDEPS_WINDOWS, test x$libijs_sysdeps = xwindows)


dnl config.status substitutions
AC_SUBST(IJS_CFLAGS)
AC_SUBST(IJS_LIBS)
AC_SUBST(IJS_DEPLIBS)
AC_SUBST(IJS_VERSION)
AC_SUBST(ijs_cflags)
ijs_libs="${IJS_LIBS} ${IJS_DEPLIBS}"
AC_SUBST(ijs_libs)
AC_SUBST(INCLUDES)
LIBIJS_LIBS="${LIBIJS_LIBS} ${IJS_DEPLIBS}"
AC_SUBST(LIBIJS_LIBS)


dnl output files and headers
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([ijs-config], [chmod +x ijs-config])
AC_CONFIG_FILES([ijs.pc])
AC_OUTPUT
