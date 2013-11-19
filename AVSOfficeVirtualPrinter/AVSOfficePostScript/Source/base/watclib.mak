#  Copyright (C) 2001-2006 Artifex Software, Inc.
#  All Rights Reserved.
#
#  This software is provided AS-IS with no warranty, either express or
#  implied.
#
#  This software is distributed under license and may not be copied, modified
#  or distributed except as expressly authorized under the terms of that
#  license.  Refer to licensing information at http://www.artifex.com/
#  or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
#  San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
#
# $Id: watclib.mak 9334 2009-01-08 09:17:18Z ghostgum $
# makefile for MS-DOS / Watcom C/C++ library testing.

libdefault: $(GLOBJ)gslib.exe
	$(NO_OP)

AROOTDIR=c:/gs
GSROOTDIR=$(AROOTDIR)/gs$(GS_DOT_VERSION)
GS_DOCDIR=$(GSROOTDIR)/doc
GS_LIB_DEFAULT=$(GSROOTDIR)/Resource/Init\;$(GSROOTDIR)/lib\;$(GSROOTDIR)/Resource/Font\;$(AROOTDIR)/fonts
SEARCH_HERE_FIRST=1
GS_INIT=gs_init.ps

!ifndef DEBUG
DEBUG=1
!endif
!ifndef TDEBUG
TDEBUG=1
!endif

GS=gslib

!ifndef BINDIR
BINDIR=.\debugobj
!endif
!ifndef GLSRCDIR
GLSRCDIR=.\base
!endif
!ifndef PSRESDIR
PSRESDIR=.\Resource
!endif
!ifndef GLGENDIR
GLGENDIR=.\debugobj
!endif
!ifndef GLOBJDIR
GLOBJDIR=.\debugobj
!endif

# Do not edit the next group of lines.
NUL=
DD=$(GLGENDIR)\$(NUL)
GLD=$(GLGENDIR)\$(NUL)

!ifndef JSRCDIR
JSRCDIR=jpeg
!endif

!ifndef PNGSRCDIR
PNGSRCDIR=libpng
!endif

!ifndef ZSRCDIR
ZSRCDIR=zlib
!endif

# Define the jbig2dec library and source location.
# See jbig2.mak for more information.

!ifndef JBIG2_LIB
JBIG2_LIB=jbig2dec
!endif

!ifndef JBIG2SRCDIR
JBIG2SRCDIR=jbig2dec
!endif

# Define the directory where the icclib source are stored.
# See icclib.mak for more information

!ifndef ICCSRCDIR
ICCSRCDIR=icclib
!endif

# Define the directory where the ijs source is stored,
# and the process forking method to use for the server.
# See ijs.mak for more information.

!ifndef IJSSRCDIR
IJSSRCDIR=ijs
IJSEXECTYPE=win
!endif

# Define the directory where the imdi library source is stored.
# See devs.mak for more information

!ifndef IMDISRCDIR
IMDISRCDIR=imdi
!endif

CFLAGS=

!ifndef WCVERSION
WCVERSION=10.0
!endif
LIBPATHS=LIBPATH $(%WATCOM)\lib386 LIBPATH $(%WATCOM)\lib386\dos
STUB=$(%WATCOM)\binb\wstub.exe

!ifndef CPU_TYPE
CPU_TYPE=386
!endif

!ifndef SYNC
SYNC=winsync
!endif

PLATFORM=watclib_
MAKEFILE=$(GLSRCDIR)\watclib.mak
TOP_MAKEFILES=$(MAKEFILE) $(GLSRCDIR)\wccommon.mak
PLATOPT=

!include $(GLSRCDIR)\wccommon.mak

!ifndef FEATURE_DEVS
FEATURE_DEVS=$(GLD)patlib.dev $(GLD)path1lib.dev
!endif
!ifndef DEVICE_DEVS
DEVICE_DEVS=$(DD)vga.dev
!endif

!ifndef COMPILE_INITS
COMPILE_INITS=0
!endif
!ifndef BAND_LIST_STORAGE
BAND_LIST_STORAGE=file
!endif
!ifndef BAND_LIST_COMPRESSOR
BAND_LIST_COMPRESSOR=zlib
!endif
!ifndef FILE_IMPLEMENTATION
FILE_IMPLEMENTATION=stdio
!endif
!ifndef STDIO_IMPLEMENTATION
STDIO_IMPLEMENTATION=
!endif

!include $(GLSRCDIR)\wctail.mak
!include $(GLSRCDIR)\devs.mak
!include $(GLSRCDIR)\contrib.mak

GLCCWIN=$(GLCC)
!include $(GLSRCDIR)\winplat.mak

watclib_1=$(GLOBJ)gp_getnv.$(OBJ) $(GLOBJ)gp_paper.$(OBJ) $(GLOBJ)gp_iwatc.$(OBJ)
!ifeq WAT32 0
watclib_2=$(GLOBJ)gp_dosfs.$(OBJ) $(GLOBJ)gp_dosfe.$(OBJ) $(GLOBJ)gp_msdos.$(OBJ)
watclib_inc=
!else
watclib_2=
watclib_inc=$(GLD)winplat.dev
!endif
watclib__=$(watclib_1) $(watclib_2)
$(GLGEN)watclib_.dev: $(watclib__) $(GLGEN)nosync.dev $(watclib_inc)
	$(SETMOD) $(GLGEN)watclib_ $(watclib_1)
	$(ADDMOD) $(GLGEN)watclib_ -obj $(watclib_2)
	$(ADDMOD) $(GLGEN)watclib_ -include $(GLGEN)nosync $(watclib_inc)

$(GLOBJ)gp_iwatc.$(OBJ): $(GLSRC)gp_iwatc.c $(stat__h) $(string__h)\
 $(gx_h) $(gp_h) $(gpmisc_h)
	$(GLCC) $(GLO_)gp_iwatc.$(OBJ) $(C_) $(GLSRC)gp_iwatc.c

BEGINFILES=*.err

LIB_ONLY=$(GLOBJ)gslib.obj $(GLOBJ)gsnogc.obj $(GLOBJ)gconfig.obj $(GLOBJ)gscdefs.obj $(GLOBJ)gsromfs$(COMPILE_INITS).$(OBJ)
ll_tr=ll.tr
$(ll_tr): $(TOP_MAKEFILES)
	echo OPTION STACK=64k >$(ll_tr)
!ifeq WAT32 0
	echo SYSTEM DOS4G >>$(ll_tr)
	echo OPTION STUB=$(STUB) >>$(ll_tr)
!endif
	echo FILE $(GLOBJ)gsromfs$(COMPILE_INITS).$(OBJ) >>$(ll_tr)
	echo FILE $(GLOBJ)gsnogc.obj >>$(ll_tr)
	echo FILE $(GLOBJ)gconfig.obj >>$(ll_tr)
	echo FILE $(GLOBJ)gscdefs.obj >>$(ll_tr)

$(GLOBJ)gslib.exe: $(LIB_ALL) $(LIB_ONLY) $(ld_tr) $(ll_tr)
	$(LINK) $(LCT) NAME gslib OPTION MAP=gslib FILE $(GLOBJ)gslib @$(ld_tr) @$(ll_tr)
