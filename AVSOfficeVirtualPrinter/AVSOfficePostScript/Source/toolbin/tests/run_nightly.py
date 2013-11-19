#!/usr/bin/python
# -*- Mode: python -*-

#    Copyright (C) 2001-2007 Artifex Software Inc.
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

# overview
# 	update revision (default is HEAD)
# 	run all testfiles with all parameter sets using the gs executable - calculate each checksum
# 	compare checksums with 1) previous results 2) baseline checksums
# 	report baseline changes
# 	email results
# 
# temporary output:
#	the output images from gs execution are not saved
#
# permanent output:
# 	checksum database for the day (an "anydbm" database [testfile-device,checksum]
# 
# input:
# 	testing.cfg - configuration many significant variables (in /home/regression/regression, or .)
# 	command line - control details of execution
# 	files in compare file directory (ususally /home/regression/comparefiles)
# 	The following are "anydbm" databases as described above
#	 	database of checksums of the raster files in /home/regression/raster
#	 	databases of past nightly runs in /home/regression/regression/daily (only the most recent is used)
# 	baseline.log - log of updates to baseline raster files (in /home/regression/regression/baseline) [text file]
# 	
# Note that the raster files themselves are not used in this process.
# 
# Hierarchy of python scripts
# 	run_nightly.py
#		gsconf.py - read testing.cfg and create an object defining the environment for all other scripts
# 		run_regression.py - execute gs, calculate checksum, insert into database
#			gs_checkall.py
#				gs_checkraster.py
#				gs_checkpdfwrite.py
#					gsparamset.py - define the "parameter set" - the set of output devices to be tested for each file
#					gstestgs.py, gstestutil.py - run gs (used the built-in Python unit test harness)
#					gssum.py - checksum calculation and database addition
#			
# 		testdiff.py - compare three checksum databases
# 		get_baseline_log.py - extract recent changes to the baseline.log 
#
#
#
#



import os
import sys, shutil
import re
import time
import optparse, myoptparse
import string
import build_revision
import gsconf, gsparamsets
import anydbm
from popen2 import Popen4

# configuration variables

myself="unknown"
myversion="2.01"

def logMessage(message,file,revision,printMessage=True):
    message_time=time.strftime("%Y-%m-%d-%H:%M:%S", time.localtime())
    message=myself+" "+revision+" "+message_time+" "+message
    if printMessage:
        print message
    message+="\n"
    if file:
        file.write(message)
        file.flush()

def die(msg):
    sendmail(gsconf.report_from, gsconf.report_to, "error running regression", msg)
    sys.exit(0)

def read_all_lines(f):
    file = open(f, 'r')
    lines = file.readlines()
    file.close()
    return string.join(lines, '')

def get_count_in_db(dbname):
    try:
        db = anydbm.open(dbname, 'r')
        if not db:
            count=0
        else:
            count=len(db)
    except:
        count=0
    print dbname,count

def get_revision(dir=None):
    if dir:
        cwd=os.getcwd()
        os.chdir(dir)

    command="svn update --non-recursive"
    if os.system(command) != 0:
        message = "run_nightly.py:get_revision.py: cannot svn update"
        logMessage(message,None,"unknown")
        return "unknown"

    command="svn info"
    p = os.popen(command)
    for line in p:
        if "Revision:" in line:
            revision=line.strip('Revision: ')
            revision=revision.strip('\n')
	    break
    else:
        revision = None
    if dir:
        os.chdir(cwd)
    return revision

