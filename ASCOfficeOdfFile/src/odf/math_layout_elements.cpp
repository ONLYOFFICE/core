
#include "math_layout_elements.h"
#include "math_token_elements.h"
#include "style_text_properties.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>


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

void math_mrow::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
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

void math_mrow::docx_convert(oox::docx_conversion_context & Context) 
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
			Context.output_stream() << Context.get_styles_context().math_text_style().str();
		Context.output_stream() << L"</m:dPr>";
			Context.output_stream() << L"<m:e>";
	}
	
	for (int i = i_start; i < i_end ; i++)
	{
		content_[i]->docx_convert(Context);
	}

	if (bDPr)
	{
			Context.output_stream() << L"</m:e>";
		Context.output_stream() << L"</m:d>";
	}
}
//---------------------------------------------------------------
const wchar_t * math_mfrac::ns = L"math";
const wchar_t * math_mfrac::name = L"mfrac";
//----------------------------------------------------------------------------------------------------
void math_mfrac::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mfrac::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mfrac::docx_convert(oox::docx_conversion_context & Context) 
{//2 elements
	if (content_.size() != 2)
	{
		return;
	}
	Context.output_stream() << L"<m:f>";
		Context.output_stream() << L"<m:num>";
			content_[0]->docx_convert(Context);
		Context.output_stream() << L"</m:num>";

		Context.output_stream() << L"<m:den>";
			content_[1]->docx_convert(Context);
		Context.output_stream() << L"</m:den>";

	Context.output_stream() << L"</m:f>";
}
//---------------------------------------------------------------
const wchar_t * math_msqrt::ns = L"math";
const wchar_t * math_msqrt::name = L"msqrt";
//----------------------------------------------------------------------------------------------------
void math_msqrt::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_msqrt::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_msqrt::docx_convert(oox::docx_conversion_context & Context) 
{//1* elements
	std::wostream & strm = Context.output_stream();

	strm << L"<m:rad>";
		strm << L"<m:radPr>";
			strm << L"<m:degHide m:val=\"1\"/>";
			strm << Context.get_styles_context().math_text_style().str();
		strm << L"</m:radPr>";

		strm << L"<m:deg/>";
		
		strm << L"<m:e>";		
			BOOST_FOREACH(const office_element_ptr & elm, content_)
			{
				elm->docx_convert(Context);
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

void math_mroot::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mroot::docx_convert(oox::docx_conversion_context & Context) 
{//2 elements
	std::wostream & strm = Context.output_stream();

	strm << L"<m:rad>";

		strm << L"<m:radPr>";
			strm << Context.get_styles_context().math_text_style().str();
		strm << L"</m:radPr>";
		
		strm << L"<m:deg>";
			content_[1]->docx_convert(Context);
		strm << L"</m:deg>";

		strm << L"<m:e>";		
			content_[0]->docx_convert(Context);
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
	if (!fontweight_)	CP_APPLY_ATTR( L"fontweight", fontweight_);
	if (!mathsize_)	CP_APPLY_ATTR(L"mathsize", mathsize_);
	if (!color_)		CP_APPLY_ATTR(L"color", color_);
}

void math_mstyle::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mstyle::docx_convert(oox::docx_conversion_context & Context) 
{
	style_text_properties textProperty;
	
	if (mathsize_)
		textProperty.content().fo_font_size_ = mathsize_;
	if (color_)
		textProperty.content().fo_color_ = color_;
	if (common_attlist_.mathvariant_)
	{
		if (common_attlist_.mathvariant_->style_.bold)
			textProperty.content().fo_font_weight_ = odf_types::font_weight(odf_types::font_weight::WBold);
		if (common_attlist_.mathvariant_->style_.italic)
			textProperty.content().fo_font_style_ = odf_types::font_style(odf_types::font_style::Italic);
	}

	Context.push_text_properties(&textProperty);
//--------------------------------------------------
	{
		std::wstringstream & strm = Context.get_styles_context().math_text_style();
		strm.str( std::wstring() );
		strm.clear();

		CP_XML_WRITER(strm)
		{ 
			CP_XML_NODE(L"m:ctrlPr")
			{
				Context.get_styles_context().start();
				Context.current_text_properties()->docx_convert(Context);

				CP_XML_NODE(L"w:rPr")
				{
					CP_XML_STREAM() << Context.get_styles_context().text_style().str();
				}	
			}
		}
	}

	BOOST_FOREACH(const office_element_ptr & elm, content_)
	{
		elm->docx_convert(Context);
	}
	
	Context.pop_text_properties();

// reset style ... todooo גםוסעט מעהוכםמ..

	{
		std::wstringstream & strm = Context.get_styles_context().math_text_style();
		strm.str( std::wstring() );
		strm.clear();

		CP_XML_WRITER(strm)
		{ 
			CP_XML_NODE(L"m:ctrlPr")
			{
				Context.get_styles_context().start();
				Context.current_text_properties()->docx_convert(Context);

				CP_XML_NODE(L"w:rPr")
				{
					CP_XML_STREAM() << Context.get_styles_context().text_style().str();
				}	
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

void math_menclose::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_menclose::docx_convert(oox::docx_conversion_context & Context) 
{//0* elements
	//BOOST_FOREACH(const office_element_ptr & elm, content_)
	//{
	//	elm->docx_convert(Context);
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

void math_mfenced::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mfenced::docx_convert(oox::docx_conversion_context & Context) 
{//0* elements
	//BOOST_FOREACH(const office_element_ptr & elm, content_)
	//{
	//	elm->docx_convert(Context);
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

void math_mpadded::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mpadded::docx_convert(oox::docx_conversion_context & Context) 
{//1* elements

}
}
}
