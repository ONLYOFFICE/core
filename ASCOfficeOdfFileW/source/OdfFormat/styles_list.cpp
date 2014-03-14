#include "precompiled_cpodf.h"
#include "styles_list.h"

#include <cpdoccore/xml/serialize.h>
//#include "style_text_properties.h"

#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore { 
namespace odf {
    
using xml::xml_char_wc;


// text:list-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_style::ns = L"text";
const wchar_t * text_list_style::name = L"list-style";


void text_list_style::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"text" == Ns && L"list-level-style-number" == Name)
        CP_CREATE_ELEMENT(text_list_style_content_);
    else if (L"text" == Ns && L"list-level-style-bullet" == Name)
        CP_CREATE_ELEMENT(text_list_style_content_);    
    else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}


// text:list-level-style-number
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_level_style_number::ns = L"text";
const wchar_t * text_list_level_style_number::name = L"list-level-style-number";


void text_list_level_style_number::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if		(L"style" == Ns && L"list-level-properties" == Name)
        CP_CREATE_ELEMENT(style_list_level_properties_);    
	else if (L"style" == Ns && L"text-properties" == Name)
        CP_CREATE_ELEMENT(style_text_properties_); 
	else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}

// style:list-level-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_list_level_properties::ns = L"style";
const wchar_t * style_list_level_properties::name = L"list-level-properties";


void style_list_level_properties::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"style", L"list-level-label-alignment")
    {
        CP_CREATE_ELEMENT(style_list_level_label_alignment_);
    }
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}

// style:list-level-label-alignment
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_list_level_label_alignment::ns = L"style";
const wchar_t * style_list_level_label_alignment::name = L"list-level-label-alignment";



void style_list_level_label_alignment::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
     CP_NOT_APPLICABLE_ELM();
}

// text:list-level-style-bullet
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_level_style_bullet::ns = L"text";
const wchar_t * text_list_level_style_bullet::name = L"list-level-style-bullet";


void text_list_level_style_bullet::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"style" == Ns && L"list-level-properties" == Name)
        CP_CREATE_ELEMENT(style_list_level_properties_);
    else if (L"style" == Ns && L"text-properties" == Name)
        CP_CREATE_ELEMENT(style_text_properties_);    
    else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}

}
}