def change_gsproduct(file):
    tmpfile = "%s.tmp" % (file,)

    startre = re.compile("^#ifndef\ GS_PRODUCT$")
    changere = re.compile("^.*?\"[A-Za-z -]+\".*?$")
    endre = re.compile("^$")

    old = open(file, "r")
    new = open(tmpfile, "w")

    state = 0
    for line in old.readlines():
        print "checking:", line
        if state == 0:
            m = startre.search(line)
            if m:
                state = 1
                
            new.write(line)
        elif state == 1:
            m = changere.search(line)
            if m:
                state = 2
                new.write("\t\"GPL Ghostscript\"\n")
		print "rewriting '%s' to GPL Ghostscript" % line.strip()
            else:
                new.write(line)
        elif state == 2:
            m = endre.search(line)
            if m:
                state = 0

            new.write(line)

    old.close()
    new.close()

    os.unlink(file)
    os.rename(tmpfile, file)

def sendmail(frm, to, subject, text):
    import smtplib

    keyword = gsconf.mailkeyword
    if keyword:
        msg = 'From: %s\r\nTo: %s\r\nSubject: %s\r\nX-Fnord: %s\r\n\r\n%s' % (frm, to, subject, keyword, text)
    else:
        msg = 'From: %s\r\nTo: %s\r\nSubject: %s\r\n\r\n%s' % (frm, to, subject, text)

#   server = smtplib.SMTP(gsconf.mail_server)
#   server.sendmail(frm, to, msg)
#   server.quit()
    SENDMAIL = "/usr/sbin/sendmail" # sendmail location
    p = os.popen("%s -t" % SENDMAIL, "w")
    p.write(msg)
    p.close()

