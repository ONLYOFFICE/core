/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "../../DataTypes/custom_shape_types_convert.h"

#include <ostream>
#include <sstream>
#include <string>

#include <boost/regex.h>
#include <boost/algorithm/string.hpp>

#include <xml/xmlchar.h>
#include "odf_document.h"

#include "serialize_elements.h"
#include "style_graphic_properties.h"

#include "odfcontext.h"

#include "../../DataTypes/length.h"
#include "../../DataTypes/borderstyle.h"

#include "../../Reader/Converter/oox_drawing.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

const wchar_t * draw_shape::ns = L"draw";
const wchar_t * draw_shape::name = L"shape";

/// draw-shape-attlist
void draw_shape_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void draw_shape::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"draw", L"enhanced-geometry")
	{
		CP_CREATE_ELEMENT(enhanced_geometry_);
		draw_enhanced_geometry* enhanced_geometry = dynamic_cast<draw_enhanced_geometry*>(enhanced_geometry_.get());
		if (enhanced_geometry)
		{
			enhanced_geometry->owner_shape = this;
		}
	}
	else
	{
		CP_CREATE_ELEMENT(content_);
	}
}
void draw_shape::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"draw:id", draw_id_);//или сюда draw_shape_attlist_???
	
	common_draw_attlists_.shape_with_text_and_styles_.add_attributes(Attributes);
    common_draw_attlists_.position_.add_attributes(Attributes);
    common_draw_attlists_.rel_size_.add_attributes(Attributes);
   
	draw_shape_attlist_.add_attributes(Attributes);

}

void draw_rect_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:filter-name", draw_filter_name_);
	CP_APPLY_ATTR(L"draw:corner-radius",draw_corner_radius_);
}
//-------------------------------------------------------------------------------------------
/// draw:rect
//-------------------------------------------------------------------------------------------
const wchar_t * draw_rect::ns = L"draw";
const wchar_t * draw_rect::name = L"rect";

void draw_rect::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_rect_attlist_.add_attributes(Attributes);
	draw_shape::add_attributes(Attributes);

	sub_type_ = 2;

	if (draw_rect_attlist_.draw_corner_radius_)
		sub_type_ = 9;
}

/// draw-ellipse-attlist

void draw_ellipse_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:filter-name", draw_filter_name_);
}

//-------------------------------------------------------------------------------------------
/// draw:ellipse
//-------------------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------------
// draw:circle
//-------------------------------------------------------------------------------------------
const wchar_t * draw_circle::ns = L"draw";
const wchar_t * draw_circle::name = L"circle";

void draw_circle::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_circle_attlist_.add_attributes(Attributes);
	draw_shape::add_attributes(Attributes);
	sub_type_ = 4;
}

/// draw-line-attlist
void draw_line_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"svg:x1", svg_x1_);
    CP_APPLY_ATTR(L"svg:x2", svg_x2_);
    CP_APPLY_ATTR(L"svg:y1", svg_y1_);
    CP_APPLY_ATTR(L"svg:y2", svg_y2_);
}
//-------------------------------------------------------------------------------------------
// draw:line
//-------------------------------------------------------------------------------------------
const wchar_t * draw_line::ns = L"draw";
const wchar_t * draw_line::name = L"line";

