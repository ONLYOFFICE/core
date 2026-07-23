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
	
	odf_reader::graphic_format_properties_ptr graphic_properties_;

	odf_reader::chart_format_properties_ptr properties_;
	_oox_fill fill_; 
	
	//std::vector<odf_reader::_property> wall_graphic_properties_;
	
	void oox_serialize(std::wostream & _Wostream);
	void oox_serialize_view3D(std::wostream & _Wostream);

	void add_chart	(int type);
	void add_axis	(odf_reader::chart::axis & content);

	void set_no_local_table (bool val); //whithout embedded tables
	//void set_content_series	(odf_reader::chart::series & content);
	void set_data_table(odf_reader::chart::simple & content);
private:
	odf_reader::chart::simple data_table_content_;
	void reset_cross_axis(); //must be called after all additions
 	bool no_used_local_tables_;
	unsigned int axis_id_ = 0xf2905;
};

}
}
