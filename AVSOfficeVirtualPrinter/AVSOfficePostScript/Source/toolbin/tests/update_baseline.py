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
# update_baseline <file>
#
# this script updates the testdata database with a new baseline sum
# for all versions of file <file>.  use this when the comparing gs is
# producing better output than the normal baseline (ie, when a regression
# is really a progression)

import gstestgs
import gsconf
import gssum
import gsparamsets
import rasterdb
import anydbm
import time
import os, sys, copy
import string
import gsutil
import optparse, myoptparse

def update_databases(outputfile,device,dpi,band,revision,options):

    log = open(gsconf.baseline_log, "a")

    outputdevice=" ".join((device,str(dpi),str(band)))
    message=time.ctime() + " " + testfile +" "+outputdevice+ " updated "+revision

    if options and options.dryrun:
        print "dryrun",message
    else:
        log.write(message+"\n")
        log.close()

    baselinedb=gsconf.baselinedb
    if options and options.dryrun:
        pass
    else:
        gssum.add_file(outputfile,baselinedb)

    if options and options.dryrun:
        pass
    else:
        rasterdb.put_file(outputfile)

    if not options.nocleanup:
        os.unlink(outputfile)

def make_entry(testfile, device, dpi, band,revision,options):
    testfilepath=gsconf.comparefileall + testfile
    if not os.path.exists(testfilepath):
        print options.myself,"cannot find","\""+testfilepath+"\""


    outputfile = "%s.%s.%d.%d" % (testfile, device, dpi, band)
    print "update_baseline.py: " + outputfile
    sys.stdout.flush()

    gs = gstestgs.Ghostscript()

    gs.gsroot = gsconf.installtree
    gs.command = gsconf.installtree+"bin/gs"
    if not os.path.exists(gs.command):
        print options.myself,"gs executable does not exist",gs.command
        return

    if gsconf.gs_stdout and gsconf.gs_stderr:
        gs.log_stdout = gsconf.logdir+gsconf.gs_stdout
        gs.log_stderr = gsconf.logdir+gsconf.gs_stderr
    gs.infile = testfilepath
    gs.outfile = outputfile
    gs.device = device
    gs.dpi = dpi
    gs.band = band

    if gs.process():
        update_databases(outputfile,device,dpi,band,revision=revision,options=options)
    else:
	print options.myself,"error from gs.process",testfilepath,device,dpi,band

def make_pdf_entry(testfile, device, dpi, band,revision,options):
    testfilepath=gsconf.comparefileall + testfile
    if not os.path.exists(testfilepath):
        print options.myself,"cannot find","\""+testfilepath+"\""

    outputfile = "%s.pdf.%s.%d.%d" % (testfile, device, dpi, band)
    print "update_baseline.py (pdf): " + outputfile
    sys.stdout.flush()

    gs = gstestgs.Ghostscript()

    gs.gsroot = gsconf.installtree
    gs.command = gsconf.installtree+"bin/gs"
    if not os.path.exists(gs.command):
        print options.myself,"gs executable does not exist",gs.command
        return

    gs.log_stdout = gsconf.logdir+gsconf.gs_stdout
    gs.log_stderr = gsconf.logdir+gsconf.gs_stderr

    gs.infile = gsconf.comparefileall + testfile
    gs.dpi = dpi
    gs.band = band

    intermediate_pdf_file = outputfile + ".pdf"
    gs.outfile = intermediate_pdf_file
    gs.device = 'pdfwrite'
    gs.dpi = None

    if not gs.process():
	print options.myself,"error (->pdf)",testfilepath,device,dpi,band
        return

    gs.infile = intermediate_pdf_file
    gs.outfile = outputfile
    gs.device = device
    gs.dpi = dpi

    if gs.process():
        update_databases(outputfile,device,dpi,band,revision=revision,options=options)
    else:
	print options.myself,"error.",testfilepath,device,dpi,band

def make_entries_paramsets(testfile,options):
    buildroot=gsconf.gsroot
    revision_filename=buildroot+"revision"
    if os.path.exists(revision_filename):
        revision_file=open(revision_filename)
        revision = revision_file.readline()
        revision=revision.strip("\n")
        revision_file.close()
    else:
        revision="unknown"

    if gsutil.check_extension(testfile):
        for params in gsparamsets.testparamsets_maximum:
            make_entry(testfile, params.device, params.resolution, params.banding,revision=revision,options=options)

def make_pdf_entries_paramsets(testfile,options):
    buildroot=gsconf.gsroot
    revision_filename=buildroot+"revision"
    if os.path.exists(revision_filename):
        revision_file=open(revision_filename)
        revision = revision_file.readline()
        revision=revision.strip("\n")
        revision_file.close()
    else:
        revision="unknown"

    if gsutil.check_extension(testfile):
        for params in gsparamsets.pdftestparamsets_maximum:
            make_pdf_entry(testfile, params.device, params.resolution, params.banding,revision=revision,options=options)

if __name__ == '__main__':

    optionsParser=optparse.OptionParser()
    optionsParser.add_option('--dryrun',action='store_true',help="simple date format")
    optionsParser.add_option('--nocleanup',action='store_true',help="do not delete output files")
    optionsParser.add_option('--pdf',action='store_true',help="update pdfwrite raster")
    optionsParser.add_option('--both',action='store_true',help="update normal and pdfwrite raster")
    (options,arguments)=myoptparse.parseCommandLineBasic(optionsParser)

    myself=options.myself

    if options.pdf:
        options.normal=False
    else:
        options.normal=True

    if options.both:
        options.pdf=True
        options.normal=True

    if options.normal:
        files=copy.copy(arguments)
        while len(files) > 0:
            testfile = os.path.basename(files.pop(0))
            testfilepath=gsconf.comparefileall + testfile
            if not os.path.exists(testfilepath):
                print myself,"cannot find","\""+testfilepath+"\""
                continue
            make_entries_paramsets(testfile,options)

    if options.pdf:
        files=copy.copy(arguments)
        while len(files) > 0:
            testfile = os.path.basename(files.pop(0))
            testfilepath=gsconf.comparefileall + testfile
            if not os.path.exists(testfilepath):
                print myself,"cannot find","\""+testfilepath+"\""
                continue
            make_pdf_entries_paramsets(testfile,options)
    
