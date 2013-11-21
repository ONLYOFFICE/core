#!/usr/bin/perl -w
# diffs two files and writes an HTML output file.
use strict;
use CGI qw(:standard :html3);
use Win32::Clipboard;

# MAIN PROGRAM
my $include = "";
my %tags = ();
my $i = 0x1c;

main();

exit 0;


sub main
{
	# Check for two arguments.
	print "usage: $0 tag_name [tag_name]\n" if @ARGV < 1;
	for(my $i = 0; $i < @ARGV; $i++)
	{
		generateClass($ARGV[$i]);
	}


	print $include;
	my $CLIP = Win32::Clipboard();
	$CLIP->Set($include);
}


sub generateClass
{

	my $tag_name = $_[0];

	$tag_name =~ s/[\[\]\(\)*\\\/]//g;
	$tag_name =~ s/\d*(.+)/${1}/g;
	return unless $tag_name; # if the tag absent

	return if $tags{$tag_name};
	
	if($tag_name =~/^[A-Z0-9]+$/)
	{
#		$include .= "#include <XLS_logic/Biff_unions/${tag_name}.h>\n";
	}
	else
	{
#		$include .= "#include <XLS_logic/Biff_records/${tag_name}.h>\n";
	}
#		$include .= "#include \"${tag_name}.h\"\n";
#	my $ii;
#	$ii = sprintf("%02X", $i++);
#	$include .= "\t\tcase 0x" . $ii . ":\n\t\t\tptg_record.reset(new ${tag_name}(record));\n\t\t\tbreak;\n";
	$include .= "\t\tcase 0x0060:\n\t\t\tptg_records.push_back(boost::shared_ptr<${tag_name}>(new ${tag_name}(record)));\n\t\t\tbreak;\n";


	#$tags{$tag_name} = "p";

}










