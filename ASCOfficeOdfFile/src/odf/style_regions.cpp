#include "precompiled_cpodf.h"
#include "style_regions.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"


namespace cpdoccore { 
namespace odf {

// style:region-left
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_region_left::ns = L"style";
const wchar_t * style_region_left::name = L"region-left";

void style_region_left::docx_convert(oox::docx_conversion_context & Context)
{
    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->docx_convert(Context);
    }
}

void style_region_left::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->xlsx_convert(Context);
    }
}

void style_region_left::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void style_region_left::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void style_region_left::add_text(const std::wstring & Text)
{
}

// style:region-right
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_region_right::ns = L"style";
const wchar_t * style_region_right::name = L"region-right";

void style_region_right::docx_convert(oox::docx_conversion_context & Context) 
{
    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->docx_convert(Context);
    }
}

void style_region_right::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->xlsx_convert(Context);
    }
}

void style_region_right::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void style_region_right::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void style_region_right::add_text(const std::wstring & Text)
{
}

// style:region-center
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_region_center::ns = L"style";
const wchar_t * style_region_center::name = L"region-center";

void style_region_center::docx_convert(oox::docx_conversion_context & Context) 
{
    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->docx_convert(Context);
    }
}

void style_region_center::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->xlsx_convert(Context);
    }
}

void style_region_center::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void style_region_center::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void style_region_center::add_text(const std::wstring & Text)
{
}


}
}
