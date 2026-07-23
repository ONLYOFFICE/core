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
#include <CPOptional.h>
#include <xml/xmlelement.h>
#include <xml/nodetype.h>

#include "office_elements_create.h"

#include "../../DataTypes/common_attlists.h"

namespace cpdoccore { 
namespace odf_writer {

//  office:settings
class office_settings : public office_element_impl<office_settings>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeOfficeSettings;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
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

    static const ElementType type = typeOfficeSettingsConfigItemSet;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
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

    static const ElementType type = typeOfficeSettingsConfigItem;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
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

    static const ElementType type = typeOfficeSettingsConfigItemMapIndexed;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)		config_name_;
	office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(settings_config_item_map_indexed);

//  config:config-item_map_named
class settings_config_item_map_named : public office_element_impl<settings_config_item_map_named>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeOfficeSettingsConfigItemMapNamed;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)		config_name_;
	office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(settings_config_item_map_named);

//  config:config-item
class settings_config_item_map_entry : public office_element_impl<settings_config_item_map_entry>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeOfficeSettingsConfigItemMapEntry;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)		config_name_;
    office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(settings_config_item_map_entry);


}
}