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

#include "table_named_expressions.h"

#include <boost/make_shared.hpp>

#include <xml/xmlchar.h>
#include <xml/attributes.h>
#include <xml/simple_xml_writer.h>

namespace cpdoccore { 

namespace odf_writer {

using xml::xml_char_wc;

// table:table-source
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_named_expressions::ns = L"table";
const wchar_t * table_named_expressions::name = L"named-expressions";

void table_named_expressions::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"table", L"named-range")
    {
        CP_CREATE_ELEMENT(named_range_);
    }
    else if CP_CHECK_NAME(L"table", L"named-expression")
    {
        CP_CREATE_ELEMENT(named_expression_);    
    }
    else
    {
    }
}

void table_named_expressions::add_child_element( const office_element_ptr & child_element)
{
 	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTableNamedRange)
	{
       named_range_.push_back(child_element);
    }
    else if (type_ == typeTableNamedExpression)
    {
        named_expression_.push_back(child_element);   
    }
    else
    {
    }
}
void table_named_expressions::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			for (size_t i = 0; i < named_expression_.size(); i++)
			{
				named_expression_[i]->serialize(CP_XML_STREAM());
			}
			for (size_t i = 0; i < named_range_.size(); i++)
			{
				named_range_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
// table:named-range
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_named_range::ns = L"table";
const wchar_t * table_named_range::name = L"named-range";


void table_named_range::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:name", table_name_);
			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:base-cell-address", table_base_cell_address_);
			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:cell-range-address", table_cell_range_address_);
			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:range-usable-as", table_range_usable_as_);
		}
	}
}

// table:named-expression
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_named_expression::ns = L"table";
const wchar_t * table_named_expression::name = L"named-expression";

void table_named_expression::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:name", table_name_);
			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:base-cell-address", table_base_cell_address_);
			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:expression", table_expression_);
		}
	}
}

}
}
