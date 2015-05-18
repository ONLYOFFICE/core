
#include "odf_content_xml.h"
#include "office_elements_create.h"

namespace cpdoccore { 
namespace odf {

void content_xml_t::docx_convert(oox::docx_conversion_context & Context) 
{
	if (xml_content_)
	{
	    return xml_content_->docx_convert(Context);
	}
}

void content_xml_t::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	if (xml_content_)
	{
	  return xml_content_->xlsx_convert(Context);
	}
}
void content_xml_t::pptx_convert(oox::pptx_conversion_context & Context)
{
	if (xml_content_)
	{
	  return xml_content_->pptx_convert(Context);
	}
}

void content_xml_t::add_attributes( const xml::attributes_wc_ptr & Attributes ) 
{}

void content_xml_t::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"office", L"document-content")
    {
        create_element_and_read(Reader, Ns, Name, xml_content_, getContext(), true);
    }
    else if CP_CHECK_NAME(L"office", L"document-styles")
    {
        create_element_and_read(Reader, Ns, Name, xml_content_, getContext(), true);
    }
    else if CP_CHECK_NAME(L"office", L"document-settings")
    {
        create_element_and_read(Reader, Ns, Name, xml_content_, getContext(), true);
    }
	else if CP_CHECK_NAME(L"manifest", L"manifest")
    {
        create_element_and_read(Reader, Ns, Name, xml_content_, getContext(), true);
    }
}

void content_xml_t::add_text(const std::wstring & Text) 
{
}   

document_context * content_xml_t::getContext() 
{ 
    return &context_; 
}

}
}
