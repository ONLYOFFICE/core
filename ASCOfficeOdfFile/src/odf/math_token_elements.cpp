
#include "math_token_elements.h"
#include "style_text_properties.h"

#include <boost/foreach.hpp>

#include <cpdoccore/odf/odf_document.h>

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
	common_attlist_.add_attributes(Attributes);
}

void math_mi::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mi::add_text(const std::wstring & Text) 
{
    text_ = Text;
}

std::wostream & math_mi::text_to_stream(::std::wostream & _strm) const
{
	if (text_) 
		_strm << *text_;
    return _strm;
}

void math_mi::docx_convert(oox::docx_conversion_context & Context) 
{
	if (!text_) return;


	CP_XML_WRITER(Context.output_stream())
    {
		CP_XML_NODE(L"m:r")
		{
		
			Context.get_styles_context().start();
			Context.current_text_properties()->docx_convert(Context);
		
			if (common_attlist_.mathvariant_)
			{
				std::wstring m_sty_val;
				if (common_attlist_.mathvariant_->style_.bold)	m_sty_val += L"b";
				if (common_attlist_.mathvariant_->style_.italic)m_sty_val += L"i";

				CP_XML_NODE(L"m:rPr")
				{
					if (!m_sty_val.empty())
					{
						CP_XML_NODE(L"m:sty")
						{
							CP_XML_ATTR(L"m:val", m_sty_val);
						}
					}
					if (common_attlist_.mathvariant_->style_.type > 0)
					{
						CP_XML_NODE(L"m:scr")
						{
							switch(common_attlist_.mathvariant_->style_.type)
							{
								case 1: CP_XML_ATTR(L"m:val", L"double-struck");	break;
								case 2: CP_XML_ATTR(L"m:val", L"fraktur");			break;
								case 3: CP_XML_ATTR(L"m:val", L"script");			break;
								case 4: CP_XML_ATTR(L"m:val", L"sans-serif");		break;
								case 5: CP_XML_ATTR(L"m:val", L"monospace");		break;
							}
						}
					}
				}
			}

			CP_XML_NODE(L"w:rPr")
			{
				CP_XML_STREAM() << Context.get_styles_context().text_style().str();
			
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
	common_attlist_.add_attributes(Attributes);
	CP_APPLY_ATTR(L"stretchy",		stretchy_);
	CP_APPLY_ATTR(L"fence",			fence_);
}

void math_mo::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mo::add_text(const std::wstring & Text) 
{
    text_ = Text;
}

std::wostream & math_mo::text_to_stream(::std::wostream & _strm) const
{
	if (text_) 
		_strm << *text_;
    return _strm;
}

void math_mo::docx_convert(oox::docx_conversion_context & Context) 
{
	if (!text_) return;
   
	CP_XML_WRITER(Context.output_stream())
    {
		CP_XML_NODE(L"m:r")
        {
			// + доп стили текста ... todoooo
			Context.get_styles_context().start();
			Context.current_text_properties()->docx_convert(Context);

			CP_XML_NODE(L"w:rPr")
			{
				CP_XML_STREAM() << Context.get_styles_context().text_style().str();
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
	common_attlist_.add_attributes(Attributes);

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

	CP_XML_WRITER(Context.output_stream())
    {
		CP_XML_NODE(L"m:r")
		{
			// + доп стили текста ... todoooo
			Context.get_styles_context().start();
			Context.current_text_properties()->docx_convert(Context);

			CP_XML_NODE(L"w:rPr")
			{
				CP_XML_STREAM() << Context.get_styles_context().text_style().str();
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
	common_attlist_.add_attributes(Attributes);

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
	common_attlist_.add_attributes(Attributes);

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
	common_attlist_.add_attributes(Attributes);

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
