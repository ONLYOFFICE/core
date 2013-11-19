#!/usr/bin/perl -w
# diffs two files and writes an HTML output file.
use strict;
use CGI qw(:standard :html3);

# MAIN PROGRAM

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



}


sub generateClass
{

my $tag_name = $_[0];

$tag_name =~ s/[\[\]\(\)*\\\/]//g;
$tag_name =~ s/\d*(.+)/${1}/g;
return unless $tag_name; # if the tag absent

#print "$tag_name \n";
#return;
my $cpp_file = "${tag_name}.cpp";
my $h_file = "${tag_name}.h";

################################# CPP #####################################
open FCPP, ">$cpp_file";

print FCPP <<CPP_L1;
#include "stdafx.h"
#include "${tag_name}.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


void ${tag_name}::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = createElement(parent, getClassName().c_str());

	own_tag->setAttribute("type", type);
	own_tag->setAttribute("row", row);
	own_tag->setAttribute("col", col);
}


void ${tag_name}::load(CFRecord& record)
{
	BYTE rec_type;
	record >> rec_type;
	type = GETBITS(rec_type, 5, 6);
//	record.skipNBytes(1); // record type
	record >> row >> col;
}


void ${tag_name}::assemble(PtgStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### ${tag_name} record is not implemented")
	std::cout << " Error!!! ${tag_name} record is not implemented." << std::endl;
	ptg_stack.push("#REF!");
}




} // namespace XLS

CPP_L1

close FCPP;

################################### header #############################################
open FH, ">$h_file";

print FH <<H_L1;
#pragma once

#include "Ptg.h"

namespace XLS
{;

class CFRecord;

class ${tag_name} : public Ptg
{
public:
	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);

	virtual void assemble(PtgStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent);

private:
	BYTE type;
	WORD row;
	WORD col;
};

} // namespace XLS

H_L1

close FH;

system "move /-Y $h_file D:\\Projects\\AVSWorksheetConverter\\source\\XLS_logic\\Biff_structures\\${h_file}";
system "move /-Y $cpp_file D:\\Projects\\AVSWorksheetConverter\\source\\XLS_logic\\Biff_structures\\${cpp_file}";
system "svn add D:\\Projects\\AVSWorksheetConverter\\source\\XLS_logic\\Biff_structures\\${tag_name}.*";

#system "devenv.exe /command \"File.AddExistingItem \"D:\\Projects\\AVSWorksheetConverter\\source\\XLS_logic\\Biff_records\\${tag_name}.h\"\""
}










