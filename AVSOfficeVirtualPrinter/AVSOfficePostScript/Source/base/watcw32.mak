#  Copyright (C) 2001-2007 Artifex Software, Inc.
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
# $Id: watcw32.mak 9126 2008-10-02 19:33:22Z giles $
# watcw32.mak
# makefile for Watcom C++ v??, Windows NT or Windows 95 platform.
# Created 1997-02-23 by Russell Lang from MSVC++ 4.0 makefile.
# Major revisions 1999-07-26 by Ray Johnston.

# ------------------------------- Options ------------------------------- #

###### This section is the only part of the file you should need to edit.

# ------ Generic options ------ #

# Define the directory for the final executable, and the
# source, generated intermediate file, and object directories
# for the graphics library (GL) and the PostScript/PDF interpreter (PS).

BINDIR=bin
GLSRCDIR=base
GLGENDIR=obj
GLOBJDIR=obj
PSSRCDIR=psi
PSLIBDIR=lib
PSRESDIR=Resource
PSGENDIR=obj
PSOBJDIR=obj

# Define the root directory for Ghostscript installation.

AROOTDIR=c:/gs
GSROOTDIR=$(AROOTDIR)/gs$(GS_DOT_VERSION)

# Define the directory that will hold documentation at runtime.

GS_DOCDIR=$(GSROOTDIR)/doc

# Define the default directory/ies for the runtime
# initialization, resource and font files.  Separate multiple directories with \;.
# Use / to indicate directories, not a single \.

GS_LIB_DEFAULT=$(GSROOTDIR)/Resource/Init\;$(GSROOTDIR)/lib\;$(GSROOTDIR)/Resource/Font\;$(AROOTDIR)/fonts

# Define whether or not searching for initialization files should always
# look in the current directory first.  This leads to well-known security
# and confusion problems, but users insist on it.
# NOTE: this also affects searching for files named on the command line:
# see the "File searching" section of Use.htm for full details.
# Because of this, setting SEARCH_HERE_FIRST to 0 is not recommended.

SEARCH_HERE_FIRST=1

# Define the name of the interpreter initialization file.
# (There is no reason to change this.)

GS_INIT=gs_init.ps

# Choose generic configuration options.

# Setting DEBUG=1 includes debugging features (-Z switch) in the code.
# Code runs substantially slower even if no debugging switches are set,
# and also takes about another 25K of memory.

DEBUG=0

# Setting TDEBUG=1 includes symbol table information for the debugger,
# and also enables stack checking.  Code is substantially slower and larger.

TDEBUG=0

# Define the name of the executable file.

GS=gswin32
GSCONSOLE=gswin32c
GSDLL=gsdll32

# To build two small executables and a large DLL use MAKEDLL=1
# To build two large executables use MAKEDLL=0

MAKEDLL=1

# Define the directory where the IJG JPEG library sources are stored,
# and the major version of the library that is stored there.
# You may need to change this if the IJG library version changes.
# See jpeg.mak for more information.

JSRCDIR=jpeg

# Define the directory where the PNG library sources are stored,
# and the version of the library that is stored there.
# You may need to change this if the libpng version changes.
# See libpng.mak for more information.

PNGSRCDIR=libpng

# Define the directory where the zlib sources are stored.
# See zlib.mak for more information.

ZSRCDIR=zlib

# Define the jbig2dec library source location.
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

IJSSRCDIR=ijs
IJSEXECTYPE=win

# Define the directory where the imdi source are stored.
# See imdi.mak for more information

!ifndef IMDISRCDIR
IMDISRCDIR=imdi
!endif

# Define any other compilation flags.

CFLAGS=

# ------ Platform-specific options ------ #

