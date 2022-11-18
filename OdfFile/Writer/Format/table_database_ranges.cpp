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

#include "table_database_ranges.h"

#include <boost/make_shared.hpp>

#include <xml/xmlchar.h>
#include <xml/attributes.h>
#include <xml/simple_xml_writer.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

using xml::xml_char_wc;

// table:table-source
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_database_ranges::ns = L"table";
const wchar_t * table_database_ranges::name = L"database-ranges";

void table_database_ranges::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"table", L"database-range")
    {
        CP_CREATE_ELEMENT(database_ranges_);
    }
    else
    {
    }
}

void table_database_ranges::add_child_element( const office_element_ptr & child_element)
{
 	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTableDatabaseRange)
	{
       database_ranges_.push_back(child_element);
    }
    else
    {
    }
}
void table_database_ranges::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			for (size_t i = 0; i < database_ranges_.size(); i++)
			{
				database_ranges_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
// table:named-range
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_database_range::ns = L"table";
const wchar_t * table_database_range::name = L"database-range";


void table_database_range::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:name", table_name_);
			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:target-range-address", table_target_range_address_);
			CP_XML_ATTR_OPT(L"table:display-filter-buttons", table_display_filter_buttons_);
		}
	}
}
//
//// table:named-expression
////////////////////////////////////////////////////////////////////////////////////////////////////
//const wchar_t * table_database_range::ns = L"table";
//const wchar_t * table_database_range::name = L"named-expression";
//
//void table_database_range::serialize(std::wostream & _Wostream)
//{
//    CP_XML_WRITER(_Wostream)
//    {
//		CP_XML_NODE_SIMPLE()
//        {
//			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:name", table_name_);
//			CP_XML_ATTR_OPT(L"table:expression", table_expression_);
//			CP_XML_ATTR_OPT(L"table:base-cell-address", table_base_cell_address_);
//		}
//	}
//}

}
}
