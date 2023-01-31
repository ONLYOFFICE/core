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

#include "office_forms.h"
#include "serialize_elements.h"
#include "style_graphic_properties.h"
#include "odfcontext.h"
#include "calcs_styles.h"

#include "../../DataTypes/length.h"
#include "../../DataTypes/borderstyle.h"

#include "../../Reader/Converter/xlsx_utils.h"
#include "../../Reader/Converter/oox_drawing.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {



void draw_shape::common_xlsx_convert(oox::xlsx_conversion_context & Context)
{
	if (enhanced_geometry_)
		enhanced_geometry_->xlsx_convert(Context);

	common_draw_shape_with_text_and_styles_attlist common_draw_attlist_ = common_draw_attlists_.shape_with_text_and_styles_;

    const unsigned int z_index		= common_draw_attlist_.common_shape_draw_attlist_.draw_z_index_.get_value_or(0);
    const std::wstring name			= common_draw_attlist_.common_shape_draw_attlist_.draw_name_.get_value_or(L"");
    const std::wstring styleName	= common_draw_attlist_.common_shape_draw_attlist_.draw_style_name_.get_value_or(L"");    
    const std::wstring textStyleName = common_draw_attlist_.common_shape_draw_attlist_.draw_text_style_name_.get_value_or(L"");

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
	if (common_draw_attlist_.common_shape_draw_attlist_.draw_transform_)
	{
		std::wstring transformStr = common_draw_attlist_.common_shape_draw_attlist_.draw_transform_.get();
		xlsx_convert_transforms(transformStr, Context);
	}
////////////////////////////////////////
	if (common_draw_attlist_.common_shape_table_attlist_.table_end_cell_address_)
	{
		std::wstring end_Anchor = common_draw_attlist_.common_shape_table_attlist_.table_end_cell_address_.get();
		const double end_x_pt = common_draw_attlist_.common_shape_table_attlist_.table_end_x_.get_value_or(length(0)).get_value_unit(length::pt);
		const double end_y_pt = common_draw_attlist_.common_shape_table_attlist_.table_end_y_.get_value_or(length(0)).get_value_unit(length::pt);

		Context.get_drawing_context().set_anchor_end(end_Anchor, end_x_pt, end_y_pt);
	}
	else if (Context.in_table_cell())
	{
		const double x_pt = common_draw_attlists_.position_.svg_x_.get_value_or(length(0)).get_value_unit(length::pt);
		const double y_pt = common_draw_attlists_.position_.svg_y_.get_value_or(length(0)).get_value_unit(length::pt);

		std::wstring Anchor = oox::getCellAddress(Context.current_table_column(), Context.current_table_row());
		Context.get_drawing_context().set_anchor_start(Anchor, x_pt, y_pt);
	}
/////////////////////////////////////////////////////////////////////////////////
	std::vector<const odf_reader::style_instance *> instances;

	odf_reader::style_instance* styleInst = 
		Context.root()->odf_context().styleContainer().style_by_name(styleName, odf_types::style_family::Graphic, false/*Context.process_headers_footers_*/);
	if (styleInst)
	{
		style_instance * defaultStyle = Context.root()->odf_context().styleContainer().style_default_by_type(odf_types::style_family::Graphic);
		if (defaultStyle)instances.push_back(defaultStyle);

		instances.push_back(styleInst);
	}
	graphic_format_properties properties = calc_graphic_properties_content(instances);

////////////////////////////////////////////////////////////////////////////////////
	properties.apply_to(Context.get_drawing_context().get_properties());
	
 	for (size_t i = 0; i < additional_.size(); i++)
	{
		Context.get_drawing_context().set_property(additional_[i]);
	}
	Context.get_drawing_context().set_is_line_shape(lined_shape_);
	Context.get_drawing_context().set_is_connector_shape(connector_);
	
	oox::_oox_fill fill;
	Compute_GraphicFill(properties.common_draw_fill_attlist_, properties.style_background_image_,
																	Context.root()->odf_context().drawStyles(), fill);	
	Context.get_drawing_context().set_fill(fill);

//////////////////////////////////////////////////////////////////////////////////////	
	Context.get_text_context().start_drawing_content();

	if (word_art_)
	{
		//Context.get_text_context().start_drawing_fill(fill);
	}

	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->xlsx_convert(Context);
    }
	std::wstring text_content_ = Context.get_text_context().end_drawing_content();

	if (!text_content_.empty())
	{
		Context.get_drawing_context().set_property(_property(L"text-content", text_content_));
	}

    Context.get_drawing_context().end_drawing();    
}
void draw_rect::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_drawing_context().start_shape(2);

	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
}
void draw_ellipse::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_drawing_context().start_shape(3);

	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
}
void draw_circle::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_drawing_context().start_shape(4);

	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
}
void draw_line::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_drawing_context().start_shape(5);
	
	reset_svg_attributes();
	
	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
}