def run_nightly(options,arguments):
    global myself

    myself=options.myself
    # what do we work on?
    revision      = options.revision
    release       = options.release
    
    # what do we build
    update           = not options.noupdate
    
    options.svn       = not options.nosvn
    options.configure = not options.noconfigure
    options.make      = not options.nomake
    options.makeclean = not options.nomakeclean
    
    options.remove   = False  # never remove the gs executable, we need it for regression testing
    
    # what do we run?
    runRegression    = not options.noregression
    testRegression   = not options.notestregression
    baselineChanges  = not options.nobaselinechanges
    printRegression  = options.printregression
    
    # where do stdout and stderr go?
    options.capture  = not options.nocapture
    
    p = os.popen('hostname -s')
    if p:
        hostname = string.strip(p.readline())
        p.close()
    else:
        hostname = 'unknown host'
        
    p = os.popen('uname -m -s')
    if p:
        hostuname=string.strip(p.readline())
        p.close()
    else:
        hostuname=""

    hostname=hostname + " (" + hostuname + ')'

    if release:
        print "NOT SUPPORTED: need to build a release",release
        sys.exit(1)

    if not revision or revision == "HEAD":
        gsroot=gsconf.gsroot
        revision_value=get_revision(gsroot)
        gsinstall=gsconf.installtree
        gspath=gsconf.installtree+"bin/gs"
        revision_full="HEAD:"+revision_value
    else:
        gsroot=gsconf.root+"gs."+revision+"/"
        gsconf.checksumdb = revision
        revision_value=revision
        gsrevision_root=gsconf.root+"gs."+revision+"/"
        gspath=gsrevision_root+"bin/gs"
        gsinstall=None
        revision_full=revision_value

    logdir=gsconf.logdir
    if not os.path.exists(logdir):
        os.mkdir(logdir)

    now=time.strftime("%Y-%m-%d-%H:%M:%S", time.localtime())
    mail_message = now+" "+revision_value+"\n"

    prefix=logdir+revision_value+"."

    update_stdout=prefix+gsconf.update_stdout
    update_stderr=prefix+gsconf.update_stderr
    make_stdout=prefix+gsconf.make_stdout
    make_stderr=prefix+gsconf.make_stderr
    config_stdout=prefix+gsconf.config_stdout
    config_stderr=prefix+gsconf.config_stderr
    install_stdout=prefix+gsconf.install_stdout
    install_stderr=prefix+gsconf.install_stderr
    
    cumulative_name=prefix+gsconf.cumulative
    
    history_name=logdir+gsconf.history        # permanent file, accumulates history across days
    
    if not os.path.exists(history_name):
        messageHistory="creating history file "+history_name
    else:
        messageHistory="opening history file "+history_name
    history_file = open(history_name, "w")    

    cumulative_file = open(cumulative_name, "w")
    message="cumulative"
    logMessage(message,cumulative_file,revision_full,printMessage=False)
    logMessage(messageHistory,cumulative_file,revision_full,printMessage=False)

    message="running on " + hostname +" my version "+myversion
    logMessage(message,cumulative_file,revision_full)
    logMessage(message,history_file,revision_full,printMessage=False)
    if options.version:
        sys.exit(1)

    if options.removeonly:
        path=gsroot
        message="removing "+path
        logMessage(message,cumulative_file,revision_full)
        shutil.rmtree(path,ignore_errors=True)
        if gsinstall:
            path=gsinstall
            message="removing "+path
            logMessage(message,cumulative_file,revision_full)
            shutil.rmtree(path,ignore_errors=True)
        sys.exit(1)

    mail_message+=message+"\n"

    if not os.path.exists(gsconf.comparefiledir + "/.svn") or \
	    os.system("svn update " + gsconf.comparefiledir) != 0:
	message = "run_nightly.py: cannot svn update " + gsconf.comparefiledir
	logMessage(message,None,"n/a")

    if not os.path.exists(gsconf.comparefiledir + "/../tests/eps" + "/.svn") or \
	    os.system("svn update " + gsconf.comparefiledir + "/../tests/eps") != 0:
	message = "run_nightly.py: cannot svn update " + gsconf.comparefiledir + "/../tests/eps"
	logMessage(message,None,"n/a")
    if os.path.exists(gsconf.comparefiledir + "/../tests/eps"):
        os.system("ln -f -s " + gsconf.comparefiledir + "/../tests/eps/* " + gsconf.comparefiledir)
    if not os.path.exists(gsconf.comparefiledir + "/../tests/ps" + "/.svn") or \
	    os.system("svn update " + gsconf.comparefiledir + "/../tests/ps") != 0:
	message = "run_nightly.py: cannot svn update " + gsconf.comparefiledir + "/../tests/ps"
	logMessage(message,None,"n/a")
    if os.path.exists(gsconf.comparefiledir + "/../tests/ps"):
        os.system("ln -f -s " + gsconf.comparefiledir + "/../tests/ps/* " + gsconf.comparefiledir)
    if not os.path.exists(gsconf.comparefiledir + "/../tests/pdf" + "/.svn") or \
	    os.system("svn update " + gsconf.comparefiledir + "/../tests/pdf") != 0:
	message = "run_nightly.py: cannot svn update " + gsconf.comparefiledir + "/../tests/pdf"
	logMessage(message,None,"n/a")
    if os.path.exists(gsconf.comparefiledir + "/../tests/pdf"):
        os.system("ln -f -s " + gsconf.comparefiledir + "/../tests/pdf/* " + gsconf.comparefiledir)
    comparefiles = os.listdir(gsconf.comparefiledir)
    count=len(comparefiles)

    message="comparefiles from "+gsconf.comparefiledir+" number of files "+str(count)
    logMessage(message,cumulative_file,revision_full)
    logMessage(message,history_file,revision_full,printMessage=False)
    mail_message+=message+"\n"

    # use the smallest that is requested
    if options.oneset:    
        gsparamsets.testparamsets = gsparamsets.testparamsets_one
        gsparamsets.pdftestparamsets = gsparamsets.pdftestparamsets_one
        gsparamsets.testparamsets_name = "one"
    elif options.minset:
        gsparamsets.testparamsets = gsparamsets.testparamsets_minimum
        gsparamsets.pdftestparamsets = gsparamsets.pdftestparamsets_minimum
        gsparamsets.testparamsets_name = "minimum"
    elif options.maxset:    
        gsparamsets.testparamsets = gsparamsets.testparamsets_maximum
        gsparamsets.pdftestparamsets = gsparamsets.pdftestparamsets_full
        gsparamsets.testparamsets_name = "maximum"
    elif options.fullset:    
        gsparamsets.testparamsets = gsparamsets.testparamsets_full
        gsparamsets.pdftestparamsets = gsparamsets.pdftestparamsets_full
        gsparamsets.testparamsets_name = "full"
    else:
        pass # use default in gsparamsets

    gsparamsets.testparamsets = gsparamsets.testparamsets_one
    gsparamsets.pdftestparamsets = gsparamsets.pdftestparamsets_full
    gsparamsets.testparamsets_name = "one+pdf"


    message="test parameter set name: "+gsparamsets.testparamsets_name
    logMessage(message,cumulative_file,revision_full)
    logMessage(message,history_file,revision_full,printMessage=False)
    mail_message+=message+"\n"

    message="logging to "+prefix+"*"
    logMessage(message,cumulative_file,revision_full)
    mail_message+=message+"\n"

    message="sending mail to "+gsconf.report_to
    logMessage(message,cumulative_file,revision_full)
    logMessage(message,history_file,revision_full,printMessage=False)

    if update:
        message="starting update_ghostscript"
        logMessage(message,cumulative_file,revision_full)
    
        (err,message) = build_revision.update_ghostscript(revision,release,
                                                          gsroot,
                                                          options,
                                                          update_stdout,update_stderr,
                                                          config_stdout,config_stderr,
                                                          make_stdout,make_stderr,
                                                          install_stdout,install_stderr,
                                                          cumulative_file
                                                          )
        if err != 0:
            die(message)
    else:
        message="skipping update of ghostscript tree and executable"
        logMessage(message,cumulative_file,revision_full)

    message = "running makemissing"
    logMessage(message,cumulative_file,revision_full)
    os.system("./makemissing 2>logs/makemissing.stderr >logs/makemissing.stdout")


    prefix=logdir+revision_value+"."

    baselines_name=prefix+gsconf.baselines
    differences_name=prefix+gsconf.differences
    regression_name=prefix+gsconf.regression
    email_name=prefix+gsconf.email

    if not os.path.exists(gspath):
        message=myself+" FATAL "+"the gs executable does not exist "+gspath
        logMessage(message,cumulative_file,revision_full)
        die(message)

    regression_file = open(regression_name, "w")
    message="regression"
    logMessage(message,regression_file,revision_full,printMessage=False)

    message="revision of executable "+revision_full
    logMessage(message,regression_file,revision_full,printMessage=False)
    logMessage(message,history_file,revision_full,printMessage=False)
    mail_message+=message+"\n"

    revision_name=prefix+revision
    revision_file = open(revision_name, "w")
    revision_file.close()

    # create string "regression_results" for regression results
    regression_results = '\n'
    if runRegression:
        print

        message="starting regression testing on "+revision
        logMessage(message,regression_file,revision_full,printMessage=False)
        logMessage(message,cumulative_file,revision_full)

        timearg=" --time="+"\""+now+"\""
        trackarg=" --track"

        # revision can be HEAD
        if revision != "HEAD":
            revisionarg=" --revision="+revision
        else: revisionarg=""
   
        # revision_value always has the revision number, also for HEAD
        revision_valuearg=" --revision_value="+revision_value

        if release:
            releasearg=" --release="+release
        else: releasearg=""
    
        command=gsconf.run_regression_script + releasearg + revisionarg + revision_valuearg + timearg + trackarg

        if options.capture:
            capture=" 2>&1"
            command+=capture

        message=command
        logMessage(message,regression_file,revision_full,printMessage=False)
        logMessage(message,cumulative_file,revision_full)

        regression_results += '\nThe complete list of regressions for today:\n'
        regression_results += command +"\n"

        p = os.popen(command)
        if p == None:
            message="cannot open and run the regression script."
            logMessage(message,cumulative_file,revision_full)
            die(message)

        pattern = 'ok$'
        for line in p.readlines():
            if printRegression: print line
            if not re.search(pattern, line):
                regression_results += line
        p.close()

        # count number of entries in daily db
        if gsconf.__dict__.has_key("checksumdb") and gsconf.checksumdb:
            newchecksum=gsconf.checksumdb
        else:
            newchecksum=time.strftime("%Y%m%d", time.localtime())

    else:
        message="skipping regression testing"
        logMessage(message,regression_file,revision_full,printMessage=False)
        logMessage(message,cumulative_file,revision_full)

        regression_results+="gs regression processing was skipped"
    # end if runRegression

    regression_results += '\n'

    # create string "difference_results" for regression results
    if testRegression:
        print

        if gsconf.__dict__.has_key("checksumdb") and gsconf.checksumdb:
            newchecksum=gsconf.checksumdb
        else:
            newchecksum=time.strftime("%Y%m%d", time.localtime())

