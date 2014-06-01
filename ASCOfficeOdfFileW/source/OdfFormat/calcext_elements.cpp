#include "precompiled_cpodf.h"
#include "calcext_elements.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/utils.h>

namespace cpdoccore { 
namespace odf {

void calcext_data_bar_attr::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT(L"calcext:axis-color",		calcext_axis_color_);
	CP_XML_ATTR_OPT(L"calcext:positive-color",	calcext_positive_color_);
	CP_XML_ATTR_OPT(L"calcext:negative-color",	calcext_negative_color_);
}

void calcext_icon_set_attr::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT(L"calcext:icon-set-type",	calcext_icon_set_type_); 

}
void calcext_condition_attr::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT(L"calcext:base-cell-address",	calcext_base_cell_address_);
	CP_XML_ATTR_OPT(L"calcext:apply-style-name",	calcext_apply_style_name_);
	CP_XML_ATTR_OPT(L"calcext:value",				calcext_value_);
}
void calcext_date_is_attr::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT(L"calcext:style",	calcext_style_);
	CP_XML_ATTR_OPT(L"calcext:date",	calcext_date_);
}
// calcext_conditional_formats
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_conditional_formats::ns = L"calcext";
const wchar_t * calcext_conditional_formats::name = L"conditional-formats";

void calcext_conditional_formats::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_conditional_formats::add_child_element( office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
void calcext_conditional_formats::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			BOOST_FOREACH(const office_element_ptr & item, content_)
			{
				item->serialize(CP_XML_STREAM());
			}
		}
	}
}
	
// calcext_conditional_format
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_conditional_format::ns = L"calcext";
const wchar_t * calcext_conditional_format::name = L"conditional-format";

void calcext_conditional_format::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_conditional_format::add_child_element( office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
void calcext_conditional_format::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			CP_XML_ATTR_OPT(L"calcext:target-range-address",calcext_target_range_address_);

			BOOST_FOREACH(const office_element_ptr & item, content_)
			{
				item->serialize(CP_XML_STREAM());
			}
		}
	}
}
	
// calcext_data_bar
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_data_bar::ns = L"calcext";
const wchar_t * calcext_data_bar::name = L"data-bar";

void calcext_data_bar::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_data_bar::add_child_element( office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
void calcext_data_bar::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			calcext_data_bar_attr_.serialize(CP_GET_XML_NODE());

			BOOST_FOREACH(const office_element_ptr & item, content_)
			{
				item->serialize(CP_XML_STREAM());
			}
		}
	}
}
// calcext_color_scale
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_color_scale::ns = L"calcext";
const wchar_t * calcext_color_scale::name = L"color-scale";

void calcext_color_scale::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_color_scale::add_child_element( office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
void calcext_color_scale::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			BOOST_FOREACH(const office_element_ptr & item, content_)
			{
				item->serialize(CP_XML_STREAM());
			}
		}
	}
}
// calcext_icon_set
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_icon_set::ns = L"calcext";
const wchar_t * calcext_icon_set::name = L"icon-set";

void calcext_icon_set::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void calcext_icon_set::add_child_element( office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
void calcext_icon_set::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			calcext_icon_set_attr_.serialize(CP_GET_XML_NODE());

			BOOST_FOREACH(const office_element_ptr & item, content_)
			{
				item->serialize(CP_XML_STREAM());
			}
		}
	}
}
	
// calcext_formatting_entry
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_formatting_entry::ns = L"calcext";
const wchar_t * calcext_formatting_entry::name = L"formatting-entry";

void calcext_formatting_entry::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			CP_XML_ATTR_OPT(L"calcext:value",calcext_value_);
			CP_XML_ATTR_OPT(L"calcext:type", calcext_type_);
		}
	}
}

// calcext_color_scale_entry
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_color_scale_entry::ns = L"calcext";
const wchar_t * calcext_color_scale_entry::name = L"color_scale_entry";

void calcext_color_scale_entry::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			CP_XML_ATTR_OPT(L"calcext:value",	calcext_value_);
			CP_XML_ATTR_OPT(L"calcext:type",	calcext_type_);
			CP_XML_ATTR_OPT(L"calcext:color",	calcext_color_);
		}
	}
}
// calcext_condition
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_condition::ns = L"calcext";
const wchar_t * calcext_condition::name = L"condition";

void calcext_condition::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			calcext_condition_attr_.serialize(CP_GET_XML_NODE());
		}
	}
}
// calcext_condition
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * calcext_date_is::ns = L"calcext";
const wchar_t * calcext_date_is::name = L"date-is";

void calcext_date_is::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			calcext_date_is_attr_.serialize(CP_GET_XML_NODE());
		}
	}
}
}
}