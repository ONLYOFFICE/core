

#include "office_elements.h"
#include "office_elements_create.h"

#include "templates.h"

namespace cpdoccore { 
namespace odf {

const wchar_t * table_body::ns = L"table";
const wchar_t * table_body::name = L"body";

void table_body::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	const std::wstring empty = L"";
	CP_APPLY_ATTR(L"table:style-name",	table_style_name_, empty);
}

const wchar_t * table_table_template::ns = L"table";
const wchar_t * table_table_template::name = L"table-template";

void table_table_template::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:style-name",	text_style_name_);
}

void table_table_template::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{   
    if CP_CHECK_NAME(L"table", L"body")
    {
        CP_CREATE_ELEMENT(table_body_);    
    }
    else
    {
    }
}
}
}