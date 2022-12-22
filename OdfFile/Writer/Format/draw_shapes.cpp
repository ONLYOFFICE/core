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

#include "draw_shapes.h"

#include <ostream>
#include <sstream>
#include <string>

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.h>
#include <boost/lexical_cast.hpp>

#include <xml/xmlchar.h>

#include <xml/attributes.h>

#include "style_graphic_properties.h"

#include "../../DataTypes/length.h"
#include "../../DataTypes/borderstyle.h"
#include "../../DataTypes/custom_shape_types_convert.h"

#include "odf_conversion_context.h"

#include "svg_creator.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

using xml::xml_char_wc;


const wchar_t * draw_shape::ns = L"draw";
const wchar_t * draw_shape::name = L"shape";
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void draw_shape::serialize(std::wostream & _Wostream)
{
	draw_base::serialize(_Wostream);
}
void draw_shape::serialize_attlist(CP_ATTR_NODE)
{
	draw_base::serialize_attlist(CP_GET_XML_NODE());
	common_xlink_attlist_.serialize(CP_GET_XML_NODE());
	
	CP_XML_ATTR_OPT(L"draw:id", draw_id_);
}

void draw_rect_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"draw:filter-name",	draw_filter_name_);
	CP_XML_ATTR_OPT(L"draw:corner-radius",	draw_corner_radius_);
}

const wchar_t * draw_rect::ns = L"draw";
const wchar_t * draw_rect::name = L"rect";

void draw_rect::serialize(std::wostream & _Wostream)
{
	sub_type_ = 2;//todooo перекинуть
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			draw_shape::serialize_attlist(CP_GET_XML_NODE());
	
			draw_rect_attlist_.serialize(CP_GET_XML_NODE());

			draw_shape::serialize(CP_XML_STREAM());
		}
	}
}

//-------------------------------------------------------------------------------------------
/// draw-ellipse-attlist

void draw_ellipse_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"draw:filter-name", draw_filter_name_);
}

//-------------------------------------------------------------------------------------------
/// draw:ellipse
//-------------------------------------------------------------------------------------------
const wchar_t * draw_ellipse::ns = L"draw";
const wchar_t * draw_ellipse::name = L"ellipse";


void draw_ellipse::serialize(std::wostream & _Wostream)
{
	sub_type_ = 3;
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			draw_shape::serialize_attlist(CP_GET_XML_NODE());
	
			draw_ellipse_attlist_.serialize(CP_GET_XML_NODE());

			draw_shape::serialize(CP_XML_STREAM());
		}
	}
}

//-------------------------------------------------------------------------------------------
// draw:circle
//-------------------------------------------------------------------------------------------
const wchar_t * draw_circle::ns = L"draw";
const wchar_t * draw_circle::name = L"circle";

void draw_circle::serialize(std::wostream & _Wostream)
{
	sub_type_ = 4;
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			draw_shape::serialize_attlist(CP_GET_XML_NODE());
	
			draw_shape::serialize(CP_XML_STREAM());
		}
	}
}

//-------------------------------------------------------------------------------------------
/// draw-line-attlist
void draw_line_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"svg:x1", svg_x1_);
    CP_XML_ATTR_OPT(L"svg:x2", svg_x2_);
    CP_XML_ATTR_OPT(L"svg:y1", svg_y1_);
    CP_XML_ATTR_OPT(L"svg:y2", svg_y2_);
}
// draw:line
const wchar_t * draw_line::ns = L"draw";
const wchar_t * draw_line::name = L"line";

void draw_line::serialize(std::wostream & _Wostream)
{
	sub_type_ = 5;
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			draw_shape::serialize_attlist(CP_GET_XML_NODE());
	
			draw_line_attlist_.serialize(CP_GET_XML_NODE());

			draw_shape::serialize(CP_XML_STREAM());
		}
	}
}
//-------------------------------------------------------------------------------------------
// draw:custom_shape
//-------------------------------------------------------------------------------------------
const wchar_t * draw_custom_shape::ns = L"draw";
const wchar_t * draw_custom_shape::name = L"custom-shape";