void draw_line::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_line_attlist_.add_attributes(Attributes);
	draw_shape::add_attributes(Attributes);
	
	sub_type_ = 5;
	lined_shape_ = true;
}
void draw_line::reset_svg_attributes()
{
	double x1 = draw_line_attlist_.svg_x1_.get_value_or(length(0)).get_value_unit(length::pt);
	double y1 = draw_line_attlist_.svg_y1_.get_value_or(length(0)).get_value_unit(length::pt);
	double x2 = draw_line_attlist_.svg_x2_.get_value_or(length(0)).get_value_unit(length::pt);
	double y2 = draw_line_attlist_.svg_y2_.get_value_or(length(0)).get_value_unit(length::pt);
	
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
//-------------------------------------------------------------------------------------------
// draw:custom_shape
//-------------------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------------
// draw:path
//-------------------------------------------------------------------------------------------
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
	if (!draw_path_attlist_.svg_d_)
	{
		bad_shape_ = true;
	}
	else
	{
		std::vector<::svg_path::_polyline> o_Polyline_pt;
		std::vector<::svg_path::_polyline> o_Polyline_cm;
	
		bool bClosed = false, bStroked = true;
		bool res = ::svg_path::parseSvgD(o_Polyline_cm, draw_path_attlist_.svg_d_.get(), false, bClosed, bStroked);
		
		if (!bClosed) lined_shape_ = true;

		for (size_t i = 0; i < o_Polyline_cm.size(); i++)
		{
			::svg_path::_polyline  & poly = o_Polyline_cm[i];

			for (size_t i = 0; i < poly.points.size(); i++)
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
		if (false == o_Polyline_pt.empty())
		{
			//сформируем xml-oox сдесь ... а то придется плодить массивы в drawing .. хоть и не красиво..
			std::wstringstream output_;   
            ::svg_path::oox_serialize(output_, o_Polyline_pt);
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
//-------------------------------------------------------------------------------------------
// draw:contour-polygon
//-------------------------------------------------------------------------------------------
const wchar_t * draw_contour_polygon::ns = L"draw";
const wchar_t * draw_contour_polygon::name = L"contour-polygon";

//-------------------------------------------------------------------------------------------
// draw:contour-path
//-------------------------------------------------------------------------------------------
const wchar_t * draw_contour_path::ns = L"draw";
const wchar_t * draw_contour_path::name = L"contour-path";

//-------------------------------------------------------------------------------------------
// draw:polygon
//-------------------------------------------------------------------------------------------
const wchar_t * draw_polygon::ns = L"draw";
const wchar_t * draw_polygon::name = L"polygon";

void draw_polygon::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_polygon_attlist_.add_attributes(Attributes);
	draw_shape::add_attributes(Attributes);

	sub_type_ = 8;
	
}
void draw_polygon::reset_polygon_path()
{
	if (!draw_polygon_attlist_.draw_points_)
	{
		bad_shape_ = true;
	}
	else
	{
		std::vector<::svg_path::_polyline> o_Polyline_pt;
		std::vector<::svg_path::_polyline> o_Polyline_cm;

		bool res = ::svg_path::parsePolygon(o_Polyline_cm, draw_polygon_attlist_.draw_points_.get(), false, true);
		
		for (size_t ind = 0 ; ind < o_Polyline_cm.size(); ind++)
		{
			::svg_path::_polyline  & poly = o_Polyline_cm[ind];
			
			for (size_t i = 0; i < poly.points.size(); i++)
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
		if (false == o_Polyline_pt.empty())
		{
			//сформируем xml-oox сдесь ... а то придется плодить массивы в drawing .. хоть и не красиво..
			std::wstringstream output_;   
            ::svg_path::oox_serialize(output_, o_Polyline_pt);
			additional_.push_back(odf_reader::_property(L"custom_path", output_.str()));
		}
	}
}
///////////////////////////////////////
void draw_polyline_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:points", draw_points_);
    CP_APPLY_ATTR(L"svg:viewBox", svg_viewbox_);

}
//-------------------------------------------------------------------------------------------
// draw:polyline
//-------------------------------------------------------------------------------------------
const wchar_t * draw_polyline::ns = L"draw";
const wchar_t * draw_polyline::name = L"polyline";

void draw_polyline::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_polyline_attlist_.add_attributes(Attributes);
	draw_shape::add_attributes(Attributes);

	sub_type_ = 14;
	lined_shape_ = true;
	
}
void draw_polyline::reset_polyline_path()
{
	if (!draw_polyline_attlist_.draw_points_)
	{
		bad_shape_ = true;
	}
	else
	{
		std::vector<::svg_path::_polyline> o_Polyline_pt;
		std::vector<::svg_path::_polyline> o_Polyline_cm;

		bool res = ::svg_path::parsePolygon(o_Polyline_cm, draw_polyline_attlist_.draw_points_.get(), false, false);

		_CP_OPT(double) start_x, start_y;
		
		for (size_t ind = 0 ; ind < o_Polyline_cm.size(); ind++)
		{
			::svg_path::_polyline  & poly = o_Polyline_cm[ind];
			
			for (size_t i = 0; i < poly.points.size(); i++)
			{
				if (poly.points[i].x)
				{
					if (!start_x)//вообщето это не верно .. но из за разных точек осей поворота фигура может "улететь"
						start_x = length(poly.points[i].x.get()/1000., length::cm).get_value_unit(length::emu); 
					poly.points[i].x =  length(poly.points[i].x.get()/1000., length::cm).get_value_unit(length::emu);// - *start_x; 
				}
				if (poly.points[i].y)
				{
					if (!start_y)
						start_y = length(poly.points[i].y.get()/1000., length::cm).get_value_unit(length::emu); 
					poly.points[i].y = length(poly.points[i].y.get()/1000., length::cm).get_value_unit(length::emu);// - *start_y; 
				}
			}
			o_Polyline_pt.push_back(poly);
		}
		if (false == o_Polyline_pt.empty())
		{
			//сформируем xml-oox сдесь ... а то придется плодить массивы в drawing .. хоть и не красиво..
			std::wstringstream output_;   
            
			::svg_path::oox_serialize(output_, o_Polyline_pt);
			additional_.push_back(odf_reader::_property(L"custom_path", output_.str()));
		}
	}
}
//-------------------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------------
// draw:path
//-------------------------------------------------------------------------------------------
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
    attlist_.add_attributes(Attributes);
}
int draw_enhanced_geometry::parsing(_CP_OPT(std::wstring) val) 
{
	int pos = 0, res = -1;
	if (!val) return res;

	for (size_t i = 0; i < val->length(); i++)
    {
		if (val->at(i) < L'0' && val->at(i) > L'9')
			return res;
	}

	if ((pos = val->find(L"$"))>=0)return res;
	res = boost::lexical_cast<int>(val.get());
	return res;
}


/// draw-enhanced_geometry_attlist
void draw_enhanced_geometry_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:type"				, draw_type_);
    CP_APPLY_ATTR(L"draw:modifiers"			, draw_modifiers_);
	CP_APPLY_ATTR(L"draw:text-path"			, draw_text_path_);
	CP_APPLY_ATTR(L"draw:enhanced-path"		, draw_enhanced_path_);
	CP_APPLY_ATTR(L"drawooo:enhanced-path"	, drawooo_enhanced_path_);
	CP_APPLY_ATTR(L"drawooo:sub-view-size"	, drawooo_sub_view_size_);
	CP_APPLY_ATTR(L"draw:mirror-horizontal"	, draw_mirror_horizontal_);
	CP_APPLY_ATTR(L"draw:mirror-vertical"	, draw_mirror_vertical_);
}
//-------------------------------------------------------------------------------------------
// draw:enhanced_geometry
//-------------------------------------------------------------------------------------------
const wchar_t * draw_enhanced_geometry::ns = L"draw";
const wchar_t * draw_enhanced_geometry::name = L"enhanced-geometry";

