
#include "math_elementaries.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

//----------------------------------------------------------------------------------------------------
const wchar_t * math_mstack::ns	= L"math";
const wchar_t * math_mstack::name	= L"mstack";
//----------------------------------------------------------------------------------------------------
void math_mstack::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mstack::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mstack::docx_convert(oox::docx_conversion_context & Context) 
{//0* elements

}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_msrow::ns		= L"math";
const wchar_t * math_msrow::name	= L"msrow";
//----------------------------------------------------------------------------------------------------
void math_msrow::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_msrow::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_msrow::docx_convert(oox::docx_conversion_context & Context) 
{

}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_msline::ns	= L"math";
const wchar_t * math_msline::name	= L"msline";
//----------------------------------------------------------------------------------------------------
void math_msline::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_msline::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_msline::docx_convert(oox::docx_conversion_context & Context) 
{

}

//---------------------------------------------------------------
const wchar_t * math_msgroup::ns	= L"math";
const wchar_t * math_msgroup::name	= L"msgroup";
//----------------------------------------------------------------------------------------------------
void math_msgroup::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_msgroup::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_msgroup::docx_convert(oox::docx_conversion_context & Context) 
{//0* elements

}

//---------------------------------------------------------------
const wchar_t * math_mlongdiv::ns	= L"math";
const wchar_t * math_mlongdiv::name	= L"mlongdiv";
//----------------------------------------------------------------------------------------------------
void math_mlongdiv::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mlongdiv::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mlongdiv::docx_convert(oox::docx_conversion_context & Context) 
{//3* elements

}

//---------------------------------------------------------------
const wchar_t * math_mscarry::ns	= L"math";
const wchar_t * math_mscarry::name	= L"mscarry";
//----------------------------------------------------------------------------------------------------
void math_mscarry::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mscarry::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mscarry::docx_convert(oox::docx_conversion_context & Context) 
{

}

//---------------------------------------------------------------
const wchar_t * math_mscarries::ns		= L"math";
const wchar_t * math_mscarries::name	= L"mscarries";
//----------------------------------------------------------------------------------------------------
void math_mscarries::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mscarries::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mscarries::docx_convert(oox::docx_conversion_context & Context) 
{//0* elements

}
//---------------------------------------------------------------

}
}