void draw_custom_shape::serialize(std::wostream & _Wostream)
{
	sub_type_ = 7;
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			draw_shape::serialize_attlist(CP_GET_XML_NODE());

			if (draw_enhanced_geometry_)draw_enhanced_geometry_->serialize(CP_XML_STREAM());
			draw_shape::serialize(CP_XML_STREAM());
		}
	}
}
void draw_custom_shape::add_child_element( const office_element_ptr & child_element)
{
	switch(child_element->get_type())
	{
		case typeDrawEnhancedGeometry:
			draw_enhanced_geometry_ = child_element;
			break;
		default:
			draw_base::add_child_element(child_element);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// draw-path-attlist
void draw_path_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"svg:d", svg_d_);
    CP_XML_ATTR_OPT(L"svg:viewBox", svg_viewbox_);

}
//-------------------------------------------------------------------------------------------
// draw:path
//-------------------------------------------------------------------------------------------
const wchar_t * draw_path::ns = L"draw";
const wchar_t * draw_path::name = L"path";

void draw_path::serialize(std::wostream & _Wostream)
{
	sub_type_ = 6;
	reset_svg_path();

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			draw_shape::serialize_attlist(CP_GET_XML_NODE());
	
			draw_path_attlist_.serialize(CP_GET_XML_NODE());

			draw_shape::serialize(CP_XML_STREAM());
		}
	}	
}
void draw_path::reset_svg_path()//todooo наоборот
{
	//if (draw_path_attlist_.svg_d_)
	//{
	//	std::vector<svg_path::_polyline> o_Polyline_pt;
	//	std::vector<svg_path::_polyline> o_Polyline_cm;
	//
	//	bool res = svg_path::createSvgD(o_Polyline_cm,draw_path_attlist_.svg_d_.get(),false);
	//	
	//	BOOST_FOREACH(svg_path::_polyline  & poly, o_Polyline_cm)
	//	{
	//		for (long i=0;i<poly.points.size();i++)
	//		{
	//			if (poly.points[i].x)
	//			{
	//				poly.points[i].x =  length(poly.points[i].x.get()/1000.,length::cm).get_value_unit(length::emu); 
	//			}
	//			if (poly.points[i].y)
	//			{
	//				poly.points[i].y = length(poly.points[i].y.get()/1000.,length::cm).get_value_unit(length::emu); 
	//			}
	//		}
	//		o_Polyline_pt.push_back(poly);
	//	}
	//	if (o_Polyline_pt.size()>0)
	//	{
	//		//сформируем xml-oox сдесь ... а то придется плодить массивы в drawing .. хоть и не красиво..
	//		std::wstringstream output_;   
	//		//svg_path::oox_serialize(output_, o_Polyline_pt);
	//		additional_.push_back(odf_writer::_property(L"custom_path",output_.str()));
	//	}
	//}
}
///////////////////////////////////////
void draw_polygon_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"draw:points", draw_points_);
    CP_XML_ATTR_OPT(L"svg:viewBox", svg_viewbox_);

}
//-------------------------------------------------------------------------------------------
// draw:polygon
//-------------------------------------------------------------------------------------------
const wchar_t * draw_polygon::ns = L"draw";
const wchar_t * draw_polygon::name = L"polygon";

