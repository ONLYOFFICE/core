#!/usr/bin/perl -w
# diffs two files and writes an HTML output file.
use strict;
use CGI qw(:standard :html3);
use Win32::Clipboard;

# MAIN PROGRAM
my $include = "";
my @names = ();
my $i = 0x1c;

main();

exit 0;


sub main
{
	# Check for two arguments.
	print "usage: $0 file_name\n" if @ARGV != 1;
	generateClass($ARGV[0]);


	print $include;
	my $CLIP = Win32::Clipboard();
	$CLIP->Set($include);
}


sub generateClass
{

	my $file_name = $_[0];

	open FH, $file_name;

	my $i = 0;

	my $setbit_inc = '';
	while (<FH>)
	{
		while(/([A-Za-z] - )?([A-Za-z0-9_-]+) \((\d+) bits*\):(.*)/)
		{
			my $name = $2;
			my $bits = $3;
			my $the_rest = $4;
			$_ = $the_rest;

			$include .= "\t${name} = ";
			$include .= ($bits > 1) ? "GETBITS" : "GETBIT";
			$include .= "(flags, $i";
			
                        $setbit_inc .= "\t";
			$setbit_inc .= ($bits > 1) ? "SETBITS" : "SETBIT";
			$setbit_inc .= "(flags, $i, ";
			
			$i += $bits - 1;
			$include .= ($bits > 1) ? ", $i);\n" : ");\n";
			$setbit_inc .= ($bits > 1) ? "$i, " : "";
			$setbit_inc .= "${name});\n";
			
			$i++;
			push(@names, "$name($bits)");
	
		}
	}
	$include .= "\n" . $setbit_inc;
	
	$include .= "\n\n";

	foreach my $iname (@names)
	{
		$iname =~ /(.+)\((\d+)\)/;
		$include .= "\t";
		if($2 == 1)
		{
			$include .= 'bool';
		}
		elsif($2 <= 8)
		{
			$include .= 'BYTE';
		}
		elsif($2 <= 16)
		{
			$include .= 'WORD';
		}
		else
		{
			$include .= 'DWORD';
		}


		$include .= " $1;\n";	
	}
	$include .= "\n\n";

	foreach my $iname (@names)
	{
		$iname =~ /(.+)\((\d+)\)/;
		$include .= "\t own_tag->setAttribute(L\"$1\", $1);\n";
	}

	$include .= "\n\n";

	foreach my $iname (@names)
	{
		$iname =~ /(.+)\((\d+)\)/;
		$include .= "\t $1 = getStructAttribute(own_tag, L\"$1\");\n";
	}

	$include .= "\n\n";

	foreach my $iname (@names)
	{
		$iname =~ /(.+)\((\d+)\)/;
		$include .= "\t\tBO_ATTRIB_MARKUP_ATTRIB($1)\n";
	}

}










