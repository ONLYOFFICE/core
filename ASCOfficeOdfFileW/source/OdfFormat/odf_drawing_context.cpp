#include "precompiled_cpodf.h"
#include "logging.h"

#include <boost/foreach.hpp>

#include <iostream>

#include "odf_drawing_context.h"
#include "odf_text_context.h"
#include "odf_style_context.h"
#include "odf_conversion_context.h"

#include "draw_frame.h"
#include "draw_shapes.h"

#include "oox_shape_defines.h"
#include "Shapes\odf_shape_mapping.h"

#include "styles.h"

#include "style_table_properties.h"
#include "style_text_properties.h"
#include "style_paragraph_properties.h"
#include "style_graphic_properties.h"



namespace cpdoccore 
{
namespace odf
{

struct 	odf_group_state;
typedef shared_ptr<odf_group_state>::Type odf_group_state_ptr;

struct 	odf_group_state
{
	odf_group_state(office_element_ptr	elm_, int level_, odf_group_state_ptr prev)
	{
		shift_x = shift_y =x = y = rotate = 0;
		scale_cx = scale_cy = 1.;
		flipH = flipV = false;

		elm = elm_;
		level = level_;
		
		prev_group = prev;
	}
	office_element_ptr	elm;
	
	int level;

	double shift_x;
	double shift_y;

	double x;
	double y;
	
	double cx;
	double cy;

	double scale_cx;
	double scale_cy;

	double rotate;

	bool flipH;
	bool flipV;

	odf_group_state_ptr prev_group;
};

struct anchor_settings
{
	_CP_OPT(length) svg_x_;
	_CP_OPT(length) svg_y_;
	_CP_OPT(length) svg_height_;
	_CP_OPT(length) svg_width_;	

	_CP_OPT(length) style_vertical_pos_svg_y_;
	_CP_OPT(length) style_horizontal_pos_svg_x_;
	
	_CP_OPT(vertical_pos)	style_vertical_pos_;
	_CP_OPT(horizontal_pos) style_horizontal_pos_;

	_CP_OPT(vertical_rel)	style_vertical_rel_;
	_CP_OPT(horizontal_rel) style_horizontal_rel_;

	_CP_OPT(length)		fo_margin_left_; 
	_CP_OPT(length)		fo_margin_top_; 
	_CP_OPT(length)		fo_margin_right_; 
	_CP_OPT(length)		fo_margin_bottom_; 

	_CP_OPT(anchor_type) anchor_type_;
	_CP_OPT(run_through) run_through_;

	_CP_OPT(style_wrap) style_wrap_;

	void clear()
	{
		svg_x_ = boost::none;
		svg_y_ = boost::none;
		svg_height_ = boost::none;
		svg_height_ = boost::none;
		
		style_vertical_pos_svg_y_ = boost::none;
		style_horizontal_pos_svg_x_ = boost::none;
		
		anchor_type_ = boost::none;
		
		style_vertical_pos_ = boost::none;
		style_horizontal_pos_ = boost::none;
		
		style_vertical_rel_	= boost::none;
		style_horizontal_rel_ = boost::none;

		fo_margin_left_		= boost::none;
		fo_margin_top_		= boost::none;
		fo_margin_right_	= boost::none;
		fo_margin_bottom_	= boost::none;

		style_wrap_ = boost::none;
	}
};	

enum _drawing_part
{
	Unknown=0,
	Area,
	Line,
	Shadow
};
struct odf_drawing_state
{
	void clear()
	{
		elements_.clear();
		
		svg_x_= boost::none;
		svg_y_= boost::none;
		svg_height_= boost::none;
		svg_width_= boost::none;

		name_ = L"";
		z_order_ = -1;
		
		rotateAngle = boost::none;
		
		path_ = L"";
		view_box_ = L"";
		path_last_command_ = L"";
		modifiers_ = L"";

		oox_shape_preset = -1;

		in_group = false;

		flipH = flipV = false;

	}
	std::vector<odf_element_state> elements_;

	_CP_OPT(length) svg_x_;
	_CP_OPT(length) svg_y_;
	_CP_OPT(length) svg_height_;
	_CP_OPT(length) svg_width_;	


	std::wstring name_;
	int z_order_;

	_CP_OPT(double) rotateAngle;

	bool flipH;
	bool flipV;

	std::wstring path_;
	std::wstring view_box_;
	std::wstring path_last_command_;
	std::wstring modifiers_;

///////////////////////
	int oox_shape_preset;
	bool in_group;

};

class odf_drawing_context::Impl
{
public:
	Impl(odf_conversion_context *odf_context) :odf_context_(odf_context)
    {	
		current_drawing_state_.clear();
		styles_context_ = odf_context_->styles_context();
		
		current_graphic_properties = NULL;
		current_paragraph_properties = NULL;

		width = height = x = y = 0;
	} 
	
	odf_drawing_state				current_drawing_state_;
	_drawing_part					current_drawing_part_;
	
	std::vector<office_element_ptr> current_level_;//постоянно меняющийся список уровней наследования

	odf_style_context			*styles_context_;
	odf_conversion_context		*odf_context_;

	void create_draw_base(int type);
	office_element_ptr create_draw_element(int type);

	style_graphic_properties		*current_graphic_properties;
	style_paragraph_properties		*current_paragraph_properties;

	anchor_settings anchor_settings_;

	graphic_format_properties		preset_graphic_format_properties;

	odf_group_state_ptr					current_group_;
	std::vector<odf_group_state_ptr>	group_list_; //группы
	std::vector<odf_drawing_state>		drawing_list_;	//все элементы(кроме групп) .. для удобства разделение по "топам"

	office_element_ptr root_element_;
	
	double x;
	double y;

