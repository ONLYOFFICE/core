#include "precompiled_cpodf.h"
#include "oox_conversion_context.h"

#include <boost/foreach.hpp>
#include <iostream>
#include <cpdoccore/xml/utils.h>
#include <cpdoccore/odf/odf_document.h>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "../odf/odfcontext.h"

namespace cpdoccore { 

void styles_context::start_process_style(const odf::style_instance * Instance)
{
   current_processed_style_ = Instance;
}

void styles_context::end_process_style()
{
    current_processed_style_ = NULL;
}

void styles_context::start()
{
    text_style_.str( std::wstring() );
    text_style_.clear();

    paragraph_nodes_.str( std::wstring() );
    paragraph_nodes_.clear();

    paragraph_attr_.str( std::wstring() );
    paragraph_attr_.clear();

	table_style_.str( std::wstring() );
    table_style_.clear();

    list_style_.str( std::wstring() );
    list_style_.clear();

	extern_node_ = L"a:rPr";

}

std::wstringstream & styles_context::text_style()
{
    return text_style_;
}

std::wstringstream & styles_context::paragraph_nodes()
{
    return paragraph_nodes_;
}

std::wstringstream & styles_context::paragraph_attr()
{
    return paragraph_attr_;
}

std::wstringstream & styles_context::table_style()
{
    return table_style_;
}

std::wstringstream & styles_context::list_style()
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