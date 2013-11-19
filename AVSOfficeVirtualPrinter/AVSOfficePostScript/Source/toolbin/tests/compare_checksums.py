#!/usr/bin/python
# -*- Mode: python -*-

#    Copyright (C) 2007 Artifex Software, Inc.
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

# $Id$

import os,sys
import optparse, myoptparse

if __name__ == "__main__":

    optionsParser=optparse.OptionParser()
    optionsParser.add_option('--checksum1',action='store',help="path to checksum db 1")
    optionsParser.add_option('--checksum2',action='store',help="path to checksum db 2")
    (options,arguments)=myoptparse.parseCommandLineBasic(optionsParser)
    
    myself=options.myself
    checksum1=options.checksum1
    checksum2=options.checksum2

    if not checksum1 or not checksum2:
        print myself,"both checksum files are required"
        sys.exit(1)

    try:
        print checksum1
        checksum1_db = anydbm.open(checksum1, 'r')
    except:
        checksum1_db=None
        print myself,"ERROR: cannot open "+checksum1

    try:
        print checksum2
        checksum2_db = anydbm.open(checksum2, 'r')
    except:
        checksum2_db=None
        print myself,"ERROR: cannot open "+checksum2        
    
    if not checksum1 or not checksum2:
        sys.exit(1)
    