# Define the drive, directory, and compiler name for the Watcom C files.
# COMPDIR contains the compiler and linker.
# INCDIR contains the include files.
# LIBDIR contains the library files.
# COMP is the full C compiler path name.
# COMPCPP is the full C++ compiler path name.
# COMPAUX is the compiler name for DOS utilities.
# RCOMP is the resource compiler name.
# LINK is the full linker path name.
# Note that INCDIR and LIBDIR are always followed by a \,
#   so if you want to use the current directory, use an explicit '.'.

COMPBASE=$(%WATCOM)
COMPDIR=$(COMPBASE)\binnt
INCDIR=$(COMPBASE)\h -i$(COMPBASE)\h\nt
LIBDIR=$(COMPBASE)\lib386;$(COMPBASE)\lib386\nt
COMP=$(COMPDIR)\wcc386
COMPCPP=$(COMPDIR)\wpp386
COMPAUX=$(COMPDIR)\wcc386
RCOMP=$(COMPDIR)\wrc
LINK=$(COMPDIR)\wlink

# Define the processor architecture. (always i386)
CPU_FAMILY=i386

# Define the processor (CPU) type.  (386, 486 or 586)
CPU_TYPE=486

# Define the math coprocessor (FPU) type.
# Options are -1 (optimize for no FPU), 0 (optimize for FPU present,
# but do not require a FPU), 87, 287, or 387.
# If you have a 486 or Pentium CPU, you should normally set FPU_TYPE to 387,
# since most of these CPUs include the equivalent of an 80387 on-chip;
# however, the 486SX and the Cyrix 486SLC do not have an on-chip FPU, so if
# you have one of these CPUs and no external FPU, set FPU_TYPE to -1 or 0.
# An xx87 option means that the executable will run only if a FPU
# of that type (or higher) is available: this is NOT currently checked
# at runtime.

FPU_TYPE=387

# Define the .dev module that implements thread and synchronization
# primitives for this platform.  Don't change this unless you really know
# what you're doing.

SYNC=winsync

# Do not edit the next group of lines.
NUL=
DD=$(GLGENDIR)\$(NUL)
GLD=$(GLGENDIR)\$(NUL)
PSD=$(PSGENDIR)\$(NUL)

# ------ Devices and features ------ #

# Choose the language feature(s) to include.  See gs.mak for details.

FEATURE_DEVS=$(PSD)psl3.dev $(PSD)pdf.dev $(PSD)dpsnext.dev $(PSD)ttfont.dev $(PSD)epsf.dev

# Choose whether to compile the .ps initialization files into the executable.
# See gs.mak for details.

COMPILE_INITS=0

# Choose whether to store band lists on files or in memory.
# The choices are 'file' or 'memory'.

BAND_LIST_STORAGE=file

# Choose which compression method to use when storing band lists in memory.
# The choices are 'lzw' or 'zlib'.

BAND_LIST_COMPRESSOR=zlib

# Choose the implementation of file I/O: 'stdio', 'fd', or 'both'.
# See gs.mak and sfxfd.c for more details.

FILE_IMPLEMENTATION=stdio

# Choose the implementation of stdio: '' for file I/O and 'c' for callouts
# See gs.mak and ziodevs.c/ziodevsc.c for more details.

STDIO_IMPLEMENTATION=c

# Choose the device(s) to include.  See devs.mak for details,
# devs.mak and contrib.mak for the list of available devices.

