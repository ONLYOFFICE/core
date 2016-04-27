
#include "math_table_elements.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

//----------------------------------------------------------------------------------------------------
const wchar_t * math_mtable::ns		= L"math";
const wchar_t * math_mtable::name	= L"mtable";
//----------------------------------------------------------------------------------------------------

void math_mtable::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mtable::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mtable::docx_convert(oox::docx_conversion_context & Context) 
{//0* elements
	std::wostream & strm = Context.output_stream();
	
	strm << L"<m:m>";		
		BOOST_FOREACH(const office_element_ptr & elm, content_)
		{
			elm->docx_convert(Context);
		}
	strm << L"</m:m>";
}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_mtr::ns	= L"math";
const wchar_t * math_mtr::name	= L"mtr";
//----------------------------------------------------------------------------------------------------

void math_mtr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mtr::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mtr::docx_convert(oox::docx_conversion_context & Context) 
{//0* elements
	std::wostream & strm = Context.output_stream();

	strm << L"<m:mr>";		
		BOOST_FOREACH(const office_element_ptr & elm, content_)
		{
			elm->docx_convert(Context);
		}
	strm << L"</m:mr>";
}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_mlabeledtr::ns		= L"math";
const wchar_t * math_mlabeledtr::name	= L"mlabeledtr";
//----------------------------------------------------------------------------------------------------

void math_mlabeledtr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mlabeledtr::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mlabeledtr::docx_convert(oox::docx_conversion_context & Context) 
{

}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_mtd::ns	= L"math";
const wchar_t * math_mtd::name	= L"mtd";
//----------------------------------------------------------------------------------------------------

void math_mtd::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mtd::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mtd::docx_convert(oox::docx_conversion_context & Context) 
{
	std::wostream & strm = Context.output_stream();

	strm << L"<m:e>";		
		BOOST_FOREACH(const office_element_ptr & elm, content_)
		{
			elm->docx_convert(Context);
		}
	strm << L"</m:e>";
}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_maligngroup::ns	= L"math";
const wchar_t * math_maligngroup::name	= L"maligngroup";
//----------------------------------------------------------------------------------------------------

void math_maligngroup::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_maligngroup::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_maligngroup::docx_convert(oox::docx_conversion_context & Context) 
{

}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_malignmark::ns		= L"math";
const wchar_t * math_malignmark::name	= L"malignmarke";
//----------------------------------------------------------------------------------------------------

void math_malignmark::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_malignmark::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_malignmark::docx_convert(oox::docx_conversion_context & Context) 
{

}
}
}
