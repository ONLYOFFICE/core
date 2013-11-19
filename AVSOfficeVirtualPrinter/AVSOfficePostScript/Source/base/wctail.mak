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
# $Id: wctail.mak 8954 2008-08-08 04:22:38Z ray $
# wctail.mak
# Last part of Watcom C/C++ makefile common to MS-DOS and MS Windows.

# Define the name of this makefile.
WCTAIL_MAK=$(GLSRCDIR)\wctail.mak

# Include the generic makefiles, except for devs.mak, contrib.mak, int.mak
#!include $(COMMONDIR)/watcdefs.mak
#!include $(COMMONDIR)/pcdefs.mak
#!include $(COMMONDIR)/generic.mak
!include $(GLSRCDIR)\version.mak
!include $(GLSRCDIR)\gs.mak
!include $(GLSRCDIR)\lib.mak
!include $(GLSRCDIR)\jpeg.mak
# zlib.mak must precede libpng.mak
!include $(GLSRCDIR)\zlib.mak
!include $(GLSRCDIR)\libpng.mak
!include $(GLSRCDIR)\jbig2.mak
!include $(GLSRCDIR)\icclib.mak
!include $(GLSRCDIR)\ijs.mak

# -------------------------- Auxiliary programs --------------------------- #

temp_tr=$(GLOBJ)_temp_.tr

$(ECHOGS_XE): $(AUXGEN)echogs.$(OBJ)
	echo OPTION STUB=$(STUB) >$(temp_tr)
	echo $(LIBPATHS) >>$(temp_tr)
	$(LINK) @$(temp_tr) FILE $(AUXGEN)echogs

$(AUXGEN)echogs.$(OBJ): $(GLSRC)echogs.c
	$(CCAUX) $(GLSRC)echogs.c $(O_)$(AUXGEN)echogs.$(OBJ)

$(GENARCH_XE): $(AUXGEN)genarch.$(OBJ)
	echo $(LIBPATHS) >$(temp_tr)
	$(LINK) @$(temp_tr) FILE $(AUXGEN)genarch

$(AUXGEN)genarch.$(OBJ): $(GLSRC)genarch.c $(stdpre_h)
	$(CCAUX) $(GLSRC)genarch.c $(O_)$(AUXGEN)genarch.$(OBJ)

$(GENCONF_XE): $(AUXGEN)genconf.$(OBJ)
	echo OPTION STUB=$(STUB) >$(temp_tr)
	echo OPTION STACK=8k >>$(temp_tr)
	echo $(LIBPATHS) >>$(temp_tr)
	$(LINK) @$(temp_tr) FILE $(AUXGEN)genconf

$(AUXGEN)genconf.$(OBJ): $(GLSRC)genconf.c $(stdpre_h)
	$(CCAUX) $(GLSRC)genconf.c $(O_)$(AUXGEN)genconf.$(OBJ)

$(GENDEV_XE): $(AUXGEN)gendev.$(OBJ)
	echo OPTION STUB=$(STUB) >$(temp_tr)
	echo OPTION STACK=8k >>$(temp_tr)
	echo $(LIBPATHS) >>$(temp_tr)
	$(LINK) @$(temp_tr) FILE $(AUXGEN)gendev

$(AUXGEN)gendev.$(OBJ): $(GLSRC)gendev.c $(stdpre_h)
	$(CCAUX) $(GLSRC)gendev.c $(O_)$(AUXGEN)gendev.$(OBJ)

$(GENINIT_XE): $(AUXGEN)geninit.$(OBJ)
	echo OPTION STUB=$(STUB) >$(temp_tr)
	echo OPTION STACK=8k >>$(temp_tr)
	echo $(LIBPATHS) >>$(temp_tr)
	$(LINK) @$(temp_tr) FILE $(AUXGEN)geninit

$(AUXGEN)geninit.$(OBJ): $(GLSRC)geninit.c $(stdpre_h)
	$(CCAUX) $(GLSRC)geninit.c $(O_)$(AUXGEN)geninit.$(OBJ)

# No special gconfig_.h is needed.
# Watcom `make' supports output redirection.
$(gconfig__h): $(WCTAIL_MAK)
	echo /* This file deliberately left blank. */ >$(gconfig__h)
