
#include "table_named_expressions.h"

#include <boost/make_shared.hpp>
#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"
#include "odfcontext.h"
#include <cpdoccore/odf/odf_document.h>


namespace cpdoccore { 
namespace odf {



// table:table-source
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_named_expressions::ns = L"table";
const wchar_t * table_named_expressions::name = L"named-expressions";

void table_named_expressions::add_attributes(const xml::attributes_wc_ptr & Attributes )
{

}

void table_named_expressions::add_child_element(xml::sax * Reader,
                                                const ::std::wstring & Ns,
                                                const ::std::wstring & Name)
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
        CP_NOT_APPLICABLE_ELM();
    }
}

void table_named_expressions::docx_convert(oox::docx_conversion_context & Context)
{
}

void table_named_expressions::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    BOOST_FOREACH(office_element_ptr const & elm, named_range_)
    {
        elm->xlsx_convert(Context);
    }
}

// table:named-range
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_named_range::ns = L"table";
const wchar_t * table_named_range::name = L"named-range";

void table_named_range::docx_convert(oox::docx_conversion_context & Context)
{

}

void table_named_range::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    if (table_cell_range_address_ && table_name_)
    {
        oox::xlsx_defined_names & ctx = Context.get_xlsx_defined_names();
        ctx.add(table_name_.get(), table_cell_range_address_.get());
    }
}

void table_named_range::add_attributes(xml::attributes_wc_ptr const & Attributes)
{
    CP_APPLY_ATTR(L"table:name", table_name_);
    CP_APPLY_ATTR(L"table:cell-range-address", table_cell_range_address_);
    CP_APPLY_ATTR(L"table:base-cell-address", table_base_cell_address_);
}

void table_named_range::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

// table:named-expression
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_named_expression::ns = L"table";
const wchar_t * table_named_expression::name = L"named-expression";

void table_named_expression::docx_convert(oox::docx_conversion_context & Context)
{

}

void table_named_expression::xlsx_convert(oox::xlsx_conversion_context & Context)
{
}

void table_named_expression::add_attributes(xml::attributes_wc_ptr const & Attributes)
{
    CP_APPLY_ATTR(L"table:name", table_name_);
    CP_APPLY_ATTR(L"table:expression", table_expression_);
    CP_APPLY_ATTR(L"table:base-cell-address", table_base_cell_address_);
}

void table_named_expression::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

}
}
