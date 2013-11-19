#!/bin/sh
# hack to restart using tclsh \
exec tclsh "$0" "$@"

#    Copyright (C) 1999, 2000, 2001 Aladdin Enterprises.  All rights reserved.
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

set TMAKE_ID {$Id: tmake.tcl 6300 2005-12-28 19:56:24Z giles $}

# tmake is intended to be a drop-in replacement for a large and
# useful subset of 'make'.  It compiles makefiles into Tcl scripts
# (lazily) and then executes the scripts.  It requires makefiles to be
# well-behaved:
#
#	- If a rule body modifies a file, then either that file is a
#	target of the rule, or the file is not a target or dependent
#	of any rule.
#
#	- If a rule body reads a file, then either that file is a
#	dependent of the rule, or the file is not a target of any rule.
#
#	- No target is the target of more than one rule.
#
# When tmake executes a build rule, it also optionally checks whether the
# dependencies in the makefile are up to date.  It can also do some
# Ghostscript-specific checks:
#
#	- The definition of xxx_h must be the list of files included
#	(directly or indirectly) by xxx.h.
#
#	- If a rule command includes the word "-include", then for
#	dependency checking, .dev is appended to any further words in that
#	command.

# Define the backward-compatibility version of this file.
set TMAKE_VERSION 106

#****** -j doesn't work yet ******#

# The following switches set the corresponding global variables used during
# execution of the compiled makefile:
set O(-d) {DEBUG o_01}		;# print a debugging trace
set O(-i) {IGNORE_ERRORS o_01}	;# ignore errors in rule bodies
set O(-j) {MAX_JOBS o_max}	;# maximum number of concurrent rule
				;# executions (NYI)
set O(-k) {KEEP_GOING o_01}	;# continue past errors, but don't build
				;# anything affected
set O(-l) {MAX_LOAD o_max}	;# maximum load for parallel execution
set O(-n) {DRYRUN o_01}		;# just print commands, don't execute them
set O(-s) {SILENT o_01}		;# don't print commands
set O(--check-dependencies)\
	{CHECK_DEPENDENCIES o_01};# check dependency lists against sources
set O(--check-gs-dependencies)\
	{CHECK_GS_DEPENDENCIES o_01};# check GS-specific dependencies
set O(--warn-multiply-defined-variables)\
	{WARN_MULTIPLE o_01}	;# warn about variables defined more than once,
				;# even if the definitions are identical
set O(--warn-redefined-variables)\
	{WARN_REDEFINED o_01}	;# warn about redefined variables
set O(--warn-undefined-variables)\
	{WARN_UNDEFINED o_01}	;# warn about undefined variables (every time)
set O(--warn-undefined-variables-once)\
	{WARN_UNDEFINED_ONCE o_01};# warn about undefined variables (only once)

# The following additional globals are defined during execution
#	MAKEFLAGS - flags for recursive invocations

set CHECKS [list\
    CHECK_DEPENDENCIES CHECK_GS_DEPENDENCIES\
]
set WARNINGS [list\
    WARN_MULTIPLE WARN_REDEFINED WARN_UNDEFINED\
]
set OPTIONS {}
foreach opt [array names O] {lappend OPTIONS [lindex $O($opt) 0]}
set GLOBALS "$OPTIONS\
    MAKEFLAGS\
"
proc init_globals {} {
    global OPTIONS GLOBALS

    foreach v $GLOBALS {global $v}
    foreach v $OPTIONS {set $v 0}
    set MAKEFLAGS ""
    set MAX_JOBS 1
    set MAX_LOAD 99999
}

# ================ Compiled script conventions ================ #

# Rule T1 ... Tk : D1 ... Dm
#	B1
#	...
#	Bn
# becomes
#	L <sourceline>; R "T1 ... Tk" "list D1 ... Dm" "list B1 ... Bn"
#
# Macro definition XYZ = DEF becomes
#	L <sourceline>; P XYZ DEF
#
# Macro usage $(XYZ) becomes [_XYZ] so we can use Tcl's 'unknown' facility
# to default macro values to the empty string, since Tcl apparently does
# not provide a way to trap references to undefined variables.
#
# At run time, for each target or dependent ABC, the procedure [@ABC]
# builds ABC (if necessary) and returns its build time.

