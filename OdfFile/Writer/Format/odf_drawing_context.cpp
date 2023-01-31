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

#include "logging.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>

#include "odf_drawing_context.h"
#include "odf_text_context.h"
#include "odf_style_context.h"
#include "odf_conversion_context.h"
#include "office_event_listeners.h"

#include "draw_frame.h"
#include "draw_shapes.h"

#include "oox_shape_defines.h"
#include "Shapes/odf_shape_mapping.h"

#include "styles.h"

#include "style_table_properties.h"
#include "style_text_properties.h"
#include "style_paragraph_properties.h"
#include "style_graphic_properties.h"
#include "style_page_layout_properties.h"
#include "style_presentation.h"

namespace cpdoccore 
{
	using namespace odf_types;

namespace odf_writer
{

struct 	odf_group_state;
typedef shared_ptr<odf_group_state>::Type odf_group_state_ptr;

struct 	odf_group_state
{
	odf_group_state(office_element_ptr	elm_, size_t level_, odf_group_state_ptr prev)
	{
		shift_x = shift_y = x = y = cx = cy = 0;
		scale_cx = scale_cy = 1.;
		flipH = flipV = false;

		elm = elm_;
		level = level_;
		
		prev_group = prev;

		graphic_properties = NULL;
	}
	office_element_ptr	elm;
	
	size_t level;

	double shift_x;
	double shift_y;

	double x;
	double y;
	
	double cx;
	double cy;

	double scale_cx;
	double scale_cy;

	_CP_OPT(double) rotate;

	bool flipH;
	bool flipV;

	graphic_format_properties *graphic_properties;

	odf_group_state_ptr prev_group;
};

struct anchor_settings
{
	_CP_OPT(length)			svg_x_;
	_CP_OPT(length)			svg_y_;
	_CP_OPT(length)			svg_height_;
	_CP_OPT(length)			svg_width_;	

	_CP_OPT(length)			style_vertical_pos_svg_y_;
	_CP_OPT(length)			style_horizontal_pos_svg_x_;
	
	_CP_OPT(vertical_pos)	style_vertical_pos_;
	_CP_OPT(horizontal_pos) style_horizontal_pos_;

	_CP_OPT(vertical_rel)	style_vertical_rel_;
	_CP_OPT(horizontal_rel) style_horizontal_rel_;

	_CP_OPT(length)			fo_margin_left_; 
	_CP_OPT(length)			fo_margin_top_; 
	_CP_OPT(length)			fo_margin_right_; 
	_CP_OPT(length)			fo_margin_bottom_; 

	_CP_OPT(anchor_type)	anchor_type_;
	_CP_OPT(run_through)	run_through_;

	_CP_OPT(style_wrap)						style_wrap_;
    _CP_OPT(odf_types::integer_or_nolimit)	style_number_wrapped_paragraphs_;
    _CP_OPT(odf_types::Bool)				style_wrap_contour_;
    _CP_OPT(odf_types::wrap_contour_mode)	style_wrap_contour_mode_;

	void clear()
	{
		svg_x_						= boost::none;
		svg_y_						= boost::none;
		svg_height_					= boost::none;
		svg_height_					= boost::none;
		
		style_vertical_pos_svg_y_	= boost::none;
		style_horizontal_pos_svg_x_ = boost::none;
		
		anchor_type_				= boost::none;
		
		style_vertical_pos_			= boost::none;
		style_horizontal_pos_		= boost::none;
		
		style_vertical_rel_			= boost::none;
		style_horizontal_rel_		= boost::none;

		fo_margin_left_				= boost::none;
		fo_margin_top_				= boost::none;
		fo_margin_right_			= boost::none;
		fo_margin_bottom_			= boost::none;

		style_wrap_					= boost::none;
		style_wrap_contour_			= boost::none;
		style_wrap_contour_mode_	= boost::none;
		style_number_wrapped_paragraphs_ = boost::none;
	}
};	

enum _drawing_part
{
	Unknown	= 0,
	Area	= 1,
	Line	= 2,
	Shadow	= 3,
	Background = 4,
	Border = 5
};
struct odf_drawing_state
{
	void clear()
	{
		elements_.clear();
		index_base = -1;
		
		svg_x_				= boost::none;
		svg_y_				= boost::none;
		svg_height_			= boost::none;
		svg_width_			= boost::none;

		cx_					= boost::none;
		cy_					= boost::none;
		
		fill_color_			= boost::none;
		name_				= L"";
		description_		= L"";
		hidden_				= false;
		z_order_			= -1;
		
		presentation_class_			= boost::none;
		presentation_placeholder_	= boost::none;

		rotateAngle_		= boost::none;
		text_rotateAngle_	= boost::none;
		
		path_				= L"";
		view_box_			= L"";
		path_last_command_	= L"";
		path_closed_		= false;

		replacement_		= L"";
		oox_shape_preset_	= -1;
		oox_shape_.reset();

		in_group_			= false;
		text_box_tableframe_= false;

		flipH_ = flipV_ = false;

	}
	std::vector<odf_element_state>	elements_;

	_CP_OPT(length) svg_x_;
	_CP_OPT(length) svg_y_;
	_CP_OPT(length) svg_height_;
	_CP_OPT(length) svg_width_;	

	_CP_OPT(double) cx_;
	_CP_OPT(double) cy_;

	bool flipH_;
	bool flipV_;

	std::wstring	name_;
	std::wstring	description_;
	int				z_order_;
	bool			hidden_;

	_CP_OPT(double)			rotateAngle_;
	_CP_OPT(unsigned int)	fill_color_;
	_CP_OPT(int)			text_rotateAngle_;
	
	_CP_OPT(presentation_class)	presentation_class_;
	_CP_OPT(std::wstring)		presentation_placeholder_;

	std::wstring				program_;
	std::wstring				replacement_;
	std::wstring				path_;
	std::wstring				view_box_;
	std::wstring				path_last_command_;
	bool						path_closed_;
	oox_shape_ptr				oox_shape_;
//----------------------------------------------------------
	int oox_shape_preset_;
	bool in_group_;
	bool text_box_tableframe_;
	int index_base;

};

class odf_drawing_context::Impl
{
public:
	Impl(odf_conversion_context *odf_context) : odf_context_(odf_context)
    {	
		current_drawing_state_.clear();
		styles_context_ = odf_context_->styles_context();
		
		current_graphic_properties		= NULL;
		current_paragraph_properties	= NULL;
		current_text_properties			= NULL;

		width = height = x = y = 0;
		
		is_header_		= false;
		is_footer_		= false;
		is_background_	= false;
	  //некоторые свойства для объектов графики не поддерживаюися в редакторах Libre && OpenOffice.net
									//в MS Office и в нашем - проблем таких нет.
	} 
	
	odf_drawing_state				current_drawing_state_;
	_drawing_part					current_drawing_part_;
	
	std::vector<office_element_ptr> current_level_;		//постоянно меняющийся список уровней наследования

	odf_style_context				*styles_context_;
	odf_conversion_context			*odf_context_;

	bool			is_footer_;
	bool			is_header_;
	bool			is_background_;
	_CP_OPT(int)	is_presentation_;

	void				create_draw_base(eOdfDrawElements type);
	office_element_ptr	create_draw_element(eOdfDrawElements type);

	graphic_format_properties		*current_graphic_properties;
	style_paragraph_properties		*current_paragraph_properties;
	style_text_properties			*current_text_properties;

	anchor_settings						anchor_settings_;

	graphic_format_properties			preset_graphic_format_properties;

	odf_group_state_ptr					current_group_;
	
	std::vector<odf_group_state_ptr>	group_list_;	//группы
	std::vector<odf_drawing_state>		drawing_list_;	//все элементы(кроме групп) .. для удобства разделение по "топам"
	
	std::vector<office_element_ptr>		tops_elements_;

	office_element_ptr					root_element_;
	
	double x;
	double y;

	double width;
	double height;

};

//-----------------------------------------------------------------------------------------------
odf_drawing_context::odf_drawing_context(odf_conversion_context *odf_context)  
	: impl_(new  odf_drawing_context::Impl(odf_context))
{
}

odf_drawing_context::~odf_drawing_context()
{
}

office_element_ptr & odf_drawing_context::get_current_element()
{
	return impl_->current_level_.back();
}

void odf_drawing_context::set_styles_context(odf_style_context*  styles_context)
{
	impl_->styles_context_ = styles_context;
}

void odf_drawing_context::set_presentation (int type)
{
	impl_->is_presentation_ = type;
}

_CP_OPT(int) odf_drawing_context::get_presentation ()
{
	return impl_->is_presentation_;
}

void odf_drawing_context::set_footer_state(bool Val)
{
	impl_->is_footer_ = Val;
}
void odf_drawing_context::set_header_state(bool Val)
{
	impl_->is_header_ = Val;
}

void odf_drawing_context::set_background_state(bool Val)
{
	impl_->is_background_ = Val;

	impl_->current_graphic_properties = new graphic_format_properties();
	start_area_properties();
}

void odf_drawing_context::check_anchor()
{
	return;
	if ((/*impl_->is_footer_ || impl_->is_header_ ||*/ impl_->is_background_) && (impl_->anchor_settings_.run_through_) && (impl_->anchor_settings_.run_through_->get_type() == run_through::Background))
	{
		set_anchor(anchor_type::Char);
		//подозрительно на подложку страницы
		impl_->anchor_settings_.style_wrap_ = style_wrap(style_wrap::RunThrough);
		impl_->anchor_settings_.run_through_ = run_through(run_through::Background);

		if (impl_->is_footer_)
			set_vertical_pos(0);
	}
}
size_t odf_drawing_context::get_group_level()
{
	return impl_->current_level_.size();
}
void odf_drawing_context::start_group()
{
    office_element_ptr group_elm = impl_->create_draw_element(drawGroup);

	draw_g* group = dynamic_cast<draw_g*>(group_elm.get());

	size_t level = impl_->current_level_.size();
//////////////////////////
	
	odf_group_state_ptr group_state = boost::shared_ptr<odf_group_state>(new odf_group_state(group_elm, level, impl_->current_group_));
	impl_->group_list_.push_back(group_state);

	impl_->current_group_ = impl_->group_list_.back();
	
	if (false == impl_->current_level_.empty())
		impl_->current_level_.back()->add_child_element(group_elm);

	impl_->current_level_.push_back(group_elm);
	
	if (group== NULL)return;

	//если группа топовая - то данные если не записать - сотруться
	if (!impl_->current_drawing_state_.name_.empty())
		group->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_name_ = impl_->current_drawing_state_.name_;
	else
		group->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_name_ = L"";
	if (impl_->current_drawing_state_.z_order_ >= 0)
		group->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_z_index_ = impl_->current_drawing_state_.z_order_;
	//if (!impl_->current_drawing_state_.description_.empty())
	//	group->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_name_ = impl_->current_drawing_state_.description_;
	if (impl_->current_drawing_state_.hidden_)
		group->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.drawooo_display_ = L"printer";
	
	impl_->current_drawing_state_.z_order_	= -1;

////////////////////////////////////////////////////////////////////////////////////////
//////////	
	impl_->styles_context_->create_style(L"",style_family::Graphic, true, false, -1);		
	
	office_element_ptr & style_group_elm = impl_->styles_context_->last_state()->get_office_element();
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_group_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		impl_->current_graphic_properties = style_->content_.get_graphic_properties();
	}
	group_state->graphic_properties = impl_->current_graphic_properties;

	group->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_style_name_ = style_name;

	impl_->current_graphic_properties->style_wrap_ = impl_->anchor_settings_.style_wrap_;
	impl_->current_graphic_properties->style_run_through_ = impl_->anchor_settings_.run_through_;	
	impl_->current_graphic_properties->style_wrap_contour_ = impl_->anchor_settings_.style_wrap_contour_;	
	impl_->current_graphic_properties->style_wrap_contour_mode_ = impl_->anchor_settings_.style_wrap_contour_mode_;	
	impl_->current_graphic_properties->style_number_wrapped_paragraphs_ = impl_->anchor_settings_.style_number_wrapped_paragraphs_;	

	impl_->current_graphic_properties->common_vertical_pos_attlist_.style_vertical_pos_		= impl_->anchor_settings_.style_vertical_pos_;
	impl_->current_graphic_properties->common_horizontal_pos_attlist_.style_horizontal_pos_	= impl_->anchor_settings_.style_horizontal_pos_;

	impl_->current_graphic_properties->common_vertical_rel_attlist_.style_vertical_rel_		= impl_->anchor_settings_.style_vertical_rel_;
	impl_->current_graphic_properties->common_horizontal_rel_attlist_.style_horizontal_rel_	= impl_->anchor_settings_.style_horizontal_rel_;

	impl_->current_graphic_properties->common_horizontal_margin_attlist_.fo_margin_left_	= impl_->anchor_settings_.fo_margin_left_; 
	impl_->current_graphic_properties->common_vertical_margin_attlist_.fo_margin_top_		= impl_->anchor_settings_.fo_margin_top_; 
	impl_->current_graphic_properties->common_horizontal_margin_attlist_.fo_margin_right_	= impl_->anchor_settings_.fo_margin_right_; 
	impl_->current_graphic_properties->common_vertical_margin_attlist_.fo_margin_bottom_	= impl_->anchor_settings_.fo_margin_bottom_; 

	if (group)
	{
		if ((impl_->anchor_settings_.anchor_type_) && (impl_->anchor_settings_.anchor_type_->get_type() == anchor_type::Page) && level == 0)
			impl_->anchor_settings_.anchor_type_ = anchor_type::Paragraph;

		group->common_draw_attlists_.shape_with_text_and_styles_.common_text_anchor_attlist_.type_ = impl_->anchor_settings_.anchor_type_;
		impl_->anchor_settings_.anchor_type_ = boost::none;
	}
	
