#!/usr/bin/tclsh

#    Copyright (C) 1998, 2000 Aladdin Enterprises.  All rights reserved.
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

# $Id: precheck.tcl 6300 2005-12-28 19:56:24Z giles $

# Check various aspects of an about-to-be-released source fileset.

# Sets the following globals for use by files using this one:
#	CHANGED(f) - defined as 1 for each f such that f~ or f.orig exists
#	NEWSCHANGED(f) - defined as 1 for each f listed as changed in the
#	  NEWS* file
#	NEWSDATE - the date in NEWSVERSION
#	NEWSVERSION - the most recent Version line from the NEWS* file

# ---------------- Utilities ---------------- #

# Print an error message.
proc message {str} {
    puts stderr "**** $str"
}

# Search for one or more occurrences of a pattern in a file.
# If any are found, set $linevar to the line containing the first occurrence,
# and return 1; otherwise, return 0.
proc grep1 {pattern file linevar} {
    upvar $linevar line
    set line ""
    set cmd [list exec egrep $pattern $file]
    catch "set line \[$cmd\]"
    if {$line == ""} {return 0}
    set line [lindex [split $line "\n"] 0]
    return 1
}

# ---------------- Main program ---------------- #

# Identify the relevant NEWS file, setting NEWS.

set news {}
foreach f [glob -nocomplain NEWS* News*.htm doc/News*.htm] {
    if {[regexp {^(doc/|)(NEWS([-_][A-Z0-9]*|)|News([-_][A-Za-z0-9]*|)\.htm)$} $f]} {lappend news $f}
}
if {[llength $news] != 1} {
    if {$news == {}} {set news -nothing-}
    puts "**** Looking for NEWS or News.htm, found: $news"
    exit 1
}
set NEWS [lindex $news 0]

# Check the date and version number in the NEWS file.
# Set NEWSVERSION

if {![grep1 {^Version [0-9.]+[( ]} $NEWS vdate]} {
    if {![grep1 {[<]/a>Version [0-9.]+[( ]} $NEWS vdate]} {
	message "Unable to parse $NEWS!"
	exit 1
    }
}
regsub -all {<[^>]*>} $vdate "" vdate
puts "Most recent version in $NEWS: $vdate"
set vdmypat {^Version [0-9.]+(.*)\((([0-9x]+)/([0-9x]+)/([0-9x]+))\)$}
set vymdpat {^Version [0-9.]+(.*)\((([0-9x]+)-([0-9x]+)-([0-9x]+))\)$}
if [regexp $vdmypat $vdate skip skip2 rdate rmonth rday ryear] {
    set dateformat %-m/%-d/%y
} elseif [regexp $vymdpat $vdate skip skip2 rdate ryear rmonth rday] {
    set dateformat %Y-%m-%d
} else {
    message "$NEWS release date is missing"
    exit 1
}
if {![string match *x* $ryear]} {
    if {$ryear >= 1900} {
    } elseif {$ryear >= 50} {
	set ryear [expr $ryear + 1900]
    } else {
	set ryear [expr $ryear + 2000]
    }
}
set curdate [exec date +$dateformat]
if {$rdate != $curdate} {
    message "Warning: current date is $curdate, release date is $rdate"
}
set NEWSVERSION $vdate

# Collect the list of changed files (those with a ~-file or .orig file,
# other than t and t.*).

proc changed_files {} {
    set changed {}
    foreach prefix {doc/ examples/ lib/ man/ src/ toolbin/ gnu/*/} {
	foreach fx [glob -nocomplain ${prefix}*~ ${prefix}*.orig] {
	    if {[regexp {^(.*)(~|\.orig)$} $fx skip f]} {
		lappend changed $f
	    }
	}
    }
    return $changed
}
catch {unset CHANGED}
foreach f [changed_files] {set CHANGED($f) 1}

# Collect the list of files identified as changed in the current NEWS
# section.

proc news_changed_files {in} {
    set changed {}
    set para {}
    set fnamepat {[-0-9A-Za-z_.*/]+}
    set fn1pat "${fnamepat}( \\\[deleted\\\]| => $fnamepat|)"
    set fnlistpat "${fn1pat}(,\[ \]*$fn1pat)*"
    while {[gets $in l] >= 0 && ![regexp {(^|</a>)Version} $l]} {
	set l [string trimright $l]
	if {$l == "" || $l != [string trimleft $l]} {
	    # End of paragraph.  Look for changed files in the previous one.
	    regsub {\) \*\*\*(.*)\*\*\*$} $para {)} para
	    if {[regexp "\\(($fnlistpat)\\)\$" $para skip files]} {
		regsub -all {,} $files { } files
		foreach f $files {
		    if {$f == {[deleted]}} {continue}
		    if {![regexp / $f]} {set f src/$f}
		    set fl [glob -nocomplain $f]
		    if {$fl != ""} {append changed " $fl"}
		}
	    }
	    set para $l
	} else {
	    append para " $l"
	}
    }
    return $changed
}
catch {unset NEWSCHANGED}
set news [open $NEWS]
while {![regexp {(^|</a>)Version} [gets $news]]} { }
foreach f [news_changed_files $news] {set NEWSCHANGED($f) 1}
close $news

# Check that the files with ~- or .orig-files are 1-for-1 with files listed
# in the current NEWS section.

puts "[array size CHANGED] files with ~ or .orig,\
 [array size NEWSCHANGED] changed in $NEWS."

# We'd like to handle gs_init.ps properly, but its embedded version number
# changes in every fileset.  **** SPECIAL HACK FOR GS ****
foreach f "$NEWS lib/gs_init.ps src/version.mak" {catch {unset CHANGED($f)}}
foreach f {lib/gs_init.ps} {catch {unset NEWSCHANGED($f)}}
set c_n {}
foreach f [array names CHANGED] {if {![info exists NEWSCHANGED($f)]} {lappend c_n $f}}
set n_c {}
foreach f [array names NEWSCHANGED] {if {![info exists CHANGED($f)]} {lappend n_c $f}}
if {$c_n != ""} {puts "**** Changed, not in $NEWS: [lsort $c_n]"}
if {$n_c != ""} {puts "**** In $NEWS, not changed: [lsort $n_c]"}