DEVICE_DEVS=$(DD)display.dev $(DD)mswindll.dev $(DD)mswinpr2.dev
DEVICE_DEVS2=$(DD)epson.dev $(DD)eps9high.dev $(DD)eps9mid.dev $(DD)epsonc.dev $(DD)ibmpro.dev
DEVICE_DEVS3=$(DD)deskjet.dev $(DD)djet500.dev $(DD)laserjet.dev $(DD)ljetplus.dev $(DD)ljet2p.dev
DEVICE_DEVS4=$(DD)cdeskjet.dev $(DD)cdjcolor.dev $(DD)cdjmono.dev $(DD)cdj550.dev
DEVICE_DEVS5=$(DD)djet500c.dev $(DD)declj250.dev $(DD)lj250.dev $(DD)jetp3852.dev $(DD)r4081.dev $(DD)lbp8.dev $(DD)uniprint.dev
DEVICE_DEVS6=$(DD)st800.dev $(DD)stcolor.dev $(DD)bj10e.dev $(DD)bj200.dev $(DD)m8510.dev $(DD)necp6.dev $(DD)bjc600.dev $(DD)bjc800.dev
DEVICE_DEVS7=$(DD)t4693d2.dev $(DD)t4693d4.dev $(DD)t4693d8.dev $(DD)tek4696.dev
DEVICE_DEVS8=$(DD)pcxmono.dev $(DD)pcxgray.dev $(DD)pcx16.dev $(DD)pcx256.dev $(DD)pcx24b.dev $(DD)pcxcmyk.dev
DEVICE_DEVS9=$(DD)pbm.dev $(DD)pbmraw.dev $(DD)pgm.dev $(DD)pgmraw.dev $(DD)pgnm.dev $(DD)pgnmraw.dev $(DD)pnm.dev $(DD)pnmraw.dev $(DD)ppm.dev $(DD)ppmraw.dev $(DD)pamcmyk32.dev
DEVICE_DEVS10=$(DD)tiffcrle.dev $(DD)tiffg3.dev $(DD)tiffg32d.dev $(DD)tiffg4.dev $(DD)tifflzw.dev $(DD)tiffpack.dev
DEVICE_DEVS11=$(DD)bmpmono.dev $(DD)bmp16.dev $(DD)bmp256.dev $(DD)bmp16m.dev $(DD)tiff12nc.dev $(DD)tiff24nc.dev $(DD)tiffgray.dev $(DD)tiff32nc.dev $(DD)tiffsep.dev
DEVICE_DEVS12=$(DD)psmono.dev $(DD)bit.dev $(DD)bitrgb.dev $(DD)bitcmyk.dev
DEVICE_DEVS13=$(DD)pngmono.dev $(DD)pnggray.dev $(DD)png16.dev $(DD)png256.dev $(DD)png16m.dev $(DD)pngalpha.dev
DEVICE_DEVS14=$(DD)jpeg.dev $(DD)jpeggray.dev $(DD)jpegcmyk.dev
DEVICE_DEVS15=$(DD)pdfwrite.dev $(DD)pswrite.dev $(DD)ps2write.dev $(DD)epswrite.dev $(DD)txtwrite.dev $(DD)pxlmono.dev $(DD)pxlcolor.dev
# Overflow for DEVS3,4,5,6,9
DEVICE_DEVS16=$(DD)ljet3.dev $(DD)ljet3d.dev $(DD)ljet4.dev $(DD)ljet4d.dev
DEVICE_DEVS17=$(DD)pj.dev $(DD)pjxl.dev $(DD)pjxl300.dev
DEVICE_DEVS18=
DEVICE_DEVS19=
DEVICE_DEVS20=

# ---------------------------- End of options ---------------------------- #

# Define the name of the makefile -- used in dependencies.

MAKEFILE=$(PSSRCDIR)\watcw32.mak
TOP_MAKEFILES=$(MAKEFILE) $(GLSRCDIR)\winlib.mak $(PSSRCDIR)\winint.mak

# Define the executable and shell invocations.

D=\\

EXP=
SH=

# Define the arguments for genconf.

CONFILES=-e ~ -p FILE~s~ps
CONFLDTR=-o

# Define the generic compilation flags.

PLATOPT=

# Make sure we get the right default target for make.

dosdefault: default

# Define the compilation flags.

!ifeq CPU_TYPE 586
CPFLAGS=-5s
!else
!ifeq CPU_TYPE 486
CPFLAGS=-4s
!else
!ifeq CPU_TYPE 386
CPFLAGS=-3s
!else
CPFLAGS=
!endif
!endif
!endif

