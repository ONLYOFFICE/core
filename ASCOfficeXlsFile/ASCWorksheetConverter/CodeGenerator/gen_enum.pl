#!/usr/bin/perl -w
# diffs two files and writes an HTML output file.
use strict;
use CGI qw(:standard :html3);

# MAIN PROGRAM

# Check for two arguments.
#print "usage: $0 file1 > out.txt\n" if @ARGV != 1;


open FH, "tags.txt";#$ARGV[0];

while (<FH>)
{
  if(/([A-Za-z][A-Za-z0-9_-]*)/)
  {
#	print "\trt_$1";
	print "\t{\"$1\",";
	for(my $i = 0; $i < 7 - (length($1)+4)/4; $i++)
	{
	  print "\t";
	}
	print "rt_$1},\n";
	next;
  }
  if(/(\d+)/)
  {
#	print $outFile "0x$1,\n";
#	printf("0x%04x,\n", $1);
	next;
  }
}

exit(0);