	double width;
	double height;

};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
odf_drawing_context::odf_drawing_context(odf_conversion_context *odf_context)  
	: impl_(new  odf_drawing_context::Impl(odf_context))
{
}

odf_drawing_context::~odf_drawing_context()
{
}

office_element_ptr & odf_drawing_context::get_current_element(){return impl_->current_level_.back();}

void odf_drawing_context::set_styles_context(odf_style_context*  styles_context)
{
	impl_->styles_context_ = styles_context;
}

void odf_drawing_context::start_group(std::wstring name, int id)
{
	office_element_ptr & group_elm = impl_->create_draw_element(5000);

	draw_g* group = dynamic_cast<draw_g*>(group_elm.get());

	int level = impl_->current_level_.size();
//////////////////////////
	
	odf_group_state_ptr group_state = boost::shared_ptr<odf_group_state>(new odf_group_state(group_elm, level,impl_->current_group_));
	impl_->group_list_.push_back(group_state);

	impl_->current_group_ = impl_->group_list_.back();
	
	if (impl_->current_level_.size()>0)
		impl_->current_level_.back()->add_child_element(group_elm);

	impl_->current_level_.push_back(group_elm);
	
	if (group== NULL)return;

	group->common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_draw_name_attlist_.draw_name_ = name;
	if (id >=0)
		group->common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_draw_z_index_attlist_.draw_z_index_ = id;

}
void odf_drawing_context::end_group()
{
	if (impl_->group_list_.size() < 1)return;
	
	impl_->current_group_ = impl_->group_list_.back()->prev_group;
	impl_->current_level_.pop_back();
}


void odf_drawing_context::clear()
{
	impl_->root_element_ = office_element_ptr();

	impl_->anchor_settings_.clear();

}

void odf_drawing_context::start_drawing()
{
	impl_->current_drawing_state_.clear();

	if (impl_->current_level_.size() < 1)
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
	else 
	{
		impl_->current_drawing_state_.in_group = true;
	}

}
void odf_drawing_context::end_drawing()
{
	if (impl_->current_drawing_state_.elements_.size() < 1) return;

	draw_base* draw = dynamic_cast<draw_base*>(impl_->current_drawing_state_.elements_[0].elm.get());

	if (draw)
	{
		if (impl_->current_drawing_state_.name_.length() > 0)
			draw->common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_draw_name_attlist_.draw_name_ = impl_->current_drawing_state_.name_;
		if (impl_->current_drawing_state_.z_order_ >= 0)
			draw->common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_draw_z_index_attlist_.draw_z_index_ = impl_->current_drawing_state_.z_order_;

		std::wstring strTransform;

		if (impl_->current_drawing_state_.in_group && impl_->current_group_)
		{
			double rotate = impl_->current_group_->rotate;
			if (impl_->current_drawing_state_.rotateAngle )
				rotate += *impl_->current_drawing_state_.rotateAngle;

			if (abs(rotate)>0.001)impl_->current_drawing_state_.rotateAngle = rotate;
		}
		double x = impl_->current_drawing_state_.svg_x_->get_value();
		double y = impl_->current_drawing_state_.svg_y_->get_value();


		if (impl_->current_drawing_state_.rotateAngle)
		{

			strTransform += std::wstring(L"rotate(") + boost::lexical_cast<std::wstring>(impl_->current_drawing_state_.rotateAngle.get()) + std::wstring(L")");
			//так как вращения все в мс относительно центра фигуры, а не от начала координат - убираем смещение

			if (impl_->current_drawing_state_.svg_x_ && impl_->current_drawing_state_.svg_y_)
			{
				if (impl_->current_drawing_state_.in_group)
				{
					strTransform += std::wstring(L" translate(") +	boost::lexical_cast<std::wstring>(impl_->current_drawing_state_.svg_x_.get()) +
																	/*(impl_->current_drawing_state_.svg_width_.get()/2))+*/ std::wstring(L",") + 
												boost::lexical_cast<std::wstring>(impl_->current_drawing_state_.svg_y_.get()) +
																	/*(impl_->current_drawing_state_.svg_height_.get()/2))+ */std::wstring(L")") ; 
				}
				else
				{
					strTransform += std::wstring(L" translate(") +	boost::lexical_cast<std::wstring>(impl_->current_drawing_state_.svg_x_.get() +
																	(impl_->current_drawing_state_.svg_width_.get()/2))+ std::wstring(L",") + 
												boost::lexical_cast<std::wstring>(impl_->current_drawing_state_.svg_y_.get() +
																	(impl_->current_drawing_state_.svg_height_.get()/2))+ std::wstring(L")") ; 
				}
			}

			impl_->current_drawing_state_.svg_x_ = boost::none;
			impl_->current_drawing_state_.svg_y_ = boost::none;
		}

		if (strTransform.length()>0)
			draw->common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_draw_transform_attlist_.draw_transform_ = strTransform;

		draw->common_draw_attlists_.position_.svg_x_ = impl_->current_drawing_state_.svg_x_;
		draw->common_draw_attlists_.position_.svg_y_ = impl_->current_drawing_state_.svg_y_;

		draw->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_		= impl_->current_drawing_state_.svg_height_;
		draw->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_		= impl_->current_drawing_state_.svg_width_;
	}
///////////////////////////////////////////////////////
	if (impl_->current_drawing_state_.in_group)
	{
		odf_group_state_ptr gr = impl_->current_group_;

		while(gr)
		{
			impl_->current_drawing_state_.flipH = impl_->current_drawing_state_.flipH ^ gr->flipH;
			impl_->current_drawing_state_.flipV = impl_->current_drawing_state_.flipV ^ gr->flipV;
			
			gr = gr->prev_group;
		}
	}
	draw_custom_shape* custom = dynamic_cast<draw_custom_shape*>(draw);
	if (custom)
	{
		draw_enhanced_geometry* enhan = dynamic_cast<draw_enhanced_geometry*>(custom->draw_enhanced_geometry_.get());
		if(enhan)
		{
			if (impl_->current_drawing_state_.flipV) enhan->draw_enhanced_geometry_attlist_.draw_mirror_vertical_ = true;
			if (impl_->current_drawing_state_.flipH) enhan->draw_enhanced_geometry_attlist_.draw_mirror_horizontal_ = true;
		}
	}else
	{
		//не поддерживается :( - нужно считать искажения на простейшие фигуры - линии, ректы, эллипсы 
	}


	impl_->current_graphic_properties->content().common_vertical_pos_attlist_.style_vertical_pos_		= impl_->anchor_settings_.style_vertical_pos_;
	impl_->current_graphic_properties->content().common_horizontal_pos_attlist_.style_horizontal_pos_	= impl_->anchor_settings_.style_horizontal_pos_;

	impl_->current_graphic_properties->content().common_vertical_rel_attlist_.style_vertical_rel_		= impl_->anchor_settings_.style_vertical_rel_;
	impl_->current_graphic_properties->content().common_horizontal_rel_attlist_.style_horizontal_rel_	= impl_->anchor_settings_.style_horizontal_rel_;

	impl_->current_graphic_properties->content().common_horizontal_margin_attlist_.fo_margin_left_	= impl_->anchor_settings_.fo_margin_left_; 
	impl_->current_graphic_properties->content().common_vertical_margin_attlist_.fo_margin_top_		= impl_->anchor_settings_.fo_margin_top_; 
	impl_->current_graphic_properties->content().common_horizontal_margin_attlist_.fo_margin_right_	= impl_->anchor_settings_.fo_margin_right_; 
	impl_->current_graphic_properties->content().common_vertical_margin_attlist_.fo_margin_bottom_	= impl_->anchor_settings_.fo_margin_bottom_; 

	if (draw)
		draw->common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_text_spreadsheet_shape_attlist_.common_text_anchor_attlist_.type_ = impl_->anchor_settings_.anchor_type_;

	impl_->current_graphic_properties->content().style_wrap_ = impl_->anchor_settings_.style_wrap_;
	impl_->current_graphic_properties->content().style_run_through_ = impl_->anchor_settings_.run_through_;
///////////////////////////////////////////////////		
	impl_->drawing_list_.push_back(impl_->current_drawing_state_);
	
///////////////
	impl_->current_drawing_state_.clear();
	impl_->current_graphic_properties = NULL;
	impl_->current_paragraph_properties = NULL;
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
office_element_ptr odf_drawing_context::Impl::create_draw_element(int type)
{
	office_element_ptr element;
	switch(type)
	{
	case 0:
		create_element(L"draw", L"frame", element, odf_context_);
		break;
	case 1:
		create_element(L"draw", L"caption", element, odf_context_);
		break;
	case 2:
		create_element(L"draw", L"rect", element, odf_context_);
		break;
	case 3:
		create_element(L"draw", L"ellipse", element, odf_context_);
		break;
	case 4:
		create_element(L"draw", L"circle", element, odf_context_);
		break;
	case 5:
		create_element(L"draw", L"line", element, odf_context_);
		break;
	case 6:
		create_element(L"draw", L"path", element, odf_context_);
		break;
	case 7:
		create_element(L"draw", L"custom-shape", element, odf_context_);
		break;
	case 8:
		create_element(L"draw", L"polygon", element, odf_context_);
		break;
	case 9:
		create_element(L"draw", L"connector", element, odf_context_);
		break;
	case 5000:
		create_element(L"draw", L"g", element, odf_context_);
		break;
	}
	if (root_element_ == NULL) root_element_ = element;

	return element;
}
void odf_drawing_context::Impl::create_draw_base(int type)
{	
	office_element_ptr & draw_elm = create_draw_element(type);

	draw_base* draw = dynamic_cast<draw_base*>(draw_elm.get());
	if (draw == NULL)return;
//////////	
	styles_context_->create_style(L"",style_family::Graphic, true, false, -1);		
	
	office_element_ptr & style_shape_elm = styles_context_->last_state()->get_office_element();
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_shape_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		current_graphic_properties = style_->style_content_.get_style_graphic_properties();
	}

	draw->common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_draw_style_name_attlist_.draw_style_name_ = style_ref(style_name);
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int level = current_level_.size();
	
	if (current_level_.size()>0)
		current_level_.back()->add_child_element(draw_elm);

	current_level_.push_back(draw_elm);

	odf_element_state state={draw_elm, style_name, style_shape_elm, level};

	current_drawing_state_.elements_.push_back(state);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
void odf_drawing_context::start_shape(int type)
{
	impl_->current_drawing_state_.oox_shape_preset = type;
	
	if (type < sizeof(Shape_Types_Mapping)/sizeof(_sh_typ))
	{
		impl_->create_draw_base(Shape_Types_Mapping[type].second);
	}
	else if (type == 1000)
	{
		impl_->create_draw_base(7);//пока кастом .. потом переделать на path, что правильнее
	}
	else if (type == 2000)
	{
		impl_->create_draw_base(10);//text-box
	}
}
void odf_drawing_context::end_shape()
{
	if (impl_->current_drawing_state_.elements_.size() < 1) 
		return;
	//вторичные, вычисляемые свойства шейпов

	draw_path* path = dynamic_cast<draw_path*>(impl_->current_drawing_state_.elements_[0].elm.get());
	if (path)
	{
		if (impl_->current_drawing_state_.path_.length()>1)
			path->draw_path_attlist_.svg_d_ = impl_->current_drawing_state_.path_;
		if (impl_->current_drawing_state_.view_box_.length()>1)
			path->draw_path_attlist_.svg_viewbox_ = impl_->current_drawing_state_.view_box_;
	}
////////////////////////////////////////////////////////////////////////////////////
	draw_connector* connector = dynamic_cast<draw_connector*>(impl_->current_level_.back().get());
	if (connector)
	{
		if (!connector->draw_connector_attlist_.draw_type_) connector->draw_connector_attlist_.draw_type_ = L"line";

		if (impl_->current_drawing_state_.svg_x_)connector->draw_line_attlist_.svg_x1_ = impl_->current_drawing_state_.svg_x_;
		else connector->draw_line_attlist_.svg_x1_ = length(0,length::cm);

		if (impl_->current_drawing_state_.svg_y_)connector->draw_line_attlist_.svg_y1_ = impl_->current_drawing_state_.svg_y_;
		else connector->draw_line_attlist_.svg_y1_ = length(0,length::cm);

		connector->draw_line_attlist_.svg_x2_ = connector->draw_line_attlist_.svg_x1_;
		connector->draw_line_attlist_.svg_y2_ = connector->draw_line_attlist_.svg_y1_;
		
		if (impl_->current_drawing_state_.svg_width_)
			connector->draw_line_attlist_.svg_x2_ = connector->draw_line_attlist_.svg_x2_.get() + impl_->current_drawing_state_.svg_width_.get();
		
		if (impl_->current_drawing_state_.svg_height_)
			connector->draw_line_attlist_.svg_y2_ = connector->draw_line_attlist_.svg_y2_.get() + impl_->current_drawing_state_.svg_height_.get();
	}
////////////////////////////////////////////////////////////////////////////////////////////
	draw_line* line = dynamic_cast<draw_line*>(impl_->current_level_.back().get());
	if (line)
	{
		line->draw_line_attlist_.svg_x1_ = impl_->current_drawing_state_.svg_x_;
		line->draw_line_attlist_.svg_y1_ = impl_->current_drawing_state_.svg_y_;
		
		if (impl_->current_drawing_state_.svg_x_ && impl_->current_drawing_state_.svg_width_)
			line->draw_line_attlist_.svg_x2_ = impl_->current_drawing_state_.svg_x_.get() + impl_->current_drawing_state_.svg_width_.get();
		
		if (impl_->current_drawing_state_.svg_y_ && impl_->current_drawing_state_.svg_height_)
			line->draw_line_attlist_.svg_y2_ = impl_->current_drawing_state_.svg_y_.get() + impl_->current_drawing_state_.svg_height_.get();
	}

//////////////////////////////////////////////////////////////////////////////////////////////////
	draw_custom_shape* custom = dynamic_cast<draw_custom_shape*>(impl_->current_drawing_state_.elements_[0].elm.get());
	if (custom)
	{
		std::wstring sub_type;
		
		if (impl_->current_drawing_state_.oox_shape_preset < sizeof(Shape_Types_Mapping)/sizeof(_sh_typ))
		{
			sub_type = Shape_Types_Mapping[impl_->current_drawing_state_.oox_shape_preset].first;
		}
		else
		{
			sub_type = L"polyline";
		}
		
		office_element_ptr enhanced_elm;
		create_element(L"draw", L"enhanced-geometry", enhanced_elm, impl_->odf_context_);
		
		start_element(enhanced_elm);
		
		draw_enhanced_geometry* enhanced = dynamic_cast<draw_enhanced_geometry*>(enhanced_elm.get());
		if (enhanced)
		{

			if (impl_->current_drawing_state_.path_.length()>1)
			{
				enhanced->draw_enhanced_geometry_attlist_.draw_enhanced_path_ =impl_->current_drawing_state_.path_;
			}
			if (impl_->current_drawing_state_.view_box_.length()>1)
			{
				enhanced->svg_viewbox_ = impl_->current_drawing_state_.view_box_;
			}
			if (sub_type.length()>1)
			{
				enhanced->draw_enhanced_geometry_attlist_.draw_type_ = sub_type;

				int res=0;
				if ((res = sub_type.find(L"ooxml")) >= 0 && impl_->current_drawing_state_.modifiers_.length()>1)
				{
					enhanced->draw_enhanced_geometry_attlist_.draw_modifiers_ = impl_->current_drawing_state_.modifiers_;
				}
			}
			else
			{
				oox_shape_ptr shape_define = oox_shape::create(impl_->current_drawing_state_.oox_shape_preset);

				if (shape_define)
				{
					impl_->current_drawing_state_.rotateAngle = boost::none;

					enhanced->svg_viewbox_										= shape_define->view_box;
					enhanced->draw_enhanced_geometry_attlist_.draw_type_		= shape_define->odf_type_name;
					enhanced->draw_enhanced_geometry_attlist_.draw_text_areas_	= shape_define->text_areas;
					if (shape_define->glue_points)
					{
						enhanced->draw_enhanced_geometry_attlist_.draw_glue_points_	= *shape_define->glue_points;
					}

					if (impl_->current_drawing_state_.modifiers_.length()>1)
					{
						enhanced->draw_enhanced_geometry_attlist_.draw_modifiers_ = impl_->current_drawing_state_.modifiers_;
					}
					else // обязательно нужны дефолтовые
						enhanced->draw_enhanced_geometry_attlist_.draw_modifiers_ = shape_define->modifiers;

					enhanced->draw_enhanced_geometry_attlist_.draw_enhanced_path_ = shape_define->enhanced_path;

					for (long i=0; i < shape_define->equations.size();i++)
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
					for (long i=0; i < shape_define->handles.size();i++)
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
						}
						end_element();
					}
				}
			}
		}
		end_element();
	}
	end_element();
}

void odf_drawing_context::start_frame()
{
	impl_->create_draw_base(0);
}

void odf_drawing_context::end_frame()
{
	end_element();
}
/////////////////////
void odf_drawing_context::start_element(office_element_ptr & elm, office_element_ptr  style_elm)
{
	int level = impl_->current_level_.size();
	
	if (impl_->current_level_.size()>0)
		impl_->current_level_.back()->add_child_element(elm);

	std::wstring style_name;
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		impl_->current_graphic_properties = style_->style_content_.get_style_graphic_properties();
	}

