#!/usr/bin/tclsh

#    Copyright (C) 1997, 2000 Aladdin Enterprises.  All rights reserved.
# 
# This software is provided AS-IS with no warranty, either express or
# implied.
# 
# This software is distributed under license and may not be copied,
# modified or distributed except as expressly authorized under the terms
# of the license contained in the file LICENSE in this distribution.
# 
# For more information about licensing, please refer to
# http://www.ghostscript.com/licensing/. For information on
# commercial licensing, go to http://www.artifex.com/licensing/ or
# contact Artifex Software, Inc., 101 Lucas Valley Road #110,
# San Rafael, CA  94903, U.S.A., +1(415)492-9861.

# $Id: suite.tcl 6300 2005-12-28 19:56:24Z giles $

# Run some or all of a Genoa test suite, optionally checking for memory leaks.
# Command line syntax:
set HELP {Usage:
    suite (--[no-]band | --[no-]check | --[no-]debug |
       --[no-]display[=<device>] | --[no-]missing | --[no-]pause |
       --[no-]print[=<device>] | --[no-]profile | --[no-]remote[=<host>] |
       --[no-]sort | --[no-]together | -<switch> |
       <dirname>[,<filename>] | <filename>)*
}

# Note: test failure is typically indicated by one or more of the following:
#	exit status
#	Error:
#	fail/FAIL
#	wrong:
#	*not* Final time

set SW(band) 1			;# use banding
set SW(check) 0			;# check for memory leaks
set SW(debug) 0			;# don't actually execute the test
set SW(display) 0		;# use display instead of printer
				;# (or device if value != 1)
set SW(missing) 0		;# only run if log file is missing
set SW(pause) 0			;# pause after each page
set SW(print) 0			;# don't discard output, print for LJ4
				;# (or device if value != 1)
set SW(profile) 0		;# assume -pg executable, profile execution
set SW(remote) 0		;# run on remote host
set SW(sort) 1			;# do in order of increasing file size
set SW(together) 0		;# run all files together, not individually

#----------------------------------------------------------------

# In addition to the switches, the following globals are used:
#	HOSTNAME = the current host name
#	REMOTED(remotename) = 1 if the given file has been copied to
#	  the given host

proc test_args {band display pause print xe switches output} {
    set args [list -K40000 -Z@:? -dBATCH]
    if {!$pause} {
	lappend args -dNOPAUSE
    }
    if {$display != "0"} {
				# Use the default device, or a given device.
	if {$display != "1"} {
	    lappend args -sDEVICE=$display -sOutputFile=/dev/null
	}
    } elseif {$print == "0"} {
	lappend args -r600 -sDEVICE=pbmraw -sOutputFile=/dev/null
    } else {
	if {$print == "1"} {set print ljet4}
	lappend args -r600 -sDEVICE=$print -sOutputFile=${output}.%03d.$print
    }
    if $band {
	lappend args -dMaxBitmap=200000 -dBufferSpace=200000
    } else {
	lappend args -dMaxBitmap=20000000 -dBufferSpace=20000000
    }
    set args [concat $args $switches]
    if {$xe == "gs"} {
	set args [concat $args {-c false 0 startjob pop -f}]
    }
    return $args
}

proc file_type {file} {
    if {[regexp {\.[a-zA-Z0-9]*(ps|PS)[a-zA-Z0-9]*$} $file]} {return ps}
    # Look for PJL in the first part of the file.
    set in [open $file]
    set first [read $in 2000]
    close $in
    if {[regexp {@PJL[ ]+ENTER[ ]+LANGUAGE[ ]*=[ ]*PCLXL} $first]} {return pclxl}
    if {[regexp {@PJL[ ]+ENTER[ ]+LANGUAGE[ ]*=[ ]*PCL} $first]} {return pcl5}
    if {[regexp "\033E" $first]} {return pcl5}
    # Unknown, assume PS.
    return ps
}

proc test_xe {file} {
    global SW

    switch [file_type $file] {
	ps {set cmd gs}
	pcl5 {set cmd pcl5}
	pclxl {set cmd pclxl}
    }
    if {$SW(profile)} {
	set cmd pgobj/$cmd
    } elseif {[file exists bin/$cmd]} {
	set cmd bin/$cmd
    } elseif {[file exists obj/$cmd]} {
	set cmd obj/$cmd
    }
    return $cmd
}

proc catch_exec {command} {
    global SW

    puts $command; flush stdout
    if {!$SW(debug) && [catch [concat exec $command] msg]} {
	puts "*** Non-zero exit code from command:"
	puts $command
	puts $msg
    }
}

proc show_exec {command} {
    global SW

    puts $command; flush stdout
    if {!$SW(debug)} {eval exec $command}
}

proc output_name {fl} {
    if {[llength $fl] == 1} {
	set output [lindex $fl 0]
    } else {
	set output "[lindex $fl 0]-[lindex $fl end]"
    }
    regsub -all / $output - output
    # Don't allow the output file name to start with -.
    regsub {^[-]+} $output {} output
    return $output
}

proc remote_name {file host} {
    global HOSTNAME

    set local $file
    if {![regexp {^/} $local]} {
	set local [pwd]/$local
    }
    while {![catch {set link [file readlink $local]}]} {
	set dir [file dirname $local]
	switch $dir {
	    . {set local $link}
	    / {set local /$link}
	    default {set local $dir/$link}
	}
    }
    regsub "^(/home/$HOSTNAME/|/$HOSTNAME/home/)" $local {} local
    if {[regexp {^/} $local]} {
	return $local
    }
    return /home/$host/$local
}

