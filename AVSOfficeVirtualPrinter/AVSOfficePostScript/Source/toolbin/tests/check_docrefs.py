#!/usr/bin/env python

#    Copyright (C) 2002-2004 Artifex Software, Inc.
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

# $Id: check_docrefs.py 8687 2008-05-02 02:19:57Z giles $

# Check that the hrefs in an HTML document mention all of a set of files.
# The requirement is that the union of all the docfiles must somewhere
# reference all the files.  Usage:
#    check_docrefs (+src | +lib | [+-]from <docfile>* | [+-]to (<directory> | <file>)*)*

# +from or +to adds files; -from or -to removes them;
# +src, or +lib execute SRC_LIST, LIB_LIST below.

# Define the Ghostscript-specific parameter lists.
SRC_LIST = [
    '+from', 'doc/Develop.htm',
    '+to', 'lib', 'src',
    '-to', '*/CVS', 'src/rinkj/CVS',
    '-to', 'src/*.mak.tcl',
    '-to', 'lib/*.upp',
    '-to', 'lib/*.ps',
    '+to', 'lib/gs_*.ps', 'lib/pdf_*.ps'
]
LIB_LIST = [
    '+from', 'doc/Psfiles.htm',
    '+to', 'examples/*.ps', 'lib/*.ps'
]

import glob, os.path, re, sys
from gstestutils import GSTestCase, gsRunTestsMain

# Utilities

# List all the files referenced from a document.
# Note that we only count files referenced as a whole, i.e., without #.

def hrefs(doc):
    prefix = os.path.dirname(doc)
    fp = open(doc, 'r')
    contents = fp.read()
    fp.close()
    pattern = re.compile('href="([^"#]*)"', re.IGNORECASE)
    return map(lambda match,pre=prefix:
                 os.path.normpath(os.path.join(pre, match)),
               re.findall(pattern, contents))

# Define a class for storing to/from information.

class DocRefs:

    def __init__(self):
        self.to = {}
        self.toDir = {}
        self.From = {}

    def doTo(self, fname, adding):
        if os.path.isdir(fname):
            self.toDir[fname] = adding
            for f in glob.glob(os.path.join(fname, '*')):
                self.doTo(f, adding)
        elif os.path.islink(fname):
            pass
        elif os.path.isfile(fname):
            self.to[os.path.normpath(fname)] = adding

    def doFrom(self, fname, adding):
        self.From[os.path.normpath(fname)] = adding

    def doFromDoc(self, docname, adding):
        for f in hrefs(docname):
            self.doFrom(f, adding)

    def cleanup(self):
        for k in self.to.keys():
            if self.to[k] == 0: del self.to[k]
        for k in self.From.keys():
            if self.From[k] == 0: del self.From[k]

# Process command line arguments and switches.

def mainArgs(arglist, root, fromProc, toProc):
    for arg in arglist:
        if arg == '+src':
            mainArgs(SRC_LIST, root, fromProc, toProc)
        elif arg == '+lib':
            mainArgs(LIB_LIST, root, fromProc, toProc)
        elif arg == '+from':
            do, adding = fromProc, 1
        elif arg == '-from':
            do, adding = fromProc, 0
        elif arg == '+to':
            do, adding = toProc, 1
        elif arg == '-to':
            do, adding = toProc, 0
        elif re.match('[+-]', arg):
            print sys.stderr >> 'Unknown switch: ' + arg
            exit(1)
        elif arg.find('*') >= 0:
            for f in glob.glob(os.path.join(root, arg)): do(f, adding)
        else:
            do(os.path.join(root, arg), adding)

class GSCheckDocRefs(GSTestCase):

    def __init__(self, root, arglist):
        self.root = root
        self.arglist = arglist
        GSTestCase.__init__(self)

    def _fromDocs(self):
        refs = DocRefs()
        mainArgs(self.arglist, self.root,
                 lambda f,b,refs=refs: refs.doFrom(f, b),
                 lambda f,b: None)
        refs.cleanup()
        return refs.From.keys()

    def shortDescription(self):
        docs = reduce(lambda a,b: a+' or '+b, self._fromDocs())
        return docs + ' must reference all relevant files.'

    def runTest(self):
        refs = DocRefs()
        mainArgs(self.arglist, self.root,
                 lambda f,b,refs=refs: refs.doFromDoc(f, b),
                 lambda f,b,refs=refs: refs.doTo(f, b))
        refs.cleanup()
        docs = self._fromDocs()
        if len(docs) == 1:
            fromFormat = docs[0] + ' fails to reference these %d files:'
            toFormat = docs[0] + ' references these %d files that do not exist:'
        else:
            fromFormat = 'These %d files are not referenced:'
            toFormat = 'These %d files are referenced but do not exist:'
        messages = []
        noFrom = []
        for f in refs.to.keys():
            if not refs.From.has_key(f):
                noFrom.append(f)
        if len(noFrom) > 0:
            messages.append(fromFormat % len(noFrom))
            noFrom.sort()
            messages += noFrom
        noTo = []
        for f in refs.to.keys():
            if not refs.to.has_key(f) and refs.toDir.has_key(os.path.dirname(f)):
                noTo.append(f)
        if len(noTo) > 0:
            messages.append(toFormat % len(noTo))
            noTo.sort()
            messages += noTo
        self.failIfMessages(messages)

# Add the tests defined in this file to a suite.

def addTests(suite, gsroot, **args):
    suite.addTest(GSCheckDocRefs(gsroot, ['+src']))
    suite.addTest(GSCheckDocRefs(gsroot, ['+lib']))

if __name__ == "__main__":
    gsRunTestsMain(addTests)