	odf_element_state state={elm, style_name, style_elm, level};
	impl_->current_drawing_state_.elements_.push_back(state);

	impl_->current_level_.push_back(elm);
}
void odf_drawing_context::end_element()
{
	impl_->current_level_.pop_back();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void odf_drawing_context::start_area_properies()
{
	impl_->current_drawing_part_ = Area;
}
void odf_drawing_context::end_area_properies()
{
	impl_->current_drawing_part_ = Unknown;
}
void odf_drawing_context::start_line_properies()
{
	impl_->current_drawing_part_ = Line;
}
void odf_drawing_context::end_line_properies()
{
	impl_->current_drawing_part_ = Unknown;
}
//void odf_drawing_context::start_shadow_properies()
//{
//	impl_->current_drawing_part_ = Shadow;
//}
//void odf_drawing_context::end_shadow_properies()
//{
//	impl_->current_drawing_part_ = Unknown;
//}
////////////////////////////////////////////////////////////////////
void odf_drawing_context::set_name(std::wstring  name)
{
	impl_->current_drawing_state_.name_ = name;
}
void odf_drawing_context::set_opacity(double percent_)
{
	if (!impl_->current_graphic_properties)return;
	if (percent_ < 0.01) return;

	switch(impl_->current_drawing_part_)
	{
	case Area:
		impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_opacity_ = percent(percent_);
		break;
	case Line:
		impl_->current_graphic_properties->content().svg_stroke_opacity_ = percent(percent_);
		break;
	}
}
void odf_drawing_context::set_shadow(int type, std::wstring hexColor, _CP_OPT(double) opacity, double dist_pt, double dist_pt_y )
{
	if (!impl_->current_graphic_properties)return;

	impl_->current_graphic_properties->content().draw_shadow_ = shadow_type1(shadow_type1::Visible);
	if (opacity) impl_->current_graphic_properties->content().draw_shadow_opacity_ = *opacity;

	impl_->current_graphic_properties->content().draw_shadow_color_ = hexColor;
	impl_->current_graphic_properties->content().draw_shadow_offset_y_ = length(length(dist_pt,length::pt).get_value_unit(length::cm),length::cm);
	
	if (dist_pt_y > 0)
		impl_->current_graphic_properties->content().draw_shadow_offset_x_ = length(length(dist_pt_y,length::pt).get_value_unit(length::cm),length::cm);
	else
		impl_->current_graphic_properties->content().draw_shadow_offset_x_ = length(length(dist_pt,length::pt).get_value_unit(length::cm),length::cm);
}

void odf_drawing_context::set_no_fill()
{
	if (!impl_->current_graphic_properties)return;

	switch(impl_->current_drawing_part_)
	{
	case Area:
		impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_ = draw_fill::none;
		break;
	case Line:
		//impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_ = draw_fill::none;??????
		impl_->current_graphic_properties->content().draw_stroke_ = line_style (line_style::None);
		break;
	}
}
void odf_drawing_context::set_solid_fill(std::wstring hexColor)
{
	if (!impl_->current_graphic_properties)return;

	switch(impl_->current_drawing_part_)
	{
	case Area:
		impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_ = draw_fill::solid;
		impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_color_ = color(std::wstring(L"#") + hexColor);
		impl_->current_graphic_properties->content().common_background_color_attlist_.fo_background_color_ = color(std::wstring(L"#") + hexColor);
		//последнее нужно - что если будут вводить текст - под текстом будет цвет фона (или он поменяется в полях текста)
		break;
	case Line:
		impl_->current_graphic_properties->content().svg_stroke_color_ = color(std::wstring(L"#") + hexColor);
		impl_->current_graphic_properties->content().draw_stroke_=line_style(line_style::Solid);//default
		impl_->current_graphic_properties->content().svg_stroke_width_ = length(length(0.08,length::pt).get_value_unit(length::cm),length::cm);//default
		break;
	}
}
void odf_drawing_context::set_z_order(int id)
{
	impl_->current_drawing_state_.z_order_ = id;

}
void odf_drawing_context::add_path_element(std::wstring command, std::wstring & strE)
{
	if (command != impl_->current_drawing_state_.path_last_command_)
	{
		impl_->current_drawing_state_.path_ += command + L" ";
		impl_->current_drawing_state_.path_last_command_ = command;
	}

	impl_->current_drawing_state_.path_ += strE + L" ";
}
void odf_drawing_context::add_modifier(std::wstring modifier)
{
	boost::algorithm::to_lower(modifier);
	int res = modifier.find(L"val ");
	if (res >=0) modifier = modifier.substr(4);
	impl_->current_drawing_state_.modifiers_ += modifier + L" ";
}
void odf_drawing_context::set_viewBox(double W, double H)
{
	impl_->current_drawing_state_.view_box_ = std::wstring(L"0 0 ") + boost::lexical_cast<std::wstring>((int)W) + L" " + boost::lexical_cast<std::wstring>((int)H); 
}
void odf_drawing_context::set_flip_H(bool bVal)
{
	if (impl_->current_graphic_properties == NULL) return;
	if (bVal == false)return;
//for image 
	if (impl_->current_graphic_properties->content().style_mirror_)
		impl_->current_graphic_properties->content().style_mirror_ = *impl_->current_graphic_properties->content().style_mirror_ + std::wstring(L" horizontal");
	else
		impl_->current_graphic_properties->content().style_mirror_ = std::wstring(L"horizontal");
//else	
	impl_->current_drawing_state_.flipH = bVal;
}
void odf_drawing_context::set_flip_V(bool bVal)
{
	if (impl_->current_graphic_properties == NULL) return;
	if (bVal == false)return;
//for image 
	if (impl_->current_graphic_properties->content().style_mirror_)
		impl_->current_graphic_properties->content().style_mirror_ = *impl_->current_graphic_properties->content().style_mirror_ + std::wstring(L" vertical");
	else
		impl_->current_graphic_properties->content().style_mirror_ = std::wstring(L"vertical");
//else
	impl_->current_drawing_state_.flipV = bVal;
}

void odf_drawing_context::set_rotate(int iVal)
{
	double dRotate = (360 - iVal/60000.)/180. * 3.14159265358979323846;
	impl_->current_drawing_state_.rotateAngle = dRotate;
}
void odf_drawing_context::set_drawings_rect(double x_pt, double y_pt, double width_pt, double height_pt)// "- 1" не задано
{
	//хороший тон сохранить все размеры в см (хотя можно и в другой системе)
	if (x_pt >= 0)	impl_->anchor_settings_.svg_x_ = length(length(x_pt,length::pt).get_value_unit(length::cm),length::cm);
	if (y_pt >= 0)	impl_->anchor_settings_.svg_y_ = length(length(y_pt,length::pt).get_value_unit(length::cm),length::cm);

	impl_->x = x_pt >=0 ? x_pt :0 ;
	impl_->y = y_pt >=0 ? y_pt :0;

	impl_->width = width_pt;
	impl_->height = height_pt;

	if (height_pt >= 0) impl_->anchor_settings_.svg_height_	= length(length(height_pt,length::pt).get_value_unit(length::cm),length::cm);	
	if (width_pt >= 0)	impl_->anchor_settings_.svg_width_	= length(length(width_pt,length::pt).get_value_unit(length::cm),length::cm);	
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
	impl_->anchor_settings_.anchor_type_ = anchor_type((anchor_type::type)type);
}
//////////////////////////////////////////////////////////////////////////////////////
void odf_drawing_context::set_vertical_rel(int from)
{
	vertical_rel::type type;

	switch(from)
	{
	case 0:	type = vertical_rel::Baseline;		break;//	relfromvBottomMargin ???
	case 1:	type = vertical_rel::Baseline;		break;//	relfromvInsideMargin ???
	case 2:	type = vertical_rel::Line;			break;//	relfromvLine          
	case 3:	type = vertical_rel::Baseline;		break;//	relfromvMargin       ???
	case 4:	type = vertical_rel::Baseline;		break;//	relfromvOutsideMargin ???
	case 5:	type = vertical_rel::Page;			
		impl_->anchor_settings_.anchor_type_ = anchor_type(anchor_type::Page);
		break;//	relfromvPage          
	case 6:	type = vertical_rel::Paragraph;	
		impl_->anchor_settings_.anchor_type_ = anchor_type(anchor_type::Paragraph);
		break;//	relfromvParagraph    
	case 7:	type = vertical_rel::Baseline;		break;//	relfromvTopMargin   ???  
	}

	impl_->anchor_settings_.style_vertical_rel_ = vertical_rel(type);
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
		case 2:	type = horizontal_rel::ParagraphStartMargin;break;	//	relfromhInsideMargin  = 2, ???
		case 3:	type = horizontal_rel::PageStartMargin;		break;	//	relfromhLeftMargin    = 3,
		case 4:	type = horizontal_rel::ParagraphStartMargin;break;	//	relfromhMargin        = 4, ???
		case 5:	type = horizontal_rel::ParagraphEndMargin;	break;	//	relfromhOutsideMargin = 5,
		case 6:	type = horizontal_rel::Page;				break;	//	relfromhPage          = 6, ???
		case 7:	type = horizontal_rel::PageEndMargin;		break;	//	relfromhRightMargin   = 7
	}

	impl_->anchor_settings_.style_horizontal_rel_ = horizontal_rel(type);
	impl_->anchor_settings_.style_horizontal_pos_ = horizontal_pos(horizontal_pos::FromLeft);//default
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
	case 4:	type =	horizontal_pos::Right;		break;//alignhRight   = 4
	}
	impl_->anchor_settings_.style_horizontal_pos_ = horizontal_pos(type);
}
void odf_drawing_context::set_horizontal_pos(double offset_pt)
{
	impl_->anchor_settings_.style_horizontal_pos_svg_x_ = length(length(offset_pt,length::pt).get_value_unit(length::cm),length::cm);
	impl_->x = offset_pt;
}

