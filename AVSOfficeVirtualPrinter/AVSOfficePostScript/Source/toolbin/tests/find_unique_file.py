#!/usr/bin/env python
# -*- Mode: python -*-
import sys, os, re
import anydbm
import gsconf
import myoptparse, optparse

def find_unique(filename):
    file=open(filename)
    lines=file.readlines()
    file.close()

    no_checksum=re.compile(".*no baseline checksum")

    testfiles={}
    comparefiledir=gsconf.comparefiledir
    discard_leader=re.compile(".*"+comparefiledir)
    for line in lines:
        line=line.strip('\n')
        if discard_leader.match(line):
            if no_checksum.match(line):
                testfile = discard_leader.sub("",line)
                value=""
                testfiles[testfile]=value

    testfilelist=testfiles.keys()
    testfilelist.sort()
    for testfile in testfilelist:
        print testfile

if __name__ == "__main__":

    optionsParser=optparse.OptionParser()
    optionsParser.add_option('--file','-f',action='store_true',help="file to be parsed - find unique testfiles")
    (options,arguments)=myoptparse.parseCommandLine(optionsParser,revisionSkip=True,testfileSkip=True,listfileSkip=True,deviceSkip=True)

    args=sys.argv[:]
    options.myself=args.pop(0)
    while len(arguments)> 0:
           filename = arguments.pop(0)
           if os.path.exists(filename):
               find_unique(filename)
           else:
               print options.myself,"cannot open",filename

    sys.exit(0)
