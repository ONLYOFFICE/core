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
#include "oox_package.h"

#include "../../Common/CPNoncopyable.h"

#include "../../Common/CPScopedPtr.h"
#include "../../Common/CPSharedPtr.h"

#include "oox_title.h"
#include "oox_plot_area.h"
#include "oox_chart_legend.h"
#include "oox_rels.h"

namespace cpdoccore { 
namespace oox {

class oox_chart_context;
typedef _CP_PTR(oox_chart_context) oox_chart_context_ptr;


class oox_chart_context
{
public:
    oox_chart_context(mediaitems_ptr & mediaitems_, std::wstring name);
    ~oox_chart_context();

	void reset_fill(oox::_oox_fill &fill_);

	void serialize(std::wostream & strm);
	void dump_rels(rels & Rels);
	
	void set_pivot_chart(const std::wstring &source)
	{
		pivot_source_ = source;
	}
	void set_title(odf_reader::chart::title & t, odf_reader::chart::title & sub)
	{
		title_.set_content(t);
		title_.set_sub_content(sub);
	}
	void add_axis(int type, odf_reader::chart::axis & content)
	{
		plot_area_.add_axis(type, content);
	}
	void set_3D_chart(bool val)
	{
		for (size_t i = 0 ; i < plot_area_.charts_.size(); ++i)
		{
			plot_area_.charts_[i]->is3D_ = val;

			for (size_t j = 0; j < plot_area_.charts_[i]->series_.size(); ++j)
				plot_area_.charts_[i]->series_[j]->is3D_ = val;
		}
	}
	void set_externalData(const std::wstring & href);
	void set_userShapes(std::pair<std::wstring, std::wstring> &link);

	void add_chart(int type)
	{
		plot_area_.add_chart(type);
		plot_area_.current_chart_ = plot_area_.charts_.back();
	}
	oox_chart_ptr get_current_chart()
	{
		return plot_area_.current_chart_;
	}
	void set_wall(odf_reader::chart::simple & l)
	{
		reset_fill(l.fill_);
		//plot_area_.wall_graphic_properties_	= l.graphic_properties_;
		plot_area_.graphic_properties_	= l.graphic_properties_;
		plot_area_.fill_	= l.fill_;
	}
	void set_floor(odf_reader::chart::simple & l)
	{
		reset_fill(l.fill_);
		//floor_.content_= l;
	}
	void set_legend(odf_reader::chart::legend & l)
	{
		reset_fill(l.fill_);
		legend_.content_= l;
	}

	void set_chart_graphic_properties(std::vector<odf_reader::_property> & prop, _oox_fill &fill)
	{
		reset_fill(fill);
		graphic_properties_= prop;
		fill_ = fill;
	}
	void set_plot_area_properties(std::vector<odf_reader::_property> & prop, std::vector<odf_reader::_property> & prop_3d, _oox_fill &fill)
	{
		reset_fill(fill);
		plot_area_.properties_		= prop; 
		plot_area_.properties_3d_	= prop_3d; 
		plot_area_.fill_			= fill; 
	}
	bool no_used_local_tables_;

	void set_cache_only	(bool val);
private:
	mediaitems_ptr						mediaitems_;
	std::vector<_rel>					rels_;
	
	cpdoccore::oox::oox_title			title_;
	cpdoccore::oox::oox_plot_area		plot_area_;
	cpdoccore::oox::oox_chart_legend	legend_;

	std::wstring						pivot_source_;
	std::wstring						externalDataId_;
	std::wstring						userShapesId_;

	std::vector<odf_reader::_property>	graphic_properties_;
	_oox_fill							fill_;
};

}
}