/*
 * (c) Copyright Ascensio System SIA 2010-2016
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


#include "office_presentation.h"
#include "draw_page.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>

#include <cpdoccore/xml/attributes.h>

#include <cpdoccore/odf/odf_document.h>
#include "odfcontext.h"

#include "serialize_elements.h"

namespace cpdoccore { 
namespace odf_reader {

// office:presentation
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_presentation::ns = L"office";
const wchar_t * office_presentation::name = L"presentation";

void office_presentation::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	if	CP_CHECK_NAME(L"draw", L"page") 
		CP_CREATE_ELEMENT(pages_);
	else if CP_CHECK_NAME(L"presentation", L"footer-decl") 
		CP_CREATE_ELEMENT(footer_decls_);
	else if CP_CHECK_NAME(L"presentation", L"date-time-decl") 
		CP_CREATE_ELEMENT(date_time_decls_);

}

void office_presentation::add_text(const std::wstring & Text)
{
}

void office_presentation::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void office_presentation::docx_convert(oox::docx_conversion_context & Context)
{
    Context.start_office_text();
	_CP_LOG << L"[info][docx] process pages (" << pages_.size() << L" elmements)" << std::endl;
	BOOST_FOREACH(const office_element_ptr & elm, pages_)
    {
        elm->docx_convert(Context);
    }
    Context.end_office_text();
}

void office_presentation::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.start_office_spreadsheet(this);
    _CP_LOG << L"[info][xlsx] process pages (" << pages_.size() << L" elmements)" << std::endl;
    BOOST_FOREACH(const office_element_ptr & elm, pages_)
    {
        elm->xlsx_convert(Context);
    }
    Context.end_office_spreadsheet();
}

void office_presentation::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.start_office_presentation();

    _CP_LOG << L"[info][pptx] process pages(" << pages_.size() << L" elmements)" << std::endl;
	
    BOOST_FOREACH(const office_element_ptr & elm, footer_decls_)
    {
		presentation_footer_decl * style = dynamic_cast<presentation_footer_decl *>(elm.get());

        if (!style)
            continue;

		std::wstring style_name_ = L"footer:" + style->presentation_name_.get_value_or(L"");
		Context.root()->odf_context().drawStyles().add(style_name_, elm);
    }
    BOOST_FOREACH(const office_element_ptr & elm, date_time_decls_)
    {
		presentation_date_time_decl * style = dynamic_cast<presentation_date_time_decl *>(elm.get());

        if (!style)
            continue;

		std::wstring style_name_ = L"datetime:" + style->presentation_name_.get_value_or(L"");
		Context.root()->odf_context().drawStyles().add(style_name_, elm);
    }
    BOOST_FOREACH(const office_element_ptr & elm, pages_)
    {
        elm->pptx_convert(Context);
    }
    Context.end_office_presentation();
}

}
}
