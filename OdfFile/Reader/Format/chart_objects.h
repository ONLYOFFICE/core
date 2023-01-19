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

#include "../../Reader/Converter/oox_drawing_fills.h"
#include "../../DataTypes/chartclass.h"

namespace cpdoccore { 
namespace odf_reader {

	class text_format_properties_content;
	typedef boost::shared_ptr<text_format_properties_content> text_format_properties_content_ptr;

namespace chart {

	enum oox_typeconvert {docx, xlsx, pptx};

	struct simple
	{
		bool					bEnabled = false;
		std::vector<_property>	properties_; 
 		
		text_format_properties_content_ptr	text_properties_;
		std::vector<_property>			graphic_properties_;
		oox::_oox_fill					fill_;
	};
	struct title : public simple
	{
		std::wstring			content_;
		
		double					pos_x = 0;
		double					pos_y = 0;		
	}; 

	struct treadline
	{
		bool					bEquation = false;
		bool					bREquation = false;
		std::vector<_property>	line_properties_;
		simple					equation_properties_;
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
        struct grid
        {
            enum grid_type {major, minor};

            grid_type				type_ = major;
            std::wstring			style_name_;
 			
			std::vector<_property>	graphic_properties_;
       };
		bool				bCategories_ = false;
		
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
