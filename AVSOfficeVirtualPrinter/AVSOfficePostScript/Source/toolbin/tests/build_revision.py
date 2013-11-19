#!/usr/bin/python
# -*- Mode: python -*-

#    Copyright (C) 2001-2006 Artifex Software Inc.
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

# This script builds a revision of gs, including svn update, configure, make, install
# 
# If the revision is HEAD, make install is done; and the svn tree is put (HEAD) and installed
# 	into gsconf.gsroot (usually /home/regression/gshead)
# 
# If the revision is not HEAD, no install is done; and the tree is put into gs.<revision>
# 
# If release is not None, then a release is built - in progress
# 


import os, sys, shutil
import time
import optparse, myoptparse
import string, re
import gsconf

myself="build_revision.py"
myversion="1.00"

def logMessage(message,file,revision,printMessage=True):
    message_time=time.strftime("%Y-%m-%d-%H:%M:%S", time.localtime())
    message=myself+" "+revision+" "+message_time+" "+message
    if printMessage:
        print message
    if file:
        message+="\n"
        file.write(message)
        file.flush()

def read_all_lines(f):
    file = open(f, 'r')
    lines = file.readlines()
    file.close()
    return string.join(lines, '')

def get_revision(dir=None):
    if dir:
        cwd=os.getcwd()
        os.chdir(dir)
    p = os.popen("svn info")
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

