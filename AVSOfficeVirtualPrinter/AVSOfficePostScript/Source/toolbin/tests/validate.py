#!/usr/bin/env python
# -*- Mode: python -*-

import sys, os, anydbm
import optparse,myoptparse
import gsconf, gsparamsets

def validate(options):
    print "%20s %s" % ( "root",gsconf.root)

    print "%20s %s" % ( "gsroot",gsconf.gsroot)
    print "%20s %s" % ( "installtree",gsconf.installtree)
    print "%20s %s" % ( "comparefiledir",gsconf.comparefiledir)
    print "%20s %s" % ( "comparefileall",gsconf.comparefileall)
    print "%20s %s" % ( "crashfiledir",gsconf.crashfiledir)
    print "%20s %s" % ( "codedir",gsconf.codedir)
    print "%20s %s" % ( "datadir",gsconf.datadir)
    print "%20s %s" % ( "logdir",gsconf.logdir)
    print "%20s %s" % ( "testdir",gsconf.testdir)
    print "%20s %s" % ( "dailydir",gsconf.dailydir)
    print "%20s %s" % ( "rasterdbdir",gsconf.rasterdbdir)
    print "%20s %s" % ( "rasterdir",gsconf.rasterdir)
    print "%20s %s" % ( "fontdir",gsconf.fontdir)
    print "%20s %s" % ( "workdir",gsconf.workdir)


    print
    print "email"
    print "%20s %s" % ( "report_to",gsconf.report_to)
    print "%20s %s" % ( "report_from",gsconf.report_from)

    print
    print "comparefiles"
    comparefiles = os.listdir(gsconf.comparefiledir)
    comparefiles.sort()
    count=len(comparefiles)
    print "comparefiles: file count",count
    if options.verbose:
        for comparefile in comparefiles:
            print comparefile

    print
    print "rasterfiles"
    rasterfiles = os.listdir(gsconf.rasterdir)
    rasterfiles.sort()
    count=len(rasterfiles)
    print "rasterfiles:   file  count",count

    print
    print "rasterfile checksum db"
    db = anydbm.open(gsconf.baselinedb, 'r')
    keys=db.keys()
    keys.sort()
    db.close()

    count=len(keys)
    
    print "rasterfile db: entry count",count

    print
    print "gsparamsets name",gsparamsets.testparamsets_name
    if options.verbose:
        for params in gsparamsets.testparamsets:
            print params.device, params.resolution, params.banding

        for params in gsparamsets.pdftestparamsets:
            print params.device, params.resolution, params.banding,"pdfwrite"

    print


if __name__ == "__main__":
    optionsParser=optparse.OptionParser()
    (options,arguments)=myoptparse.parseCommandLine(optionsParser)

    validate(options)
