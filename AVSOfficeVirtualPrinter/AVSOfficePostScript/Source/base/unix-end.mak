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
# $Id: unix-end.mak 8479 2008-01-07 20:58:40Z giles $
# Partial makefile common to all Unix and Desqview/X configurations.
# This is the next-to-last part of the makefile for these configurations.

# Define the rule for building standard configurations.
STDDIRS:
	@if test ! -d $(BINDIR); then mkdir $(BINDIR); fi
	@if test ! -d $(GLGENDIR); then mkdir $(GLGENDIR); fi
	@if test ! -d $(GLOBJDIR); then mkdir $(GLOBJDIR); fi
	@if test ! -d $(PSGENDIR); then mkdir $(PSGENDIR); fi
	@if test ! -d $(PSOBJDIR); then mkdir $(PSOBJDIR); fi

# Define a rule for building profiling configurations.
PGDIRS: STDDIRS
	@if test ! -d $(BINDIR)/$(PGRELDIR); then mkdir $(BINDIR)/$(PGRELDIR); fi
	@if test ! -d $(GLGENDIR)/$(PGRELDIR); then mkdir $(GLGENDIR)/$(PGRELDIR); fi
	@if test ! -d $(GLOBJDIR)/$(PGRELDIR); then mkdir $(GLOBJDIR)/$(PGRELDIR); fi
	@if test ! -d $(PSGENDIR)/$(PGRELDIR); then mkdir $(PSGENDIR)/$(PGRELDIR); fi
	@if test ! -d $(PSOBJDIR)/$(PGRELDIR); then mkdir $(PSOBJDIR)/$(PGRELDIR); fi

PGDEFS=GENOPT='-DPROFILE' CFLAGS='$(CFLAGS_PROFILE) $(GCFLAGS) $(XCFLAGS)'\
 LDFLAGS='$(XLDFLAGS) -pg' XLIBS='Xt SM ICE Xext X11'\
 BINDIR=$(BINDIR)/$(PGRELDIR)\
 GLGENDIR=$(GLGENDIR)/$(PGRELDIR) GLOBJDIR=$(GLOBJDIR)/$(PGRELDIR)\
 PSGENDIR=$(PSGENDIR)/$(PGRELDIR) PSOBJDIR=$(PSOBJDIR)/$(PGRELDIR)

pg: PGDIRS
	$(MAKE) $(PGDEFS) default

pgclean: PGDIRS
	$(MAKE) $(PGDEFS) clean

# Define a rule for building debugging configurations.
DEBUGDIRS: STDDIRS
	@if test ! -d $(BINDIR)/$(DEBUGRELDIR); then mkdir $(BINDIR)/$(DEBUGRELDIR); fi
	@if test ! -d $(GLGENDIR)/$(DEBUGRELDIR); then mkdir $(GLGENDIR)/$(DEBUGRELDIR); fi
	@if test ! -d $(GLOBJDIR)/$(DEBUGRELDIR); then mkdir $(GLOBJDIR)/$(DEBUGRELDIR); fi
	@if test ! -d $(PSGENDIR)/$(DEBUGRELDIR); then mkdir $(PSGENDIR)/$(DEBUGRELDIR); fi
	@if test ! -d $(PSOBJDIR)/$(DEBUGRELDIR); then mkdir $(PSOBJDIR)/$(DEBUGRELDIR); fi

DEBUGDEFS=GENOPT='-DDEBUG' CFLAGS='$(CFLAGS_DEBUG) $(GCFLAGS) $(XCFLAGS)'\
 BINDIR=$(BINDIR)/$(DEBUGRELDIR)\
 GLGENDIR=$(GLGENDIR)/$(DEBUGRELDIR) GLOBJDIR=$(GLOBJDIR)/$(DEBUGRELDIR)\
 PSGENDIR=$(PSGENDIR)/$(DEBUGRELDIR) PSOBJDIR=$(PSOBJDIR)/$(DEBUGRELDIR)

debug: DEBUGDIRS
	$(MAKE) $(DEBUGDEFS) default

debugclean: DEBUGDIRS
	$(MAKE) $(DEBUGDEFS) clean

# Emacs tags maintenance.

TAGS:
	etags -t $(GLSRC)*.[ch] $(PSSRC)*.[ch]
