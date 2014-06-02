#include "precompiled_cpodf.h"
#include "table_named_expressions.h"

#include <boost/make_shared.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore { 
namespace odf {

using xml::xml_char_wc;

// table:table-source
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_named_expressions::ns = L"table";
const wchar_t * table_named_expressions::name = L"named-expressions";

void table_named_expressions::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
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

void table_named_expressions::add_child_element(office_element_ptr & child_element)
{
 	ElementType type = child_element->get_type();

    if (type == typeTableNamedRange)
	{
       named_range_.push_back(child_element);
    }
    else if (type == typeTableNamedExpression)
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
			BOOST_FOREACH(const office_element_ptr & elm, named_expression_)
			{
				elm->serialize(CP_XML_STREAM());
			}
			BOOST_FOREACH(const office_element_ptr & elm, named_range_)
			{
				elm->serialize(CP_XML_STREAM());
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
			CP_XML_ATTR_OPT(L"table:name", table_name_);
			CP_XML_ATTR_OPT(L"table:cell-range-address", table_cell_range_address_);
			CP_XML_ATTR_OPT(L"table:base-cell-address", table_base_cell_address_);
			CP_XML_ATTR_OPT(L"table:range-usable-as", table_range_usable_as_);
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
			CP_XML_ATTR_OPT(L"table:name", table_name_);
			CP_XML_ATTR_OPT(L"table:expression", table_expression_);
			CP_XML_ATTR_OPT(L"table:base-cell-address", table_base_cell_address_);
		}
	}
}

}
}
