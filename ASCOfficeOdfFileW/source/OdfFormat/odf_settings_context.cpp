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

void odf_settings_context::add_property(std::wstring name, std::wstring type, std::wstring value)
{
	if (current_view_ < 0) return;
	if (name.empty() || type.empty()) return;

	office_element_ptr prop;
	create_element (L"config", L"config-item", prop, odf_context_);

	settings_config_item *item = NULL;
	item = dynamic_cast<settings_config_item*>(prop.get());
	if (!item) return;

	item->config_name_	= name;
	item->config_type_	= type;
	item->content_		= value;

	if (current_table_ < 0)
	{
		views_[current_view_].content.push_back(prop);
	}
	else
	{
		views_[current_view_].tables.back().content.push_back(prop);
	}
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

		for (int v = 0 ; v < views_.size(); v++)
		{
			office_element_ptr views_entry_elm;
			{
				create_element (L"config", L"config-item-map-entry" , views_entry_elm, odf_context_);
				views_elm->add_child_element(views_entry_elm);
			}

			for (int i = 0; i < views_[v].content.size(); i++)
			{
				views_entry_elm->add_child_element(views_[v].content[i]);
			}

			if (!views_[v].tables.empty())
			{
				office_element_ptr tables_elm;
				create_element(L"config", L"config-item-map-named", tables_elm, odf_context_);
				views_entry_elm->add_child_element(tables_elm);
				
				item_map_named= dynamic_cast<settings_config_item_map_named*>(tables_elm.get());
				if (item_map_named) item_map_named->config_name_ = L"Tables";

				for (int t = 0 ; t < views_[v].tables.size(); t++)
				{
					office_element_ptr table_elm;
					create_element(L"config", L"config-item-map-entry", table_elm, odf_context_);
					tables_elm->add_child_element(table_elm);
					
					item_map_entry= dynamic_cast<settings_config_item_map_entry*>(table_elm.get());
					if (item_map_entry) item_map_entry->config_name_ = views_[v].tables[t].name;

					for (int j = 0; j < views_[v].tables[t].content.size(); j++)
					{
						table_elm->add_child_element(views_[v].tables[t].content[j]);
					}
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