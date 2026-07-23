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
#include "abstract_xml.h"
#include "paragraph_elements.h"


namespace cpdoccore { 
namespace odf_reader {


// abstract-xml
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * abstract_xml::ns = L"";
const wchar_t * abstract_xml::name = L"abstract-xml";

std::wostream & abstract_xml::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
	for (size_t i = 0; i < xml_content_.size(); i++)
    {
        xml_content_[i]->text_to_stream(_Wostream, bXmlEncode);
    }
    return _Wostream;
}

std::wostream & abstract_xml::xml_to_stream(std::wostream & _Wostream) const
{
	for (size_t i = 0; i < xml_content_.size(); i++)
    {
		xml_content_[i]->xml_to_stream(_Wostream);
    }
    return _Wostream;
}

void abstract_xml::add_attributes( const xml::attributes_wc_ptr & )
{
}

void abstract_xml::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(xml_content_);
}

void abstract_xml::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::text::create(Text) ;
    xml_content_.push_back( elm );
}
void abstract_xml::add_space(const std::wstring & Text)
{
    office_element_ptr elm = text::text::create(Text) ;
    xml_content_.push_back( elm );
}
}
}
