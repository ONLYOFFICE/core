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

# $Id: leaks.tcl 6300 2005-12-28 19:56:24Z giles $

# This tool helps detect memory leaks in a -ZA trace from Ghostscript.
# It reads a memory trace from stdin and prints unmatched allocations on
# stdout.  Currently it is slightly specialized for our PCL5 environment,
# in that it looks for the string "allocated" in the trace to mark the
# beginning of the interesting region, and the string "Final time" to
# mark the end.  Usage:
#	<<program>> -Z:A ... >t.log
#	leaks.tcl <t.log >t.report

# We keep track of the trace in the following global arrays:
#	A(<addr>) holds a string of the form line#:line of the last
#	   allocation event that allocated a block at address addr.
#	lines(<i>) holds other interesting lines of the input trace file -
#	  the "allocated" and "Final" lines, and anomalous alloc/free events.
#	next holds the line number of the next line.

proc init_leaks {} {
    global A lines next
    catch {unset A}
    catch {unset lines}
    set next 0
}

# The addMXN procedures handle input events as follows:
#	M=1 for allocator events, M=0 for other events
#	X=+ for allocation, X=- for deallocation
#	N=1 if A(addr) exists, N=0 if not
proc add1+0 {il addr} {global A;set A($addr) $il}
proc add1+1 {il addr} {
    global A lines
    regexp {^([0-9]+):(.*)$} $A($addr) all i l
    puts "**** Warning: reallocation: $il"
    puts "**** Previous allocation: $A($addr)"
    set lines($i) $l
    set A($addr) $il
}
proc add1-1 {il addr} {global A;unset A($addr)}
proc add1-0 {il addr} {
    global lines
    if {$addr == "0"} {return}
    regexp {^([0-9]+):(.*)$} $il ignore i l
    puts "**** Warning: no alloc event: $il"
    set lines($i) $l
}
proc add0+0 {il addr} {
    if [regexp {Final|allocated} $il] {
	uplevel {set lines($n) $l}
	if [regexp "Final time" $il] {uplevel {set go 0}}
    }
}
proc add0+1 {il addr} [info body add0+0]
proc add0-0 {il addr} [info body add0+0]
proc add0-1 {il addr} [info body add0+0]

proc read_trace {{fname %stdin}} {
    global A lines next
    set n $next
    if {$fname == "%stdin"} {
	set in stdin
    } else {
	set in [open $fname]
    }
    # Skip to the first "allocated" line.  See below for why we bother
    # checking for EOF.
    while {[gets $in l] >= 0} {
	if [regexp "memory allocated" $l] break
	incr n
    }
    if {$n == 0} {
	puts stderr "Empty input file!"
	if {$fname != "%stdin"} {close $in}
	exit
    }
    set lines($n) $l
    incr n
    set sign +			;# arbitrary, + or -
    set addr ""			;# arbitrary
    set go 1
    # When processing a syntactically correct trace file, add0+0 will set
    # go to 0 when it detects the "Final time" line; but we add a check here
    # just so invalid files won't loop forever.
    while {$go && [gets $in l] >= 0} {
	add[regexp {^\[a.*([+-]).*\].*0x([0-9a-f]+)} $l all sign addr]${sign}[info exists A($addr)] "$n:$l" $addr
	incr n
    }
    if {!$go} {
	# This is normal termination.  The last line has not been stored.
	incr n -1
	set lines($n) $l
	incr n
    }
    if {$fname != "%stdin"} {
	close $in
    }
    set next $n
}

proc print_leaks {} {
    global A lines
    foreach addr [array names A] {
	regexp {^([0-9]+):(.*)$} $A($addr) all i l
	set lines($i) $l
    }
    foreach i [lsort -integer [array names lines]] {
	puts "$i: $lines($i)"
    }
}

if {$argv0 != "tclsh"} {
    init_leaks
    read_trace
    print_leaks
}