void draw_polygon::serialize(std::wostream & _Wostream)
{
	sub_type_ = 8;
	reset_polygon_path();

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			draw_shape::serialize_attlist(CP_GET_XML_NODE());
	
			draw_polygon_attlist_.serialize(CP_GET_XML_NODE());

			draw_shape::serialize(CP_XML_STREAM());
		}
	}	
}
void draw_polygon::reset_polygon_path()
{
	//if (draw_polygon_attlist_.draw_points_)
	//{
	//	std::vector<svg_path::_polyline> o_Polyline_pt;
	//	std::vector<svg_path::_polyline> o_Polyline_cm;

	//	bool res = svg_path::createPolygon(o_Polyline_cm,draw_polygon_attlist_.draw_points_.get(),false);
	//	
	//	BOOST_FOREACH(svg_path::_polyline  & poly, o_Polyline_cm)
	//	{
	//		for (long i=0;i<poly.points.size();i++)
	//		{
	//			if (poly.points[i].x)
	//			{
	//				poly.points[i].x =  length(poly.points[i].x.get()/1000.,length::cm).get_value_unit(length::emu); 
	//			}
	//			if (poly.points[i].y)
	//			{
	//				poly.points[i].y = length(poly.points[i].y.get()/1000.,length::cm).get_value_unit(length::emu); 
	//			}
	//		}
	//		o_Polyline_pt.push_back(poly);
	//	}
	//	if (o_Polyline_pt.size()>0)
	//	{
	//		//сформируем xml-oox сдесь ... а то придется плодить массивы в drawing .. хоть и не красиво..
	//		std::wstringstream output_;   
	//		svg_path::oox_serialize(output_, o_Polyline_pt);
	//		additional_.push_back(odf_writer::_property(L"custom_path",output_.str()));
	//	}
	//}
}
//-------------------------------------------------------------------------------------------
void draw_equation_attlist::serialize(CP_ATTR_NODE)
{	
	CP_XML_ATTR_OPT_ENCODE_STRING(L"draw:name", draw_name_);
	CP_XML_ATTR_OPT(L"draw:formula", draw_formula_);
}
void draw_handle_attlist::serialize(CP_ATTR_NODE)
{	
	CP_XML_ATTR_OPT(L"draw:handle-position", draw_handle_position_);

	CP_XML_ATTR_OPT(L"draw:handle-range-y-maximum", draw_handle_range_y_maximum_);
	CP_XML_ATTR_OPT(L"draw:handle-range-y-minimum", draw_handle_range_y_minimum_);

	CP_XML_ATTR_OPT(L"draw:handle-range-x-maximum", draw_handle_range_x_maximum_);
	CP_XML_ATTR_OPT(L"draw:handle-range-x-minimum", draw_handle_range_x_minimum_);
	
	CP_XML_ATTR_OPT(L"draw:handle-radius-range-maximum", draw_handle_radius_range_maximum_);
	CP_XML_ATTR_OPT(L"draw:handle-radius-range-minimum", draw_handle_radius_range_minimum_);
	
	CP_XML_ATTR_OPT(L"draw:handle-polar", draw_handle_polar_);
}
//-------------------------------------------------------------------------------------------
// draw:path
//-------------------------------------------------------------------------------------------
const wchar_t * draw_handle::ns = L"draw";
const wchar_t * draw_handle::name = L"handle";

void draw_handle::serialize(std::wostream & _Wostream)
{
  	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			draw_handle_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}
const wchar_t * draw_equation::ns = L"draw";
const wchar_t * draw_equation::name = L"equation";

void draw_equation::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			draw_equation_attlist_.serialize(CP_GET_XML_NODE());
		}
	}	
}
int draw_enhanced_geometry::parsing(_CP_OPT(std::wstring) val) 
{
	int res = -1;
	if (!val) return res;

	BOOST_FOREACH(wchar_t c, val.get())
    {
		if (c < L'0' && c > L'9')
			return res;
	}

	if (std::wstring::npos != val->find(L"$"))
		return res;

	res = boost::lexical_cast<int>(val.get());
	return res;
}