void draw_path::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	reset_svg_path();
///////////////////////////////////////////////////////////////////////
	Context.get_drawing_context().start_shape(6);

	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
}

void draw_connector::xlsx_convert(oox::xlsx_conversion_context & Context)
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
	Context.get_drawing_context().start_shape(6);

	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
}
void draw_polygon::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	reset_polygon_path();
///////////////////////////////////////////////////////////////////////
	Context.get_drawing_context().start_shape(8);

	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
}
void draw_polyline::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	reset_polyline_path();
///////////////////////////////////////////////////////////////////////
	Context.get_drawing_context().start_shape(8);

	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
}
void draw_custom_shape::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_drawing_context().start_shape(7);

	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
}
void draw_caption::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	//const std::wstring style = common_shape_draw_attlist_.draw_text_style_name_.get_value_or(L"");

	Context.get_drawing_context().start_shape(1);//rect с наваротами-атрибутами .. а-ля TextBox
	
	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
}
void draw_enhanced_geometry::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
	find_draw_type_oox();

	if (attlist_.draw_mirror_horizontal_)
	{
		Context.get_drawing_context().set_property(_property(L"flipH", *attlist_.draw_mirror_horizontal_));
	}
	if (attlist_.draw_mirror_vertical_)
	{
		Context.get_drawing_context().set_property(_property(L"flipV", *attlist_.draw_mirror_vertical_));
	}
	if (draw_type_oox_index_)
	{
		Context.get_drawing_context().set_property(_property(L"oox-geom-index", draw_type_oox_index_.get()));	
		Context.get_drawing_context().set_property(_property(L"oox-geom", bOoxType_));	

		if (word_art_ == true)
			Context.get_drawing_context().set_property(_property(L"wordArt", true));	

	}
	if (sub_type_)
	{
		Context.get_drawing_context().start_shape(sub_type_.get());
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
			Context.get_drawing_context().set_property(odf_reader::_property(L"custom_path", output_.str()));

			if (false == bStroked)
			{
				Context.get_drawing_context().set_property(odf_reader::_property(L"custom_path_s", false));
			}
			if (attlist_.drawooo_sub_view_size_)
			{
				std::vector< std::wstring > splitted;			    
				boost::algorithm::split(splitted, *attlist_.drawooo_sub_view_size_, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
				
				if (splitted.size() == 2)
				{
					int w = boost::lexical_cast<int>(splitted[0]);
					int h = boost::lexical_cast<int>(splitted[1]);
					
					Context.get_drawing_context().set_property(odf_reader::_property(L"custom_path_w", w));
					Context.get_drawing_context().set_property(odf_reader::_property(L"custom_path_h", h));
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
					
					Context.get_drawing_context().set_property(odf_reader::_property(L"custom_path_w", w));
					Context.get_drawing_context().set_property(odf_reader::_property(L"custom_path_h", h));
				}
			}
		}
		else if (!draw_type_oox_index_)
		{
			draw_type_oox_index_ = 0;
		}
	}
	if (attlist_.draw_modifiers_)
	{
		if (bOoxType_)
			Context.get_drawing_context().set_property(_property(L"oox-draw-modifiers", attlist_.draw_modifiers_.get()));	
		else
		{
		}
	}
}
void dr3d_scene::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	//const std::wstring style = common_shape_draw_attlist_.draw_text_style_name_.get_value_or(L"");

	Context.get_drawing_context().start_shape(1);//rect с наваротами-атрибутами .. а-ля TextBox
	
	common_xlsx_convert(Context);

	Context.get_drawing_context().end_shape();
}
void dr3d_light::xlsx_convert(oox::xlsx_conversion_context & Context)
{

}
void dr3d_cube::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_drawing_context().start_shape(sub_type_); //reset type
}
void dr3d_sphere::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_drawing_context().start_shape(sub_type_); //reset type

}
void draw_control::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	if (!control_id_) return;

	oox::forms_context::_state & state = Context.get_forms_context().get_state_element(*control_id_);
	if (state.id.empty()) return;

	form_element* control = dynamic_cast<form_element*>(state.element);
	if (!control) return;
	
	if (state.ctrlPropId.empty())
	{
		std::wstring target;
		state.ctrlPropId = Context.get_mediaitems()->add_control_props(target);
		
		std::wstringstream strm;		
		control->serialize_control_props(strm);

		Context.add_control_props(state.ctrlPropId, target, strm.str());
	}

	Context.get_drawing_context().start_control(state.ctrlPropId, control->object_type_);
	
	common_xlsx_convert(Context);
	
	if (control->linked_cell_)
	{
		Context.get_drawing_context().set_property(_property(L"linked_cell", control->linked_cell_.get()));
	}
	if (control->disabled_)
	{
		Context.get_drawing_context().set_property(_property(L"disabled", control->disabled_->get()));
	}
	if (control->value_)
	{
		Context.get_drawing_context().set_property(_property(L"value", control->value_.get()));
	}
	else if (control->current_value_)
	{
		Context.get_drawing_context().set_property(_property(L"value", control->current_value_.get()));
	}
	if (control->label_)
	{
		Context.get_drawing_context().set_property(_property(L"label", control->label_.get()));
		Context.get_drawing_context().set_property(_property(L"text-content", control->label_.get()));
	}
	//if (control->name_)
	//{
	//	Context.get_drawing_context().set_name(control->name_.get());
	//}
	form_value_range* value_range = dynamic_cast<form_value_range*>(control);

	if (value_range)
	{
		if (value_range->min_value_)
		{
			Context.get_drawing_context().set_property(_property(L"min_value", value_range->min_value_.get()));
		}
		if (value_range->max_value_)
		{
			Context.get_drawing_context().set_property(_property(L"max_value", value_range->max_value_.get()));
		}
		if (value_range->step_size_)
		{
			Context.get_drawing_context().set_property(_property(L"step", value_range->step_size_.get()));
		}
		if (value_range->page_step_size_)
		{
			Context.get_drawing_context().set_property(_property(L"page_step", value_range->page_step_size_.get()));
		}
		if (value_range->orientation_)
		{
			Context.get_drawing_context().set_property(_property(L"orientation", value_range->orientation_.get()));
		}
	}
	form_combobox* combobox = dynamic_cast<form_combobox*>(control);

	if (combobox)
	{
		//items_;
		if (combobox->source_cell_range_)
		{	
			Context.get_drawing_context().set_property(_property(L"cell_range", combobox->source_cell_range_.get()));
		}
		if (combobox->list_source_)
		{	
			Context.get_drawing_context().set_property(_property(L"list_source", combobox->list_source_.get()));
		}
	}
	form_listbox* listbox = dynamic_cast<form_listbox*>(control);
	if (listbox)
	{
		if (listbox->source_cell_range_)
		{	
			Context.get_drawing_context().set_property(_property(L"cell_range", listbox->source_cell_range_.get()));
		}
		if (listbox->list_source_)
		{	
			Context.get_drawing_context().set_property(_property(L"list_source", listbox->list_source_.get()));
		}
	}
	form_checkbox* checkbox = dynamic_cast<form_checkbox*>(control);
	if (checkbox)
	{
		Context.get_drawing_context().set_property(_property(L"checkbox_state", checkbox->current_state_));
	}

	//_CP_OPT(std::wstring)		label_;
	//_CP_OPT(std::wstring)		title_;
	//_CP_OPT(odf_types::Bool)	dropdown_;

	Context.get_drawing_context().end_control();
}

}
}
