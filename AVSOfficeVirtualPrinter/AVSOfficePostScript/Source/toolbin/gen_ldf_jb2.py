#!/bin/env python
#
# script to generate ldf_jb2.mak
# whose source sometimes changes

# $Id: gen_ldf_jb2.py 8022 2007-06-05 22:23:38Z giles $

import time, glob
import os.path

outfile="ldf_jb2.mak"

license = """#    Copyright (C) %d Artifex Software, Inc.  All rights reserved.
#
# This software is provided AS-IS with no warranty, either express or
# implied.
#
# This software is distributed under license and may not be copied,
# modified or distributed except as expressly authorized under the terms
# of the license contained in the file LICENSE in this distribution.
#
# For more information about licensing, please refer to
# http://www.ghostscript.com/licensing/. For information on
# commercial licensing, go to http://www.artifex.com/licensing/ or
# contact Artifex Software, Inc., 101 Lucas Valley Road #110,
# San Rafael, CA  94903, U.S.A., +1(415)492-9861.

# $Id: gen_ldf_jb2.py 8022 2007-06-05 22:23:38Z giles $
""" % time.gmtime().tm_year

comment = """
# makefile for Luratech ldf_jb2 library code.
# Users of this makefile must define the following:
#       SHARE_JBIG2 - whether to compile in or link to the library
#       JBIG2SRCDIR - the library source directory
#
# gs.mak and friends define the following:
#       JBIG2OBJDIR - the output obj directory
#       JBIG2GENDIR - generated (.dev) file directory
#       LDF_JB2I_ - include path for the library
#       JB2CF_ - cflags for building the library
#
# We define the ldf_jb2.dev target and its dependencies
#
# This partial makefile compiles the ldf_jb2 library for use in
# Ghostscript.
"""

common = """
LDF_JB2_MAK=$(GLSRC)%s

LDF_JB2_SRC=$(JBIG2SRCDIR)$(D)
LDF_JB2_GEN=$(JBIG2OBJDIR)$(D)
LDF_JB2_OBJ=$(JBIG2OBJDIR)$(D)

# paths to source directories
LDF_JB2_COMMON=$(JBIG2SRCDIR)$(D)source$(D)common$(D)
LDF_JB2_COMPRESS=$(JBIG2SRCDIR)$(D)source$(D)compress$(D)
""" % outfile

dev = """
ldf_jb2_OBJS=$(ldf_jb2_common_OBJS) $(ldf_jb2_compress_OBJS)
ldf_jb2_HDRS=$(ldf_jb2_common_HDRS) $(ldf_jb2_compress_HDRS)

# switch in the selected library .dev
$(LDF_JB2_GEN)ldf_jb2.dev : $(TOP_MAKEFILES) $(LDF_JB2_MAK) $(LDF_JB2_GEN)ldf_jb2_$(SHARE_JBIG2).dev
	$(CP_) $(LDF_JB2_GEN)ldf_jb2_$(SHARE_JBIG2).dev $(LDF_JB2_GEN)ldf_jb2.dev

# external link .dev
$(LDF_JB2_GEN)ldf_jb2_1.dev : $(TOP_MAKEFILES) $(LDF_JB2_MAK) $(ECHOGS_XE)
	$(SETMOD) $(LDF_JB2_GEN)ldf_jb2_1 -lib ldf_jb2

# compile our own .dev
$(LDF_JB2_GEN)ldf_jb2_0.dev : $(TOP_MAKEFILES) $(LDF_JB2_MAK) $(ECHOGS_XE) $(ldf_jb2_OBJS)
	$(SETMOD) $(LDF_JB2_GEN)ldf_jb2_0 $(ldf_jb2_common_OBJS)
	$(ADDMOD) $(LDF_JB2_GEN)ldf_jb2_0 $(ldf_jb2_compress_OBJS)

# define our specific compiler
LDF_JB2_CC=$(CC_) $(I_)$(LDF_JB2I_) $(II)$(LDF_JB2_COMMON) $(II)$(LDF_JB2_COMPRESS)$(_I) $(JB2CF_)
LDF_JB2_O=$(O_)$(LDF_JB2_OBJ)

LDF_JB2_DEP=$(AK) $(LDF_JB2_MAK)
"""

common_srcs = glob.glob("ldf_jb2/source/common/*.c")
common_srcs.sort()
common_hdrs = glob.glob("ldf_jb2/source/common/*.h")
common_hdrs.sort()
compress_srcs = glob.glob("ldf_jb2/source/compress/*.c")
compress_srcs.sort()
compress_hdrs = glob.glob("ldf_jb2/source/compress/*.h")
compress_srcs.sort()

source = """
# source files to build from the CSDK source
"""

source += """
ldf_jb2_common_OBJS = \\
"""
for file in common_srcs:
    name = os.path.splitext(os.path.basename(file))[0]
    source += "\t$(LDF_JB2_OBJ)%s.$(OBJ)" % name
    if file != common_srcs[-1]: source += "\t\t\\"
    source += "\n"

source += """
ldf_jb2_compress_OBJS = \\
"""
for file in compress_srcs:
    name = os.path.splitext(os.path.basename(file))[0]
    source += "\t$(LDF_JB2_OBJ)%s.$(OBJ)" % name
    if file != compress_srcs[-1]: source += "\t\t\\"
    source += "\n"

source += """
ldf_jb2_common_HDRS = \\
"""
for file in common_hdrs:
    source += "\t$(LDF_JB2_COMMON)%s" % os.path.basename(file)
    if file != common_hdrs[-1]: source += "\t\t\\"
    source += "\n"

source += """
ldf_jb2_compress_HDRS = \\
"""
for file in compress_hdrs:
    source += "\t$(LDF_JB2_COMPRESS)%s" % os.path.basename(file)
    if file != compress_hdrs[-1]: source += "\t\t\\"
    source += "\n"


rules = """
# explicit rules for building each source file
# for simplicity we have every source file depend on all headers

"""

for file in common_srcs:
    cfile = os.path.basename(file)
    name = os.path.splitext(cfile)[0]
    rules += "$(LDF_JB2_OBJ)%s.$(OBJ)" % name
    rules += " : $(LDF_JB2_COMMON)%s" % cfile
    rules += " $(LDF_JB2_DEP) $(ldf_jb2_HDRS)\n"
    rules += "\t$(LDF_JB2_CC) $(LDF_JB2_O)%s.$(OBJ)" % name
    rules += " $(C_) $(LDF_JB2_COMMON)%s\n" % cfile
    rules += "\n"

for file in compress_srcs:
    cfile = os.path.basename(file)
    name = os.path.splitext(cfile)[0]
    rules += "$(LDF_JB2_OBJ)%s.$(OBJ)" % name
    rules += " : $(LDF_JB2_COMPRESS)%s" % cfile
    rules += " $(LDF_JB2_DEP) $(ldf_jb2_HDRS)\n"
    rules += "\t$(LDF_JB2_CC) $(LDF_JB2_O)%s.$(OBJ)" % name
    rules += " $(C_) $(LDF_JB2_COMPRESS)%s\n" % cfile
    rules += "\n"



print license + comment + common
print source
print dev
print rules
print "# end of file"
