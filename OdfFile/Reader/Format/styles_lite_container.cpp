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

#include <vector>
#include <boost/unordered_map.hpp>

#include "styles_lite_container.h"
#include "office_settings.h"

namespace cpdoccore { 

namespace odf_reader {

struct container_element
{
    std::wstring		name;
    office_element_ptr	elm;
};

struct styles_lite_container::Impl
{
  
	std::vector<container_element>			array_;
	boost::unordered_map<std::wstring, int>	map_;
};

styles_lite_container::styles_lite_container(): impl_(new styles_lite_container::Impl() )
{
}

styles_lite_container::~styles_lite_container()
{
}

void styles_lite_container::add(const std::wstring & name, office_element_ptr content)
{
    container_element ref = {name, content};
    
	impl_->array_.push_back(ref);
    impl_->map_[name] = impl_->array_.size() - 1;    
}

office_element_ptr styles_lite_container::find_by_style_name(const std::wstring & name)
{
    boost::unordered_map<std::wstring, int>::const_iterator i = impl_->map_.find(name);
    
	if (i != impl_->map_.end())
    {
        return impl_->array_[i->second].elm;
    }
    else
    {
        return office_element_ptr();
    }
}
class doc_props_container::Impl
{
public:
	Impl() {}

	std::map<std::wstring, std::wstring> map_user_defineds;
};

doc_props_container::doc_props_container() : impl_(new doc_props_container::Impl())
{
}

doc_props_container::~doc_props_container()
{
}
void doc_props_container::add_user_defined(const std::wstring & name, const std::wstring & value)
{
	impl_->map_user_defineds.insert(std::make_pair(name, value));
}
std::wstring doc_props_container::get_user_defined(const std::wstring & name)
{
	std::map<std::wstring, std::wstring>::iterator pFind = impl_->map_user_defineds.find(name);

	return pFind != impl_->map_user_defineds.end() ? pFind->second : L"";
}

//----------------------------------------------------------------------------------
struct settings_value
{
    std::wstring name;
	std::wstring value;
};

struct settings_elm
{
	std::vector<settings_value>				array_;
    boost::unordered_map<std::wstring, int>	map_;
};

struct view_elm : settings_elm
{
	boost::unordered_map<std::wstring, int>	map_tables;
	std::vector<settings_elm>				tables;
};
//------------------------------------------------------------------------------------
class settings_container::Impl
{
public:
    Impl() : inViewTable(false), inView(false) {}

	settings_elm				common; 
    settings_elm				common_view;

	std::vector<view_elm>		views;

	bool inViewTable;
	bool inView;

	std::map<std::wstring, std::wstring> map_user_defineds;
};

settings_container::settings_container(): impl_(new settings_container::Impl() )
{
}

settings_container::~settings_container()
{
}

void settings_container::add(const std::wstring & name, const std::wstring & value)
{
    settings_value ref = {name, value};
    
	impl_->common.array_.push_back(ref);
    impl_->common.map_[name] = impl_->common.array_.size() - 1;    
}

void settings_container::start_view ()
{
	impl_->inView = true;

	view_elm elm;
	impl_->views.push_back(elm);
}

void settings_container::end_view ()
{
	impl_->inView = false;
}

void settings_container::start_table_view (const std::wstring & name)
{
	if (name.empty()) return;

	impl_->inViewTable = true;
	
	settings_elm elm;

	impl_->views.back().tables.push_back(elm);
    impl_->views.back().map_tables[name] = impl_->views.back().tables.size() - 1;    
}

void settings_container::end_table_view ()
{
	impl_->inViewTable = false;
}

int settings_container::get_views_count()
{
	return impl_->views.size();
}

int settings_container::get_table_view_count(int ind, std::wstring name)
{
	if (ind < 0 || ind >= (int)impl_->views.size()) return 0;
	
    boost::unordered_map<std::wstring, int>::const_iterator i = impl_->views[ind].map_tables.find(name);
	
	if (i != impl_->views[ind].map_tables.end())
	{
		return impl_->views[ind].tables[i->second].array_.size();
	}
	
	return 0;
}

void settings_container::add_view(const std::wstring & name, const std::wstring & value)
{
    settings_value ref = {name, value};
    
	if (impl_->inView)
	{
		if (impl_->inViewTable)
		{
			impl_->views.back().tables.back().array_.push_back(ref);
			impl_->views.back().tables.back().map_[name] = impl_->views.back().tables.back().array_.size() - 1;   
		}
		else
		{
			impl_->views.back().array_.push_back(ref);
			impl_->views.back().map_[name] = impl_->views.back().array_.size() - 1;   
		}
	}
	else
	{
		impl_->common_view.array_.push_back(ref);
		impl_->common_view.map_[name] = impl_->common_view.array_.size() - 1;    
	}
}
std::pair<std::wstring, std::wstring> settings_container::get_table_view(int index_view, const std::wstring & table_name, int index)
{
	std::pair<std::wstring, std::wstring> value;
	
	if (index		< 0) return value;
	if (index_view	< 0 || 
		index_view	>= (int)impl_->views.size()) return value;
	
    boost::unordered_map<std::wstring, int>::const_iterator i = impl_->views[index_view].map_tables.find(table_name);
	
	if (i == impl_->views[index_view].map_tables.end()) return value;
		
	if (index < (int)impl_->views[index_view].tables[i->second].array_.size())
	{
		value.first		= impl_->views[index_view].tables[i->second].array_[index].name;
		value.second	= impl_->views[index_view].tables[i->second].array_[index].value;
	}

	return value;
}
_CP_OPT(std::wstring) settings_container::find_by_name(const std::wstring & name)
{
	_CP_OPT(std::wstring) value;
    boost::unordered_map<std::wstring, int>::const_iterator i = impl_->common.map_.find(name);
    
	if (i != impl_->common.map_.end())
    {
		value = impl_->common.array_[i->second].value;
    }

	return value;
}

_CP_OPT(std::wstring) settings_container::find_view_by_name(const std::wstring & name, int ind)
{
	_CP_OPT(std::wstring) value;

	if (ind < 0 || ind >= (int)impl_->views.size())
	{
		boost::unordered_map<std::wstring, int>::const_iterator i = impl_->common_view.map_.find(name);
	    
		if (i != impl_->common_view.map_.end())
		{
			value = impl_->common_view.array_[i->second].value;
		}
	}
	else
	{
		boost::unordered_map<std::wstring, int>::const_iterator i = impl_->views[ind].map_.find(name);
	    
		if (i != impl_->views[ind].map_.end())
		{
			value = impl_->views[ind].array_[i->second].value;
		}
		else
		{
			return find_view_by_name(name);
		}
	}
	return value;
}

}
}
