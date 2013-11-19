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
# $Id: unixlink.mak 8559 2008-02-29 08:13:08Z ray $
# Partial makefile common to all Unix configurations.
# This part of the makefile contains the linking steps.

# Define the name of this makefile.
UNIXLINK_MAK=$(GLSRC)unixlink.mak

# The following prevents GNU make from constructing argument lists that
# include all environment variables, which can easily be longer than
# brain-damaged system V allows.

.NOEXPORT:

# ----------------------------- Main program ------------------------------ #

### Interpreter main program

INT_ARCHIVE_ALL=$(PSOBJ)imainarg.$(OBJ) $(PSOBJ)imain.$(OBJ) \
 $(GLOBJ)gconfig.$(OBJ) $(GLOBJ)gscdefs.$(OBJ)
XE_ALL=$(PSOBJ)gs.$(OBJ) $(INT_ARCHIVE_ALL) $(INT_ALL) $(DEVS_ALL)

# Build a library archive for the entire interpreter.
# This is not used in a standard build.
liar_tr=$(GLOBJ)liar.tr
GS_A=$(GS).a
$(GS_A): $(obj_tr) $(ECHOGS_XE) $(INT_ARCHIVE_ALL) $(INT_ALL) $(DEVS_ALL)
	rm -f $(GS_A)
	$(ECHOGS_XE) -w $(liar_tr) -n - $(AR) $(ARFLAGS) $(GS_A)
	$(ECHOGS_XE) -a $(liar_tr) -n -s $(INT_ARCHIVE_ALL) -s
	cat $(obj_tr) >>$(liar_tr)
	$(ECHOGS_XE) -a $(liar_tr) -s -
	$(SH) <$(liar_tr)
	$(RANLIB) $(GS_A)

# Here is the final link step.  The stuff with LD_RUN_PATH is for SVR4
# systems with dynamic library loading; I believe it's harmless elsewhere.
# The resetting of the environment variables to empty strings is for SCO Unix,
# which has limited environment space.
ldt_tr=$(PSOBJ)ldt.tr
$(GS_XE): $(ld_tr) $(ECHOGS_XE) $(XE_ALL) $(PSOBJ)gsromfs$(COMPILE_INITS).$(OBJ)
	$(ECHOGS_XE) -w $(ldt_tr) -n - $(CCLD) $(LDFLAGS) -o $(GS_XE)
	$(ECHOGS_XE) -a $(ldt_tr) -n -s $(PSOBJ)gsromfs$(COMPILE_INITS).$(OBJ) $(PSOBJ)gs.$(OBJ) -s
	cat $(ld_tr) >>$(ldt_tr)
	$(ECHOGS_XE) -a $(ldt_tr) -s - $(EXTRALIBS) $(STDLIBS)
	if [ x$(XLIBDIR) != x ]; then LD_RUN_PATH=$(XLIBDIR); export LD_RUN_PATH; fi; \
	XCFLAGS= XINCLUDE= XLDFLAGS= XLIBDIRS= XLIBS= \
	FEATURE_DEVS= DEVICE_DEVS= DEVICE_DEVS1= DEVICE_DEVS2= DEVICE_DEVS3= \
	DEVICE_DEVS4= DEVICE_DEVS5= DEVICE_DEVS6= DEVICE_DEVS7= DEVICE_DEVS8= \
	DEVICE_DEVS9= DEVICE_DEVS10= DEVICE_DEVS11= DEVICE_DEVS12= \
	DEVICE_DEVS13= DEVICE_DEVS14= DEVICE_DEVS15= DEVICE_DEVS16= \
	DEVICE_DEVS17= DEVICE_DEVS18= DEVICE_DEVS19= DEVICE_DEVS20= \
	DEVICE_DEVS_EXTRA= \
	$(SH) <$(ldt_tr)
