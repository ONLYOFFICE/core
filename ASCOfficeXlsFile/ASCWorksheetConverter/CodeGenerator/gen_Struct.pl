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


BiffStructurePtr ${tag_name}::clone()
{
	return BiffStructurePtr(new ${tag_name}(*this));
}


void ${tag_name}::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"something", something);
	//std::for_each(rgRun.begin(), rgRun.end(), boost::bind(&FormatRun::toXML, _1, xml_tag));
	/*
	if(fExtSt)
	{
		extRst.toXML(xml_tag);
	}
	*/
}


void ${tag_name}::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
#pragma message("####################### ${tag_name} record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! ${tag_name} record has no BiffStructure::fromXML() implemented.");
}


void ${tag_name}::store(CFRecord& record)
{
#pragma message("####################### ${tag_name} record has no BiffStructure::store() implemented")
	Log::error(" Error!!! ${tag_name} record has no BiffStructure::store() implemented.");
	//record << something;
}


void ${tag_name}::load(CFRecord& record)
{
	record.skipNBytes(2); // reserved
	WORD flags;
	record >> flags;
	record >> something >> something >> something >> something >> something;
	GenerateCompileError();
}


} // namespace XLS

CPP_L1

close FCPP;

################################### header #############################################
open FH, ">$h_file";

print FH <<H_L1;
#pragma once

#include "BiffStructure.h"
//#include <XLS_logic/Biff_structures/Phs.h>
//#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class ${tag_name} : public BiffStructure
{
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	WORD something;
};

typedef boost::shared_ptr<${tag_name}> ${tag_name}Ptr;

} // namespace XLS

H_L1

close FH;

system "move /-Y $h_file ..\\source\\XLS_logic\\Biff_structures\\${h_file}";
system "move /-Y $cpp_file ..\\source\\XLS_logic\\Biff_structures\\${cpp_file}";
system "svn add ..\\source\\XLS_logic\\Biff_structures\\${tag_name}.*";

#system "devenv.exe /command \"File.AddExistingItem \"D:\\Projects\\AVSWorksheetConverter\\source\\XLS_logic\\Biff_records\\${tag_name}.h\"\""
}










