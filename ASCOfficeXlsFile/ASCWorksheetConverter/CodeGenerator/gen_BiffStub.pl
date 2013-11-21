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

namespace XLS
{;

${tag_name}::${tag_name}(MSXML2::IXMLDOMDocument2Ptr xml_doc, MSXML2::IXMLDOMElementPtr parent)
:	BiffRecord(xml_doc, parent)
{
}


${tag_name}::~${tag_name}()
{
}


BaseObjectPtr ${tag_name}::clone()
{
	return BaseObjectPtr(new ${tag_name}(*this));
}


void ${tag_name}::readFields(CFRecord& record)
{
#pragma message("####################### ${tag_name} record is not implemented")
	std::cout << " Error!!! ${tag_name} record is not implemented." << std::endl;
	//record >> some_value;
}


/*
bool ${tag_name}::loadContent(MSXML2::IXMLDOMElementPtr own_tag, CFStreamCacheReader& reader)
{
	cell.toXML(xml_doc_, own_tag);
	return true;
}
*/

} // namespace XLS

CPP_L1

close FCPP;

################################### header #############################################
open FH, ">$h_file";

print FH <<H_L1;
#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of ${tag_name} record in BIFF8
class ${tag_name}: public BiffRecord
{
//	BIFF_RECORD_DEFINE_TYPE_INFO(${tag_name})
public:
	${tag_name}(MSXML2::IXMLDOMDocument2Ptr xmlDoc, MSXML2::IXMLDOMElementPtr parent);
	~${tag_name}();

	BaseObjectPtr clone();
        
	void readFields(CFRecord& record);
	//bool loadContent(MSXML2::IXMLDOMElementPtr own_tag, CFStreamCacheReader& reader);
private:
//	WORD userName;
public:
	BO_ATTRIB_MARKUP_BEGIN
//		BO_ATTRIB_MARKUP_ATTRIB(userName)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

H_L1

close FH;

system "move /-Y $h_file D:\\Projects\\AVSWorksheetConverter\\source\\XLS_logic\\Biff_records\\${h_file}";
system "move /-Y $cpp_file D:\\Projects\\AVSWorksheetConverter\\source\\XLS_logic\\Biff_records\\${cpp_file}";
system "svn add D:\\Projects\\AVSWorksheetConverter\\source\\XLS_logic\\Biff_records\\${tag_name}.*";

#system "devenv.exe /command \"File.AddExistingItem \"D:\\Projects\\AVSWorksheetConverter\\source\\XLS_logic\\Biff_records\\${tag_name}.h\"\""
}