	if (level == 0)
	{
		impl_->tops_elements_.push_back(group_elm);
	}
}
void odf_drawing_context::end_group()
{
	if (impl_->group_list_.empty())return;
	
	impl_->current_group_ = impl_->group_list_.back()->prev_group;
	impl_->group_list_.pop_back();

	if (!impl_->current_level_.empty())
		impl_->current_level_.pop_back();
}


void odf_drawing_context::clear()
{
	impl_->root_element_ = office_element_ptr();

	impl_->anchor_settings_.clear();

}

void odf_drawing_context::start_drawing()
{
	//if (impl_->current_level_.empty())
	{
		impl_->current_drawing_state_.svg_x_ = impl_->anchor_settings_.svg_x_;
		impl_->current_drawing_state_.svg_y_ = impl_->anchor_settings_.svg_y_;
		
		if (impl_->anchor_settings_.style_horizontal_pos_svg_x_)//?????
		{
			impl_->current_drawing_state_.svg_x_ = /**impl_->current_drawing_state_.svg_x_  +*/ *impl_->anchor_settings_.style_horizontal_pos_svg_x_;
			impl_->anchor_settings_.style_horizontal_pos_svg_x_= boost::none;
		}
		if (impl_->anchor_settings_.style_vertical_pos_svg_y_)
		{
			impl_->current_drawing_state_.svg_y_ = /**impl_->current_drawing_state_.svg_y_  +*/ *impl_->anchor_settings_.style_vertical_pos_svg_y_;
			impl_->anchor_settings_.style_vertical_pos_svg_y_= boost::none;
		}

		impl_->current_drawing_state_.svg_width_ = impl_->anchor_settings_.svg_width_;
		impl_->current_drawing_state_.svg_height_ = impl_->anchor_settings_.svg_height_;
	}
	//else 
	if (false == impl_->current_level_.empty() && impl_->current_group_)
	{
		impl_->current_drawing_state_.in_group_ = true;
	}

}
void odf_drawing_context::end_drawing_background(odf_types::common_draw_fill_attlist & common_draw_attlist)
{
	end_area_properties();
	if (impl_->current_drawing_state_.elements_.empty() == false) return;

	if (!impl_->is_background_ || !impl_->current_graphic_properties) return;

	common_draw_attlist.apply_from(impl_->current_graphic_properties->common_draw_fill_attlist_);
	
	delete impl_->current_graphic_properties;
	
	impl_->current_graphic_properties	= NULL;
	impl_->current_drawing_state_.clear();
}
void odf_drawing_context::end_drawing()
{
	int index = impl_->current_drawing_state_.index_base < 0 ? 0 : impl_->current_drawing_state_.index_base;

	draw_base* draw = impl_->current_drawing_state_.elements_.empty() ? NULL : dynamic_cast<draw_base*>(impl_->current_drawing_state_.elements_[index].elm.get());
	if (draw)
	{
		if (impl_->current_drawing_state_.presentation_class_ || impl_->current_drawing_state_.presentation_placeholder_)
		{
			_CP_OPT(std::wstring) draw_layer;
			if (impl_->is_presentation_.get() > 0)
			{//masters				
				draw_layer = L"backgroundobjects";

				if (!impl_->current_drawing_state_.presentation_class_)	
					impl_->current_drawing_state_.presentation_class_ = presentation_class::outline;

				draw->common_draw_attlists_.shape_with_text_and_styles_.common_presentation_attlist_.presentation_user_transformed_ = true;
				draw->common_draw_attlists_.shape_with_text_and_styles_.common_presentation_attlist_.presentation_placeholder_ = false;
			}
			else
			{//slide
				if (impl_->current_drawing_state_.presentation_class_)
				{
					draw_layer = L"layout";
					
					if (impl_->current_drawing_state_.oox_shape_preset_ != 3000)
						draw->common_draw_attlists_.shape_with_text_and_styles_.common_presentation_attlist_.presentation_placeholder_ = true;
				}
			}
			draw->common_draw_attlists_.shape_with_text_and_styles_.common_presentation_attlist_.presentation_class_ = impl_->current_drawing_state_.presentation_class_;
			draw->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_layer_ = draw_layer;
		}
		if (!impl_->current_drawing_state_.name_.empty())
			draw->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_name_ = impl_->current_drawing_state_.name_;
		if (impl_->current_drawing_state_.z_order_ >= 0)
			draw->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_z_index_ = impl_->current_drawing_state_.z_order_;
		if (impl_->current_drawing_state_.hidden_)
			draw->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.drawooo_display_ = L"printer";

		std::wstring strTransform;

		_CP_OPT(double) rotate = impl_->current_drawing_state_.rotateAngle_;
		
		if (impl_->current_drawing_state_.in_group_ && impl_->current_group_)
		{
			odf_group_state_ptr gr = impl_->current_group_;

			while(gr)
			{
				impl_->current_drawing_state_.flipH_ = impl_->current_drawing_state_.flipH_ ^ gr->flipH;
				impl_->current_drawing_state_.flipV_ = impl_->current_drawing_state_.flipV_ ^ gr->flipV;
				
				gr = gr->prev_group;
			}			
			
			if (impl_->current_group_->rotate)
				rotate = (rotate ? *rotate : 0) + *impl_->current_group_->rotate;

		}
        double x = impl_->current_drawing_state_.svg_x_ ? impl_->current_drawing_state_.svg_x_->get_value() : 0;
        double y = impl_->current_drawing_state_.svg_y_ ? impl_->current_drawing_state_.svg_y_->get_value() : 0;

		if (rotate)
		{
			double angle = *rotate;//impl_->current_drawing_state_.rotateAngle_ ? *impl_->current_drawing_state_.rotateAngle_ : 0;
			
			length new_x;
			length new_y;
			
			if (impl_->current_drawing_state_.svg_width_ && impl_->current_drawing_state_.svg_height_)
			{
				length cx = *impl_->current_drawing_state_.svg_width_;
				length cy = *impl_->current_drawing_state_.svg_height_;
 
                new_x = (cx / 2.) - ((cx / 2.) * cos(-angle) - (cy / 2.) * sin(-angle) );
                new_y = (cy / 2.) - ((cx / 2.) * sin(-angle) + (cy / 2.) * cos(-angle) );
			}

			strTransform += std::wstring(L"rotate(") + boost::lexical_cast<std::wstring>(*rotate) + std::wstring(L")");

			if (impl_->current_drawing_state_.svg_x_ && impl_->current_drawing_state_.svg_y_)
			{
				length pos_x = *impl_->current_drawing_state_.svg_x_ + new_x;
				length pos_y = *impl_->current_drawing_state_.svg_y_ + new_y;
                
				strTransform +=   std::wstring(L" translate(")  + boost::lexical_cast<std::wstring>(pos_x)
								+ std::wstring(L",")            + boost::lexical_cast<std::wstring>(pos_y)
								+ std::wstring(L")") ;
				
				impl_->current_drawing_state_.svg_x_ = boost::none;
				impl_->current_drawing_state_.svg_y_ = boost::none;
			}
		}

		if (strTransform.empty() == false && !draw->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_transform_)
			draw->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_transform_ = strTransform;

		draw->common_draw_attlists_.position_.svg_x_ = impl_->current_drawing_state_.svg_x_;
		draw->common_draw_attlists_.position_.svg_y_ = impl_->current_drawing_state_.svg_y_;

		draw->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_		= impl_->current_drawing_state_.svg_height_;
		draw->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_		= impl_->current_drawing_state_.svg_width_;
	}
///////////////////////////////////////////////////////
	presentation_placeholder * placeholder = impl_->current_drawing_state_.elements_.empty() ? NULL : dynamic_cast<presentation_placeholder*>(impl_->current_drawing_state_.elements_[index].elm.get());
	if (placeholder)
	{
		placeholder->presentation_object_	= impl_->current_drawing_state_.presentation_class_;
		placeholder->svg_x_					= impl_->current_drawing_state_.svg_x_;
		placeholder->svg_y_					= impl_->current_drawing_state_.svg_y_;

		placeholder->svg_height_			= impl_->current_drawing_state_.svg_height_;
		placeholder->svg_width_				= impl_->current_drawing_state_.svg_width_;
	}

	draw_custom_shape* custom = dynamic_cast<draw_custom_shape*>(draw);
	if (custom)
	{
		draw_enhanced_geometry* enhan = dynamic_cast<draw_enhanced_geometry*>(custom->draw_enhanced_geometry_.get());
		if(enhan)
		{
			if (impl_->current_drawing_state_.flipV_) enhan->attlist_.draw_mirror_vertical_ = true;
			if (impl_->current_drawing_state_.flipH_) enhan->attlist_.draw_mirror_horizontal_ = true;
		}
	}else
	{
		//не поддерживается :( - нужно считать искажения на простейшие фигуры - линии, ректы, эллипсы 
	}

	if (impl_->current_graphic_properties)
	{
		impl_->current_graphic_properties->common_vertical_pos_attlist_.style_vertical_pos_		= impl_->anchor_settings_.style_vertical_pos_;
		impl_->current_graphic_properties->common_horizontal_pos_attlist_.style_horizontal_pos_	= impl_->anchor_settings_.style_horizontal_pos_;

		impl_->current_graphic_properties->common_vertical_rel_attlist_.style_vertical_rel_		= impl_->anchor_settings_.style_vertical_rel_;
		impl_->current_graphic_properties->common_horizontal_rel_attlist_.style_horizontal_rel_	= impl_->anchor_settings_.style_horizontal_rel_;

		impl_->current_graphic_properties->common_horizontal_margin_attlist_.fo_margin_left_	= impl_->anchor_settings_.fo_margin_left_; 
		impl_->current_graphic_properties->common_vertical_margin_attlist_.fo_margin_top_		= impl_->anchor_settings_.fo_margin_top_; 
		impl_->current_graphic_properties->common_horizontal_margin_attlist_.fo_margin_right_	= impl_->anchor_settings_.fo_margin_right_; 
		impl_->current_graphic_properties->common_vertical_margin_attlist_.fo_margin_bottom_	= impl_->anchor_settings_.fo_margin_bottom_; 

		if (draw && !impl_->current_drawing_state_.in_group_)
			draw->common_draw_attlists_.shape_with_text_and_styles_.common_text_anchor_attlist_.type_ = impl_->anchor_settings_.anchor_type_;

		impl_->current_graphic_properties->style_wrap_			= impl_->anchor_settings_.style_wrap_;
		impl_->current_graphic_properties->style_run_through_	= impl_->anchor_settings_.run_through_;	
		impl_->current_graphic_properties->style_wrap_contour_	= impl_->anchor_settings_.style_wrap_contour_;	
		impl_->current_graphic_properties->style_wrap_contour_mode_ = impl_->anchor_settings_.style_wrap_contour_mode_;	
		impl_->current_graphic_properties->style_number_wrapped_paragraphs_ = impl_->anchor_settings_.style_number_wrapped_paragraphs_;	
	}
	//if (impl_->anchor_settings_.anchor_type_ && impl_->anchor_settings_.anchor_type_->get_type()== anchor_type::AsChar)
	//{
	//	draw->common_draw_attlists_.position_.svg_x_ = boost::none;
	//	draw->common_draw_attlists_.position_.svg_y_ = boost::none;
	//}
///////////////////////////////////////////////////		
	impl_->drawing_list_.push_back(impl_->current_drawing_state_);
	
	if (	impl_->current_drawing_state_.elements_.empty() == false 
		&&	impl_->current_drawing_state_.elements_[0].level == 0 
		&&	impl_->current_drawing_state_.elements_[0].elm ) // не base_index -> см draw_a
	{
		impl_->tops_elements_.push_back(impl_->current_drawing_state_.elements_[0].elm);
	}

///////////////
	impl_->current_drawing_state_.clear();
	
	impl_->current_graphic_properties	= NULL;
	impl_->current_paragraph_properties = NULL;
	impl_->current_text_properties		= NULL;
}

////////////////////////////////////////////////////////////////////////////
office_element_ptr odf_drawing_context::Impl::create_draw_element(eOdfDrawElements type)
{
	office_element_ptr element;
	switch(type)
	{
	case drawFrame:
		create_element(L"draw", L"frame", element, odf_context_);
		break;
	case drawCaption:
		create_element(L"draw", L"caption", element, odf_context_);
		break;
	case drawRect:
		create_element(L"draw", L"rect", element, odf_context_);
		break;
	case drawEllipse:
		create_element(L"draw", L"ellipse", element, odf_context_);
		break;
	case drawCircle:
		create_element(L"draw", L"circle", element, odf_context_);
		break;
	case drawLine:
		create_element(L"draw", L"line", element, odf_context_);
		break;
	case drawPath:
		create_element(L"draw", L"path", element, odf_context_);
		break;
	case drawCustom:
		create_element(L"draw", L"custom-shape", element, odf_context_);
		break;
	case drawPolygon:
		create_element(L"draw", L"polygon", element, odf_context_);
		break;
	case drawConnector:
	case drawCurveConnector:
	{
		create_element(L"draw", L"connector", element, odf_context_);
		draw_connector* connector = dynamic_cast<draw_connector*>(element.get());

		if (type == drawCurveConnector)	connector->draw_connector_attlist_.draw_type_ = L"curve";
		else							connector->draw_connector_attlist_.draw_type_ = L"standard";
	}break;
	case drawGroup:
		create_element(L"draw", L"g", element, odf_context_);
		break;
	}
	if (root_element_ == NULL) root_element_ = element;

	return element;
}
void odf_drawing_context::Impl::create_draw_base(eOdfDrawElements type)
{	
    office_element_ptr draw_elm = create_draw_element(type);

	draw_base* draw = dynamic_cast<draw_base*>(draw_elm.get());
	if (draw == NULL)return;
//////////	
	if (is_presentation_ && current_drawing_state_.presentation_class_)
		styles_context_->create_style(L"", style_family::Presentation, true, false, -1);		
	else
		styles_context_->create_style(L"", style_family::Graphic, true, false, -1);		
	
	office_element_ptr & style_shape_elm = styles_context_->last_state()->get_office_element();
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_shape_elm.get());
	if (style_)
	{
        style_name = style_->style_name_;
		current_graphic_properties = style_->content_.get_graphic_properties();
	}

	if (is_presentation_ && current_drawing_state_.presentation_class_)
		draw->common_draw_attlists_.shape_with_text_and_styles_.common_presentation_attlist_.presentation_style_name_ = style_name;
	else
		draw->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_style_name_ = style_name;
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	size_t level = current_level_.size();
	
	if (false == current_level_.empty())
		current_level_.back()->add_child_element(draw_elm);

	current_level_.push_back(draw_elm);

	odf_element_state state(draw_elm, style_name, style_shape_elm, level);

	if (current_drawing_state_.index_base < 0)
	{
		current_drawing_state_.index_base = (int)current_drawing_state_.elements_.size();
	}
	current_drawing_state_.elements_.push_back(state);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (type == drawCustom)
		current_drawing_state_.oox_shape_ = oox_shape_ptr(new oox_shape());
}

void odf_drawing_context::start_shape(int ooxDrawPreset)
{
	impl_->current_drawing_state_.oox_shape_preset_ = ooxDrawPreset;
	
	if (ooxDrawPreset < sizeof(Shape_Types_Mapping)/sizeof(_sh_typ))
	{
		impl_->create_draw_base(Shape_Types_Mapping[ooxDrawPreset].second);
	}
	else if (ooxDrawPreset == 1000)
	{
		impl_->create_draw_base(drawCustom);//пока кастом .. потом переделать на path, что правильнее
	}
	else if (ooxDrawPreset == 1001)
	{
		impl_->create_draw_base(drawPath); //path
	}
	else if (ooxDrawPreset == 2000)
	{
		start_text_box();
	}
	else if (ooxDrawPreset == 3000)
	{
		start_image(L"");
	}
	else if (ooxDrawPreset > 2000 && ooxDrawPreset < 3000)	//custom text path
	{
		impl_->create_draw_base(drawCustom);
	}
}
bool odf_drawing_context::is_text_box()
{
	if (impl_->current_drawing_state_.oox_shape_preset_ >= 2000 && impl_->current_drawing_state_.oox_shape_preset_ < 3000)
		return true;

	return false;
}

bool odf_drawing_context::is_wordart()
{
	if (impl_->current_drawing_state_.oox_shape_preset_ > 2000 && impl_->current_drawing_state_.oox_shape_preset_ < 3000)
		return true;	

	return false;
}

bool odf_drawing_context::change_text_box_2_wordart()
{
	if (impl_->current_drawing_state_.oox_shape_preset_ > 2000 && impl_->current_drawing_state_.oox_shape_preset_ < 3000)
		return true;	

	if (impl_->current_drawing_state_.presentation_class_)	return false;	
	if (impl_->current_drawing_state_.text_box_tableframe_)	return false;	
	if (impl_->current_drawing_state_.elements_.empty())	return false;

	draw_text_box* t = dynamic_cast<draw_text_box*>(impl_->current_drawing_state_.elements_.back().elm.get());
	if (t)
	{
	//------------------------------------------------------------------------
		office_element_ptr draw_elm = impl_->create_draw_element(drawCustom);
		
		draw_base* draw = dynamic_cast<draw_base*>(draw_elm.get());
		if (draw == NULL)	return false;

		size_t sz = impl_->current_level_.size();
		if (sz < 2)			return false;

		size_t sz_state = impl_->current_drawing_state_.elements_.size();
		if (sz_state < 2)	return false;

		if (sz > 2) //в группе ??
		{
			draw_base* draw_old = dynamic_cast<draw_base*>(impl_->current_level_[sz-2].get());
			if (draw_old)
			{
				draw_old->content_[draw_old->content_.size() - 1] = draw_elm;
			}
		}
	//----------------------------------------------

		odf_element_state state = impl_->current_drawing_state_.elements_[sz_state - 2];
		state.elm = draw_elm;

		draw->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_style_name_ = state.style_name;

		impl_->current_level_.erase (impl_->current_level_.end() - 2, impl_->current_level_.end());
		impl_->current_level_.push_back(draw_elm);

		impl_->current_drawing_state_.elements_.erase(impl_->current_drawing_state_.elements_.end() - 2, impl_->current_drawing_state_.elements_.end());
		impl_->current_drawing_state_.elements_.push_back( state);

		impl_->current_drawing_state_.oox_shape_preset_ = 2031;//plain text

		if (sz == 2)	impl_->root_element_ = draw_elm;
		return true;
	}
	draw_rect* s = dynamic_cast<draw_rect*>(impl_->current_drawing_state_.elements_.back().elm.get());
	if (s)
	{
	//------------------------------------------------------------------------
		office_element_ptr draw_elm = impl_->create_draw_element(drawCustom);
		
		draw_base* draw = dynamic_cast<draw_base*>(draw_elm.get());
		if (draw == NULL)	return false;

		size_t sz = impl_->current_level_.size();
		if (sz < 1)			return false;

		size_t sz_state = impl_->current_drawing_state_.elements_.size();
		if (sz_state < 1)	return false;

		if (sz > 1) //в группе ??
		{
			draw_base* draw_old = dynamic_cast<draw_base*>(impl_->current_level_[sz - 1].get());
			if (draw_old)
			{
				if (draw_old->content_.size() > 1)
					draw_old->content_[draw_old->content_.size() - 1] = draw_elm;
			}
		}
	//----------------------------------------------

		odf_element_state state = impl_->current_drawing_state_.elements_[sz_state - 1];
		state.elm = draw_elm;

		draw->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_style_name_ = state.style_name;

		impl_->current_level_.erase (impl_->current_level_.end() - 1, impl_->current_level_.end());
		impl_->current_level_.push_back(draw_elm);

		impl_->current_drawing_state_.elements_.erase(impl_->current_drawing_state_.elements_.end() - 1, impl_->current_drawing_state_.elements_.end());
		impl_->current_drawing_state_.elements_.push_back( state);

		impl_->current_drawing_state_.oox_shape_preset_ = 2031;//plain text

		if (sz == 1)	impl_->root_element_ = draw_elm;
		return true;
	}

	return false;
}

