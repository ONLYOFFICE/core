/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
#include "text_elements.h"

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

//---------------------------first graphics elements then everything else
	if (enhanced_geometry_)
		enhanced_geometry_->docx_convert(Context);

//---------------------------on another context
	//here can be not only text, but also tables, other objects...
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
			Compute_GraphicFill(graphicProp->common_draw_fill_attlist_, graphicProp->style_background_image_, Context.root(), fill);	
			
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
		ElementType type = content_[i]->get_type();
		if (type == typeTextP)
		{
			auto _p = dynamic_cast<text::p*>(content_[i].get());
			if (_p)
			{
				if(!_p->paragraph_.content_.size() && _p->paragraph_.attrs_.text_style_name_.empty())
					continue;
			}
		}

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
	draw_shape *shape = Context.get_drawing_context().get_current_shape();//owner
	bool set_shape = oox_convert(shape->additional_);

	if (!set_shape)
	{
		shape->bad_shape_ = true;
	}
}

void dr3d_scene::docx_convert(oox::docx_conversion_context & Context)
{
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
