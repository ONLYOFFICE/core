/*
 * (c) Copyright Ascensio System SIA 2010-2017
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "oox_conversion_context.h"

#include <iostream>
#include <cpdoccore/xml/utils.h>
#include <cpdoccore/odf/odf_document.h>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "../odf/odfcontext.h"
#include "../odf/style_text_properties.h"

namespace cpdoccore { 

void styles_context::start_process_style(const odf_reader::style_instance * Instance)
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

void styles_context::docx_serialize_text_style(std::wostream & strm, std::wstring parenStyleId, std::wstring & strChange)
{
    if (!text_style_.str().empty())
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"w:rPr")
			{
				if (!parenStyleId.empty())
				{
					CP_XML_STREAM() << L"<w:rStyle w:val=\"" << parenStyleId << L"\" />";
				}
				const std::wstring & test_str = text_style_.str();
				CP_XML_STREAM() << test_str;

				if (!strChange.empty())//rPrChange
				{
					CP_XML_STREAM() << strChange;
					strChange.clear();
				}
			}
		}
	}
}

void styles_context::docx_serialize_table_style(std::wostream & strm, std::wstring & strChange)
{
    if (!table_style_.str().empty())
    {
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"w:tblPr")
			{
				CP_XML_STREAM() << table_style_.str();
				if (!strChange.empty())//tblPrChange
				{
					CP_XML_STREAM() << strChange;
					strChange.clear();
				}
			}
		}
    }
}
namespace oox
{
math_context::math_context(odf_reader::fonts_container & fonts, bool graphic) :
base_font_size_(12), fonts_container_(fonts)
{
	graphRPR_ = graphic;

	if (graphRPR_)	nsRPr_ = L"a:rPr";
	else			nsRPr_ = L"w:rPr";
}
void math_context::start()
{
	text_properties_ = odf_reader::style_text_properties_ptr(new odf_reader::style_text_properties());
	
	text_properties_->content().style_font_name_	= L"Cambria Math";
	text_properties_->content().fo_font_size_		= odf_types::length(base_font_size_, odf_types::length::pt);
}

std::wstring math_context::end()
{
	std::wstring math = math_stream_.str();
	
	math_stream_.str( std::wstring() );
	math_stream_.clear();

	return math;
}

}
}