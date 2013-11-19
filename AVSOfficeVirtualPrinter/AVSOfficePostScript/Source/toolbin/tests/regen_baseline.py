#!/usr/bin/python
# -*- Mode: python -*-

# change name of baseline db to baseline db.now
# create new baseline db (anydbm)
# regenerate baseline checksums from checksum raster files
# for all files in raster directory
# calculate checksum
# add filename and checksum to db

import os, time, optparse, myoptparse, gsconf, gssum, anydbm, rasterdb, re

def regen_baseline(rasterdir,workdir,baselinedbname,listfile,options=None):
    if options == None:
        myself="regen_baseline.py"
    else:
        myself=options.myself

    if listfile:               # listfile may be a subset of the files in rasterdir
        print listfile
        f=open(listfile)
        rasterfiles=f.readlines()
        f.close
    else:
        rasterfiles = os.listdir(rasterdir)
        rasterfiles.sort()

    if not os.path.exists(workdir):
        os.mkdir(workdir)

    total=len(rasterfiles)
    all=0
    print "%50s %s %i" % (rasterdir,"total files",total)
    gzmatch=re.compile('.*gz$')
    for rasterfile_raw in rasterfiles:
        rasterfile_raw=rasterfile_raw.strip("\n")
        fullname=rasterdir+rasterfile_raw
#        if not os.path.isfile(fullname):
#            print myself,"ignoring (not regular file)",fullname
#            continue
        if not gzmatch.match(rasterfile_raw):
            print myself,"ignoring (not gz)",rasterfile_raw
            continue

        rasterfile=rasterfile_raw.replace(".gz","")
        rasterfilepath=workdir+rasterfile

        if not os.path.exists(rasterfilepath):
            if options.verbose: print "gz ",rasterfile_raw
            rasterdb.get_file(rasterfile,rasterdir,output=rasterfilepath)

        if options.verbose: print rasterfilepath
        sum=gssum.add_file(rasterfilepath,baselinedbname)
        all+=1
        if options.verbose: print "%100s %s %i %s %i" % (rasterfile,sum,all,"of",total)
        if options.delete and os.path.exists(rasterfilepath):
            os.unlink(rasterfilepath)


if __name__ == "__main__":

    optionsParser=optparse.OptionParser()

    optionsParser.add_option('--new',action='store_true',help="start new db file")
    optionsParser.add_option('--delete',action='store_true',help="delete intermediate raster files")
    optionsParser.add_option('--listfile',action='store',help="listfile:\"list\"",default=None)

    (options,arguments)=myoptparse.parseCommandLineBasic(optionsParser)

    listfile=options.listfile

    baselinedb=gsconf.baselinedb
    if options.new:
        now=time.strftime("%Y-%m-%d-%H:%M:%S", time.localtime())
        print options.myself,now
        if os.path.exists(baselinedb):
            baselinedb_backup=baselinedb+"."+now
            print "saving",baselinedb,"to",baselinedb_backup
            os.rename(baselinedb,baselinedb_backup)
        print options.myself,"create new baseline database"
        db = anydbm.open(baselinedb, 'c')
    else:
        print options.myself,"use existing baseline database"
        db = anydbm.open(baselinedb, 'r')

    print baselinedb
    regen_baseline(gsconf.rasterdbdir,gsconf.workdir,baselinedb,listfile=listfile,options=options)
