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
# compare_checksumdb.py firstname secondname
#
# this script provides the difference between two sets of checksums
# the files must be anydbm files, ususally with the testfile name as the key

version="1.0"

import sys, os
import optparse, myoptparse
import anydbm

if __name__ == "__main__":
    
    optionsParser=optparse.OptionParser()
    optionsParser.add_option('--option',action='store_true',help="sample additional option")
    optionsParser.add_option('--nosvn',action='store_true',help="no not update from svn")
    optionsParser.add_option('--nomake',action='store_true',help="no not make")

    (options,arguments)=myoptparse.parseCommandLine(optionsParser)
    print options.revision

    old_dbname=arguments.pop(0)
    new_dbname=arguments.pop(0)

    print myself,old_dbname,new_dbname

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

    if not old_db:
        print myself,"empty checksum database",old_dbname
    if not new_db:
        print myself,"empty checksum database",new_dbname


    if not new_db or not old_db:

    normal_re = re.compile("^(.*?)\.(p[bgpk]mraw)\.(\d+)\.(\d+)$")
    pdfwrite_re = re.compile("^(.*?)\.(ps|pdf)\.pdf\.(p[bgpk]mraw)\.(\d+)\.(\d+)$")

    diffs = []

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
    print myself,new_name,"new differences from",old_name,"(",str(length)," differences)"
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
