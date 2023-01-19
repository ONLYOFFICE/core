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
#include <xml/utils.h>
#include "odf_document.h"

#include "serialize_elements.h"
#include "style_graphic_properties.h"

#include "odfcontext.h"

#include "../../DataTypes/length.h"
#include "../../DataTypes/borderstyle.h"

#include "../../Reader/Converter/xlsx_utils.h"
#include "../../Reader/Converter/docx_drawing.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

void draw_shape::common_docx_convert(oox::docx_conversion_context & Context)
{
    Context.get_drawing_context().start_shape(this);
    
    const _CP_OPT(std::wstring) name = 
        common_draw_attlists_.shape_with_text_and_styles_.
        common_shape_draw_attlist_.draw_name_;
	
	Context.get_drawing_context().add_name_object(name.get_value_or(L"Shape"));

//---------------------------сначала элементы графики  потом все остальное	
	if (enhanced_geometry_)
		enhanced_geometry_->docx_convert(Context);

//---------------------------на другом контексте
	//тут может быть не только текст , но и таблицы, другие объекты ...
 	oox::StreamsManPtr prev = Context.get_stream_man();
	
	std::wstringstream temp_stream(Context.get_drawing_context().get_text_stream_shape());
	Context.set_stream_man( boost::shared_ptr<oox::streams_man>( new oox::streams_man(temp_stream) ));

	Context.reset_context_state();

	if (word_art_)
	{
		const std::wstring styleName = common_draw_attlists_.shape_with_text_and_styles_.
						common_shape_draw_attlist_.draw_style_name_.get_value_or(L"");

		style_instance* styleInst = Context.root()->odf_context().styleContainer().style_by_name(styleName, odf_types::style_family::Graphic,Context.process_headers_footers_);
		style_content * content = styleInst ? styleInst->content() : NULL;
		graphic_format_properties * graphicProp = content ? content->get_graphic_properties() : NULL;

		if (graphicProp)
		{
			oox::_oox_fill fill;
			Compute_GraphicFill(graphicProp->common_draw_fill_attlist_, graphicProp->style_background_image_,
																	Context.root()->odf_context().drawStyles(), fill);	
			if ((fill.bitmap) && (fill.bitmap->rId.empty()))
			{
				std::wstring href = fill.bitmap->xlink_href_;
				fill.bitmap->rId = Context.get_mediaitems()->add_or_find(href, oox::typeImage, fill.bitmap->isInternal, href, Context.get_type_place());
				fill.bitmap->name_space = L"w14";
			}

			std::wstringstream strm_fill, strm_ln;
			oox::oox_serialize_fill(strm_fill, fill, L"w14");
			std::wstring textFill = strm_fill.str();

			std::vector<_property> props;
			graphicProp->apply_to(props);

			oox::oox_serialize_ln(strm_ln, props, false, L"w14");
			std::wstring textLn = strm_ln.str();

			std::wstring text_props;
			if (!textLn.empty())
			{
				text_props += textLn;
			}	
			if (!textFill.empty())
			{
				text_props += L"<w14:textFill>" + textFill + L"</w14:textFill>";
			}
			Context.set_drawing_text_props(text_props);
		}
	}

	for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->docx_convert(Context);
    }

	Context.back_context_state();

	Context.get_drawing_context().get_text_stream_shape() = temp_stream.str();
	Context.set_stream_man(prev);
}
void draw_rect::docx_convert(oox::docx_conversion_context & Context)
{
 	if (Context.get_drawing_context().get_current_level() > 0 && !Context.get_drawing_context().in_group() )
	{ 
		if(Context.delayed_converting_ == false)
			Context.add_delayed_element(this);
		return;
	}
	common_docx_convert(Context);

	if (draw_rect_attlist_.draw_corner_radius_)
	{
		draw_shape * shape = Context.get_drawing_context().get_current_shape();//owner

		double val = draw_rect_attlist_.draw_corner_radius_->get_value() * 6500;
		shape->additional_.push_back(_property(L"oox-draw-modifiers", std::to_wstring((int)val)));	
	}

	draw_shape::docx_convert(Context);
}
void draw_ellipse::docx_convert(oox::docx_conversion_context & Context)
{
	//if (Context.get_drawing_context().get_current_level() >0 )return;
 	if (Context.get_drawing_context().get_current_level() > 0 && !Context.get_drawing_context().in_group() )
	{ 
		if(Context.delayed_converting_ == false)
			Context.add_delayed_element(this);
		return;
	}
	common_docx_convert(Context);
	//...
	draw_shape::docx_convert(Context);
}
void draw_circle::docx_convert(oox::docx_conversion_context & Context)
{
	//if (Context.get_drawing_context().get_current_level() >0 )return;
 	if (Context.get_drawing_context().get_current_level() > 0 && !Context.get_drawing_context().in_group() )
	{ 
		if(Context.delayed_converting_ == false)
			Context.add_delayed_element(this);
		return;
	}
	common_docx_convert(Context);
	//...
	draw_shape::docx_convert(Context);
}
void draw_line::docx_convert(oox::docx_conversion_context & Context)
{
	//if (Context.get_drawing_context().get_current_level() >0 )return;

	reset_svg_attributes();	
	
	common_docx_convert(Context);
	//...

	draw_shape::docx_convert(Context);
}

void draw_path::docx_convert(oox::docx_conversion_context & Context)
{
	//if (Context.get_drawing_context().get_current_level() >0 )return;
 	if (Context.get_drawing_context().get_current_level() > 0 && !Context.get_drawing_context().in_group() )
	{ 
		if(Context.delayed_converting_ == false)
			Context.add_delayed_element(this);
		return;
	}
	common_docx_convert(Context);
	//...
	reset_svg_path();
	draw_shape::docx_convert(Context);
}

