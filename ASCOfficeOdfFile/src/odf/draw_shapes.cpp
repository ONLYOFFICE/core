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

#include "draw_shapes.h"
#include "datatypes/custom_shape_types_convert.h"

#include <ostream>
#include <sstream>
#include <string>

#include <boost/foreach.hpp>
#include <boost/regex.h>
#include <boost/algorithm/string.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/odf/odf_document.h>

#include "serialize_elements.h"
#include "style_graphic_properties.h"

#include "datatypes/length.h"
#include "datatypes/borderstyle.h"

#include "odfcontext.h"
#include "../odf/svg_parser.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

const wchar_t * draw_shape::ns = L"draw";
const wchar_t * draw_shape::name = L"shape";
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// draw-shape-attlist
void draw_shape_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void draw_shape::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void draw_shape::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"draw:id",			draw_id_);//или сюда draw_shape_attlist_???
	
	common_draw_attlists_.shape_with_text_and_styles_.add_attributes(Attributes);
    common_draw_attlists_.position_.add_attributes(Attributes);
    common_draw_attlists_.rel_size_.add_attributes(Attributes);
 	common_presentation_attlist_.add_attributes(Attributes);
   
	draw_shape_attlist_.add_attributes(Attributes);

}

void draw_rect_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:filter-name", draw_filter_name_);
}

const wchar_t * draw_rect::ns = L"draw";
const wchar_t * draw_rect::name = L"rect";

void draw_rect::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_rect_attlist_.add_attributes(Attributes);
	draw_shape::add_attributes(Attributes);

	sub_type_ = 2;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// draw-ellipse-attlist

void draw_ellipse_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:filter-name", draw_filter_name_);
}

/// draw:ellipse
const wchar_t * draw_ellipse::ns = L"draw";
const wchar_t * draw_ellipse::name = L"ellipse";


void draw_ellipse::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_ellipse_attlist_.add_attributes(Attributes);
	draw_shape::add_attributes(Attributes);

	sub_type_ = 3;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// draw-circle-attlist

void draw_circle_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    //CP_APPLY_ATTR(L"draw:filter-name", draw_filter_name_);
}
// draw:circle
const wchar_t * draw_circle::ns = L"draw";
const wchar_t * draw_circle::name = L"circle";

void draw_circle::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_circle_attlist_.add_attributes(Attributes);
	draw_shape::add_attributes(Attributes);
	sub_type_ = 4;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// draw-line-attlist
void draw_line_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"svg:x1", svg_x1_);
    CP_APPLY_ATTR(L"svg:x2", svg_x2_);
    CP_APPLY_ATTR(L"svg:y1", svg_y1_);
    CP_APPLY_ATTR(L"svg:y2", svg_y2_);
}
// draw:line
const wchar_t * draw_line::ns = L"draw";
const wchar_t * draw_line::name = L"line";

void draw_line::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_line_attlist_.add_attributes(Attributes);
	draw_shape::add_attributes(Attributes);
	
	sub_type_ = 5;
}
void draw_line::reset_svg_attributes()
{
	double x1=draw_line_attlist_.svg_x1_.get_value_or(length(0)).get_value_unit(length::pt);
	double y1=draw_line_attlist_.svg_y1_.get_value_or(length(0)).get_value_unit(length::pt);
	double x2=draw_line_attlist_.svg_x2_.get_value_or(length(0)).get_value_unit(length::pt);
	double y2=draw_line_attlist_.svg_y2_.get_value_or(length(0)).get_value_unit(length::pt);
	
	if (x1 > x2)
	{
		common_draw_attlists_.position_.svg_x_	 = draw_line_attlist_.svg_x2_;
		common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_ = length(x1-x2, length::pt);
		
		additional_.push_back(_property(L"flipH",true));
	}else
	{
		common_draw_attlists_.position_.svg_x_	 = draw_line_attlist_.svg_x1_;
		common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_ = length(x2-x1, length::pt);
	}
	if (y1 > y2)
	{
		common_draw_attlists_.position_.svg_y_	 = draw_line_attlist_.svg_y2_;
		common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_ = length(y1-y2, length::pt);

		additional_.push_back(_property(L"flipV",true));
	}else
	{
		common_draw_attlists_.position_.svg_y_	 = draw_line_attlist_.svg_y1_;
		common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_ = length(y2-y1, length::pt);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// draw-custom_shape_attlist
void draw_custom_shape_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
// draw:custom_shape
const wchar_t * draw_custom_shape::ns = L"draw";
const wchar_t * draw_custom_shape::name = L"custom-shape";

void draw_custom_shape::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_custom_shape_attlist_.add_attributes(Attributes);
	draw_shape::add_attributes(Attributes);
	
	sub_type_ = 7;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// draw-path-attlist
void draw_path_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"svg:d", svg_d_);
    CP_APPLY_ATTR(L"svg:viewBox", svg_viewbox_);

}
// draw:path
const wchar_t * draw_path::ns = L"draw";
const wchar_t * draw_path::name = L"path";

