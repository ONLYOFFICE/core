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

#include "office_spreadsheet.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"

namespace cpdoccore { 
namespace odf_reader {

// office:spreadsheet
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_spreadsheet::ns = L"office";
const wchar_t * office_spreadsheet::name = L"spreadsheet";

void office_spreadsheet::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if CP_CHECK_NAME(L"table", L"database-ranges")
    {
        CP_CREATE_ELEMENT(table_database_ranges_);    
    }
	else
		CP_CREATE_ELEMENT(content_);
}

void office_spreadsheet::add_text(const std::wstring & Text)
{
    // TODO : error
}

void office_spreadsheet::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void office_spreadsheet::docx_convert(oox::docx_conversion_context & Context)
{
    Context.start_office_text();
    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->docx_convert(Context);
    }
    Context.end_office_text();
}

void office_spreadsheet::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.start_office_spreadsheet(this);
    _CP_LOG << L"[info][xlsx] process spreadsheet (" << content_.size() << L" elmements)" << std::endl;
   
	BOOST_FOREACH(const office_element_ptr & elm, table_database_ranges_)
    {
        elm->xlsx_convert(Context);
    }
	BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->xlsx_convert(Context);
    }
    Context.end_office_spreadsheet();
}

}
}
