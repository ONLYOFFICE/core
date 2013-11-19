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
# $Id: macos-mcp.mak 9334 2009-01-08 09:17:18Z ghostgum $
# Makefile for CodeWarrior XML project file creation from Darwin/MacOSX.

# Run this file through make on MacOS X (or any other system with shell
# scripting) to produce an xml project file. Import that file into
# Metrowerks Codewarrior to perform the actual build targeting MacOS.

# ------------------------------- Options ------------------------------- #

####### The following are the only parts of the file you should need to edit.

# Define the directory for the final executable, and the
# source, generated intermediate file, and object directories
# for the graphics library (GL) and the PostScript/PDF interpreter (PS).

BINDIR=./bin
GLSRCDIR=./base
GLGENDIR=./obj
GLOBJDIR=./obj
PSSRCDIR=./psi
PSLIBDIR=./lib
PSRESDIR=./Resource
PSGENDIR=./obj
PSOBJDIR=./obj

# Do not edit the next group of lines.

include $(GLSRCDIR)/version.mak
DD=$(GLGENDIR)/
GLD=$(GLGENDIR)/
PSD=$(PSGENDIR)/

# ------ Generic options ------ #

# Define the default directory/ies for the runtime
# initialization, resource and font files.  Separate multiple directories with a :.

GS_LIB_DEFAULT=:Resource/Init:lib,:Resource/Font,:fonts,:examples

GS_DOCDIR=:doc

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

GENOPT=

# Define the name of the executable file. Not used for the CW project, don't change it

GS=macgs

# Define the platform name.

PLATFORM=macos_


# don't use any shared libraries, they are compiled into the Ghostscript Lib

SHARE_LIBPNG=0
SHARE_JPEG=0
SHARE_ZLIB=0
SHARE_JBIG2=0
SHARE_JPX=0

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

JBIG2_LIB=jbig2dec
JBIG2SRCDIR=jbig2dec

# Define the japser library source location.
# See jasper.mak for more information.

JPX_LIB=jasper
JPXSRCDIR=jasper

# Define the directory where the icclib source are stored.
# See icclib.mak for more information

ICCSRCDIR=icclib

# IJS has not been ported to MacOS Classic. If you do the port,
# you'll need to set these values. You'll also need to
# include the ijs.mak makefile (right after icclib.mak).
#
# Define the directory where the ijs source is stored,
# and the process forking method to use for the server.
# See ijs.mak for more information.

#IJSSRCDIR=ijs
#IJSEXECTYPE=unix

# Define the directory where the imdi library source is stored.
# See devs.mak for more information

IMDISRCDIR=imdi

# ------ Platform-specific options ------ #

# Define the name of the C compiler.

# we don't want to build, so CC and LD don't have to do anything, just echo the arguments
CC=echo
CFLAGS=
CCLD=
LDFLAGS=
EXTRALIBS=
STDLIBS=
# Define the .dev module that implements thread and synchronization
# primitives for this platform.
# No real sync in MacOS 8/9, so leave it at nosync

SYNC=nosync

# ------ Devices and features ------ #

# Choose the language feature(s) to include.  See gs.mak for details.

FEATURE_DEVS=$(PSD)psl3.dev $(PSD)pdf.dev $(PSD)dpsnext.dev $(PSD)ttfont.dev $(PSD)epsf.dev $(GLD)pipe.dev $(PSD)macres.dev $(PSD)macpoll.dev $(PSD)jbig2.dev
#FEATURE_DEVS=$(PSD)psl3.dev $(PSD)pdf.dev
#FEATURE_DEVS=$(PSD)psl3.dev $(PSD)pdf.dev $(PSD)dpsnext.dev $(PSD)ttfont.dev $(PSD)rasterop.dev $(GLD)pipe.dev
# The following is strictly for testing.
FEATURE_DEVS_ALL=$(PSD)psl3.dev $(PSD)pdf.dev $(PSD)dpsnext.dev $(PSD)ttfont.dev $(PSD)rasterop.dev $(PSD)double.dev $(PSD)trapping.dev $(PSD)stocht.dev $(GLD)pipe.dev $(GLD)macres.dev $(PSD)jbig2.dev $(PSD)jpx.dev $(PSD)macpoll.dev
#FEATURE_DEVS=$(FEATURE_DEVS_ALL)

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

DEVICE_DEVS=$(DD)macos.dev $(DD)macos_.dev $(DD)display.dev

