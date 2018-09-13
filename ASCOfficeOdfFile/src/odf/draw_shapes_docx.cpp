﻿/*
 * (c) Copyright Ascensio System SIA 2010-2018
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

#include <boost/algorithm/string.hpp>
#include <boost/regex.h>

#include <xml/xmlchar.h>
#include <xml/attributes.h>
#include <odf/odf_document.h>

#include "serialize_elements.h"
#include "style_graphic_properties.h"

#include "odfcontext.h"

#include "datatypes/length.h"
#include "datatypes/borderstyle.h"

#include "../docx/xlsx_utils.h"
#include "../docx/oox_drawing.h"

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

//////////////////////////////на другом контексте

	//тут может быть не только текст , но и таблицы, другие объекты ...
 	oox::StreamsManPtr prev = Context.get_stream_man();
	
	std::wstringstream temp_stream(Context.get_drawing_context().get_text_stream_shape());
	Context.set_stream_man( boost::shared_ptr<oox::streams_man>( new oox::streams_man(temp_stream) ));

//сначала элементы графики  потом все остальное	
	for (size_t i = 0; i < content_.size(); i++)
    {
		ElementType type = content_[i]->get_type();
		
		if (type == typeDrawCustomShape) // || .... 
		{
			content_[i]->docx_convert(Context);
		}
    }
	Context.reset_context_state();

	for (size_t i = 0; i < content_.size(); i++)
    {
		ElementType type = content_[i]->get_type();
		
		if (type != typeDrawCustomShape)
		{
			content_[i]->docx_convert(Context);
		}
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
	std::wstring odf_path; //общая часть - объединить ...
	if (draw_enhanced_geometry_attlist_.drawooo_enhanced_path_)
		odf_path = draw_enhanced_geometry_attlist_.drawooo_enhanced_path_.get();
	else if (draw_enhanced_geometry_attlist_.draw_enhanced_path_)
		odf_path = draw_enhanced_geometry_attlist_.draw_enhanced_path_.get();
	
	if (!odf_path.empty())
	{
		std::vector<::svg_path::_polyline> o_Polyline;
	
		bool res = false;
		
		try
		{
			res = ::svg_path::parseSvgD(o_Polyline, odf_path, true);
		}
		catch(...)
		{
			res = false; 
		}
		
		if (!o_Polyline.empty() && res )
		{
			//сформируем xml-oox сдесь ... а то придется плодить массивы в drawing .. хоть и не красиво..
			std::wstringstream output_;   
            
			::svg_path::oox_serialize(output_, o_Polyline);
			shape->additional_.push_back(odf_reader::_property(L"custom_path", output_.str()));
			
			set_shape = true;
			int w = 0;
			int h = 0;

			if (draw_enhanced_geometry_attlist_.drawooo_sub_view_size_)
			{
				std::vector< std::wstring > splitted;			    
				boost::algorithm::split(splitted, *draw_enhanced_geometry_attlist_.drawooo_sub_view_size_, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
				
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
		}
	}

	if (draw_enhanced_geometry_attlist_.draw_modifiers_)
	{
		if (bOoxType_)
			shape->additional_.push_back(_property(L"oox-draw-modifiers", draw_enhanced_geometry_attlist_.draw_modifiers_.get()));	
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
void dr3d_extrude::docx_convert(oox::docx_conversion_context & Context)
{
	reset_svg_path();

}
void dr3d_light::docx_convert(oox::docx_conversion_context & Context)
{

}
}
}