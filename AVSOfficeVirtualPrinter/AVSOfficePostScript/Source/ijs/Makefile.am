## Copyright (C) 2001-2007 Artifex Software, Inc.
##
## Permission is hereby granted, free of charge, to any person
## obtaining a copy of this software and associated documentation
## files (the "Software"), to deal in the Software without
## restriction, including without limitation the rights to use, copy,
## modify, merge, publish, distribute, sublicense, and/or sell copies
## of the Software, and to permit persons to whom the Software is
## furnished to do so, subject to the following conditions:
##
## The above copyright notice and this permission notice shall be
## included in all copies or substantial portions of the Software.
##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
## EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
## MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
## NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
## BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
## ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
## CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
## SOFTWARE.

## $Id: Makefile.am 8371 2007-11-16 02:10:56Z giles $

## Process this file with automake to produce Makefile.in.

AUTOMAKE_OPTIONS = 1.6 foreign dist-bzip2 no-dependencies 

@SET_MAKE@

MAINT_CHARSET = latin1

## Variables

pkgincludedir=$(includedir)/ijs
m4datadir = $(datadir)/aclocal
pkgconfigdir = $(libdir)/pkgconfig

lib_LTLIBRARIES = libijs.la

pkginclude_HEADERS = \
	ijs_client.h \
	ijs.h \
	ijs_server.h

noinst_HEADERS = unistd_.h

bin_PROGRAMS = ijs_client_example ijs_server_example

bin_SCRIPTS = ijs-config


libijs_la_SOURCES = \
	ijs.c \
	ijs_client.c \
	ijs_server.c
#ijs_version.c
EXTRA_libijs_la_SOURCES = ijs_exec_unix.c ijs_exec_win.c
if SYSDEPS_WINDOWS
libijs_la_LIBADD = ijs_exec_win.lo $(IJS_DEPLIBS)
else
libijs_la_LIBADD = ijs_exec_unix.lo $(IJS_DEPLIBS)
endif
# Uncommment to build an unversioned library (version in soname)
libijs_la_LDFLAGS = -release $(IJS_VERSION) -rpath $(libdir)
# Uncomment to build a versioned library
#libijs_la_LDFLAGS = \
# -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
# -rpath $(libdir)


ijs_client_example_SOURCES = ijs_client_example.c
ijs_client_example_LDADD = libijs.la

ijs_server_example_SOURCES = ijs_server_example.c
ijs_server_example_LDADD = libijs.la


## Data

pkgconfig_DATA = ijs.pc

man_MANS = ijs-config.1

EXTRA_DIST = \
	common.mak unix.mak windows.mak \
	ijs_spec.sgml state.eps state.fig ijs_spec.pdf \
	$(man_MANS)

## Rules

# Libtool dependency
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

doc: ijs_spec.ps ijs_spec.pdf

# We don't use db2pdf because it can't handle embedded .eps
ijs_spec.ps: ijs_spec.sgml
	$(DB2PS) ijs_spec.sgml

ijs_spec.pdf: ijs_spec.ps
	$(PS2PDF) ijs_spec.ps

# Note that ijs_spec.pdf is included in the dist list above,
# so the 'doc' target needs to be build for 'make dist' to work.

## Clean

CLEANSCRIPTS = \
	depcomp \
	install-sh \
	ltconfig \
	mdate-sh \
	missing \
	mkinstalldirs \
	config.guess \
	config.sub \
	ltmain.sh

CLEANFILES = gmon.out core ijs_spec.log ijs_spec.tex ijs_spec.aux ijs_spec.ps ijs_spec.pdf
DISTCLEANFILES = ijs-config config.cache config.log config.status
# ijs-config.in ijs.pc ijs.pc.in ijs.m4 ijs.h
MAINTAINERCLEANFILES = INSTALL aclocal.m4 config.h.in configure Makefile.in stamp-h.in $(CLEANSCRIPTS)