void draw_enhanced_geometry::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    attlist_.add_attributes(Attributes);
    CP_APPLY_ATTR(L"svg:viewBox", svg_viewbox_);
}

typedef shared_ptr<draw_handle> draw_handle_ptr;

void draw_enhanced_geometry::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"draw", L"handle")
    {
        CP_CREATE_ELEMENT(draw_handles_);
	}
	else if CP_CHECK_NAME(L"draw", L"equation")
	{
		CP_CREATE_ELEMENT(draw_equations_);
	}
    else
    {
        not_applicable_element(L"draw_enhanced_geometry", Reader, Ns, Name);
    }

}
std::wstring convert_equation(const std::wstring& formula)
{
	std::wstring result;
	std::wstring operators;
	std::vector<std::wstring> values;

	size_t pos = 0;
	bool operator_prev = false;
	while (pos < formula.size())
	{
		if ((formula[pos] == L'+' || formula[pos] == L'/' || formula[pos] == L'*' || formula[pos] == L'-') 
			&& pos > 0 && !operator_prev)
		{
			if (operators.size() > 1)
				return L"";
			operator_prev = true;
			operators += formula[pos++];
		}
		else if (formula[pos] == L'i' && formula[pos + 1] == L'f')
		{
			if (false == operators.empty())
				return L"";
			operator_prev = true;
			operators += L"?:"; pos += 2;
		}
		else if (formula[pos] == L'?')
		{
			operator_prev = false;
			values.emplace_back();
			values.back() = L"gd"; pos += 2;
			while (pos < formula.size() && formula[pos] >= L'0' && formula[pos] <= L'9')
			{
				values.back() += formula[pos++];
			}
		}
		else if (formula[pos] == L'c')
		{
			if (false == operators.empty())
				return L"";
			operator_prev = true;
			operators += L"cos";  pos += 3;
		}
		else if (formula[pos] == L'b')
		{//bottom
			operator_prev = false;
			values.emplace_back();
			values.back() = L"h";  pos += 6;
		}
		else if (formula[pos] == L't')
		{//top
			operator_prev = false;
			values.emplace_back();
			values.back() = L"0";  pos += 3;
		}
		else if (formula[pos] == L'r')
		{//right
			operator_prev = false;
			values.emplace_back();
			values.back() = L"w";  pos += 5;
		}
		else if (formula[pos] == L's')
		{
			if (false == operators.empty())
				return L"";
			operator_prev = true;
			if (pos + 2 < formula.size() && formula[pos + 1] == L'i')
			{
				operators = L"sin"; pos += 3;
			}
			else if (pos + 3 < formula.size() && formula[pos + 1] == L'q')
			{
				operators = L"sqrt"; pos += 4;
			}
		}
		else if (formula[pos] == L'm')
		{
			if (false == operators.empty())
				return L"";
			operator_prev = true;
			if (pos + 2 < formula.size() && formula[pos + 1] == L'a')
			{
				operators = L"max"; pos += 3;
			}
			else if (pos + 2 < formula.size() && formula[pos + 1] == L'i')
			{
				operators = L"min"; pos += 3;
			}
			else pos++;			
		}
		else if (formula[pos] == L'l')
		{
			operator_prev = false;
			if (pos + 8 < formula.size() && formula[pos + 1] == L'o' && formula[pos + 2] == L'g')
			{
				if (formula[pos + 3] == L'w')
				{
					values.emplace_back();
					values.back() = L"w"; pos += 8;
				}
				else
				{
					values.emplace_back();
					values.back() = L"h"; pos += 9;
				}
			}
			else if (pos + 3 < formula.size() && formula[pos + 1] == L'e' && formula[pos + 2] == L'f')
			{//left
				values.emplace_back();
				values.back() = L"0";  pos += 4;				
			}
			else pos++;
		}
		else if (formula[pos] == L'$')
		{
			operator_prev = false;

			if (pos + 1 < formula.size() && formula[pos + 1] >= L'0' && formula[pos + 1] <= L'9')
			{
				std::wstring strVal = formula.substr(pos + 1, 1);
				int adj = XmlUtils::GetInteger(strVal);

				values.emplace_back();
				values.back() = L"adj" + std::to_wstring(adj + 1);
			}
			pos += 2;
		}
		else if (formula[pos] >= L'0' && formula[pos] <= L'9' || formula[pos] == L'-')
		{
			operator_prev = false;

			values.emplace_back();
			size_t pos_start = pos;
			while (pos < formula.size() && formula[pos] >= L'0' && formula[pos] <= L'9' || (formula[pos] == L'-' && pos_start == pos))
			{
				values.back() += formula[pos++];
			}
		}
		else pos++;
	}

	
	if (operators.empty())
	{
		result = L"val";
	}
	else if (operators.size() < 2)
	{
		return L"";
	}
	else
	{
		result = operators;
	}

	for (int i = 0; i < values.size(); ++i)
	{
		result += L" " + values[i];
	}
	return result;
}
void draw_enhanced_geometry::find_draw_type_oox()
{
	word_art_	= false;
	bOoxType_	= false;
	
	if (attlist_.draw_text_path_ && *attlist_.draw_text_path_ == true)
	{
		draw_type_oox_index_ = 0;
		word_art_ = true;
		sub_type_ = 1;
	}

	if (attlist_.draw_type_)
	{
		std::wstring odf_type = attlist_.draw_type_.get();

		if (word_art_)
		{
			int count = sizeof(_OO_OOX_wordart) / sizeof(_shape_converter);

			for (int i = 0; i < count; i++)
			{
				if (_OO_OOX_wordart[i].odf_reader == odf_type)
				{
					word_art_ = true;
					draw_type_oox_index_ = i;
					break;
				}
			}
		}
		if (!draw_type_oox_index_)
		{
			int count = sizeof(_OO_OOX_custom_shapes) / sizeof(_shape_converter);
			size_t pos = odf_type.find(L"ooxml-");

			if (pos == std::wstring::npos)
			{
				for (long i = 0; i < count; i++)
				{
					if (_OO_OOX_custom_shapes[i].odf_reader == odf_type)
					{
						draw_type_oox_index_ = i;
						break;
					}	
				}
			}
			else
			{
				bOoxType_ = true;
				
				std::wstring oox_type = odf_type.substr(pos + 6);
				for (long i = 0; i < count; i++)
				{
					if (_OO_OOX_custom_shapes[i].oox == oox_type)
					{
						draw_type_oox_index_ = i;
						break;
					}	
				}
			}
			if ((draw_type_oox_index_) && (*draw_type_oox_index_== 96))//L"textBox" "mso-spt24"
			{
				sub_type_ = 1;//textBox
			}
		}
	}

	if (attlist_.drawooo_enhanced_path_)		odf_path_ = attlist_.drawooo_enhanced_path_.get();
	else if (attlist_.draw_enhanced_path_)		odf_path_ = attlist_.draw_enhanced_path_.get();
}
bool draw_enhanced_geometry::oox_convert(std::vector<odf_reader::_property>& props)
{
	find_draw_type_oox();

	owner_shape->word_art_ = word_art_;

	bool set_shape = false;

	if (attlist_.draw_mirror_horizontal_)
	{
		props.push_back(_property(L"flipH", *attlist_.draw_mirror_horizontal_));
	}
	if (attlist_.draw_mirror_vertical_)
	{
		props.push_back(_property(L"flipV", *attlist_.draw_mirror_vertical_));
	}
	if (draw_type_oox_index_)
	{
		props.push_back(_property(L"oox-geom-index", draw_type_oox_index_.get()));
		props.push_back(_property(L"oox-geom", bOoxType_));

		if (word_art_)
			props.push_back(_property(L"wordArt", true));

		set_shape = true;
	}

	if (sub_type_)
	{
		owner_shape->sub_type_ = sub_type_.get();
		set_shape = true;
	}
	std::vector<std::pair<std::wstring, std::wstring>> equations;
	if (false == draw_equations_.empty())
	{
		for (size_t i = 0; i < draw_equations_.size(); i++)
		{
			draw_equation* eq = dynamic_cast<draw_equation*>(draw_equations_[i].get());
			if (eq)
			{
				std::wstring name = eq->attlist_.draw_name_.get_value_or(L"");
				std::wstring value = eq->attlist_.draw_formula_.get_value_or(L"");

				XmlUtils::replace_all(name, L"f", L"gd");
				value = convert_equation(value);

				if (value.empty())
				{
					if (!draw_type_oox_index_) 
						set_shape = false;
					equations.clear();
					break;
				}
				else
				{// 
					equations.push_back(std::make_pair(name, value));
				}
			}
		}

		if (false == equations.empty())
		{
			std::wstringstream output1_;
			::svg_path::oox_serialize(output1_, equations);
			props.push_back(odf_reader::_property(L"custom_equations", output1_.str()));
		}
	}

	if (!odf_path_.empty())
	{
		bool bCPathWithArgs = (std::wstring::npos != odf_path_.find(L"?"));
		if (bOoxType_ || (bCPathWithArgs && false == equations.empty()))
		{
			std::vector<::svg_path::_polylineS> o_Polyline;

			bool res = false;
			bool bClosed = false, bStroked = true;

			try
			{
				res = ::svg_path::parseSvgS(o_Polyline, odf_path_, true, bClosed, bStroked);
			}
			catch (...)
			{
				res = false;
			}

			if (!o_Polyline.empty() && res)
			{
				set_shape = true;

				std::wstringstream output_;
				::svg_path::oox_serialize(output_, o_Polyline);
				props.push_back(odf_reader::_property(L"custom_path", output_.str()));

				if (false == bStroked)
				{
					props.push_back(odf_reader::_property(L"custom_path_s", false));
				}
			}
		}
		else if (!bCPathWithArgs)
		{
			std::vector<::svg_path::_polyline> o_Polyline;

			bool res = false;
			bool bClosed = false, bStroked = true;

			try
			{
				res = ::svg_path::parseSvgD(o_Polyline, odf_path_, true, bClosed, bStroked);
			}
			catch (...)
			{
				res = false;
			}
			//if (!bClosed) lined_shape_ = true;

			if (!o_Polyline.empty() && res)
			{
				//сформируем xml-oox сдесь ... а то придется плодить массивы в drawing .. хоть и не красиво..
				std::wstringstream output_;

				::svg_path::oox_serialize(output_, o_Polyline);
				props.push_back(odf_reader::_property(L"custom_path", output_.str()));

				set_shape = true;

				if (false == bStroked)
				{
					props.push_back(odf_reader::_property(L"custom_path_s", false));
				}
			}
		}
	}

	if (attlist_.drawooo_sub_view_size_)
	{
		std::vector< std::wstring > splitted;
		boost::algorithm::split(splitted, *attlist_.drawooo_sub_view_size_, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);

		if (splitted.size() == 2)
		{
			int w = boost::lexical_cast<int>(splitted[0]);
			int h = boost::lexical_cast<int>(splitted[1]);

			props.push_back(odf_reader::_property(L"custom_path_w", w));
			props.push_back(odf_reader::_property(L"custom_path_h", h));
		}
		else if (splitted.size() == 4)
		{///???? rect ???
			int l = boost::lexical_cast<int>(splitted[0]);
			int t = boost::lexical_cast<int>(splitted[1]);
			int r = boost::lexical_cast<int>(splitted[2]);
			int b = boost::lexical_cast<int>(splitted[3]);

		}

		//if (shape->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_)
		//{
		//	int w_shape = shape->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_->get_value();
		//	if (w_shape < 1) shape->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_ = length(1, length::pt);
		//}
		//if (shape->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_)
		//{
		//	int h_shape = shape->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_->get_value();
		//	if (h_shape < 1) shape->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_ = length(1, length::pt);
		//}
	}
	else if (svg_viewbox_)
	{
		std::vector< std::wstring > splitted;
		boost::algorithm::split(splitted, *svg_viewbox_, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);

		if (splitted.size() == 4)
		{
			int w = boost::lexical_cast<int>(splitted[2]);
			int h = boost::lexical_cast<int>(splitted[3]);

			props.push_back(odf_reader::_property(L"custom_path_w", w));
			props.push_back(odf_reader::_property(L"custom_path_h", h));
		}
	}
	if (attlist_.draw_modifiers_ && set_shape && bOoxType_)
	{
		props.push_back(_property(L"oox-draw-modifiers", attlist_.draw_modifiers_.get()));
	}

	return set_shape;
}