# ================ Command line processing ================ #

# Argument type handlers
proc o_01 {vvar argv} {
    upvar $vvar var
    set var 1
    return 0
}
proc o_max {vvar argv} {
    upvar $vvar var
    if {[llength $argv] > 1 && [regexp {^[0-9]+$} [lindex $argv 1]]} {
	set var [lindex $argv 1]; return 1
    } else {
	set var 99999; return 0
    }
}

proc tmake_args {args} {
    global O GLOBALS CHECKS COMPILE DEFINES JOBS MAKEFILE TARGETS WARNINGS
    foreach v $GLOBALS {global $v}

    set argv $args
    while {[llength $argv] > 0} {
	set n 0
	set copy 1
	set arg [lindex $argv 0]
	switch -glob -- $arg {
	    -C {    # -C is not implemented
		set copy 0
	    }
	    --check-all {
		foreach v $CHECKS {set $v 1}
	    }
	    --compile-only {	# compile-time switch only
		set COMPILE 1
	    }
	    -f {set MAKEFILE [lindex $argv 1]; set n 1; set copy 0}
	    -m {    # -m is ignored for compatibility with GNU make;
		    # also, because MAKEFLAGS omits the initial '-', we need a
		    # dummy switch in case there are variable definitions (!).
		set copy 0
	    }
	    --warn-all {
		foreach v $WARNINGS {set $v 1}
	    }
	    -* {
		if [info exists O($arg)] {
		    set opt $O($arg)
		    set n [[lindex $opt 1] [lindex $opt 0] $argv]
		} else {
		    puts "Unknown option: $arg"
		    puts {Usage: tmake (<option> | <var>=<value> | <target>)*}
		    puts {Options:}
		    puts {    --check-all --check-dependencies --check-gs-dependencies}
		    puts {    --compile-only -d -i -k -n -s}
		    puts {    --warn-all --warn-multiply-defined-variables --warn-redefined-variables}
		    puts {    --warn-undefined-variables --warn-undefined-variables-once}
		    puts {    -f <file> -j <jobs> -l <load>}
		    exit
		}
	    }
	    *=* {
		regexp {^([^=]*)=(.*)$} $arg skip lhs rhs
		lappend DEFINES [list $lhs $rhs]
		set copy 0
	    }
	    default {
		lappend TARGETS $arg
		set copy 0
	    }
	}
	if $copy {lappend MAKEFLAGS [lrange $argv 0 $n]}
	set argv [lreplace $argv 0 $n]
    }
}
proc tmake {args} {
    global argv0
    global GLOBALS COMPILE DEFINES JOBS MAKEFILE TARGETS
    global TMAKE_TIME TMAKE_VERSION
    foreach v $GLOBALS {global $v}

    set TMAKE_TIME [file mtime $argv0]
    init_globals
    set MAKEFILE makefile
    set TARGETS ""
    set DEFINES [list]
    set COMPILE 0
    set JOBS {}
    eval tmake_args $args
    # POSIX requires the following nonsense:
    regsub {^-([^-])} $MAKEFLAGS {\1} MAKEFLAGS
    if {$MAKEFLAGS == ""} {set MAKEFLAGS m}
    foreach d $DEFINES {
	append MAKEFLAGS " [lindex $d 0]='[lindex $d 1]'"
    }
    init_definition
    foreach d $DEFINES {
	catch {unset _[lindex $d 0]}
	V [lindex $d 0] [lindex $d 1] command-line
	set _($d) 1
    }
    if {$COMPILE} {
	# Just compile the given makefile(s).
	tcompile $MAKEFILE $TMAKE_VERSION
    } {
	# Build the selected targets.
	tsource $MAKEFILE $TMAKE_VERSION
	init_execution
	foreach t $TARGETS {
	    global errorInfo TARGET_FAILED

	    set TARGET_FAILED ""
	    switch [set status [catch "@$t" result]] {
		0 {continue}
		1 {if {$TARGET_FAILED != ""} {exit 1}}
	    }
	    puts stderr $errorInfo
	    exit $status
	}
    }
}

