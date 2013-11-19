#!/usr/bin/env python

#    Copyright (C) 2002-2006 Artifex Software, Inc.  All rights reserved.
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

# $Id: check_source.py 8409 2007-11-27 20:43:09Z giles $

# Check basic hygiene of source code.

import os
from gstestutils import GSTestCase, gsRunTestsMain

################ Check that every file certain to be source has an
################ Id or RCSfile line.

class GSCheckForIdLines(GSTestCase):

    def __init__(self, root, dirName, extensions = ['*'], skip = []):
        self.root = root
        self.dirName = dirName
        self.extensions = extensions
        self.skip = skip
        GSTestCase.__init__(self)

    def shortDescription(self):
        return "All relevant files must have correct Id or RCSfile lines. (checking %s/)" % self.dirName

    def runTest(self):
        import re, glob
        pattern = re.compile("[$][ ]*(Id|RCSfile):[ ]*([^, \t\$]+)")
        d, extns, omit = self.root + self.dirName, self.extensions, self.skip
        omit = map((lambda o,d=d: d+'/'+o), omit)
        missing = []
        incorrect = []
        for e in extns:
            for f in glob.glob(d + '/*.' + e):
                if f in omit or f[-8:] == '.mak.tcl' or os.path.isdir(f):
                    continue
                fp = open(f, 'r')
                contents = fp.read(10000)
                fp.close()
                if len(contents) < 20:  # skip very short files
                    continue
                found = pattern.search(contents)
                if found == None:
                    missing.append(f)
                elif found.group(2) != os.path.basename(f):
                    incorrect.append(f + ' has ' + found.group(1) + ': '+ found.group(2))                    
        if missing:
            missing = ['These %d files have no Id or RCSfile line:' % len(missing)] + missing
        if incorrect:
            incorrect = ['These %d files have an incorrect Id or RCSfile line:' % len(incorrect)] + incorrect
        self.failIfMessages(missing + incorrect)

################ Check that all .h files have Ghostscript standard
################ double-inclusion protection.

class GSCheckDoubleInclusion(GSTestCase):

    def __init__(self, root, skip = []):
        self.root = root
        self.skip = map(lambda f,r=self.root: r+f, skip)
        GSTestCase.__init__(self)

    def runTest(self):
        """All .h files must have double-inclusion protection."""
        import re, glob
        messages = []
        for fname in glob.glob(self.root + 'src/*.h'):
            if fname in self.skip:
                continue
            # Double inclusion protection is required, but the name of the
            # preprocessor symbol doesn't have to follow the Ghostscript
            # convention.
            fp = open(fname, 'r')
            found = 0
            while 1:
                line = fp.readline()
                if not line:
                    break               # EOF
                line = line.strip()
                if not line:
                    continue            # skip blank lines
                if line[:2] == '//':
                    continue            # skip single-line comments
                if re.match(r'\s*\/\*', line): # skip comments
                    while not re.search(r'\*\/', line):
                        line = fp.readline()
                        if not line:
                            break       # EOF
                    if not line:
                        break           # EOF
                    continue
                m = re.match(r'#[ ]*if(ndef[ ]+([a-zA-Z0-9_]+)|[ ]+!defined\(([a-zA-Z0-9_]+)\))$', line)
                if m:
                    # Check for #define in the next line.
                    sym = m.group(2) or m.group(3)
                    line = fp.readline().strip()
                    if re.match(r'#[ ]*define[ ]+' + sym + r'[\s]*$', line):
                        found = 1
                        break
                break                   # no protection
            fp.close()
            if not found:
                messages.append(fname)
        if messages:
            messages = ['These %d files do not have double-inclusion protection:' % len(messages)] + messages
        self.failIfMessages(messages)

################ Main program

gsSourceSets = [
    ('doc', ['*'], ['Changes.htm', 'gsdoc.el', 'FTL.txt']),
    ('lib', ['eps', 'ps'], []),
    ('man', ['*'], []),
    ('src', ['c', 'cpp', 'h', 'mak'], []),
    ('toolbin', ['*'], ['pre.chk'])
    ]
gsDoubleInclusionOK = [
    'src/gconf.h',
    'src/gxdtfill.h',
    'src/gxfillsl.h',
    'src/gxfilltr.h',
    'src/gxfillts.h',
    'src/gxsamplp.h',
    'src/gdevpdfb.h',
    'src/idicttpl.h'
    ]

# Add the tests defined in this file to a suite.

def addTests(suite, gsroot, **args):
    for dir, extns, skip in gsSourceSets:
        suite.addTest(GSCheckForIdLines(gsroot, dir, extns, skip))
    suite.addTest(GSCheckDoubleInclusion(gsroot, gsDoubleInclusionOK))

if __name__ == "__main__":
    gsRunTestsMain(addTests)
