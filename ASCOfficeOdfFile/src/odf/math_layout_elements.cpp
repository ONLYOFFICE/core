
#include "math_layout_elements.h"

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
void math_mrow::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mrow::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mrow::docx_convert(oox::docx_conversion_context & Context) 
{
	BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->docx_convert(Context);
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
{
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
{
	BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->docx_convert(Context);
    }
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
{

}
//---------------------------------------------------------------
const wchar_t * math_mstyle::ns = L"math";
const wchar_t * math_mstyle::name = L"mstyle";
//----------------------------------------------------------------------------------------------------
void math_mstyle::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
// ver 2	
    CP_APPLY_ATTR(L"math:fontweight", fontweight_);
    
// ver 3	
	if (!fontweight_) CP_APPLY_ATTR( L"fontweight", fontweight_);
}

void math_mstyle::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mstyle::docx_convert(oox::docx_conversion_context & Context) 
{
	BOOST_FOREACH(const office_element_ptr & elm, content_)
	{
		elm->docx_convert(Context);
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
{
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
{
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
{

}
}
}
