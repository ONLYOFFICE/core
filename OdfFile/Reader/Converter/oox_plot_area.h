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

#include "oox_types_chart.h"
#include "oox_chart_axis.h"

            
namespace cpdoccore {
namespace oox {

class oox_plot_area: boost::noncopyable
{
public:
	oox_plot_area();
	~oox_plot_area(){}
 
    std::vector<oox_chart_ptr>			charts_;
	oox_chart_ptr						current_chart_;   
	std::vector<oox_axis_content_ptr>	axis_;
	
	std::vector<odf_reader::_property>	graphic_properties_;
	std::vector<odf_reader::_property>	properties_;
	std::vector<odf_reader::_property>	properties_3d_;
	_oox_fill							fill_; 
	
	//std::vector<odf_reader::_property> wall_graphic_properties_;
	
	void oox_serialize(std::wostream & _Wostream);
	void oox_serialize_view3D(std::wostream & _Wostream);

	void add_chart	(int type);
	void add_axis	(int type,	odf_reader::chart::axis & content);

	void set_no_local_table (bool val); //whithout embedded tables
	//void set_content_series	(odf_reader::chart::series & content);

private:
	void reset_cross_axis(); //обязательно после всех добавлений
 	bool no_used_local_tables_;

	unsigned int axis_id_ = 0xf2905;

};

}
}
