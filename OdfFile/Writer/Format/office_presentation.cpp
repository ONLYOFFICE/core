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

#include "office_presentation.h"
#include "draw_page.h"

#include <xml/xmlchar.h>
#include <xml/attributes.h>

#include "office_elements_create.h"

#include <xml/simple_xml_writer.h>

namespace cpdoccore { 
namespace odf_writer {

// office:presentation
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_presentation::ns = L"office";
const wchar_t * office_presentation::name = L"presentation";


void office_presentation::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	if	CP_CHECK_NAME(L"draw", L"page") 
	{
		CP_CREATE_ELEMENT(pages_);
	}
	else if CP_CHECK_NAME(L"presentation", L"footer-decl") 
	{
		CP_CREATE_ELEMENT(footer_decls_);
	}
	else if CP_CHECK_NAME(L"presentation", L"date-time-decl") 
	{
		CP_CREATE_ELEMENT(date_time_decls_);
	}
}

void office_presentation::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typePresentationDateTimeDecl)
    {
		date_time_decls_.push_back(child_element);
    } 
    else if ( type_ == typePresentationFooterDecl)
    {
        footer_decls_.push_back(child_element);
    }   
	else if ( type_ == typeDrawPage)
    {
        pages_.push_back(child_element);
    }

}
void office_presentation::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			for (size_t i = 0; i < pages_.size(); i++)
			{
				pages_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
}
}