void draw_path::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_path_attlist_.add_attributes(Attributes);
	draw_shape::add_attributes(Attributes);

	sub_type_ = 6;
	
}
void draw_path::reset_svg_path()
{
	if (draw_path_attlist_.svg_d_)
	{
		std::vector<svg_path::_polyline> o_Polyline_pt;
		std::vector<svg_path::_polyline> o_Polyline_cm;
	
		bool res = svg_path::parseSvgD(o_Polyline_cm,draw_path_attlist_.svg_d_.get(),false);
		
		BOOST_FOREACH(svg_path::_polyline  & poly, o_Polyline_cm)
		{
			for (long i=0;i<poly.points.size();i++)
			{
				if (poly.points[i].x)
				{
					poly.points[i].x =  length(poly.points[i].x.get()/1000.,length::cm).get_value_unit(length::emu); 
				}
				if (poly.points[i].y)
				{
					poly.points[i].y = length(poly.points[i].y.get()/1000.,length::cm).get_value_unit(length::emu); 
				}
			}
			o_Polyline_pt.push_back(poly);
		}
		if (o_Polyline_pt.size()>0)
		{
			//сформируем xml-oox сдесь ... а то придется плодить массивы в drawing .. хоть и не красиво..
			std::wstringstream output_;   
            svg_path::oox_serialize(output_, o_Polyline_pt);
			additional_.push_back(odf_reader::_property(L"custom_path",output_.str()));
		}
	}
}
///////////////////////////////////////
void draw_polygon_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:points", draw_points_);
    CP_APPLY_ATTR(L"svg:viewBox", svg_viewbox_);

}
// draw:polygon
const wchar_t * draw_polygon::ns = L"draw";
const wchar_t * draw_polygon::name = L"polygon";

// draw:contour-polygon
const wchar_t * draw_contour_polygon::ns = L"draw";
const wchar_t * draw_contour_polygon::name = L"contour-polygon";

void draw_polygon::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_polygon_attlist_.add_attributes(Attributes);
	draw_shape::add_attributes(Attributes);

	sub_type_ = 8;
	
}
void draw_polygon::reset_polygon_path()
{
	if (draw_polygon_attlist_.draw_points_)
	{
		std::vector<svg_path::_polyline> o_Polyline_pt;
		std::vector<svg_path::_polyline> o_Polyline_cm;

		bool res = svg_path::parsePolygon(o_Polyline_cm,draw_polygon_attlist_.draw_points_.get(),false, true);
		
		BOOST_FOREACH(svg_path::_polyline  & poly, o_Polyline_cm)
		{
			for (long i=0;i<poly.points.size();i++)
			{
				if (poly.points[i].x)
				{
					poly.points[i].x =  length(poly.points[i].x.get()/1000.,length::cm).get_value_unit(length::emu); 
				}
				if (poly.points[i].y)
				{
					poly.points[i].y = length(poly.points[i].y.get()/1000.,length::cm).get_value_unit(length::emu); 
				}
			}
			o_Polyline_pt.push_back(poly);
		}
		if (o_Polyline_pt.size()>0)
		{
			//сформируем xml-oox сдесь ... а то придется плодить массивы в drawing .. хоть и не красиво..
			std::wstringstream output_;   
            svg_path::oox_serialize(output_, o_Polyline_pt);
			additional_.push_back(odf_reader::_property(L"custom_path",output_.str()));
		}
	}
}
///////////////////////////////////////
void draw_polyline_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:points", draw_points_);
    CP_APPLY_ATTR(L"svg:viewBox", svg_viewbox_);

}
// draw:polyline
const wchar_t * draw_polyline::ns = L"draw";
const wchar_t * draw_polyline::name = L"polyline";