void odf_drawing_context::end_shape()
{
	if (impl_->current_drawing_state_.elements_.empty()) 
		return;

	if (impl_->current_drawing_state_.oox_shape_preset_ == 2000) return end_text_box();
	if (impl_->current_drawing_state_.oox_shape_preset_ == 3000) return end_image();
	//вторичные, вычисляемые свойства шейпов

	if (isLineShape())
	{
		impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_ = draw_fill::none;
	}

	draw_path* path = dynamic_cast<draw_path*>(impl_->current_level_.back().get());
	if (path)
	{
		if (impl_->current_drawing_state_.view_box_.empty() && impl_->current_drawing_state_.svg_width_ && impl_->current_drawing_state_.svg_height_)
			set_viewBox( impl_->current_drawing_state_.svg_width_->get_value_unit(length::cm) * 1000, 
						 impl_->current_drawing_state_.svg_height_->get_value_unit(length::cm) *1000);
		
		if (!impl_->current_drawing_state_.path_.empty()) 		path->draw_path_attlist_.svg_d_ = impl_->current_drawing_state_.path_;
		if (!impl_->current_drawing_state_.view_box_.empty())	path->draw_path_attlist_.svg_viewbox_ = impl_->current_drawing_state_.view_box_;
	}
////////////////////////////////////////////////////////////////////////////////////////////
	draw_line* line = dynamic_cast<draw_line*>(impl_->current_level_.back().get());
	if (line)
	{
		if (!line->draw_line_attlist_.svg_x1_) line->draw_line_attlist_.svg_x1_ = impl_->current_drawing_state_.svg_x_;
		if (!line->draw_line_attlist_.svg_y1_) line->draw_line_attlist_.svg_y1_ = impl_->current_drawing_state_.svg_y_;
		
		if (line->draw_line_attlist_.svg_x1_ && impl_->current_drawing_state_.svg_width_ && !line->draw_line_attlist_.svg_x2_)
			line->draw_line_attlist_.svg_x2_ = line->draw_line_attlist_.svg_x1_.get() + impl_->current_drawing_state_.svg_width_.get();
		
		if (line->draw_line_attlist_.svg_y1_ && impl_->current_drawing_state_.svg_height_ && !line->draw_line_attlist_.svg_y2_)
			line->draw_line_attlist_.svg_y2_ = line->draw_line_attlist_.svg_y1_.get() + impl_->current_drawing_state_.svg_height_.get();
		
		_CP_OPT(double) rotate = impl_->current_drawing_state_.rotateAngle_;
		if (impl_->current_drawing_state_.in_group_ && impl_->current_group_)
		{			
			if (impl_->current_group_->rotate)
				rotate = (rotate ? *rotate : 0) + *impl_->current_group_->rotate;
		}

		if (rotate)
		{
			double angle = *rotate;//impl_->current_drawing_state_.rotateAngle_ ? *impl_->current_drawing_state_.rotateAngle_ : 0;

            if (line->draw_line_attlist_.svg_x1_)
                line->draw_line_attlist_.svg_x1_ = *line->draw_line_attlist_.svg_x1_ / 2 - (*line->draw_line_attlist_.svg_x1_ / 2 * cos(-angle) - *line->draw_line_attlist_.svg_y1_ / 2 * sin(-angle) );
            if (line->draw_line_attlist_.svg_y1_)
                line->draw_line_attlist_.svg_y1_ = *line->draw_line_attlist_.svg_y1_ / 2 - (*line->draw_line_attlist_.svg_x1_ / 2 * sin(-angle) + *line->draw_line_attlist_.svg_y1_ / 2 * cos(-angle) );

            if (line->draw_line_attlist_.svg_x2_)
                line->draw_line_attlist_.svg_x2_ = *line->draw_line_attlist_.svg_x2_ / 2 - (*line->draw_line_attlist_.svg_x2_ / 2 * cos(-angle) - *line->draw_line_attlist_.svg_y2_ / 2 * sin(-angle) );
            if (line->draw_line_attlist_.svg_y2_)
                line->draw_line_attlist_.svg_y2_ = *line->draw_line_attlist_.svg_y2_ / 2 - (*line->draw_line_attlist_.svg_x2_ / 2 * sin(-angle) + *line->draw_line_attlist_.svg_y2_ / 2 * cos(-angle) );

			line->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_transform_= L"";

			impl_->current_drawing_state_.rotateAngle_ = boost::none;
		}
		impl_->current_drawing_state_.svg_height_ = boost::none;
		impl_->current_drawing_state_.svg_width_ = boost::none;
		
		impl_->current_drawing_state_.svg_x_ = boost::none;
		impl_->current_drawing_state_.svg_y_ = boost::none;
	
		if (impl_->current_drawing_state_.flipV_)
		{
			_CP_OPT(length) tmp;
			
			tmp = line->draw_line_attlist_.svg_y1_;
			line->draw_line_attlist_.svg_y1_ = line->draw_line_attlist_.svg_y2_;
			line->draw_line_attlist_.svg_y2_ = tmp;
		}
		if (impl_->current_drawing_state_.flipH_)
		{
			_CP_OPT(length) tmp;
			
			tmp = line->draw_line_attlist_.svg_x1_;
			line->draw_line_attlist_.svg_x1_ = line->draw_line_attlist_.svg_x2_;
			line->draw_line_attlist_.svg_x2_ = tmp;
		}
	}

////////////////////////////////////////////////////////////////////////////////////
	draw_connector* connector = dynamic_cast<draw_connector*>(impl_->current_level_.back().get());
	if (connector)
	{
		if (!connector->draw_connector_attlist_.draw_type_) 
			connector->draw_connector_attlist_.draw_type_ = L"line";
	}

//////////////////////////////////////////////////////////////////////////////////////////////////
	draw_custom_shape* custom = dynamic_cast<draw_custom_shape*>(impl_->current_level_.back().get());
	if (custom)
	{
		std::wstring sub_type;
		bool text_shape = false;

		if (impl_->current_drawing_state_.oox_shape_preset_ < sizeof(Shape_Types_Mapping)/sizeof(_sh_typ))
		{
			sub_type = Shape_Types_Mapping[impl_->current_drawing_state_.oox_shape_preset_].first;
		}
		else if (impl_->current_drawing_state_.oox_shape_preset_ > 2000 && impl_->current_drawing_state_.oox_shape_preset_ < 3000)// 3000 - все равно сюда не попадет
		{
			text_shape = true;
		}
		//else
		//{
		//	sub_type = L"polyline";
		//	line_always_present = true;
		//}
		
		office_element_ptr enhanced_elm;
		create_element(L"draw", L"enhanced-geometry", enhanced_elm, impl_->odf_context_);
		
		start_element(enhanced_elm);
		
		draw_enhanced_geometry* enhanced = dynamic_cast<draw_enhanced_geometry*>(enhanced_elm.get());
		if (enhanced)
		{
			if (text_shape)
			{
				enhanced->attlist_.draw_text_path_same_letter_heights_ = false;
				enhanced->attlist_.draw_text_path_scale_ = L"path" ;
				enhanced->attlist_.draw_text_path_mode_	= L"shape" ;
				enhanced->attlist_.draw_text_path_		= true; 

				enhanced->attlist_.draw_text_rotate_angle_ = impl_->current_drawing_state_.text_rotateAngle_;
			}

			if (!impl_->current_drawing_state_.path_.empty())
			{
				enhanced->attlist_.draw_enhanced_path_ =impl_->current_drawing_state_.path_;
			}
			if (!impl_->current_drawing_state_.view_box_.empty())
			{
				enhanced->svg_viewbox_ = impl_->current_drawing_state_.view_box_;
			}
			if (!sub_type.empty())
			{
				enhanced->attlist_.draw_type_ = sub_type;
			}
			else
			{
				oox_shape_ptr shape_define = oox_shape::create(impl_->current_drawing_state_.oox_shape_preset_);
				
				if (!shape_define) 
					shape_define = impl_->current_drawing_state_.oox_shape_;
				
				if (shape_define)
				{
					if (impl_->current_drawing_state_.oox_shape_ && impl_->current_drawing_state_.oox_shape_->view_box)
						enhanced->svg_viewbox_										= impl_->current_drawing_state_.oox_shape_->view_box;
					else
						enhanced->svg_viewbox_										= shape_define->view_box;

					enhanced->attlist_.draw_type_			= shape_define->odf_type_name;
					enhanced->attlist_.draw_text_areas_		= shape_define->text_areas;
					
					enhanced->attlist_.draw_glue_points_	= shape_define->glue_points;
					enhanced->attlist_.draw_sub_view_size_	= shape_define->sub_view_size;

					if (impl_->current_drawing_state_.oox_shape_ && !impl_->current_drawing_state_.oox_shape_->modifiers.empty())
					{
						enhanced->attlist_.draw_modifiers_ = impl_->current_drawing_state_.oox_shape_->modifiers;
					}
					else // обязательно нужны дефолтовые
						enhanced->attlist_.draw_modifiers_ = shape_define->modifiers;

					if (!shape_define->enhanced_path.empty())
						enhanced->attlist_.draw_enhanced_path_ = shape_define->enhanced_path;
					else
						enhanced->attlist_.draw_enhanced_path_ = impl_->current_drawing_state_.path_;

					for (size_t i = 0; i < shape_define->equations.size(); i++)
					{
						office_element_ptr elm_eq;
						create_element(L"draw", L"equation", elm_eq, impl_->odf_context_);					
						start_element(elm_eq);
						draw_equation* eq = dynamic_cast<draw_equation*>(elm_eq.get());
						if (eq)
						{
							eq->draw_equation_attlist_.draw_name_ = shape_define->equations[i].name;
							eq->draw_equation_attlist_.draw_formula_ = shape_define->equations[i].formula;
						}
						end_element();
					}
				//-----------------------------
					for (size_t i = 0; i < shape_define->handles.size(); i++)
					{
						office_element_ptr elm_h;
						create_element(L"draw", L"handle", elm_h, impl_->odf_context_);					
						start_element(elm_h);
						draw_handle* h = dynamic_cast<draw_handle*>(elm_h.get());
						if (h)
						{
							if (shape_define->handles[i].position)	h->draw_handle_attlist_.draw_handle_position_		 = *shape_define->handles[i].position;
							if (shape_define->handles[i].y_minimum)	h->draw_handle_attlist_.draw_handle_range_y_minimum_ = *shape_define->handles[i].y_minimum;
							if (shape_define->handles[i].y_maximum)	h->draw_handle_attlist_.draw_handle_range_y_maximum_ = *shape_define->handles[i].y_maximum;
							if (shape_define->handles[i].x_minimum)	h->draw_handle_attlist_.draw_handle_range_x_minimum_ = *shape_define->handles[i].x_minimum;
							if (shape_define->handles[i].x_maximum)	h->draw_handle_attlist_.draw_handle_range_x_maximum_ = *shape_define->handles[i].x_maximum;

							if (shape_define->handles[i].r_maximum)	h->draw_handle_attlist_.draw_handle_radius_range_maximum_ = *shape_define->handles[i].r_maximum;
							if (shape_define->handles[i].r_minimum)	h->draw_handle_attlist_.draw_handle_radius_range_minimum_ = *shape_define->handles[i].r_minimum;
						}
						end_element();
					}
				}
			}
		}
		end_element();
	}
	if (impl_->current_drawing_state_.flipV_)
	{
		impl_->current_graphic_properties->style_mirror_ = std::wstring(L"vertical");
	}
	if (impl_->current_drawing_state_.flipH_)
	{
		if (impl_->current_graphic_properties->style_mirror_)
			impl_->current_graphic_properties->style_mirror_ = *impl_->current_graphic_properties->style_mirror_ + std::wstring(L" horizontal");
		else
			impl_->current_graphic_properties->style_mirror_ = std::wstring(L"horizontal");
	}

	end_element();
}

bool odf_drawing_context::isCustomClosed()
{
	return impl_->current_drawing_state_.path_closed_;
}

bool odf_drawing_context::isLineShape()
{
	if (impl_->current_level_.empty()) return false;

	draw_line* line = dynamic_cast<draw_line*>(impl_->current_level_.back().get());
	if (line) return true;

	draw_connector* connector = dynamic_cast<draw_connector*>(impl_->current_level_.back().get());
	if (connector) return true;

	draw_path* path = dynamic_cast<draw_path*>(impl_->current_level_.back().get());
	if (path) return true;

	switch(impl_->current_drawing_state_.oox_shape_preset_)
	{
	case 20:	//SimpleTypes::shapetypeBentConnector2:
	case 21:	//SimpleTypes::shapetypeBentConnector3:
	case 22:	//SimpleTypes::shapetypeBentConnector4:
	case 23:	//SimpleTypes::shapetypeBentConnector5:
	case 47:	//SimpleTypes::shapetypeCurvedConnector2:
	case 48:	//SimpleTypes::shapetypeCurvedConnector3:
	case 49:	//SimpleTypes::shapetypeCurvedConnector4:
	case 50:	//SimpleTypes::shapetypeCurvedConnector5:
	case 31:	//SimpleTypes::shapetypeBracketPair
	case 30:	//SimpleTypes::shapetypeBracePair
	case 148:	//SimpleTypes::shapetypeRightBracket
	case 111:	//SimpleTypes::shapetypeLeftBracket
	case 147:	//SimpleTypes::shapetypeRightBrace
	case 110:	//SimpleTypes::shapetypeLeftBrace
	case 18:	//SimpleTypes::shapetypeArc
		return true;
	case 1000:
		if (impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_.get_value_or(draw_fill(draw_fill::solid)).get_type() == draw_fill::none)
			return true;
	default:
		return false;
	}
	return false;
}

void odf_drawing_context::corrected_line_fill() //for vml objects
{
	if (!impl_->current_graphic_properties)return;
	
	if ( !impl_->current_graphic_properties->draw_stroke_ &&
			 !impl_->current_graphic_properties->svg_stroke_color_ &&
			 !impl_->current_graphic_properties->draw_stroke_dash_ &&
			 !impl_->current_graphic_properties->svg_stroke_width_ && 
			 !impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_ &&
			 !impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_color_ && 
			 !impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_image_name_ &&
			!impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_hatch_name_ &&
			!impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_gradient_name_)
	{
		impl_->current_graphic_properties->svg_stroke_width_ = length(length(1. ,length::pt).get_value_unit(length::cm),length::cm);
		impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_ = draw_fill::none;;
	}
}
void odf_drawing_context::start_frame()
{
	impl_->create_draw_base(drawFrame);
	
	if (impl_->current_graphic_properties)
	{
		if (!impl_->anchor_settings_.style_wrap_)
		{
			impl_->anchor_settings_.style_wrap_ = style_wrap(style_wrap::RunThrough);
		}
		impl_->current_graphic_properties->common_background_color_attlist_.fo_background_color_ = odf_types::background_color(odf_types::background_color::Transparent);
	}
}