def update_ghostscript(revision,release,
                       gsroot,
                       options,
                       update_stdout,update_stderr,
                       config_stdout,config_stderr,
                       make_stdout,make_stderr,
                       install_stdout,install_stderr,
                       cumulative_file
                       ):

    myself=options.myself+" update_ghostscript"

    if options and options.__dict__.has_key("verbose") and options.verbose:
        print myself,gsroot

    if release:
        print myself,"release not yet supported"

    if options and options.__dict__.has_key("capture") and options.capture:
        captureOutput = True
    else:
        captureOutput = False

    update_stdout_file = open(update_stdout, "w")
    message="update_stdout"
    logMessage(message,update_stdout_file,revision,printMessage=False)
    update_stdout_file.close()

    update_stderr_file = open(update_stderr, "w")
    message="update_stderr"
    logMessage(message,update_stderr_file,revision,printMessage=False)
    update_stderr_file.close()

    cwd=os.getcwd()

    revision_file = gsroot+"revision"   # remove history until we have successfully completed the update"
    if os.path.exists(revision_file):
        os.unlink(revision_file)

    # to prevent errors from left-over installations, remove the install tree
    if revision == "HEAD":
        installtree=gsconf.installtree
        if os.path.exists(installtree):
            message="remove old installation directory tree "+installtree
            logMessage(message,cumulative_file,revision)
            # do a check for sanity of the path
            if os.path.exists(installtree+"/bin") and os.path.exists(installtree+"/bin/gs"):
                shutil.rmtree(installtree+"/bin",ignore_errors=True)
                shutil.rmtree(installtree+"/man",ignore_errors=True)
                shutil.rmtree(installtree+"/share",ignore_errors=True)

    update_stdout_file = open(update_stdout, "w")

    if options and options.__dict__.has_key("svn") and options.svn:

        product_file = "%sbase/gscdef.c" % (gsroot)
        if os.path.exists(product_file):
            os.unlink(product_file)

        # make the source tree corrrect from svn repository

        message="svn checkout "+gsroot
        logMessage(message,update_stdout_file,revision,printMessage=False)
        logMessage(message,cumulative_file,revision)

        host="http://svn.ghostscript.com/ghostscript/trunk/gs "

        if os.path.exists(gsroot):
            revisionarg="-r"+revision+" "
            command="svn update "+revisionarg+gsroot
        else:
            revisionarg="-r"+revision+" "
            command="svn co "+revisionarg+host+gsroot

        logMessage(message,update_stdout_file,revision,printMessage=False)
        logMessage(command,cumulative_file,revision)

        if captureOutput:
            capture=" >> "+update_stdout
            command+=capture
        if os.system(command) != 0:
            msg = "Ghostscript update failed during svn update\n\n"
            msg = msg + "stdout log:\n\n"
            msg = msg + read_all_lines(update_stdout)
            msg = msg + "\nstderr log:\n\n"
            msg = msg + read_all_lines(update_stderr)
            logMessage(message,cumulative_file,revision)
            return (1,msg)

        if revision == "HEAD":
            revision_value=get_revision(gsroot)
        else:
            revision_value = revision
            
        sys.modules["gsconf"].__dict__["revision"] = revision_value


    os.chdir(gsroot)

    message="revision is " + str(revision)
    logMessage(message,update_stdout_file,revision,printMessage=False)
    logMessage(message,cumulative_file,revision)

    if options and options.__dict__.has_key("configure") and options.configure:
        change_gsproduct(product_file)

        message="product change complete:"+product_file
        logMessage(message,update_stdout_file,revision,printMessage=False)
        logMessage(message,cumulative_file,revision,)
	
        if revision == "HEAD":
            installpath=gsconf.installtree
            command="./autogen.sh --prefix=" + installpath
            # HACK: configuring with cups support makes the build script
            # try to install outside of prefix. Disable for now.
            command+=" --disable-cups"
        else:
            command="./autogen.sh"
            
        logMessage(command,update_stdout_file,revision,printMessage=False)
        logMessage(command,cumulative_file,revision)
        if captureOutput:
            capture=" > " +  config_stdout + " 2> " + config_stderr
            command+=capture
        if os.system(command) != 0:
            msg = "Ghostscript update failed during configuration.\n\n"
            msg = msg + "stdout log:\n\n"
            msg = msg + read_all_lines(make_stdout)
            msg = msg + "\nstderr log:\n\n"
            msg = msg + read_all_lines(make_stderr)
            logMessage(message,cumulative_file,revision)
            return (1,msg)

        message="configuration complete"
        logMessage(message,update_stdout_file,revision,printMessage=False)
        logMessage(message,cumulative_file,revision)


    if options and options.__dict__.has_key("makeclean") and options.makeclean:
        command ="make clean "
        logMessage(command,update_stdout_file,revision,printMessage=False)
        logMessage(command,cumulative_file,revision)
        if captureOutput:
            capture=" > /dev/null 2> /dev/null"
            command+=capture
        if os.system(command) != 0:
            msg = "Ghostscript update failed during make clean\n\n"
            logMessage(message,cumulative_file,revision)
            return (1,msg)

        message="make clean complete"
        logMessage(message,update_stdout_file,revision,printMessage=False)
        logMessage(message,cumulative_file,revision)
    
    if options and options.__dict__.has_key("make") and options.make:
        command="make "
        logMessage(command,update_stdout_file,revision,printMessage=False)
        logMessage(command,cumulative_file,revision)
        if captureOutput:
            capture="> " + make_stdout + " 2> " + make_stderr
            command+=capture
        if os.system(command) != 0:
            msg = "Ghostscript update failed during make\n\n"
            msg = msg + "stdout log:\n\n"
            msg = msg + read_all_lines(make_stdout)
            msg = msg + "\nstderr log:\n\n"
            msg = msg + read_all_lines(make_stderr)
            logMessage(message,cumulative_file,revision)
            return (1,msg)

        message="make complete"
        logMessage(message,update_stdout_file,revision,printMessage=False)
        logMessage(message,cumulative_file,revision)

    if revision == "HEAD":
        command="make install "
        logMessage(command,update_stdout_file,revision,printMessage=False)
        logMessage(command,cumulative_file,revision)
        if captureOutput:
            capture="> " + install_stdout + " 2> " + install_stderr
            command+=capture
        if os.system(command) != 0:
            msg = "Ghostscript update failed during install\n\n"
            msg = msg + "stdout log:\n\n"
            msg = msg + read_all_lines(install_stdout)
            msg = msg + "\nstderr log:\n\n"
            msg = msg + read_all_lines(install_stderr)
            logMessage(message,cumulative_file,revision)
            return (1,msg)

        message="make install complete"
        logMessage(message,update_stdout_file,revision,printMessage=False)
        logMessage(message,cumulative_file,revision)

        message="installation directory tree is "+gsroot
        logMessage(message,cumulative_file,revision)

    revision_file_name="revision"
    revision_file = open(revision_file_name, "w")    
    message=revision_value+"\n"
    revision_file.write(message)
    revision_file.close()
    
    os.chdir(cwd) # return to directory 

    update_stdout_file.close()
    
    return (0,"success")

