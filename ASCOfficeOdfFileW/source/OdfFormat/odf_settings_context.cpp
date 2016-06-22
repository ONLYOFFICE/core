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
}

void odf_settings_context::set_odf_context(odf_conversion_context * Context)
{
	odf_context_ = Context;
	
}

void odf_settings_context::process_office_settings(office_element_ptr root )
{
	settings_config_item_set			*item_set			= NULL;
	settings_config_item_map_named		*item_map_named		= NULL;
	settings_config_item_map_indexed	*item_map_indexed	= NULL;
	settings_config_item_map_entry		*item_map_entry		= NULL;

	if (!views_.content.empty() || !views_.tables.empty())
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

		office_element_ptr views_entry_elm;
		{
			create_element (L"config", L"config-item-map-entry" , views_entry_elm, odf_context_);
			views_elm->add_child_element(views_entry_elm);
		}

		for (int i = 0; i < views_.content.size(); i++)
		{
			views_entry_elm->add_child_element(views_.content[i]);
		}

		if (!views_.tables.empty())
		{
			office_element_ptr tables_elm;
			create_element(L"config", L"config-item-map-named", tables_elm, odf_context_);
			views_entry_elm->add_child_element(tables_elm);
			item_map_named= dynamic_cast<settings_config_item_map_named*>(tables_elm.get());
			if (item_map_named) item_map_indexed->config_name_ = L"Tables";

			for (std::map<std::wstring, std::vector<office_element_ptr>>::iterator it = views_.tables.begin(); it != views_.tables.end(); it++)
			{
				office_element_ptr table_elm;
				create_element(L"config", L"config-item-map-entry", table_elm, odf_context_);
				tables_elm->add_child_element(table_elm);
				item_map_entry= dynamic_cast<settings_config_item_map_entry*>(table_elm.get());
				if (item_map_entry) item_map_indexed->config_name_ = it->first;

				for (int j = 0; j < it->second.size(); j++)
				{
					table_elm->add_child_element(it->second[j]);
				}
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

		for (int j = 0; j < config_content_.size(); j++)
		{
			ooo_config_elm->add_child_element(config_content_[j]);
		}
	}
}


}
}