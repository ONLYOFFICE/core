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

my $cpp_file = "$tag_name.cpp";
my $h_file = "$tag_name.h";


################################# CPP #####################################
open FCPP, ">$cpp_file";

print FCPP <<CPP_L1;
#include "stdafx.h"
#include "${tag_name}.h"
//#include <XLS_logic/Biff_records/InterfaceHdr.h>

//#include <XLS_logic/Biff_unions/PAGESETUP.h>

namespace XLS
{;


${tag_name}::${tag_name}(MSXML2::IXMLDOMDocument2Ptr xml_doc, MSXML2::IXMLDOMElementPtr parent)
:	CompositeObject(xml_doc, parent)
{
}


${tag_name}::~${tag_name}()
{
}


// ${tag_name} = 
const bool ${tag_name}::loadContent(MSXML2::IXMLDOMElementPtr own_tag, CFStreamCacheReader& reader)
{
#pragma message("####################### ${tag_name} union is not implemented")
	std::cout << " Error!!! ${tag_name} union is not implemented!!!!!!!!!!!!!!!" << std::endl;

/*
	if(!loadFirstMandatory<InterfaceHdr>(own_tag, reader))
	{
		return false;
	}
	loadMandatory<Mms>(own_tag, reader);
	loadOptional<Mms>(own_tag, reader);
	loadChildN<Continue>(own_tag, reader, 0, 0);
*/

	return true;
}

} // namespace XLS

CPP_L1

close FCPP;

################################### header #############################################
open FH, ">$h_file";

print FH <<H_L1;
#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of ${tag_name} union of records 
class ${tag_name}: public CompositeObject
{
public:
	${tag_name}(MSXML2::IXMLDOMDocument2Ptr xmlDoc, MSXML2::IXMLDOMElementPtr parent);
	~${tag_name}();

	virtual const bool loadContent(MSXML2::IXMLDOMElementPtr own_tag, CFStreamCacheReader& reader);

};

} // namespace XLS

H_L1

close FH;

system "move $h_file D:\\Projects\\AVSWorksheetConverter\\source\\XLS_logic\\Biff_unions\\${h_file}";
system "move $cpp_file D:\\Projects\\AVSWorksheetConverter\\source\\XLS_logic\\Biff_unions\\${cpp_file}";
system "svn add D:\\Projects\\AVSWorksheetConverter\\source\\XLS_logic\\Biff_unions\\${tag_name}.*";


}










