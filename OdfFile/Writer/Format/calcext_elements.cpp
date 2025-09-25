/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "calcext_elements.h"

#include <xml/xmlchar.h>

#include <xml/attributes.h>
#include <xml/utils.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

void calcext_data_bar_attr::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT(L"calcext:axis-color", axis_color_);
	CP_XML_ATTR_OPT(L"calcext:positive-color", positive_color_);
	CP_XML_ATTR_OPT(L"calcext:negative-color", negative_color_);
	CP_XML_ATTR_OPT(L"calcext:axis-position", axis_position_);
	CP_XML_ATTR_OPT(L"calcext:gradient", gradient_);
	CP_XML_ATTR_OPT(L"calcext:min-length", min_length_);
	CP_XML_ATTR_OPT(L"calcext:max-length", max_length_);
}

void calcext_icon_set_attr::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT(L"calcext:icon-set-type",	calcext_icon_set_type_); 

}
void calcext_condition_attr::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT_ENCODE_STRING(L"calcext:base-cell-address",	calcext_base_cell_address_);
	CP_XML_ATTR_OPT(L"calcext:apply-style-name", calcext_apply_style_name_);
	CP_XML_ATTR_OPT_ENCODE_STRING(L"calcext:value", calcext_value_);
	CP_XML_ATTR_OPT(L"loext:stdDev", loext_stdDev_);
}
void calcext_date_is_attr::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT(L"calcext:style",	calcext_style_);
	CP_XML_ATTR_OPT(L"calcext:date",	calcext_date_);
}
void calcext_sparkline_group_attr::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT(L"calcext:id", id_);
	CP_XML_ATTR_OPT(L"calcext:type", type_);
	CP_XML_ATTR_OPT(L"calcext:line-width", line_width_);
	CP_XML_ATTR_OPT(L"calcext:first", first_);
	CP_XML_ATTR_OPT(L"calcext:last", last_);
	CP_XML_ATTR_OPT(L"calcext:high", high_);
	CP_XML_ATTR_OPT(L"calcext:low", low_);
	CP_XML_ATTR_OPT(L"calcext:markers", markers_);
	CP_XML_ATTR_OPT(L"calcext:display-hidden", display_hidden_);
	CP_XML_ATTR_OPT(L"calcext:right-to-left", right_to_left_);
	CP_XML_ATTR_OPT(L"calcext:display-x-axis", display_x_axis_);
	CP_XML_ATTR_OPT(L"calcext:display-empty-cells-as", display_empty_cells_as_);
	CP_XML_ATTR_OPT(L"calcext:date-axis", date_axis_);
	CP_XML_ATTR_OPT(L"calcext:min-axis-type", min_axis_type_);
	CP_XML_ATTR_OPT(L"calcext:max-axis-type", max_axis_type_);
	CP_XML_ATTR_OPT(L"calcext:manual-min", manual_min_);
	CP_XML_ATTR_OPT(L"calcext:color-series", color_series_);
	CP_XML_ATTR_OPT(L"calcext:color-negative", color_negative_);
	CP_XML_ATTR_OPT(L"calcext:color-axis", color_axis_);
	CP_XML_ATTR_OPT(L"calcext:color-markers", color_markers_);
	CP_XML_ATTR_OPT(L"calcext:color-first", color_first_);
	CP_XML_ATTR_OPT(L"calcext:color-last", color_last_);
	CP_XML_ATTR_OPT(L"calcext:color-high", color_high_);
	CP_XML_ATTR_OPT(L"calcext:color-low", color_low_);
}


// calcext_conditional_formats
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_conditional_formats::ns = L"calcext";
const wchar_t * calcext_conditional_formats::name = L"conditional-formats";

void calcext_conditional_formats::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_conditional_formats::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
void calcext_conditional_formats::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
	
// calcext_conditional_format
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_conditional_format::ns = L"calcext";
const wchar_t * calcext_conditional_format::name = L"conditional-format";

void calcext_conditional_format::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_conditional_format::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
void calcext_conditional_format::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			CP_XML_ATTR_OPT(L"calcext:target-range-address",calcext_target_range_address_);

            for (int i = (int)content_.size() - 1; i >= 0; i--)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
	
