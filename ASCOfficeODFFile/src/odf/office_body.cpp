#include "precompiled_cpodf.h"
#include "office_body.h"

#include "serialize_elements.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>

#include <cpdoccore/odf/odf_document.h>
#include "odfcontext.h"

namespace cpdoccore { 
namespace odf {

using xml::xml_char_wc;

// office:body
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_body::ns = L"office";
const wchar_t * office_body::name = L"body";

::std::wostream & office_body::text_to_stream(::std::wostream & _Wostream) const
{
    if (content_)
        content_->text_to_stream(_Wostream);
    return _Wostream;
}

office_body::office_body()
{}

void office_body::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    
}

void office_body::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void office_body::add_text(const std::wstring & Text)
{
    // TODO : error
}

void office_body::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.start_body();

    if (content_)
        content_->xlsx_convert(Context);

    Context.end_body();
}

void office_body::docx_convert(oox::docx_conversion_context & Context)
{
    Context.start_body();

    std::vector<style_master_page*> & masterPages = Context.root()->odf_context().pageLayoutContainer().master_pages();
    if (!masterPages.empty())
    {
        Context.set_master_page_name(masterPages[0]->style_master_page_attlist_.style_name_.get_value_or(style_ref(L"Standard")).style_name() );
    }

    Context.set_page_properties(Context.root()->odf_context().pageLayoutContainer().page_layout_first()->name());

    if (content_)
        content_->docx_convert(Context);

	Context.get_headers_footers().set_enable_write(true);
    if (page_layout_instance * lastPageLayout = Context.root()->odf_context().pageLayoutContainer().page_layout_by_name(Context.get_page_properties()))
	{
        lastPageLayout->docx_convert_serialize(Context.output_stream(), Context);
	}

    Context.end_body();    
}
void office_body::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.start_body();

    if (content_)
        content_->pptx_convert(Context);

    Context.end_body();
}
}
}
