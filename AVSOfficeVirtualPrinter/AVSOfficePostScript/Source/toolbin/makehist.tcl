#!/usr/bin/tclsh

#    Copyright (C) 1999-2007 Artifex Software, Inc.
#    All rights reserved.
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

# $Id: makehist.tcl 8323 2007-10-26 01:49:56Z giles $

# Make various Ghostscript filesets.  Assumes the current directory is gs.
#   makehist
#	(merges doc/News.htm and doc/Changes.htm into doc/History#.htm)

# Acquire the Ghostscript version numbers.
proc setnum {num3} {
    global Num3 Dot Dir

    set Num3 $num3

    set Dot [expr ${Num3} / 100].[format %02d [expr ${Num3} % 100]]

# Set other, derived globals.
    set Dir gs$Dot
}
setnum [exec egrep {^[0-9]+$} lib/gs_init.ps]
set Work {c:\work}

# Execute a shell command with *- and ?-expansion.
proc sh {args} {
    if {[lindex $args 0] == "-c"} {
	set cmd [concat sh $args]
    } elseif {[regexp {[*?]} $args]} {
	set cmd [list sh -c [join $args]]
    } else {
	set cmd $args
    }
    puts stdout $cmd
    puts stdout [eval exec -- $cmd 2>@ stderr]
}

