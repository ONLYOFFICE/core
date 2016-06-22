#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements_create.h"

#include "common_attlists.h"

namespace cpdoccore { 
namespace odf_writer {

//  office:settings
class office_settings : public office_element_impl<office_settings>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeSettings;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);
   
	office_element_ptr_array content_;
};

CP_REGISTER_OFFICE_ELEMENT2(office_settings);

//  config:config-item-set
class settings_config_item_set : public office_element_impl<settings_config_item_set>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeSettingsConfigItemSet;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);
   
	std::wstring				config_name_;
    office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(settings_config_item_set);

//  config:config-item
class settings_config_item : public office_element_impl<settings_config_item>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeSettingsConfigItem;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

	std::wstring	config_name_;
 	std::wstring	config_type_;
	std::wstring	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(settings_config_item);

//  config:config-item_map_indexed
class settings_config_item_map_indexed : public office_element_impl<settings_config_item_map_indexed>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeSettingsConfigItemMapIndexed;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

	std::wstring				config_name_;
	office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(settings_config_item_map_indexed);

//  config:config-item_map_named
class settings_config_item_map_named : public office_element_impl<settings_config_item_map_named>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeSettingsConfigItemMapNamed;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

	std::wstring				config_name_;
	office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(settings_config_item_map_named);

//  config:config-item
class settings_config_item_map_entry : public office_element_impl<settings_config_item_map_entry>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeSettingsConfigItemMapEntry;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(settings_config_item_map_entry);


}
}