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

#include "draw_common.h"
#include "../../DataTypes/custom_shape_types_convert.h"

#include <ostream>
#include <sstream>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/regex.h>

#include <xml/xmlchar.h>
#include "odf_document.h"

#include "serialize_elements.h"

#include "style_graphic_properties.h"

#include "../../DataTypes/length.h"
#include "../../DataTypes/borderstyle.h"

#include "odfcontext.h"
#include "calcs_styles.h"

#include "../../Reader/Converter/oox_drawing.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {


void draw_shape::common_pptx_convert(oox::pptx_conversion_context & Context)
{
	common_shape_draw_attlist	&common_draw_attlist_		= common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_;
	common_presentation_attlist	&common_presentation_attlist_= common_draw_attlists_.shape_with_text_and_styles_.common_presentation_attlist_;

    const unsigned int z_index		= common_draw_attlist_.draw_z_index_.get_value_or(0);
    const std::wstring name			= common_draw_attlist_.draw_name_.get_value_or(L"");
    const std::wstring textStyleName = common_draw_attlist_.draw_text_style_name_.get_value_or(L"");
	const std::wstring xmlId		= common_draw_attlist_.draw_id_.get_value_or(L"");

 ///////////////////////////////////////////	
	Context.get_slide_context().set_name(name);
	Context.get_slide_context().set_id(xmlId);
	
	const _CP_OPT(length) svg_widthVal =  common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_;    
    const _CP_OPT(length) svg_heightVal = common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_;

	if (svg_widthVal && svg_heightVal)
	{
		const double width_pt = svg_widthVal.get_value_or(length(0)).get_value_unit(length::pt);
		const double height_pt = svg_heightVal.get_value_or(length(0)).get_value_unit(length::pt);

		const double x_pt = common_draw_attlists_.position_.svg_x_.get_value_or(length(0)).get_value_unit(length::pt);
		const double y_pt = common_draw_attlists_.position_.svg_y_.get_value_or(length(0)).get_value_unit(length::pt);

		Context.get_slide_context().set_rect(width_pt, height_pt, x_pt, y_pt);
	}
////////////////////////////////////////////////////////////
	if (common_draw_attlist_.draw_transform_)
	{
		std::wstring transformStr = common_draw_attlist_.draw_transform_.get();
		pptx_convert_transforms(transformStr, Context);
	}
////////////////////////////////////////
	std::wstring Anchor;
	if (common_draw_attlists_.shape_with_text_and_styles_.common_shape_table_attlist_.table_end_cell_address_)
	{
		Anchor = common_draw_attlists_.shape_with_text_and_styles_.common_shape_table_attlist_.table_end_cell_address_.get();
		const double a_x_pt = common_draw_attlists_.shape_with_text_and_styles_.common_shape_table_attlist_.table_end_x_.get_value_or(length(0)).get_value_unit(length::pt);
		const double a_y_pt = common_draw_attlists_.shape_with_text_and_styles_.common_shape_table_attlist_.table_end_y_.get_value_or(length(0)).get_value_unit(length::pt);

		Context.get_slide_context().set_anchor(Anchor,a_x_pt,a_y_pt);
	}
/////////////////////////////////////////////////////////////////////////////////
	std::vector<const odf_reader::style_instance *> instances;

	const std::wstring grStyleName		= common_draw_attlist_.draw_style_name_.get_value_or(L"");
	const std::wstring baseStyleName	= common_draw_attlists_.shape_with_text_and_styles_.common_presentation_attlist_.presentation_style_name_.get_value_or(L"");

	odf_reader::style_instance* grStyleInst = 
		Context.root()->odf_context().styleContainer().style_by_name(grStyleName, odf_types::style_family::Graphic,Context.process_masters_);
	
	odf_reader::style_instance* baseStyleInst = 
		Context.root()->odf_context().styleContainer().style_by_name(baseStyleName, odf_types::style_family::Presentation,Context.process_masters_);

	if (baseStyleInst)//векторная фигура презентаций
	{
		style_instance * defaultStyle = Context.root()->odf_context().styleContainer().style_default_by_type(odf_types::style_family::Presentation);
		if (defaultStyle)instances.push_back(defaultStyle);

		instances.push_back(baseStyleInst);
	}
	else if (grStyleInst)//обычная векторная фигура
	{		
		style_instance * defaultStyle = Context.root()->odf_context().styleContainer().style_default_by_type(odf_types::style_family::Graphic);
		if (defaultStyle)instances.push_back(defaultStyle);

		instances.push_back(grStyleInst);
	}
	
	oox::_oox_fill fill;

	graphic_format_properties_ptr properties = calc_graphic_properties_content(instances);
	if (properties)
	{
		properties->apply_to(Context.get_slide_context().get_properties());
		Compute_GraphicFill(properties->common_draw_fill_attlist_, properties->style_background_image_, Context.root(), fill);
	}
 	for (size_t i = 0; i < additional_.size(); i++)
	{
		Context.get_slide_context().set_property(additional_[i]);
	}
	if (!textStyleName.empty())
	{
		odf_reader::style_instance* textStyleInst = 
			Context.root()->odf_context().styleContainer().style_by_name(textStyleName, odf_types::style_family::Paragraph, Context.process_masters_);

		paragraph_format_properties paragraph_properties = calc_paragraph_properties_content(textStyleInst);
	}
///////////////////////////////////////////////////////////////////////////////////////	
	Context.get_slide_context().set_is_line_shape(lined_shape_);
	Context.get_slide_context().set_is_connector_shape(connector_);

	Context.get_slide_context().set_fill(fill);

	if (common_presentation_attlist_.presentation_class_)
	{
		Context.get_slide_context().set_placeHolder_type(common_presentation_attlist_.presentation_class_->get_type_ms());
		
		if (idx_in_owner >= 0)
			Context.get_slide_context().set_placeHolder_idx(idx_in_owner);
	}
////////////////////////////////////////////////////////////////////////////////////
	Context.get_text_context().start_object();
	
	if (enhanced_geometry_)
		enhanced_geometry_->pptx_convert(Context);

	if (word_art_)
	{
		//set fill & stroke to text
	}

	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->pptx_convert(Context);
    }
	std::wstring text_content_ = Context.get_text_context().end_object();

	if (!text_content_.empty())
	{
		Context.get_slide_context().set_property(_property(L"text-content",text_content_));
	}
}
void draw_rect::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_slide_context().start_shape(sub_type_);

	common_pptx_convert(Context);

	Context.get_slide_context().end_shape();
}
void draw_ellipse::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_slide_context().start_shape(sub_type_);

	common_pptx_convert(Context);

	Context.get_slide_context().end_shape();
}
void draw_circle::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_slide_context().start_shape(sub_type_);

	common_pptx_convert(Context);

	Context.get_slide_context().end_shape();
}
void draw_line::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_slide_context().start_shape(sub_type_);
	
	reset_svg_attributes();
	
	common_pptx_convert(Context);

	Context.get_slide_context().end_shape();
}