void odf_drawing_context::end_frame()
{
	end_element();
}
/////////////////////
void odf_drawing_context::start_element(office_element_ptr elm, office_element_ptr  style_elm)
{
    size_t level = (int)impl_->current_level_.size();
	
	//если  фейковый предыдущий уровень (для сохранения порядка выше) - привязывааем к уровню выше

	for (int i = impl_->current_level_.size() - 1; elm && i >= 0; i--)
	{
		if (impl_->current_level_[i])
		{
			impl_->current_level_[i]->add_child_element(elm);
			break;
		}
	}

	std::wstring style_name;
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		impl_->current_graphic_properties = style_->content_.get_graphic_properties();

		if (impl_->current_drawing_state_.name_.empty())
		{
			impl_->current_drawing_state_.name_ = std::wstring(L"Object_") + style_name;
		}
	}

	odf_element_state state(elm, style_name, style_elm, level);
	impl_->current_drawing_state_.elements_.push_back(state);

	impl_->current_level_.push_back(elm);
}
void odf_drawing_context::end_element()
{
	impl_->current_level_.pop_back();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void odf_drawing_context::start_area_properties(bool bBackground)
{
	impl_->current_drawing_part_ = bBackground ? Background : Area;
}
void odf_drawing_context::end_area_properties()
{
	impl_->current_drawing_part_ = Unknown;
}
void odf_drawing_context::start_line_properties(bool reset)
{
	impl_->current_drawing_part_ = Line;
	if (reset)
		impl_->current_graphic_properties->draw_stroke_ = boost::none;
}
void odf_drawing_context::end_line_properties()
{
	impl_->current_drawing_part_ = Unknown;
}
//void odf_drawing_context::start_shadow_properties()
//{
//	impl_->current_drawing_part_ = Shadow;
//}
//void odf_drawing_context::end_shadow_properties()
//{
//	impl_->current_drawing_part_ = Unknown;
//}
////////////////////////////////////////////////////////////////////
void odf_drawing_context::set_name(const std::wstring & name)
{
	if (name.empty()) return;

	impl_->current_drawing_state_.name_ = name;
}
void odf_drawing_context::set_description (const std::wstring & description)
{
	if (description.empty()) return;

	impl_->current_drawing_state_.description_ = description;
}
void odf_drawing_context::set_hidden (bool bVal)
{
	impl_->current_drawing_state_.hidden_ = bVal;
}
void odf_drawing_context::set_opacity(double percent_)
{
	if (!impl_->current_graphic_properties)return;

	switch(impl_->current_drawing_part_)
	{
	case Area:
	{
		if (impl_->current_drawing_state_.oox_shape_preset_  == 3000)
			impl_->current_graphic_properties->common_draw_fill_attlist_.draw_image_opacity_ = percent(percent_);
		else
			impl_->current_graphic_properties->common_draw_fill_attlist_.draw_opacity_ = percent(percent_);
	}break;
	case Line:
	{
		impl_->current_graphic_properties->svg_stroke_opacity_ = percent(percent_);
	}break;
	}
}
void odf_drawing_context::set_grayscale()
{
	if (!impl_->current_graphic_properties)return;

	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_color_mode_ = L"greyscale";
}
void odf_drawing_context::set_white_balance(double red, double green, double blue)
{
	if (!impl_->current_graphic_properties)return;

	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_red_ = red;
	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_green_ = green;
	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_blue_ = blue;
}

void odf_drawing_context::set_shadow(int type, std::wstring hexColor, _CP_OPT(double) opacity, double dist_pt, double dist_pt_y )
{
	if (!impl_->current_graphic_properties)return;

	size_t res = hexColor.find(L"#");
	if (std::wstring::npos == res) 
		hexColor = std::wstring(L"#") + hexColor;

	impl_->current_graphic_properties->common_shadow_attlist_.draw_shadow_offset_x_ = length(length(dist_pt, length::pt).get_value_unit(length::cm), length::cm);
	
	if (dist_pt_y > 0)
		impl_->current_graphic_properties->common_shadow_attlist_.draw_shadow_offset_y_ = length(length(dist_pt_y, length::pt).get_value_unit(length::cm), length::cm);
	else
		impl_->current_graphic_properties->common_shadow_attlist_.draw_shadow_offset_y_ = length(length(dist_pt, length::pt).get_value_unit(length::cm), length::cm);
	
	impl_->current_graphic_properties->common_shadow_attlist_.draw_shadow_ = shadow_type1(shadow_type1::Visible);
	if (opacity) impl_->current_graphic_properties->common_shadow_attlist_.draw_shadow_opacity_ = *opacity;

	impl_->current_graphic_properties->common_shadow_attlist_.draw_shadow_color_ = hexColor;
}

void odf_drawing_context::set_placeholder_id (std::wstring val)
{
	if (!impl_->is_presentation_) return;

	impl_->current_drawing_state_.presentation_placeholder_ = val;
}
void odf_drawing_context::set_placeholder_type (int val)
{
	if (!impl_->is_presentation_) return;
	
	switch(val)
	{
		case 0:
		{
			if (impl_->is_presentation_ == 2) //notes master
				impl_->current_drawing_state_.presentation_class_ = presentation_class::notes; 
			else if (impl_->is_presentation_ == 3) //handout master
				impl_->current_drawing_state_.presentation_class_ = presentation_class::handout; 
			else
				impl_->current_drawing_state_.presentation_class_ = presentation_class::outline; 		
		}break;
		case 1:		impl_->current_drawing_state_.presentation_class_ = presentation_class::chart; 		break;
		case 2:		impl_->current_drawing_state_.presentation_class_ = presentation_class::graphic; 	break;
		case 3:		impl_->current_drawing_state_.presentation_class_ = presentation_class::title;		break;
		case 4:		impl_->current_drawing_state_.presentation_class_ = presentation_class::graphic;	break;
		case 5:		impl_->current_drawing_state_.presentation_class_ = presentation_class::date_time;	break;
		case 6:		impl_->current_drawing_state_.presentation_class_ = presentation_class::footer;		break;
		case 7:		impl_->current_drawing_state_.presentation_class_ = presentation_class::header;		break;
		case 8:		impl_->current_drawing_state_.presentation_class_ = presentation_class::object;		break;
		case 9:		impl_->current_drawing_state_.presentation_class_ = presentation_class::object;		break;
		case 10:	impl_->current_drawing_state_.presentation_class_ = presentation_class::graphic;	break;
		case 11:	impl_->current_drawing_state_.presentation_class_ = presentation_class::page;		break;
		case 12:	impl_->current_drawing_state_.presentation_class_ = presentation_class::page_number;break;
		case 13:	impl_->current_drawing_state_.presentation_class_ = presentation_class::subtitle;	break;
		case 14:	impl_->current_drawing_state_.presentation_class_ = presentation_class::table;		break;
		case 15:	impl_->current_drawing_state_.presentation_class_ = presentation_class::title;		break;
		default:		
			impl_->current_drawing_state_.presentation_class_ = presentation_class::outline; 			break;
	}
	//todooo draw_layer for master for sldnum, datetime ...
}
void odf_drawing_context::set_group_fill()
{
	if (!impl_->current_graphic_properties) return;
	
	if (!impl_->current_group_) return;
	if (!impl_->current_group_->graphic_properties) return;

	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_ = impl_->current_group_->graphic_properties->common_draw_fill_attlist_.draw_fill_;
	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_color_ = impl_->current_group_->graphic_properties->common_draw_fill_attlist_.draw_fill_color_;
	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_image_name_ = impl_->current_group_->graphic_properties->common_draw_fill_attlist_.draw_fill_image_name_;
	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_hatch_name_ = impl_->current_group_->graphic_properties->common_draw_fill_attlist_.draw_fill_hatch_name_;
	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_gradient_name_ = impl_->current_group_->graphic_properties->common_draw_fill_attlist_.draw_fill_gradient_name_;
}
void odf_drawing_context::set_no_fill()
{
	if (!impl_->current_graphic_properties) return;

	switch(impl_->current_drawing_part_)
	{
	case Area:
		if ((/*impl_->is_footer_ || impl_->is_header_ ||*/ impl_->is_background_) && 
			(impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_) && 
			(impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_->get_type() == draw_fill::bitmap))
		{
		}
		else
			impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_ = draw_fill::none;
		break;
	case Line:
		//impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_ = draw_fill::none;??????
		impl_->current_graphic_properties->draw_stroke_ = line_style (line_style::None);
		break;
	}
}
void odf_drawing_context::set_type_fill(int type)
{
	if (!impl_->current_graphic_properties)return;

	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_ = (draw_fill::type)type;
}

void odf_drawing_context::set_fill_color(unsigned int Color)
{
	impl_->current_drawing_state_.fill_color_ = Color;
}

_CP_OPT(unsigned int) odf_drawing_context::get_fill_color()
{
	return impl_->current_drawing_state_.fill_color_;
}
_CP_OPT(odf_types::color) odf_drawing_context::get_line_color()
{
	return impl_->current_graphic_properties->svg_stroke_color_ ;
}
void odf_drawing_context::set_line_color(std::wstring hexColor)
{
	if (!impl_->current_graphic_properties)	return;
	if (hexColor.empty()) return;
	
	size_t res = hexColor.find(L"#");
	if (std::wstring::npos == res)
		hexColor = std::wstring(L"#") + hexColor;

	impl_->current_graphic_properties->svg_stroke_color_ = hexColor;
}
void odf_drawing_context::set_solid_fill(std::wstring hexColor)
{
	if (!impl_->current_graphic_properties)	return;
	if (hexColor.empty()) return;
	
	size_t res = hexColor.find(L"#");
	if (std::wstring::npos == res)
		hexColor = std::wstring(L"#") + hexColor;

	switch(impl_->current_drawing_part_)
	{
		case Background:
		{
			impl_->current_graphic_properties->common_background_color_attlist_.fo_background_color_ = color(hexColor); 
		}break;
		case Area:
		{
			impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_color_ = hexColor;
			//impl_->current_graphic_properties->common_background_color_attlist_.fo_background_color_	= color(hexColor); - default transparent
			//последнее нужно - что если будут вводить текст - под текстом будет цвет фона (или он поменяется в полях текста)
			
			if ((/*impl_->is_footer_ || impl_->is_header_ ||*/ impl_->is_background_) && 
				(impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_) && 
				(impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_->get_type() == draw_fill::bitmap))
			{
			}
			else
				impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_ = draw_fill::solid;
		}break;
		case Line:
		{
			impl_->current_graphic_properties->svg_stroke_color_ =  hexColor;
			if (!impl_->current_graphic_properties->draw_stroke_)
				impl_->current_graphic_properties->draw_stroke_ = line_style(line_style::Solid);//default
			if (!impl_->current_graphic_properties->svg_stroke_width_)
				impl_->current_graphic_properties->svg_stroke_width_ = length(length(1, length::pt).get_value_unit(length::cm), length::cm);//default
		}break;
	}
}
	
void odf_drawing_context::set_z_order(int id)
{
	if (id < 0)
		id = 0x7fffffff + id;
	
	//if (impl_->current_drawing_state_.in_group)
	//{
	//	id += (impl_->current_group_->level+1) *100;
	//}

	impl_->current_drawing_state_.z_order_ = id + 1;
}
void odf_drawing_context::set_path(std::wstring path_string)
{
	impl_->current_drawing_state_.path_ = path_string;
}
void odf_drawing_context::add_path_element(std::wstring command, std::wstring strE)
{
	XmlUtils::replace_all(strE, L"gd", L"?f");
	
	if (command != impl_->current_drawing_state_.path_last_command_)
	{
		impl_->current_drawing_state_.path_ += command;
		if (!strE.empty())
			impl_->current_drawing_state_.path_ += L" ";

		impl_->current_drawing_state_.path_last_command_ = command;
	}

	impl_->current_drawing_state_.path_ += strE + L" ";

	if (command == L"Z") 
		impl_->current_drawing_state_.path_closed_ = true;
	if (command == L"N") 
		impl_->current_drawing_state_.path_last_command_.clear();
}
void odf_drawing_context::add_modifier (std::wstring modifier)
{
	if (!impl_->current_drawing_state_.oox_shape_) return;

	boost::algorithm::to_lower(modifier);

	if ( std::wstring::npos != modifier.find(L"val ") )
		modifier = modifier.substr(4);
	
	impl_->current_drawing_state_.oox_shape_->modifiers += modifier + L" ";
}
int GetFormulaType2(const WCHAR& c1, const WCHAR& c2) 
{
	switch (c1)
	{
	case (WCHAR)'*':		return 0;
	case (WCHAR)'+':		return ((WCHAR)'-' == c2) ? 1 : 2;
	case (WCHAR)'?':		return 3;
	case (WCHAR)'a':		return ((WCHAR)'b' == c2) ? 4 : 5;
	case (WCHAR)'c':		return ((WCHAR)'a' == c2) ? 6 : 7;
	case (WCHAR)'m':		return ((WCHAR)'a' == c2) ? 8 : (((WCHAR)'i' == c2) ? 16 : 9);
	case (WCHAR)'p':		return 10;
	case (WCHAR)'s':		return ((WCHAR)'a' == c2) ? 11 : (((WCHAR)'i' == c2) ? 12 : 13);					
	case (WCHAR)'t':		return 14;
	case (WCHAR)'v':		return 15;
	default:
		break;
	}
	return 0;
}

void odf_drawing_context::set_textarea (std::wstring l, std::wstring t, std::wstring r, std::wstring b)
{
	if (!impl_->current_drawing_state_.oox_shape_) return;

	impl_->current_drawing_state_.oox_shape_->text_areas = l + L" " + t + L" " + r + L" " + b;

	XmlUtils::replace_all(impl_->current_drawing_state_.oox_shape_->text_areas, L"gd", L"?f");
}
void odf_drawing_context::add_handle (std::wstring x, std::wstring y, std::wstring refX, std::wstring refY,
						std::wstring minX, std::wstring maxX, std::wstring minY, std::wstring maxY)
{
	if (!impl_->current_drawing_state_.oox_shape_) return;
	
	oox_shape::_handle h;

	XmlUtils::replace_all(x, L"gd", L"?f");
	XmlUtils::replace_all(y, L"gd", L"?f");

	h.position = x + L" " + y;

	if (!maxX.empty())	h.x_maximum= maxX;
	if (!minX.empty())	h.x_minimum= minX;

	if (!maxY.empty())	h.y_maximum= maxY;
	if (!minY.empty())	h.y_minimum= minY;

	impl_->current_drawing_state_.oox_shape_->handles.push_back(h);
}

void odf_drawing_context::add_formula (std::wstring name, std::wstring fmla)
{
	if (!impl_->current_drawing_state_.oox_shape_) return;

	size_t nStart = 0;
	size_t nCurrent = 0;

    const wchar_t* pData = fmla.c_str();

	int nFound = 0, x = 0, y = 0;

	std::wstring val[4];

	while (nCurrent < fmla.length())
	{
		if (pData[nCurrent] == (WCHAR)' ')
		{
			if (nStart < nCurrent)
			{
				if (0 == nFound)
				{
					if ((nCurrent - nStart) > 1)
					{
						x = GetFormulaType2(pData[nStart], pData[nStart + 1]);
					}
					else
					{
						x = 0;
					}
				}
				else
				{
					val[nFound-1] = std::wstring( pData + nStart, (ULONG)(nCurrent - nStart));
				}
				nStart = nCurrent + 1;
				++nFound;
			}
		}
		++nCurrent;
	}
	if (nStart < nCurrent)
	{
		if (0 == nFound)
		{
			if ((nCurrent - nStart) > 1)
			{
				y = GetFormulaType2(pData[nStart], pData[nStart + 1]);
			}
			else
			{
				y = 0;
			}
		}
		else
		{
			val[nFound-1] = std::wstring( pData + nStart, (ULONG)(nCurrent - nStart));
		}
	}
	std::wstring odf_fmla = L"";

	switch(x)
	{
		case 0:
			odf_fmla = val[0] + L"*" + val[1] + L"/" + val[2]; 
			break;
		case 1:
			odf_fmla = val[0] + L"+" + val[1] + L"-" + val[2]; 
			break;
		case 2:
			odf_fmla = val[0] + L"+" + val[1]; 
			break;
		case 3:
			odf_fmla = L"if(";
			for (int i = 0; i < nFound - 1; i++)
			{
				odf_fmla += val[i] + L",";
			}
			odf_fmla += val[nFound-1] + L")"; break;
		case 4:
			odf_fmla = L"abs(" + val[0] + L")";
			break;
		case 7:
			odf_fmla = val[0] + L"*cos(pi*(" + val[1] + L")/10800000)";
			break;
		case 12:
			odf_fmla = val[0] + L"*sin(pi*(" + val[1] + L")/10800000)";
			break;
		case 13:
			odf_fmla = L"sqrt(" + val[0] + L")";
			break;
		case 15:
			odf_fmla = val[0];
			break;
		default:
			odf_fmla = fmla;
			break;
	}

	//XmlUtils::replace_all(odf_fmla, L"gd", L"?f");
	XmlUtils::replace_all(odf_fmla, L"h", L"logheight");
	XmlUtils::replace_all(odf_fmla, L"w", L"logwidth");
	XmlUtils::replace_all(odf_fmla, L"adj", L"$");
	//XmlUtils::replace_all(name, L"gd", L"f");

	impl_->current_drawing_state_.oox_shape_->add(name, odf_fmla);
}

void odf_drawing_context::set_viewBox (double W, double H)
{
	if (W  < 0.01)
	{
		if (impl_->current_drawing_state_.svg_width_)
			W = impl_->current_drawing_state_.svg_width_->get_value_unit(length::emu);
	}
	if (H < 0.01)
	{
		if (impl_->current_drawing_state_.svg_height_)
			H = impl_->current_drawing_state_.svg_height_->get_value_unit(length::emu);
	}
	impl_->current_drawing_state_.view_box_ = std::wstring(L"0 0 ") + std::to_wstring((int)W) + L" " + std::to_wstring((int)H); 
	
	if (impl_->current_drawing_state_.oox_shape_)
	{
		impl_->current_drawing_state_.oox_shape_->view_box  = impl_->current_drawing_state_.view_box_;

		if (impl_->current_drawing_state_.oox_shape_->sub_view_size)
			impl_->current_drawing_state_.oox_shape_->sub_view_size = *impl_->current_drawing_state_.oox_shape_->sub_view_size + L" " + std::to_wstring((int)W) + L" " + std::to_wstring((int)H);
		else
			impl_->current_drawing_state_.oox_shape_->sub_view_size = std::to_wstring((int)W) + L" " + std::to_wstring((int)H);
	}
}
void odf_drawing_context::set_flip_H(bool bVal)
{
	impl_->current_drawing_state_.flipH_ = bVal;
}
void odf_drawing_context::set_flip_V(bool bVal)
{
	impl_->current_drawing_state_.flipV_ = bVal;
}

void odf_drawing_context::set_rotate(double dVal)
{
	if (dVal > 180) dVal = dVal - 360;
	double dRotate = dVal / 180. * 3.14159265358979323846;
	impl_->current_drawing_state_.rotateAngle_ = dRotate;
}

void odf_drawing_context::set_drawings_rect(_CP_OPT(double) x_pt, _CP_OPT(double) y_pt, _CP_OPT(double) width_pt, _CP_OPT(double) height_pt)// "- 1" не задано
{
	//хороший тон сохранить все размеры в см (хотя можно и в другой системе)
	if (x_pt)
	{
		impl_->x = *x_pt;
		impl_->anchor_settings_.svg_x_ = length(length(*x_pt,length::pt).get_value_unit(length::cm),length::cm);
	}
	if (y_pt)
	{
		impl_->y = *y_pt;
		impl_->anchor_settings_.svg_y_ = length(length(*y_pt,length::pt).get_value_unit(length::cm),length::cm);
	}

	if (height_pt)	
	{
		impl_->height = *height_pt;
		impl_->anchor_settings_.svg_height_	= length(length(*height_pt,length::pt).get_value_unit(length::cm),length::cm);	
	}
	if (width_pt)
	{
		impl_->width = *width_pt;
		impl_->anchor_settings_.svg_width_	= length(length(*width_pt,length::pt).get_value_unit(length::cm),length::cm);	
	}
}
void odf_drawing_context::set_object_background(bool Val)
{
	if (Val)
	{
		impl_->anchor_settings_.run_through_ = run_through(run_through::Background);
		//impl_->anchor_settings_.style_wrap_ = style_wrap(style_wrap::RunThrough);
	}
}
void odf_drawing_context::set_object_foreground(bool Val)
{
	if (Val)
	{
		if (impl_->is_footer_|| impl_->is_header_ || impl_->is_background_)
		{
			impl_->anchor_settings_.run_through_ = run_through(run_through::Background);
		}
		else
		{
			impl_->anchor_settings_.run_through_ = run_through(run_through::Foreground);
		}
		//impl_->anchor_settings_.style_wrap_ = style_wrap(style_wrap::RunThrough);
	}
}

void odf_drawing_context::set_margin_left	(double valPt)
{
	impl_->anchor_settings_.fo_margin_left_ = length(length(valPt,length::pt).get_value_unit(length::cm),length::cm);
}
void odf_drawing_context::set_margin_right	(double valPt)
{
	impl_->anchor_settings_.fo_margin_right_ = length(length(valPt,length::pt).get_value_unit(length::cm),length::cm);
}
void odf_drawing_context::set_margin_top	(double valPt)
{
	impl_->anchor_settings_.fo_margin_top_ = length(length(valPt,length::pt).get_value_unit(length::cm),length::cm);
}
void odf_drawing_context::set_margin_bottom	(double valPt)
{
	impl_->anchor_settings_.fo_margin_bottom_ = length(length(valPt,length::pt).get_value_unit(length::cm),length::cm);
}
void odf_drawing_context::set_anchor(int  type)
{
	if (impl_->current_drawing_state_.in_group_) return;
	
	if ((impl_->is_footer_|| impl_->is_header_ || impl_->is_background_) && type == anchor_type::Page)
	{
		type = anchor_type::Paragraph;
	}

	impl_->anchor_settings_.anchor_type_ = anchor_type((anchor_type::type)type);
}
anchor_type::type odf_drawing_context::get_anchor()
{
	if (impl_->anchor_settings_.anchor_type_) return impl_->anchor_settings_.anchor_type_->get_type();
	else return anchor_type::AsChar;
}
//////////////////////////////////////////////////////////////////////////////////////
void odf_drawing_context::set_vertical_rel(int from)
{
	vertical_rel::type type;

	switch(from)
	{
		case 0:	type = vertical_rel::Baseline;			break;//	relfromvBottomMargin ???
		case 1:	type = vertical_rel::ParagraphContent;	break;//	relfromvInsideMargin ???
		case 2:	type = vertical_rel::Baseline;			break;//	relfromvLine          
		case 3:	type = vertical_rel::PageContent; 		break;//	relfromvMargin     //Paragraph
		case 4:	type = vertical_rel::Baseline;			break;//	relfromvOutsideMargin ???
		case 5:	type = vertical_rel::Page;				break;//	relfromvPage          
		case 6:	type = vertical_rel::Paragraph;			break;//	relfromvParagraph    
		case 7:	type = vertical_rel::Baseline;			break;//	relfromvTopMargin   ???  
	}

	impl_->anchor_settings_.style_vertical_rel_ = vertical_rel(type);

	if (!impl_->anchor_settings_.style_vertical_pos_)
		impl_->anchor_settings_.style_vertical_pos_ = vertical_pos(vertical_pos::FromTop);//default
}
void odf_drawing_context::set_vertical_pos(int align)
{
	vertical_pos::type type;
	switch(align)
	{
		case 0:	type =	vertical_pos::Bottom;		break;//alignvBottom  = 0,
		case 1:	type =	vertical_pos::Middle;		break;//alignvCenter  = 1,
		case 2:	type =	vertical_pos::FromTop;		break;//alignvInside  = 2, ???
		case 3:	type =	vertical_pos::Below;		break;//alignvOutside = 3, ???
		case 4:	type =	vertical_pos::Top;			break;//alignvTop     = 4
	}
	impl_->anchor_settings_.style_vertical_pos_ = vertical_pos(type);
}
void odf_drawing_context::set_vertical_pos( double offset_pt)
{
	impl_->anchor_settings_.style_vertical_pos_svg_y_ = length(length(offset_pt,length::pt).get_value_unit(length::cm),length::cm);

	impl_->y = offset_pt;
}
void odf_drawing_context::set_horizontal_rel(int from)
{
	horizontal_rel::type type;
	switch(from)
	{
		case 0:	type = horizontal_rel::Char;				break;	//	relfromhCharacter     = 0,
		case 1:	type = horizontal_rel::Paragraph;			break;	//	relfromhColumn        = 1,
		case 2:	type = horizontal_rel::Paragraph;			break;	//	relfromhInsideMargin  = 2, ???
		case 3:	type = horizontal_rel::PageStartMargin;		break;	//	relfromhLeftMargin    = 3,
		case 4:	type = horizontal_rel::PageContent;			break;	//	relfromhMargin        = 4, ??? //ParagraphStartMargin
		case 5:	type = horizontal_rel::ParagraphEndMargin;	break;	//	relfromhOutsideMargin = 5,
		case 6:	type = horizontal_rel::Page;				break;	//	relfromhPage          = 6, ???
		case 7:	type = horizontal_rel::PageEndMargin;		break;	//	relfromhRightMargin   = 7
		case 8: type = horizontal_rel::PageContent;			break;
	}
	impl_->anchor_settings_.style_horizontal_rel_ = horizontal_rel(type);
	
	if (!impl_->anchor_settings_.style_horizontal_pos_) //default
		impl_->anchor_settings_.style_horizontal_pos_ = horizontal_pos(horizontal_pos::FromLeft);
}
void odf_drawing_context::set_horizontal_pos(int align)
{
	horizontal_pos::type type;

	switch(align)
	{
		case 0:	type =	horizontal_pos::Center;		break;//alignhCenter  = 0,
		case 1:	type =	horizontal_pos::FromInside;	break;//alignhInside  = 1,
		case 2:	type =	horizontal_pos::FromLeft;	break;//alignhLeft    = 2,
		case 3:	type =	horizontal_pos::Outside;	break;//alignhOutside = 3,
		case 4:	type =	horizontal_pos::Right;		
			if (impl_->anchor_settings_.style_horizontal_rel_ && impl_->anchor_settings_.style_horizontal_rel_->get_type() == horizontal_rel::ParagraphStartMargin)
				impl_->anchor_settings_.style_horizontal_rel_ = horizontal_rel::ParagraphEndMargin;
			break;//alignhRight   = 4
	}
	impl_->anchor_settings_.style_horizontal_pos_ = horizontal_pos(type);
}
void odf_drawing_context::set_horizontal_pos(double offset_pt)
{
	impl_->anchor_settings_.style_horizontal_pos_svg_x_ = length(length(offset_pt, length::pt).get_value_unit(length::cm), length::cm);
	impl_->x = offset_pt;
}
void odf_drawing_context::set_default_wrap_style()
{
	if (impl_->is_header_ || impl_->is_footer_ || impl_->is_background_)
	{
		impl_->anchor_settings_.style_wrap_ = style_wrap::RunThrough;
	}
}
void odf_drawing_context::set_wrap_style(style_wrap::type type)
{
	impl_->anchor_settings_.style_wrap_ = style_wrap(type);
}
void odf_drawing_context::set_wrap_contour()
{
	impl_->anchor_settings_.style_wrap_contour_ = true;
	impl_->anchor_settings_.style_wrap_contour_mode_ = wrap_contour_mode(wrap_contour_mode::Full);
	impl_->anchor_settings_.style_number_wrapped_paragraphs_ = integer_or_nolimit(integer_or_nolimit::NoLimit);

}
void odf_drawing_context::set_overlap (bool val)
{
	if (val)
	{
		impl_->anchor_settings_.style_wrap_ = style_wrap(style_wrap::RunThrough);//??
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////void odf_drawing_context::set_group_shift(double x, double y)
void odf_drawing_context::set_group_shift (double x, double y)
{
	if ( impl_->group_list_.empty() )return;

	impl_->current_group_->shift_y = y;
	impl_->current_group_->shift_x = x;
}
void odf_drawing_context::set_group_scale (double cx, double cy)
{
	if ( impl_->group_list_.empty() )return;

	impl_->current_group_->scale_cy = cy;
	impl_->current_group_->scale_cx = cx;
}
void odf_drawing_context::set_group_position(_CP_OPT(double) x, _CP_OPT(double) y, _CP_OPT(double) change_x, _CP_OPT(double) change_y)
{
	if ( impl_->group_list_.empty() )return;

	if ( !x || !y ) return;

    if (change_x && impl_->current_group_->x)
        impl_->current_group_->x = *change_x;
    if (change_y && impl_->current_group_->y)
        impl_->current_group_->y = *change_y;

    impl_->current_group_->shift_x = (*x /impl_->current_group_->scale_cx - change_x.get_value_or(0)) ;
    impl_->current_group_->shift_y = (*y /impl_->current_group_->scale_cy - change_y.get_value_or(0)) ;
}

void odf_drawing_context::set_group_size( _CP_OPT(double) cx, _CP_OPT(double) cy, _CP_OPT(double) change_cx, _CP_OPT(double) change_cy)
{
	if ( impl_->group_list_.empty() )return;
	
	if (change_cx && cx)
		impl_->current_group_->scale_cx = *cx / *change_cx;
	
	if (change_cy && cy)
		impl_->current_group_->scale_cy = *cy / *change_cy;

    if (change_cx && impl_->current_group_->cx)
        impl_->current_group_->cx = *change_cx;
	else if (cx)
		impl_->current_group_->cx = *cx;
    
	if (change_cy && impl_->current_group_->cy)
        impl_->current_group_->cy = *change_cy;
	else if (cy)
		impl_->current_group_->cy = *cy;
}
void odf_drawing_context::set_group_flip_V(bool bVal)
{
	if ( impl_->group_list_.empty() )return;

	impl_->current_group_->flipV = bVal;
}
void odf_drawing_context::set_group_z_order(int Val)
{
	if ( impl_->group_list_.empty() )return;

	draw_g* group = dynamic_cast<draw_g*>(impl_->current_group_->elm.get());

	if (Val >= 0)
		group->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_z_index_ = Val;

}
void odf_drawing_context::set_group_name(const std::wstring & name)
{
	if ( impl_->group_list_.empty() )return;

	draw_g* group = dynamic_cast<draw_g*>(impl_->current_group_->elm.get());

	if ( !name.empty() )
		group->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_name_ = name;
}
void odf_drawing_context::set_group_flip_H(bool bVal)
{
	if ( impl_->group_list_.empty() )return;

	impl_->current_group_->flipH = bVal;
}

void odf_drawing_context::set_group_rotate(int iVal)
{
	if ( impl_->group_list_.empty() )return;
	
	odf_group_state_ptr gr = impl_->current_group_;

	double dRotate = (360 - iVal)/180. * 3.14159265358979323846;

	int step = 2;
	while (gr && step > 0)
	{
		dRotate += gr->rotate ? *gr->rotate : 0;
		gr = gr->prev_group;
		step--;
	}
	impl_->current_group_->rotate = dRotate;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void odf_drawing_context::set_position_line(_CP_OPT(double) & x_pt, _CP_OPT(double) & y_pt, _CP_OPT(double) & x2_pt, _CP_OPT(double) & y2_pt)
{
	draw_line* line = dynamic_cast<draw_line*>(impl_->current_level_.back().get());
	if (line == NULL) return;

	if (impl_->current_drawing_state_.in_group_ && impl_->current_group_ && x_pt)
		x_pt = *x_pt * impl_->current_group_->scale_cx + impl_->current_group_->shift_x ;			
			// +  (impl_->current_group_->flipH ? (impl_->current_group_->cx - 2 * x_pt): 0);
	
	if (x_pt && !line->draw_line_attlist_.svg_x1_) 
		line->draw_line_attlist_.svg_x1_ = length(length(*x_pt,length::pt).get_value_unit(length::cm),length::cm);

	if (impl_->current_drawing_state_.in_group_ && impl_->current_group_ && y_pt)
		y_pt = *y_pt * impl_->current_group_->scale_cy + impl_->current_group_->shift_y;
			 //+  (impl_->current_group_->flipV ? (impl_->current_group_->cy - 2 * y_pt): 0);

	if (y_pt && !line->draw_line_attlist_.svg_y1_) 
		line->draw_line_attlist_.svg_y1_ = length(length(*y_pt,length::pt).get_value_unit(length::cm),length::cm);

///////////////////////////////////////
	if (impl_->current_drawing_state_.in_group_ && impl_->current_group_ && x2_pt)
		x2_pt = *x2_pt * impl_->current_group_->scale_cx + impl_->current_group_->shift_x ;			
			// +  (impl_->current_group_->flipH ? (impl_->current_group_->cx - 2 * x_pt): 0);

	if (x2_pt && !line->draw_line_attlist_.svg_x2_) line->draw_line_attlist_.svg_x2_ = length(length(*x2_pt,length::pt).get_value_unit(length::cm),length::cm);

	if (impl_->current_drawing_state_.in_group_ && impl_->current_group_ && y2_pt)
		y2_pt = *y2_pt * impl_->current_group_->scale_cy + impl_->current_group_->shift_y;
			 //+  (impl_->current_group_->flipV ? (impl_->current_group_->cy - 2 * y_pt): 0);

	if (y2_pt && !line->draw_line_attlist_.svg_y2_) line->draw_line_attlist_.svg_y2_ = length(length(*y2_pt,length::pt).get_value_unit(length::cm),length::cm);

}
void odf_drawing_context::get_position(_CP_OPT(double) & x_pt, _CP_OPT(double) & y_pt)
{
	if (impl_->current_drawing_state_.svg_x_ && impl_->current_drawing_state_.svg_y_)
	{
		x_pt	= impl_->current_drawing_state_.svg_x_->get_value_unit(length::pt);
		y_pt	= impl_->current_drawing_state_.svg_y_->get_value_unit(length::pt);
	}
	else if (impl_->anchor_settings_.svg_x_ && impl_->anchor_settings_.svg_y_)
	{
		x_pt	= impl_->anchor_settings_.svg_x_->get_value_unit(length::pt);
		y_pt	= impl_->anchor_settings_.svg_y_->get_value_unit(length::pt);
	}
}
void odf_drawing_context::set_position(_CP_OPT(double) & x_pt, _CP_OPT(double) & y_pt)
{
	double x = x_pt ? *x_pt : 0;
	double y = y_pt ? *y_pt : 0;

	//double cx = *impl_->current_drawing_state_.cx_;
	//double cy = *impl_->current_drawing_state_.cy_;
	
	if (impl_->current_drawing_state_.in_group_)
	{
		for( int i = (int)impl_->group_list_.size() - 1; i >= 0 ; i--)
		{			
			//if (impl_->group_list_[i]->rotate)
			//{
			//	x = (x * cos(-impl_->group_list_[i]->rotate.get()) - y * sin(-impl_->group_list_[i]->rotate.get()) );
			//	y = (x * sin(-impl_->group_list_[i]->rotate.get()) + y * cos(-impl_->group_list_[i]->rotate.get()) );
			//}
			x = (x + impl_->group_list_[i]->shift_x) * impl_->group_list_[i]->scale_cx ;	
			y = (y + impl_->group_list_[i]->shift_y) * impl_->group_list_[i]->scale_cy;

			//cx *= impl_->group_list_[i]->scale_cx;
			//cy *= impl_->group_list_[i]->scale_cy;
		}
	}
	else
	{
		if (false == impl_->group_list_.empty())
		{
			impl_->current_group_->shift_x = x / impl_->current_group_->scale_cx - impl_->current_group_->shift_x;
			impl_->current_group_->shift_y = y / impl_->current_group_->scale_cy - impl_->current_group_->shift_y;
		}
	}
	if (!impl_->current_drawing_state_.svg_x_ || impl_->current_drawing_state_.in_group_)
	{
		impl_->current_drawing_state_.svg_x_ = length(length(x, length::pt).get_value_unit(length::cm), length::cm);
	}
	if (!impl_->current_drawing_state_.svg_y_ || impl_->current_drawing_state_.in_group_)
	{
		impl_->current_drawing_state_.svg_y_ = length(length(y, length::pt).get_value_unit(length::cm), length::cm);
	}

}
void odf_drawing_context::get_size( _CP_OPT(double) & width_pt, _CP_OPT(double) & height_pt)
{
	if (impl_->current_drawing_state_.svg_width_ && impl_->current_drawing_state_.svg_height_)
	{
		width_pt	= impl_->current_drawing_state_.svg_width_->get_value_unit(length::pt);
		height_pt	= impl_->current_drawing_state_.svg_height_->get_value_unit(length::pt);
	}
	else if (impl_->anchor_settings_.svg_width_ && impl_->anchor_settings_.svg_height_)
	{
		width_pt	= impl_->anchor_settings_.svg_width_->get_value_unit(length::pt);
		height_pt	= impl_->anchor_settings_.svg_height_->get_value_unit(length::pt);
	}
}
void odf_drawing_context::set_size( _CP_OPT(double) & width_pt, _CP_OPT(double) & height_pt, bool reset_always)
{
	impl_->current_drawing_state_.cx_ = width_pt;
	impl_->current_drawing_state_.cy_ = height_pt;
	
	if (impl_->current_drawing_state_.in_group_)
	{
		if (width_pt)
		{

			for( int i = (int)impl_->group_list_.size() - 1; i >= 0 ; i--)
			{
				width_pt  = *width_pt * impl_->group_list_[i]->scale_cx;
			}
			impl_->current_drawing_state_.svg_width_ = length(length(*width_pt,length::pt).get_value_unit(length::cm), length::cm);
		}
		if (height_pt)
		{
			for( int i = (int)impl_->group_list_.size() - 1; i >= 0 ; i--)
			{
				height_pt = *height_pt * impl_->group_list_[i]->scale_cy;
			}
			impl_->current_drawing_state_.svg_height_= length(length(*height_pt,length::pt).get_value_unit(length::cm), length::cm);	
		}
	}
	else
	{
		if (false == impl_->group_list_.empty())
		{
			if (impl_->current_group_->scale_cx)
				impl_->current_group_->scale_cx = *width_pt / impl_->current_group_->scale_cx;
			
			if (impl_->current_group_->scale_cy)
				impl_->current_group_->scale_cy = *height_pt / impl_->current_group_->scale_cy;
		}
		else
		{
			if ((!impl_->current_drawing_state_.svg_width_ || reset_always) && width_pt)
				impl_->current_drawing_state_.svg_width_ = length(length(*width_pt, length::pt).get_value_unit(length::cm), length::cm);

			if ((!impl_->current_drawing_state_.svg_height_ || reset_always) && height_pt)
				impl_->current_drawing_state_.svg_height_ = length(length(*height_pt, length::pt).get_value_unit(length::cm), length::cm);
		}
	}
}
void odf_drawing_context::set_line_width(double pt)
{
	if (!impl_->current_graphic_properties) return;

	if (pt < 0.0001)
		impl_->current_graphic_properties->draw_stroke_ = line_style::None;

	impl_->current_graphic_properties->svg_stroke_width_ = length(length(pt,length::pt).get_value_unit(length::cm), length::cm);
}

void odf_drawing_context::set_line_tail(int type, int len, int width)
{
	if (!impl_->current_graphic_properties)return;

	impl_->current_graphic_properties->draw_marker_end_ = add_marker_style(type);

	if (impl_->current_drawing_state_.svg_width_ && impl_->current_drawing_state_.svg_height_)
	{
		double sz_x =impl_->current_drawing_state_.svg_width_->get_value_unit(odf_types::length::pt);
		double sz_y =impl_->current_drawing_state_.svg_height_->get_value_unit(odf_types::length::pt);

		impl_->current_graphic_properties->draw_marker_end_width_ = length((std::max)(sz_x, sz_y) / 10., odf_types::length::pt); 
	}
	if (!impl_->current_graphic_properties->draw_marker_end_width_)
	{
		switch(width)
		{
		case 0://lineendwidthLarge
			impl_->current_graphic_properties->draw_marker_end_width_ = length(0.4, length::cm); break;
		case 1://lineendwidthMedium
			impl_->current_graphic_properties->draw_marker_end_width_ = length(0.3, length::cm); break;
		case 2://lineendwidthSmall
			impl_->current_graphic_properties->draw_marker_end_width_ = length(0.2, length::cm); break;
		}
	}
}
void odf_drawing_context::set_line_head(int type, int len, int width)
{
	if (!impl_->current_graphic_properties)return;
	
	impl_->current_graphic_properties->draw_marker_start_ = add_marker_style(type);

	if (impl_->current_drawing_state_.svg_width_ && impl_->current_drawing_state_.svg_height_)
	{
		double sz_x =impl_->current_drawing_state_.svg_width_->get_value_unit(odf_types::length::pt);
		double sz_y =impl_->current_drawing_state_.svg_height_->get_value_unit(odf_types::length::pt);

		impl_->current_graphic_properties->draw_marker_start_width_ = length((std::max)(sz_x, sz_y) / 10., odf_types::length::pt); 
	}
	if (!impl_->current_graphic_properties->draw_marker_start_width_)
	{
		switch(width)
		{
			case 0://lineendwidthLarge
				impl_->current_graphic_properties->draw_marker_start_width_ = length(0.4, length::cm); break;
			case 1://lineendwidthMedium
				impl_->current_graphic_properties->draw_marker_start_width_ = length(0.3, length::cm); break;
			case 2://lineendwidthSmall
				impl_->current_graphic_properties->draw_marker_start_width_ = length(0.2, length::cm); break;
		}
	}
}

void odf_drawing_context::set_corner_radius	(odf_types::length corner)
{
	if (impl_->current_level_.empty())return;

	draw_rect* draw = dynamic_cast<draw_rect*>(impl_->current_level_.back().get());

	if (draw)
	{
		draw->draw_rect_attlist_.draw_corner_radius_ = corner;
	}

}

std::wstring odf_drawing_context::add_marker_style(int type)
{
	if (type == 0) return L"";

	std::wstring str_types [] = {L"None", L"ArrowMarker", L"DiamondMarker", L"OvalMarker", L"StealthMarker", L"TriangleMarker"};

	style * style_=NULL;
	if (impl_->styles_context_->find_odf_style(str_types[type],style_family::Marker,style_)) return str_types[type];
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
//	генерация типа маркера
	odf_writer::office_element_ptr marker_element;

	odf_writer::create_element(L"draw",L"marker", marker_element, impl_->odf_context_);
	impl_->styles_context_->add_style(marker_element, false, true, style_family::Marker);
	impl_->styles_context_->last_state()->set_name(str_types[type]);

	draw_marker * marker = dynamic_cast<draw_marker *>(marker_element.get());
	if (!marker) return L"";

	marker->draw_name_ = str_types[type];
	marker->draw_display_name_ = str_types[type]; ;

	switch(type)
	{
	case 2:
		marker->svg_d_ = L"M0 564l564 567 567-567-567-564z";
		marker->svg_viewBox_ = L"0 0 1131 1131";
		break;
	case 3:
		marker->svg_d_ = L"M462 1118l-102-29-102-51-93-72-72-93-51-102-29-102-13-105 13-102 29-106 51-102 72-89 93-72 102-50 102-34 106-9 101 9 106 34 98 50 93 72 72 89 51 102 29 106 13 102-13 105-29 102-51 102-72 93-93 72-98 51-106 29-101 13z";
		marker->svg_viewBox_ = L"0 0 1131 1131";
		break;
	case 4:
		marker->svg_d_ = L"M1013 1491l118 89-567-1580-564 1580 114-85 136-68 148-46 161-17 161 13 153 46z";
		marker->svg_viewBox_ = L"0 0 1131 1580";
		break;
	case 5:
		marker->svg_d_ = L"M1321 3493h-1321l702-3493z";
		marker->svg_viewBox_ = L"0 0 1321 3493";
		break;
	case 1:
	default:
		marker->svg_d_ =L"M0 2108v17 17l12 42 30 34 38 21 43 4 29-8 30-21 25-26 13-34 343-1532 339 1520 13 42 29 34 39 21 42 4 42-12 34-30 21-42v-39-12l-4 4-440-1998-9-42-25-39-38-25-43-8-42 8-38 25-26 39-8 42z";
		marker->svg_viewBox_ = L"0 0 1122 2243";
	}

	return str_types[type];
}
void odf_drawing_context::set_line_dash_preset(int style)
{
	if (!impl_->current_graphic_properties)return;

	if ((impl_->current_graphic_properties->draw_stroke_) && 
		(impl_->current_graphic_properties->draw_stroke_->get_type() == line_style::None) )return;

	switch(style)	//+создать стиль, привзать имена
	{
		case 0://presetlinedashvalDash    
		case 7://presetlinedashvalSysDash     
			impl_->current_graphic_properties->draw_stroke_=line_style(line_style::Dash);		break;
		case 1://presetlinedashvalDashDot    
		case 8://presetlinedashvalSysDashDot  
			impl_->current_graphic_properties->draw_stroke_=line_style(line_style::DotDash);	break;
		case 2://presetlinedashvalDot       
		case 10://presetlinedashvalSysDot        
			impl_->current_graphic_properties->draw_stroke_=line_style(line_style::Dotted);		break;
		case 3://presetlinedashvalLgDash  
		case 4://presetlinedashvalLgDashDot  
			impl_->current_graphic_properties->draw_stroke_=line_style(line_style::LongDash);	break;
		case 5://presetlinedashvalLgDashDotDot 
		case 9://presetlinedashvalSysDashDotDot 
			impl_->current_graphic_properties->draw_stroke_=line_style(line_style::DotDotDash);	break;
		case 6://presetlinedashvalSolid      
			impl_->current_graphic_properties->draw_stroke_=line_style(line_style::Solid);		break;
	}
}
void odf_drawing_context::set_text_properties(style_text_properties *text_properties)
{
	if (impl_->current_drawing_state_.elements_.empty()) return;

	if (!impl_->current_text_properties)
	{
		draw_base* draw = dynamic_cast<draw_base*>(impl_->current_drawing_state_.elements_[0].elm.get());
		if (draw)
		{
			if(!draw->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_text_style_name_)
			{
				impl_->styles_context_->create_style(L"", style_family::Paragraph, true, false, -1);		
			
				office_element_ptr & style_shape_elm = impl_->styles_context_->last_state()->get_office_element();
				style* style_ = dynamic_cast<style*>(style_shape_elm.get());
				if (style_)
				{
					impl_->current_text_properties = style_->content_.get_style_text_properties();
					draw->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_text_style_name_ = style_->style_name_;
				}
			}
			else
			{
				//??? find by name
			}
		}
	}
	if (impl_->current_text_properties)
		impl_->current_text_properties ->apply_from(text_properties);
}
void odf_drawing_context::set_paragraph_properties(style_paragraph_properties *paragraph_properties)
{
	if (impl_->current_drawing_state_.elements_.empty()) return;

	if (!impl_->current_paragraph_properties)
	{
		draw_base* draw = dynamic_cast<draw_base*>(impl_->current_drawing_state_.elements_[0].elm.get());
		if (draw)
		{
			if(!draw->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_text_style_name_)
			{
				impl_->styles_context_->create_style(L"", style_family::Paragraph, true, false, -1);		
			
				office_element_ptr & style_shape_elm = impl_->styles_context_->last_state()->get_office_element();
				style* style_ = dynamic_cast<style*>(style_shape_elm.get());
				if (style_)
				{
					impl_->current_paragraph_properties = style_->content_.get_style_paragraph_properties();
					draw->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_text_style_name_ = style_->style_name_;
				}
			}
			else
			{
				//??? find by name
			}
		}
	}
	if (impl_->current_paragraph_properties)
		impl_->current_paragraph_properties ->apply_from(paragraph_properties);
}
void odf_drawing_context::set_graphic_properties(style_graphic_properties *graphic_properties)
{
	if (impl_->current_graphic_properties && graphic_properties)
		impl_->current_graphic_properties->apply_from(graphic_properties->content_);
}
graphic_format_properties* odf_drawing_context::get_graphic_properties()
{
	return impl_->current_graphic_properties;
}

void odf_drawing_context::set_textarea_vertical_align(int align)
{
	if (!impl_->current_graphic_properties)return;
	switch(align)
	{
	case 0://SimpleTypes::textanchoringtypeB: 
			impl_->current_graphic_properties->draw_textarea_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Bottom);	break;
	case 1://SimpleTypes::textanchoringtypeCtr: 
			impl_->current_graphic_properties->draw_textarea_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Middle);	break;
	case 2://SimpleTypes::textanchoringtypeDist: 
			impl_->current_graphic_properties->draw_textarea_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Baseline);break;
	case 3://SimpleTypes::textanchoringtypeJust: 
			impl_->current_graphic_properties->draw_textarea_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Justify);	break;
	case 4://SimpleTypes::textanchoringtypeT: 
			impl_->current_graphic_properties->draw_textarea_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Top);		break;
	}
}
void odf_drawing_context::set_textarea_wrap(bool Val)
{
	if (!impl_->current_graphic_properties)return;

	if (Val)
		impl_->current_graphic_properties->fo_wrap_option_ = wrap_option(wrap_option::Wrap);
	else
		impl_->current_graphic_properties->fo_wrap_option_ = wrap_option(wrap_option::NoWrap);

}
void odf_drawing_context::set_textarea_fit_to_size(bool val)
{
	if (!impl_->current_graphic_properties)return;

	impl_->current_graphic_properties->draw_fit_to_size_ = val;
}

