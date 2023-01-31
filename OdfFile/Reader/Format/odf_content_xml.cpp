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

#include "odf_content_xml.h"
#include "office_elements_create.h"

namespace cpdoccore { 
namespace odf_reader {

void content_xml_t::docx_convert(oox::docx_conversion_context & Context) 
{
	if (content_)
	{
	    return content_->docx_convert(Context);
	}
}

void content_xml_t::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	if (content_)
	{
	  return content_->xlsx_convert(Context);
	}
}
void content_xml_t::pptx_convert(oox::pptx_conversion_context & Context)
{
	if (content_)
	{
	  return content_->pptx_convert(Context);
	}
}

void content_xml_t::add_attributes( const xml::attributes_wc_ptr & Attributes ) 
{}

void content_xml_t::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
 	if CP_CHECK_NAME(L"office", L"document")
    {
        create_element_and_read(Reader, Ns, Name, content_, &context_, true);
    }
	else if CP_CHECK_NAME(L"office", L"document-content")
    {
        create_element_and_read(Reader, Ns, Name, content_, &context_, true);
    }
    else if CP_CHECK_NAME(L"office", L"document-styles")
    {
        create_element_and_read(Reader, Ns, Name, content_, &context_, true);
    }
    else if CP_CHECK_NAME(L"office", L"document-settings")
    {
        create_element_and_read(Reader, Ns, Name, content_, &context_, true);
    }
	else if CP_CHECK_NAME(L"office", L"document-meta")
    {
        create_element_and_read(Reader, Ns, Name, content_, &context_, true);
    }
	else if CP_CHECK_NAME(L"manifest", L"manifest")
    {
        create_element_and_read(Reader, Ns, Name, content_, &context_, true);
    }
	else if ( CP_CHECK_NAME(L"math", L"math") || CP_CHECK_NAME(L"", L"math"))
    {
        create_element_and_read(Reader, Ns, Name, content_, &context_, true);
    }	

}
  

}
}
