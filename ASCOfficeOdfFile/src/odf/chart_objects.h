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

#include <xml/attributes.h>
#include "../docx/oox_drawing_fills.h"
//описание всяких графических объектов ( вложенная мелочевка) которые используются в xlsx & odf_reader - 

namespace cpdoccore { 
namespace odf_reader {

class text_format_properties_content;

namespace chart {

	enum oox_typeconvert {docx,xlsx,pptx};

    enum class_type 
    {
        chart_unknown,
        chart_line, 
        chart_area,
        chart_circle,
        chart_ring,
        chart_scatter,
        chart_radar,
        chart_bar,
        chart_stock,
        chart_bubble,
        chart_surface,
        chart_gantt,
		chart_filled_radar
    };

	struct title
	{
		title() : bEnabled(false), pos_x(0), pos_y(0) {}
		bool					bEnabled;
		
		std::wstring			content_;
		std::vector<_property>	text_properties_;
		
		double					pos_x;
		double					pos_y;
		
	} ; 
	struct simple
	{
		simple() : bEnabled(false) {}
		
		bool					bEnabled;
		std::vector<_property>	properties_; 
 		std::vector<_property>	text_properties_;
		std::vector<_property>	graphic_properties_;
		oox::_oox_fill			fill_;
	};
	struct treadline
	{
		bool					bEquation;
		bool					bREquation;
		std::vector<_property>	line_properties_;
		simple					equation_properties_;

		treadline(){bEquation = false; bREquation = false;}

	};
	struct legend : public simple
    {
		std::wstring position;
		std::wstring align;
	};
	struct plot_area : public simple
    {
		std::wstring			cell_range_address_;
		std::vector<_property>	properties_3d_; 
		//series_in_column_or_row
		//uses_first_as_label
	};
	struct axis: public simple
    {
		axis() : bCategories_(false), type_(3) {}
        struct grid
        {
            enum grid_type {major, minor};

            grid_type				type_;
            std::wstring			style_name_;
 			
			std::vector<_property>	graphic_properties_;
       };
		bool				bCategories_;
		
		title title_;
        
		std::wstring		dimension_;
        std::wstring		chart_name_;
        std::wstring		style_name_;
        std::vector<grid>	grids_;
		int					type_;		
	};
    struct series : public simple
    {
		struct point : public simple
        {
			point(unsigned int rep/*, std::wstring const & styleName*/) : repeated_(rep)/*, style_name_(styleName)*/{}

            unsigned int	repeated_;
        };

        std::wstring		name_;
		std::wstring		cell_range_address_;

        std::wstring		label_cell_address_;
        class_type			class_;
        std::wstring		attached_axis_;
        std::wstring		style_name_;  
        std::vector<point>	points_;
		
		simple				mean_value_;
		simple				error_indicator_;
		treadline			regression_curve_;

		series(){}
        series(
				std::wstring const & rangeAddress,
				std::wstring const & labelCell,
				class_type classType,
				std::wstring const & attachedAxis,
				std::wstring const & styleName                        
            ) : 
				cell_range_address_(rangeAddress),
                label_cell_address_(labelCell),
                class_(classType),
                attached_axis_(attachedAxis),
                style_name_(styleName)
			{}

    };
}
}
}