void odf_drawing_context::set_textarea_rotation(double val)
{
	odf_style_state_ptr style_state = impl_->styles_context_->last_state(style_family::Paragraph);
	if (style_state)
	{
		impl_->current_text_properties = style_state->get_text_properties();
	}

	if (!impl_->current_text_properties) return;

	impl_->current_text_properties->content_.style_text_rotation_angle_ = (int)val;
}

void odf_drawing_context::set_textarea_font(std::wstring & latin, std::wstring & cs, std::wstring & ea)
{
	if (impl_->current_drawing_state_.elements_.empty())return;

	odf_style_state_ptr style_state = impl_->styles_context_->last_state(style_family::Paragraph);
	if (style_state)
	{
		impl_->current_text_properties = style_state->get_text_properties();
	}

	if (!impl_->current_text_properties) return;

	if (!ea.empty())	impl_->current_text_properties->content_.fo_font_family_			= latin;
	if (!cs.empty())	impl_->current_text_properties->content_.style_font_family_complex_	= cs;
	if (!latin.empty())	impl_->current_text_properties->content_.style_font_family_asian_	= ea;

}
void odf_drawing_context::set_textarea_fontcolor(std::wstring hexColor)
{
	if (hexColor.empty()) return;
	if (impl_->current_drawing_state_.elements_.empty())return;

	odf_style_state_ptr style_state = impl_->styles_context_->last_state(style_family::Paragraph);
	if (style_state)
	{
		impl_->current_text_properties = style_state->get_text_properties();
	}

	if (!impl_->current_text_properties) return;

	if (std::wstring::npos == hexColor.find(L"#")) 
		hexColor = std::wstring(L"#") + hexColor;

	impl_->current_text_properties->content_.fo_color_ = hexColor;
}
void odf_drawing_context::set_textarea_writing_mode(int mode)
{
	if (mode == 1) return;//незачем
	if (impl_->current_drawing_state_.elements_.empty())return;

	if (impl_->current_drawing_state_.oox_shape_preset_ > 2000 && impl_->current_drawing_state_.oox_shape_preset_ < 3000)
	{
		switch(mode)
		{
			case 5://textverticaltypeWordArtVert:
			case 6://textverticaltypeWordArtVertRtl:
			case 3://SimpleTypes::textverticaltypeVert: 
			case 2://SimpleTypes::textverticaltypeMongolianVert:
			case 0://SimpleTypes::textverticaltypeEaVert: 
				impl_->current_drawing_state_.rotateAngle_ = 90. / 180. * 3.14159265358979323846;
				break;			
			case 4://SimpleTypes::textverticaltypeVert270:
				impl_->current_drawing_state_.rotateAngle_ = 270. / 180. * 3.14159265358979323846;
				break;
			case 1://SimpleTypes::textverticaltypeHorz: 
			default:
				break;
		}

		if (impl_->current_drawing_state_.flipH_ && impl_->current_drawing_state_.rotateAngle_ )
		{
			impl_->current_drawing_state_.rotateAngle_  = - *impl_->current_drawing_state_.rotateAngle_;
			impl_->current_drawing_state_.flipH_ = false;
		}
		return;	
	}

	if (!impl_->current_paragraph_properties)
	{
		style* style_ = dynamic_cast<style*>(impl_->current_drawing_state_.elements_[0].style_elm.get());
		if (style_)impl_->current_paragraph_properties = style_->content_.get_style_paragraph_properties();
	}
	
	style_paragraph_properties	* paragraph_properties = impl_->odf_context_->text_context()->get_paragraph_properties();
	draw_base* draw = dynamic_cast<draw_base*>(impl_->current_drawing_state_.elements_[0].elm.get());
	if (draw)
	{
		style* style_ = NULL;
		if(!draw->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_text_style_name_)
		{
			impl_->styles_context_->create_style(L"", style_family::Paragraph, true, false, -1);		
		
			office_element_ptr & style_shape_elm = impl_->styles_context_->last_state()->get_office_element();
			style_ = dynamic_cast<style*>(style_shape_elm.get());
			if (style_)
				draw->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_text_style_name_ = style_->style_name_;
		}
		else
		{
			std::wstring style_name = *draw->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_text_style_name_;
			//найти
		}
		if (style_ && !paragraph_properties)
		{
			paragraph_properties = style_->content_.get_style_paragraph_properties();
		}
	}
	
	if (paragraph_properties)
	{
		switch(mode)
		{
			case 5://textverticaltypeWordArtVert:
			case 6://textverticaltypeWordArtVertRtl:
			case 4://SimpleTypes::textverticaltypeVert270: //нужно отзеркалить по горизонтали текст
			case 3://SimpleTypes::textverticaltypeVert: 
			case 2://SimpleTypes::textverticaltypeMongolianVert:
				
				paragraph_properties->content_.style_writing_mode_ = odf_types::writing_mode(odf_types::writing_mode::TbRl);	
				break;
			case 0://SimpleTypes::textverticaltypeEaVert: 
				paragraph_properties->content_.style_writing_mode_ = odf_types::writing_mode(odf_types::writing_mode::TbRl);	
				break;
			case 1://SimpleTypes::textverticaltypeHorz: 
			default:
				paragraph_properties->content_.style_writing_mode_ = odf_types::writing_mode(odf_types::writing_mode::LrTb);	
				break;
		}
	}
	if (impl_->current_paragraph_properties)
	{
		switch(mode)
		{
			case 5://textverticaltypeWordArtVert:
			case 6://textverticaltypeWordArtVertRtl:
			case 4://SimpleTypes::textverticaltypeVert270: //нужно отзеркалить по горизонтали текст
			case 3://SimpleTypes::textverticaltypeVert: 
			case 2://SimpleTypes::textverticaltypeMongolianVert:
				impl_->current_paragraph_properties->content_.style_writing_mode_ = odf_types::writing_mode(odf_types::writing_mode::TbRl);	
				break;
			case 0://SimpleTypes::textverticaltypeEaVert: 
				impl_->current_paragraph_properties->content_.style_writing_mode_ = odf_types::writing_mode(odf_types::writing_mode::TbRl);	
				break;
			case 1://SimpleTypes::textverticaltypeHorz: 
			default:
				impl_->current_paragraph_properties->content_.style_writing_mode_ = odf_types::writing_mode(odf_types::writing_mode::LrTb);	
				break;
		}
	}
}
void odf_drawing_context::set_textarea_padding(_CP_OPT(double) & left, _CP_OPT(double) & top, _CP_OPT(double) & right, _CP_OPT(double) & bottom)//in pt
{
	if (!impl_->current_graphic_properties)return;

	if (left)	impl_->current_graphic_properties->common_padding_attlist_.fo_padding_left_		= length(*left,	length::pt);
	if (top)	impl_->current_graphic_properties->common_padding_attlist_.fo_padding_top_		= length(*top,	length::pt);
	if (right)	impl_->current_graphic_properties->common_padding_attlist_.fo_padding_right_	= length(*right,length::pt);
	if (bottom)	impl_->current_graphic_properties->common_padding_attlist_.fo_padding_bottom_	= length(*bottom,length::pt);
}
//------------------------------------------------------------------------------------------------------------------
void odf_drawing_context::start_image(std::wstring odf_path)
{	
	if (/*impl_->is_footer_ || impl_->is_header_ ||*/ impl_->is_background_)//AstraIntlCaseStudyFinal0.docx
	{
		start_shape(142/*SimpleTypes::shapetypeRect*/);
		start_bitmap_style();
		
		draw_fill_image * fill_image = dynamic_cast<draw_fill_image *>(impl_->styles_context_->last_state()->get_office_element().get());
		if (fill_image)
		{
			fill_image->xlink_attlist_.href_= odf_path;
		}
		set_opacity(50);
		return;
	}

	impl_->current_drawing_state_.oox_shape_preset_ = 3000;
	
	start_frame();

	//добавить в стиль ссыль на базовый стиль Graphics - зачемто нужно :(
	set_parent_style(L"Graphics");

///////////////////////////////////////////////////////////////////////////////////////////////
	office_element_ptr image_elm;
	create_element(L"draw", L"image", image_elm, impl_->odf_context_);

	draw_image* image = dynamic_cast<draw_image*>(image_elm.get());
	if (image == NULL)return;

	image->common_xlink_attlist_.type_		= xlink_type::Simple;
	image->common_xlink_attlist_.show_		= xlink_show::Embed;
	image->common_xlink_attlist_.actuate_	= xlink_actuate::OnLoad;

	if (!odf_path.empty())   image->common_xlink_attlist_.href_= odf_path; //may be later set
	
	start_element(image_elm);
			
	set_image_style_repeat(1);//default
}
void odf_drawing_context::start_object(std::wstring name, bool in_frame)
{
	if (in_frame)
		start_frame();
	else
	{
		//remove text_box - он лишний (оставляя фейковый, который не запишется)
		impl_->current_level_.back() = office_element_ptr(); // чтоб внутрении элементы добавлялись к тому что выше
		
		if (impl_->current_level_.size() > 1)
		{
			draw_base* draw = dynamic_cast<draw_base*>(impl_->current_level_[impl_->current_level_.size() - 2].get());
			if (draw)
			{
				if (false == draw->content_.empty())
					draw->content_.pop_back();
			}

		}
	}
	
	office_element_ptr object_elm;
	create_element(L"draw", L"object", object_elm, impl_->odf_context_);

	draw_object* object = dynamic_cast<draw_object*>(object_elm.get());
	if (object == NULL)return;

    object->common_xlink_attlist_.href_		= std::wstring(L"./") + name;
	object->common_xlink_attlist_.type_		= xlink_type::Simple;
	object->common_xlink_attlist_.show_		= xlink_show::Embed;
	object->common_xlink_attlist_.actuate_	= xlink_actuate::OnLoad;

	start_element(object_elm);
}
void odf_drawing_context::start_control(const std::wstring& id)
{
	office_element_ptr control_elm;
	create_element(L"draw", L"control", control_elm, impl_->odf_context_);

	draw_control* control = dynamic_cast<draw_control*>(control_elm.get());
	if (control == NULL)return;

	if (impl_->root_element_ == NULL) impl_->root_element_ = control_elm;
	
	control->control_id_ = id;
//--------------------	
	impl_->styles_context_->create_style(L"", style_family::Graphic, true, false, -1);		
	office_element_ptr & style_control_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_control_elm.get());
	if (style_)
	{
        style_name = style_->style_name_;
		impl_->current_graphic_properties = style_->content_.get_graphic_properties();
	}
	control->common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_style_name_ = style_name;
