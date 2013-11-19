#!/usr/bin/env python

#    Copyright (C) 2003 Artifex Software, Inc. All rights reserved.
#    All Rights Reserved.
#
# This software is provided AS-IS with no warranty, either express or
# implied.
#
# This software is distributed under license and may not be copied, modified
# or distributed except as expressly authorized under the terms of that
# license.  Refer to licensing information at http://www.artifex.com/
# or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
# San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.

# $Id: check_comments.py 8409 2007-11-27 20:43:09Z giles $

# Check for C++ style comments

import os
import re
from glob import glob

import gsconf
from gstestutils import GSTestCase, gsRunTestsMain

class GSCheckForComments(GSTestCase):

    def __init__(self, root, dirName, extensions=['*'], skip=[]):
        self.root = root
        self.dirName = dirName
        self.extensions = extensions
        self.skip = skip
        GSTestCase.__init__(self)

    def shortDescription(self):
        return "All relevant files must not have C++ style comments"\
               " (checking %s)" % (self.dirName,)

    def runTest(self):
        d, extns, skip = self.root + self.dirName, self.extensions, self.skip
        skip = map((lambda o,d=d: d + os.sep + o), skip)
        incorrect = []
        for e in extns:
            for f in glob(d + os.sep + '*.' + e):
                if f in skip or os.path.isdir(f):
                    continue
                fp = open(f, 'r')
                text_code = fp.read()
                fp.close()

                pattern = re.compile("(\\\\.)|(\")|(')|(/\*)|(\*/)|(//)")
                mi = pattern.finditer(text_code)
                try:
                    inComment = 0
		    inString = 0
                    inStringSq = 0	# single quoted string
                    inStringDq = 0	# double quoted string
                    while 1:
                        m = mi.next()
                        mstr = m.group()
			if mstr[0] == '\\':		# skip quoted characters (may be ' or ")
			    continue
                        if mstr == '"' and not inComment and not inStringSq:
                            inStringDq = not inStringDq
			    inString = inStringDq
                            continue
                        if mstr == "'" and not inComment and not inStringDq:
                            inStringSq = not inStringSq
			    inString = inStringSq
                            continue
                        if not inString and mstr == '/*':
                            inComment = 1
                            continue
                        if inComment and mstr == '*/':
                            inComment = 0
                            continue
                        if not inString and not inComment and mstr == '//':
                            incorrect.append(f)
                            break
                except StopIteration:
                    continue
                    
        if incorrect:
            incorrect = ['These %d files have C++ style comments:' % (len(incorrect),)] + incorrect

        self.failIfMessages(incorrect)

## Main stuff

checkDirs = [
    ('src', ['c', 'h'],
     # list of exempt files
     ['dwdll.h',
      'dwimg.h',
      'dwinst.h',
      'dwsetup.h',
      'dwtext.h',
      'dwuninst.h',
      'gdevhpij.c',
      'dmmain.c',
      'gdevmac.c',
      'gdevmacxf.c',
      'gdevwdib.c',
      'gp_mac.c',
      'gp_macio.c',
      'macsysstat.h'
     ])
    ]

def addTests(suite, gsroot, **args):
    for dir, extns, skip in checkDirs:
        suite.addTest(GSCheckForComments(gsroot, dir, extns, skip))

if __name__ == "__main__":
    gsRunTestsMain(addTests)