# Move all or none of a list of files to a directory.
proc movelist {name files todir} {
    set ex {}
    set nex {}
    foreach f $files {
	if {[file exists $f]} {lappend ex $f} else {lappend nex $f}
    }
    if {$ex == ""} {
	puts stderr "$name archives do not exist."
	return
    }
    if {$nex != ""} {
	puts stderr "Missing files: $nex"
	exit 1
    }
    if {![file isdirectory $todir]} {
	puts "Creating $todir"; flush stdout
	sh mkdir -p $todir
    }
    foreach f $files {
	if {[regexp {^/c} $f]} {
	    sh cp $f $todir
	} else {
	    sh mv $f $todir
	}
    }
    sh ls -l $todir/*
}

# Create a symbolic link from name to path.
proc ln-s {path name} {sh ln -s $path $name}

# Merge News and Changes into History#.
proc mergehist {news changes histn tmph details detailn tmpd} {
    # Merge News, Changes, and the existing History# to tmph; News, Details and Details# to tmpd.
    # Return 0 if OK, non-0 or an error if not.

    # Define the pattern for the version/date line.
    set vdpattern {Version ([0-9.]+) \((([0-9]+)-0*([1-9][0-9]*)-0*([1-9][0-9]*))\)}

    # Scan the News file to find the header line.
    while {![regexp {^<h[1-3]><a name="Version} [set l [gets $news]]]} {
	if {[eof $news]} {
	    puts stderr "EOF scanning News for header line"
	    return 1
	}
    }
    if {![regexp $vdpattern $l skip nver ndate nyear nmonth nday]} {
	puts stderr "Can't parse header line in News: $l"
	return 1
    }
    # Change the header tag to h1 for compatiability with existing files.
    regsub -all {h[1-3]>} $l h2> l
    set monthnames [list\
	January February March April May June\
	July August September October November December]
    set nmonthname [lindex $monthnames [expr $nmonth - 1]]

    # Read the rest of the News file to determine whether there are any
    # Incompatible Changes, which we need to know for the TOC entry.
    set nlines [list $l]
    set have_changes 0
    while {[string first "2.0 end contents" [set l [gets $news]]] < 0} {
	if {[eof $news]} {
	    puts stderr "EOF scanning News for Incompatible changes"
	    return 1
	}
	if {[string first "Incompatible changes</h" $l] >= 0} {
	    set have_changes 1
	}
	lappend nlines $l
    }

    # Copy the prefix of the existing History# file.
    while {[string first <li> [set l [gets $histn]]] != 0} {
	if {[eof $histn]} {
	    puts stderr "EOF copying History# up to <li>"
	    return 1
	}
	puts $tmph $l
    }

    # Make sure this isn't a backward rev.
    if {![regexp $vdpattern $l skip hver hdate]} {
	puts stderr "Can't parse header line in History: $l"
	return 1
    }
    if {$hver > $nver} {
	puts stderr "First History version = $hver > first News version = $nver"
	return 1
    }
    set hline $l

    # Copy the prefix of the existing Details# file.
    while {[string first <li> [set l [gets $detailn]]] != 0} {
	if {[eof $detailn]} {
	    puts stderr "EOF copying Detail# up to <li>"
	    return 1
	}
	puts $tmpd $l
    }
    set dline $l

    # Advance the Changes file to the first line of the body
    while {[string first <body> [set l [gets $changes]]] != 0} {
	if {[eof $changes]} {
	    puts stderr "EOF seeking Changes start"
	    return 1
	}
    }

    # Advance the Details file to the first line of the body
    while {[string first <body> [set l [gets $details]]] != 0} {
	if {[eof $details]} {
	    puts stderr "EOF seeking Detail start"
	    return 1
	}
    }

    # Create the new TOC entry.
    puts $tmph "<li><a href=\"#Version$nver\">Version $nver ($ndate)</a>"

    # Copy the rest of the History TOC and preamble.
    puts $tmph $hline
    while {[string first <h2> [set l [gets $histn]]] != 0} {
	if {[eof $histn]} {
	    puts stderr "EOF copying History TOC and preamble"
	    return 1
	}
	puts $tmph $l
    }

    # If there is a History section for this version, delete it.
    if {![regexp $vdpattern $l skip hver hdate]} {
	puts stderr "Can't parse header line in History: $l"
	return 1
    }
    if {$hver == $nver} {
	# Skip over the history section.
	while {[set l [gets $histn]] != "<hr>"} {
	    if {[eof $histn]} {
		puts stderr "EOF skipping old History section"
		return 1
	    }
	}
	# Skip the following blank line, too.
	gets $histn
	set l [gets $histn]
    }
    set hline $l

    # Create the new TOC entry.
    puts $tmpd "<li><a href=\"#Version$nver\">Version $nver ($ndate)</a>"

    # Copy the rest of the Details TOC and preamble.
    puts $tmpd $dline
    while {[string first <h2> [set l [gets $detailn]]] != 0} {
	if {[eof $detailn]} {
	    puts stderr "EOF copying Details TOC and preamble"
	    return 1
	}
	puts $tmpd $l
    }

    # If there is a Details section for this version, delete it.
    if {![regexp $vdpattern $l skip hver hdate]} {
	puts stderr "Can't parse header line in Details#: $l"
	return 1
    }
    if {$hver == $nver} {
	# Skip over the Details section.
	while {[set l [gets $detailn]] != "<hr>"} {
	    if {[eof $detailn]} {
		puts stderr "EOF skipping old Details section"
		return 1
	    }
	}
	# Skip the following blank line, too.
	gets $detailn
	    set l [gets $detailn]
    }
    set dline $l

    # Copy the comment and "incompatible changes" sections from News.
    foreach l $nlines {
	puts $tmph $l
	puts $tmpd $l
    }

    # Copy the rest of Changes, changing links to Details.htm to Details#.htm
    puts $tmph "<h3><a name=\"${nver}_changelog\"></a>Changelog</h3>"
    set inum [expr int($hver)]
    while {[string first </body> [set l [gets $changes]]] < 0} {
	if {[eof $changes]} {
	    puts stderr "EOF copying rest of Changes"
	    return 1
	}
	regsub {href=\"Details.htm#} $l "href=\"Details$inum.htm#" l
	puts $tmph $l
    }
    puts $tmph <hr>

    # Copy the rest of Details.
    puts $tmpd "<h3><a name=\"${nver}_changelog\"></a>Changelog</h3>"
    while {[string first </body> [set l [gets $details]]] < 0} {
	if {[eof $details]} {
	    puts stderr "EOF copying rest of Details"
	    return 1
	}
	puts $tmpd $l
    }
    puts $tmpd <hr>

    # Copy the rest of the History# file, changing the date at the end.
    puts $tmph ""
    puts $tmph $hline
    while {[string first "<small>Ghostscript version " [set l [gets $histn]]] != 0} {
	if {[eof $histn]} {
	    puts stderr "EOF seeking History# Ghostscript version"
	    return 1
	}
	puts $tmph $l
    }
    
    puts $tmph "<small>Ghostscript version $nver, $nday $nmonthname $nyear"
    while {[gets $histn l] >= 0} {
	puts $tmph $l
    }

    # Copy the rest of the Details# file, changing the date at the end.
    puts $tmpd ""
    puts $tmpd $dline
    while {[string first "<small>Ghostscript version " [set l [gets $detailn]]] != 0} {
	if {[eof $detailn]} {
	    puts stderr "EOF seeking Details# Ghostscript version"
	    return 1
	}
	puts $tmpd $l
    }
    
    puts $tmpd "<small>Ghostscript version $nver, $nday $nmonthname $nyear"
    while {[gets $detailn l] >= 0} {
	puts $tmpd $l
    }

    return 0
}
proc makehist {} {
    global Dot

    set tmphname /tmp/[pid]h.htm
    set tmpdname /tmp/[pid]d.htm
    set news [open doc/News.htm]
    set changes [open doc/Changes.htm]
    set details [open doc/Details.htm]
    set inum [expr int($Dot)]
    set histname doc/History$inum.htm
    set detailname doc/Details$inum.htm
    set historyn [open $histname]
    set detailn [open $detailname]
    set tmph [open $tmphname w]
    set tmpd [open $tmpdname w]
    set ecode [catch {set code [mergehist $news $changes $historyn $tmph $details $detailn $tmpd]} errMsg]
    close $tmph
    close $tmpd
    close $historyn
    close $detailn
    close $changes
    close $news
    if {$ecode == 0 && $code == 0} {
	file rename -force $tmphname $histname
	file rename -force $tmpdname $detailname
    } else {
	file delete $tmphname
	file delete $tmpdname
	puts "results in: $tmphname and $tmpdname"
	if {$ecode != 0} {
	    puts "Error encountered: $errMsg"
	    error $errMsg
	}
    }
}

# Run ourselves
eval makehist $args

