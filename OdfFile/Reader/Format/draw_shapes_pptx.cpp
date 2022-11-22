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

 ///////////////////////////////////////////	
	Context.get_slide_context().set_name(name);
	
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
	graphic_format_properties properties = calc_graphic_properties_content(instances);
	
////////////////////////////////////////////////////////////////////////////////////
	properties.apply_to(Context.get_slide_context().get_properties());
	
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

	oox::_oox_fill fill;
	Compute_GraphicFill(properties.common_draw_fill_attlist_, properties.style_background_image_, 
																		Context.root()->odf_context().drawStyles() ,fill);	
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
		
	}else
	{
		common_draw_attlists_.position_.svg_x_	 = draw_line_attlist_.svg_x1_;
		common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_ = length(x2-x1, length::pt);
	}
	if (y1 > y2)
	{
		common_draw_attlists_.position_.svg_y_	 = draw_line_attlist_.svg_y2_;
		common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_ = length(y1-y2, length::pt);

	}else
	{
		common_draw_attlists_.position_.svg_y_	 = draw_line_attlist_.svg_y1_;
		common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_ = length(y2-y1, length::pt);
	}
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
	
//перебъем заливку .. 
	oox::_oox_fill fill;
	fill.type = 0;
	Context.get_slide_context().set_fill(fill);

	Context.get_slide_context().end_shape();
}
void draw_enhanced_geometry::pptx_convert(oox::pptx_conversion_context & Context) 
{
	find_draw_type_oox();

	bool set_shape = false;

	if (attlist_.draw_mirror_horizontal_)
	{
		Context.get_slide_context().set_property(_property(L"flipH", *attlist_.draw_mirror_horizontal_));
	}
	if (attlist_.draw_mirror_vertical_)
	{
		Context.get_slide_context().set_property(_property(L"flipV", *attlist_.draw_mirror_vertical_));
	}
	if (draw_type_oox_index_)
	{
		Context.get_slide_context().set_property(_property(L"oox-geom-index", draw_type_oox_index_.get()));	
		Context.get_slide_context().set_property(_property(L"oox-geom", bOoxType_));	
		
		if (word_art_ == true)
			Context.get_slide_context().set_property(_property(L"wordArt", true));	

		set_shape = true;
	}
	if (sub_type_)
	{
		Context.get_slide_context().start_shape(sub_type_.get());
		set_shape = true;
	}

	if (!odf_path_.empty())
	{
		std::vector<::svg_path::_polyline> o_Polyline;
	
		bool res = false;
		bool bClosed = false, bStroked = true;
		
		try
		{
			res = ::svg_path::parseSvgD(o_Polyline, odf_path_, true, bClosed, bStroked);
		}
		catch(...)
		{
			res = false; 
		}
		//if (!bClosed) lined_shape_ = true;
		
		if (o_Polyline.size() > 1 && res )
		{
			//сформируем xml-oox сдесь ... а то придется плодить массивы в drawing .. хоть и не красиво..
			std::wstringstream output_;   
            ::svg_path::oox_serialize(output_, o_Polyline);
			Context.get_slide_context().set_property(odf_reader::_property(L"custom_path", output_.str()));

			set_shape = true;

			if (false == bStroked)
			{
				Context.get_slide_context().set_property(odf_reader::_property(L"custom_path_s", false));
			}
			if (attlist_.drawooo_sub_view_size_)
			{
				std::vector< std::wstring > splitted;			    
				boost::algorithm::split(splitted, *attlist_.drawooo_sub_view_size_, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
				
				if (splitted.size() == 2)
				{
					int w = boost::lexical_cast<int>(splitted[0]);
					int h = boost::lexical_cast<int>(splitted[1]);
					
					Context.get_slide_context().set_property(odf_reader::_property(L"custom_path_w", w));
					Context.get_slide_context().set_property(odf_reader::_property(L"custom_path_h", h));
				}
				else if (splitted.size() == 4)
				{///???? rect ???
					int l = boost::lexical_cast<int>(splitted[0]);
					int t = boost::lexical_cast<int>(splitted[1]);
					int r = boost::lexical_cast<int>(splitted[2]);
					int b = boost::lexical_cast<int>(splitted[3]);

				}
			}
			else if (svg_viewbox_)
			{
				std::vector< std::wstring > splitted;			    
				boost::algorithm::split(splitted, *svg_viewbox_, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
				
				if (splitted.size() == 4)
				{
					int w = boost::lexical_cast<int>(splitted[2]);
					int h = boost::lexical_cast<int>(splitted[3]);
					
					Context.get_slide_context().set_property(odf_reader::_property(L"custom_path_w", w));
					Context.get_slide_context().set_property(odf_reader::_property(L"custom_path_h", h));
				}
			}
		}
	}
	if (attlist_.draw_modifiers_)
	{
		if (bOoxType_)
			Context.get_slide_context().set_property(_property(L"oox-draw-modifiers", attlist_.draw_modifiers_.get()));	
		else
		{
		}

		//if (draw_handle_geometry_.size()>0)
		//{
		//	if (draw_handle_geometry_[0].min < draw_handle_geometry_[0].max)
		//	{
		//		Context.get_slide_context().set_property(_property(L"draw-modifiers-min",draw_handle_geometry_[0].min));	
		//		Context.get_slide_context().set_property(_property(L"draw-modifiers-max",draw_handle_geometry_[0].max));	
		//	}
		//}
	}
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