!ifeq FPU_TYPE 586
FPFLAGS=-fp5
!else
!ifeq FPU_TYPE 486
FPFLAGS=-fp4
!else
!ifeq FPU_TYPE 386
FPFLAGS=-fp3
!else
FPFLAGS=
!endif
!endif
!endif


!ifneq DEBUG 0
CD=-dDEBUG
!else
CD=
!endif

!ifneq TDEBUG 0
# What options should WATCOM use for $(CT) when debugging?
CT=-d2
LCT=DEBUG ALL
COMPILE_FULL_OPTIMIZED=    # no optimization when debugging
COMPILE_WITH_FRAMES=    # no optimization when debugging
COMPILE_WITHOUT_FRAMES=    # no optimization when debugging
!else
CT=-d1
LCT=DEBUG LINES
COMPILE_FULL_OPTIMIZED=-oilmre -s
COMPILE_WITH_FRAMES=-of+
COMPILE_WITHOUT_FRAMES=-s
!endif

!ifneq DEBUG 0
CS=
!else
CS=-s
!endif

# Specify function prolog type
COMPILE_FOR_DLL=-bd
COMPILE_FOR_EXE=
COMPILE_FOR_CONSOLE_EXE=

GENOPT=-d+ $(CD) $(CT) $(CS) -zq -zp8

CCFLAGS=$(PLATOPT) $(FPFLAGS) $(CPFLAGS) $(CFLAGS) $(XCFLAGS)
CC=$(COMP) $(CCFLAGS) @$(GLGENDIR)\ccf32.tr
CPP=$(COMPCPP) $(CCFLAGS) @$(GLGENDIR)\ccf32.tr
!ifneq MAKEDLL 0
WX=$(COMPILE_FOR_DLL)
!else
WX=$(COMPILE_FOR_EXE)
!endif
CC_WX=$(CC) $(WX)
CC_=$(CC_WX) $(COMPILE_FULL_OPTIMIZED)
CC_D=$(CC_WX) $(COMPILE_WITH_FRAMES)
CC_INT=$(CC)
CC_NO_WARN=$(CC_)
CC_SHARED=$(CC_)

# No additional flags are needed for Windows compilation.
CCWINFLAGS=

# Compiler for auxiliary programs

CCAUX=$(COMPAUX) -I$(INCDIR) -otexan

# Define the files to be removed by `make clean'.
# nmake expands macros when encountered, not when used,
# so this must precede the !include statements.

BEGINFILES2=gsdll32.rex gswin32.rex gswin32c.rex

# Define the switches for the compilers.

C_=
O_=-fo=
RO_=$(O_)

# Include the generic makefiles.

!include $(GLSRCDIR)\version.mak
# psromfs.mak must precede lib.mak
!include $(PSSRCDIR)\psromfs.mak
!include $(GLSRCDIR)\winlib.mak
!include $(PSSRCDIR)\winint.mak

# -------------------------- Auxiliary programs --------------------------- #

$(GLGENDIR)\ccf32.tr: $(TOP_MAKEFILES)
	echo $(GENOPT) -I$(INCDIR) -DCHECK_INTERRUPTS -D_Windows -D__WIN32__ -D_WATCOM_ > $(GLGENDIR)\ccf32.tr

$(ECHOGS_XE): $(GLSRC)echogs.c
	$(CCAUX) $(GLSRC)echogs.c $(O_)$(GLOBJ)echogs.obj 
	$(LINK) FILE $(GLOBJ)echogs.obj NAME $(ECHOGS_XE)

# Don't create genarch if it's not needed
!ifdef GENARCH_XE
$(GENARCH_XE): $(GLSRC)genarch.c $(GENARCH_DEPS) $(GLGENDIR)\ccf32.tr
	$(CCAUX) $(GLSRC)genarch.c @$(GLGENDIR)\ccf32.tr $(O_)$(GLOBJ)genarch.obj 
	$(LINK) FILE $(GLOBJ)genarch.obj NAME $(GENARCH_XE)