#DEVICE_DEVS1=
#DEVICE_DEVS2=
#DEVICE_DEVS3=
#DEVICE_DEVS4=
#DEVICE_DEVS5=
#DEVICE_DEVS6=
#DEVICE_DEVS7=
#DEVICE_DEVS8=
#DEVICE_DEVS9=
#DEVICE_DEVS10=
#DEVICE_DEVS11=
#DEVICE_DEVS12=
#DEVICE_DEVS13=
#DEVICE_DEVS14=
#DEVICE_DEVS15=
#DEVICE_DEVS16=
#DEVICE_DEVS17=
#DEVICE_DEVS18=
#DEVICE_DEVS19=
#DEVICE_DEVS20=

DEVICE_DEVS1=$(DD)bmpmono.dev $(DD)bmpgray.dev $(DD)bmpsep1.dev $(DD)bmpsep8.dev $(DD)bmp16.dev $(DD)bmp256.dev $(DD)bmp16m.dev $(DD)bmp32b.dev
DEVICE_DEVS2=
DEVICE_DEVS3=$(DD)deskjet.dev $(DD)djet500.dev $(DD)laserjet.dev $(DD)ljetplus.dev $(DD)ljet2p.dev $(DD)ljet3.dev $(DD)ljet3d.dev $(DD)ljet4.dev $(DD)ljet4d.dev $(DD)lj5mono.dev $(DD)lj5gray.dev
DEVICE_DEVS4=$(DD)cdeskjet.dev $(DD)cdjcolor.dev $(DD)cdjmono.dev $(DD)cdj550.dev $(DD)pj.dev $(DD)pjxl.dev $(DD)pjxl300.dev
DEVICE_DEVS5=$(DD)uniprint.dev
DEVICE_DEVS6=$(DD)bj10e.dev $(DD)bj200.dev $(DD)bjc600.dev $(DD)bjc800.dev
DEVICE_DEVS7=$(DD)faxg3.dev $(DD)faxg32d.dev $(DD)faxg4.dev
DEVICE_DEVS8=$(DD)pcxmono.dev $(DD)pcxgray.dev $(DD)pcx16.dev $(DD)pcx256.dev $(DD)pcx24b.dev $(DD)pcxcmyk.dev
DEVICE_DEVS9=$(DD)pbm.dev $(DD)pbmraw.dev $(DD)pgm.dev $(DD)pgmraw.dev $(DD)pgnm.dev $(DD)pgnmraw.dev $(DD)pnm.dev $(DD)pnmraw.dev $(DD)ppm.dev $(DD)ppmraw.dev $(DD)pkm.dev $(DD)pkmraw.dev $(DD)pksm.dev $(DD)pksmraw.dev $(DD)pamcmyk32.dev
DEVICE_DEVS10=$(DD)tiffcrle.dev $(DD)tiffg3.dev $(DD)tiffg32d.dev $(DD)tiffg4.dev $(DD)tifflzw.dev $(DD)tiffpack.dev
DEVICE_DEVS11=$(DD)tiff12nc.dev $(DD)tiff24nc.dev $(DD)tiffgray.dev $(DD)tiff32nc.dev $(DD)tiffsep.dev
DEVICE_DEVS12=$(DD)psmono.dev $(DD)psgray.dev $(DD)psrgb.dev $(DD)bit.dev $(DD)bitrgb.dev $(DD)bitcmyk.dev
DEVICE_DEVS13=$(DD)pngmono.dev $(DD)pnggray.dev $(DD)png16.dev $(DD)png256.dev $(DD)png16m.dev
DEVICE_DEVS14=$(DD)jpeg.dev $(DD)jpeggray.dev $(DD)jpegcmyk.dev
DEVICE_DEVS15=$(DD)pdfwrite.dev $(DD)pswrite.dev $(DD)ps2write.dev $(DD)epswrite.dev $(DD)txtwrite.dev $(DD)pxlmono.dev $(DD)pxlcolor.dev

DEVICE_DEVS16=$(DD)bbox.dev
DEVICE_DEVS17=
DEVICE_DEVS18=
DEVICE_DEVS19=
DEVICE_DEVS20=$(DD)cljet5.dev $(DD)cljet5c.dev

# ---------------------------- End of options --------------------------- #

# Define the name of the partial makefile that specifies options --
# used in dependencies.

MAKEFILE=$(GLSRCDIR)/macos-mcp.mak
TOP_MAKEFILES=


