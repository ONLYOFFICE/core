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

#include "math_layout_elements.h"
#include "math_token_elements.h"
#include "style_text_properties.h"
#include "math_limit_elements.h"

#include <xml/xmlchar.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

//---------------------------------------------------------------
const wchar_t * math_mrow::ns = L"math";
const wchar_t * math_mrow::name = L"mrow";
//----------------------------------------------------------------------------------------------------
math_mrow::math_mrow()
{
	next_element_to_prev_ = false;
}
void math_mrow::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mrow::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{//0* elements
	if (next_element_to_prev_)
	{
		content_.back()->add_child_element(Reader, Ns, Name);
		next_element_to_prev_ = false;
	}
	else
	{
		CP_CREATE_ELEMENT(content_);
	}
	
	if ( Name == L"munderover" )
		next_element_to_prev_ = true;
}

void math_mrow::oox_convert(oox::math_context & Context)
{
	if (content_.size() < 1) return; 
	
	int i_start = 0, i_end = content_.size();

	math_mo* mo_test_first	= dynamic_cast<math_mo*>(content_[i_start].get());
	math_mo* mo_test_last	= dynamic_cast<math_mo*>(content_[i_end-1].get());

	bool bDPr = false;
	if ((mo_test_first) && (mo_test_first->fence_) && (*mo_test_first->fence_))
	{
		i_start++; 
		bDPr = true;
	}
	
	if ((mo_test_last) && (mo_test_last->fence_) && (*mo_test_last->fence_))
	{
		i_end--;
		bDPr = true;
	}

	bool need_e_old = Context.is_need_e_, need_e = false;
	if (bDPr)
	{
		Context.output_stream() << L"<m:d>";
		
		Context.output_stream() << L"<m:dPr>";
			if ((mo_test_first) && (mo_test_first->fence_) && (*mo_test_first->fence_))
			{
				Context.output_stream() << L"<m:begChr m:val=\"";
					mo_test_first->text_to_stream(Context.output_stream());
				Context.output_stream() << L"\"/>";
			}
			if ((mo_test_last) && (mo_test_last->fence_) && (*mo_test_last->fence_))
			{
				Context.output_stream() << L"<m:endChr m:val=\"";
					mo_test_last->text_to_stream(Context.output_stream());
				Context.output_stream() << L"\"/>";
			}
			Context.output_stream() << Context.math_style_stream().str();
		Context.output_stream() << L"</m:dPr>";
		
		need_e = true;		
	}
	else need_e = Context.is_need_e_;

	Context.is_need_e_ = false;

	if (need_e)
	{
		Context.output_stream() << L"<m:e>";
	}
	for (int i = i_start; i < i_end ; i++)
	{
		office_math_element* math_element = dynamic_cast<office_math_element*>(content_[i].get());
		math_element->oox_convert(Context);		
	}
	if (need_e)
	{
		Context.output_stream() << L"</m:e>";
	}
	if (bDPr)
	{
		Context.output_stream() << L"</m:d>";
	}
	Context.is_need_e_ = need_e_old;
}
//---------------------------------------------------------------
const wchar_t * math_mfrac::ns = L"math";
const wchar_t * math_mfrac::name = L"mfrac";
//----------------------------------------------------------------------------------------------------
void math_mfrac::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mfrac::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mfrac::oox_convert(oox::math_context & Context)
{//2 elements
	if (content_.size() != 2)
	{
		return;
	}
	bool need_e = Context.is_need_e_;
	if (need_e)
	{
		Context.output_stream() << L"<m:e>";
	}
	Context.is_need_e_ = false;

	office_math_element* math_element = NULL;

	Context.output_stream() << L"<m:f>";
		Context.output_stream() << L"<m:num>";
		
			math_element = dynamic_cast<office_math_element*>(content_[0].get());
			math_element->oox_convert(Context);
		Context.output_stream() << L"</m:num>";
	
		Context.is_need_e_ = false;

		Context.output_stream() << L"<m:den>";
			math_element = dynamic_cast<office_math_element*>(content_[1].get());
			math_element->oox_convert(Context);
		Context.output_stream() << L"</m:den>";

	Context.output_stream() << L"</m:f>";

	if (need_e)
	{
		Context.output_stream() << L"</m:e>";
	}
	Context.is_need_e_ = need_e;
}
//---------------------------------------------------------------
const wchar_t * math_msqrt::ns = L"math";
const wchar_t * math_msqrt::name = L"msqrt";
//----------------------------------------------------------------------------------------------------
void math_msqrt::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_msqrt::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_msqrt::oox_convert(oox::math_context & Context)
{//1* elements
	std::wostream & strm = Context.output_stream();

	strm << L"<m:rad>";
		strm << L"<m:radPr>";
			strm << L"<m:degHide m:val=\"1\"/>";
			strm << Context.math_style_stream().str();
		strm << L"</m:radPr>";

		strm << L"<m:deg/>";
		
		strm << L"<m:e>";		
		
		Context.is_need_e_ = false;
			for (size_t i = 0 ; i < content_.size(); i++)
			{
				office_math_element* math_element = dynamic_cast<office_math_element*>(content_[i].get());
				math_element->oox_convert(Context);
			}
		strm << L"</m:e>";
	strm << L"</m:rad>";
}
//---------------------------------------------------------------
const wchar_t * math_mroot::ns = L"math";
const wchar_t * math_mroot::name = L"mroot";
//----------------------------------------------------------------------------------------------------
void math_mroot::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mroot::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mroot::oox_convert(oox::math_context & Context)
{//2 elements
	std::wostream & strm = Context.output_stream();

	strm << L"<m:rad>";

		office_math_element* math_element = NULL;

		strm << L"<m:radPr>";
			strm << Context.math_style_stream().str();
		strm << L"</m:radPr>";
		
		strm << L"<m:deg>";
			math_element = dynamic_cast<office_math_element*>(content_[1].get());
			math_element->oox_convert(Context);		
		strm << L"</m:deg>";
			
		Context.is_need_e_ = false;

		strm << L"<m:e>";		
			math_element = dynamic_cast<office_math_element*>(content_[0].get());
			math_element->oox_convert(Context);		
		strm << L"</m:e>";		

	strm << L"</m:rad>";
}
//---------------------------------------------------------------
const wchar_t * math_mstyle::ns = L"math";
const wchar_t * math_mstyle::name = L"mstyle";
//----------------------------------------------------------------------------------------------------
void math_mstyle::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	common_attlist_.add_attributes(Attributes);
// ver 2	
    CP_APPLY_ATTR(L"math:fontweight", fontweight_);
	CP_APPLY_ATTR(L"math:mathsize", mathsize_);
	CP_APPLY_ATTR(L"math:color", color_);
   