!endif

$(GENCONF_XE): $(GLSRC)genconf.c $(GENCONF_DEPS)
	$(CCAUX) $(GLSRC)genconf.c $(O_)$(GLOBJ)genconf.obj 
	$(LINK) FILE $(GLOBJ)genconf.obj NAME $(GENCONF_XE)

$(GENDEV_XE): $(GLSRC)gendev.c $(GENDEV_DEPS)
	$(CCAUX) $(GLSRC)gendev.c $(O_)$(GLOBJ)gendev.obj 
	$(LINK) FILE $(GLOBJ)gendev.obj NAME $(GENDEV_XE)

$(GENHT_XE): $(GLSRC)genht.c $(GENHT_DEPS)
	$(CCAUX) $(GENHT_CFLAGS) $(GLSRC)genht.c $(O_)$(GLOBJ)genht.obj 
	$(LINK) FILE $(GLOBJ)genht.obj NAME $(GENHT_XE)

# PSSRC and PSOBJ aren't defined yet, so we spell out the definitions.
$(GENINIT_XE): $(PSSRCDIR)$(D)geninit.c $(GENINIT_DEPS)
	$(CCAUX) $(PSSRCDIR)$(D)geninit.c $(O_)$(PSOBJDIR)$(D)geninit.obj 
	$(LINK) FILE $(GLOBJ)geninit.obj NAME $(GENINIT_XE)

# -------------------------------- Library -------------------------------- #

# make sure the target directories exist - use special Watcom .BEFORE
# (This is not the best way to do this, but we will have to wait until
# the makefiles get disentangled to do it better.)
.BEFORE
	@if not exist $(GLGENDIR) mkdir $(GLGENDIR)
	@if not exist $(GLOBJDIR) mkdir $(GLOBJDIR)
	@if not exist $(PSGENDIR) mkdir $(PSGENDIR)
	@if not exist $(PSOBJDIR) mkdir $(PSOBJDIR)
	@if not exist $(BINDIR) mkdir $(BINDIR)

# See winlib.mak

# ---------------------------- Watcom objects ----------------------------- #

$(GLOBJ)gp_mktmp.$(OBJ): $(GLSRC)gp_mktmp.c $(stat__h) $(string__h)
	$(GLCC) $(GLO_)gp_mktmp.$(OBJ) $(C_) $(GLSRC)gp_mktmp.c

# ----------------------------- Main program ------------------------------ #

#LIBCTR=libc32.tr
LIBCTR=

GSCONSOLE_XE=$(BINDIR)\$(GSCONSOLE).exe
GSDLL_DLL=$(BINDIR)\$(GSDLL).dll

DWOBJLINK=$(PSOBJ)dwdll.obj, $(GLOBJ)dwimg.obj, $(PSOBJ)dwmain.obj, $(GLOBJ)dwtext.obj, $(PSOBJ)gscdefw.obj, $(PSOBJ)gp_wgetw.obj
DWOBJNOLINK= $(PSOBJ)dwnodll.obj, $(GLOBJ)dwimg.obj, $(PSOBJ)dwmain.obj, $(GLOBJ)dwtext.obj
OBJCLINK=$(PSOBJ)dwmainc.obj, $(PSOBJ)dwdllc.obj, $(PSOBJ)gscdefw.obj, $(PSOBJ)gp_wgetw.obj
OBJCNOLINK=$(PSOBJ)dwmainc.obj, $(PSOBJ)dwnodllc.obj

!ifneq MAKEDLL 0

# The non DLL compiles of the some modules also in the DLL
$(GLOBJ)gp_wgetw.$(OBJ): $(GLSRC)gp_wgetv.c $(AK) $(gscdefs_h)
	$(CC) $(COMPILE_FOR_CONSOLE_EXE) $(GLCCFLAGS) $(CCWINFLAGS) $(I_)$(GLI_)$(_I) $(GLF_) $(GLO_)gp_wgetw.$(OBJ) $(C_) $(GLSRC)gp_wgetv.c

