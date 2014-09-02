#pragma once

#include <string>
#include <vector>

#include <cpdoccore/CPOptional.h>

#include "office_elements.h"
#include "office_elements_create.h"

#include "anchortype.h"
#include "stylewrap.h"
#include "gradientstyle.h"

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

	void set_drawings_rect(_CP_OPT(double) x_pt, _CP_OPT(double) y_pt, _CP_OPT(double) width_pt, _CP_OPT(double) height_pt);
	void clear();
	void set_styles_context(odf_style_context*  styles_context);//для embedded 

	void set_footer_header_state(bool Val);

	void set_margin_left	(double valPt);
	void set_margin_right	(double valPt);
	void set_margin_top		(double valPt);
	void set_margin_bottom	(double valPt);

	void set_anchor	(int type);//anchor_type

	void set_object_background(bool Val);
	void set_object_foreground(bool Val);

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
	
	void start_group();		
		void set_group_flip_H	(bool bVal);
		void set_group_flip_V	(bool bVal);
		void set_group_z_order	(int Val);
		void set_group_name		(std::wstring name);

		void set_group_rotate	(int iVal);
		void set_group_size		(_CP_OPT(double) cx, _CP_OPT(double) cy, _CP_OPT(double) change_cx, _CP_OPT(double) change_cy);
		void set_group_position	(_CP_OPT(double) x, _CP_OPT(double) y, _CP_OPT(double) change_x, _CP_OPT(double) change_y);

		void set_group_scale	(double cx, double cy);
		void set_group_shift	(double x, double y);
	void end_group();

	void start_shape(int type);
	void end_shape();

	void start_image(std::wstring file_path);
	void end_image();
		
	void start_text_box();
		void set_text_box_min_size(double w_pt, double h_pt);
		void set_text_box_parent_style(std::wstring style_name);
	void end_text_box();

	void start_object(std::wstring name);
	void end_object();

	office_element_ptr & get_root_element();

	void start_element(office_element_ptr & elm, office_element_ptr  style_elm = office_element_ptr());
    void end_element();

	bool is_exist_content();
//////////////////////////////////////////////////////////////////////////////////////
	void set_path(std::wstring path_string);
	void add_path_element(std::wstring command, std::wstring & elm);
	void add_modifier(std::wstring modifier);
	
	void set_viewBox(double W, double H);

	void finalize(office_element_ptr & root_elm);

///////////////////////////////////////////////////////////////////////////////////////

	void set_size( _CP_OPT(double) & width_pt, _CP_OPT(double) & height_pt);
	void set_position(_CP_OPT(double) & x_pt, _CP_OPT(double) & y_pt);
	
	void get_size( double & width_pt, double & height_pt);
	
	void set_name(std::wstring  name);
	void set_z_order(int id);
	
	void set_flip_H(bool bVal);
	void set_flip_V(bool bVal);

	void set_rotate(double iVal);

	void set_no_fill();
	void set_solid_fill(std::wstring hexColor);
	void set_opacity(double percent);
//////////////////////////////////////////////////////////////
	void start_area_properties();
	void end_area_properties();
	
	void start_line_properties();
		void set_line_width(double pt);
		void set_line_dash_preset(int style);
		
		void set_line_head(int type, int len, int width);
		void set_line_tail(int type, int len, int width);

		std::wstring add_marker_style(int type);
	void end_line_properties();
	
	//void start_shadow_properties();
	//void end_shadow_properties();
// пока одной функией ..
	void set_shadow(int type, std::wstring hexColor, _CP_OPT(double) opacity, double dist_pt, double dist_pt_y = -1);

	void set_text(odf_text_context* text_context);
	void set_textarea_vertical_align(int align);
	void set_textarea_padding(double left,double top,double right,double bottom);//in cm
	void set_textarea_writing_mode(int mode);
	void set_textarea_wrap(bool val);
	void set_textarea_fontcolor(std::wstring hexColor);
	void set_textarea_font(_CP_OPT(std::wstring) & latin, _CP_OPT(std::wstring) & cs, _CP_OPT(std::wstring) & ea);
//////////////////////////////////////////////////////////////////////////////////////
	void start_gradient_style();
		void set_gradient_type(gradient_style style);
		void set_gradient_start(std::wstring hexColor, _CP_OPT(double) & intensiv);
		void set_gradient_end  (std::wstring hexColor, _CP_OPT(double) & intensiv);
		void set_gradient_rect(double l, double t, double r,double b);
		void set_gradient_angle(double angle);
	void end_gradient_style();
////////////////////////////////////////////////////////////////////////////////////////
	void start_opacity_style();
		void set_opacity_type(gradient_style style);
		void set_opacity_start(double val);
		void set_opacity_end  (double val);
		void set_opacity_rect(double l, double t, double r,double b);
		void set_opacity_angle(double angle);
	void end_opacity_style();
//////////////////////////////////////////////////////////////////////////////////////
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