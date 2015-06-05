

#include "office_presentation.h"
#include "draw_page.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>

#include <cpdoccore/xml/attributes.h>

#include <cpdoccore/odf/odf_document.h>
#include "odfcontext.h"

#include "serialize_elements.h"

namespace cpdoccore { 
namespace odf {

// office:presentation
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_presentation::ns = L"office";
const wchar_t * office_presentation::name = L"presentation";

void office_presentation::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	if	CP_CHECK_NAME(L"draw", L"page") 
		CP_CREATE_ELEMENT(pages_);
	else if CP_CHECK_NAME(L"presentation", L"footer-decl") 
		CP_CREATE_ELEMENT(footer_decls_);
	else if CP_CHECK_NAME(L"presentation", L"date-time-decl") 
		CP_CREATE_ELEMENT(date_time_decls_);

}

void office_presentation::add_text(const std::wstring & Text)
{
}

void office_presentation::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void office_presentation::docx_convert(oox::docx_conversion_context & Context)
{
    Context.start_office_text();
	_CP_LOG << L"[info][docx] process pages (" << pages_.size() << L" elmements)" << std::endl;
	BOOST_FOREACH(const office_element_ptr & elm, pages_)
    {
        elm->docx_convert(Context);
    }
    Context.end_office_text();
}

void office_presentation::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.start_office_spreadsheet(this);
    _CP_LOG << L"[info][xlsx] process pages (" << pages_.size() << L" elmements)" << std::endl;
    BOOST_FOREACH(const office_element_ptr & elm, pages_)
    {
        elm->xlsx_convert(Context);
    }
    Context.end_office_spreadsheet();
}

void office_presentation::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.start_office_presentation();

    _CP_LOG << L"[info][pptx] process pages(" << pages_.size() << L" elmements)" << std::endl;
	
    BOOST_FOREACH(const office_element_ptr & elm, footer_decls_)
    {
		presentation_footer_decl * style = dynamic_cast<presentation_footer_decl *>(elm.get());

        if (!style)
            continue;

		std::wstring style_name_ = L"footer:" + style->presentation_name_.get_value_or(L"");
		Context.root()->odf_context().drawStyles().add(style_name_, elm);
    }
    BOOST_FOREACH(const office_element_ptr & elm, date_time_decls_)
    {
		presentation_date_time_decl * style = dynamic_cast<presentation_date_time_decl *>(elm.get());

        if (!style)
            continue;

		std::wstring style_name_ = L"datetime:" + style->presentation_name_.get_value_or(L"");
		Context.root()->odf_context().drawStyles().add(style_name_, elm);
    }
    BOOST_FOREACH(const office_element_ptr & elm, pages_)
    {
        elm->pptx_convert(Context);
    }
    Context.end_office_presentation();
}

}
}
