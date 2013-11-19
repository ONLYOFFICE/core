#!/usr/bin/python
# -*- Mode: python -*-

import os, sys
import optparse, myoptparse

if __name__ == "__main__":

    os.umask(0002)

    optionsParser=optparse.OptionParser()

    optionsParser.add_option('--version',action='store_true',help="get my version")

    optionsParser.add_option('--low',action='store',help="low version of range to run")
    optionsParser.add_option('--high',action='store',help="high version of range to run")

    optionsParser.add_option('--removeonly',action='store_true',help="remove the build directory - do nothing else")

    optionsParser.add_option('--noupdate',action='store_true',help="do not update the gs executable")
    # "update" includes svn, configure, makeclean, clean, install

    optionsParser.add_option('--nosvn',action='store_true',help="do not update the source from the svn repository")
    optionsParser.add_option('--noconfigure',action='store_true',help="do not run auto configure")
    optionsParser.add_option('--nomakeclean',action='store_true',help="do not make clean before make")
    optionsParser.add_option('--nomake',action='store_true',help="do not make")

    optionsParser.add_option('--nocapture',action='store_true',help="do not capture stdout and stderr from commands")

    optionsParser.add_option('--noregression',action='store_true',help="do not run regression")
    optionsParser.add_option('--notestregression',action='store_true',help="do not test regression")
    optionsParser.add_option('--printregression',action='store_true',help="print regression output")
    optionsParser.add_option('--printmailmessage',action='store_true',help="print mail message")

    optionsParser.add_option('--fullset',action='store_true',help="full gsparamsets",default="True")
    optionsParser.add_option('--minset',action='store_true',help="minimum gsparamsets")
    optionsParser.add_option('--maxset',action='store_true',help="maximum gsparamsets")

    optionsParser.add_option('--release',action='store_true',help="under contruction")
    optionsParser.add_option('--oldrevision',action='store',help="old revision for checksum comparison",default=None)

    (options,arguments)=myoptparse.parseCommandLine(optionsParser,deviceSkip=True,testfileSkip=True,listfileSkip=True,revisionSkip=True)

    if options.version:
        print options.myself,"version",myversion
        sys.exit(1)

    low=int(options.low)
    high=int(options.high)

    overall_result=0
    for revision in range(low,high+1):
        options.revision=revision
        result = run_nightly(options,arguments)
        if result > 0:
            overall_result=1
        print options.myself,revision,"result",result

    sys.exit(overall_result)
