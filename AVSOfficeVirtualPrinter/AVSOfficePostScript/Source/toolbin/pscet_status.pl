#!/usr/bin/perl -w
$/="";

# script for generating reports from the pscet status document
# $Id: pscet_status.pl 8022 2007-06-05 22:23:38Z giles $

$mydb = "pscet_status.txt";
open(MYDB,$mydb) or die "Unable to open $mydb:$!\n";

$total=0;
$unclass=0;
$reviewed=0;
$ok=0;
$assigned=0;
$unassigned=0;
$repeats=0;
$diff=0;
$device_dependent=0;
$VERBOSE=0;

$JOBPAGEREG="^[0-9][^ \t]*";

while (<MYDB>) {
  if (/$JOBPAGEREG/) {
    $matches = 0;
    @lines = split('\n');
    for (@lines) { $matches++ if /$JOBPAGEREG/; }
    print "bad entry: $matches, $_" if $matches != 1;
    $total++;
    if (/DIFF/) {
      $diff++;
      $device_dependent++ if (/Device Dependent/i);
      if (/count:/) {
        $unclass++;
      } else {
        $reviewed++;
        if (/assign/i) {
          $assigned++;
        } else {
          $unassigned++;
          print "Unassigned $_";
        }
        $repeats++ if (/(repeat|same as)/); # && print "repeat $_\n";
      }
    } else {
      if (/(OK|AOK)/) {
        $ok++;
      } else {
        print "not classified\n";
      }
    }
  } else {
    print "bad record: $_\n" if $VERBOSE;
  }

}

close(MYDB);

print "Total pages=$total\n";
print "Total pages ok or aok=$ok\n";
print "Total different=$diff\n";
print "Total pages unclassified=$unclass\n";
print "Total pages reviewed and different (not accepted as ok or aok)=$reviewed\n";
print "Total reviewed and assigned=$assigned\n";
print "Total reviewed and unassigned=$unassigned\n";
print "possible duplicate problems using keywords 'repeat' and 'same as' (overestimates)=$repeats\n";
print "Device dependent problems (different)=$device_dependent\n";
