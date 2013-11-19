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

namespace OSHARED
{;


void ${tag_name}::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = BiffStructure::createElement(parent, getClassName().c_str());

	own_tag->setAttribute("something", something);
	//std::for_each(rgRun.begin(), rgRun.end(), boost::bind(&FormatRun::toXML, _1, own_tag));
	/*
	if(fExtSt)
	{
		extRst.toXML(own_tag);
	}
	*/
}


void ${tag_name}::load(XLS::CFRecord& record)
{
	record.skipNBytes(2); // reserved
	WORD flags;
	record >> flags;
	record >> something >> something >> something >> something >> something;
	GenerateCompileError();
}


} // namespace OSHARED

CPP_L1

close FCPP;

################################### header #############################################
open FH, ">$h_file";

print FH <<H_L1;
#pragma once

#include "BiffStructure.h"
//#include <XLS_logic/Biff_structures/Phs.h>
//#include <XLS_logic/Biff_structures/BiffString.h>

namespace OSHARED
{;

class CFRecord;

class ${tag_name} : public XLS::BiffStructure
{
public:
	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(XLS::CFRecord& record);

private:
	WORD something;
};

} // namespace OSHARED

H_L1

close FH;

system "move /-Y $h_file D:\\Projects\\AVSWorksheetConverter\\source\\XLS_logic\\Biff_structures\\${h_file}";
system "move /-Y $cpp_file D:\\Projects\\AVSWorksheetConverter\\source\\XLS_logic\\Biff_structures\\${cpp_file}";
system "svn add D:\\Projects\\AVSWorksheetConverter\\source\\XLS_logic\\Biff_structures\\${tag_name}.*";

#system "devenv.exe /command \"File.AddExistingItem \"D:\\Projects\\AVSWorksheetConverter\\source\\XLS_logic\\Biff_records\\${tag_name}.h\"\""
}










