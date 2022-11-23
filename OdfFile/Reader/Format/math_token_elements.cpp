/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "math_token_elements.h"
#include "style_text_properties.h"

#include <xml/xmlchar.h>
#include <xml/simple_xml_writer.h>
#include <xml/utils.h>

#include "../Format/odf_document.h"

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

void math_mi::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mi::add_text(const std::wstring & Text) 
{
    text_ = Text;
}

std::wostream & math_mi::text_to_stream(std::wostream & _strm, bool bXmlEncode) const
{
	if (text_) 
		_strm << *text_;
    return _strm;
}

void math_mi::oox_convert(oox::math_context & Context)
{
	if (!text_) return;

	CP_XML_WRITER(Context.output_stream())
    {
		CP_XML_NODE(L"m:r")
		{		
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

			Context.text_properties_->content_.oox_serialize(CP_XML_STREAM(), Context.graphRPR_, Context.fonts_container_);

			CP_XML_NODE(L"m:t")
			{
				//CP_XML_ATTR(L"xml:space", L"preserve");
				std::wstring new_text_ = text_.get() + L" ";
				CP_XML_STREAM() << XmlUtils::EncodeXmlString( *text_ );
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

void math_mo::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mo::add_text(const std::wstring & Text) 
{
    text_ = Text;
}

std::wostream & math_mo::text_to_stream(std::wostream & _strm, bool bXmlEncode) const
{
	if (text_) 
		_strm << *text_;
    return _strm;
}

void math_mo::oox_convert(oox::math_context & Context)
{
	if (!text_) return;
   
	CP_XML_WRITER(Context.output_stream())
    {
		CP_XML_NODE(L"m:r")
        {
			// + доп стили текста ... todoooo

			Context.text_properties_->content_.oox_serialize(CP_XML_STREAM(), Context.graphRPR_, Context.fonts_container_);
			
			CP_XML_NODE(L"m:t")
			{
				//CP_XML_ATTR(L"xml:space", L"preserve");
				CP_XML_STREAM() << XmlUtils::EncodeXmlString( *text_ );
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

void math_mn::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mn::add_text(const std::wstring & Text) 
{
    text_ = Text;
}


void math_mn::oox_convert(oox::math_context & Context)
{
	if (!text_) return;

	CP_XML_WRITER(Context.output_stream())
    {
		CP_XML_NODE(L"m:r")
		{
			// + доп стили текста ... todoooo

			Context.text_properties_->content_.oox_serialize(CP_XML_STREAM(), Context.graphRPR_, Context.fonts_container_);
	
			CP_XML_NODE(L"m:t")
			{
				//CP_XML_ATTR(L"xml:space", L"preserve");
				CP_XML_STREAM() << XmlUtils::EncodeXmlString( *text_ );
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

void math_ms::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_ms::add_text(const std::wstring & Text) 
{
    text_ = Text;
}


void math_ms::oox_convert(oox::math_context & Context)
{

}
//----------------------------------------------------------------------------------------------------
const wchar_t * math_mspace::ns = L"math";
const wchar_t * math_mspace::name = L"mspace";
//----------------------------------------------------------------------------------------------------
void math_mspace::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mspace::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mspace::add_text(const std::wstring & Text) 
{
    text_ = Text;
}


void math_mspace::oox_convert(oox::math_context & Context)
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

void math_mtext::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mtext::add_text(const std::wstring & Text) 
{
    text_ = Text;
}


void math_mtext::oox_convert(oox::math_context & Context)
{
	if (text_)
	{
		Context.output_stream() << L"<m:r><m:rPr><m:nor/></m:rPr><m:t>";
			Context.output_stream() << XmlUtils::EncodeXmlString(*text_);
		Context.output_stream() << L"</m:t></m:r>";
	}
}
//----------------------------------------------------------------------------------------------------
const wchar_t * math_mglyph::ns = L"math";
const wchar_t * math_mglyph::name = L"mglyph";
//----------------------------------------------------------------------------------------------------
void math_mglyph::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	common_attlist_.add_attributes(Attributes);

}

void math_mglyph::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mglyph::add_text(const std::wstring & Text) 
{
    text_ = Text;
}


void math_mglyph::oox_convert(oox::math_context & Context)
{

}
}
}
