
#include "math_elements.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

//---------------------------------------------------------------
const wchar_t * office_math::ns = L"math";
const wchar_t * office_math::name = L"math";

//----------------------------------------------------------------------------------------------------

void office_math::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void office_math::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME1(L"semantics")
    {
        CP_CREATE_ELEMENT(semantics_);
    }
}


void office_math::docx_convert(oox::docx_conversion_context & Context) 
{
	if (semantics_)
		semantics_->docx_convert(Context);
}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_semantics::ns = L"math";
const wchar_t * math_semantics::name = L"semantics";
//----------------------------------------------------------------------------------------------------

void math_semantics::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void math_semantics::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME1(L"annotation")
    {
        CP_CREATE_ELEMENT(annotation_);
    }
	else
        CP_CREATE_ELEMENT(content_);

}


void math_semantics::docx_convert(oox::docx_conversion_context & Context) 
{
	Context.start_math_formula();

	BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->docx_convert(Context);
    }

	Context.end_math_formula();
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

void math_annotation::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);

}

void math_annotation::add_text(const std::wstring & Text) 
{
    text_ = Text;
}

void math_annotation::docx_convert(oox::docx_conversion_context & Context) 
{

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

void math_annotation_xml::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);

}

void math_annotation_xml::add_text(const std::wstring & Text) 
{
    text_ = Text;
}

void math_annotation_xml::docx_convert(oox::docx_conversion_context & Context) 
{

}
//----------------------------------------------------------------------------------------------------

}
}
