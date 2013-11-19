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

# $Id: make_testdb 6300 2005-12-28 19:56:24Z giles $

#
# make_testdb <dir>
#
# this script creates a gstestdb database and intializes it
# with the md5sums of the test data files run through the
# executable last built in HEAD/ and installed in HEAD gshead/

import gstestgs
import gsconf
import gssum
import gsparamsets
import rasterdb
import anydbm
import os, sys
import string
import gsutil

def make_compare_entry(dbname,ifile, device, dpi, band):

    ofile = "%s.%s.%d.%d" % (ifile, device, dpi, band)
    print "creating entry: " + ofile + "...",
    sys.stdout.flush()
    
    gs = gstestgs.Ghostscript()

    gs.gsroot = gsconf.gsroot
    gs.log_stdout = gsconf.gs_stdout
    gs.log_stderr = gsconf.gs_stderr
    gs.command = gsconf.headinstallpath

    gs.infile = gsconf.comparefiledir + ifile
    gs.outfile = ofile
    gs.device = device
    gs.dpi = dpi
    gs.band = band

    if not gssum.exists(ofile,dbname):
        if gs.process():
	    try:
       	        gssum.add_file(ofile,dbname)
                rasterdb.put_file(ofile)
    	        os.unlink(ofile)
    	        print "done."
            except OSError:
                print "no output produced."
        else:
	    print "error."
    else:
	print "exists."

def make_pdfcompare_entry(dbname,ifile, device, dpi, band):
    ofile = "%s.pdf.%s.%d.%d" % (ifile, device, dpi, band)
    print "creating entry: " + ofile + "...",
    sys.stdout.flush()

    if gssum.exists(ofile,dbname):
        print "exists."
        return
    
    gs = gstestgs.Ghostscript()

    gs.gsroot = gsconf.gsroot
    gs.log_stdout = gsconf.gs_stdout
    gs.log_stderr = gsconf.gs_stderr
    gs.command = gsconf.headinstallpath

    gs.infile = gsconf.comparefiledir + ifile
    gs.dpi = dpi
    gs.band = band

    # make file->PDF

    tfile = ofile + ".pdf"
    gs.outfile = tfile
    gs.device = 'pdfwrite'
    gs.dpi = None

    if not gs.process():
        print "error."
        return

    gs.infile = tfile
    gs.outfile = ofile
    gs.device = device
    gs.dpi = dpi

    if gs.process():
        try:
            gssum.add_file(ofile,dbname)
            rasterdb.put_file(ofile)
            os.unlink(tfile)
            os.unlink(ofile)
            print "done."
        except OSError:
            print "no output produced."
    else:
        print "error."


if __name__ == "__main__":
    # create the testdatadb
    dbname = gsconf.testdatadb
    db = anydbm.open(dbname, 'c')
    db.close()

    files = os.listdir(gsconf.comparefiledir)
    files.sort()
    for f in files:
        if gsutil.check_extension(f):
            for params in gsparamsets.testparamsets:
                make_compare_entry(dbname,f, params.device, params.resolution, params.banding)
            for params in gsparamsets.pdftestparamsets:
                make_pdfcompare_entry(dbname,f, params.device, params.resolution, params.banding)
