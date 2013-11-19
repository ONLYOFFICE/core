#pragma once
#include <cpdoccore/xml/attributes.h>

//описание всяких графических объектов ( вложенная мелочевка) которые используются в xlsx & odf - 

namespace cpdoccore { 
namespace odf {

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
		std::wstring content_;
		std::vector<_property> text_properties_;
		
		double pos_x;
		double pos_y;
		
	} ; 
	struct simple
	{
		std::vector<_property> properties_; 
 		std::vector<_property> text_properties_;
		std::vector<_property> graphic_properties_;
	};
	struct treadline
	{
		bool bEquation;
		bool bREquation;
		std::vector<_property> line_properties_;
		simple equation_properties_;

		treadline(){bEquation = false;bREquation = false;}

	};
    struct axis
    {
        struct grid
        {
            enum grid_type {major, minor};

            grid_type type_;
            std::wstring style_name_;
 			
			std::vector<_property> graphic_properties_;
       };

		title title_;
        
		std::wstring dimension_;
        std::wstring chart_name_;
        std::wstring style_name_;
        std::vector<grid> grids_;
		
		std::vector<_property> properties_; 
 		std::vector<_property> text_properties_;
		std::vector<_property> graphic_properties_;
  };
    struct series
    {
        struct point
        {
            point(unsigned int rep, std::wstring const & styleName) : repeated_(rep), style_name_(styleName)
            {}

            unsigned int repeated_;
            std::wstring style_name_;
        };

        std::wstring name_;
		std::wstring cell_range_address_;

        std::wstring label_cell_address_;
        class_type class_;
        std::wstring attached_axis_;
        std::wstring style_name_;  
        std::vector<point> points_;
		
		simple mean_value_;
		simple error_indicator_;
		treadline regression_curve_;

		std::vector<_property> properties_; 
 		std::vector<_property> text_properties_;
		std::vector<_property> graphic_properties_;

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