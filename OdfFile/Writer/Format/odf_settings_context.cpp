/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "odf_settings_context.h"
#include "ods_conversion_context.h"

#include "logging.h"

#include "office_settings.h"

#include <iostream>

namespace cpdoccore {

	using namespace odf_types;

namespace odf_writer {

odf_settings_context::odf_settings_context()
{    
	current_view_	= -1;
	current_table_	= -1;
}

void odf_settings_context::set_odf_context(odf_conversion_context * Context)
{
	odf_context_ = Context;
	
}

void odf_settings_context::start_view()
{
	_view v;
	views_.push_back(v);
	current_view_ = views_.size() - 1;
}

void odf_settings_context::end_view()
{
	current_view_ = -1;
}

void odf_settings_context::set_current_view(int id)
{
	if (id < 0 || id >= views_.size()) return;

	current_view_ = id;
}

void odf_settings_context::start_table(std::wstring name)
{
	if (current_view_ < 0) return;

	_table t;
	t.name = name;

	views_[current_view_].tables.push_back(t);

	current_table_ = views_[current_view_].tables.size() - 1;
}

void odf_settings_context::end_table()
{
	current_table_ = -1;
}
void odf_settings_context::set_modify_info(const std::wstring& crypt, const std::wstring& algorithm, const std::wstring& solt, const std::wstring& hash, int iteration_count)
{
	office_element_ptr elm_item_set;
	create_element(L"config", L"config-item-set", elm_item_set, odf_context_);
	config_content_.push_back(elm_item_set);
	
	settings_config_item_set *item_set = dynamic_cast<settings_config_item_set*>(elm_item_set.get());
	if (item_set)
	{
		item_set->config_name_ = (algorithm != L"PBKDF2") ? L"OOXMLModifyPasswordInfo" : L"ModifyPasswordInfo";

		office_element_ptr prop;
		if (false == crypt.empty())
		{
			prop = create_property(L"crypt-name", L"string", crypt);							elm_item_set->add_child_element(prop);
		}
		prop = create_property(L"algorithm-name", L"string", algorithm);						elm_item_set->add_child_element(prop);
		prop = create_property(L"salt", L"base64Binary", solt);									elm_item_set->add_child_element(prop);
		prop = create_property(L"iteration-count", L"int", std::to_wstring(iteration_count));	elm_item_set->add_child_element(prop);
		prop = create_property(L"hash", L"base64Binary", hash);									elm_item_set->add_child_element(prop);
	}
}
office_element_ptr odf_settings_context::create_property(const std::wstring &name, const std::wstring &type, const std::wstring &value)
{
	office_element_ptr prop;
	create_element(L"config", L"config-item", prop, odf_context_);

	settings_config_item *item = NULL;
	item = dynamic_cast<settings_config_item*>(prop.get());
	if (!item) return prop;

	item->config_name_ = name;
	item->config_type_ = type;
	item->content_ = value;

	return prop;
}
void odf_settings_context::add_property(const std::wstring &name, const std::wstring &type, const std::wstring &value)
{
	if (current_view_ < 0) return;
	if (name.empty() || type.empty()) return;

	office_element_ptr prop = create_property(name, type, value);

	if (current_table_ < 0)
	{
		views_[current_view_].content.push_back(prop);
	}
	else
	{
		views_[current_view_].tables.back().content.push_back(prop);
	}
}
void odf_settings_context::add_common_views_property(const std::wstring &name, const std::wstring &type, const std::wstring &value)
{
	if (name.empty() || type.empty()) return;

	office_element_ptr prop = create_property(name, type, value);
	common_views_content_.push_back(prop);
}
void odf_settings_context::add_config_content_item(const std::wstring &name, const std::wstring &type, const std::wstring &value)
{
	if (name.empty() || type.empty()) return;

	office_element_ptr prop = create_property(name, type, value);
	config_content_.push_back(prop);	
}

void odf_settings_context::process_office_settings(office_element_ptr root )
{
	settings_config_item_set			*item_set			= NULL;
	settings_config_item_map_named		*item_map_named		= NULL;
	settings_config_item_map_indexed	*item_map_indexed	= NULL;
	settings_config_item_map_entry		*item_map_entry		= NULL;

	if (!views_.empty())
	{
		office_element_ptr ooo_view_elm;
		{
			create_element (L"config", L"config-item-set", ooo_view_elm, odf_context_);
			root->add_child_element(ooo_view_elm);
			item_set = dynamic_cast<settings_config_item_set*>(ooo_view_elm.get());
			if (item_set) item_set->config_name_ = L"ooo:view-settings";
		}
		office_element_ptr views_elm;
		{
			create_element (L"config", L"config-item-map-indexed" , views_elm, odf_context_);
			ooo_view_elm->add_child_element(views_elm);
			item_map_indexed= dynamic_cast<settings_config_item_map_indexed*>(views_elm.get());
			if (item_map_indexed) item_map_indexed->config_name_ = L"Views";
		}

		for (size_t v = 0 ; v < views_.size(); v++)
		{
			office_element_ptr views_entry_elm;
			{
				create_element (L"config", L"config-item-map-entry" , views_entry_elm, odf_context_);
				views_elm->add_child_element(views_entry_elm);
			}

			if (!views_[v].tables.empty())
			{
				office_element_ptr tables_elm;
				create_element(L"config", L"config-item-map-named", tables_elm, odf_context_);
				views_entry_elm->add_child_element(tables_elm);
				
				item_map_named= dynamic_cast<settings_config_item_map_named*>(tables_elm.get());
				if (item_map_named) item_map_named->config_name_ = L"Tables";

				for (size_t t = 0 ; t < views_[v].tables.size(); t++)
				{
					office_element_ptr table_elm;
					create_element(L"config", L"config-item-map-entry", table_elm, odf_context_);
					tables_elm->add_child_element(table_elm);
					
					item_map_entry= dynamic_cast<settings_config_item_map_entry*>(table_elm.get());
					if (item_map_entry) item_map_entry->config_name_ = views_[v].tables[t].name;

					for (size_t j = 0; j < views_[v].tables[t].content.size(); j++)
					{
						table_elm->add_child_element(views_[v].tables[t].content[j]);
					}
				}
			}

			for (size_t i = 0; i < views_[v].content.size(); i++)
			{
				views_entry_elm->add_child_element(views_[v].content[i]);
			}
			for (size_t i = 0; i < common_views_content_.size(); i++)
			{
				views_entry_elm->add_child_element(common_views_content_[i]);
			}
		}
	}
	if (!config_content_.empty())
	{
		office_element_ptr ooo_config_elm;
		create_element (L"config", L"config-item-set", ooo_config_elm, odf_context_);
		root->add_child_element(ooo_config_elm);
		
		item_set = dynamic_cast<settings_config_item_set*>(ooo_config_elm.get());
		if (item_set) item_set->config_name_ = L"ooo:configuration-settings";

		for (size_t j = 0; j < config_content_.size(); j++)
		{
			ooo_config_elm->add_child_element(config_content_[j]);
		}
	}
}


}
}