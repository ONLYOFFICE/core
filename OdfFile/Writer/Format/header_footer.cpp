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

#include "header_footer.h"
#include <xml/attributes.h>



namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

void style_header_footer_properties_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"svg:height",		svg_height_);
    CP_XML_ATTR_OPT(L"fo:min-height",	fo_min_height_);
   
	common_horizontal_margin_attlist_.serialize(CP_GET_XML_NODE());
    common_vertical_margin_attlist_.serialize(CP_GET_XML_NODE());
    common_margin_attlist_.serialize(CP_GET_XML_NODE());
    common_border_attlist_.serialize(CP_GET_XML_NODE());
    common_border_line_width_attlist_.serialize(CP_GET_XML_NODE());
    common_padding_attlist_.serialize(CP_GET_XML_NODE());
    common_background_color_attlist_.serialize(CP_GET_XML_NODE());
    common_shadow_attlist_.serialize(CP_GET_XML_NODE());
    CP_XML_ATTR_OPT(L"style:dynamic-spacing", style_dynamic_spacing_);
}

/// style-header-footer-properties
const wchar_t * style_header_footer_properties::ns		= L"style";
const wchar_t * style_header_footer_properties::name	= L"header-footer-properties";

void style_header_footer_properties::create_child_element(  const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"background-image" == Name)
    {
        CP_CREATE_ELEMENT(style_background_image_);
    }
    else
    {
        not_applicable_element(L"style-header-footer-properties",  Ns, Name);
    }    
}
void style_header_footer_properties::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {	
			style_header_footer_properties_attlist_.serialize(CP_GET_XML_NODE());
			if (style_background_image_) style_background_image_->serialize(CP_XML_STREAM());
		}
	}
}
//////////////////////////////////////////
void header_footer_content::create_child_element(const std::wstring & Ns, const std::wstring & Name, odf_conversion_context * Context)
{
	if CP_CHECK_NAME(L"text", L"tracked-changes") 
	{
		CP_CREATE_ELEMENT_SIMPLE(tracked_changes_);
	}
    else
		CP_CREATE_ELEMENT_SIMPLE(content_);
}
void header_footer_content::add_child_element( const office_element_ptr & child_element, odf_conversion_context * Context)
{
	if (!child_element) return;

	switch(child_element->get_type())
	{
		case typeTextTrackedChanges:
		{
			tracked_changes_ = child_element;	
		}break;
		case typeTextChangedRegion:
		{
			if (!tracked_changes_)
				create_child_element(L"text", L"tracked-changes", Context);

			tracked_changes_->add_child_element(child_element);
		}break;
		default:
			content_.push_back(child_element);
	}
}
void header_footer_content::serialize(std::wostream & strm)
{
	if (tracked_changes_)
		tracked_changes_->serialize(strm);

	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->serialize(strm);
	}
}
}
}