//-------------------------------------------------------------------------------------------
/// draw-enhanced_geometry_attlist
void draw_enhanced_geometry_attlist::serialize(CP_ATTR_NODE)
{
	if (draw_modifiers_ && draw_modifiers_->empty())
	{
		draw_modifiers_ = boost::none;
	}
    CP_XML_ATTR_OPT(L"draw:type",				draw_type_);
	CP_XML_ATTR_OPT(L"drawooo:sub-view-size",	draw_sub_view_size_);
	CP_XML_ATTR_OPT(L"draw:text-areas",			draw_text_areas_);
	CP_XML_ATTR_OPT(L"draw:text-rotate-angle",	draw_text_rotate_angle_);
    CP_XML_ATTR_OPT(L"draw:modifiers",			draw_modifiers_);
	//CP_XML_ATTR_OPT(L"drawooo:enhanced-path",	draw_enhanced_path_);	
    CP_XML_ATTR_OPT(L"draw:enhanced-path",		draw_enhanced_path_);	
	CP_XML_ATTR_OPT(L"draw:glue-points",		draw_glue_points_);
	CP_XML_ATTR_OPT(L"draw:mirror-vertical",	draw_mirror_vertical_);
	CP_XML_ATTR_OPT(L"draw:mirror-horizontal",	draw_mirror_horizontal_);

	CP_XML_ATTR_OPT(L"draw:text-path",						draw_text_path_);
	CP_XML_ATTR_OPT(L"draw:text-path-mode",					draw_text_path_mode_);
	CP_XML_ATTR_OPT(L"draw:text-path-scale",				draw_text_path_scale_);
	CP_XML_ATTR_OPT(L"draw:text-path-same-letter-heights",	draw_text_path_same_letter_heights_);
}
//-------------------------------------------------------------------------------------------
// draw:enhanced_geometry
//-------------------------------------------------------------------------------------------
const wchar_t * draw_enhanced_geometry::ns = L"draw";
const wchar_t * draw_enhanced_geometry::name = L"enhanced-geometry";

void draw_enhanced_geometry::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"svg:viewBox", svg_viewbox_);
			attlist_.serialize(CP_GET_XML_NODE());
			
			for (size_t i = 0; i < draw_equation_.size(); i++)
			{
				draw_equation_[i]->serialize(CP_XML_STREAM());
			}		
			
			for (size_t i = 0; i < draw_handle_.size(); i++)
			{
				draw_handle_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}

typedef shared_ptr<draw_handle> draw_handle_ptr;

void draw_enhanced_geometry::create_child_element( const std::wstring & Ns, const std::wstring & Name)
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
        //not_applicable_element(L"draw_enhanced_geometry", Reader, Ns, Name);
    }

}
void draw_enhanced_geometry::add_child_element( const office_element_ptr & child_element)
{
 	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeDrawHandle)
	{
		draw_handle_.push_back(child_element);
	}
	else if (type_ == typeDrawEquation)
	{
		draw_equation_.push_back(child_element);
	}
    else
    {
        //not_applicable_element(L"draw_enhanced_geometry", Reader, Ns, Name);
    }

}
//-------------------------------------------------------------------------------------------
// draw:caption
//-------------------------------------------------------------------------------------------
const wchar_t * draw_caption::ns = L"draw";
const wchar_t * draw_caption::name = L"caption";

void draw_caption::serialize(std::wostream & _Wostream)
{
	sub_type_ = 1;	

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			draw_shape::serialize_attlist(CP_GET_XML_NODE());
	
			draw_shape::serialize(CP_XML_STREAM());
		}
	}
}
//-------------------------------------------------------------------------------------------
/// draw-connector-attlist
void draw_connector_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"svg:d", svg_d_);
    CP_XML_ATTR_OPT(L"svg:viewBox", svg_viewbox_);
    CP_XML_ATTR_OPT(L"draw:type",draw_type_);

}
//-------------------------------------------------------------------------------------------
// draw:connector
//-------------------------------------------------------------------------------------------
const wchar_t * draw_connector::ns = L"draw";
const wchar_t * draw_connector::name = L"connector";

