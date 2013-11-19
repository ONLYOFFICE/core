#!/usr/bin/env python

#    Copyright (C) 2001-2004 Artifex Software Inc.
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

# $Id: gscheck_raster.py 6300 2005-12-28 19:56:24Z giles $

#
# gscheck_raster.py
#
# rasterizes input files in several configurations and checks them
# against known baselines
#

myself="gscheck_testfiles.py"
import sys, os
import gsconf, gstestutils


def add_compare_test(suite, gsroot, testfile, device, dpi, band, track, now=None):

    logdir=gsconf.logdir
    if now == None:
        now=time.strftime("%Y-%m-%d-%H:%M:%S", time.localtime())
    prefix=logdir+now+"."

    log_stdout=prefix+gsconf.gs_stdout
    log_stderr=prefix+gsconf.gs_stderr

    suite.addTest(GSCompareTestCase(gsroot=gsroot,
                                    file=gsconf.comparefiledir + testfile,
                                    device=device,dpi=dpi,band=band,
                                    log_stdout=log_stdout,
                                    log_stderr=log_stderr,
                                    track_daily=track,now=now)
                  )

def addTests(suite,gsroot,now,options=None, **args):
    if args.has_key('track'):
        track = args['track']
    else:
        track = 0

    # get a list of test files
    comparefiles = os.listdir(gsconf.comparefiledir)
    comparefiles.sort()

    if sys.modules["gsconf"].__dict__.has_key("revision"):
        print myself,gsconf.revision
    
#    for testfile in comparefiles:
#        print myself,testfile

    for testfile in comparefiles:
        if gsutil.check_extension(testfile):
            print testfile

if __name__ == '__main__':
    gstestutils.gsRunTestsMain(addTests)
