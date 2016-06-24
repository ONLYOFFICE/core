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

#include "draw_common.h"
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
#include "../docx/xlsx_utils.h"
#include "style_graphic_properties.h"
#include "odfcontext.h"
#include "calcs_styles.h"
#include "svg_parser.h"

#include "datatypes/length.h"
#include "datatypes/borderstyle.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {



void draw_shape::common_xlsx_convert(oox::xlsx_conversion_context & Context)
{
	common_draw_shape_with_styles_attlist common_draw_attlist_ = common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_;

    const int z_index = common_draw_attlist_.common_draw_z_index_attlist_.draw_z_index_.get_value_or(0);

    const std::wstring name = common_draw_attlist_.common_draw_name_attlist_.draw_name_.get_value_or(L"");

    const std::wstring styleName = common_draw_attlist_.common_draw_style_name_attlist_.draw_style_name_.get_value_or(style_ref(L"")).style_name();
    
    const std::wstring textStyleName = common_draw_attlists_.shape_with_text_and_styles_.
        common_draw_text_style_name_attlist_.draw_text_style_name_.get_value_or(style_ref(L"")).style_name();

 ///////////////////////////////////////////	
	Context.get_drawing_context().start_drawing(name);
	
	const _CP_OPT(length) svg_widthVal =  common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_;    
    const _CP_OPT(length) svg_heightVal = common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_;

	if (svg_widthVal && svg_heightVal)
	{
		const double width_pt = svg_widthVal.get_value_or(length(0)).get_value_unit(length::pt);
		const double height_pt = svg_heightVal.get_value_or(length(0)).get_value_unit(length::pt);

		const double x_pt = common_draw_attlists_.position_.svg_x_.get_value_or(length(0)).get_value_unit(length::pt);
		const double y_pt = common_draw_attlists_.position_.svg_y_.get_value_or(length(0)).get_value_unit(length::pt);

		Context.get_drawing_context().set_rect(width_pt, height_pt, x_pt, y_pt);
	}
////////////////////////////////////////////////////////////
	if (common_draw_attlist_.common_draw_transform_attlist_.draw_transform_)
	{
		std::wstring transformStr = common_draw_attlist_.common_draw_transform_attlist_.draw_transform_.get();
		xlsx_convert_transforms(transformStr,Context);
		//oox_convert_transforms(transformStr, additional_);
	}
////////////////////////////////////////
	std::wstring Anchor;
	if (common_draw_attlist_.common_text_spreadsheet_shape_attlist_.table_end_cell_address_)
	{
		Anchor = common_draw_attlist_.common_text_spreadsheet_shape_attlist_.table_end_cell_address_.get();
		const double a_x_pt = common_draw_attlist_.common_text_spreadsheet_shape_attlist_.table_end_x_.get_value_or(length(0)).get_value_unit(length::pt);
		const double a_y_pt = common_draw_attlist_.common_text_spreadsheet_shape_attlist_.table_end_y_.get_value_or(length(0)).get_value_unit(length::pt);

		Context.get_drawing_context().set_anchor(Anchor,a_x_pt,a_y_pt);
	}
/////////////////////////////////////////////////////////////////////////////////
	std::vector<const odf_reader::style_instance *> instances;

	odf_reader::style_instance* styleInst = 
		Context.root()->odf_context().styleContainer().style_by_name(styleName, odf_types::style_family::Graphic,false/*Context.process_headers_footers_*/);
	if (styleInst)
	{
		style_instance * defaultStyle = Context.root()->odf_context().styleContainer().style_default_by_type(odf_types::style_family::Graphic);
		if (defaultStyle)instances.push_back(defaultStyle);

		instances.push_back(styleInst);
	}
	graphic_format_properties properties = calc_graphic_properties_content(instances);

////////////////////////////////////////////////////////////////////////////////////
	properties.apply_to(Context.get_drawing_context().get_properties());
	
	BOOST_FOREACH(odf_reader::_property const & p, additional_)
	{
		Context.get_drawing_context().set_property(p);
	}
	
	oox::_oox_fill fill;
	Compute_GraphicFill(properties.common_draw_fill_attlist_, properties.style_background_image_,
																	Context.root()->odf_context().drawStyles() ,fill);	
	Context.get_drawing_context().set_fill(fill);
//////////////////////////////////////////////////////////////////////////////////////	
	Context.get_text_context().start_drawing_content();

	BOOST_FOREACH(office_element_ptr const & elm, content_)
    {
        elm->xlsx_convert(Context);
    }
	std::wstring text_content_ = Context.get_text_context().end_drawing_content();

	if (text_content_.length()>0)
	{
		Context.get_drawing_context().set_property(_property(L"text-content",text_content_));
	}

    Context.get_drawing_context().end_drawing();    
}
void draw_rect::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_drawing_context().start_shape(2);

	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
	Context.get_drawing_context().clear();
}
void draw_ellipse::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_drawing_context().start_shape(3);

	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
	Context.get_drawing_context().clear();
}
void draw_circle::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_drawing_context().start_shape(4);

	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
	Context.get_drawing_context().clear();
}
void draw_line::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_drawing_context().start_shape(5);
	
	reset_svg_attributes();
	
	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
	Context.get_drawing_context().clear();
}


void draw_path::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	reset_svg_path();
///////////////////////////////////////////////////////////////////////
	Context.get_drawing_context().start_shape(6);

	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
	Context.get_drawing_context().clear();
}

void draw_connector::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	reset_svg_path();
///////////////////////////////////////////////////////////////////////
	Context.get_drawing_context().start_shape(6);

	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
	Context.get_drawing_context().clear();
}
void draw_polygon::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	reset_polygon_path();
///////////////////////////////////////////////////////////////////////
	Context.get_drawing_context().start_shape(8);

	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
	Context.get_drawing_context().clear();
}
void draw_polyline::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	reset_polyline_path();
///////////////////////////////////////////////////////////////////////
	Context.get_drawing_context().start_shape(8);

	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
	Context.get_drawing_context().clear();
}
void draw_custom_shape::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_drawing_context().start_shape(7);

	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
	Context.get_drawing_context().clear();
}
void draw_caption::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	//const std::wstring style = common_draw_text_style_name_attlist_.draw_text_style_name_.get_value_or(style_ref(L"")).style_name();

	Context.get_drawing_context().start_shape(1);//rect с наваротами-атрибутами .. а-ля TextBox
	
	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
	Context.get_drawing_context().clear();
}
void draw_enhanced_geometry::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
	find_draw_type_oox();

	if (sub_type_)
	{
		Context.get_drawing_context().start_shape(sub_type_.get());
	}
	if (draw_type_oox_index_)
	{
		Context.get_drawing_context().set_property(_property(L"odf-custom-draw-index",draw_type_oox_index_.get()));	
	}
	if (draw_enhanced_geometry_attlist_.draw_modifiers_)
	{
		Context.get_drawing_context().set_property(_property(L"draw-modifiers",draw_enhanced_geometry_attlist_.draw_modifiers_.get()));	

		if (draw_handle_geometry_.size()>0)
		{
			if (draw_handle_geometry_[0].min < draw_handle_geometry_[0].max)
			{
				Context.get_drawing_context().set_property(_property(L"draw-modifiers-min",draw_handle_geometry_[0].min));	
				Context.get_drawing_context().set_property(_property(L"draw-modifiers-max",draw_handle_geometry_[0].max));	
			}
		}
	}
}
}
}