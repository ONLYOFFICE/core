#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"

namespace cpdoccore { 
namespace odf_reader {

/// \class  office_settings
/// \brief  office:settings
class office_settings : public office_element_impl<office_settings>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeSettings;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_text(const std::wstring & Text){}

private:
    office_element_ptr_array content_;
   
};

CP_REGISTER_OFFICE_ELEMENT2(office_settings);

/// \class  config_item_set
/// \brief  config:config-item-set
class settings_config_item_set : public office_element_impl<settings_config_item_set>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeSettingsConfigItemSet;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_text(const std::wstring & Text){}

private:
    office_element_ptr_array content_;

    
};
CP_REGISTER_OFFICE_ELEMENT2(settings_config_item_set);

/// \class  config_item
/// \brief  config:config-item
class settings_config_item : public office_element_impl<settings_config_item>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeSettingsConfigItem;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_text(const std::wstring & Text){}

private:
    office_element_ptr_array content_;

    
};
CP_REGISTER_OFFICE_ELEMENT2(settings_config_item);

/// \class  config_item_map_indexed
/// \brief  config:config-item_map_indexed
class settings_config_item_map_indexed : public office_element_impl<settings_config_item_map_indexed>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeSettingsConfigItemMapIndexed;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_text(const std::wstring & Text){}

private:
    office_element_ptr_array content_;

    
};
CP_REGISTER_OFFICE_ELEMENT2(settings_config_item_map_indexed);

/// \class  config_item_map_named
/// \brief  config:config-item_map_named
class settings_config_item_map_named : public office_element_impl<settings_config_item_map_named>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeSettingsConfigItemMapNamed;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_text(const std::wstring & Text){}

private:
    office_element_ptr_array content_;

    
};
CP_REGISTER_OFFICE_ELEMENT2(settings_config_item_map_named);

/// \class  config_item
/// \brief  config:config-item
class settings_config_item_map_entry : public office_element_impl<settings_config_item_map_entry>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeSettingsConfigItemMapEntry;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_text(const std::wstring & Text){}

private:
    office_element_ptr_array content_;

    
};
CP_REGISTER_OFFICE_ELEMENT2(settings_config_item_map_entry);


}
}