//--------------------	
	start_element(control_elm);
}
void odf_drawing_context::end_control()
{
	end_shape();
}
void odf_drawing_context::start_media(std::wstring name)
{
	start_frame();

	office_element_ptr plugin_elm;
	create_element(L"draw", L"plugin", plugin_elm, impl_->odf_context_);

	draw_plugin* plugin = dynamic_cast<draw_plugin*>(plugin_elm.get());
	if (plugin == NULL)return;

    plugin->common_xlink_attlist_.href_		= name;
	plugin->common_xlink_attlist_.type_		= xlink_type::Simple;
	plugin->common_xlink_attlist_.show_		= xlink_show::Embed;
	plugin->common_xlink_attlist_.actuate_	= xlink_actuate::OnLoad;

	plugin->draw_mime_type_ = L"application/vnd.sun.star.media";
	
	start_element(plugin_elm);
}
void odf_drawing_context::add_image_replacement()
{
	if (impl_->current_drawing_state_.replacement_.empty()) return;

	office_element_ptr image_elm;
	create_element(L"draw", L"image", image_elm, impl_->odf_context_);

	draw_image* image = dynamic_cast<draw_image*>(image_elm.get());
	if (image == NULL)return;

	image->common_xlink_attlist_.href_		= impl_->current_drawing_state_.replacement_;
	image->common_xlink_attlist_.type_		= xlink_type::Simple;
	image->common_xlink_attlist_.show_		= xlink_show::Embed;
	image->common_xlink_attlist_.actuate_	= xlink_actuate::OnLoad;
	
	start_element(image_elm);			
		set_image_style_repeat(1);//default
	end_element();
}
void odf_drawing_context::end_media()
{
	end_element();
	
	add_image_replacement();

	end_frame();
}

void odf_drawing_context::start_text_box()
{	
	impl_->current_drawing_state_.oox_shape_preset_ = 2000;

	start_frame();

	//if (impl_->is_footer_ ==false && impl_->is_header_ ==false)
	//	set_parent_style(L"Frame");	
	
	office_element_ptr text_box_elm;
	create_element(L"draw", L"text-box", text_box_elm, impl_->odf_context_);

	start_element(text_box_elm);
}
void odf_drawing_context::set_text_box_min_size(bool val)
{
	if (impl_->current_graphic_properties)
	{
		impl_->current_graphic_properties->draw_auto_grow_height_ = true;
		//impl_->current_graphic_properties->draw_auto_grow_width_ = true; //Example_2.xlsx
		
		impl_->current_graphic_properties->draw_fit_to_size_ = false;
		impl_->current_graphic_properties->style_shrink_to_fit_ = false;
	}	
	
	if (impl_->current_drawing_state_.elements_.empty()) return;

	draw_text_box* draw = dynamic_cast<draw_text_box*>(impl_->current_drawing_state_.elements_.back().elm.get());
	
	if (draw)
	{
		if (!draw->attlist_.fo_min_height_)
			draw->attlist_.fo_min_height_ = impl_->anchor_settings_.svg_height_;
		if (!draw->attlist_.fo_min_width_)
			draw->attlist_.fo_min_width_ = impl_->anchor_settings_.svg_width_;
	}
}