void draw_connector::serialize(std::wostream & _Wostream)
{
	sub_type_ = 9; //коннектор - линия, если ломаная (ниже определяется) - то путь

	reset_svg_path();

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			draw_shape::serialize_attlist(CP_GET_XML_NODE());
			
			draw_connector_attlist_.serialize(CP_GET_XML_NODE());
			draw_line_attlist_.serialize(CP_GET_XML_NODE());
	
			draw_shape::serialize(CP_XML_STREAM());
		}
	}	
}
void draw_connector::reset_svg_path()
{
	//if (draw_connector_attlist_.svg_d_)
	//{
	//	std::vector<svg_path::_polyline> o_Polyline_pt;
	//	std::vector<svg_path::_polyline> o_Polyline_cm;
	//
	//	bool res = svg_path::createSvgD(o_Polyline_cm,draw_connector_attlist_.svg_d_.get(),false);
	//
	//	double x1=common_draw_attlists_.position_.svg_x_.get_value_or(length(0)).get_value_unit(length::emu);
	//	double y1=common_draw_attlists_.position_.svg_y_.get_value_or(length(0)).get_value_unit(length::emu);
	//	
	//	BOOST_FOREACH(svg_path::_polyline  & poly, o_Polyline_cm)
	//	{
	//		for (long i=0;i<poly.points.size();i++)
	//		{
	//			if (poly.points[i].x)
	//			{
	//				poly.points[i].x =  length(poly.points[i].x.get()/1000.,length::cm).get_value_unit(length::emu)-x1; 
	//			}
	//			if (poly.points[i].y)
	//			{
	//				poly.points[i].y = length(poly.points[i].y.get()/1000.,length::cm).get_value_unit(length::emu)-y1; 
	//			}
	//		}
	//		o_Polyline_pt.push_back(poly);
	//	}
	//	if (o_Polyline_pt.size()>0)
	//	{
	//		sub_type_ = 6;
	//		//сформируем xml-oox сдесь ... а то придется плодить массивы в drawing .. хоть и не красиво..
	//		std::wstringstream output_;   
	//		svg_path::oox_serialize(output_, o_Polyline_pt);
	//		additional_.push_back(odf_writer::_property(L"custom_path",output_.str()));
	//	}
	//}
}
//-------------------------------------------------------------------------------------------
// draw:control
//-------------------------------------------------------------------------------------------
const wchar_t * draw_control::ns = L"draw";
const wchar_t * draw_control::name = L"control";

void draw_control::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"xml:id", xml_id_);
			CP_XML_ATTR_OPT(L"draw:caption-id", caption_id_);
			CP_XML_ATTR_OPT(L"draw:control", control_id_);

			draw_shape::serialize_attlist(CP_GET_XML_NODE());
	
			draw_shape::serialize(CP_XML_STREAM());
			if (draw_glue_point_)
				draw_glue_point_->serialize(CP_XML_STREAM());
		}
	}
}
void draw_control::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"draw", L"glue-point")
    {
        CP_CREATE_ELEMENT(draw_glue_point_);
	}
    else
    {
        //not_applicable_element(L"draw_enhanced_geometry", Reader, Ns, Name);
    }

}
void draw_control::add_child_element( const office_element_ptr & child_element)
{
 	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeDrawGluePoint)
	{
		draw_glue_point_ = child_element;
	}
    else
    {
        //not_applicable_element(L"draw_enhanced_geometry", Reader, Ns, Name);
    }

}
//-------------------------------------------------------------------------------------------
// dr3d:scene
//-------------------------------------------------------------------------------------------
const wchar_t * dr3d_scene::ns = L"dr3d";
const wchar_t * dr3d_scene::name = L"scene";

void dr3d_scene::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			draw_shape::serialize_attlist(CP_GET_XML_NODE());
			dr3d_attlist_.serialize(CP_GET_XML_NODE());
			
			draw_shape::serialize(CP_XML_STREAM());

			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