#########################################################

if __name__ == "__main__":

    os.umask(0002)
    
    optionsParser=optparse.OptionParser()

    optionsParser.add_option('--version',action='store_true',help="get my version")

    optionsParser.add_option('--revision',action='store',help="under contruction",default="HEAD")
    optionsParser.add_option('--release',action='store',help="under contruction",default=None)

    optionsParser.add_option('--time','-m',action='store',help="provide start time",default=None)

    optionsParser.add_option('--nosvn',action='store_true',help="do not update the revision source from the svn repository")
    optionsParser.add_option('--noconfigure',action='store_true',help="do not run auto configure")
    optionsParser.add_option('--nomakeclean',action='store_true',help="do not make clean before make")
    optionsParser.add_option('--nomake',action='store_true',help="do not make")

    optionsParser.add_option('--nocapture',action='store_true',help="do not capture stdout and stderr from commands")

    optionsParser.add_option('--remove',action='store_true',help="remove the built directories")
    
    (options,arguments)=myoptparse.parseCommandLineBasic(optionsParser)
    
    myself=options.myself

    if options.version:
        print options.myself,"version",myversion
        sys.exit(1)

    options.svn       = not options.nosvn
    options.configure = not options.noconfigure
    options.make      = not options.nomake
    options.makeclean = not options.nomakeclean

    options.capture   = not options.nocapture

    revision          = options.revision
    release           = options.release
    
    now=options.time

    logdir=gsconf.logdir
    if not os.path.exists(logdir):
        os.mkdir(logdir)

    if now:
        pass # use command line value of time
    else:
        now=time.strftime("%Y-%m-%d-%H:%M:%S", time.localtime())
    
    print myself,revision
    if revision == "HEAD":
        prefix=logdir+now+"."
        gsroot=gsconf.gsroot
        gspath=gsconf.installtree+"bin/gs"
        gsinstall=gsconf.installtree
    else:
        prefix=logdir+revision+"."+now+"."
        gsroot=gsconf.root+"gs."+revision+"/"
        gspath=gsroot+"bin/gs"
        gsinstall=None
        
    update_stdout=prefix+gsconf.update_stdout
    update_stderr=prefix+gsconf.update_stderr
    config_stdout=prefix+gsconf.config_stdout
    config_stderr=prefix+gsconf.config_stderr
    make_stdout=prefix+gsconf.make_stdout
    make_stderr=prefix+gsconf.make_stderr
    install_stdout=prefix+gsconf.install_stdout
    install_stderr=prefix+gsconf.install_stderr
        
    cumulative_file=None

    (err,message) = update_ghostscript(revision,release,
                             gsroot,
                             options,
                             update_stdout,update_stderr,
                             config_stdout,config_stderr,
                             make_stdout,make_stderr,
                             install_stdout,install_stderr,
                             cumulative_file=None
                             )
    if err != 0:
        print message
        sys.exit(1)

    revision=get_revision(gsroot)
        
    if not os.path.exists(gspath):
        message=myself+" FATAL "+"the gs executable does not exist "+gspath
        logMessage(message,None,revision)
        sys.exit(1)

    if options.remove:
        if os.path.exists(gsroot):
            print "shutil.rmtree(gsroot)",gsroot
            shutil.rmtree(gsroot)
            if gsinstall:
                print "shutil.rmtree(gsinstall)",gsinstall
                shutil.rmtree(gsinstall)            

    print myself,revision,"done"

    sys.exit(0)
