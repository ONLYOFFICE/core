
#include "office_settings.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>

namespace cpdoccore { 
namespace odf_writer {

// office:scripts
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_settings::ns = L"office";
const wchar_t * office_settings::name = L"settings";

void office_settings::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void office_settings::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			BOOST_FOREACH(const office_element_ptr & elm, content_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}
void office_settings::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

// config_item_set
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * settings_config_item_set::ns = L"config";
const wchar_t * settings_config_item_set::name = L"config-item-set";

void settings_config_item_set::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void settings_config_item_set::serialize(std::wostream & _Wostream)
{
	if (config_name_.empty()) return;

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR( L"config:name", config_name_);

			BOOST_FOREACH(const office_element_ptr & elm, content_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}
void settings_config_item_set::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
// config_item
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * settings_config_item::ns = L"config";
const wchar_t * settings_config_item::name = L"config-item";

void settings_config_item::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
}

void settings_config_item::serialize(std::wostream & _Wostream)
{
	if (config_name_.empty() || config_type_.empty()) return;

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR( L"config:name", config_name_);
			CP_XML_ATTR( L"config:type", config_type_);

			CP_XML_STREAM() << content_;
		}
	}
}
void settings_config_item::add_child_element( const office_element_ptr & child_element)
{
}
// config_item_map_indexed
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * settings_config_item_map_indexed::ns = L"config";
const wchar_t * settings_config_item_map_indexed::name = L"config-item-map-indexed";


void settings_config_item_map_indexed::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void settings_config_item_map_indexed::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			if (!config_name_.empty()) 
			{
				CP_XML_ATTR( L"config:name", config_name_);
			}

			BOOST_FOREACH(const office_element_ptr & elm, content_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}
void settings_config_item_map_indexed::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
// config_item_map_named
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * settings_config_item_map_named::ns = L"config";
const wchar_t * settings_config_item_map_named::name = L"config-item-map-named";

void settings_config_item_map_named::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void settings_config_item_map_named::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			if (!config_name_.empty()) 
			{
				CP_XML_ATTR( L"config:name", config_name_);
			}
			BOOST_FOREACH(const office_element_ptr & elm, content_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}
void settings_config_item_map_named::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
// config_item_map_entry
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * settings_config_item_map_entry::ns = L"config";
const wchar_t * settings_config_item_map_entry::name = L"config-item-map-entry";

void settings_config_item_map_entry::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void settings_config_item_map_entry::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			BOOST_FOREACH(const office_element_ptr & elm, content_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}
void settings_config_item_map_entry::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

}
}