void draw_path::pptx_convert(oox::pptx_conversion_context & Context)
{
	reset_svg_path();
///////////////////////////////////////////////////////////////////////
	Context.get_slide_context().start_shape(sub_type_);

	common_pptx_convert(Context);

	Context.get_slide_context().end_shape();
}
void draw_polygon::pptx_convert(oox::pptx_conversion_context & Context)
{
	//closed shape
	reset_polygon_path();
///////////////////////////////////////////////////////////////////////
	Context.get_slide_context().start_shape(sub_type_);

	common_pptx_convert(Context);

	Context.get_slide_context().end_shape();
}
void draw_polyline::pptx_convert(oox::pptx_conversion_context & Context)
{
	//line
	reset_polyline_path();
///////////////////////////////////////////////////////////////////////
	Context.get_slide_context().start_shape(sub_type_);

	common_pptx_convert(Context);

	Context.get_slide_context().end_shape();
}
//void dr3d_rotate::pptx_convert(oox::pptx_conversion_context & Context)
//{
//	//closed shape
//	reset_polygon_path();
/////////////////////////////////////////////////////////////////////////
//	Context.get_slide_context().start_shape(sub_type_);
//
//	common_pptx_convert(Context);
//
//	Context.get_slide_context().end_shape();
//}
void draw_custom_shape::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_slide_context().start_shape(sub_type_);

	common_pptx_convert(Context);

	Context.get_slide_context().end_shape();
}
void draw_caption::pptx_convert(oox::pptx_conversion_context & Context)
{
	//const std::wstring style = common_shape_draw_attlist_.draw_text_style_name_.get_value_or(L"");

	Context.get_slide_context().start_shape(sub_type_);//rect с наваротами-атрибутами .. а-ля TextBox
	
	common_pptx_convert(Context);

	Context.get_slide_context().end_shape();

}
void draw_connector::reset_svg_attributes()
{
	double x1=draw_line_attlist_.svg_x1_.get_value_or(length(0)).get_value_unit(length::pt);
	double y1=draw_line_attlist_.svg_y1_.get_value_or(length(0)).get_value_unit(length::pt);
	double x2=draw_line_attlist_.svg_x2_.get_value_or(length(0)).get_value_unit(length::pt);
	double y2=draw_line_attlist_.svg_y2_.get_value_or(length(0)).get_value_unit(length::pt);
	
	if (x1 > x2)
	{
		common_draw_attlists_.position_.svg_x_	 = draw_line_attlist_.svg_x2_;
		common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_ = length(x1-x2, length::pt);

		additional_.push_back(_property(L"flipH", true));
		
	}else
	{
		common_draw_attlists_.position_.svg_x_	 = draw_line_attlist_.svg_x1_;
		common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_ = length(x2-x1, length::pt);
	}
	if (y1 > y2)
	{
		common_draw_attlists_.position_.svg_y_	 = draw_line_attlist_.svg_y2_;
		common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_ = length(y1-y2, length::pt);

		additional_.push_back(_property(L"flipV", true));

	}else
	{
		common_draw_attlists_.position_.svg_y_	 = draw_line_attlist_.svg_y1_;
		common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_ = length(y2-y1, length::pt);
	}
}

