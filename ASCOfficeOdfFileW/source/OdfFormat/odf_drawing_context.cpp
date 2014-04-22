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
#include "shape_types_mapping.h"


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
		flipH = false;
		flipV = false;	
		tile = false;
		
		path_ = L"";
		view_box_ = L"";
		path_last_command_ = L"";
		modifiers_ = L"";

		fill_ = true; //потом по нормальному .. с типами

		oox_shape_preset = -1;

	}
	std::vector<odf_element_state> elements_;

	_CP_OPT(length) svg_x_;
	_CP_OPT(length) svg_y_;
	_CP_OPT(length) svg_height_;
	_CP_OPT(length) svg_width_;	

	std::wstring name_;
	int z_order_;

	bool flipH;
	bool flipV;
	bool tile;
	_CP_OPT(double) rotateAngle;

	std::wstring path_;
	std::wstring view_box_;
	std::wstring path_last_command_;
	std::wstring modifiers_;

	bool fill_;
	int oox_shape_preset;

};

class odf_drawing_context::Impl
{
public:
	Impl(odf_conversion_context *odf_context) :odf_context_(odf_context)
    {	
		current_drawing_state_.clear();
		styles_context_ = &odf_context_->styles_context();
	} 
	std::vector<odf_drawing_state> drawing_list_;//все элементы .. для удобства разделение по "топам"
	
	odf_drawing_state current_drawing_state_;
	
	std::vector<office_element_ptr> current_level_;//постоянно меняющийся список уровней наследования

	odf_style_context * styles_context_;
	odf_conversion_context *odf_context_;

	void create_draw_base(int type);
	office_element_ptr create_draw_element(int type);

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
	style* style_ = dynamic_cast<style*>(impl_->current_drawing_state_.elements_[0].style_elm.get());
	if (style_)
	{
		style_graphic_properties * gr_properties = style_->style_content_.get_style_graphic_properties();
		if (gr_properties)
		{		
			if (impl_->current_drawing_state_.flipH)
				gr_properties->content().style_mirror_ = std::wstring(L"horizontal");
			if (impl_->current_drawing_state_.flipV)
				gr_properties->content().style_mirror_ = std::wstring(L"vertical");

			if (impl_->current_drawing_state_.fill_ == false)
				gr_properties->content().common_draw_fill_attlist_.draw_fill_ = draw_fill::none;

			gr_properties->content().draw_fit_to_size_ = false;
			//draw:fit-to-size="false"
			//fo:clip
			//draw:image-opacity
		}
	}

///////////////////////////////////////////////////////////////////
	impl_->drawing_list_.push_back(impl_->current_drawing_state_);
	impl_->current_drawing_state_.clear();
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
	else
	{
		impl_->create_draw_base(7);//пока кастом .. потом переделать на path, что правильнее
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
			}
			else
			{
				oox_shape_ptr shape_define = oox_shape::create(impl_->current_drawing_state_.oox_shape_preset);

				if (shape_define)
				{		
					enhanced->draw_enhanced_geometry_attlist_.draw_type_		= L"non-primitive";
					enhanced->svg_viewbox_										= shape_define->view_box;
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
void odf_drawing_context::set_name(std::wstring  name)
{
	impl_->current_drawing_state_.name_ = name;
}
void odf_drawing_context::set_no_fill()
{
	impl_->current_drawing_state_.fill_ = false;
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
	impl_->current_drawing_state_.flipH= true;
}
void odf_drawing_context::set_flip_V(bool bVal)
{
	impl_->current_drawing_state_.flipV= true;
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//вложенные элементы
void odf_drawing_context::start_image(std::wstring & path)
{
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
void odf_drawing_context::end_image()
{
	end_element();
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


}
}