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

# $Id: get_baselines,v 1.3 2004/05/02 19:23:01 ray Exp $

#
# get_baselines <date>
#
# This script gets the baseline log entries since DATE.  DATE should
# be provided in ctime format (eg: 'Thu Jun 12 12:17:28 2003')

import sys
import time
import string

import gsconf

def usage():
    print "usage: get_baselines <date>"
    print
    print "Dates should be specificied in ctime format."
    print
    sys.exit(1)

if len(sys.argv) == 2:
    try:
        from_date = time.mktime(time.strptime(sys.argv[1]))
    except:
        print "ERROR: Could not parse date."
        sys.exit(1)
else:
    usage()


try:
    baseline_log = open(name)
except:
    print "ERROR: cannot open baseline log",name
    sys.exit(1)

changes = {}
for line in baseline_log.readlines():
    line = line[:-1]
    tokens = string.split(line)
    tm = time.mktime(time.strptime(string.join(tokens[:5])))
    if from_date <= tm:
        changes[string.join(tokens[5:])] = 1

if len(changes) > 0:
    for k in changes.keys():
        print k
else:
    print "No baseline updates."

   
