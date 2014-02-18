#include "precompiled_cpodf.h"
#include "office_settings.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"

namespace cpdoccore { 
namespace odf {

// office:scripts
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_settings::ns = L"office";
const wchar_t * office_settings::name = L"settings";

void office_settings::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void office_settings::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

// config_item_set
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * settings_config_item_set::ns = L"config";
const wchar_t * settings_config_item_set::name = L"config-item-set";

void settings_config_item_set::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void settings_config_item_set::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}
// config_item
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * settings_config_item::ns = L"config";
const wchar_t * settings_config_item::name = L"config-item";

void settings_config_item::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void settings_config_item::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}
// config_item_map_indexed
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * settings_config_item_map_indexed::ns = L"config";
const wchar_t * settings_config_item_map_indexed::name = L"config-item-map-indexed";

void settings_config_item_map_indexed::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void settings_config_item_map_indexed::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}
// config_item_map_named
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * settings_config_item_map_named::ns = L"config";
const wchar_t * settings_config_item_map_named::name = L"config-item-map-named";

void settings_config_item_map_named::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void settings_config_item_map_named::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}
// config_item_map_entry
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * settings_config_item_map_entry::ns = L"config";
const wchar_t * settings_config_item_map_entry::name = L"config-item-map-entry";

void settings_config_item_map_entry::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void settings_config_item_map_entry::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}


}
}
