
#include "table_database_ranges.h"

#include <boost/make_shared.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore { 
namespace odf {

using xml::xml_char_wc;

// table:table-source
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_database_ranges::ns = L"table";
const wchar_t * table_database_ranges::name = L"database-ranges";

void table_database_ranges::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"table", L"database-range")
    {
        CP_CREATE_ELEMENT(database_ranges_);
    }
    else
    {
    }
}

void table_database_ranges::add_child_element(office_element_ptr & child_element)
{
 	ElementType type = child_element->get_type();

    if (type == typeTableDatabaseRange)
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
			BOOST_FOREACH(const office_element_ptr & elm, database_ranges_)
			{
				elm->serialize(CP_XML_STREAM());
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
			CP_XML_ATTR_OPT(L"table:name", table_name_);
			CP_XML_ATTR_OPT(L"table:target-range-address", table_target_range_address_);
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
//			CP_XML_ATTR_OPT(L"table:name", table_name_);
//			CP_XML_ATTR_OPT(L"table:expression", table_expression_);
//			CP_XML_ATTR_OPT(L"table:base-cell-address", table_base_cell_address_);
//		}
//	}
//}

}
}
