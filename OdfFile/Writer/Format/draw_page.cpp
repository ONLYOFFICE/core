/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
			office_element_ptr_array deffer_serialization;

			attlist_.serialize(CP_GET_XML_NODE());
			for (int i = 0; i < content_.size(); i++)
			{
				presentation_notes* presentation_notes_ = dynamic_cast<presentation_notes*>(content_[i].get());

				if (presentation_notes_)
				{
					deffer_serialization.push_back(content_[i]);
					continue;
				}
				
				content_[i]->serialize(CP_XML_STREAM());
			}

			if (animation_)
				animation_->serialize(CP_XML_STREAM());

			for (size_t i = 0; i < deffer_serialization.size(); i++)
				deffer_serialization[i]->serialize(CP_XML_STREAM());
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
