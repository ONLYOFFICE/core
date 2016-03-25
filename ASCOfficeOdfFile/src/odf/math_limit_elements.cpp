
#include "math_limit_elements.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

//---------------------------------------------------------------
const wchar_t * math_msub::ns = L"math";
const wchar_t * math_msub::name = L"msub";
//----------------------------------------------------------------------------------------------------
void math_msub::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_msub::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	//<msub> base subscript </msub>

	CP_CREATE_ELEMENT(content_);
}

void math_msub::docx_convert(oox::docx_conversion_context & Context) 
{
	if (content_.size() != 2)
	{
		return;//todooo
	}
    std::wostream & strm = Context.output_stream();
	
	strm << L"<m:sSub>";

		strm << L"<m:e>";
			content_[0]->docx_convert(Context);
		strm << L"</m:e>";
		
		strm << L"<m:sub>";
			content_[1]->docx_convert(Context);
		strm << L"</m:sub>";

	strm << L"</m:sSub>";
}
//---------------------------------------------------------------
const wchar_t * math_msup::ns = L"math";
const wchar_t * math_msup::name = L"msup";
//----------------------------------------------------------------------------------------------------
void math_msup::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_msup::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_msup::docx_convert(oox::docx_conversion_context & Context) 
{
	if (content_.size() != 2)
	{
		return;//todooo
	}
    std::wostream & strm = Context.output_stream();
	
	strm << L"<m:sSup>";

		strm << L"<m:e>";
			content_[0]->docx_convert(Context);
		strm << L"</m:e>";
		
		strm << L"<m:sup>";
			content_[1]->docx_convert(Context);
		strm << L"</m:sup>";

	strm << L"</m:sSup>";
}

//---------------------------------------------------------------
const wchar_t * math_msubsup::ns = L"math";
const wchar_t * math_msubsup::name = L"msubsup";
//----------------------------------------------------------------------------------------------------
void math_msubsup::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_msubsup::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_msubsup::docx_convert(oox::docx_conversion_context & Context) 
{

}
//---------------------------------------------------------------
const wchar_t * math_none::ns = L"math";
const wchar_t * math_none::name = L"none";
//----------------------------------------------------------------------------------------------------
void math_none::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_none::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_none::docx_convert(oox::docx_conversion_context & Context) 
{

}

//---------------------------------------------------------------
const wchar_t * math_mprescripts::ns = L"math";
const wchar_t * math_mprescripts::name = L"mprescripts";
//----------------------------------------------------------------------------------------------------
void math_mprescripts::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mprescripts::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mprescripts::docx_convert(oox::docx_conversion_context & Context) 
{

}
//---------------------------------------------------------------
const wchar_t * math_mmultiscripts::ns = L"math";
const wchar_t * math_mmultiscripts::name = L"mmultiscripts";
//----------------------------------------------------------------------------------------------------
void math_mmultiscripts::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mmultiscripts::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mmultiscripts::docx_convert(oox::docx_conversion_context & Context) 
{

}
//---------------------------------------------------------------
const wchar_t * math_munderover::ns = L"math";
const wchar_t * math_munderover::name = L"munderover";
//----------------------------------------------------------------------------------------------------
void math_munderover::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_munderover::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_munderover::docx_convert(oox::docx_conversion_context & Context) 
{

}
//---------------------------------------------------------------
const wchar_t * math_mover::ns = L"math";
const wchar_t * math_mover::name = L"mover";
//----------------------------------------------------------------------------------------------------
void math_mover::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mover::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mover::docx_convert(oox::docx_conversion_context & Context) 
{

}
//---------------------------------------------------------------
const wchar_t * math_munder::ns = L"math";
const wchar_t * math_munder::name = L"munder";
//----------------------------------------------------------------------------------------------------
void math_munder::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_munder::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_munder::docx_convert(oox::docx_conversion_context & Context) 
{

}
}
}
