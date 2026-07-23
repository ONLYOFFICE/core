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

#include "../../Reader/Converter/oox_drawing_fills.h"
#include "../../DataTypes/chartclass.h"

namespace cpdoccore { 
namespace odf_reader {

	class text_format_properties;
	typedef boost::shared_ptr<text_format_properties> text_format_properties_ptr;

	class graphic_format_properties;
	typedef boost::shared_ptr<graphic_format_properties> graphic_format_properties_ptr;
	
	class chart_format_properties;
	typedef boost::shared_ptr<chart_format_properties> chart_format_properties_ptr;

namespace chart {

	enum oox_typeconvert {docx, xlsx, pptx};

	struct simple
	{
		bool bEnabled = false;

		odf_reader::chart_format_properties_ptr properties_; 		
		odf_reader::text_format_properties_ptr text_properties_;
		odf_reader::graphic_format_properties_ptr graphic_properties_;

		oox::_oox_fill fill_;
	};
	struct title : public simple
	{
		std::wstring content_;
		
		double pos_x = 0;
		double pos_y = 0;		
	}; 

	struct treadline
	{
		bool bEquation = false;
		bool bREquation = false;

		graphic_format_properties_ptr graphic_properties_;
		chart_format_properties_ptr properties_;

		simple equation_properties_;
	};
	struct legend : public simple
    {
		std::wstring position;
		std::wstring align;

		// значения в pt
		_CP_OPT(double) x;
		_CP_OPT(double) y;
		_CP_OPT(double) width;
		_CP_OPT(double) height;
	};
	struct plot_area : public simple
    {
		std::wstring cell_range_address_;
		chart_format_properties_ptr	properties_;
		//series_in_column_or_row
		//uses_first_as_label
	};
	struct axis: public simple
    {
        struct grid
        {
            enum grid_type {major, minor};

            grid_type type_ = major;
            std::wstring style_name_;
 			
			graphic_format_properties_ptr graphic_properties_;
		};
		bool bCategories_ = false;

		title title_;
        
		std::wstring		dimension_;
        std::wstring		chart_name_;
        std::wstring		style_name_;
        std::vector<grid>	grids_;
		int					type_ = 3;		
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
		odf_types::chart_class::type class_;
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
				odf_types::chart_class::type classType,
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