#        message="verify existence of the checksum database "+newchecksum
#        logMessage(message,cumulative_file,revision_full,printMessage=False)
#        logMessage(message,regression_file,revision_full)

        message="starting difference_results"
        logMessage(message,cumulative_file,revision_full,printMessage=False)
        logMessage(message,regression_file,revision_full)

        if options.oldrevision:
            oldchecksum=options.oldrevision
        else:
            # find the most recent checksum file in the gsconf.dailydir
            oneday=24*60*60
            testtime=time.time()
            for count in range(1,30):
                testtime-=oneday
                oldchecksum=time.strftime("%Y%m%d", time.localtime(testtime))
                datafile=gsconf.dailydir+oldchecksum+".db"
                if os.path.exists(datafile):
                    try:
                        db = anydbm.open(datafile)
                        if db:
                            db_hasdata=True
                            db.close()
                            if db_hasdata:
                                message="old checksum database used "+datafile
                                logMessage(message,cumulative_file,revision_full,printMessage=True)
                                break
                            else:
                                message="old checksum database empty "+datafile
                                logMessage(message,cumulative_file,revision_full,printMessage=True)
                    except:
                        message="old checksum database could not be opened "+datafile
                        logMessage(message,cumulative_file,revision_full,printMessage=True)
                else:
                    message="old checksum database does not exist "+datafile
                    logMessage(message,cumulative_file,revision_full,printMessage=True)
            else:
                message="cannot find any recent checksum database file",
                logMessage(message,cumulative_file,revision_full)
                message="FATAL cannot find any recent checksum database file"
                logMessage(message,cumulative_file,revision_full)
                die(message)

        oldchecksumpath = gsconf.dailydir + oldchecksum + ".db"
        if not os.path.exists(oldchecksumpath):
            message=myself+" FATAL "+"the old checksum database does not exist "+oldchecksumpath
            logMessage(message,cumulative_file,revision_full)
            die(message)

        recent_ctime = time.ctime(testtime)

        message="found recent checksum database "+oldchecksum
        logMessage(message,cumulative_file,revision_full,printMessage=False)
        logMessage(message,regression_file,revision_full)
        logMessage(message,history_file,revision_full,printMessage=False)

        difference_results = 'The following regression changes happened since recent report: '+oldchecksum
        difference_results += '\n'

        command=gsconf.testdiff_script + " " + oldchecksum + " " + newchecksum
        if options.capture:
            capture=" 2>&1"
            command+=capture

        logMessage(command,regression_file,revision_full,printMessage=False)
        logMessage(command,cumulative_file,revision_full)
        p = os.popen(command)
        if p:
            for line in p.readlines():
                if printRegression: print line
                difference_results += line
            p.close()
        else:
            difference_results = difference_results + 'no results from difference script\n'

        message="get baseline updates"
        logMessage(message,regression_file,revision_full,printMessage=False)
        logMessage(message,cumulative_file,revision_full)


        message="marcos: about to call get_baseline_log_script"
        logMessage(message,cumulative_file,revision_full)

        # Get updated baseline_changes
        baseline_changes = 'The following files had their baselines updated:\n'

        command=gsconf.get_baseline_log_script + " '" + recent_ctime + "'"
        baseline_changes += command+"\n"

        logMessage(command,cumulative_file,revision_full)
        p = Popen4(command,1)
        if p:
            for line in p.fromchild.readlines():
                baseline_changes += line
                p.wait()
        else:
            baseline_changes = baseline_changes + 'no results from baseline script\n'
            baseline_changes += "\n"

        message="marcos: done with get_baseline_log_script"
        logMessage(message,cumulative_file,revision_full)

        logMessage(baseline_changes,regression_file,revision_full)
        logMessage(difference_results,regression_file,revision_full)
    else:
        difference_results="difference testing was skipped"
    difference_results += '\n'
    # end if testRegression
    
    message="marcos: writing to differences file"
    logMessage(message,cumulative_file,revision_full)

    differences_file = open(differences_name, "w")
    message="differences"
    differences_file.write(message)
    logMessage(message,differences_file,revision_full,printMessage=False)


    if baselineChanges:
        print

        baselines_file = open(baselines_name, "w")
        message="baseline changes"
        baselines_file.write(message)
        logMessage(baseline_changes,baselines_file,revision_full,printMessage=False)

    else:
        baseline_changes = "difference testing was skipped"
        
    message="marcos: done writing to differences file"
    logMessage(message,cumulative_file,revision_full)

    # end if baselineChanges

    mail_message+=difference_results+"\n"
    mail_message+=regression_results+"\n"
    mail_message+=baseline_changes+"\n"

    email_file = open(email_name, "w")
    subject='gs regression report - %s - revision: %s' % (now,revision_full)

    message="subject "+subject
    logMessage(message,cumulative_file,revision_full)
    logMessage(message,email_file,revision_full,printMessage=False)

    message="send mail to "+gsconf.report_to
    logMessage(message,cumulative_file,revision_full)
    logMessage(message,email_file,revision_full,printMessage=False)

    logMessage(mail_message,email_file,revision_full,printMessage=False)
    email_file.close()

    if options.printmailmessage:
        print
        print "mail_message start"
        print mail_message
        print "mail_message end"    
        print

    while True:
        try:
            print "sendmail"
            sendmail(gsconf.report_from, gsconf.report_to, subject, mail_message)
            break
            time.sleep(5)
        except:
            print "sendmail fail"
            time.sleep(15)
            pass

    message="done"
    logMessage(message,cumulative_file,revision_full)

    regression_file.close()
    cumulative_file.close()
    history_file.close()

    print regression_name
    print cumulative_name
    print regression_name

    return 0

#########################################################

if __name__ == "__main__":

    os.umask(0002)

    optionsParser=optparse.OptionParser()

    optionsParser.add_option('--version',action='store_true',help="get my version")

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
    optionsParser.add_option('--nobaselinechanges',action='store_true',help="do not analyze baselinechanges")
    optionsParser.add_option('--printregression',action='store_true',help="print regression output")
    optionsParser.add_option('--printmailmessage',action='store_true',help="print mail message")

    optionsParser.add_option('--fullset',action='store_true',help="full gsparamsets")
    optionsParser.add_option('--minset',action='store_true',help="minimum gsparamsets")
    optionsParser.add_option('--maxset',action='store_true',help="maximum gsparamsets")
    optionsParser.add_option('--oneset',action='store_true',help="one gsparamsets")    

    optionsParser.add_option('--release',action='store_true',help="under contruction")
    optionsParser.add_option('--oldrevision',action='store',help="old revision for checksum comparison",default=None)

    (options,arguments)=myoptparse.parseCommandLine(optionsParser,deviceSkip=True,testfileSkip=True,listfileSkip=True)

    if options.version:
        print options.myself,"version",myversion
        sys.exit(1)

    result = run_nightly(options,arguments)

    sys.exit(result)