void draw_connector::docx_convert(oox::docx_conversion_context & Context)
{
	//if (Context.get_drawing_context().get_current_level() >0 )return;
	if (draw_connector_attlist_.draw_type_)
	{
		if (*draw_connector_attlist_.draw_type_ == L"curve")
		{
			sub_type_ = 11;
		}
	}
	reset_svg_attributes();	
	common_docx_convert(Context);
	//...
	reset_svg_path();

	draw_shape::docx_convert(Context);
}
void draw_polygon::docx_convert(oox::docx_conversion_context & Context)
{
	//if (Context.get_drawing_context().get_current_level() >0 )return;
 	if (Context.get_drawing_context().get_current_level() > 0 && !Context.get_drawing_context().in_group() )
	{ 
		if(Context.delayed_converting_ == false)
			Context.add_delayed_element(this);
		return;
	}
	common_docx_convert(Context);
	//...
	reset_polygon_path();
	draw_shape::docx_convert(Context);
}
void draw_polyline::docx_convert(oox::docx_conversion_context & Context)
{
	//if (Context.get_drawing_context().get_current_level() >0 )return;

	common_docx_convert(Context);
	//...
	reset_polyline_path();
	draw_shape::docx_convert(Context);
}
void draw_custom_shape::docx_convert(oox::docx_conversion_context & Context)
{
	//if (Context.get_drawing_context().get_current_level() >0 )return;
 	if (Context.get_drawing_context().get_current_level() > 0 && !Context.get_drawing_context().in_group() )
	{ 
		if(Context.delayed_converting_ == false)
			Context.add_delayed_element(this);
		return;
	}
	common_docx_convert(Context);
	//...
	draw_shape::docx_convert(Context);
}
void draw_caption::docx_convert(oox::docx_conversion_context & Context)
{
	//if (Context.get_drawing_context().get_current_level() >0 )return;
 	if (Context.get_drawing_context().get_current_level() > 0 && !Context.get_drawing_context().in_group() )
	{ 
		if(Context.delayed_converting_ == false)
			Context.add_delayed_element(this);
		return;
	}
	common_docx_convert(Context);
	//...
	draw_shape::docx_convert(Context);
}
void draw_enhanced_geometry::docx_convert(oox::docx_conversion_context & Context)
{
	find_draw_type_oox();

	draw_shape * shape = Context.get_drawing_context().get_current_shape();//owner

	shape->word_art_ = word_art_;

	bool set_shape = false;

	if (attlist_.draw_mirror_horizontal_)
	{
		shape->additional_.push_back(_property(L"flipH", *attlist_.draw_mirror_horizontal_));
	}
	if (attlist_.draw_mirror_vertical_)
	{
		shape->additional_.push_back(_property(L"flipV", *attlist_.draw_mirror_vertical_));
	}
	if (draw_type_oox_index_)
	{
		shape->additional_.push_back(_property(L"oox-geom-index", draw_type_oox_index_.get()));	
		shape->additional_.push_back(_property(L"oox-geom", bOoxType_));	

		if (shape->word_art_ == true)
			shape->additional_.push_back(_property(L"wordArt", true));

		set_shape = true;
	}

	if (sub_type_)
	{
		shape->sub_type_ = sub_type_.get();
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
		
		if (!o_Polyline.empty() && res )
		{
			//сформируем xml-oox сдесь ... а то придется плодить массивы в drawing .. хоть и не красиво..
			std::wstringstream output_;   
            
			::svg_path::oox_serialize(output_, o_Polyline);
			shape->additional_.push_back(odf_reader::_property(L"custom_path", output_.str()));
			
			set_shape = true;
			int w = 0;
			int h = 0;

			if (false == bStroked)
			{
				shape->additional_.push_back(odf_reader::_property(L"custom_path_s", false));
			}
			if (attlist_.drawooo_sub_view_size_)
			{
				std::vector< std::wstring > splitted;			    
				boost::algorithm::split(splitted, *attlist_.drawooo_sub_view_size_, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
				
				if (splitted.size() == 2)
				{
					w = boost::lexical_cast<int>(splitted[0]);
					h = boost::lexical_cast<int>(splitted[1]);
					
					shape->additional_.push_back(odf_reader::_property(L"custom_path_w", w));
					shape->additional_.push_back(odf_reader::_property(L"custom_path_h", h));
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
					
					shape->additional_.push_back(odf_reader::_property(L"custom_path_w", w));
					shape->additional_.push_back(odf_reader::_property(L"custom_path_h", h));
				}
			}
		}
	}

	if (attlist_.draw_modifiers_)
	{
		if (bOoxType_)
			shape->additional_.push_back(_property(L"oox-draw-modifiers", attlist_.draw_modifiers_.get()));	
		else
		{
		}
	}

	if (!set_shape)
	{
		shape->bad_shape_ = true;
	}
}

void dr3d_scene::docx_convert(oox::docx_conversion_context & Context)
{
	//if (Context.get_drawing_context().get_current_level() >0 )return;
 	if (Context.get_drawing_context().get_current_level() > 0 && !Context.get_drawing_context().in_group() )
	{ 
		if(Context.delayed_converting_ == false)
			Context.add_delayed_element(this);
		return;
	}
	common_docx_convert(Context);
	//...
	draw_shape::docx_convert(Context);
}
void dr3d_light::docx_convert(oox::docx_conversion_context & Context)
{

}
void dr3d_cube::docx_convert(oox::docx_conversion_context & Context)
{

}
void dr3d_sphere::docx_convert(oox::docx_conversion_context & Context)
{

}
}
}
