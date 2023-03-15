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
#pragma once

#include <string>
#include <vector>
#include <map>

#include <CPOptional.h>

#include "office_elements_create.h"

#include "../../DataTypes/stylewrap.h"
#include "../../DataTypes/gradientstyle.h"
#include "../../DataTypes/common_attlists.h"

namespace OOX {namespace Vml { class CShapeType; }}

namespace cpdoccore {
namespace odf_writer
{

class odf_conversion_context;
class odf_style_context;
class odf_text_context;
class style_paragraph_properties;
class style_text_properties;
class style_graphic_properties;
class graphic_format_properties;

class odf_drawing_context
{
public:
	odf_drawing_context		(odf_conversion_context *odf_context);
    ~odf_drawing_context	();

	void			set_presentation (int type);
	_CP_OPT(int)	get_presentation ();

	void set_drawings_rect	(_CP_OPT(double) x_pt, _CP_OPT(double) y_pt, _CP_OPT(double) width_pt, _CP_OPT(double) height_pt);
	void clear				();
	void set_styles_context	(odf_style_context*  styles_context);//для embedded 

	void set_parent_style	(std::wstring style_name);

	void set_header_state		(bool Val);
	void set_footer_state		(bool Val);
	void set_background_state	(bool Val);
	
	void check_anchor		();

	void set_margin_left	(double valPt);
	void set_margin_right	(double valPt);
	void set_margin_top		(double valPt);
	void set_margin_bottom	(double valPt);

	void							set_anchor(int type);//anchor_type
	odf_types::anchor_type::type	get_anchor();

	void set_object_background(bool Val);
	void set_object_foreground(bool Val);

	void set_vertical_rel	(int from);
	void set_vertical_pos	(int align);
	void set_vertical_pos	(double offset_pt);

	void set_horizontal_rel	(int from);
	void set_horizontal_pos	(int align);
	void set_horizontal_pos	(double offset_pt);

	void set_default_wrap_style();
	void set_wrap_style(odf_types::style_wrap::type  style);
	void set_wrap_contour();

	void set_overlap		(bool val);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	office_element_ptr & get_current_style_element() ;
	office_element_ptr & get_current_element();

	void start_drawing();
	void end_drawing();
	void end_drawing_background(odf_types::common_draw_fill_attlist & common_draw_attlist);
	
	size_t	get_group_level();	
	void start_group();		
		void set_group_flip_H	(bool bVal);
		void set_group_flip_V	(bool bVal);
		void set_group_z_order	(int Val);
		void set_group_name		(const std::wstring & name);

		void set_group_rotate	(int iVal);
		void set_group_size		(_CP_OPT(double) cx, _CP_OPT(double) cy, _CP_OPT(double) change_cx, _CP_OPT(double) change_cy);
		void set_group_position	(_CP_OPT(double) x, _CP_OPT(double) y, _CP_OPT(double) change_x, _CP_OPT(double) change_y);

		void set_group_scale	(double cx, double cy);
		void set_group_shift	(double x, double y);

		void set_corner_radius	(odf_types::length corner);
	void end_group();

	void start_shape	(int ooxDrawPreset);
	void end_shape		();

	void start_frame	();
	void end_frame		();

	void start_image	(std::wstring odf_file_path);
	void end_image		();
	
	bool change_text_box_2_wordart();
	bool is_wordart();
	bool is_text_box();
	
	graphic_format_properties* get_graphic_properties();

	void set_graphic_properties		(style_graphic_properties *graphic_properties);	
	void set_paragraph_properties	(style_paragraph_properties *paragraph_properties);
	void set_text_properties		(style_text_properties *text_properties);
	
	void start_text_box					();
		void set_text_box_min_size		(bool val);
		void set_text_box_min_size		(double w_pt, double h_pt);
		void set_text_box_tableframe	(bool val);		
	void end_text_box					();

	void start_object(std::wstring ref, bool in_frame = true); //формулы, диаграммы ... 
	void end_object(bool in_frame = true);

	void start_object_ole(std::wstring ref);
	void end_object_ole();

	void start_control(const std::wstring& id);
	void end_control();

	void start_media(std::wstring ref);
	void end_media();

	void start_link_object(std::wstring href);
	void end_link_object();

	void add_image_replacement();
	void set_image_replacement(std::wstring ref);

	void set_program(std::wstring program);

	bool isLineShape();
	bool isCustomClosed();

	void corrected_line_fill();

	office_element_ptr & get_root_element();

	void start_element	(office_element_ptr elm, office_element_ptr  style_elm = office_element_ptr());
    void end_element	();

	bool is_exist_content();
	bool is_current_empty();
//////////////////////////////////////////////////////////////////////////////////////
	void set_path			(std::wstring path_string);
	void add_path_element	(std::wstring command, std::wstring elm);
	void add_modifier		(std::wstring modifier);
	void add_formula		(std::wstring name, std::wstring fmla);
	void set_textarea		(std::wstring l, std::wstring t, std::wstring r, std::wstring b);
	void add_handle			(std::wstring x, std::wstring y, std::wstring refX, std::wstring refY,
							std::wstring minX, std::wstring maxX, std::wstring minY, std::wstring maxY);
	
