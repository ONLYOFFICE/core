#pragma once

#include <string>
#include <vector>

#include <cpdoccore/CPOptional.h>

#include "office_elements.h"
#include "office_elements_create.h"

#include "anchortype.h"
#include "stylewrap.h"

namespace cpdoccore {
namespace odf
{

class odf_conversion_context;
class odf_style_context;
class odf_text_context;

class odf_drawing_context
{
public:
	odf_drawing_context(odf_conversion_context *odf_context);
    ~odf_drawing_context();

	void set_drawings_rect(double x_pt, double y_pt, double width_pt, double height_pt);
	void clear();
	void set_styles_context(odf_style_context*  styles_context);//для embedded 

	void set_margin_left	(double valPt);
	void set_margin_right	(double valPt);
	void set_margin_top		(double valPt);
	void set_margin_bottom	(double valPt);

	void set_anchor	(int type);//anchor_type

	void set_vertical_rel(int from);
	void set_vertical_pos(int align);
	void set_vertical_pos(double offset_pt);

	void set_horizontal_rel(int from);
	void set_horizontal_pos(int align);
	void set_horizontal_pos(double offset_pt);

	void set_wrap_style(style_wrap::type  style);
	void set_overlap (bool val);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	office_element_ptr & get_current_style_element() ;
	office_element_ptr & get_current_element();

	void start_drawing();
	void end_drawing();
	
	void start_group(std::wstring name, int id);
		//void set_group_size_koef( double cx, double cy);
		//void set_group_position_delta(double x_pt, double y_pt);
		//void set_group_child_position	(double x_pt,		double y_pt);
		
		void set_group_flip_H(bool bVal);
		void set_group_flip_V(bool bVal);

		void set_group_rotate(int iVal);
		void set_group_size( double cx, double cy, double ch_cx, double ch_cy);
		void set_group_position(double x, double y, double ch_x, double ch_y);
	void end_group();

	void start_shape(int type);
	void end_shape();

	void start_image(std::wstring & path);
	void end_image();
		
	void start_text_box();
		void set_text_box_min_size(double w_pt, double h_pt);
	void end_text_box();

	void start_object(std::wstring name);
	void end_object();

	office_element_ptr & get_root_element();

	void start_element(office_element_ptr & elm, office_element_ptr  style_elm = office_element_ptr());
    void end_element();

	bool is_exist_content();
//////////////////////////////////////////////////////////////////////////////////////
	void add_path_element(std::wstring command, std::wstring & elm);
	void add_modifier(std::wstring modifier);
	
	void set_viewBox(double W, double H);

	void finalize(office_element_ptr & root_elm);

///////////////////////////////////////////////////////////////////////////////////////

	void set_size( double width_pt, double height_pt);
	void set_position(double x_pt, double y_pt);
	
	void get_size( double & width_pt, double & height_pt);
	
	void set_name(std::wstring  name);
	void set_z_order(int id);
	
	void set_flip_H(bool bVal);
	void set_flip_V(bool bVal);

	void set_rotate(int iVal);

	void set_no_fill();
	void set_solid_fill(std::wstring hexColor);
	void set_opacity(double percent);
//////////////////////////////////////////////////////////////
	void start_area_properies();
	void end_area_properies();
	
	void start_line_properies();
		void set_line_width(double pt);
		void set_line_dash_preset(int style);
		
		void set_line_head(int type, int len, int width);
		void set_line_tail(int type, int len, int width);

		std::wstring add_marker_style(int type);
	void end_line_properies();
	
	//void start_shadow_properies();
	//void end_shadow_properies();
// пока одной функией ..
	void set_shadow(int type, std::wstring hexColor, _CP_OPT(double) opacity, double dist_pt, double dist_pt_y = -1);

	void set_text(odf_text_context* text_context);
	void set_textarea_vertical_align(int align);
	void set_textarea_padding(double left,double top,double right,double bottom);//in cm
	void set_textarea_writing_mode(int mode);
	void set_textarea_wrap(bool val);
//////////////////////////////////////////////////////////////////////////////////////
	void start_gradient_style();
		void set_gradient_type(int type);
		void set_gradient_start(std::wstring hexColor, _CP_OPT(double) & intensiv);
		void set_gradient_end  (std::wstring hexColor, _CP_OPT(double) & intensiv);
		void set_gradient_rect(double l, double t, double r,double b);
		void set_gradient_center(double x, double y);
		void set_opacity(_CP_OPT(double) & start, _CP_OPT(double) & end);//стиль
		void set_gradient_angle(double angle);
	void end_gradient_style();
////////////////////////////////////////////////////////////////////////////////////////
	void start_hatch_style();
		void set_hatch_line_color(std::wstring hexColor);
		void set_hatch_area_color(std::wstring hexColor);
		void set_hatch_type(int type);
	void end_hatch_style();
////////////////////////////////////////////////////////////////////////////////////////
	void start_bitmap_style();
		void set_bitmap_link(std::wstring link);
	void end_bitmap_style();
//////////////////////////////////////////////////////////////////////////////////////
	void set_image_style_repeat(int style);
	void set_image_client_rect(double l_pt, double t_pt, double r_pt, double b_pt);
	
	void set_bitmap_tile_align(int align);
	void set_bitmap_tile_scale_x(double scale_x);
	void set_bitmap_tile_scale_y(double scale_y);
	void set_bitmap_tile_translate_y(double y);
	void set_bitmap_tile_translate_x(double x);

private:
	
	void start_frame();
	void end_frame();

    class Impl;
    _CP_PTR(Impl) impl_;

};

}
}