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

#include "draw_page.h"

#include <xml/xmlchar.h>
#include <xml/attributes.h>

#include "office_elements_create.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {


void draw_page_attr::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"draw:id", draw_id_);

	CP_XML_ATTR_OPT_ENCODE_STRING(L"draw:name", draw_name_);
	CP_XML_ATTR_OPT_ENCODE_STRING(L"draw:style-name", draw_style_name_);
	CP_XML_ATTR_OPT_ENCODE_STRING(L"draw:master-page-name",	draw_master_page_name_);
    
 	CP_XML_ATTR_OPT(L"style:page-layout-name",	style_page_layout_name_); 
	
	CP_XML_ATTR_OPT(L"presentation:presentation-page-layout-name",	presentation_page_layout_name_);    
	CP_XML_ATTR_OPT(L"presentation:use-date-time-name",				presentation_use_date_time_name_);
    CP_XML_ATTR_OPT(L"presentation:use-footer-name",				presentation_use_footer_name_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_page::ns = L"draw";
const wchar_t * draw_page::name = L"page";

void draw_page::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
	if	CP_CHECK_NAME(L"anim", L"par") 
		CP_CREATE_ELEMENT(animation_);
	else
		CP_CREATE_ELEMENT(content_);
}
void draw_page::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();
	
	if(type_ == typeAnimPar)
		animation_ = child_element;
	else
		content_.push_back(child_element);
}

void draw_page::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			attlist_.serialize(CP_GET_XML_NODE());
			for (int i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}

			if (animation_)
				animation_->serialize(CP_XML_STREAM());
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * presentation_footer_decl::ns = L"presentation";
const wchar_t * presentation_footer_decl::name = L"footer-decl";

//void presentation_footer_decl::add_attributes( const xml::attributes_wc_ptr & Attributes )
//{
//	CP_XML_ATTR_OPT(L"presentation:name",	presentation_name_);
//}

//------------------------------------------------------
const wchar_t * presentation_date_time_decl::ns = L"presentation";
const wchar_t * presentation_date_time_decl::name = L"date-time-decl";
//
//void presentation_date_time_decl::add_attributes( const xml::attributes_wc_ptr & Attributes )
//{
//	CP_XML_ATTR_OPT(L"presentation:name",		presentation_name_);
//	CP_XML_ATTR_OPT(L"presentation:source",	presentation_source_);
//	CP_XML_ATTR_OPT(L"style:data-style-name",	style_data_style_name_);
//}
//

//------------------------------------------------------
const wchar_t * presentation_notes::ns = L"presentation";
const wchar_t * presentation_notes::name = L"notes";

void presentation_notes::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void presentation_notes::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void presentation_notes::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			attlist_.serialize(CP_GET_XML_NODE());
			for (int i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}

}
}
