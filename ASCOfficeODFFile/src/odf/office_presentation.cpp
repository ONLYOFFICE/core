#include "precompiled_cpodf.h"

#include "office_presentation.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"

namespace cpdoccore { 
namespace odf {

// office:presentation
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_presentation::ns = L"office";
const wchar_t * office_presentation::name = L"presentation";

void office_presentation::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	if CP_CHECK_NAME(L"draw", L"page") 
		CP_CREATE_ELEMENT(pages_);
	else if CP_CHECK_NAME(L"presentation", L"footer-decl") 
		CP_CREATE_ELEMENT(footer_decl_);
	else if CP_CHECK_NAME(L"presentation", L"date-time-decl") 
		CP_CREATE_ELEMENT(date_time_decl_);

}

void office_presentation::add_text(const std::wstring & Text)
{
    // TODO : error
}

void office_presentation::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void office_presentation::docx_convert(oox::docx_conversion_context & Context)
{
    Context.start_office_text();
	_CP_LOG(info) << L"[info][docx] process pages (" << pages_.size() << L" elmements)" << std::endl;
	BOOST_FOREACH(const office_element_ptr & elm, pages_)
    {
        elm->docx_convert(Context);
    }
    Context.end_office_text();
}

void office_presentation::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.start_office_spreadsheet(this);
    _CP_LOG(info) << L"[info][xlsx] process pages (" << pages_.size() << L" elmements)" << std::endl;
    BOOST_FOREACH(const office_element_ptr & elm, pages_)
    {
        elm->xlsx_convert(Context);
    }
    Context.end_office_spreadsheet();
}

void office_presentation::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.start_office_presentation();

    _CP_LOG(info) << L"[info][pptx] process pages(" << pages_.size() << L" elmements)" << std::endl;
    BOOST_FOREACH(const office_element_ptr & elm, pages_)
    {
        elm->pptx_convert(Context);
    }
    Context.end_office_presentation();
}

}
}