//-------------------------------------------------------------------------------------------
/// draw-caption-attlist
//-------------------------------------------------------------------------------------------
void draw_caption_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
//-------------------------------------------------------------------------------------------
// draw:caption
//-------------------------------------------------------------------------------------------
const wchar_t * draw_caption::ns = L"draw";
const wchar_t * draw_caption::name = L"caption";

void draw_caption::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_caption_attlist_.add_attributes(Attributes);
	draw_shape::add_attributes(Attributes);
	
	sub_type_ = 1;	
}
//-------------------------------------------------------------------------------------------
/// draw-connector-attlist
//-------------------------------------------------------------------------------------------
void draw_connector_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"svg:d",			svg_d_);
    CP_APPLY_ATTR(L"svg:viewBox",	svg_viewbox_);	
    CP_APPLY_ATTR(L"draw:type",		draw_type_);

	CP_APPLY_ATTR(L"draw:start-shape"		, draw_start_shape_);
	CP_APPLY_ATTR(L"draw:end-shape"			, draw_end_shape_);
	CP_APPLY_ATTR(L"draw:start-glue-point"	, draw_start_glue_point_);
	CP_APPLY_ATTR(L"draw:end-glue-point"	, draw_end_glue_point_);
}
//-------------------------------------------------------------------------------------------
// draw:connector
//-------------------------------------------------------------------------------------------
const wchar_t * draw_connector::ns = L"draw";
const wchar_t * draw_connector::name = L"connector";