void dr3d_scene::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void dr3d_scene::add_child_element( const office_element_ptr & child_element)
{
 	if (!child_element) return;
	
	content_.push_back(child_element);
}

//-------------------------------------------------------------------------------------------
// dr3d:extrude
//-------------------------------------------------------------------------------------------
const wchar_t * dr3d_extrude::ns = L"dr3d";
const wchar_t * dr3d_extrude::name = L"extrude";

void dr3d_extrude::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {	
			draw_path::serialize(CP_XML_STREAM());
		}
	}
}

//-------------------------------------------------------------------------------------------
// dr3d:rotate
//-------------------------------------------------------------------------------------------
const wchar_t * dr3d_rotate::ns = L"dr3d";
const wchar_t * dr3d_rotate::name = L"rotate";

void dr3d_rotate::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {	
			draw_path::serialize(CP_XML_STREAM());
		}
	}
}
//-------------------------------------------------------------------------------------------
// dr3d:light
//-------------------------------------------------------------------------------------------
const wchar_t * dr3d_light::ns = L"dr3d";
const wchar_t * dr3d_light::name = L"light";

void dr3d_light::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {	
			CP_XML_ATTR_OPT(L"dr3d:diffuse_color",	dr3d_diffuse_color_);
			CP_XML_ATTR_OPT(L"dr3d:direction",		dr3d_direction_);
			CP_XML_ATTR_OPT(L"dr3d:specular",		dr3d_specular_);
			CP_XML_ATTR_OPT(L"dr3d:enabled",		dr3d_enabled_);
		}
	}
}
//-------------------------------------------------------------------------------------------
// dr3d:cube
//-------------------------------------------------------------------------------------------
const wchar_t * dr3d_cube::ns = L"dr3d";
const wchar_t * dr3d_cube::name = L"cube";

void dr3d_cube::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {	
			draw_shape::serialize_attlist(CP_GET_XML_NODE());			

			CP_XML_ATTR_OPT(L"dr3d:max-edge",	dr3d_max_edge_);
			CP_XML_ATTR_OPT(L"dr3d:min-edge",	dr3d_min_edge_);
			CP_XML_ATTR_OPT(L"dr3d:transform",	dr3d_transform_);
			
			draw_shape::serialize(CP_XML_STREAM());
		}
	}
}
//-------------------------------------------------------------------------------------------
// dr3d:sphere
//-------------------------------------------------------------------------------------------
const wchar_t * dr3d_sphere::ns = L"dr3d";
const wchar_t * dr3d_sphere::name = L"sphere";

void dr3d_sphere::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {	
			draw_shape::serialize_attlist(CP_GET_XML_NODE());			

			CP_XML_ATTR_OPT(L"dr3d:max-edge",	dr3d_size_);
			CP_XML_ATTR_OPT(L"dr3d:min-edge",	dr3d_center_);
			CP_XML_ATTR_OPT(L"dr3d:transform",	dr3d_transform_);

			draw_shape::serialize(CP_XML_STREAM());
		}
	}
}
//-------------------------------------------------------------------------------------------
const wchar_t * draw_a::ns = L"draw";
const wchar_t * draw_a::name = L"a";
//-------------------------------------------------------------------------------------------
void draw_a::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void draw_a::add_child_element( const office_element_ptr & child_element)
{
 	if (!child_element) return;
	
	content_.push_back(child_element);
}
void draw_a::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			xlink_attlist_.serialize(CP_GET_XML_NODE());

			CP_XML_ATTR_OPT(L"office:name",				office_name_);
			CP_XML_ATTR_OPT(L"text:style-name",			text_style_name_);
			CP_XML_ATTR_OPT(L"text:visited-style-name", text_visited_style_name_);
			CP_XML_ATTR_OPT(L"office:target-frame-name",office_target_frame_name_);

			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
}
}