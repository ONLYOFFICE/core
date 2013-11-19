#  Copyright (C) 2007 Artifex Software, Inc.
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
# $Id: expat.mak 8405 2007-11-22 02:46:59Z giles $
# makefile for expat, the XML stream parsig library
#
# Users of this makefile must define the following:
#	SHARE_EXPAT - 1 to link a system (shared) library
#		      0 to compile in the referenced source,
#	EXPAT_CFLAGS - Compiler flags for building the source,
#	EXPATSRCDIR - the expat source top-level directory,
#	EXPATGENDIR - directory for intermediate generated files,
#	EXPATOBJDIR - directory for object files.

# Define the name of this makefile
EXPAT_MAK=$(GLSRC)expat.mak

# local aliases
EXPATSRC=$(EXPATSRCDIR)$(D)lib$(D)
EXPATGEN=$(EXPATGENDIR)$(D)
EXPATOBJ=$(EXPATOBJDIR)$(D)
EXPATO_=$(O_)$(EXPATOBJ)

EXPATCC=$(CC_) $(I_)$(EXPATSRC)lib$(_I) $(EXPAT_CFLAGS)

expat.clean : expat.config-clean expat.clean-not-config-clean

# would be nice if we used an explicit object list here
expat.clean-not-config-clean :
	$(RM_) $(EXPATOBJ)*.$(OBJ)

expat.config-clean :
	$(RM_) $(EXPATGEN)expat.dev
	$(RM_) $(EXPATGEN)expat_0.dev
	$(RM_) $(EXPATGEN)expat_1.dev

expat_=$(EXPATOBJ)xmlparse.$(OBJ) \
	$(EXPATOBJ)xmltok.$(OBJ) \
	$(EXPATOBJ)xmlrole.$(OBJ)

expat_xmlparse_hdrs=$(EXPATSRC)expat.h \
	$(EXPATSRC)xmlrole.h \
	$(EXPATSRC)xmltok.h

expat_xmlrole_hdrs=$(EXPATSRC)ascii.h \
	$(EXPATSRC)xmlrole.h \
	$(EXPATSRC)expat_external.h \
	$(EXPATSRC)internal.h

expat_xmltok_hdrs=$(EXPATSRC)xmltok_impl.c \
	$(EXPATSRC)xmltok_ns.c \
	$(EXPATSRC)ascii.h \
	$(EXPATSRC)asciitab.h \
	$(EXPATSRC)iasciitab.h \
	$(EXPATSRC)latin1tab.h \
	$(EXPATSRC)nametab.h \
	$(EXPATSRC)utf8tab.h \
	$(EXPATSRC)xmltok.h \
	$(EXPATSRC)xmltok_impl.h \
	$(EXPATSRC)expat_external.h \
	$(EXPATSRC)internal.h

$(EXPATOBJ)xmlparse.$(OBJ) : $(EXPATSRC)xmlparse.c $(expat_xmlparse_hdrs)
	$(EXPATCC) $(EXPATO_)xmlparse.$(OBJ) $(C_) $(EXPATSRC)xmlparse.c

$(EXPATOBJ)xmlrole.$(OBJ) : $(EXPATSRC)xmlrole.c $(expat_xmlrole_hdrs)
	$(EXPATCC) $(EXPATO_)xmlrole.$(OBJ) $(C_) $(EXPATSRC)xmlrole.c

$(EXPATOBJ)xmltok.$(OBJ) : $(EXPATSRC)xmltok.c $(expat_xmltok_hdrs)
	$(EXPATCC) $(EXPATO_)xmltok.$(OBJ) $(C_) $(EXPATSRC)xmltok.c

# Copy the target definition we want
$(EXPATGEN)expat.dev : $(TOP_MAKEFILES) $(EXPAT_MAK) \
 $(EXPATGEN)expat_$(SHARE_EXPAT).dev
	$(CP_) $(EXPATGEN)expat_$(SHARE_EXPAT).dev $(EXPATGEN)expat.dev

# Define the compiled in target
$(EXPATGEN)expat_0.dev : $(EXPAT_MAK) $(ECHOGS_XE) $(expat_)
	$(SETMOD) $(EXPATGEN)expat_0 $(expat_)

# Define the external link target
$(EXPATGEN)expat_1.dev : $(EXPAT_MAK) $(ECHOGS_XE)
	$(SETMOD) $(EXPATGEN)expat_1 -lib expat