int pptx_convert_glue_point(int gluePoint)
{
	if (gluePoint < 4)
		return 4 - gluePoint;

	return gluePoint - 4;
}

void draw_connector::pptx_convert(oox::pptx_conversion_context & Context)
{
	if (draw_connector_attlist_.draw_type_)
	{
		if (*draw_connector_attlist_.draw_type_ == L"curve")
		{
			sub_type_ = 11;
		}
	}
	reset_svg_attributes();
	reset_svg_path();
///////////////////////////////////////////////////////////////////////

	Context.get_slide_context().start_shape(sub_type_);

	common_pptx_convert(Context);

	if(draw_connector_attlist_.draw_start_shape_)
		Context.get_slide_context().set_connector_start_shape(draw_connector_attlist_.draw_start_shape_.value());
	if (draw_connector_attlist_.draw_end_shape_)
		Context.get_slide_context().set_connector_end_shape(draw_connector_attlist_.draw_end_shape_.value());
	if(draw_connector_attlist_.draw_start_glue_point_)
		Context.get_slide_context().set_connector_start_glue_point(pptx_convert_glue_point(draw_connector_attlist_.draw_start_glue_point_.value()));
	if (draw_connector_attlist_.draw_end_glue_point_)
		Context.get_slide_context().set_connector_end_glue_point(pptx_convert_glue_point(draw_connector_attlist_.draw_end_glue_point_.value()));


	int connector_size = 5;
	if (draw_connector_attlist_.svg_d_)
	{
		std::vector<::svg_path::_polyline> polylines;
		bool closed, stroked;
		::svg_path::parseSvgD(polylines, draw_connector_attlist_.svg_d_.value(), false, closed, stroked);

		const int v = polylines.size() - 1;
		const int min = 2;
		const int max = 5;
		connector_size = v < min ? min : (v > max ? max : v); // clamp(v, min, max)
	}
	
	std::wstring drawType = draw_connector_attlist_.draw_type_.get_value_or(L"standart");
	std::wstring pptx_prst;

	if (drawType == L"curve")
		pptx_prst = L"curvedConnector" + std::to_wstring(connector_size);
	else if (drawType == L"lines")
		pptx_prst = L"bentConnector" + std::to_wstring(connector_size);
	else if (drawType == L"line")
		pptx_prst = L"straightConnector1";
	else if (drawType == L"standart")
		pptx_prst = L"bentConnector" + std::to_wstring(connector_size);
	else
		pptx_prst = L"straightConnector1";

	Context.get_slide_context().set_connector_draw_type(pptx_prst);
		

//перебъем заливку .. 
	oox::_oox_fill fill;
	fill.type = 0;
	Context.get_slide_context().set_fill(fill);

	Context.get_slide_context().end_shape();
}
void draw_enhanced_geometry::pptx_convert(oox::pptx_conversion_context & Context) 
{
	find_draw_type_oox();

	bool set_shape = oox_convert(Context.get_slide_context().get_properties());
	

	if (!set_shape)
	{
		Context.get_slide_context().start_shape(1); //restart type shape
	}
}
void dr3d_scene::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_slide_context().start_shape(sub_type_);

	common_pptx_convert(Context);

	Context.get_slide_context().end_shape();

}
void dr3d_light::pptx_convert(oox::pptx_conversion_context & Context)
{

}
void dr3d_cube::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_slide_context().start_shape(sub_type_); //reset type
}
void dr3d_sphere::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_slide_context().start_shape(sub_type_); //reset type
}
}
}