void draw_connector::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_connector_attlist_.add_attributes(Attributes);
    draw_line_attlist_.add_attributes(Attributes);
	draw_shape::add_attributes(Attributes);

	sub_type_ = 10; //коннектор - линия, если ломаная (ниже определяется) - то путь
	lined_shape_ = true;	
	connector_ = true;
}
void draw_connector::reset_svg_path()
{
	if (draw_connector_attlist_.svg_d_)
	{
		sub_type_ = 8;

		std::vector<::svg_path::_polyline> o_Polyline_pt;
		std::vector<::svg_path::_polyline> o_Polyline_cm;
	
		bool bClosed = false, bStroked = true;
		bool res = ::svg_path::parseSvgD(o_Polyline_cm, draw_connector_attlist_.svg_d_.get(), false, bClosed, bStroked);

		if (!bClosed) lined_shape_ = true;
	
		double x1=common_draw_attlists_.position_.svg_x_.get_value_or(length(0)).get_value_unit(length::emu);
		double y1=common_draw_attlists_.position_.svg_y_.get_value_or(length(0)).get_value_unit(length::emu);
		
		for (size_t ind = 0 ; ind < o_Polyline_cm.size(); ind++)
		{
			::svg_path::_polyline  & poly = o_Polyline_cm[ind];
			
			for (size_t i = 0; i < poly.points.size();i++)
			{
				if (poly.points[i].x)
				{
					poly.points[i].x =  length(poly.points[i].x.get()/1000., length::cm).get_value_unit(length::emu) - x1; 
				}
				if (poly.points[i].y)
				{
					poly.points[i].y = length(poly.points[i].y.get()/1000., length::cm).get_value_unit(length::emu) - y1; 
				}
			}
			o_Polyline_pt.push_back(poly);
		}
		if (o_Polyline_pt.size() > 0)
		{
			sub_type_ = 6;
			//сформируем xml-oox сдесь ... а то придется плодить массивы в drawing .. хоть и не красиво..
			std::wstringstream output_;   
            ::svg_path::oox_serialize(output_, o_Polyline_pt);
			additional_.push_back(odf_reader::_property(L"custom_path",output_.str()));
		}
	}
}
///////////////////////////////////////

