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

struct 	odf_element_state
{
	office_element_ptr	elm;
	std::wstring		style_name;

	office_element_ptr	style_elm;
	
	int level;
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
		tile = false;
		
		path_ = L"";
		view_box_ = L"";
		path_last_command_ = L"";
		modifiers_ = L"";

		oox_shape_preset = -1;
	}
	std::vector<odf_element_state> elements_;

	_CP_OPT(length) svg_x_;
	_CP_OPT(length) svg_y_;
	_CP_OPT(length) svg_height_;
	_CP_OPT(length) svg_width_;	

	std::wstring name_;
	int z_order_;

	bool tile;
	_CP_OPT(double) rotateAngle;

	std::wstring path_;
	std::wstring view_box_;
	std::wstring path_last_command_;
	std::wstring modifiers_;


///////////////////////
	int oox_shape_preset;

};

class odf_drawing_context::Impl
{
public:
	Impl(odf_conversion_context *odf_context) :odf_context_(odf_context)
    {	
		current_drawing_state_.clear();
		styles_context_ = &odf_context_->styles_context();
		
		current_graphic_properties = NULL;
	} 
	std::vector<odf_drawing_state> drawing_list_;//все элементы .. для удобства разделение по "топам"
	
	odf_drawing_state current_drawing_state_;
	_drawing_part current_drawing_part_;
	
	std::vector<office_element_ptr> current_level_;//постоянно меняющийся список уровней наследования

	odf_style_context			*styles_context_;
	odf_conversion_context		*odf_context_;

	void create_draw_base(int type);
	office_element_ptr create_draw_element(int type);

	style_graphic_properties		*current_graphic_properties;
	style_paragraph_properties		*current_paragraph_properties;

};

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

void odf_drawing_context::start_drawing()
{
	impl_->current_drawing_state_.clear();
	//text_context_.clear();
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
		if (impl_->current_drawing_state_.rotateAngle)
		{
			strTransform = std::wstring(L"rotate(") + boost::lexical_cast<std::wstring>(impl_->current_drawing_state_.rotateAngle.get()) + std::wstring(L")");
			//так как вращения все в мс относительно центра фигуры, а не от начала координат - убираем смещение
			if (impl_->current_drawing_state_.svg_x_ && impl_->current_drawing_state_.svg_y_)
			{
				strTransform += std::wstring(L" translate(") +	boost::lexical_cast<std::wstring>(impl_->current_drawing_state_.svg_x_.get() +
																	(impl_->current_drawing_state_.svg_width_.get()/2))+ std::wstring(L",") + 
																boost::lexical_cast<std::wstring>(impl_->current_drawing_state_.svg_y_.get() +
																	(impl_->current_drawing_state_.svg_height_.get()/2))+ std::wstring(L")") ; 
			}

			impl_->current_drawing_state_.svg_x_ = boost::none;
			impl_->current_drawing_state_.svg_y_ = boost::none;
		}
		if (strTransform.length()>0)
			draw->common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_draw_transform_attlist_.draw_transform_ = strTransform;

		draw->common_draw_attlists_.position_.svg_x_ = impl_->current_drawing_state_.svg_x_;
		draw->common_draw_attlists_.position_.svg_y_ = impl_->current_drawing_state_.svg_y_;
		draw->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_ = impl_->current_drawing_state_.svg_height_;
		draw->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_ = impl_->current_drawing_state_.svg_width_;
	}
///////////////////////////////////////////////////		
	impl_->drawing_list_.push_back(impl_->current_drawing_state_);
	