# Define the auxilary program dependency. (we don't have any)

AK=

# Define the compilation rules and flags.

CCFLAGS=$(GENOPT) $(CFLAGS)
CC_=$(CC) $(CCFLAGS)
# define CCAUX as the real cc compiler, we use this to build the code generation tools
CCAUX=cc
# These are the specific warnings we have to turn off to compile those
# specific few files that need this.  We may turn off others in the future.
CC_NO_WARN=$(CC_) -Wno-cast-qual -Wno-traditional

# ---------------- End of platform-specific section ---------------- #

include $(GLSRCDIR)/unixhead.mak

# Redefine the platform name before going on
PLATFORM=macos_

include $(GLSRCDIR)/gs.mak
# psromfs.mak must precede lib.mak
include $(PSSRCDIR)/psromfs.mak
include $(GLSRCDIR)/lib.mak
include $(PSSRCDIR)/int.mak
include $(GLSRCDIR)/jpeg.mak
# zlib.mak must precede libpng.mak
include $(GLSRCDIR)/zlib.mak
include $(GLSRCDIR)/libpng.mak
include $(GLSRCDIR)/jbig2.mak
include $(GLSRCDIR)/jasper.mak
include $(GLSRCDIR)/ldf_jb2.mak
include $(GLSRCDIR)/lwf_jp2.mak
include $(GLSRCDIR)/icclib.mak
include $(GLSRCDIR)/devs.mak
include $(GLSRCDIR)/contrib.mak
#include $(GLSRCDIR)/unix-aux.mak
#include $(GLSRCDIR)/unixlink.mak
#include $(GLSRCDIR)/unix-dll.mak
include $(GLSRCDIR)/unix-end.mak
#include $(GLSRCDIR)/unixinst.mak



# ------------------------------------------------------------------- #
# ------------------------------------------------------------------- #



# --------------- mac device sources (move to devs.mak??) ----------- #

gdevmac_h=$(GLSRC)gdevmac.h
gdevmacttf_h=$(GLSRC)gdevmacttf.h

macos_=$(GLOBJ)gdevmac.$(OBJ) 

$(GLD)macos.dev: $(macos_) $(GLOBJ)gdevemap.$(OBJ)
	$(SETDEV) $(PSD)macos $(macos_)

$(GLOBJ)gdevmac.$(OBJ): $(GLSRC)gdevmac.c $(gp_mac_h) $(gdevmac_h)
$(GLOBJ)gdevemap.$(OBJ): $(GLSRC)gdevemap.c
$(GLOBJ)gdevmacxf.$(OBJ):  $(GLSRC)gdevmacxf.c

# -------------------- Platform specific sources -------------------- #

gp_mac_h=$(GLSRC)gp_mac.h
gconfig_h=$(GLOBJ)gconfig.h

macsystypes_h=$(GLSRC)macsystypes.h
systypes_h=$(GLOBJ)sys/types.h

$(GLOBJ)gp_mac.$(OBJ): $(GLSRC)gp_mac.c
$(GLOBJ)gp_macio.$(OBJ): $(GLSRC)gp_macio.c $(gx_h) $(gp_h) $(gpmisc_h)
$(GLOBJ)gp_stdin.$(OBJ): $(GLSRC)gp_stdin.c $(AK) $(stdio__h) $(gx_h) $(gp_h)

# ------------------------------------------------------------------- #

MAC1=$(GLOBJ)gp_macio.$(OBJ) $(GLOBJ)gp_mac.$(OBJ) $(GLOBJ)gdevmacxf.$(OBJ) $(GLOBJ)gp_stdin.$(OBJ)
MAC2=$(GLOBJ)gp_getnv.$(OBJ) $(GLOBJ)gp_paper.$(OBJ) $(GLOBJ)gp_nsync.$(OBJ) $(GLOBJ)gdevemap.$(OBJ) $(GLOBJ)gsdll.$(OBJ)

$(GLD)macos_.dev: $(MAC1)
	$(SETMOD) $(DD)macos_ $(MAC1) $(MAC)
	$(ADDMOD) $(DD)macos_ -obj $(MAC2)
	# uncomment the line below if you need the legacy macstdio device
	#$(ADDMOD) $(DD)macos_ -iodev macstdio  # macstdio does not work with MSL!!!

