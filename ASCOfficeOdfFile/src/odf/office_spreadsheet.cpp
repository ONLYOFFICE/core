
#include "office_spreadsheet.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"

namespace cpdoccore { 
namespace odf {

// office:spreadsheet
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_spreadsheet::ns = L"office";
const wchar_t * office_spreadsheet::name = L"spreadsheet";

void office_spreadsheet::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
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
    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->xlsx_convert(Context);
    }
    Context.end_office_spreadsheet();
}

}
}
