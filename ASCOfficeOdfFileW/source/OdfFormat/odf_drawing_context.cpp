#include "precompiled_cpodf.h"
#include "logging.h"

#include <boost/foreach.hpp>
#include <iostream>

#include "odf_drawing_context.h"
#include "odf_text_context.h"
#include "odf_style_context.h"
#include "odf_conversion_context.h"


namespace cpdoccore {
namespace odf
{

odf_drawing_context::odf_drawing_context(odf_style_context * styles_context,odf_conversion_context *odf_context)
{
	styles_context_ = styles_context;
	odf_context_ = odf_context;

}
odf_drawing_context::~odf_drawing_context()
{
}
void odf_drawing_context::set_styles_context(odf_style_context*  styles_context)
{
	styles_context_ = styles_context;
}
void odf_drawing_context::start_drawing()//frame???
{
	office_element_ptr frame_elm;
	create_element(L"draw", L"frame", frame_elm, odf_context_);

	draw_frame* frame = dynamic_cast<draw_frame*>(frame_elm.get());
	if (frame == NULL)return;
//////////////////////////////////////////////////////////////////////////////////////////////
	styles_context_->create_style(L"",style_family::Graphic, true, false, -1);		
	
	office_element_ptr & style_frame_elm = styles_context_.last_state().get_office_element();
	std::wstring style_name;

	style* style = dynamic_cast<style*>(style_frame_elm.get());
	if (style)
	{
		style_name = style->style_name_;
		odf::style_table_graphic_properties * gr_properties = style->style_content_.get_style_graphic_properties()
		if (gr_properties)
		{		
		}
	}
/////////////////////////////////////////////////////////////////////////////////////////////
	frame->draw_style_name_ = style_name;

/////////////////////////////////////////////////////////////////////////////////////////////
	int level = current_level_.size();

	odf_drawing_state state={frame_elm, style_name, style_frame_elm, level};

	drawing_elements_list_.push_back(state);
	if (current_level_.size()>0)
		current_level_.back()->add_child_element(elm);

	current_level_.push_back(elm);
}
void odf_drawing_context::end_drawing()
{
	end_element();
}

void odf_drawing_context::start_image(std::wstring & path)
{
	office_element_ptr image_elm;
	create_element(L"draw", L"image", image_elm, odf_context_);

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
void odf_drawing_context::start_element(office_element_ptr & elm)
{
	int level = current_level_.size();

	odf_drawing_state state={elm, L"", office_element_ptr(), level};

	drawing_elements_list_.push_back(state);
	if (current_level_.size()>0)
		current_level_.back()->add_child_element(elm);

	current_level_.push_back(elm);
}
void odf_drawing_context::end_element()
{
	current_level_.pop_back();
}
}
}