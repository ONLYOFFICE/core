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
# $Id: winplat.mak 8022 2007-06-05 22:23:38Z giles $
# Common makefile section for builds on 32-bit MS Windows, including the
# Watcom MS-DOS build.

# Define the name of this makefile.
WINPLAT_MAK=$(GLSRC)winplat.mak

# Define generic Windows-specific modules.

winplat_=$(GLOBJ)gp_ntfs.$(OBJ) $(GLOBJ)gp_win32.$(OBJ)
$(GLD)winplat.dev : $(WINPLAT_MAK) $(ECHOGS_XE) $(winplat_)
	$(SETMOD) $(GLD)winplat $(winplat_)

$(GLOBJ)gp_ntfs.$(OBJ): $(GLSRC)gp_ntfs.c $(AK)\
 $(dos__h) $(memory__h) $(stdio__h) $(string__h) $(windows__h)\
 $(gp_h) $(gpmisc_h) $(gsmemory_h) $(gsstruct_h) $(gstypes_h) $(gsutil_h)
	$(GLCCWIN) $(GLO_)gp_ntfs.$(OBJ) $(C_) $(GLSRC)gp_ntfs.c

$(GLOBJ)gp_win32.$(OBJ): $(GLSRC)gp_win32.c $(AK)\
 $(dos__h) $(malloc__h) $(stdio__h) $(string__h) $(windows__h)\
 $(gp_h) $(gsmemory_h) $(gstypes_h)
	$(GLCCWIN) $(GLO_)gp_win32.$(OBJ) $(C_) $(GLSRC)gp_win32.c

# Define the Windows thread / synchronization module.

winsync_=$(GLOBJ)gp_wsync.$(OBJ)
$(GLD)winsync.dev : $(WINPLAT_MAK) $(ECHOGS_XE) $(winsync_)
	$(SETMOD) $(GLD)winsync $(winsync_)
	$(ADDMOD) $(GLD)winsync -replace $(GLD)nosync

$(GLOBJ)gp_wsync.$(OBJ): $(GLSRC)gp_wsync.c $(AK)\
 $(dos__h) $(malloc__h) $(stdio__h) $(string__h) $(windows__h)\
 $(gp_h) $(gsmemory_h) $(gstypes_h)
	$(GLCCWIN) $(GLO_)gp_wsync.$(OBJ) $(C_) $(GLSRC)gp_wsync.c