//-------------------------------------------------------------------------------------------
// dr3d:scene
//-------------------------------------------------------------------------------------------
const wchar_t * dr3d_scene::ns = L"dr3d";
const wchar_t * dr3d_scene::name = L"scene";

void dr3d_scene::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	dr3d_attlist_.add_attributes(Attributes);
	draw_shape::add_attributes(Attributes);

	sub_type_ = 12; 
	
}
//-------------------------------------------------------------------------------------------
// dr3d:extrude
//-------------------------------------------------------------------------------------------
const wchar_t * dr3d_extrude::ns = L"dr3d";
const wchar_t * dr3d_extrude::name = L"extrude";

void dr3d_extrude::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	draw_path::add_attributes(Attributes);
	
	//sub_type_ = ??
}

//-------------------------------------------------------------------------------------------
// dr3d:rotate
//-------------------------------------------------------------------------------------------
const wchar_t * dr3d_rotate::ns = L"dr3d";
const wchar_t * dr3d_rotate::name = L"rotate";

void dr3d_rotate::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	draw_path::add_attributes(Attributes);
	
	//sub_type_ = ??
}
//-------------------------------------------------------------------------------------------
// dr3d:light
//-------------------------------------------------------------------------------------------
const wchar_t * dr3d_light::ns = L"dr3d";
const wchar_t * dr3d_light::name = L"light";

