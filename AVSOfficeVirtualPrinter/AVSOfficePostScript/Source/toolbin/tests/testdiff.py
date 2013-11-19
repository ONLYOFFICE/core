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

# $Id$

#
# testdiff <start date> [<end date>]
#
# this script provides the difference between two sets of regression results.
# if end date is omitted, the current date will be used.
#
# dates should be specified as YYYYMMDD

version="2.1"

import sys, os
import re
import time
import anydbm
import gsconf

def usage():
    print "testdiff <old_name> [<new_name>]",version
    print
    print "if new_name is omitted, the current date will be used."
    print
    sys.exit(1)

args=sys.argv[:]
myself=os.path.basename(args.pop(0))

if len(sys.argv) == 2:
    old_name = sys.argv[1]
    new_name = time.strftime("%Y%m%d", time.localtime(time.time()))
elif len(sys.argv) == 3:
    old_name = sys.argv[1]
    new_name = sys.argv[2]
elif len(sys.argv) == 4:
    old_name = sys.argv[1]
    new_name = sys.argv[2]
    daily_db = sys.argv[3]
else:
    usage()

old_dbname = gsconf.dailydir + old_name + '.db'
new_dbname = gsconf.dailydir + new_name + '.db'
baseline_db_path = gsconf.baselinedb

print myself,gsconf.dailydir,old_name,new_name,baseline_db_path

try:
    old_db = anydbm.open(old_dbname, 'r')
except:
    old_db=None
    print myself,"ERROR: Test results for %s %s were not found." % (old_name,old_dbname)
    
try:
    new_db = anydbm.open(new_dbname, 'r')
except:
    new_db=None
    print myself,"ERROR: Test results for %s %s were not found." % (new_name,new_dbname)

try:
    baseline_db = anydbm.open(baseline_db_path, 'r')
except:
    baseline_db=None
    print myself,"ERROR: Baseline database could not be opened %s." % (baseline_db_path)

if not old_db:
    print myself,"empty checksum database",old_dbname
if not new_db:
    print myself,"empty checksum database",new_dbname
if not baseline_db:
    print myself,"empty checksum database",baseline_dbname

if not new_db or not old_db or not baseline_db:
    print myself,"sys.exit(1) called"
    sys.exit(1)

# find any checksum in new that is a new mis-match from baseline, then compare with old

normal_re = re.compile("^(.*?)\.(p[bgpk]mraw)\.(\d+)\.(\d+)$")
pdfwrite_re = re.compile("^(.*?)\.(ps|pdf)\.pdf\.(p[bgpk]mraw)\.(\d+)\.(\d+)$")

new_diffs = []
all_diffs = []

keys = new_db.keys()
for k in keys:
    if k in baseline_db.keys():
        if new_db[k] != baseline_db[k]:

            all_diffs.append(k)

            if k in old_db.keys():
                if old_db[k] == baseline_db[k]:
                    new_diffs.append(k)  # new mismatch
                    continue

list = []
for d in new_diffs:
    type = ''
    filename = ''
    device = ''
    dpi = 0
    banded = 0
    
    m = pdfwrite_re.search(d)
    if m:
        type = 'pdfwrite'
        filename = m.group(1) + "." + m.group(2)
        device = m.group(3)
        dpi = int(m.group(4))
        banded = int(m.group(5))
    else:
        m = normal_re.search(d)
        if m:
            type = 'normal  '
            filename = m.group(1)
            device = m.group(2)
            dpi = int(m.group(3))
            banded = int(m.group(4))

    if not type:
        print myself,"WARNING: unknown device",d
        continue

    if banded:
        bandstr = "banded"
    else:
        bandstr = "noband"

    list.append((type, filename, device, dpi, bandstr))

length = len(list)
if length>0:
    print
    print myself,new_name,"new differences from",old_name,"("+str(length)+" differences)"
    list.sort()
    for l in list:
        print "%s %s (%s/%d/%s)" % (l[0], l[1], l[2], l[3], l[4])
else:
    print myself,new_name,"0 differences from",old_name    

list = []
for d in all_diffs:
    type = ''
    filename = ''
    device = ''
    dpi = 0
    banded = 0
    
    m = pdfwrite_re.search(d)
    if m:
        type = 'pdfwrite'
        filename = m.group(1) + "." + m.group(2)
        device = m.group(3)
        dpi = int(m.group(4))
        banded = int(m.group(5))
    else:
        m = normal_re.search(d)
        if m:
            type = 'normal  '
            filename = m.group(1)
            device = m.group(2)
            dpi = int(m.group(3))
            banded = int(m.group(4))

    if not type:
        print myself,"WARNING: unknown device",d
        continue

    if banded:
        bandstr = "banded"
    else:
        bandstr = "noband"

    list.append((type, filename, device, dpi, bandstr))

length = len(list)
if length > 0:
    print
    print myself,new_name,"differences from baseline ("+str(length)+" differences)"
    list.sort()
    for l in list:
        print "%s %s (%s/%d/%s)" % (l[0], l[1], l[2], l[3], l[4])
else:
    print myself,new_name,"0 differences from baseline",baseline_db_path