void draw_polyline::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_polyline_attlist_.add_attributes(Attributes);
	draw_shape::add_attributes(Attributes);

	sub_type_ = 8;
	
}
void draw_polyline::reset_polyline_path()
{
	if (draw_polyline_attlist_.draw_points_)
	{
		std::vector<svg_path::_polyline> o_Polyline_pt;
		std::vector<svg_path::_polyline> o_Polyline_cm;

		bool res = svg_path::parsePolygon(o_Polyline_cm, draw_polyline_attlist_.draw_points_.get(), false, false);

		_CP_OPT(double) start_x, start_y;
		
		BOOST_FOREACH(svg_path::_polyline  & poly, o_Polyline_cm)
		{
			for (long i=0;i<poly.points.size();i++)
			{
				if (poly.points[i].x)
				{
					if (!start_x)//вообщето это не верно .. но из за разных точек осей поворота фигура может "улететь"
						start_x = length(poly.points[i].x.get()/1000.,length::cm).get_value_unit(length::emu); 
					poly.points[i].x =  length(poly.points[i].x.get()/1000.,length::cm).get_value_unit(length::emu);// - *start_x; 
				}
				if (poly.points[i].y)
				{
					if (!start_y)
						start_y = length(poly.points[i].y.get()/1000.,length::cm).get_value_unit(length::emu); 
					poly.points[i].y = length(poly.points[i].y.get()/1000.,length::cm).get_value_unit(length::emu);// - *start_y; 
				}
			}
			o_Polyline_pt.push_back(poly);
		}
		if (o_Polyline_pt.size()>0)
		{
			//сформируем xml-oox сдесь ... а то придется плодить массивы в drawing .. хоть и не красиво..
			std::wstringstream output_;   
            svg_path::oox_serialize(output_, o_Polyline_pt);
			additional_.push_back(odf_reader::_property(L"custom_path",output_.str()));
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void draw_equation_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{	
	CP_APPLY_ATTR(L"draw:name", draw_name_);
	CP_APPLY_ATTR(L"draw:formula", draw_formula_);
}
void draw_handle_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{	
	CP_APPLY_ATTR(L"draw:handle-position", draw_handle_position_);

	CP_APPLY_ATTR(L"draw:handle-range-y-maximum", draw_handle_range_y_maximum_);
	CP_APPLY_ATTR(L"draw:handle-range-y-minimum", draw_handle_range_y_minimum_);

	CP_APPLY_ATTR(L"draw:handle-range-x-maximum", draw_handle_range_x_maximum_);
	CP_APPLY_ATTR(L"draw:handle-range-x-minimum", draw_handle_range_x_minimum_);
	
	CP_APPLY_ATTR(L"draw:handle-radius-range-maximum", draw_handle_radius_range_maximum_);
	CP_APPLY_ATTR(L"draw:handle-radius-range-minimum", draw_handle_radius_range_minimum_);
	
	CP_APPLY_ATTR(L"draw:handle-polar", draw_handle_polar_);
}
// draw:path
const wchar_t * draw_handle::ns = L"draw";
const wchar_t * draw_handle::name = L"handle";

void draw_handle::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_handle_attlist_.add_attributes(Attributes);
}
const wchar_t * draw_equation::ns = L"draw";
const wchar_t * draw_equation::name = L"equation";

void draw_equation::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_equation_attlist_.add_attributes(Attributes);
}
int draw_enhanced_geometry::parsing(_CP_OPT(std::wstring) val) 
{
	int pos=0, res=-1;
	if (!val)return res;

	BOOST_FOREACH(wchar_t c, val.get())
    {
		if (c < L'0' && c > L'9')
			return res;
	}

	if ((pos = val->find(L"$"))>=0)return res;
	res = boost::lexical_cast<int>(val.get());
	return res;
}


/// draw-enhanced_geometry_attlist
void draw_enhanced_geometry_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:type"		, draw_type_);
    CP_APPLY_ATTR(L"draw:modifiers"	, draw_modifiers_);
	CP_APPLY_ATTR(L"draw:text-path"	, draw_text_path_);
}
// draw:enhanced_geometry
const wchar_t * draw_enhanced_geometry::ns = L"draw";
const wchar_t * draw_enhanced_geometry::name = L"enhanced-geometry";

void draw_enhanced_geometry::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_enhanced_geometry_attlist_.add_attributes(Attributes);
    CP_APPLY_ATTR(L"svg:viewBox", svg_viewbox_);
}

typedef shared_ptr<draw_handle> draw_handle_ptr;

