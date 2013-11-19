#!/usr/bin/perl -w
# diffs two files and writes an HTML output file.
use strict;
use CGI qw(:standard :html3);

# MAIN PROGRAM

# Check for two arguments.
print "usage: $0 tag_name \n" if @ARGV != 1;

my $cpp_file = "$ARGV[0].cpp";
my $h_file = "$ARGV[0].h";
my $tag_name = $ARGV[0];

################################# CPP #####################################
open FCPP, ">$cpp_file";

print FCPP <<CPP_L1;
#include "stdafx.h"
#include "${tag_name}.h"

namespace XLS
{;


${tag_name}::${tag_name}(MSXML2::IXMLDOMDocument2Ptr xml_doc, MSXML2::IXMLDOMElementPtr parent)
:	AttribImportBiffRecord(xml_doc, parent)
{
}


${tag_name}::~${tag_name}()
{
}


void ${tag_name}::readFields(CFRecordPtr record)
{
	//*record >> some_value;
}


} // namespace XLS

CPP_L1

close FCPP;

################################### header #############################################
open FH, ">$h_file";

print FH <<H_L1;
#pragma once

#include "AttribImportBiffRecord.h"

namespace XLS
{;


// Logical representation of ${tag_name} record in BIFF8
class ${tag_name}: public AttribImportBiffRecord
{
//	BIFF_RECORD_DEFINE_TYPE_INFO(${tag_name})
public:
	${tag_name}(MSXML2::IXMLDOMDocument2Ptr xmlDoc, MSXML2::IXMLDOMElementPtr parent);
	~${tag_name}();

	void readFields(CFRecordPtr record);
public:
	BO_ATTRIB_MARKUP_BEGIN
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

H_L1

close FH;


exit(0);










