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
# run_regression.py
#
# runs ghostscript regression tests

import sys, os, time, myoptparse, optparse, shutil
import anydbm
import gstestutils, gsconf, gsparamsets
import check_all
import gscheck_all

def get_revision_of_HEAD(dir=None):
    if dir:
        cwd=os.getcwd()
        os.chdir(dir)
    p = os.popen("svn info")
    for line in p:
        if "Revision:" in line:
            revision=line.strip('Revision: ')
            revision=revision.strip('\n')
	    break
    else:
        revision = None
    if dir:
        os.chdir(cwd)
    return revision

def testAll(track,gsroot,now,options):
    suite = gstestutils.GSTestSuite()

    print options.myself,gsconf.comparefiledir
    print options.myself,"test parameter set: "+gsparamsets.testparamsets_name

    if not os.path.exists(gsroot):
        print options.myself,"FATAL: gsroot directory does not exist",gsroot        
        sys.exit(1)

    gsexecutable=gsroot+"bin/gs"
    if not os.path.exists(gsexecutable):
        print options.myself,"FATAL: gsexecutable does not exist",gsexecutable        
        sys.exit(1)

    revision=get_revision_of_HEAD(dir=gsroot)
    print options.myself,"from tree:",gsroot,"revision is:",revision
    
    # Add tests based on running Ghostscript.
    gscheck_all.addTests(suite, gsroot=gsroot, track=track, now=now, options=options)

    # Add tests not based on actually running Ghostscript.
    check_all.addTests(suite, gsroot=gsroot)

    # run all the tests
    runner = gstestutils.GSTestRunner(verbosity=2)

    cwd=os.getcwd()

    start_time=time.strftime("%Y-%m-%d-%H:%M:%S", time.localtime())
    print options.myself,start_time,"executing in",gsroot
    os.chdir(gsroot)

    result = runner.run(suite)

    end_time=time.strftime("%Y-%m-%d-%H:%M:%S", time.localtime())
    print options.myself,end_time,"complete",cwd
    os.chdir(cwd)

if __name__ == "__main__":

    optionsParser=optparse.OptionParser()
    optionsParser.add_option('--track',action='store_true',help="track sums in baseline db")
    optionsParser.add_option('--time',action='store',help="provide start time",default=None)
    optionsParser.add_option('--revision_value',action='store',help="revision - always an svn revision number",default=None)

    (options,arguments)=myoptparse.parseCommandLine(optionsParser,testfileSkip=True,listfileSkip=True,deviceSkip=True)

    now=options.time
    revision=options.revision
    revision_value=options.revision_value

    if revision != "HEAD":
        gsconf.checksumdb = gsconf.dailydir+revision # mhw +".db"

    if revision != "HEAD":
        gsroot=gsconf.root+"gs."+revision+"/"
    else:
        gsroot=gsconf.gsroot

    if options.track:
        if gsconf.__dict__.has_key("checksumdb") and gsconf.checksumdb:
            dbname=gsconf.checksumdb
        else:
            dbname=gsconf.get_dailydb_name()

        # create (truncate) the database for the daily checksums
        db = anydbm.open(dbname, "n")
        db.close()
        print options.myself,"daily database for checksums",dbname
    
    testAll(options.track,gsroot,now,options)

    if options.track:
        dbname = dbname + ".db" # mhw
        if not os.path.exists(dbname):
            print options.myself,"ERROR","the checksum database does not exist",dbname

        # copy from the db by date to the db by revision
        if revision == "HEAD":
            dbname_by_revision_value = gsconf.dailydir+revision_value+".db"

        print options.myself,"copy from",dbname,"to",dbname_by_revision_value
        shutil.copy(dbname,dbname_by_revision_value)