// ver 3	
	if (!fontweight_)	CP_APPLY_ATTR( L"fontweight",	fontweight_);
	if (!mathsize_)		CP_APPLY_ATTR(L"mathsize",		mathsize_);
	if (!color_)		CP_APPLY_ATTR(L"color",			color_);
}

void math_mstyle::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mstyle::oox_convert(oox::math_context & Context)
{
	Context.text_properties_ = odf_reader::style_text_properties_ptr(new odf_reader::style_text_properties());
	
	Context.text_properties_->content_.style_font_name_ = L"Cambria Math";
	Context.text_properties_->content_.fo_font_size_ = odf_types::length(Context.base_font_size_, odf_types::length::pt);
	
	if (mathsize_)
		Context.text_properties_->content_.fo_font_size_ = mathsize_;

	if (color_)
		Context.text_properties_->content_.fo_color_ = color_;
	
	if (common_attlist_.mathvariant_)
	{
		if (common_attlist_.mathvariant_->style_.bold)
			Context.text_properties_->content_.fo_font_weight_ = odf_types::font_weight(odf_types::font_weight::WBold);
		if (common_attlist_.mathvariant_->style_.italic)
			Context.text_properties_->content_.fo_font_style_ = odf_types::font_style(odf_types::font_style::Italic);
	}

//--------------------------------------------------
	{
		std::wstringstream & strm = Context.math_style_stream();
		strm.str( std::wstring() );
		strm.clear();

		CP_XML_WRITER(strm)
		{ 
			CP_XML_NODE(L"m:ctrlPr")
			{
				Context.text_properties_->content_.oox_serialize(CP_XML_STREAM(), Context.graphRPR_, Context.fonts_container_);
			}
		}
	}
	bool need_e_old = Context.is_need_e_;
	Context.is_need_e_ = content_.size() > 1 ? true : need_e_old;

	for (size_t i = 0; i < content_.size(); i++)
	{
		//math_munder*	munder_test	= dynamic_cast<math_munder*>(content_[i].get());
		//math_mfrac*		frac_test	= dynamic_cast<math_mfrac*>(content_[i].get());
		//math_mrow*		row_test	= dynamic_cast<math_mrow*>(content_[i].get());
		
		//if (row_test || munder_test || frac_test)
		//	Context.output_stream() << L"<m:e>";
		
		office_math_element* math_element = dynamic_cast<office_math_element*>(content_[i].get());
		math_element->oox_convert(Context);		

		//if (row_test || munder_test || frac_test)
		//	Context.output_stream() << L"</m:e>";
		
		//office_math_element* math_element = dynamic_cast<office_math_element*>(content_[i].get());
		//math_element->oox_convert(Context);
	}
//reset to default math text props
	Context.text_properties_ = odf_reader::style_text_properties_ptr(new odf_reader::style_text_properties());
	
	Context.text_properties_->content_.style_font_name_	= L"Cambria Math";
	Context.text_properties_->content_.fo_font_size_		= odf_types::length(Context.base_font_size_, odf_types::length::pt);

	Context.is_need_e_ = need_e_old;
	{
		std::wstringstream & strm = Context.math_style_stream();
		strm.str( std::wstring() );
		strm.clear();

		CP_XML_WRITER(strm)
		{ 
			CP_XML_NODE(L"m:ctrlPr")
			{
				Context.text_properties_->content_.oox_serialize(CP_XML_STREAM(), Context.graphRPR_, Context.fonts_container_);
			}
		}
	}
}
//---------------------------------------------------------------
const wchar_t * math_menclose::ns = L"math";
const wchar_t * math_menclose::name = L"menclose";
//----------------------------------------------------------------------------------------------------
void math_menclose::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
//// ver 2	
//    CP_APPLY_ATTR(L"math:fontweight", fontweight_);
//    
//// ver 3	
//	if (!fontweight_) CP_APPLY_ATTR( L"fontweight", fontweight_);
}

