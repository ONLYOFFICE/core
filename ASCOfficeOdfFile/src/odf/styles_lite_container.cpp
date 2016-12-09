/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
//----------------------------------------------------------------------------------
struct settings_elm
{
	std::vector<container_element>			array_;
    boost::unordered_map<std::wstring, int>	map_;
};

class settings_container::Impl
{
public:
	settings_container::Impl(){}

	settings_elm				common;
	settings_elm				common_view;

	std::vector<settings_elm>	views;
};

settings_container::settings_container(): impl_(new settings_container::Impl() )
{
}

settings_container::~settings_container()
{
}

void settings_container::add(const std::wstring & name, office_element_ptr content)
{
    container_element ref = {name, content};
    
	impl_->common.array_.push_back(ref);
    impl_->common.map_[name] = impl_->common.array_.size() - 1;    
}

void settings_container::start_view ()
{
	settings_elm elm;
	impl_->views.push_back(elm);
}

void settings_container::end_view ()
{
}

int	settings_container::get_views_count()
{
	return impl_->views.size();
}

void settings_container::add_view(const std::wstring & name, office_element_ptr content)
{
    container_element ref = {name, content};
    
	if (impl_->views.empty())
	{
		impl_->common_view.array_.push_back(ref);
		impl_->common_view.map_[name] = impl_->common_view.array_.size() - 1;    
	}
	else
	{
		impl_->views.back().array_.push_back(ref);
		impl_->views.back().map_[name] = impl_->views.back().array_.size() - 1;    
	}
}
_CP_OPT(std::wstring) settings_container::find_by_name(const std::wstring & name)
{
	_CP_OPT(std::wstring) value;
    boost::unordered_map<std::wstring, int>::const_iterator i = impl_->common.map_.find(name);
    
	if (i != impl_->common.map_.end())
    {
        office_element_ptr & elm = impl_->common.array_[i->second].elm;
		settings_config_item* sett = dynamic_cast<settings_config_item*>(elm.get());	
		if (sett)
			value = sett->content_;
    }

	return value;
}
_CP_OPT(std::wstring) settings_container::find_view_by_name(const std::wstring & name, int ind)
{
	_CP_OPT(std::wstring) value;

	if (ind < 0 || ind >= impl_->views.size())
	{
		boost::unordered_map<std::wstring, int>::const_iterator i = impl_->common_view.map_.find(name);
	    
		if (i != impl_->common_view.map_.end())
		{
			office_element_ptr & elm = impl_->common_view.array_[i->second].elm;
			settings_config_item* sett = dynamic_cast<settings_config_item*>(elm.get());	
			if (sett)
				value = sett->content_;
		}
	}
	else
	{
		boost::unordered_map<std::wstring, int>::const_iterator i = impl_->views[ind].map_.find(name);
	    
		if (i != impl_->views[ind].map_.end())
		{
			office_element_ptr & elm = impl_->views[ind].array_[i->second].elm;
			settings_config_item* sett = dynamic_cast<settings_config_item*>(elm.get());	
			if (sett)
				value = sett->content_;
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