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
#pragma once

#include "office_elements.h"
#include "documentcontext.h"

namespace cpdoccore { 
namespace odf_reader {

class content_xml_t : public common::read_doc_element
{
public:
    content_xml_t()
    {}
    office_element * get_content()
    {
        return content_.get();
    }

    const office_element * get_content() const
    {
        return content_.get();
    }
    void docx_convert(oox::docx_conversion_context & Context);
    void xlsx_convert(oox::xlsx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context);

    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_text(const std::wstring & Text) {}
	virtual void add_space(const std::wstring & Text) {}
   
    friend class odf_document;
	
    office_element_ptr	content_;
    document_context	context_;
};

typedef shared_ptr<content_xml_t>::Type content_xml_t_ptr;

}
}