// calcext_data_bar
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_data_bar::ns = L"calcext";
const wchar_t * calcext_data_bar::name = L"data-bar";

void calcext_data_bar::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_data_bar::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
void calcext_data_bar::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			attr_.serialize(CP_GET_XML_NODE());

			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
// calcext_color_scale
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_color_scale::ns = L"calcext";
const wchar_t * calcext_color_scale::name = L"color-scale";

void calcext_color_scale::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_color_scale::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
void calcext_color_scale::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			for (size_t i = 0; i < content_.size(); i++)
			{
				if (!content_[i]) continue;
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
// calcext_icon_set
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_icon_set::ns = L"calcext";
const wchar_t * calcext_icon_set::name = L"icon-set";

void calcext_icon_set::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_icon_set::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
void calcext_icon_set::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			attr_.serialize(CP_GET_XML_NODE());

			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
	
// calcext:formatting-entry
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_formatting_entry::ns	= L"calcext";
const wchar_t * calcext_formatting_entry::name	= L"formatting-entry";

void calcext_formatting_entry::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			CP_XML_ATTR_OPT_ENCODE_STRING(L"calcext:value", value_);
			CP_XML_ATTR_OPT(L"calcext:show-value", show_value_);
			CP_XML_ATTR_OPT(L"calcext:type", type_);
		}
	}
}

// calcext_color_scale_entry
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_color_scale_entry::ns	= L"calcext";
const wchar_t * calcext_color_scale_entry::name = L"color-scale-entry";

void calcext_color_scale_entry::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			CP_XML_ATTR_OPT_ENCODE_STRING(L"calcext:value",	calcext_value_);
			CP_XML_ATTR_OPT(L"calcext:type",	calcext_type_);
			CP_XML_ATTR_OPT(L"calcext:color",	calcext_color_);
		}
	}
}
// calcext_condition
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_condition::ns	= L"calcext";
const wchar_t * calcext_condition::name = L"condition";

void calcext_condition::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			attr_.serialize(CP_GET_XML_NODE());
		}
	}
}
// calcext_condition
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_date_is::ns = L"calcext";
const wchar_t * calcext_date_is::name = L"date-is";

void calcext_date_is::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			attr_.serialize(CP_GET_XML_NODE());
		}
	}
}
//---------------------------------------------------------------------------------------------------------
const wchar_t* calcext_sparkline_groups::ns = L"calcext";
const wchar_t* calcext_sparkline_groups::name = L"sparkline-groups";

void calcext_sparkline_groups::create_child_element(const std::wstring& Ns, const std::wstring& Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_sparkline_groups::add_child_element(const office_element_ptr& child_element)
{
	content_.push_back(child_element);
}
void calcext_sparkline_groups::serialize(std::wostream& _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------
const wchar_t* calcext_sparkline_group::ns = L"calcext";
const wchar_t* calcext_sparkline_group::name = L"sparkline-group";

void calcext_sparkline_group::create_child_element(const std::wstring& Ns, const std::wstring& Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_sparkline_group::add_child_element(const office_element_ptr& child_element)
{
	content_.push_back(child_element);
}
void calcext_sparkline_group::serialize(std::wostream& _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			attr_.serialize(CP_GET_XML_NODE());
			
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------
const wchar_t* calcext_sparklines::ns = L"calcext";
const wchar_t* calcext_sparklines::name = L"sparklines";

void calcext_sparklines::create_child_element(const std::wstring& Ns, const std::wstring& Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_sparklines::add_child_element(const office_element_ptr& child_element)
{
	content_.push_back(child_element);
}
void calcext_sparklines::serialize(std::wostream& _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------
const wchar_t* calcext_sparkline::ns = L"calcext";
const wchar_t* calcext_sparkline::name = L"sparkline";

void calcext_sparkline::serialize(std::wostream& _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_ATTR_OPT(L"calcext:data-range", data_range_);
			CP_XML_ATTR_OPT(L"calcext:cell-address", cell_address_);

		}
	}
}
}
}
