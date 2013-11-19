#!/usr/bin/env python

#    Copyright (C) 2001 Artifex Software, Inc.
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

# $Id: gscheck_fuzzypdf.py 8409 2007-11-27 20:43:09Z giles $

#
# gscheck_fuzzypdf.py
#
# does a fuzzy compare against the output of ps->raster and ps->pdf->raster
# to make sure that the pdfwrite device is functioning as expected

import os
import string
import gstestutils
import gsconf, gstestgs, gsparamsets


def fuzzy_compare(file1, file2, tolerance=2, windowsize=5):
    cmd = gsconf.fuzzy + ' -w%d -t%d %s %s > /dev/null 2> /dev/null' % (windowsize, tolerance, file1, file2)

    ret = os.system(cmd)
    if ret == 0:
        return 1
    else:
        return 0
		
class GSFuzzyCompareTestCase(gstestgs.GhostscriptTestCase):
    def shortDescription(self):
        return "Checking pdfwrite fuzzy test of %s (%s/%d/%d)" % (self.file[string.rindex(self.file, '/') + 1:], self.device, self.dpi, self.band)
	
    def runTest(self):
        file1 = '%s.%s.%d.%d' % (self.file, self.device, self.dpi, self.band)
	file2 = '%s.%s.%d.%d.pdf' % (self.file, 'pdfwrite', self.dpi, self.band)
	file3 = '%s.pdfwrite.%s.%d.%d' % (self.file, self.device, self.dpi, self.band)

	gs = gstestgs.Ghostscript()
	gs.command = self.gs
	gs.dpi = self.dpi
	gs.band = self.band
	gs.infile = self.file
	gs.device = self.device
	if self.log_stdout:
	    gs.log_stdout = self.log_stdout
	if self.log_stderr:
	    gs.log_stderr = self.log_stderr

	# do PostScript->device (pbmraw, pgmraw, ppmraw, pkmraw)

	gs.outfile = file1
	if not gs.process():
	    self.fail("non-zero exit code trying to rasterize " + self.file)

	# do PostScript->pdfwrite
		
	gs.device = 'pdfwrite'
        gs.dpi = None
	gs.outfile = file2
	if not gs.process():
	    self.fail("non-zero exit code trying to distill " + self.file)

	# do PDF->device (pbmraw, pgmraw, ppmraw, pkmraw)
		
	gs.device = self.device
        gs.dpi = self.dpi
	gs.infile = file2
	gs.outfile = file3
        if not gs.process():
            self.fail("non-zero exit code trying to rasterize " + file2)

	# fuzzy compare PostScript->device with PostScript->PDF->device
		
	ret = fuzzy_compare(file1, file3)
	os.unlink(file1)
	os.unlink(file2)
	os.unlink(file3)
	self.assert_(ret, "fuzzy match failed")

# Add the tests defined in this file to a suite

def add_compare_test(suite, f, device, dpi, band):
    suite.addTest(GSFuzzyCompareTestCase(gsroot, file=gsconf.comparefiledir + f, device=device, dpi=dpi, band=band))

def addTests(suite, gsroot, **args):
    # get a list of test files
    comparefiles = os.listdir(gsconf.comparefiledir)


    for testfile in comparefiles:

# this comparison is not good...
        if testfile[-3:] == '.ps':
	    for params in gsparamsets.testparamsets:
	        add_compare_test(suite, gsroot,testfile, params.device, params.resolution, params.banding)

if __name__ == "__main__":
    gstestutils.gsRunTestsMain(addTests)