void odf_drawing_context::start_action(std::wstring value)
{
	office_element_ptr elm_listeners;
	create_element(L"office", L"event-listeners", elm_listeners, impl_->odf_context_);

	start_element(elm_listeners);
	
	office_element_ptr elm;
	create_element(L"presentation", L"event-listener", elm, impl_->odf_context_);

	start_element(elm);
	
	presentation_event_listener * event_ = dynamic_cast<presentation_event_listener*>(impl_->current_level_.back().get());

	if (event_)
	{
		event_->attlist_.script_event_name_ = L"dom:click";
		
		if (std::wstring::npos != value.find(L"noaction") ||
			std::wstring::npos != value.find(L"media"))
		{
			event_->attlist_.script_event_name_ = boost::none;
		}
		else if (std::wstring::npos != value.find(L"program"))
		{
			event_->attlist_.presentation_action_ = L"execute";
		}
		else if (std::wstring::npos != value.find(L"hlinkshowjump"))
		{
			if (std::wstring::npos != value.find(L"previousslide"))
				event_->attlist_.presentation_action_ = L"previous-page";
			else if (std::wstring::npos != value.find(L"nextslide"))
				event_->attlist_.presentation_action_ = L"next-page";
			else if (std::wstring::npos != value.find(L"firstslide"))
				event_->attlist_.presentation_action_ = L"first-page";
			else if (std::wstring::npos != value.find(L"lastslide"))
				event_->attlist_.presentation_action_ = L"last-page";
			else if (std::wstring::npos != value.find(L"endshow"))
				event_->attlist_.presentation_action_ = L"end";
		}
		else if (std::wstring::npos != value.find(L"hlinksldjump"))
		{
			event_->attlist_.presentation_action_ = L"previous-page";
		}
		else if (std::wstring::npos != value.find(L"macro"))
		{
		}
		else if (std::wstring::npos != value.find(L"hlinkfile"))
		{
		}
		else if (std::wstring::npos != value.find(L"hlinkpres"))
		{
		}
		else
		{//hyperlink
			event_->attlist_.presentation_action_	= L"show";
		}
	}
}
void odf_drawing_context::start_link_object(std::wstring href)
{
	if (href.empty()) return;
	
	office_element_ptr element, style;
	create_element(L"draw", L"a", element, impl_->odf_context_);

	size_t level = impl_->current_level_.size();
	
	if (false == impl_->current_level_.empty())
		impl_->current_level_.back()->add_child_element(element);

	impl_->current_level_.push_back(element);

	odf_element_state state(element, L"", style, level);
	impl_->current_drawing_state_.elements_.push_back(state);

	if (impl_->root_element_ == NULL)
	{
		impl_->root_element_ = element;
		impl_->anchor_settings_.anchor_type_ = anchor_type::Paragraph;
	}

	draw_a* draw_a_ = dynamic_cast<draw_a*>(impl_->current_level_.back().get());
	if (draw_a_)
	{
		draw_a_->xlink_attlist_.href_		= href;
		draw_a_->xlink_attlist_.type_		= xlink_type::Simple;
		draw_a_->xlink_attlist_.show_		= xlink_show::Embed;
		draw_a_->xlink_attlist_.actuate_	= xlink_actuate::OnRequest;
	}
}
void odf_drawing_context::end_link_object()
{
	end_element();
}
void odf_drawing_context::add_link(std::wstring href)
{
	if (href.empty()) return;

	presentation_event_listener* event_ = dynamic_cast<presentation_event_listener*>(impl_->current_level_.back().get());

	if (event_)
	{
		event_->attlist_.common_xlink_attlist_.href_	= href;
		event_->attlist_.common_xlink_attlist_.type_	= xlink_type::Simple;
		event_->attlist_.common_xlink_attlist_.show_	= xlink_show::Embed;
		event_->attlist_.common_xlink_attlist_.actuate_	= xlink_actuate::OnRequest;
	}
}

void odf_drawing_context::add_sound(std::wstring href)
{
	presentation_event_listener * event_ = dynamic_cast<presentation_event_listener*>(impl_->current_level_.back().get());
	if (event_)
	{
		event_->attlist_.script_event_name_	= L"dom:click";

		if (!event_->attlist_.presentation_action_)
			event_->attlist_.presentation_action_ = L"sound";
	}

	office_element_ptr elm;
	create_element(L"presentation", L"sound", elm, impl_->odf_context_);

	start_element(elm);

	presentation_sound *sound = dynamic_cast<presentation_sound*>(elm.get());
	if (sound)
	{
		sound->common_xlink_attlist_.href_		= href;
		sound->common_xlink_attlist_.type_		= xlink_type::Simple;
		sound->common_xlink_attlist_.show_		= xlink_show::Embed;
		sound->common_xlink_attlist_.actuate_	= xlink_actuate::OnRequest;
	}
	end_element();
}
void odf_drawing_context::end_action()
{
	end_element();
	end_element();
}
void odf_drawing_context::set_text_box_min_size(double w_pt, double h_pt)
{
	if (impl_->current_drawing_state_.elements_.empty()) return;

	draw_text_box* draw = dynamic_cast<draw_text_box*>(impl_->current_drawing_state_.elements_.back().elm.get());

	if (draw)
	{
		if (h_pt >0) draw->attlist_.fo_min_height_= length(length(h_pt,length::pt).get_value_unit(length::cm), length::cm);
		if (w_pt >0) draw->attlist_.fo_min_width_= length(length(w_pt,length::pt).get_value_unit(length::cm), length::cm);


	}
}
void odf_drawing_context::set_text_box_tableframe(bool val)
{
	if (impl_->current_drawing_state_.elements_.empty()) return;

	impl_->current_drawing_state_.text_box_tableframe_ = val;

}
void odf_drawing_context::set_parent_style(std::wstring style_name)
{
	if (impl_->current_drawing_state_.elements_.empty()) return;

	//добавить в стиль ссыль на базовый стиль Frame - зачемто нужно для таблиц которые не инлайн 
	style* style_ = dynamic_cast<style*>(impl_->current_drawing_state_.elements_[0].style_elm.get()); // на "головной" элекмент
	
	if (style_)
	{
		if (style_name.length() > 0)
			style_->style_parent_style_name_ = style_name;
		else
			style_->style_parent_style_name_ = boost::none;
	}
}

void odf_drawing_context::end_image()
{
	if (/*impl_->is_footer_ || impl_->is_header_ ||*/ impl_->is_background_)
	{
		end_bitmap_style();
		end_shape();
		return;
	}
	if (impl_->current_drawing_state_.flipH_)
	{
		if (impl_->current_graphic_properties->style_mirror_)
			impl_->current_graphic_properties->style_mirror_ = *impl_->current_graphic_properties->style_mirror_ + std::wstring(L" horizontal");
		else
			impl_->current_graphic_properties->style_mirror_ = std::wstring(L"horizontal");
	}	
	end_element();
	end_frame();
}
void odf_drawing_context::end_text_box()
{
	if (impl_->current_drawing_state_.elements_.empty()) return;

	draw_text_box* draw = dynamic_cast<draw_text_box*>(impl_->current_level_.back().get());

	if ((draw) && (!draw->attlist_.fo_min_height_))
	{
		draw->attlist_.fo_min_height_= impl_->current_drawing_state_.svg_height_;
	}
	//impl_->current_drawing_state_.svg_height_ = boost::none;///??? demo.docx
	
	end_element();

	if (impl_->current_graphic_properties->svg_stroke_width_ || 
		(impl_->current_graphic_properties->draw_stroke_ && impl_->current_graphic_properties->draw_stroke_->get_type() != line_style::None))
	{
		std::wstringstream ss;
		ss << impl_->current_graphic_properties->svg_stroke_width_->get_length();

		if (impl_->current_graphic_properties->draw_stroke_) 
			ss << L" " << *impl_->current_graphic_properties->draw_stroke_;
		else ss << L" solid";

		if (impl_->current_graphic_properties->svg_stroke_color_) 
			ss << L" " << *impl_->current_graphic_properties->svg_stroke_color_;
		else ss << L" #000000";

		impl_->current_graphic_properties->common_border_attlist_.fo_border_ = ss.str();
	}
	
	if (impl_->current_graphic_properties->common_shadow_attlist_.draw_shadow_)
	{
		std::wstringstream shadow_style;
		
		if (impl_->current_graphic_properties->common_shadow_attlist_.draw_shadow_color_)
			shadow_style << *impl_->current_graphic_properties->common_shadow_attlist_.draw_shadow_color_;
		else shadow_style << L"#000000";

		shadow_style << L" ";	
		shadow_style << *impl_->current_graphic_properties->common_shadow_attlist_.draw_shadow_offset_x_;
		shadow_style << L" ";
		shadow_style << *impl_->current_graphic_properties->common_shadow_attlist_.draw_shadow_offset_y_;

		impl_->current_graphic_properties->common_shadow_attlist_.style_shadow_ = shadow_style.str();

		impl_->current_graphic_properties->common_shadow_attlist_.draw_shadow_offset_x_	= boost::none;
		impl_->current_graphic_properties->common_shadow_attlist_.draw_shadow_offset_y_	= boost::none;
		impl_->current_graphic_properties->common_shadow_attlist_.draw_shadow_color_	= boost::none;
		impl_->current_graphic_properties->common_shadow_attlist_.draw_shadow_			= boost::none;

		if (!impl_->current_graphic_properties->common_border_attlist_.fo_border_)
			impl_->current_graphic_properties->common_border_attlist_.fo_border_ = std::wstring(L"#000000 solid 0.06pt");
	}
	
	if (!impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_)
		impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_ = draw_fill::none;

	if (!impl_->current_graphic_properties->draw_stroke_)
		impl_->current_graphic_properties->draw_stroke_ = line_style (line_style::None);

	end_frame();
}
void odf_drawing_context::end_object(bool in_frame)
{
	end_element();

	if (in_frame)
		end_frame();
}
void odf_drawing_context::start_object_ole(std::wstring ref)
{
	start_frame();
	
	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_ = draw_fill::none;
	impl_->current_graphic_properties->draw_stroke_ = line_style(line_style::None);
	impl_->current_graphic_properties->draw_ole_draw_aspect_ = 1;
//------------------------------------------------------------------------------------------------------	
	office_element_ptr object_elm;
	create_element(L"draw", L"object-ole", object_elm, impl_->odf_context_);

	draw_object_ole* object = dynamic_cast<draw_object_ole*>(object_elm.get());
	if (object == NULL)return;

    object->common_xlink_attlist_.href_		= ref;
	object->common_xlink_attlist_.type_		= xlink_type::Simple;
	object->common_xlink_attlist_.show_		= xlink_show::Embed;
	object->common_xlink_attlist_.actuate_	= xlink_actuate::OnLoad;

	start_element(object_elm);
}
void odf_drawing_context::end_object_ole()
{
	end_element();

	add_image_replacement();

	end_frame();
}

void odf_drawing_context::set_image_replacement(std::wstring ref)
{
	impl_->current_drawing_state_.replacement_ = ref;
}

void odf_drawing_context::set_program(std::wstring program)
{
	impl_->current_drawing_state_.program_ = program;
}