proc copy_to_remote {file host {cached 1}} {
    global REMOTED

    set remote [remote_name $file $host]
    set cmd [list rsync -tz -e ssh $file $host:[file dirname $remote]]
    if {!$cached} {
	show_exec $cmd
    } elseif {![info exists REMOTED($remote)]} {
	show_exec $cmd
	set REMOTED($remote) 1
    }
}

proc copy_from_remote {file host} {
    set remote [remote_name $file $host]
    show_exec [list rsync -tz -e ssh $host:$remote [file dirname $file]]
}

proc suite {filelist switches} {
    global SW

    set files [list]
    if $SW(together) {
	set left $filelist
	set max_files 100
	set max_files1 [expr $max_files - 1]
	while {[llength $left] > $max_files} {
	    lappend files [lrange $left 0 $max_files1]
	    set left [lreplace $left 0 $max_files1]
	}
	if {$left != {}} {
	    lappend files $left
	}
    } else {
	foreach f $filelist {lappend files [list $f]}
    }
    foreach fl $files {
	set test_xe [test_xe [lindex $fl 0]]
	set output [output_name $fl]
	set test_args [test_args $SW(band) $SW(display) $SW(pause) $SW(print) $test_xe $switches $output]
	if $SW(pause) {
	    set in1 [list]
	    set in0 [list]
	} else {
	    set in1 [list -_ <]
	    set in0 [list -_]
	}
	if {![regexp {gs$} $test_xe]} {
	    set pre [list]
	    set post $fl
	    set lib [glob lib/*.ps]
	} elseif {[llength $fl] == 1} {
	    set pre [list]
	    set post [concat $in1 [list [lindex $fl 0]]]
	    set lib [list]
	} else {
	    set pre [concat cat $fl |]
	    set post $in0
	    set lib [list]
	}
	if $SW(check) {
	    puts $fl
	    flush stdout
	    set command [concat $pre $test_xe -ZA $test_args $post > t]
	    catch_exec $command
	    exec leaks < t > ${output}.leak
	} else {
	    set log ${output}.log
	    set main [concat $pre time $test_xe $test_args $post]
	    set command [concat $main | tee -a $log >@ stdout 2>@ stderr]
	    if {$SW(missing) && [file exists $log]} {
		# Do nothing.
	    } elseif {$SW(profile)} {
		if {$SW(remote) != "0"} {
		    global HOSTNAME

		    set remdir [pwd]
		    regsub "^(/home/$HOSTNAME/|/$HOSTNAME/home/)" $remdir /home/$SW(remote)/ remdir
		    set command "cd $remdir; export GS_LIB=./lib:./fonts; $main >> $log"
		    exec echo $command > $log
		    copy_to_remote $test_xe $SW(remote)
		    foreach lf $lib {copy_to_remote $lf $SW(remote)}
		    copy_to_remote $log $SW(remote) 0
		    catch_exec [list ssh $SW(remote) $command >@ stdout 2>@ stderr]
		    copy_from_remote gmon.out $SW(remote)
		    copy_from_remote $log $SW(remote)
		} else {
		    exec echo $command > $log
		    catch_exec $command
		}
		show_exec [list gprof $test_xe > ${output}.out]
	    } else {
		exec echo $command > $log
		catch_exec $command
	    }
	}
    }
}

# Define the suites, sorted by increasing file size.
# We compute the sorted lists lazily.
proc compare_file_size {file1 file2} {
    expr [file size $file1] - [file size $file2]
}
proc get_suite {dir {sort 1}} {
    global list

    set files [glob -nocomplain $dir/*.bin]
    if {$files == {}} {
	set files [glob -nocomplain $dir/*.ps]
    }
    if !$sort {
	return [lsort $files]
    }
    if [info exists list($dir)] {
	return $list($dir)
    }
    set files [lsort -command compare_file_size $files]
    if {[lindex $files 0] == "0release.bin"} {
	set files [lrange $files 1 end]
    }
    return [set list($dir) $files]
}

# Run the program.
if {$argv == [list --help] || ($argv == "" && [regexp {suite$} $argv0])} {
    global HELP
    puts -nonewline stderr $HELP
    puts -nonewline stderr Defaults:
    foreach var [lsort [array names SW]] {
	if {$SW($var) != 0} {
	    puts -nonewline stderr " --$var"
	}
    }
    puts stderr ""
    exit
}

regexp {^([^.]+)(\.|$)} [exec hostname] skip HOSTNAME
catch {unset REMOTED}
set switches ""
set files ""
puts "-- [exec date]"
puts "-- $argv0 $argv"
foreach file $argv {
    if [regexp {^-} $file] {
	if {$files != ""} {suite $files $switches; set files ""}
	if {[regexp {^--no-(.*)$} $file all var]} {
	    if [info exists SW($var)] {
		set SW($var) 0
	    } else {
		puts "Unknown switch $file"
		exit 1
	    }
	} elseif {[regexp {^--(.*)$} $file all var]} {
	    if [info exists SW($var)] {
		set SW($var) 1
	    } elseif {[regexp {^(display|print|remote)=(.*)$} $var all var value]} {
		set SW($var) $value
	    } else {
		puts "Unknown switch $file"
		exit 1
	    }
	} else {
	    lappend switches $file
	}
    } elseif {[file isdir $file]} {
	if {$files != ""} {suite $files $switches; set files ""}
	suite [get_suite $file $SW(sort)] $switches
    } elseif {[regexp {(.*),(.*)$} $file all suite from] && [file isdir $suite]} {
	if {$files != ""} {suite $files $switches; set files ""}
	set sfiles [get_suite $suite $SW(sort)]
	set first [lsearch $sfiles $suite/$from]
	if {$first < 0} {
	    puts "No such file: $suite/$from"
	} else {
	    suite [lreplace $sfiles 0 [expr $first - 1]] $switches
	}
    } else {
	lappend files $file
    }
}
if {$files != ""} {suite $files $switches; set files ""}
