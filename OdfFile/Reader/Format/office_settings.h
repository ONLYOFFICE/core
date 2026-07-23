/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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