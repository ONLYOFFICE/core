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
# $Id: psromfs.mak 9212 2008-11-09 19:09:30Z leonardo $
# mkromfs macros for PostScript %rom% when COMPILE_INITS=1

# The following list of files needed by the interpreter is maintained here.
# This changes infrequently, but is a potential point of bitrot, but since
# unix-inst.mak uses this macro, problems should surface when testing installed
# versions.

#	Resource files go into Resource/...
#	The init files are in the %rom%Resource/Init/ directory
#       Any EXTRA_INIT_FILES go into %rom%lib/

RESOURCE_LIST=CIDFont$(D) CMap$(D) ColorSpace$(D) Decoding$(D) Encoding$(D) Font$(D) IdiomSet$(D) Init$(D) ProcSet$(D) SubstCID$(D)

#	Note: gs_cet.ps is only needed to match Adobe CPSI defaults
PS_ROMFS_ARGS=-c -P $(PSRESDIR)$(D) -d Resource/ $(RESOURCE_LIST) \
   -d lib/ -P $(PSLIBDIR)$(D) $(EXTRA_INIT_FILES)

# We'd really like to have all of the files in Resourc/Init/ be dependencies
# for COMPILE_INITS=1, but we settle for just the main one. "touch" it if
# you change any Resource/Init files to force re-build of the %rom% data.
PS_ROMFS_DEPS=$(PSRESDIR)$(D)Init$(D)$(GS_INIT)