void math_menclose::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_menclose::oox_convert(oox::math_context & Context)
{//0* elements
 	//for (size_t i = 0; i < content_.size(); i++)
	//{
		//office_math_element* math_element = dynamic_cast<office_math_element*>(elm.get());
		//math_element->oox_convert(Context);
	//}
}
//---------------------------------------------------------------
const wchar_t * math_mfenced::ns = L"math";
const wchar_t * math_mfenced::name = L"mfenced";
//----------------------------------------------------------------------------------------------------
void math_mfenced::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
//// ver 2	
//    CP_APPLY_ATTR(L"math:fontweight", fontweight_);
//    
//// ver 3	
//	if (!fontweight_) CP_APPLY_ATTR( L"fontweight", fontweight_);
}

void math_mfenced::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mfenced::oox_convert(oox::math_context & Context)
{//0* elements
 	//for (size_t i = 0; i < content_.size(); i++)
	//{
		//office_math_element* math_element = dynamic_cast<office_math_element*>(elm.get());
		//math_element->oox_convert(Context);
	//}
}
//---------------------------------------------------------------
const wchar_t * math_mpadded::ns = L"mpadded";
const wchar_t * math_mpadded::name = L"mpadded";
//----------------------------------------------------------------------------------------------------
void math_mpadded::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
//// ver 2	
//    CP_APPLY_ATTR(L"math:fontweight", fontweight_);
//    
//// ver 3	
//	if (!fontweight_) CP_APPLY_ATTR( L"fontweight", fontweight_);
}

void math_mpadded::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mpadded::oox_convert(oox::math_context & Context)
{//1* elements

}
}
}
