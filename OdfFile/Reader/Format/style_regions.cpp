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

#include "style_regions.h"
#include "text_elements.h"
#include "paragraph_elements.h"

#include <xml/xmlchar.h>

#include "serialize_elements.h"

namespace cpdoccore { 
namespace odf_reader {

const wchar_t * style_region_impl::ns = L"style";
const wchar_t * style_region_impl::name = L"region";

void style_region_impl::docx_convert(oox::docx_conversion_context & Context) 
{
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
    }
}

void style_region_impl::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->xlsx_convert(Context);
    }
}
void style_region_impl::xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
	{
		text::p* paragr = dynamic_cast<text::p*>(content_[i].get());
		text::h* header = dynamic_cast<text::h*>(content_[i].get());

		if (paragr || header)
		{
			text::paragraph* p = paragr ? &paragr->paragraph_ : &header->paragraph_;

			if (false == p->attrs_.text_style_name_.empty())
			{
			}
			
			for (size_t j = 0; j < p->content_.size(); j++)
			{
				text::paragraph_content_element *element = dynamic_cast<text::paragraph_content_element*>(p->content_[j].get());

				if (element)
				{
					element->xlsx_serialize(_Wostream, Context);
				}
				else
				{
					CP_SERIALIZE_TEXT(p->content_[j], true);
				}
			}
		}
		else
		{
			CP_SERIALIZE_TEXT(content_[i], true);
		}
		if (i < content_.size() - 1)
			_Wostream << L"&#xA;";
	}
}
void style_region_impl::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void style_region_impl::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

// style:region-left
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_region_left::ns = L"style";
const wchar_t * style_region_left::name = L"region-left";

void style_region_left::xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context)
{
	_Wostream << L"&amp;L"; 
	style_region_impl::xlsx_serialize(_Wostream, Context);
}
// style:region-right
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_region_right::ns = L"style";
const wchar_t * style_region_right::name = L"region-right";

void style_region_right::xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context)
{
	_Wostream << L"&amp;R"; 
	style_region_impl::xlsx_serialize(_Wostream, Context);
}
// style:region-center
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_region_center::ns = L"style";
const wchar_t * style_region_center::name = L"region-center";

void style_region_center::xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context)
{
	_Wostream << L"&amp;C"; 
	style_region_impl::xlsx_serialize(_Wostream, Context);
}
}
}
