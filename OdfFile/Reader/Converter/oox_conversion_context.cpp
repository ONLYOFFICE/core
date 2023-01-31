/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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
#include <xml/utils.h>
#include <xml/simple_xml_writer.h>

#include "../Format/odf_document.h"
#include "../Format/odfcontext.h"
#include "../Format/style_text_properties.h"
#include "../Format/style_paragraph_properties.h"

namespace cpdoccore { 
namespace oox
{
void forms_context::start_element (_form_type type)
{
	current_state_.clear();

	current_state_.type	= type;
}
void forms_context::set_id (const std::wstring& id)
{
	current_state_.id = id;
}
void forms_context::set_name (const std::wstring& name)
{
	current_state_.name	= name;
}
void forms_context::set_label (const std::wstring& label)
{
	current_state_.label = label;
}
void forms_context::set_uuid (const std::wstring& uuid)
{
	current_state_.uuid	= uuid;
}
void forms_context::set_value (const std::wstring &value)
{
	current_state_.value = value;
}
void forms_context::set_element(odf_reader::form_element *elm)
{
	current_state_.element = elm;
}
void forms_context::end_element ()
{
	mapElements_.insert( std::make_pair(current_state_.id, current_state_));

	current_state_.clear();
}
forms_context::_state& forms_context::get_state_element (std::wstring id)
{
	std::map<std::wstring, _state>::iterator it = mapElements_.find(id);
	
	if (it != mapElements_.end())
	{
		return it->second;
	}
	else 
		return 	current_state_; //empty
}
//----------------------------------------------------------------------------------------------------------------
void tabs_context::reset()
{
	for (size_t i = 0; i < tabs.size(); i++)
	{
		odf_reader::style_tab_stop *tab_stop = dynamic_cast<odf_reader::style_tab_stop*>(tabs[i].get());
		if (tab_stop)
		{
			double pos = tab_stop->margin_left + tab_stop->style_position_.get_value_unit(odf_types::length::pt);
			
			clear_tabs.insert(std::make_pair((int)pos, tabs[i]));
		}
	}
	tabs.clear();
}
void tabs_context::add(const odf_reader::office_element_ptr & element, double margin_left)
{
	odf_reader::style_tab_stop *tab_stop = dynamic_cast<odf_reader::style_tab_stop*>(element.get());
	if (tab_stop)
	{
		tab_stop->margin_left = margin_left;
		
		double pos = margin_left + tab_stop->style_position_.get_value_unit(odf_types::length::pt);

		std::map<int, odf_reader::office_element_ptr>::iterator pFind = clear_tabs.find((int)pos);

		if (pFind != clear_tabs.end())
		{
			clear_tabs.erase(pFind);
		}
		tabs.push_back(element);
	}
}
void tabs_context::docx_convert(oox::docx_conversion_context & Context)
{
	if (clear_tabs.empty() && tabs.empty()) return;
	
	std::wstringstream & _pPr = Context.get_styles_context().paragraph_nodes();
 
    _pPr << L"<w:tabs>";

		for (std::map<int, odf_reader::office_element_ptr>::iterator it = clear_tabs.begin(); it != clear_tabs.end(); ++it)
		{
			odf_reader::style_tab_stop * tab_stop = dynamic_cast<odf_reader::style_tab_stop*>(it->second.get());
			tab_stop->docx_convert(Context, true);				
		}

		for (size_t i = 0; i < tabs.size(); i++)
		{
			odf_reader::style_tab_stop * tab_stop = dynamic_cast<odf_reader::style_tab_stop*>(tabs[i].get());
			tab_stop->docx_convert(Context, false);
		}
    _pPr << L"</w:tabs>";
}

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
	const std::wstring & text_style_str = text_style_.str();
  
	if (!text_style_str.empty() || !text_style_ext_.empty())
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"w:rPr")
			{
				if (!parenStyleId.empty())
				{
					CP_XML_STREAM() << L"<w:rStyle w:val=\"" << parenStyleId << L"\" />";
				}
				CP_XML_STREAM() << text_style_str;

				if (!strChange.empty())//rPrChange
				{
					CP_XML_STREAM() << strChange;
					strChange.clear();
				}
				if (!text_style_ext_.empty())
				{
					CP_XML_STREAM() << text_style_ext_;
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

math_context::math_context(odf_reader::fonts_container & fonts, bool graphic) :
						base_font_size_(12), fonts_container_(fonts), is_need_e_(false)
{
	graphRPR_ = graphic;

	if (graphRPR_)	nsRPr_ = L"a:rPr";
	else			nsRPr_ = L"w:rPr";
}
void math_context::start()
{
	text_properties_ = odf_reader::style_text_properties_ptr(new odf_reader::style_text_properties());
	
	text_properties_->content_.style_font_name_ = L"Cambria Math";
	text_properties_->content_.fo_font_size_ = odf_types::length(base_font_size_, odf_types::length::pt);
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
