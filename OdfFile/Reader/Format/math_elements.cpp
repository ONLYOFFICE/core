/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "math_elements.h"
#include "../Converter/StarMath2OOXML/cconversionsmtoooxml.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

//---------------------------------------------------------------
const wchar_t * office_math_element::ns = L"math";
const wchar_t * office_math_element::name = L"math-element";
//---------------------------------------------------------------
const wchar_t * office_math::ns = L"math";
const wchar_t * office_math::name = L"math";

//----------------------------------------------------------------------------------------------------

void office_math::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void office_math::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME1(L"semantics")
    {
        CP_CREATE_ELEMENT(semantics_);
    }
}


void office_math::oox_convert(oox::math_context & Context, int iTypeConversion)
{
	if (semantics_)
	{
		office_math_element* math_element = dynamic_cast<office_math_element*>(semantics_.get());
		math_element->oox_convert(Context,iTypeConversion);
	}
}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_semantics::ns = L"math";
const wchar_t * math_semantics::name = L"semantics";
//----------------------------------------------------------------------------------------------------

void math_semantics::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void math_semantics::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME1(L"annotation")
    {
        CP_CREATE_ELEMENT(annotation_);
    }
	else
        CP_CREATE_ELEMENT(content_);

}


void math_semantics::oox_convert(oox::math_context & Context)
{
    this->oox_convert(Context,0);
}
void math_semantics::oox_convert(oox::math_context &Context, int iTypeConversion)
{
    math_annotation* annotation = dynamic_cast<math_annotation*>(annotation_.get());
    math_annotation_xml* annotation_xml = dynamic_cast<math_annotation_xml*>(annotation_.get());
   
    std::wstring annotation_text;
    if ((annotation) && (annotation->text_)) annotation_text = *annotation->text_;
    else if ((annotation_xml) && (annotation_xml->text_)) annotation_text = *annotation_xml->text_;

    bool result = false;
    if (!annotation_text.empty())
    {
        result = true;
        StarMath::CParserStarMathString parser;
        StarMath::CConversionSMtoOOXML converter;
       
        // базовые свойства шрифта для математики
        /*parser.set*/ /*?*/ /*converter.set*/ Context.base_font_name_;
        /*parser.set*/ /*?*/ /*converter.set*/ Context.base_font_size_;
        /*parser.set*/ /*?*/ /*converter.set*/ Context.base_alignment_;
        /*parser.set*/ /*?*/ /*converter.set*/ Context.base_font_italic_;
        /*parser.set*/ /*?*/ /*converter.set*/ Context.base_font_bold_;

        parser.SetBaseFont(Context.base_font_name_);
        parser.SetBaseSize(Context.base_font_size_);
        parser.SetBaseAlignment(Context.base_alignment_);
        parser.SetBaseItalic(Context.base_font_italic_);
        parser.SetBaseBold(Context.base_font_bold_);

        /*result = */converter.StartConversion(parser.Parse(annotation_text,iTypeConversion),parser.GetAlignment());


        Context.output_stream() << converter.GetOOXML();
    }

    if (!result)
    {
        for (size_t i = 0; i < content_.size(); i++)
        {
            office_math_element* math_element = dynamic_cast<office_math_element*>(content_[i].get());
            math_element->oox_convert(Context);
        }
    }
}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_annotation::ns = L"math";
const wchar_t * math_annotation::name = L"annotation";
//----------------------------------------------------------------------------------------------------

void math_annotation::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
// ver 2	
   CP_APPLY_ATTR(L"math:encoding", encoding_);

// ver 3
    if (!encoding_)	CP_APPLY_ATTR(L"encoding", encoding_);
	
}

void math_annotation::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);

}

void math_annotation::add_text(const std::wstring & Text) 
{
    text_ = Text;
}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_annotation_xml::ns = L"math";
const wchar_t * math_annotation_xml::name = L"annotation-xml";
//----------------------------------------------------------------------------------------------------

void math_annotation_xml::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
// ver 2	
   CP_APPLY_ATTR(L"math:encoding", encoding_);

// ver 3
    if (!encoding_)	CP_APPLY_ATTR(L"encoding", encoding_);
	
}

void math_annotation_xml::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);

}

void math_annotation_xml::add_text(const std::wstring & Text) 
{
    text_ = Text;
}

//----------------------------------------------------------------------------------------------------

}
}