# ================ Compilation ================ #

# ---------------- Utilities ---------------- #

# Convert variable references from $(vname) to [_vname],
# escape characters that need to be quoted within "",
# and surround the result with "".
proc quote {defn {refsvar ""}} {
    set orig $defn
    set fixed ""
    set refs {}
    while {[regexp {^(([^$]|\$[^$(])*)\$(\$|\(([^)]*)\))(.*)$} $orig skip pre skip2 dollar var orig]} {
	regsub -all {([][\"$])} $pre {\\\1} pre
	if {$dollar == "\$"} {
	    append fixed "$pre\\\$"
	} else {
	    append fixed "$pre\[_$var\]"
	}
	lappend refs $var
    }
    regsub -all {([][\"$])} $orig {\\\1} orig
    append fixed $orig
    if {[string match {*[ \\]*} $fixed] || $fixed == ""} {
	return "\"$fixed\""
    }
    if {$refsvar != ""} {
	upvar $refsvar rv
	set rv $refs
    }
    return $fixed
}

# ---------------- Writing ---------------- #

# Write the boilerplate at the beginning of the converted file.
proc write_header {out fname} {
    global TMAKE_ID TMAKE_VERSION

    puts $out {#!/bin/tcl}
    puts $out "# $fname created [exec date] by tmake with"
    puts $out "#   TMAKE_VERSION = ${TMAKE_VERSION}"
    set id $TMAKE_ID
    regexp {^[$]Id: (.*) [$]$} $id skip id
    puts $out "#   TMAKE_ID = $id"
}

# Write the definition of a macro.
proc write_macro {out var defn linenum} {
    puts $out "L [list $linenum];P $var {[quote $defn]}"
}

# Write an 'include'.
proc write_include {out fname} {
    global TMAKE_VERSION

    puts $out "tsource [quote $fname] $TMAKE_VERSION"
}

# Write a rule.
proc write_rule {out targets deps commands linenum} {
	# Convert all uses of 'make' or $(MAKE) in rule bodies to tmake.
    set body list
    foreach c $commands {
	regsub {^(make|\$\(MAKE\)) } $c {tmake $(MAKEFLAGS) MAKELEVEL=$(MAKELEVEL_1) } c
	append body " [quote $c]"
    }
    puts $out "L [list $linenum];R [quote $targets] [quote [string trim $deps]] [list $body]"
}

# ---------------- Top level ---------------- #

proc lgets {in lvar lnvar} {
    upvar $lvar line $lnvar linenum
    set line ""
    set len [gets $in line]
    if {$len < 0} {return $len}
    incr linenum
    while {[regsub {\\$} $line {} line]} {
	if {[gets $in l] < 0} {break}
	incr linenum
	append line $l
    }
    return [string length $line]
}

proc mak2tcl {inname {outname ""}} {
    set in [open $inname]
    if {$outname == ""} {
	set out stdout
    } {
	set out [open $outname w]
    }
    write_header $out $outname
    set linenum 1
    set line ""
    while {1} {
	while {$line == ""} {
	    set lnfirst $linenum
	    if {[lgets $in line linenum] < 0} break
	}
	if {$line == ""} break
	if {[string index $line 0] == "#"} {
	    set line ""
	    continue
	}
	if {[regexp {^([0-9A-Za-z_]+)[ ]*=[ ]*(.*)[ ]*$} $line skip var defn]} {
	    write_macro $out $var $defn ${inname}:$lnfirst
	    set line ""
	    continue
	}
	if {[regexp {^([^:]+):(.*)$} $line skip targets deps]} {
	    set commands {}
	    while {[lgets $in line linenum] > 0 && [regexp {^[#	]} $line]} {
		regsub {^[	]} $line {} line
		lappend commands $line
	    }
	    write_rule $out $targets $deps $commands ${inname}:$lnfirst
	    continue
	}
	if {[regexp {^(!|)include[ ]+("|)([^ "]*)("|)$} $line skip skip2 skip3 fname]} {
	    write_include $out $fname
	    set line ""
	    continue
	}
	# Recognize some GNU constructs
	if {[regexp {^unexport } $line]} {
	    set line ""
	    continue
	}
	puts "${inname}:$lnfirst: Unrecognized line: $line"
	set line ""
    }
    if {$out != "stdout"} {
	close $out
    }
    close $in
}

proc tcompile {fname version} {
    global TMAKE_TIME

    set mf $fname
    while {![catch {set mf [file readlink $mf]}]} {}
    set tf ${mf}.tcl
    if {![file exists $tf] || [file mtime $tf] < [file mtime $mf] || [file mtime $tf] < $TMAKE_TIME} {
	puts "Compiling $mf to $tf."
	flush stdout
	mak2tcl $mf $tf
    }
    return $tf
}

# ================ Runtime support ================ #

# The R procedure numbers rules consecutively.  For rule i:
#	C(i) is the rule body (a list of shell command lines)
#	T(i) is the list of targets
#	N(target) is the source line number

# Other runtime global variables:
#	I counts the number of defined rules
#	DEPS(file) is the implicit dependencies for a file

# ---------------- Definition ---------------- #

proc init_definition {} {
    global DEBUG I DEPS

    set I 0
    catch {unset DEPS}
    if {$DEBUG} {
	proc ifdebug {script} {uplevel $script}
    } {
	proc ifdebug {script} {}
    }
    V MAKELEVEL 0 implicit
}

proc var_value {var} {
    # Skip over the "return"
    set value [info body _$var]
    if {[regexp "^proc _$var {} \\\[list return (.*)\\\];" $value skip value]} {
    } else {
	regexp {^return (.*)$} $value skip value
    }
    return $value
}

proc unsubst_vars {defn} {
    set result ""
    while {[regexp {^((\\.|[^\\[])*)\[_([^]]*)\](.*)$} $defn skip before skip2 var defn]} {
	set result "${result}${before}\$\($var\)"
    }
    return "${result}${defn}"
}

proc redefined {var value lnum} {
    global WARN_MULTIPLE WARN_REDEFINED

    if {$WARN_MULTIPLE || ($WARN_REDEFINED && $value != [var_value $var])} {
	global _

	set old_lnum $_($var)
	if {!(($old_lnum == "default" || $old_lnum == "implicit") && $lnum == "command-line")} {
	    set old_value [var_value $var]
	    puts "${lnum}: warning: variable `$var' redefined as [unsubst_vars $value]"
	    puts "${old_lnum}: variable `$var' previously defined as [unsubst_vars $old_value]"
	}
    }
}

# Set the source line number.
proc L {lnum} {
    global LN

    set LN $lnum
}

# Record a macro value.
proc V {var value lnum} {
    global _

    if {![info exists _($var)]} {
	ifdebug {puts "$var=$value"}
	set _($var) $lnum
	proc _$var {} [list return $value]
    } elseif {[set old $_($var)] == "default"} {
	unset _($var)
	V $var $value $lnum
    } elseif {$old != "command-line"} {
	redefined $var $value $lnum
	unset _($var)
	V $var $value $lnum
    }
}

# Define a macro.
proc P {var vexpr} {
    global _ LN

    if {![info exists _($var)]} {
	ifdebug {puts "$var=$vexpr"}
	set _($var) $LN
	proc _$var {} "proc _$var {} \[list return $vexpr\];_$var"
    } elseif {[set old $_($var)] == "default"} {
	unset _($var)
	P $var $vexpr
    } elseif {$old != "command-line"} {
	redefined $var $vexpr $LN
	unset _($var)
	P $var $vexpr
    }
}

# Define a rule.
# Record the very first target as the default target.
proc R {tl dl body} {
    global TARGETS

    if {$TARGETS == ""} {
	lappend TARGETS [lindex $tl 0]
    }
    proc R {tl dl body} {
	global C I N T LN

	set C([incr I]) $body
	foreach t [set T($I) $tl] {
	    set N($t) $LN
	    proc @$t {} [list target $t $dl $I]
	}
	ifdebug {puts "#${I}: ${LN}: $tl <= $dl"}
    }
    R $tl $dl $body
}

# ---------------- Dependency checking ---------------- #

# Normalize a file name by removing all occurrences of ./,
# all occurrences of <dir>/../,
# and any occurrences of ../<dir>/ that are vacuous relative to CWD.
proc normalize_fname {fname} {
    global CWD

    set parts {}
    set count 0
    foreach part [split $fname /] {
	if {$part == "" || $part == "."} continue
	if {$part == ".."} {
	    if {$parts == ""} {
		incr count
	    } else {
		set parts [lreplace $parts end end]
	    }
	} else {
	    lappend parts $part
	}
    }
    if {$parts == ""} {return /}
    set i [expr [llength $CWD] - $count]
    if {$i >= 0} {
	while {$count > 0 && $parts != "" && [lindex $CWD $i] == [lindex $parts 0]} {
	    set parts [lrange $parts 1 end]
	    incr count -1
	    incr i
	}
    }
    while {$count > 0} {
	set parts ".. $parts"
	incr count -1
    }
    if {$parts == ""} {return .}
    return [join $parts /]
}
proc normalize_tail {fname} {
    return [file tail [normalize_fname $fname]]
}

# Return the indirect dependencies of a .c / .cpp / .h file.
set EXTDEPS(.c) c_deps
set EXTDEPS(.cpp) c_deps
proc c_deps {dep} {
    global errorCode

    set list ""
    set gre {^#[ 	]*include[ 	]+\"|/\*|\*/}
    set re {^#[\ \	]*include[\ \	]+"([^"]*)"}
    set out ""
    if {[catch {set out [exec egrep $gre $dep]}] != 0 && !([lindex $errorCode 0] == "CHILDSTATUS" && [lindex $errorCode 2] == 1)} {
	tmake_error "Can't read $dep for dependency check."
    } else {
	set active 1
	foreach inc [split $out "\n"] {
	    # The handling of comment brackets isn't quite right,
	    # but it's good enough for all likely uses.
	    if [regexp $re $inc skip fname] {
		if $active {lappend list $fname}
	    } elseif {[string first */ $inc] >= 0} {
		set active 1
	    } elseif {[string first /* $inc] >= 0} {
		set active 0
	    }
	}
    }
    ifdebug {puts "deps: $dep => $list"}
    return $list
}
set EXTDEPS(.h) h_deps
proc h_deps {dep} {
    global _ CHECK_GS_DEPENDENCIES

    set list [c_deps $dep]
    set DEPS($dep) $list	;# do early for check_dep_set
    if {$CHECK_GS_DEPENDENCIES && [regsub {\.h$} [normalize_tail $dep] _h var] != 0 && [info procs _$var] != ""} {
	check_dep_set $var [_$var] [list $dep] "" $_($var)
    }
    return $list
}
proc deps {dep} {
    global DEPS EXTDEPS

    if [info exists DEPS($dep)] {
	return $DEPS($dep)
    }
    set extn [file extension $dep]
    if {![info exists EXTDEPS($extn)]} {
	# No information.
	return ""
    }
    return [set DEPS($dep) [$EXTDEPS($extn) $dep]]
}

# Check the dependencies of a rule or _h definition.
proc check_dep_set {target deplist rootlist extraroots lnum} {
    # What we require is that the set of files in the dependency list is
    # identical (considering only the tails) to the set of files in the
    # transitive closure, under the operation of getting indirect
    # dependencies, of the set of root files.
    foreach d $deplist {set isdep([normalize_tail $d]) $d}
    foreach r $extraroots {set isroot([normalize_tail $r]) $r}
    set roots $rootlist
    while {$roots != ""} {
	set root [lindex $roots 0]
	set roots [lrange $roots 1 end]
	set norm [normalize_tail $root]
	set isroot($norm) $root
	if {![info exists isdep($norm)]} {
	    set missing($norm) 1
	}
	foreach inc [deps $root] {
	    set norm [normalize_tail $inc]
	    if [info exists isdep($norm)] {
		lappend roots $isdep($norm)
	    } else {
		set missing($norm) 1
	    }
	}
    }
    if {[array size missing] > 0} {
	puts "${lnum}: $target is missing dependencies: [array names missing]"
    }
    # Now the only elements left in isdep that aren't in isroot are ones
    # that weren't in the transitive closure, which is an error.
    if {[array size isdep] > 0} {
	set extra ""
	foreach d [array names isdep] {
	    if {![info exists isroot($d)]} {lappend extra $d}
	}
	if {$extra != ""} {
	    puts "${lnum}: $target has extra dependencies: $extra"
	}
    }
}

# Check the dependencies of a rule about to be executed.
proc check_deps {i dl} {
    # The root files are those files in the dependency list that are
    # mentioned in some command in the rule body.  This is not foolproof,
    # but it is a very good heuristic.
    global C N T

    foreach d $deplist {set isdep([normalize_tail $d]) $d}
    set roots ""
    foreach cmd [eval $C($i)] {
	foreach word [split $cmd { 	'"`;}] {
	    if [info exists isdep([normalize_tail $word])] {
		lappend roots $word
	    }
	}
    }
    set xroots ""
    if [regexp {^(.*):[0-9]+$} $lnum skip fname] {
	lappend xroots $fname
    }
    ifdebug {puts "roots: $roots"}
    set targets $T($i)
    check_dep_set [string trim $targets] $roots $xroots $N([lindex $targets 0])
}

# ---------------- Execution ---------------- #

# ****** NONE OF THE FOLLOWING IS IMPLEMENTED YET. ******
# For each rule i needing execution:
#	S(i) is the rule's execution status:
#	    -2 if the rule has not been executed yet
#	    -1 if the rule is being executed
#	    0 if the rule completed execution successfully
#	    >0 (the exit status) if execution ended with an error
#	W(i) is the list of rules with S() < 0 that must be executed
#	    before rule i
#	A(i) is the list of rules j such that W(j) includes i
# In addition,
#	PW(j) == i for some j, 0 <= j < [array size PW], iff S(i) == -2
#	PX(j) == i for some j, 0 <= j < [array size PX], iff S(i) == -1

proc init_execution {} {
    global CWD

    set CWD [split [exec pwd] /]
    if {[lindex $CWD 0] == ""} {
	set CWD [lrange $CWD 1 end]
    }
    foreach v {S W A PW PX} {
	global $v
	catch {unset $v}
    }
}

# Patch in case we're running a pre-8.0 tcl.
if {[info command clock] == ""} {
    proc clock {ignore} {exec date +%s}
}

# Replace the following on systems that don't have /proc
# (don't ask me how!).
proc proc_loadavg {} {
    set fid [open /proc/loadavg]
    set load [lindex [read $fid] 0]
    close $fid
    return $load
}
proc proc_exists {pid} {
    return [file exists /proc/$pid]
}

proc tset {p t} {
    proc @$p {} [list return $t]
    ifdebug {puts "ftime($p) <- $t"}
}

# Handle an undefined variable.
proc unknown_ {cmd var} {
    global WARN_UNDEFINED WARN_UNDEFINED_ONCE LN

    if {$WARN_UNDEFINED || $WARN_UNDEFINED_ONCE} {
	puts "${LN}: warning: undefined variable `$var'"
	set def $WARN_UNDEFINED_ONCE
    } else {
	set def 1
    }
    if {$def} {V $var "" default}
    return ""
}

# Handle an undefined dependent.
proc unknown@ {cmd var} {
    if {[catch {tset $var [file mtime $var]}]} {
	global N TARGET_FAILED

	set tlist "$var"
	set i -1
	puts "*** No rule to make target $var"
	catch {while {1} {
	    set cmd [info level $i]
	    if {[lindex $cmd 0] == "target"} {
		set t [lindex $cmd 1]
		if {[info exists N($t)]} {
		    set at "$N($t):"
		} {
		    set at ""
		}
		puts "\trequired by ${at}$t"
	    }
	    incr i -1
	}}
	puts "*** Can't continue."
	set TARGET_FAILED $var
	return -errorcode error
    }
    return [$cmd]
}

# Hook into tcl's undefined-procedure mechanism.
rename unknown old_unknown
proc unknown {cmd args} {
    if {[regexp {^([_@])(.*)$} $cmd skip 1st var]} {
	if {$args == ""} {return [unknown$1st $cmd $var]}
    }
    eval old_unknown [concat [list $cmd] $args]
}

proc reap_jobs {} {
    global JOBS

    set jobs {}
    foreach j $JOBS {
	if {[proc_exists [lindex $j 0]]} {
	    lappend jobs $j
	}
    }
    set JOBS $jobs
}

proc shell_exec {cmds} {
    global JOBS MAX_JOBS MAX_LOAD

    set args [list sh -c - $cmds <@ stdin >@ stdout 2>@ stderr]
    if {$MAX_JOBS <= 1} {
	return [eval exec $args]
    }
    while {[llength $JOBS] > 0 &&
	    ([llength $JOBS] > $MAX_JOBS || [proc_loadavg] >= $MAX_LOAD)} {
	# There doesn't seem to be any standard way of either yielding
	# the CPU, or sleeping for less than 1 second....
	reap_jobs
    }
    lappend JOBS [eval exec $args &]
}

# Print an error message.
proc tmake_error {str} {
    set level [_MAKELEVEL]
    if {$level == 0} {set level ""} {set level "\[$level\]"}
    puts "tmake$level: *** $str"
}

# Execute the commands in the body of rule i.
proc rexec {i dl} {
    global C T CHECK_DEPENDENCIES DRYRUN IGNORE_ERRORS SILENT

    set cmds [eval $C($i)]
    set ok 1
    if {$CHECK_DEPENDENCIES} {
	check_deps $i $dl
    }
    if {$DRYRUN} {
	foreach c $cmds {
	    if {!$SILENT || ![regexp {^@} $c]} {puts $c}
	}
	flush stdout
    } else {
	set status 0
	foreach c $cmds {
	    if {!([regsub {^@} $c "" c] || $SILENT)} {puts $c}
	    set ignore [regsub {^-} $c "" c]
	    set status [catch {shell_exec $c}]
	    if {$status != 0 && !($ignore || $IGNORE_ERRORS)} {break}
	}
	flush stdout
	if {$status} {
	    global errorCode IGNORE_ERRORS KEEP_GOING

	    set info $errorCode
	    set code 255
	    catch {
		if {[lindex $info 0] == "CHILDSTATUS"} {
		    set code [lindex $info 2]
		}
	    }
	    tmake_error "\[$T($i)\] Error $code"
	    if {!$IGNORE_ERRORS} {
		if {!$KEEP_GOING} {exit $code}
		set ok 0
	    }
	}
    }
    # Set the last mod time of dummy targets to -infinity, so that they
    # won't force their dependents to rebuild.
    foreach t $T($i) {
	if {[file exists $t]} {
	    tset $t [file mtime $t]
	} {
	    tset $t -0x80000000
	}
    }
    return $ok
}

proc target {t dl i} {
    if {[catch {set mt [file mtime $t]}]} {
	ifdebug {puts "no ttime($t)"}
	foreach d $dl {@$d}
	rexec $i $dl
	return [@$t]
    }
    ifdebug {puts "ttime($t)=$mt"}
    set do 0
	# The 'functional' interpretation of dependency would allow us
	# to stop as soon as we reach the first dependent that is newer
	# than the target, but all 'make' programs build all dependents,
	# and some 'operational' rules depend on this.
    foreach d $dl {
	# For safety, the following test should be a >= rather than a >,
	# but this causes excessive unnecessary rebuilding because of
	# rules whose bodies take <1 second to execute.
	if {[@$d] > $mt} {
	    ifdebug {puts "time($d)=[@$d] > ttime($t)=$mt"}
	    set do 1
	}
    }
    if {$do} {rexec $i $dl; return [@$t]}
    tset $t $mt
    ifdebug {puts "OK: $t"}
    return $mt
}

proc _MAKEFLAGS {} {
    global MAKEFLAGS

    set flags $MAKEFLAGS
    if {[regexp {^[^-]} $flags]} {set flags "-$flags"}
    V MAKEFLAGS $flags implicit
    return $flags
}
proc _MAKELEVEL_1 {} {
    V MAKELEVEL_1 [set level1 [expr [_MAKELEVEL] + 1]] implicit
    return $level1
}

proc tsource {fname {version 0}} {
    set tf [tcompile $fname $version]
    uplevel [list source $tf]
}

# ================ Do it ================ #

eval tmake $argv
