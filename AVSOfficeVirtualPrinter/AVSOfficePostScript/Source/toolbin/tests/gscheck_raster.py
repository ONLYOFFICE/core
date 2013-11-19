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

# $Id: gscheck_raster.py 9206 2008-11-06 23:51:27Z marcos $

#
# gscheck_raster.py
#
# rasterizes input files in several configurations and checks them
# against known baselines
#

myself="gscheck_raster.py"
import sys, os, stat
import string, calendar, time
import gstestutils
import gssum, gsconf, gstestgs, gsparamsets, gsutil
import shutil

import time

class GSCompareTestCase(gstestgs.GhostscriptTestCase):

    def shortDescription(self):
        file = "%s.%s.%d.%d" % (self.file[string.rindex(self.file, '/') + 1:], self.device, self.dpi, self.band)
	rasterfilename = gsconf.rasterdbdir + file + ".gz"

        if self.band:
            banded = "banded"
        else:
            banded = "noband"
        filename_base= os.path.basename(self.file)
        filename_details= "%s (%s/%ddpi/%s)" % (filename_base, self.device, self.dpi,banded)

	if not os.access(rasterfilename, os.F_OK):
            message="ERROR \ncannot find "+rasterfilename+" for "+filename_details
            print myself,message
            self.skip = 1
        else:
            ct = time.localtime(os.stat(rasterfilename)[stat.ST_MTIME])
            baseline_date = "%s %d, %4d %02d:%02d" % ( calendar.month_abbr[ct[1]], ct[2], ct[0], ct[3], ct[4] )
            message="Checking %s against baseline set on %s" % (filename_details,baseline_date)

        return message

    def runTest(self):
        if hasattr(self, "skip") and self.skip == 1:
	    self.assert_(True)
	    return

	outputfile = "%s.%s.%d.%d" % (self.file[string.rindex(self.file, '/') + 1:], self.device, self.dpi, self.band)

	gs = gstestgs.Ghostscript()

	gs.gsroot = self.gsroot
	gs.device = self.device
	gs.dpi = self.dpi
	gs.band = self.band
	gs.infile = self.file
	gs.outfile = outputfile
	if self.log_stdout:
	    gs.log_stdout = self.log_stdout
	if self.log_stderr:
	    gs.log_stderr = self.log_stderr

	if gs.process():
	    sum = gssum.make_sum(outputfile)
        else:
	    sum = ''

        if os.path.exists(outputfile):
            shutil.move(outputfile, gsconf.datadir+"/raster.daily")
#           os.unlink(outputfile)

	if sum and self.track_daily:	                                                  # add test result to daily database
            if gsconf.__dict__.has_key("checksumdb") and gsconf.checksumdb:
                dbname=gsconf.dailydir+gsconf.checksumdb # mhw +".db"
            else:
                dbname=gsconf.get_dailydb_name()
            gssum.add_file(outputfile, dbname=dbname, sum=sum)

	if not sum:
	    message=myself+" output file "+outputfile+" was not created for input file: " + self.file
	    self.fail(message)
        else:
            if gssum.exists(outputfile,gsconf.baselinedb):
                sum_baseline=gssum.get_sum(outputfile,gsconf.baselinedb)
                message=myself+' checksum did not match baseline (' + outputfile + ') for input file: ' + self.file
                self.assertEqual(sum,sum_baseline,message)
            else:
                message = myself+" no baseline checksum (" + outputfile + ") for file: " + self.file
                self.fail(message)

# add compare tests
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
	    for params in gsparamsets.testparamsets:
	        add_compare_test(suite,
                                 gsroot,testfile,
                                 params.device,params.resolution,params.banding,
                                 track,now)

if __name__ == '__main__':
    gstestutils.gsRunTestsMain(addTests)
