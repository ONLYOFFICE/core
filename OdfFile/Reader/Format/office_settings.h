/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include <iosfwd>

#include "office_elements.h"
#include "office_elements_create.h"

namespace cpdoccore { 
namespace odf_reader {

//  office:settings
class office_settings : public office_element_impl<office_settings>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeSettings;
    CPDOCCORE_DEFINE_VISITABLE();

    office_element_ptr_array content_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
	virtual void add_text(const std::wstring & Text){} 
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

	std::wstring				config_name_;
    office_element_ptr_array	content_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
	virtual void add_text(const std::wstring & Text){}
   
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

	std::wstring	config_name_;
 	std::wstring	config_type_;
	std::wstring	content_;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
	virtual void add_text(const std::wstring & Text){	content_ = Text;	}
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

	std::wstring				config_name_;
    office_element_ptr_array	content_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
	virtual void add_text(const std::wstring & Text){}
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

	std::wstring				config_name_;
    office_element_ptr_array	content_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
	virtual void add_text(const std::wstring & Text){}
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

 	std::wstring				config_name_;
   office_element_ptr_array		content_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
	virtual void add_text(const std::wstring & Text){}
};
CP_REGISTER_OFFICE_ELEMENT2(settings_config_item_map_entry);


}
}