void draw_enhanced_geometry::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"draw", L"handle")
    {
        CP_CREATE_ELEMENT(draw_handle_);
	}
	else if CP_CHECK_NAME(L"draw", L"equation")
	{
		CP_CREATE_ELEMENT(draw_equation_);
	}
    else
    {
        not_applicable_element(L"draw_enhanced_geometry", Reader, Ns, Name);
    }

}
void draw_enhanced_geometry::find_draw_type_oox()
{
	word_art_ = false;

	if (draw_enhanced_geometry_attlist_.draw_text_path_ &&
			*draw_enhanced_geometry_attlist_.draw_text_path_ == true)
	{
		draw_type_oox_index_ = 0;
		word_art_ = true;
		sub_type_ = 1;
	}

	if (draw_enhanced_geometry_attlist_.draw_type_)
	{
		std::wstring odf_type = draw_enhanced_geometry_attlist_.draw_type_.get();

		if (word_art_)
		{
			int count = sizeof(_OO_OOX_wordart) / sizeof(_shape_converter);

			for (long i=0; i< count; i++)
			{
				if (_OO_OOX_wordart[i].odf_reader == odf_type)
				{
					draw_type_oox_index_ = i;
					break;
				}
				
			}
		}
		else
		{
			int count = sizeof(_OO_OOX_custom_shapes) / sizeof(_shape_converter);

			for (long i=0; i< count; i++)
			{
				if (_OO_OOX_custom_shapes[i].odf_reader == odf_type)
				{
					draw_type_oox_index_ = i;
					break;
				}
				
			}
			if ((draw_type_oox_index_) && (*draw_type_oox_index_== 179))//L"textBox"
			{
				sub_type_ = 1;//textBox
			}
		}

	}


	std::wstringstream str;

    BOOST_FOREACH(const office_element_ptr & parElement, draw_handle_)
    {
		draw_handle * handle = dynamic_cast<draw_handle *>(parElement.get());
        
		int min = -1;
		int max = -1;
		
		try
		{
			min = parsing(handle->draw_handle_attlist_.draw_handle_range_y_minimum_);//пока статик .. и выдается только цыфровое значение
			if (min<0)min =parsing(handle->draw_handle_attlist_.draw_handle_range_x_minimum_);
			if (min<0)min = parsing(handle->draw_handle_attlist_.draw_handle_radius_range_minimum_);
		}
		catch(...)
		{
		}
		if (min<0)min=0;

		try
		{	
			max = parsing(handle->draw_handle_attlist_.draw_handle_range_y_maximum_);
			if (max<0)max = parsing(handle->draw_handle_attlist_.draw_handle_range_x_maximum_);
			if (max<0)max = parsing(handle->draw_handle_attlist_.draw_handle_radius_range_maximum_);
		}
		catch(...)
		{
		}		
		draw_handle_geometry elm={min, max};
		draw_handle_geometry_.push_back(elm);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// draw-caption-attlist
void draw_caption_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
// draw:caption
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_caption::ns = L"draw";
const wchar_t * draw_caption::name = L"caption";

void draw_caption::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_caption_attlist_.add_attributes(Attributes);
	draw_shape::add_attributes(Attributes);
	
	sub_type_ = 1;	
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// draw-connector-attlist
void draw_connector_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"svg:d", svg_d_);
    CP_APPLY_ATTR(L"svg:viewBox", svg_viewbox_);
    CP_APPLY_ATTR(L"draw:type",draw_type_);

}
// draw:connector
const wchar_t * draw_connector::ns = L"draw";
const wchar_t * draw_connector::name = L"connector";

void draw_connector::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_connector_attlist_.add_attributes(Attributes);
    draw_line_attlist_.add_attributes(Attributes);
	draw_shape::add_attributes(Attributes);

	sub_type_ = 5; //коннектор - линия, если ломаная (ниже определяется) - то путь
	
}
void draw_connector::reset_svg_path()
{
	if (draw_connector_attlist_.svg_d_)
	{
		std::vector<svg_path::_polyline> o_Polyline_pt;
		std::vector<svg_path::_polyline> o_Polyline_cm;
	
		bool res = svg_path::parseSvgD(o_Polyline_cm,draw_connector_attlist_.svg_d_.get(),false);
	
		double x1=common_draw_attlists_.position_.svg_x_.get_value_or(length(0)).get_value_unit(length::emu);
		double y1=common_draw_attlists_.position_.svg_y_.get_value_or(length(0)).get_value_unit(length::emu);
		
		BOOST_FOREACH(svg_path::_polyline  & poly, o_Polyline_cm)
		{
			for (long i=0;i<poly.points.size();i++)
			{
				if (poly.points[i].x)
				{
					poly.points[i].x =  length(poly.points[i].x.get()/1000.,length::cm).get_value_unit(length::emu)-x1; 
				}
				if (poly.points[i].y)
				{
					poly.points[i].y = length(poly.points[i].y.get()/1000.,length::cm).get_value_unit(length::emu)-y1; 
				}
			}
			o_Polyline_pt.push_back(poly);
		}
		if (o_Polyline_pt.size()>0)
		{
			sub_type_ = 6;
			//сформируем xml-oox сдесь ... а то придется плодить массивы в drawing .. хоть и не красиво..
			std::wstringstream output_;   
            svg_path::oox_serialize(output_, o_Polyline_pt);
			additional_.push_back(odf_reader::_property(L"custom_path",output_.str()));
		}
	}
}
///////////////////////////////////////

}
}
