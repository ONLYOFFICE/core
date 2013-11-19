#!/usr/bin/env python
# -*- Mode: python -*-

#    Copyright (C) 2001 Artifex Software Inc.
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

# $Id: dump_testdb,v 1.7 2004/07/14 18:21:24 ray Exp $

#
# dump_baseline.py [<dbfile>]
#
# dumps (prints out) the contents of the baselinedb

import string, sys, anydbm, gsconf, os, optparse, myoptparse

if __name__ == "__main__":

    optionsParser=optparse.OptionParser()
    (options,arguments)=myoptparse.parseCommandLineBasic(optionsParser)

    if len(arguments) == 1:
        filename=arguments.pop(0)
    else:
        filename=gsconf.baselinedb

    if not os.path.exists(filename):
        print "cannot open",filename
        sys.exit(1)

    print "opening ", filename
    db = anydbm.open(filename)

    if db:
        keys=db.keys()
        keys.sort()

        count=0
        for k in keys:
            count+=1
            if options.verbose:
                print '%s %s' % (db[k], k)

        print options.myself,"number of entries",count,"in database",filename

    else:
        print options.myself,"no entries in database",filename
