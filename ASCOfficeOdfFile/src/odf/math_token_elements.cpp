
#include "math_token_elements.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/simple_xml_writer.h>
#include <cpdoccore/xml/utils.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

//---------------------------------------------------------------
const wchar_t * math_mi::ns = L"math";
const wchar_t * math_mi::name = L"mi";
//----------------------------------------------------------------------------------------------------
void math_mi::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mi::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mi::add_text(const std::wstring & Text) 
{
    text_ = Text;
}

void math_mi::docx_convert(oox::docx_conversion_context & Context) 
{
	if (!text_) return;

	std::wostream & strm = Context.output_stream();
   
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"m:r")
		{
			CP_XML_NODE(L"w:rPr")
			{
				CP_XML_NODE(L"w:rFonts")
				{
					CP_XML_ATTR(L"w:hAnsi", L"Cambria Math");
					CP_XML_ATTR(L"w:ascii", L"Cambria Math");
				}
			}
			CP_XML_NODE(L"m:t")
			{
				//CP_XML_ATTR(L"xml:space", L"preserve");
				CP_XML_STREAM() << xml::utils::replace_text_to_xml( *text_ );
			}
		}
	}
}
//----------------------------------------------------------------------------------------------------
const wchar_t * math_mo::ns = L"math";
const wchar_t * math_mo::name = L"mo";
//----------------------------------------------------------------------------------------------------
void math_mo::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mo::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mo::add_text(const std::wstring & Text) 
{
    text_ = Text;
}


void math_mo::docx_convert(oox::docx_conversion_context & Context) 
{
	if (!text_) return;

    std::wostream & strm = Context.output_stream();
   
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"m:r")
        {
			CP_XML_NODE(L"w:rPr")
			{
				CP_XML_NODE(L"w:rFonts")
				{
					CP_XML_ATTR(L"w:hAnsi", L"Cambria Math");
					CP_XML_ATTR(L"w:ascii", L"Cambria Math");
				}
			}	
			CP_XML_NODE(L"m:t")
			{
				//CP_XML_ATTR(L"xml:space", L"preserve");
				CP_XML_STREAM() << xml::utils::replace_text_to_xml( *text_ );
			}
		}
	}
}
//----------------------------------------------------------------------------------------------------
const wchar_t * math_mn::ns = L"math";
const wchar_t * math_mn::name = L"mn";
//----------------------------------------------------------------------------------------------------
void math_mn::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mn::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mn::add_text(const std::wstring & Text) 
{
    text_ = Text;
}


void math_mn::docx_convert(oox::docx_conversion_context & Context) 
{
	if (!text_) return;

	std::wostream & strm = Context.output_stream();
   
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"m:r")
		{
			CP_XML_NODE(L"w:rPr")
			{
				CP_XML_NODE(L"w:rFonts")
				{
					CP_XML_ATTR(L"w:hAnsi", L"Cambria Math");
					CP_XML_ATTR(L"w:ascii", L"Cambria Math");
				}
			}
			CP_XML_NODE(L"m:t")
			{
				//CP_XML_ATTR(L"xml:space", L"preserve");
				CP_XML_STREAM() << xml::utils::replace_text_to_xml( *text_ );
			}
		}
	}
}
//----------------------------------------------------------------------------------------------------
const wchar_t * math_ms::ns = L"math";
const wchar_t * math_ms::name = L"ms";
//----------------------------------------------------------------------------------------------------
void math_ms::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_ms::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_ms::add_text(const std::wstring & Text) 
{
    text_ = Text;
}


void math_ms::docx_convert(oox::docx_conversion_context & Context) 
{

}
//----------------------------------------------------------------------------------------------------
const wchar_t * math_mspace::ns = L"math";
const wchar_t * math_mspace::name = L"mspace";
//----------------------------------------------------------------------------------------------------
void math_mspace::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mspace::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mspace::add_text(const std::wstring & Text) 
{
    text_ = Text;
}


void math_mspace::docx_convert(oox::docx_conversion_context & Context) 
{

}
//----------------------------------------------------------------------------------------------------
const wchar_t * math_mtext::ns = L"math";
const wchar_t * math_mtext::name = L"mtext";
//----------------------------------------------------------------------------------------------------
void math_mtext::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mtext::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mtext::add_text(const std::wstring & Text) 
{
    text_ = Text;
}


void math_mtext::docx_convert(oox::docx_conversion_context & Context) 
{

}
//----------------------------------------------------------------------------------------------------
const wchar_t * math_mglyph::ns = L"math";
const wchar_t * math_mglyph::name = L"mglyph";
//----------------------------------------------------------------------------------------------------
void math_mglyph::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mglyph::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mglyph::add_text(const std::wstring & Text) 
{
    text_ = Text;
}


void math_mglyph::docx_convert(oox::docx_conversion_context & Context) 
{

}
}
}
