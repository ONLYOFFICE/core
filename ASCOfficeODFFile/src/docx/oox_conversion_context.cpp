#include "precompiled_cpodf.h"
#include "oox_conversion_context.h"

#include <boost/foreach.hpp>
#include <iostream>
#include <cpdoccore/xml/utils.h>
#include <cpdoccore/odf/odf_document.h>
#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore { 
namespace oox {

void styles_context::start()
{
    text_style_.str( std::wstring() );
    text_style_.clear();

    paragraph_style_.str( std::wstring() );
    paragraph_style_.clear();

    table_style_.str( std::wstring() );
    table_style_.clear();

    list_style_.str( std::wstring() );
    list_style_.clear();
}

std::wostream & styles_context::text_style()
{
    return text_style_;
}

std::wstringstream & styles_context::paragraph_style()
{
    return paragraph_style_;
}

std::wostream & styles_context::table_style()
{
    return table_style_;
}

std::wostream & styles_context::list_style()
{
    return list_style_;
}

void styles_context::docx_serialize_text_style(std::wostream & strm)
{
    if (!text_style_.str().empty())
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"w:rPr")
			{
				CP_XML_STREAM() << text_style_.str();
			}
		}
	}
}


void styles_context::docx_serialize_table_style(std::wostream & strm)
{
    if (!table_style_.str().empty())
    {
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"w:tblPr")
			{
				CP_XML_STREAM() << table_style_.str();
			}
		}
    }
}
}
}