	void set_viewBox		(double W, double H);

	void finalize(office_element_ptr & root_elm);

///////////////////////////////////////////////////////////////////////////////////////
	void get_position		(_CP_OPT(double) & x_pt, _CP_OPT(double) & y_pt);
	void set_position		(_CP_OPT(double) & x_pt, _CP_OPT(double) & y_pt);
	void set_position_line	(_CP_OPT(double) & x_pt, _CP_OPT(double) & y_pt, _CP_OPT(double) & x1_pt, _CP_OPT(double) & y1_pt); 
	
	void get_size			( _CP_OPT(double) & width_pt, _CP_OPT(double) & height_pt);
	void set_size			( _CP_OPT(double) & width_pt, _CP_OPT(double) & height_pt, bool reset_always = false);
	
	void set_name			(const std::wstring & name);
	void set_description	(const std::wstring & descr);
	void set_hidden			(bool bVal);
	void set_z_order		(int id);
	
	void set_flip_H			(bool bVal);
	void set_flip_V			(bool bVal);

	void set_rotate			(double dVal);

	void set_no_fill		();
	void set_group_fill		();
	void set_type_fill		(int type);//for area - temp for objects
	void set_solid_fill		(std::wstring hexColor);
	void set_opacity		(double percent);

//////////////////////////////////////////////////////////////
	void start_area_properties(bool reset = false);
	void end_area_properties();
	
	void start_line_properties		(bool reset = false);
		void set_line_width			(double pt);
		void set_line_dash_preset	(int style);
		
		void set_line_head			(int type, int len, int width);
		void set_line_tail			(int type, int len, int width);

		std::wstring add_marker_style(int type);
	void end_line_properties		();
	
	_CP_OPT(unsigned int)	get_fill_color ();
	void					set_fill_color (unsigned int color);

	_CP_OPT(odf_types::color) get_line_color();
	void set_line_color	(std::wstring hexColor);
	//void start_shadow_properties();
	//void end_shadow_properties();
// пока одной функией ..
	void set_shadow	(int type, std::wstring hexColor, _CP_OPT(double) opacity, double dist_pt, double dist_pt_y = -1);
	void set_grayscale();
	void set_white_balance(double red, double green, double blue);

	void set_text					(odf_text_context* text_context);
	void set_textarea_vertical_align(int align);
	void set_textarea_padding		(_CP_OPT(double) & left, _CP_OPT(double) & top, _CP_OPT(double) & right, _CP_OPT(double) & bottom);//in pt
	void set_textarea_writing_mode	(int mode);
	void set_textarea_wrap			(bool val);
	void set_textarea_fontcolor		(std::wstring hexColor);
	void set_textarea_font			(std::wstring & latin, std::wstring & cs, std::wstring & ea);
	void set_textarea_fit_to_size	(bool val);
	void set_textarea_rotation		(double val);

	void set_placeholder_id			(std::wstring val);
	void set_placeholder_type		(int val);
//////////////////////////////////////////////////////////////////////////////////////
	void start_gradient_style	();
		void set_gradient_type	(odf_types::gradient_style::type style);
		void set_gradient_start	(std::wstring hexColor, _CP_OPT(double) & intensiv);
		void set_gradient_end	(std::wstring hexColor, _CP_OPT(double) & intensiv);
		void set_gradient_rect(	double l, double t, double r,double b);
		void set_gradient_center(double cx, double cy);
		void set_gradient_angle	(double angle);
	void end_gradient_style		();
////////////////////////////////////////////////////////////////////////////////////////
	void start_opacity_style	();
		void set_opacity_type	(odf_types::gradient_style style);
		void set_opacity_start	(double val);
		void set_opacity_end	(double val);
		void set_opacity_rect	(double l, double t, double r,double b);
		void set_opacity_angle	(double angle);
	void end_opacity_style		();
//////////////////////////////////////////////////////////////////////////////////////
	void start_hatch_style();
		void set_hatch_line_color(std::wstring hexColor);
		void set_hatch_area_color(std::wstring hexColor);
		void set_hatch_type(int type);
	void end_hatch_style();
////////////////////////////////////////////////////////////////////////////////////////
	void start_bitmap_style();
		void set_bitmap_link(std::wstring link, bool bExternal = false);
	void end_bitmap_style();
//////////////////////////////////////////////////////////////////////////////////////
	void set_image_style_repeat		(int style);
	void set_image_client_rect_pt	(double l, double t, double r, double b);
	void set_image_client_rect_inch	(double l, double t, double r, double b);
	
	void set_bitmap_tile_align		(int align);
	void set_bitmap_tile_scale_x	(double scale_x);
	void set_bitmap_tile_scale_y	(double scale_y);
	void set_bitmap_tile_translate_y(double y);
	void set_bitmap_tile_translate_x(double x);

	void start_action(std::wstring value);
		void add_sound	(std::wstring href);
		void add_link	(std::wstring href);
	void end_action();

private:
	
    class Impl;
    _CP_PTR(Impl) impl_;
};
typedef shared_ptr<odf_drawing_context>::Type	odf_drawing_context_ptr;

}
}