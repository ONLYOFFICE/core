#!/usr/bin/python
# -*- Mode: python -*-

# change name of baseline db to baseline db.now
# create new baseline db (anydbm)
# regenerate baseline checksums from checksum raster files
# for all files in raster directory
# calculate checksum
# add filename and checksum to db

import os, time, optparse, myoptparse, gsconf, gssum, anydbm, rasterdb, re

def regen_baseline(rasterdir,workdir,baselinedbname,filelist,options=None):
    if options == None:
        myself="regen_baseline.py"
    else:
        myself=options.myself

    if filelist:               # filelist may be a subset of the files in rasterdir
        print filelist
        f=open(filelist)
        rasterfiles=f.readlines()
        f.close
        print rasterfiles
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
        if options.verbose: print "try",rasterfile_raw
        fullname=rasterdir+rasterfile_raw
        if not os.path.isfile(fullname):
            print myself,"ignoring (not regular file)",fullname
            continue
        if not gzmatch.match(rasterfile_raw):
            print myself,"ignoring (not gz)",rasterfile_raw
            continue

        rasterfile=rasterfile_raw.replace(".gz","")
        rasterfilepath=workdir+rasterfile
        if not gssum.exists(rasterfile,baselinedbname):
            if not os.path.exists(rasterfilepath):
                if options.verbose: print "gz ",rasterfile_raw
                rasterdb.get_file(rasterfile,rasterdir,output=rasterfilepath)
            if options.verbose: print rasterfilepath
            gssum.add_file(rasterfilepath,baselinedbname)
            all+=1
            if options.verbose: print "%100s %i %s %i" % (rasterfile,all,"of",total)
            if options.delete and os.path.exists(rasterfilepath):
                os.unlink(rasterfilepath)


if __name__ == "__main__":

    optionsParser=optparse.OptionParser()
    optionsParser.add_option('--delete','-d',action='store_true',help="delete intermediate raster files")
    (options,arguments)=myoptparse.parseCommandLine(optionsParser,
                                                    revisionSkip=True,
                                                    deviceSkip=True,
                                                    testfileSkip=True,
                                                    )

    if len(arguments) > 0:
        filelist=arguments.pop(0)
    else:
        filelist=None

    now=time.strftime("%Y-%m-%d-%H:%M:%S", time.localtime())
    print options.myself,now

    baselinedb=gsconf.baselinedb
    if not options.nocleanup:
        if os.path.exists(baselinedb):
            baselinedb_backup=baselinedb+"."+now
            print "saving",baselinedb,"to",baselinedb_backup
            os.rename(baselinedb,baselinedb_backup)
    else:
        print options.myself,"no cleanup, use existing baseline database"

    print baselinedb
    db = anydbm.open(baselinedb, 'c')
    regen_baseline(gsconf.rasterdbdir,gsconf.workdir,baselinedb,filelist=filelist,options=options)