$(GLOBJ)gscdefw.$(OBJ): $(GLGEN)gscdefs.c $(AK) $(gscdefs_h)
	$(CC) $(COMPILE_FOR_CONSOLE_EXE) $(GLCCFLAGS) $(CCWINFLAGS) $(I_)$(GLI_)$(_I) $(GLF_) $(GLO_)gscdefw.$(OBJ) $(C_) $(GLGEN)gscdefs.c

# The graphical small EXE loader
$(GS_XE): $(GSDLL_DLL) $(PSOBJ)$(GSDLL).lib $(DWOBJ) $(GSCONSOLE_XE) $(PSOBJ)$(GS).res \
		$(GLOBJ)gp_wgetw.obj $(GLOBJ)gscdefw.obj
	$(LINK) system nt_win $(LCT) Name $(GS_XE) File $(DWOBJLINK) Library $(PSOBJ)$(GSDLL).lib

# The console mode small EXE loader
$(GSCONSOLE_XE): $(OBJC) $(PSOBJ)$(GS).res $(PSSRCDIR)\dw32c.def \
		$(GLOBJ)gp_wgetw.obj $(GLOBJ)gscdefw.obj
	$(LINK) system nt option map $(LCT) Name $(GSCONSOLE_XE) File $(OBJCLINK) Library $(PSOBJ)$(GSDLL).lib

# The big DLL
$(GSDLL_DLL): $(GS_ALL) $(DEVS_ALL) $(PSOBJ)gsdll.$(OBJ) $(GLOBJ)gp_mktmp.obj $(PSOBJ)$(GSDLL).res $(PSOBJ)gsromfs$(COMPILE_INITS).$(OBJ)
	$(LINK) system nt_dll initinstance terminstance $(LCT) Name $(GSDLL_DLL) File $(GLOBJ)gsdll.obj $(PSOBJ)gsromfs$(COMPILE_INITS).$(OBJ) $(GLOBJ)gp_mktmp.obj @$(ld_tr) @$(PSSRC)gsdll32w.lnk

$(PSOBJ)$(GSDLL).lib: $(GSDLL_DLL)
	erase $(PSOBJ)$(GSDLL).lib
	wlib $(PSOBJ)$(GSDLL) +$(GSDLL_DLL)

!else
# The big graphical EXE
$(GS_XE): $(GSCONSOLE_XE) $(GS_ALL) $(DEVS_ALL) $(PSOBJ)gsdll.$(OBJ) $(GLOBJ)gp_mktmp.obj $(DWOBJNO) $(PSOBJ)$(GS).res $(PSOBJ)dwmain32.def $(PSOBJ)gsromfs$(COMPILE_INITS).$(OBJ)
	$(LINK) option map $(LCT) Name $(GS) File $(GLOBJ)gsdll,$(OBJ) $(PSOBJ)gsromfs$(COMPILE_INITS).$(OBJ) gp_mktmp.obj $(DWOBJNOLINK) @$(ld_tr) 

# The big console mode EXE
$(GSCONSOLE_XE):  $(GS_ALL) $(DEVS_ALL) $(PSOBJ)gsdll.$(OBJ) $(GLOBJ)gp_mktmp.obj $(OBJCNO) $(PSOBJ)$(GS).res $(PSSRCDIR)\dw32c.def $(PSOBJ)gsromfs$(COMPILE_INITS).$(OBJ)
	$(LINK) option map $(LCT) Name $(GSCONSOLE_XE) File $(GLOBJ)gsdll,$(OBJ) $(PSOBJ)gsromfs$(COMPILE_INITS).$(OBJ) $(GLOBJ)gp_mktmp.obj $(OBJCNOLINK) @$(ld_tr) 
!endif

# end of makefile