///////////////
	impl_->current_drawing_state_.clear();
	impl_->current_graphic_properties = NULL;
	impl_->current_paragraph_properties = NULL;
}

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
	}

	return element;
}
void odf_drawing_context::Impl::create_draw_base(int type)
{	
	office_element_ptr & draw_elm = create_draw_element(type);

	draw_base* draw = dynamic_cast<draw_base*>(draw_elm.get());
	if (draw == NULL)return;
//////////	
	styles_context_->create_style(L"",style_family::Graphic, true, false, -1);		
	
	office_element_ptr & style_shape_elm = styles_context_->last_state().get_office_element();
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

////////////////////////////////////////////////////////////////////////////////////////////
	draw_line* line = dynamic_cast<draw_line*>(impl_->current_level_.back().get());
	if (line)
	{
		line->draw_line_attlist_.svg_x1_ = impl_->current_drawing_state_.svg_x_;
		line->draw_line_attlist_.svg_y1_ = impl_->current_drawing_state_.svg_y_;
		line->draw_line_attlist_.svg_x2_ = impl_->current_drawing_state_.svg_x_.get() + impl_->current_drawing_state_.svg_width_.get();
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
void odf_drawing_context::start_element(office_element_ptr & elm)
{
	int level = impl_->current_level_.size();

	odf_element_state state={elm, L"", office_element_ptr(), level};

	impl_->current_drawing_state_.elements_.push_back(state);
	
	if (impl_->current_level_.size()>0)
		impl_->current_level_.back()->add_child_element(elm);

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

void odf_drawing_context::start_shadow_properies()
{
	impl_->current_drawing_part_ = Shadow;
}
void odf_drawing_context::end_shadow_properies()
{
	impl_->current_drawing_part_ = Unknown;
}
////////////////////////////////////////////////////////////////////
void odf_drawing_context::set_name(std::wstring  name)
{
	impl_->current_drawing_state_.name_ = name;
}
void odf_drawing_context::set_opacity(double percent_)
{
	if (!impl_->current_graphic_properties)return;

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
void odf_drawing_context::set_no_fill()
{
	if (!impl_->current_graphic_properties)return;

	switch(impl_->current_drawing_part_)
	{
	case Area:
		impl_->current_graphic_properties->content().common_draw_fill_attlist_.draw_fill_ = draw_fill::none;
		break;
	case Line:
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
		break;
	case Line:
		impl_->current_graphic_properties->content().svg_stroke_color_ = color(std::wstring(L"#") + hexColor);
		impl_->current_graphic_properties->content().draw_stroke_=line_style(line_style::Solid);//default
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
	if (!impl_->current_graphic_properties)return;
	impl_->current_graphic_properties->content().style_mirror_ = std::wstring(L"horizontal");
}
void odf_drawing_context::set_flip_V(bool bVal)
{
	if (!impl_->current_graphic_properties)return;
	impl_->current_graphic_properties->content().style_mirror_ = std::wstring(L"vertical");
}
void odf_drawing_context::set_tile(bool bVal)
{
	impl_->current_drawing_state_.tile = true;
}
void odf_drawing_context::set_rotate(int iVal)
{
	double dRotate = iVal/60000.;
	impl_->current_drawing_state_.rotateAngle = (360 - dRotate)/180. * 3.14159265358979323846;
}
void odf_drawing_context::set_rect(double x_pt, double y_pt, double width_pt, double height_pt)
{
	//хороший тон сохранить все размеры в см (хотя можно и в другой системе)
	impl_->current_drawing_state_.svg_x_ = length(length(x_pt,length::pt).get_value_unit(length::cm),length::cm);
	impl_->current_drawing_state_.svg_y_ = length(length(y_pt,length::pt).get_value_unit(length::cm),length::cm);

	impl_->current_drawing_state_.svg_height_ = length(length(height_pt,length::pt).get_value_unit(length::cm),length::cm);	
	impl_->current_drawing_state_.svg_width_ = length(length(width_pt,length::pt).get_value_unit(length::cm),length::cm);	
}
void odf_drawing_context::set_line_width(double pt)
{
	if (!impl_->current_graphic_properties)return;
	impl_->current_graphic_properties->content().svg_stroke_width_ = length(length(pt,length::pt).get_value_unit(length::cm),length::cm);
}

void odf_drawing_context::set_line_head(int type, int len, int width)
{
	if (!impl_->current_graphic_properties)return;
	//создать стиль, привзать имена
	
	//impl_->current_graphic_properties->content().draw_marker_start_ = marker_style(marker_style::from_ms(type));
}
void odf_drawing_context::set_line_tail(int type, int len, int width)
{
	if (!impl_->current_graphic_properties)return;

	//создать стиль, привзать имена

	//impl_->current_graphic_properties->content().draw_marker_end_ = marker_style(marker_style::from_ms(type));
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

void odf_drawing_context::set_textarea_writing_mode(int mode)
{
	if (mode == 1) return;//незачем
	if (impl_->current_drawing_state_.elements_.size() < 1)return;

	if (!impl_->current_paragraph_properties)
	{
		style* style_ = dynamic_cast<style*>(impl_->current_drawing_state_.elements_[0].style_elm.get());
		if (style_)impl_->current_paragraph_properties = style_->style_content_.get_style_paragraph_properties();
	}
	
	style_paragraph_properties	* paragraph_properties=NULL;
	draw_base* draw = dynamic_cast<draw_base*>(impl_->current_drawing_state_.elements_[0].elm.get());
	if (draw)
	{
		style* style_ = NULL;
		if(!draw->common_draw_attlists_.shape_with_text_and_styles_.common_draw_text_style_name_attlist_.draw_text_style_name_)
		{
			impl_->styles_context_->create_style(L"",style_family::Paragraph, true, false, -1);		
		
			office_element_ptr & style_shape_elm = impl_->styles_context_->last_state().get_office_element();
			style_ = dynamic_cast<style*>(style_shape_elm.get());
			if (style_)
				draw->common_draw_attlists_.shape_with_text_and_styles_.common_draw_text_style_name_attlist_.draw_text_style_name_ = style_->style_name_;
		}
		else
		{
			std::wstring style_name = draw->common_draw_attlists_.shape_with_text_and_styles_.common_draw_text_style_name_attlist_.draw_text_style_name_->style_name();
			//найти
		}
		if (style_)
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

	office_element_ptr image_elm;
	create_element(L"draw", L"image", image_elm, impl_->odf_context_);

	draw_image* image = dynamic_cast<draw_image*>(image_elm.get());
	if (image == NULL)return;

    image->common_xlink_attlist_.href_= path;
	image->common_xlink_attlist_.type_= xlink_type::Simple;
	image->common_xlink_attlist_.show_ = xlink_show::Embed;
	image->common_xlink_attlist_.actuate_= xlink_actuate::OnLoad;

	start_element(image_elm);
}
void odf_drawing_context::start_text_box()
{	
	start_frame();

	if (impl_->current_graphic_properties)
	{

	}

	office_element_ptr text_box_elm;
	create_element(L"draw", L"text-box", text_box_elm, impl_->odf_context_);

	start_element(text_box_elm);
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

bool odf_drawing_context::is_exist_content()
{
	return (impl_->drawing_list_.size()>0 ? true : false);
}

void odf_drawing_context::finalize(office_element_ptr & root_elm)//для привязки 
{
	for (int i=0; i< impl_->drawing_list_.size(); i++)
	{
		if (impl_->drawing_list_[i].elements_.size() > 0)
		{
			root_elm->add_child_element(impl_->drawing_list_[i].elements_[0].elm);
		}
	}
}
office_element_ptr & odf_drawing_context::get_current_style_element()
{
	if (impl_->current_drawing_state_.elements_.size() > 0)
		return impl_->current_drawing_state_.elements_.back().style_elm;
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
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state().get_office_element().get());
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
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state().get_office_element().get());
	if (!gradient) return;

	gradient->draw_start_color_ = color(std::wstring(L"#") + hexColor);
	gradient->draw_start_intensity_ = 100.;
}
void odf_drawing_context::set_gradient_end  (std::wstring hexColor, _CP_OPT(double) & intensiv)
{
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state().get_office_element().get());
	if (!gradient) return;

	gradient->draw_end_color_ = color(std::wstring(L"#") + hexColor);
	gradient->draw_end_intensity_ = 100.;
}
void odf_drawing_context::set_gradient_center(double x, double y)
{
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state().get_office_element().get());
	if (!gradient) return;
	
	gradient->draw_cy_ = percent(x);
	gradient->draw_cx_ = percent(y);
}
void odf_drawing_context::set_gradient_rect(double l, double t, double r,double b)
{
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state().get_office_element().get());
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

		draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state().get_office_element().get());
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
	draw_gradient * gradient = dynamic_cast<draw_gradient *>(impl_->styles_context_->last_state().get_office_element().get());
	if (!gradient) return;

	gradient->draw_angle_ = (360 - angle)/180. * 3.14159265358979323846;
}

}
}