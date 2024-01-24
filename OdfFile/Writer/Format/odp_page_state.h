/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include <vector>

#include "../../../OOXML/Base/Unit.h"

#include "odf_drawing_context.h"
#include "odf_comment_context.h"
#include "odf_controls_context.h"
#include "style_presentation.h"

namespace cpdoccore {

namespace odf_types
{
	class color;
}

namespace odf_writer {

class odp_conversion_context;
class odf_text_context;

class style;


struct anim_state
{
	anim_state() : empty(true),  attlist(NULL), id(-1) {}
	
	int										id;
	office_element_ptr						elm;	
	odf_types::common_anim_smil_attlist*		attlist;
	odf_types::anim_par_attlist*				par_attlist;
	odf_types::anim_set_attlist*				set_attlist;
	odf_types::anim_transition_filter_attlist* 	transition_filter_attlist;
	odf_types::anim_animate_attlist*			animate_attlist;
	odf_types::anim_animate_motion_attlist*		motion_attlist;
	odf_types::anim_animate_color_attlist*		color_attlist;
	odf_types::anim_animate_transform_attlist*	transform_attlist;
	odf_types::anim_audio_attlist*				audio_attlist;

	bool empty;
};


class odp_page_state
{
public:
	odp_page_state(odf_conversion_context * Context, office_element_ptr & elm);
		void set_page_name		(std::wstring name);
		void set_page_id		(int id);
		void set_page_style		(office_element_ptr & _style);
		void set_page_duration	(int id);

		void hide_page();
		
		void set_master_page(std::wstring name);
		void set_layout_page(std::wstring name);
	
	void add_child_element( const office_element_ptr & child_element);
	void finalize_page();
///////////////////////////////
	odf_drawing_context		*drawing_context(){return  &drawing_context_;}
	odf_comment_context		*comment_context(){return  &comment_context_;}
	odf_controls_context	*controls_context(){return  &controls_context_;}

	std::wstring			page_name_;
	int						page_id_;
	office_element_ptr		page_elm_;
	office_element_ptr		page_style_elm_;

	std::vector<anim_state>			anim_levels;
	std::vector<office_element_ptr>	transactions;

	void start_timing();
		void start_anim_level(const std::wstring& ns, const std::wstring& name);
		void end_anim_level();

		void start_timing_par();
		void end_timing_par();

		void start_timing_seq();
		void end_timing_seq();

		void start_timing_set();
		void end_timing_set();

		void start_timing_transition_filter();
		void end_timing_transition_filter();

		void start_timing_anim();
		void end_timing_anim();

		void start_timing_motion();
		void end_timing_motion();

		void start_timing_anim_clr();
		void end_timing_anim_clr();

		void start_timing_transform();
		void end_timing_transform();

		void start_anim_audio();
		void end_anim_audio();

		void set_anim_id			(int val);
		void set_anim_type			(const odf_types::presentation_node_type& val);
		void set_anim_duration		(int val);
		void set_anim_restart		(std::wstring val);
		void set_anim_fill			(const odf_types::smil_fill& val);
		void set_anim_begin			(const std::wstring& val);
		void set_anim_node_type		(const odf_types::presentation_node_type& val);
		void set_anim_preset_class	(const odf_types::preset_class& val);
		void set_anim_preset_id		(const odf_types::preset_id& val);
		void set_anim_attribute_name(const odf_types::smil_attribute_name& val);
		void set_anim_to			(const std::wstring& val);
		void set_anim_target_element(const std::wstring& val);
		void set_anim_auto_reverse	(bool val);
		void set_anim_subtype		(const std::wstring& val);
		void set_anim_accelerate	(double val);
		void set_anim_decelerate	(double val);

		void set_anim_animation_formula(const std::wstring& val);
		void set_anim_animation_keytimes(const odf_types::smil_key_times& val);
		void set_anim_animation_values(const odf_types::smil_values& val);
		void set_anim_animation_by(const std::wstring& val);
		void set_anim_animation_from(const std::wstring& val);
		void set_anim_animation_to(const std::wstring& val);
		void set_anim_animation_type(const odf_types::svg_type& val);

		void set_anim_transition_filter_mode(const std::wstring& val);
		void set_anim_transition_filter_type(const odf_types::smil_transition_type& val);
		void set_anim_transition_filter_subtype(const std::wstring& val);
		void set_anim_transition_filter_direction(const std::wstring& val);

		void set_anim_motion_path(const std::wstring& val);

		void set_anim_color_to(const std::wstring& val);
		void set_anim_color_by(const std::wstring& val);
		void set_anim_color_interpolation(const std::wstring& val);
		void set_anim_color_direction(const std::wstring& val);

		void set_anim_transform_type(const odf_types::svg_type& val);
		void set_anim_transform_from(const std::wstring& val);
		void set_anim_transform_to(const std::wstring& val);
		void set_anim_transform_by(const std::wstring& val);
		
		void set_anim_audio_xlink(const std::wstring& val);


	void end_timing();

	void start_transition();
		void set_transition_type	(int val);
		void set_transition_subtype	(std::wstring val);
		void set_transition_direction(std::wstring val);
		void set_transition_speed	(int val);
		void set_transition_duration(int val);
		void set_transition_sound	(std::wstring ref, bool loop);
	void end_transition(){}
private:

    odf_conversion_context *		context_;   
	
	odf_drawing_context				drawing_context_;	
	odf_controls_context			controls_context_;
	odf_comment_context				comment_context_;	
	
	style_drawing_page_properties*	page_properties_;

	friend class odp_slide_context;

};


}
}