void dr3d_light::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"dr3d:diffuse_color", dr3d_diffuse_color_);
	CP_APPLY_ATTR(L"dr3d:direction",	dr3d_direction_);
	CP_APPLY_ATTR(L"dr3d:specular",		dr3d_specular_);
	CP_APPLY_ATTR(L"dr3d:enabled",		dr3d_enabled_);
}
//-------------------------------------------------------------------------------------------
// dr3d:cube
//-------------------------------------------------------------------------------------------
const wchar_t * dr3d_cube::ns = L"dr3d";
const wchar_t * dr3d_cube::name = L"cube";

void dr3d_cube::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	draw_shape::add_attributes(Attributes);

	CP_APPLY_ATTR(L"dr3d:max-edge",		dr3d_max_edge_);
	CP_APPLY_ATTR(L"dr3d:min-edge",		dr3d_min_edge_);
	CP_APPLY_ATTR(L"dr3d:transform",	dr3d_transform_);

	sub_type_ = 15;
}
//-------------------------------------------------------------------------------------------
// dr3d:sphere
//-------------------------------------------------------------------------------------------
const wchar_t * dr3d_sphere::ns = L"dr3d";
const wchar_t * dr3d_sphere::name = L"sphere";

void dr3d_sphere::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	draw_shape::add_attributes(Attributes);

	CP_APPLY_ATTR(L"dr3d:size",		dr3d_size_); 
	CP_APPLY_ATTR(L"dr3d:center",	dr3d_center_); 
	CP_APPLY_ATTR(L"dr3d:transform",dr3d_transform_);

	sub_type_ = 16;
}
//-------------------------------------------------------------------------------------------
// draw:control
//-------------------------------------------------------------------------------------------
const wchar_t * draw_control::ns = L"draw";
const wchar_t * draw_control::name = L"control";

void draw_control::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"xml:id", xml_id_);
	CP_APPLY_ATTR(L"draw:caption-id", caption_id_);
	CP_APPLY_ATTR(L"draw:control", control_id_);

	draw_shape::add_attributes(Attributes);
}

void draw_control::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if CP_CHECK_NAME(L"draw", L"glue-point")
    {
        CP_CREATE_ELEMENT(draw_glue_point_);
    }   
}

}
}