void odf_drawing_context::set_wrap_style(style_wrap::type type)
{
	impl_->anchor_settings_.style_wrap_ = style_wrap(type);
}
void odf_drawing_context::set_overlap (bool val)
{
	//if (val) impl_->anchor_settings_.run_through_ = run_through(run_through::Background);
	//else impl_->anchor_settings_.run_through_ = run_through(run_through::Foreground);

	if (val) impl_->anchor_settings_.style_wrap_ = style_wrap(style_wrap::RunThrough);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void odf_drawing_context::set_group_position(double x, double y, double ch_x, double ch_y)
{
	if (impl_->group_list_.size()<1)return;

	impl_->current_group_->x = ch_x; 
	impl_->current_group_->y = ch_y; 

	if (impl_->current_group_->prev_group)
	{
		//impl_->group_list_.back()->x *= impl_->current_group_->prev_group->scale_cx;
		//impl_->group_list_.back()->y *= impl_->current_group_->prev_group->scale_cy;
	
		double x1= (x-ch_x/*-x*/)* impl_->current_group_->prev_group->scale_cx;
		double y1= (y-ch_y/*-y*/)* impl_->current_group_->prev_group->scale_cy;

		double x2= impl_->current_group_->prev_group->shift_x ;
		double y2= impl_->current_group_->prev_group->shift_y ;

		impl_->current_group_->shift_x = x2 + x1  
								+(impl_->current_group_->prev_group->flipH ? (impl_->current_group_->prev_group->cx - 2 *x1) : 0) ;
		impl_->current_group_->shift_y = y2 + y1 
								+(impl_->current_group_->prev_group->flipV ? (impl_->current_group_->prev_group->cy - 2 *y1) : 0) ;
	}else
	{
		impl_->current_group_->shift_x = impl_->x + (x-ch_x) ;
		impl_->current_group_->shift_y = impl_->y + (y-ch_y);
	}

}
void odf_drawing_context::set_group_size( double cx, double cy, double ch_cx, double ch_cy)
{
	if (impl_->group_list_.size()<1)return;

	impl_->current_group_->scale_cx = cx / ch_cx;
	impl_->current_group_->scale_cy = cy / ch_cy;

	if(impl_->current_group_->prev_group)
	{
		impl_->current_group_->scale_cx *= impl_->current_group_->prev_group->scale_cx;
		impl_->current_group_->scale_cy *= impl_->current_group_->prev_group->scale_cy;
	}
	else
	{
		double first_scale_x = impl_->width/ cx;
		double first_scale_y = impl_->height/ cx;
		
		impl_->current_group_->scale_cx *= first_scale_x;
		impl_->current_group_->scale_cy *= first_scale_y;
	}

	impl_->current_group_->cx = ch_cx * impl_->current_group_->scale_cx;
	impl_->current_group_->cy = ch_cy * impl_->current_group_->scale_cy;

}
void odf_drawing_context::set_group_flip_V(bool bVal)
{
	if (impl_->group_list_.size()<1)return;

	impl_->current_group_->flipV= bVal;
}

void odf_drawing_context::set_group_flip_H(bool bVal)
{
	if (impl_->group_list_.size()<1)return;

	impl_->current_group_->flipH= bVal;
}
void odf_drawing_context::set_group_rotate(int iVal)
{
	if (impl_->group_list_.size()<1)return;

	double dRotate = (360 - iVal/60000.)/180. * 3.14159265358979323846;

	odf_group_state_ptr gr = impl_->current_group_;

	int step = 2;
	while (gr && step > 0)
	{
		dRotate += gr->rotate;
		gr = gr->prev_group;
		step--;
	}
	impl_->current_group_->rotate = dRotate;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void odf_drawing_context::set_position(double x_pt, double y_pt)
{
	if (impl_->group_list_.size() < 1)return;

	if (!impl_->current_drawing_state_.svg_x_) 
	{
		if (impl_->current_drawing_state_.in_group && impl_->current_group_)
		{
			x_pt *= impl_->current_group_->scale_cx;
			
			x_pt += impl_->current_group_->shift_x ;
				// +  (impl_->current_group_->flipH ? (impl_->current_group_->cx - 2 * x_pt): 0);
		}

		impl_->current_drawing_state_.svg_x_ = length(length(x_pt,length::pt).get_value_unit(length::cm),length::cm);
	}
	if (!impl_->current_drawing_state_.svg_y_) 
	{
		if (impl_->current_drawing_state_.in_group && impl_->current_group_)
		{
			y_pt *= impl_->current_group_->scale_cy;
			
			y_pt += impl_->current_group_->shift_y ;
				 //+  (impl_->current_group_->flipV ? (impl_->current_group_->cy - 2 * y_pt): 0);
		}

		impl_->current_drawing_state_.svg_y_ = length(length(y_pt,length::pt).get_value_unit(length::cm),length::cm);
	}
}
void odf_drawing_context::get_size( double & width_pt, double & height_pt)
{
	if (!impl_->current_drawing_state_.svg_width_ || !impl_->current_drawing_state_.svg_height_) return;

	width_pt	= impl_->current_drawing_state_.svg_width_->get_value_unit(length::pt);
	height_pt	= impl_->current_drawing_state_.svg_height_->get_value_unit(length::pt);
}
void odf_drawing_context::set_size( double width_pt, double height_pt)
{
	if (impl_->current_drawing_state_.in_group)
	{
		width_pt  *= impl_->group_list_.back()->scale_cx;
		height_pt *= impl_->group_list_.back()->scale_cy;
	}
	//if (!impl_->current_drawing_state_.svg_width_)	
	if (width_pt >= 0) impl_->current_drawing_state_.svg_width_ = length(length(width_pt,length::pt).get_value_unit(length::cm),length::cm);
	//if (!impl_->current_drawing_state_.svg_height_)
	if (height_pt >=0) impl_->current_drawing_state_.svg_height_= length(length(height_pt,length::pt).get_value_unit(length::cm),length::cm);	
}
void odf_drawing_context::set_line_width(double pt)
{
	if (!impl_->current_graphic_properties)return;
	impl_->current_graphic_properties->content().svg_stroke_width_ = length(length(pt,length::pt).get_value_unit(length::cm),length::cm);
}

void odf_drawing_context::set_line_tail(int type, int len, int width)
{
	if (!impl_->current_graphic_properties)return;

	impl_->current_graphic_properties->content().draw_marker_start_ = add_marker_style(type);

	switch(width)
	{
	case 0://lineendwidthLarge
		impl_->current_graphic_properties->content().draw_marker_end_width_ = length(0.4,length::cm); break;
	case 1://lineendwidthMedium
		impl_->current_graphic_properties->content().draw_marker_end_width_ = length(0.3,length::cm); break;
	case 2://lineendwidthSmall
		impl_->current_graphic_properties->content().draw_marker_end_width_ = length(0.2,length::cm); break;
	}
}
void odf_drawing_context::set_line_head(int type, int len, int width)
{
	if (!impl_->current_graphic_properties)return;
	
	impl_->current_graphic_properties->content().draw_marker_end_ = add_marker_style(type);

	switch(width)
	{
	case 0://lineendwidthLarge
		impl_->current_graphic_properties->content().draw_marker_start_width_ = length(0.4,length::cm); break;
	case 1://lineendwidthMedium
		impl_->current_graphic_properties->content().draw_marker_start_width_ = length(0.3,length::cm); break;
	case 2://lineendwidthSmall
		impl_->current_graphic_properties->content().draw_marker_start_width_ = length(0.2,length::cm); break;
	}
}
std::wstring odf_drawing_context::add_marker_style(int type)
{
	if (type == 2) return L"";

	std::wstring str_types [] = {L"ArrowMarker", L"DiamondMarker", L"None", L"OvalMarker", L"StealthMarker", L"TriangleMarker"};

	style * style_=NULL;
	if (impl_->styles_context_->find_odf_style(str_types[type],style_family::Marker,style_)) return str_types[type];
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
//	генерация типа маркера
	odf::office_element_ptr marker_element;

	odf::create_element(L"draw",L"marker", marker_element, impl_->odf_context_);
	impl_->styles_context_->add_style(marker_element,false,true, style_family::Marker);
	impl_->styles_context_->last_state()->set_name(str_types[type]);

	draw_marker * marker = dynamic_cast<draw_marker *>(marker_element.get());
	if (!marker) return L"";

	marker->draw_name_ = str_types[type];
	marker->draw_display_name_ = str_types[type]; ;

	switch(type)
	{
	case 1:
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
	case 0:
	default:
		marker->svg_d_ =L"M0 2108v17 17l12 42 30 34 38 21 43 4 29-8 30-21 25-26 13-34 343-1532 339 1520 13 42 29 34 39 21 42 4 42-12 34-30 21-42v-39-12l-4 4-440-1998-9-42-25-39-38-25-43-8-42 8-38 25-26 39-8 42z";
		marker->svg_viewBox_ = L"0 0 1122 2243";
	}

	return str_types[type];
}
void odf_drawing_context::set_line_dash_preset(int style)
{
	if (!impl_->current_graphic_properties)return;

	switch(style)	//+создать стиль, привзать имена
	{
		case 0://presetlinedashvalDash    
		case 7://presetlinedashvalSysDash     
			impl_->current_graphic_properties->content().draw_stroke_=line_style(line_style::Dash);		break;
		case 1://presetlinedashvalDashDot    
		case 8://presetlinedashvalSysDashDot  
			impl_->current_graphic_properties->content().draw_stroke_=line_style(line_style::DotDash);	break;
		case 2://presetlinedashvalDot       
		case 10://presetlinedashvalSysDot        
			impl_->current_graphic_properties->content().draw_stroke_=line_style(line_style::Dotted);	break;
		case 3://presetlinedashvalLgDash  
		case 4://presetlinedashvalLgDashDot  
			impl_->current_graphic_properties->content().draw_stroke_=line_style(line_style::LongDash);	break;
		case 5://presetlinedashvalLgDashDotDot 
		case 9://presetlinedashvalSysDashDotDot 
			impl_->current_graphic_properties->content().draw_stroke_=line_style(line_style::DotDotDash);break;
		case 6://presetlinedashvalSolid      
			impl_->current_graphic_properties->content().draw_stroke_=line_style(line_style::Solid);	 break;
	}
}

void odf_drawing_context::set_textarea_vertical_align(int align)
{
	if (!impl_->current_graphic_properties)return;
	switch(align)
	{
	case 0://SimpleTypes::textanchoringtypeB: 
			impl_->current_graphic_properties->content().draw_textarea_vertical_align_ = odf::vertical_align(odf::vertical_align::Bottom);	break;
	case 1://SimpleTypes::textanchoringtypeCtr: 
			impl_->current_graphic_properties->content().draw_textarea_vertical_align_ = odf::vertical_align(odf::vertical_align::Middle);	break;
	case 2://SimpleTypes::textanchoringtypeDist: 
			impl_->current_graphic_properties->content().draw_textarea_vertical_align_ = odf::vertical_align(odf::vertical_align::Baseline);break;
	case 3://SimpleTypes::textanchoringtypeJust: 
			impl_->current_graphic_properties->content().draw_textarea_vertical_align_ = odf::vertical_align(odf::vertical_align::Justify);	break;
	case 4://SimpleTypes::textanchoringtypeT: 
			impl_->current_graphic_properties->content().draw_textarea_vertical_align_ = odf::vertical_align(odf::vertical_align::Top);		break;
	}
}
void odf_drawing_context::set_textarea_wrap(bool Val)
{
	if (!impl_->current_graphic_properties)return;

	if (Val)
		impl_->current_graphic_properties->content().fo_wrap_option_ = wrap_option(wrap_option::Wrap);
	else
		impl_->current_graphic_properties->content().fo_wrap_option_ = wrap_option(wrap_option::NoWrap);

}
void odf_drawing_context::set_textarea_writing_mode(int mode)
{
	if (mode == 1) return;//незачем
	if (impl_->current_drawing_state_.elements_.size() < 1)return;

	if (!impl_->current_paragraph_properties)
	{
		style* style_ = dynamic_cast<style*>(impl_->current_drawing_state_.elements_[0].style_elm.get());
		if (style_)impl_->current_paragraph_properties = style_->style_content_.get_style_paragraph_properties();
	}
	
	style_paragraph_properties	* paragraph_properties = impl_->odf_context_->text_context()->get_paragraph_properties();
	draw_base* draw = dynamic_cast<draw_base*>(impl_->current_drawing_state_.elements_[0].elm.get());
	if (draw)
	{
		style* style_ = NULL;
		if(!draw->common_draw_attlists_.shape_with_text_and_styles_.common_draw_text_style_name_attlist_.draw_text_style_name_)
		{
			impl_->styles_context_->create_style(L"",style_family::Paragraph, true, false, -1);		
		
			office_element_ptr & style_shape_elm = impl_->styles_context_->last_state()->get_office_element();
			style_ = dynamic_cast<style*>(style_shape_elm.get());
			if (style_)
				draw->common_draw_attlists_.shape_with_text_and_styles_.common_draw_text_style_name_attlist_.draw_text_style_name_ = style_->style_name_;
		}
		else
		{
			std::wstring style_name = draw->common_draw_attlists_.shape_with_text_and_styles_.common_draw_text_style_name_attlist_.draw_text_style_name_->style_name();
			//найти
		}
		if (style_ && !paragraph_properties)
		{
			paragraph_properties = style_->style_content_.get_style_paragraph_properties();
		}
	}
	if (paragraph_properties == NULL && impl_->current_paragraph_properties == NULL)return;	
	
	switch(mode)
	{
		case 5://textverticaltypeWordArtVert:
		case 6://textverticaltypeWordArtVertRtl:
		case 4://SimpleTypes::textverticaltypeVert270: //нужно отзеркалить по горизонтали текст
		case 3://SimpleTypes::textverticaltypeVert: 
		case 2://SimpleTypes::textverticaltypeMongolianVert:
			paragraph_properties->content().style_writing_mode_ = odf::writing_mode(odf::writing_mode::TbRl);	
			impl_->current_paragraph_properties->content().style_writing_mode_ = odf::writing_mode(odf::writing_mode::TbRl);	
			break;
		case 0://SimpleTypes::textverticaltypeEaVert: 
			paragraph_properties->content().style_writing_mode_ = odf::writing_mode(odf::writing_mode::TbRl);	
			impl_->current_paragraph_properties->content().style_writing_mode_ = odf::writing_mode(odf::writing_mode::TbRl);	
			break;
		case 1://SimpleTypes::textverticaltypeHorz: 
		default:
			paragraph_properties->content().style_writing_mode_ = odf::writing_mode(odf::writing_mode::LrTb);	
			impl_->current_paragraph_properties->content().style_writing_mode_ = odf::writing_mode(odf::writing_mode::LrTb);	
			break;
	}


}

void odf_drawing_context::set_textarea_padding(double left,double top, double right,double bottom)//in cm
{
	if (!impl_->current_graphic_properties)return;

	impl_->current_graphic_properties->content().common_padding_attlist_.fo_padding_left_	= length(left,	length::cm);
	impl_->current_graphic_properties->content().common_padding_attlist_.fo_padding_top_	= length(top,	length::cm);
	impl_->current_graphic_properties->content().common_padding_attlist_.fo_padding_right_	= length(right,	length::cm);
	impl_->current_graphic_properties->content().common_padding_attlist_.fo_padding_bottom_	= length(bottom,length::cm);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//вложенные элементы
void odf_drawing_context::start_image(std::wstring & path)
{	
	start_frame();

	//добавить в стиль ссыль на базовый стиль Frame - зачемто нужно :(
	style* style_ = dynamic_cast<style*>(impl_->current_drawing_state_.elements_.back().style_elm.get());
	if (style_)
	{
		style_->style_parent_style_name_ = L"Frame";
	}
///////////////////////////////////////////////////////////////////////////////////////////////
	office_element_ptr image_elm;
	create_element(L"draw", L"image", image_elm, impl_->odf_context_);

	draw_image* image = dynamic_cast<draw_image*>(image_elm.get());
	if (image == NULL)return;

    image->common_xlink_attlist_.href_= path;
	image->common_xlink_attlist_.type_= xlink_type::Simple;
	image->common_xlink_attlist_.show_ = xlink_show::Embed;
	image->common_xlink_attlist_.actuate_= xlink_actuate::OnLoad;

	start_element(image_elm);
			
	set_image_style_repeat(1);//default
}
void odf_drawing_context::start_object(std::wstring name)
{
	start_frame();
	
	office_element_ptr object_elm;
	create_element(L"draw", L"object", object_elm, impl_->odf_context_);

	draw_object* object = dynamic_cast<draw_object*>(object_elm.get());
	if (object == NULL)return;

    object->common_xlink_attlist_.href_= std::wstring(L"./") + name;
	object->common_xlink_attlist_.type_= xlink_type::Simple;
	object->common_xlink_attlist_.show_ = xlink_show::Embed;
	object->common_xlink_attlist_.actuate_= xlink_actuate::OnLoad;

	start_element(object_elm);
}

void odf_drawing_context::start_text_box()
{	
	start_frame();

	//добавить в стиль ссыль на базовый стиль Frame - зачемто нужно :(
	style* style_ = dynamic_cast<style*>(impl_->current_drawing_state_.elements_.back().style_elm.get());
	if (style_)
	{
		style_->style_parent_style_name_ = L"Frame";
	}

	if (impl_->current_graphic_properties)
	{

	}

	office_element_ptr text_box_elm;
	create_element(L"draw", L"text-box", text_box_elm, impl_->odf_context_);

	start_element(text_box_elm);
}
void odf_drawing_context::set_text_box_min_size(double w_pt, double h_pt)
{
	draw_text_box* draw = dynamic_cast<draw_text_box*>(impl_->current_drawing_state_.elements_.back().elm.get());

	if (draw)
	{
		if (h_pt >=0) draw->draw_text_box_attlist_.fo_min_height_= length(length(h_pt,length::pt).get_value_unit(length::cm), length::cm);
		if (w_pt >=0) draw->draw_text_box_attlist_.fo_min_width_= length(length(w_pt,length::pt).get_value_unit(length::cm), length::cm);

	}
}
void odf_drawing_context::end_image()
{
	end_element();

	end_frame();
}
void odf_drawing_context::end_text_box()
{
	end_element();

	end_frame();
}
void odf_drawing_context::end_object()
{
	end_element();

	end_frame();
}

bool odf_drawing_context::is_exist_content()
{
	return (impl_->drawing_list_.size()>0 ? true : false);
}

void odf_drawing_context::finalize(office_element_ptr & root_elm)//для привязки 
{
	for (int i=0; i< impl_->group_list_.size(); i++)
	{
		if (impl_->group_list_[i]->level ==0 )
		{
			root_elm->add_child_element(impl_->group_list_[i]->elm);
		}
	}	
	for (int i=0; i< impl_->drawing_list_.size(); i++)
	{
		if (impl_->drawing_list_[i].elements_.size() > 0 && impl_->drawing_list_[i].elements_[0].level == 0 )
		{
			root_elm->add_child_element(impl_->drawing_list_[i].elements_[0].elm);
		}
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
	if (text_context == NULL || impl_->current_level_.size() <1 )return;

	for (long i=0; i< text_context->text_elements_list_.size(); i++)
	{
		if (text_context->text_elements_list_[i].level ==0)
		{
			impl_->current_level_.back()->add_child_element(text_context->text_elements_list_[i].elm);
		}
	}

	if (impl_->current_graphic_properties)
	{
		//автоувеличение при добавлении текста
		impl_->current_graphic_properties->content().draw_auto_grow_height_ = false;
		impl_->current_graphic_properties->content().draw_auto_grow_width_ = false;
		impl_->current_graphic_properties->content().draw_fit_to_size_ = false;//???

	}
}
void odf_drawing_context::start_gradient_style()
{
	odf::office_element_ptr gradient_element;

	odf::create_element(L"draw",L"gradient", gradient_element, impl_->odf_context_);
	impl_->styles_context_->add_style(gradient_element,false,true, style_family::Gradient);

	draw_gradient * gradient = dynamic_cast<draw_gradient *>(gradient_element.get());
	if (!gradient) return;

	gradient->draw_name_ = impl_->styles_context_->find_free_name(style_family::Gradient);
	gradient->draw_display_name_ = std::wstring(L"User") + gradient->draw_name_.get() ;
	
	impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_gradient_name_ = gradient->draw_name_;
	impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_ = draw_fill(draw_fill::gradient);

}
void odf_drawing_context::end_gradient_style()
{
}
void odf_drawing_context::set_gradient_type(int type)
{
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state()->get_office_element().get());
	if (!gradient) return;

	switch(type)
	{
	case 1:	gradient->draw_style_ = gradient_style(gradient_style::linear);		break;
	case 2: gradient->draw_style_ = gradient_style(gradient_style::radial);		break;
	case 3: gradient->draw_style_ = gradient_style(gradient_style::rectangular); break;
	case 4: gradient->draw_style_ = gradient_style(gradient_style::square);		break;
	}	
}
void odf_drawing_context::set_gradient_start(std::wstring hexColor, _CP_OPT(double) & intensiv)
{
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state()->get_office_element().get());
	if (!gradient) return;

	gradient->draw_start_color_ = color(std::wstring(L"#") + hexColor);
	gradient->draw_start_intensity_ = 100.;
}
void odf_drawing_context::set_gradient_end  (std::wstring hexColor, _CP_OPT(double) & intensiv)
{
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state()->get_office_element().get());
	if (!gradient) return;

	gradient->draw_end_color_ = color(std::wstring(L"#") + hexColor);
	gradient->draw_end_intensity_ = 100.;
}
void odf_drawing_context::set_gradient_center(double x, double y)
{
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state()->get_office_element().get());
	if (!gradient) return;
	
	gradient->draw_cy_ = percent(x);
	gradient->draw_cx_ = percent(y);
}
void odf_drawing_context::set_gradient_rect(double l, double t, double r,double b)
{
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state()->get_office_element().get());
	if (!gradient) return;
	
	gradient->draw_cy_ = percent((b-t)/2. + 50.);
	gradient->draw_cx_ = percent((r-l)/2. + 50.);
}
void odf_drawing_context::set_opacity(_CP_OPT(double) & start, _CP_OPT(double) & end)
{
	if (!start && !end)return;

	if (!start) start = 100;
	if (!end)	end = 100;

	if (*start == *end)
	{
		set_opacity(*start);
	}
	else
	{
		odf::office_element_ptr opacity_element;

		odf::create_element(L"draw",L"opacity", opacity_element, impl_->odf_context_);
		impl_->styles_context_->add_style(opacity_element,false,true, style_family::Opacity);

		draw_opacity * opacity = dynamic_cast<draw_opacity *>(opacity_element.get());
		if (!opacity) return;

		opacity->draw_name_ = impl_->styles_context_->find_free_name(style_family::Opacity);
		opacity->draw_display_name_ = std::wstring(L"User") + opacity->draw_name_.get() ;
		
		impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_opacity_name_ = opacity->draw_name_;
///////////////////////////////////////////////////////////////////////////////////////////////////
		opacity->draw_start_ = *start;
		opacity->draw_end_ = *end;

		draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state()->get_office_element().get());
		if (gradient)
		{
			opacity->draw_style_ =  gradient->draw_style_;
		}
		else
		{
			opacity->draw_style_ = gradient_style(gradient_style::linear);
		}
	}
}
void odf_drawing_context::set_gradient_angle(double angle)
{
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state()->get_office_element().get());
	if (!gradient) return;

	gradient->draw_angle_ = (int)((360 - angle)/180. * 3.14159265358979323846);
}
void odf_drawing_context::start_hatch_style()
{
	odf::office_element_ptr hatch_element;

	odf::create_element(L"draw",L"hatch", hatch_element, impl_->odf_context_);
	impl_->styles_context_->add_style(hatch_element,false,true, style_family::Hatch);

	draw_hatch * hatch = dynamic_cast<draw_hatch *>(hatch_element.get());
	if (!hatch) return;

	hatch->draw_name_ = impl_->styles_context_->find_free_name(style_family::Hatch);
	hatch->draw_display_name_ = std::wstring(L"User") + hatch->draw_name_.get() ;
	
	impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_hatch_name_ = hatch->draw_name_;
	impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_ = draw_fill(draw_fill::hatch);
}
void odf_drawing_context::set_hatch_line_color(std::wstring hexColor)
{
	if (!impl_->current_graphic_properties)return;

	//impl_->current_graphic_properties->content().svg_stroke_color_ = color(std::wstring(L"#") + hexColor);
	//impl_->current_graphic_properties->content().draw_stroke_=line_style(line_style::Solid);//default

	draw_hatch * hatch = dynamic_cast<draw_hatch *>(impl_->styles_context_->last_state()->get_office_element().get());
	if (!hatch) return;

	hatch->draw_color_ = color(std::wstring(L"#") + hexColor);

}
void odf_drawing_context::set_hatch_area_color(std::wstring hexColor)
{
	if (!impl_->current_graphic_properties)return;

	impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_color_ = color(std::wstring(L"#") + hexColor);
	impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_hatch_solid_ = true;
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
	odf::office_element_ptr fill_image_element;

	odf::create_element(L"draw",L"fill-image", fill_image_element, impl_->odf_context_);
	impl_->styles_context_->add_style(fill_image_element,false,true, style_family::FillImage);

	draw_fill_image * fill_image = dynamic_cast<draw_fill_image *>(fill_image_element.get());
	if (!fill_image) return;

	fill_image->draw_name_ = impl_->styles_context_->find_free_name(style_family::FillImage);
	fill_image->draw_display_name_ = std::wstring(L"User") + fill_image->draw_name_.get() ;
	
	impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_image_name_ = fill_image->draw_name_;
	impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_ = draw_fill(draw_fill::bitmap);
	
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
			impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_image_ref_point_ = fill_image_ref_point(fill_image_ref_point::bottom);break;
		case 1: //	rectalignmentBL  = 1,
			impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_image_ref_point_ = fill_image_ref_point(fill_image_ref_point::bottom_left);break;
		case 2: //	rectalignmentBR  = 2,
			impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_image_ref_point_ = fill_image_ref_point(fill_image_ref_point::bottom_right);break;
		case 3: //	rectalignmentCtr = 3,
			impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_image_ref_point_ = fill_image_ref_point(fill_image_ref_point::center);break;
		case 4: //	rectalignmentL   = 4,
			impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_image_ref_point_ = fill_image_ref_point(fill_image_ref_point::left);break;
		case 5: //	rectalignmentR   = 5,
			impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_image_ref_point_ = fill_image_ref_point(fill_image_ref_point::right);break;
		case 6: //	rectalignmentT   = 6,
			impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_image_ref_point_ = fill_image_ref_point(fill_image_ref_point::top);break;
		case 7: //	rectalignmentTL  = 7,
			impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_image_ref_point_ = fill_image_ref_point(fill_image_ref_point::top_left);break;
		case 8: //	rectalignmentTR  = 8,
			impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_image_ref_point_ = fill_image_ref_point(fill_image_ref_point::top_right);break;
	}
}
void odf_drawing_context::set_image_style_repeat(int style)
{
	if (!impl_->current_graphic_properties)return;

	if (style == 1)
		impl_->current_graphic_properties->content().common_draw_fill_attlist_.style_repeat_ = style_repeat(style_repeat::Stretch);
	else if (style ==2)
		impl_->current_graphic_properties->content().common_draw_fill_attlist_.style_repeat_ = style_repeat(style_repeat::Repeat);
	else
		impl_->current_graphic_properties->content().common_draw_fill_attlist_.style_repeat_ = style_repeat(style_repeat::NoRepeat);//default
}
void odf_drawing_context::set_image_client_rect(double l_pt, double t_pt, double r_pt, double b_pt)
{
	if (!impl_->current_graphic_properties)return;
	
	//<top>, <right>, <bottom>, <left> 
	std::wstringstream str_stream;
	str_stream << std::wstring(L"rect(") 
							<< length(length(t_pt,length::pt).get_value_unit(length::cm),length::cm) << std::wstring(L",")
							<< length(length(r_pt,length::pt).get_value_unit(length::cm),length::cm) << std::wstring(L",")
							<< length(length(b_pt,length::pt).get_value_unit(length::cm),length::cm) << std::wstring(L",")
							<< length(length(l_pt,length::pt).get_value_unit(length::cm),length::cm) << std::wstring(L")");

	impl_->current_graphic_properties->content().fo_clip_ = str_stream.str();

}
void odf_drawing_context::set_bitmap_link(std::wstring link)
{
	std::wstring odf_ref_name ;
	
	impl_->odf_context_->mediaitems()->add_or_find(link,_mediaitems::typeImage,odf_ref_name);
	
	draw_fill_image * fill_image = dynamic_cast<draw_fill_image *>(impl_->styles_context_->last_state()->get_office_element().get());
	if (!fill_image) return;
   
	fill_image->xlink_attlist_.href_= odf_ref_name;
	//fill_image->xlink_attlist_.type_= xlink_type::Simple;
	fill_image->xlink_attlist_.show_ = xlink_show::Embed;
	fill_image->xlink_attlist_.actuate_= xlink_actuate::OnLoad;
}
void odf_drawing_context::set_bitmap_tile_scale_x(double scale_x)
{
	if (!impl_->current_graphic_properties)return;
	
	impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_image_width_ = 
				length(length(scale_x,length::pt).get_value_unit(length::cm),length::cm);

}
void odf_drawing_context::set_bitmap_tile_scale_y(double scale_y)
{
	if (!impl_->current_graphic_properties)return;
	
	impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_image_height_ = 
				length(length(scale_y,length::pt).get_value_unit(length::cm),length::cm);

}
void odf_drawing_context::set_bitmap_tile_translate_y(double y)
{
	if (!impl_->current_graphic_properties)return;
	impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_image_ref_point_y_ = percent(y);

}
void odf_drawing_context::set_bitmap_tile_translate_x(double x)
{
	if (!impl_->current_graphic_properties)return;
	impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_image_ref_point_x_ = percent(x);
}

office_element_ptr & odf_drawing_context::get_root_element()
{
	return impl_->root_element_;
}
///////////////////
}
}