bool odf_drawing_context::is_exist_content()
{
	return (impl_->drawing_list_.empty() ? false : true);
}
bool odf_drawing_context::is_current_empty()
{
	return impl_->current_drawing_state_.elements_.empty();
}
void odf_drawing_context::finalize(office_element_ptr & root_elm)//для привязки 
{
	for (size_t i = 0; i < impl_->tops_elements_.size(); i++)
	{
		root_elm->add_child_element(impl_->tops_elements_[i]);
	}	
}
office_element_ptr & odf_drawing_context::get_current_style_element()
{
	if (impl_->current_drawing_state_.elements_.size() > 0)
		return impl_->current_drawing_state_.elements_.back().style_elm;
	else
		throw;
}
void odf_drawing_context::set_text(odf_text_context* text_context)
{
	if (text_context == NULL || impl_->current_level_.empty() ) return;
	
	if (!impl_->current_level_.back()) return; // фейковый текстбокс к примеру

	//if (impl_->is_presentation_ && *impl_->is_presentation_) return; 

	for (size_t i = 0; i < text_context->text_elements_list_.size(); i++)
	{
		if (text_context->text_elements_list_[i].level == 0)
		{
			impl_->current_level_.back()->add_child_element(text_context->text_elements_list_[i].elm);
		}
	}

	if ((impl_->current_graphic_properties) && 
		!impl_->current_graphic_properties->draw_auto_grow_height_)
	{
		//автоувеличение при добавлении текста
		impl_->current_graphic_properties->draw_auto_grow_height_ = false;
		impl_->current_graphic_properties->draw_auto_grow_width_ = false;
		//impl_->current_graphic_properties->draw_fit_to_size_ = false;//???

	}

	if (impl_->current_drawing_state_.oox_shape_preset_ > 2000 && impl_->current_drawing_state_.oox_shape_preset_ < 3000)
	{
		//настройки цвета - перетащить в линии и заливки - так уж нужно wordart-у оо
		style_text_properties *text_properties_ = text_context->get_text_properties();
		
		if (text_properties_)
		{
			color color_ = text_properties_->content_.fo_color_.get_value_or(color(L"#000000"));
			impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_color_ = color_;			
			
			if (text_properties_->content_.style_text_outline_)
			{
				//line
				impl_->current_graphic_properties->svg_stroke_color_	= color_;
				if (!impl_->current_graphic_properties->draw_stroke_)
					impl_->current_graphic_properties->draw_stroke_		= line_style(line_style::Solid);
				if (!impl_->current_graphic_properties->svg_stroke_width_)
					impl_->current_graphic_properties->svg_stroke_width_	= length(1,length::pt);
				
				impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_ = draw_fill(draw_fill::none);
			}
			else
			{
				impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_ = draw_fill(draw_fill::solid);			
			}
		}
		//text_context->text_elements_list_[0].style_elm.get()
		//start_line_properties();
		//	
		//end_line_properties();
		//
		//start_area_properties();

		//end_area_properties();
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------
void odf_drawing_context::start_gradient_style()
{
	if (!impl_->current_graphic_properties) return;

	odf_writer::office_element_ptr gradient_element;

	odf_writer::create_element(L"draw",L"gradient", gradient_element, impl_->odf_context_);

	draw_gradient * gradient = dynamic_cast<draw_gradient *>(gradient_element.get());
	if (!gradient) return;

	gradient->draw_name_ = impl_->styles_context_->find_free_name(style_family::Gradient);
	gradient->draw_display_name_ = std::wstring(L"User") + gradient->draw_name_.get() ;

	gradient->draw_start_color_ = impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_color_;
	if (gradient->draw_start_color_) gradient->draw_start_intensity_ = 100.;
	
	gradient->draw_border_ = 0;

	switch(impl_->current_drawing_part_)
	{
		case Area:
			impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_gradient_name_ = gradient->draw_name_;
			impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_ = draw_fill(draw_fill::gradient);
			break;
		case Line:
			if (!impl_->current_graphic_properties->draw_stroke_)
				impl_->current_graphic_properties->draw_stroke_ = line_style(line_style::Solid);//default
			if (!impl_->current_graphic_properties->svg_stroke_width_)
				impl_->current_graphic_properties->svg_stroke_width_ = length(length(1, length::pt).get_value_unit(length::cm), length::cm);//default
			break;
	}
	impl_->styles_context_->add_style(gradient_element, false, true, style_family::Gradient);
}
void odf_drawing_context::set_gradient_type(gradient_style::type style)
{
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state(style_family::Gradient)->get_office_element().get());
	if (!gradient) return;

	gradient->draw_style_ = style;
}
void odf_drawing_context::set_gradient_start(std::wstring hexColor, _CP_OPT(double) & intensiv)
{
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state(style_family::Gradient)->get_office_element().get());
	if (!gradient) return;

	if (std::wstring::npos == hexColor.find(L"#"))
		hexColor = std::wstring(L"#") + hexColor;
	
	gradient->draw_start_color_		= hexColor;
	gradient->draw_start_intensity_ = 100.;

	if (impl_->current_drawing_part_ == Line)
	{
		impl_->current_graphic_properties->svg_stroke_color_ =  hexColor;
	}
}
void odf_drawing_context::set_gradient_end  (std::wstring hexColor, _CP_OPT(double) & intensiv)
{
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state(style_family::Gradient)->get_office_element().get());
	if (!gradient) return;

	if (std::wstring::npos == hexColor.find(L"#"))
		hexColor = std::wstring(L"#") + hexColor;

	gradient->draw_end_color_		= hexColor;
	gradient->draw_end_intensity_	= 100.;
}
void odf_drawing_context::set_gradient_angle(double angle)
{
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state(style_family::Gradient)->get_office_element().get());
	if (!gradient) return;

	gradient->draw_angle_ = 270 - angle;//(360 - angle)/180. * 3.14159265358979323846;
}
void odf_drawing_context::set_gradient_rect(double l, double t, double r, double b)
{
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state(style_family::Gradient)->get_office_element().get());
	if (!gradient) return;
	
	gradient->draw_cy_ = percent((b - t)/2. + 50.);
	gradient->draw_cx_ = percent((r - l)/2. + 50.);
}
void odf_drawing_context::set_gradient_center(double cx, double cy)
{
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state(style_family::Gradient)->get_office_element().get());
	if (!gradient) return;
	
	gradient->draw_cy_ = percent(cx * 100.);
	gradient->draw_cx_ = percent(cy * 100.);
}
void odf_drawing_context::end_gradient_style()
{
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state(style_family::Gradient)->get_office_element().get());
	if (!gradient) return;

	if (!gradient->draw_start_color_) 
		gradient->draw_start_color_ = std::wstring(L"#ffffff");
	if (!gradient->draw_start_color_) 
		gradient->draw_start_color_ = std::wstring(L"#000000");
}
//-------------------------------------------------------------------------------------------------------------------------------------
void odf_drawing_context::start_opacity_style()
{
	if (!impl_->current_graphic_properties) return;

	odf_writer::office_element_ptr opacity_element;
	odf_writer::create_element(L"draw",L"opacity", opacity_element, impl_->odf_context_);

	draw_opacity * opacity = dynamic_cast<draw_opacity *>(opacity_element.get());
	if (!opacity) return;

	opacity->draw_name_ = impl_->styles_context_->find_free_name(style_family::Opacity);
	opacity->draw_display_name_ = std::wstring(L"User") + opacity->draw_name_.get() ;
	
	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_opacity_name_ = opacity->draw_name_;
	
	impl_->styles_context_->add_style(opacity_element, false, true, style_family::Opacity);
}
void odf_drawing_context::set_opacity_start(double val)
{
	draw_opacity * opacity = dynamic_cast<draw_opacity *>(impl_->styles_context_->last_state(style_family::Opacity)->get_office_element().get());
	if (!opacity) return;

	opacity->draw_start_ = val;
}
void odf_drawing_context::set_opacity_angle(double angle)
{
	draw_opacity * opacity = dynamic_cast<draw_opacity *>(impl_->styles_context_->last_state(style_family::Opacity)->get_office_element().get());
	if (!opacity) return;

	opacity->draw_angle_ = 270 - angle;//(360 - angle)/180. * 3.14159265358979323846;
}
void odf_drawing_context::set_opacity_type(gradient_style style)
{
	draw_opacity * opacity = dynamic_cast<draw_opacity *>(impl_->styles_context_->last_state(style_family::Opacity)->get_office_element().get());
	if (!opacity) return;

	opacity->draw_style_ = style;
}
void odf_drawing_context::set_opacity_rect(double l, double t, double r,double b)
{
	draw_opacity * opacity = dynamic_cast<draw_opacity *>(impl_->styles_context_->last_state(style_family::Opacity)->get_office_element().get());
	if (!opacity) return;
	
	opacity->draw_cy_ = percent((b-t)/2. + 50.);
	opacity->draw_cx_ = percent((r-l)/2. + 50.);
}
void odf_drawing_context::set_opacity_end (double val)
{
	draw_opacity * opacity = dynamic_cast<draw_opacity *>(impl_->styles_context_->last_state(style_family::Opacity)->get_office_element().get());
	if (!opacity) return;

	opacity->draw_end_ = val;
}
void odf_drawing_context::end_opacity_style()
{
}
//-------------------------------------------------------------------------------------------------------------------------------------
void odf_drawing_context::start_hatch_style()
{
	if (!impl_->current_graphic_properties) return;

	odf_writer::office_element_ptr hatch_element;

	odf_writer::create_element(L"draw",L"hatch", hatch_element, impl_->odf_context_);

	draw_hatch * hatch = dynamic_cast<draw_hatch *>(hatch_element.get());
	if (!hatch) return;

	hatch->draw_name_ = impl_->styles_context_->find_free_name(style_family::Hatch);
	hatch->draw_display_name_ = std::wstring(L"User") + hatch->draw_name_.get() ;
	
	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_hatch_name_ = hatch->draw_name_;
	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_ = draw_fill(draw_fill::hatch);
	
	impl_->styles_context_->add_style(hatch_element, false, true, style_family::Hatch);
}
void odf_drawing_context::set_hatch_line_color(std::wstring hexColor)
{
	if (!impl_->current_graphic_properties)return;

	//impl_->current_graphic_properties->svg_stroke_color_ = color(std::wstring(L"#") + hexColor);
	//impl_->current_graphic_properties->draw_stroke_=line_style(line_style::Solid);//default

	draw_hatch * hatch = dynamic_cast<draw_hatch *>(impl_->styles_context_->last_state()->get_office_element().get());
	if (!hatch) return;

	if (std::wstring::npos == hexColor.find(L"#"))
		hexColor = std::wstring(L"#") + hexColor;
	
	hatch->draw_color_ =  hexColor;
}
void odf_drawing_context::set_hatch_area_color(std::wstring hexColor)
{
	if (!impl_->current_graphic_properties)return;

	if (std::wstring::npos == hexColor.find(L"#"))
		hexColor = std::wstring(L"#") + hexColor;

	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_color_ = hexColor;
	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_hatch_solid_ = true;
}
void odf_drawing_context::set_hatch_type(int type)
{
	draw_hatch * hatch = dynamic_cast<draw_hatch *>(impl_->styles_context_->last_state()->get_office_element().get());
	if (!hatch) return;
	
	switch(type)
	{
	case 0: //presetpatternvalCross = 0, // (Cross)
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.25, length::cm);
		break;
	case 1: //	presetpatternvalDashDnDiag, // (Dashed Downward Diagonal) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.15, length::cm);
		break;
	case 2: //	presetpatternvalDashHorz, // (Dashed Horizontal) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.15, length::cm);
		break;
	case 3: //	presetpatternvalDashUpDiag, // (Dashed Upward DIagonal) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.15, length::cm);
		break;
	case 4: //	presetpatternvalDashVert, // (Dashed Vertical) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 900;
		hatch->draw_distance_ = length(0.15, length::cm);
		break;
	case 5: //	presetpatternvalDiagBrick, // (Diagonal Brick) 
		hatch->draw_style_ = hatch_style(hatch_style::doublee);
		hatch->draw_rotation_ = 450;
		hatch->draw_distance_ = length(0.175, length::cm);
		break;
	case 6: //	presetpatternvalDiagCross, // (Diagonal Cross) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 450;
		hatch->draw_distance_ = length(0.175, length::cm);
		break;
	case 7: //	presetpatternvalDivot, // (Divot) 
		hatch->draw_style_ = hatch_style(hatch_style::doublee);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.05, length::cm);
		break;
	case 8: //	presetpatternvalDkDnDiag, // (Dark Downward Diagonal) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 1350;
		hatch->draw_distance_ = length(0.05, length::cm);
		break;
	case 9: //	presetpatternvalDkHorz, // (Dark Horizontal) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.25, length::cm);
		break;
	case 10: //	presetpatternvalDkUpDiag, // (Dark Upward Diagonal) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 450;
		hatch->draw_distance_ = length(0.05, length::cm);
		break;
	case 11: //	presetpatternvalDkVert, // (Dark Vertical) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 900;
		hatch->draw_distance_ = length(0.25, length::cm);
		break;
	case 12: //	presetpatternvalDnDiag, // (Downward Diagonal) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 1350;
		hatch->draw_distance_ = length(0.2, length::cm);
		break;
	case 13: //	presetpatternvalDotDmnd, // (Dotted Diamond) 
		hatch->draw_style_ = hatch_style(hatch_style::doublee);
		hatch->draw_rotation_ = 450;
		hatch->draw_distance_ = length(0.15, length::cm);
		break;
	case 14: //	presetpatternvalDotGrid, // (Dotted Grid) 
		hatch->draw_style_ = hatch_style(hatch_style::doublee);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.1, length::cm);
		break;
	case 15: //	presetpatternvalHorz, // (Horizontal) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.15, length::cm);
		break;
	case 16: //	presetpatternvalHorzBrick, // (Horizontal Brick) 
		hatch->draw_style_ = hatch_style(hatch_style::doublee);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.175, length::cm);
		break;
	case 17: //	presetpatternvalLgCheck, // (Large Checker Board) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.25, length::cm);
		break;
	case 18: //	presetpatternvalLgConfetti, // (Large Confetti) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.1, length::cm);
		break;
	case 19: //	presetpatternvalLgGrid, // (Large Grid) 
		hatch->draw_style_ = hatch_style(hatch_style::doublee);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.25, length::cm);
		break;
	case 20: //	presetpatternvalLtDnDiag, // (Light Downward Diagonal) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 1350;
		hatch->draw_distance_ = length(0.1, length::cm);
		break;
	case 21: //	presetpatternvalLtHorz, // (Light Horizontal) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.1, length::cm);
		break;
	case 22: //	presetpatternvalLtUpDiag, // (Light Upward Diagonal) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 450;
		hatch->draw_distance_ = length(0.1, length::cm);
		break;
	case 23: //	presetpatternvalLtVert, // (Light Vertical) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 900;
		hatch->draw_distance_ = length(0.1, length::cm);
		break;
	case 24: //	presetpatternvalNarHorz, // (Narrow Horizontal) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.05, length::cm);
		break;
	case 25: //	presetpatternvalNarVert, // (Narrow Vertical) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 900;
		hatch->draw_distance_ = length(0.05, length::cm);
		break;
	case 26: //	presetpatternvalOpenDmnd, // (Open Diamond) 
		hatch->draw_style_ = hatch_style(hatch_style::doublee);
		hatch->draw_rotation_ = 450;
		hatch->draw_distance_ = length(0.25, length::cm);
		break;
	case 27: //	presetpatternvalPct10, // (10%) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.125, length::cm);
		break;
	case 28: //	presetpatternvalPct20, // (20%) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.11, length::cm);
		break;
	case 29: //	presetpatternvalPct25, // (25%) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.09, length::cm);
		break;
	case 30: //	presetpatternvalPct30, // (30%) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.07, length::cm);
		break;
	case 31: //	presetpatternvalPct40, // (40%) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.06, length::cm);
		break;
	case 32: //	presetpatternvalPct5, // (5%) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.15, length::cm);
		break;
	case 33: //	presetpatternvalPct50, // (50%) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.05, length::cm);
		break;
	case 34: //	presetpatternvalPct60, // (60%) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.04, length::cm);
		break;
	case 35: //	presetpatternvalPct70, // (70%) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.03, length::cm);
		break;
	case 36: //	presetpatternvalPct75, // (75%) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.025, length::cm);
		break;
	case 37: //	presetpatternvalPct80, // (80%) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.02, length::cm);
		break;
	case 38: //	presetpatternvalPct90, // (90%) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.01, length::cm);
		break;
	case 39: //	presetpatternvalPlaid, // (Plaid) 
		hatch->draw_style_ = hatch_style(hatch_style::doublee);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.15, length::cm);
		break;
	case 40: //	presetpatternvalShingle, // (Shingle) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.25, length::cm);
		break;
	case 41: //	presetpatternvalSmCheck, // (Small Checker Board) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 450;
		hatch->draw_distance_ = length(0.15, length::cm);
		break;
	case 42: //	presetpatternvalSmConfetti, // (Small Confetti) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.05, length::cm);
		break;
	case 43: //	presetpatternvalSmGrid, // (Small Grid) 
		hatch->draw_style_ = hatch_style(hatch_style::doublee);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.1, length::cm);
		break;
	case 44: //	presetpatternvalSolidDmnd, // (Solid Diamond) 
		hatch->draw_style_ = hatch_style(hatch_style::doublee);
		hatch->draw_rotation_ = 450;
		hatch->draw_distance_ = length(0.15, length::cm);
		break;
	case 45: //	presetpatternvalSphere, // (Sphere) 
		hatch->draw_style_ = hatch_style(hatch_style::doublee);
		hatch->draw_rotation_ = 450;
		hatch->draw_distance_ = length(0.1, length::cm);
		break;
	case 46: //	presetpatternvalTrellis, // (Trellis) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.25, length::cm);
		break;
	case 47: //	presetpatternvalUpDiag, // (Upward Diagonal) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 450;
		hatch->draw_distance_ = length(0.2, length::cm);
		break;
	case 48: //	presetpatternvalVert, // (Vertical) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 900;
		hatch->draw_distance_ = length(0.15, length::cm);
		break;
	case 49: //	presetpatternvalWave, // (Wave) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.25, length::cm);
		break;
	case 50: //	presetpatternvalWdDnDiag, // (Wide Downward Diagonal) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 1350;
		hatch->draw_distance_ = length(0.3, length::cm);
		break;
	case 51: //	presetpatternvalWdUpDiag, // (Wide Upward Diagonal) 
		hatch->draw_style_ = hatch_style(hatch_style::single);
		hatch->draw_rotation_ = 450;
		hatch->draw_distance_ = length(0.3, length::cm);
		break;
	case 52: //	presetpatternvalWeave, // (Weave) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 0;
		hatch->draw_distance_ = length(0.25, length::cm);
		break;
	case 53: //	presetpatternvalZigZag, // (Zig Zag) 
		hatch->draw_style_ = hatch_style(hatch_style::triple);
		hatch->draw_rotation_ = 450;
		hatch->draw_distance_ = length(0.25, length::cm);
		break;
	}
}

void odf_drawing_context::end_hatch_style()
{
}

void odf_drawing_context::start_bitmap_style()
{
	if (!impl_->current_graphic_properties) return;
	if (impl_->current_drawing_state_.oox_shape_preset_ == 3000) return;

	odf_writer::office_element_ptr fill_image_element;

	odf_writer::create_element(L"draw", L"fill-image", fill_image_element, impl_->odf_context_);

	draw_fill_image * fill_image = dynamic_cast<draw_fill_image *>(fill_image_element.get());
	if (!fill_image) return;

	fill_image->draw_name_				= impl_->styles_context_->find_free_name(style_family::FillImage);
	fill_image->draw_display_name_		= std::wstring(L"User") + fill_image->draw_name_.get() ;
	fill_image->xlink_attlist_.type_	= xlink_type::Simple;
	fill_image->xlink_attlist_.show_	= xlink_show::Embed;
	fill_image->xlink_attlist_.actuate_	= xlink_actuate::OnLoad;
	
	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_image_name_ = fill_image->draw_name_;
	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_ = draw_fill(draw_fill::bitmap);
	
	impl_->styles_context_->add_style(fill_image_element, false, true, style_family::FillImage);
	
	set_image_style_repeat(0);

}
void odf_drawing_context::end_bitmap_style()
{
}
void odf_drawing_context::set_bitmap_tile_align(int align)
{
	if (!impl_->current_graphic_properties)return;
	switch (align)
	{
		case 0: //	rectalignmentB   = 0,
			impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_image_ref_point_ = fill_image_ref_point(fill_image_ref_point::bottom);break;
		case 1: //	rectalignmentBL  = 1,
			impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_image_ref_point_ = fill_image_ref_point(fill_image_ref_point::bottom_left);break;
		case 2: //	rectalignmentBR  = 2,
			impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_image_ref_point_ = fill_image_ref_point(fill_image_ref_point::bottom_right);break;
		case 3: //	rectalignmentCtr = 3,
			impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_image_ref_point_ = fill_image_ref_point(fill_image_ref_point::center);break;
		case 4: //	rectalignmentL   = 4,
			impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_image_ref_point_ = fill_image_ref_point(fill_image_ref_point::left);break;
		case 5: //	rectalignmentR   = 5,
			impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_image_ref_point_ = fill_image_ref_point(fill_image_ref_point::right);break;
		case 6: //	rectalignmentT   = 6,
			impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_image_ref_point_ = fill_image_ref_point(fill_image_ref_point::top);break;
		case 7: //	rectalignmentTL  = 7,
			impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_image_ref_point_ = fill_image_ref_point(fill_image_ref_point::top_left);break;
		case 8: //	rectalignmentTR  = 8,
			impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_image_ref_point_ = fill_image_ref_point(fill_image_ref_point::top_right);break;
	}
}
void odf_drawing_context::set_image_style_repeat(int style)
{
	if (!impl_->current_graphic_properties)		return;

	if (style == 1)
		impl_->current_graphic_properties->common_draw_fill_attlist_.style_repeat_ = style_repeat(style_repeat::Stretch);
	else if (style ==2)
		impl_->current_graphic_properties->common_draw_fill_attlist_.style_repeat_ = style_repeat(style_repeat::Repeat);
	else
		impl_->current_graphic_properties->common_draw_fill_attlist_.style_repeat_ = style_repeat(style_repeat::NoRepeat);//default
}
void odf_drawing_context::set_image_client_rect_pt(double l, double t, double r, double b)
{
	if (!impl_->current_graphic_properties)return;
	
	//<top>, <right>, <bottom>, <left> 
	std::wstringstream str_stream;
	str_stream << std::wstring(L"rect(") 
							<< length(length(t, length::pt).get_value_unit(length::cm),length::cm) << std::wstring(L",")
							<< length(length(r, length::pt).get_value_unit(length::cm),length::cm) << std::wstring(L",")
							<< length(length(b, length::pt).get_value_unit(length::cm),length::cm) << std::wstring(L",")
							<< length(length(l, length::pt).get_value_unit(length::cm),length::cm) << std::wstring(L")");

	impl_->current_graphic_properties->fo_clip_ = str_stream.str();

}
void odf_drawing_context::set_image_client_rect_inch(double l, double t, double r, double b)
{
	if (!impl_->current_graphic_properties)return;
	
	//<top>, <right>, <bottom>, <left> 
	std::wstringstream str_stream;
	//str_stream	<< std::wstring(L"rect(") 
	//			<< t << std::wstring(L"in, ")
	//			<< r << std::wstring(L"in, ")
	//			<< b << std::wstring(L"in, ")
	//			<< l << std::wstring(L"in)");

	str_stream	<< std::wstring(L"rect(") 
				<< (t * 2.54) << std::wstring(L"cm, ")
				<< (r * 2.54) << std::wstring(L"cm, ")
				<< (b * 2.54) << std::wstring(L"cm, ")
				<< (l * 2.54) << std::wstring(L"cm)");
	impl_->current_graphic_properties->fo_clip_ = str_stream.str();

}
void odf_drawing_context::set_bitmap_link(std::wstring file_path)
{
	std::wstring odf_ref_name ;	
	impl_->odf_context_->mediaitems()->add_or_find(file_path, _mediaitems::typeImage, odf_ref_name);
	
	if (impl_->current_drawing_state_.oox_shape_preset_ == 3000)
	{
		if (impl_->current_level_.empty()) return;
		
		draw_image* image = dynamic_cast<draw_image*>(impl_->current_level_.back().get());
		if (image == NULL)return;

		image->common_xlink_attlist_.href_= odf_ref_name;
	}
	else
	{
		draw_fill_image * fill_image = dynamic_cast<draw_fill_image *>(impl_->styles_context_->last_state()->get_office_element().get());
		if (!fill_image) return;
	   
		fill_image->xlink_attlist_.href_= odf_ref_name;
	}
}
void odf_drawing_context::set_bitmap_tile_scale_x(double scale_x)
{
	if (!impl_->current_graphic_properties)return;
	
	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_image_width_ = 
				length(length(scale_x,length::pt).get_value_unit(length::inch), length::inch);

}
void odf_drawing_context::set_bitmap_tile_scale_y(double scale_y)
{
	if (!impl_->current_graphic_properties)return;
	
	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_image_height_ = 
				length(length(scale_y,length::pt).get_value_unit(length::inch), length::inch);

}
void odf_drawing_context::set_bitmap_tile_translate_y(double y)
{
	if (!impl_->current_graphic_properties)return;
	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_image_ref_point_y_ = percent(y);

}
void odf_drawing_context::set_bitmap_tile_translate_x(double x)
{
	if (!impl_->current_graphic_properties)return;
	impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_image_ref_point_x_ = percent(x);
}

office_element_ptr & odf_drawing_context::get_root_element()
{
	return impl_->root_element_;
}
///////////////////
}
}