# Define polling as a separable feature because it is not needed by the gslib.
macpoll_=$(GLOBJ)gp_macpoll.$(OBJ)
$(GLD)macpoll.dev: $(ECHOGS_XE) $(macpoll_)
	$(SETMOD) $(GLD)macpoll $(macpoll_)

$(GLOBJ)gp_macpoll.$(OBJ): $(GLSRC)gp_macpoll.c $(AK)\
 $(gx_h) $(gp_h) $(gpcheck_h) $(iapi_h) $(iref_h) $(iminst_h) $(imain_h)


# ------------------------------------------------------------------- #

# -------------------------- Auxiliary files ------------------------ #

# No special gconfig_.h is needed.
$(gconfig__h):
	echo "/* This file deliberately left blank. */" > $(gconfig__h)

$(ECHOGS_XE): $(GLSRC)echogs.c $(AK) $(ECHOGS_DEPS)
	$(CCAUX) $(I_)$(GLSRCDIR)$(_I) $(O_)$(ECHOGS_XE) $(GLSRC)echogs.c

$(GENARCH_XE): $(GLSRC)genarch.c $(AK) $(GENARCH_DEPS)
	$(CCAUX) $(I_)$(GLSRCDIR)$(_I) $(O_)$(GENARCH_XE) $(GLSRC)genarch.c

$(GENCONF_XE): $(GLSRC)genconf.c $(AK) $(GENCONF_DEPS)
	$(CCAUX) $(I_)$(GLSRCDIR)$(_I) $(O_)$(GENCONF_XE) $(GLSRC)genconf.c

$(GENDEV_XE): $(GLSRC)gendev.c $(AK) $(GENDEV_DEPS)
	$(CCAUX) $(I_)$(GLSRCDIR)$(_I) $(O_)$(GENDEV_XE) $(GLSRC)gendev.c

$(GENHT_XE): $(GLSRC)genht.c $(AK) $(GENHT_DEPS)
	$(CCAUX) $(GENHT_CFLAGS) $(O_)$(GENHT_XE) $(GLSRC)genht.c

$(GENINIT_XE): $(GLSRC)geninit.c $(AK) $(GENINIT_DEPS)
	$(CCAUX) $(I_)$(GLSRCDIR)$(_I) $(O_)$(GENINIT_XE) $(GLSRC)geninit.c


MKROMFS_OBJS=$(MKROMFS_ZLIB_OBJS) \
 (GLOBJ)gscdefs.$(OBJ) $(GLOBJ)gpmisc.$(OBJ) $(GLOBJ)gp_macio.$(OBJ) $(GLOBJ)gp_mac.$(OBJ)
$(MKROMFS_XE): $(GLSRC)mkromfs.c $(MKROMFS_COMMON_DEPS) $(MKROMFS_OBJS)
	$(CCAUX) $(GENOPT) $(CFLAGS_DEBUG) $(I_)$(GLSRCDIR)$(_I) $(I_)$(GLOBJ)$(_I) $(I_)$(ZSRCDIR)$(_I) $(GLSRC)mkromfs.c $(O_)$(MKROMFS_XE) $(MKROMFS_OBJS) -lm

# ---------------------- CW XML Project file ------------------------ #

ldt_tr=$(PSOBJ)ldt.tr
CWPROJ_XML=./ghostscript.mcp.xml

$(CWPROJ_XML): $(gconfigd_h)
	-mkdir -p obj/sys
	$(CP_) $(macsystypes_h) $(systypes_h)
	$(SH) $(GLSRC)macgenmcpxml.sh `$(CAT) $(ld_tr)` >  $(CWPROJ_XML)
	$(CP_) $(GLSRC)gconf.c $(GLOBJ)gconfig.c
	$(CP_) $(GLSRC)iconf.c $(GLOBJ)iconfig.c
	$(CP_) $(GLSRC)gscdef.c $(GLOBJ)gscdefs.c
	/Developer/Tools/SetFile -c CWIE -t TEXT $(CWPROJ_XML)

$(GS_XE): $(ld_tr) $(ECHOGS_XE) $(XE_ALL) $(CWPROJ_XML) $(PSOBJ)gsromfs$(COMPILE_INITS).$(OBJ)

# ------------------------------------------------------------------- #

# This has to come last so it won't be taken as the default target.
$(AK):
	if ( $(CCAUX) --version | egrep "^2\.7\.([01]|2(\.[^1-9]|$$))" >/dev/null ); then echo -Dconst= >$(AK); else echo -Wcast-qual -Wwrite-strings >$(AK); fi

