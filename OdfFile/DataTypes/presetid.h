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

#include <string>
#include "odfattributes.h"

namespace cpdoccore { namespace odf_types {

class preset_id
{
public:
	enum type
	{
		none,

		ooo_entrance_appear,
		ooo_entrance_fly_in,
		ooo_entrance_venetian_blinds,
		ooo_entrance_box,
		ooo_entrance_checkerboard,
		ooo_entrance_circle,
		ooo_entrance_fly_in_slow,
		ooo_entrance_diamond,
		ooo_entrance_dissolve_in,
		ooo_entrance_fade_in,
		ooo_entrance_flash_once,
		ooo_entrance_peek_in,
		ooo_entrance_plus,
		ooo_entrance_random_bars,
		ooo_entrance_spiral_in,
		ooo_entrance_split,
		ooo_entrance_stretchy,
		ooo_entrance_diagonal_squares,
		ooo_entrance_swivel,
		ooo_entrance_wedge,
		ooo_entrance_wheel,
		ooo_entrance_wipe,
		ooo_entrance_zoom,
		ooo_entrance_random,
		ooo_entrance_boomerang,
		ooo_entrance_bounce,
		ooo_entrance_colored_lettering,
		ooo_entrance_movie_credits,
		ooo_entrance_ease_in,
		ooo_entrance_float,
		ooo_entrance_turn_and_grow,
		ooo_entrance_breaks,
		ooo_entrance_pinwheel,
		ooo_entrance_rise_up,
		ooo_entrance_falling_in,
		ooo_entrance_thread,
		ooo_entrance_unfold,
		ooo_entrance_whip,
		ooo_entrance_ascend,
		ooo_entrance_center_revolve,
		ooo_entrance_fade_in_and_swivel,
		ooo_entrance_descend,
		ooo_entrance_sling,
		ooo_entrance_spin_in,
		ooo_entrance_compress,
		ooo_entrance_magnify,
		ooo_entrance_curve_up,
		ooo_entrance_fade_in_and_zoom,
		ooo_entrance_glide,
		ooo_entrance_expand,
		ooo_entrance_flip,
		ooo_entrance_fold,

		ooo_emphasis_fill_color,
		ooo_emphasis_font,
		ooo_emphasis_font_color,
		ooo_emphasis_font_size,
		ooo_emphasis_font_style,
		ooo_emphasis_grow_and_shrink,
		ooo_emphasis_line_color,
		ooo_emphasis_spin,
		ooo_emphasis_transparency,
		ooo_emphasis_bold_flash,
		ooo_emphasis_blast,
		ooo_emphasis_bold_reveal,
		ooo_emphasis_color_over_by_word,
		ooo_emphasis_reveal_underline,
		ooo_emphasis_color_blend,
		ooo_emphasis_color_over_by_letter,
		ooo_emphasis_complementary_color,
		ooo_emphasis_complementary_color_2,
		ooo_emphasis_contrasting_color,
		ooo_emphasis_darken,
		ooo_emphasis_desaturate,
		ooo_emphasis_flash_bulb,
		ooo_emphasis_flicker,
		ooo_emphasis_grow_with_color,
		ooo_emphasis_lighten,
		ooo_emphasis_style_emphasis,
		ooo_emphasis_teeter,
		ooo_emphasis_vertical_highlight,
		ooo_emphasis_wave,
		ooo_emphasis_blink,
		ooo_emphasis_shimmer,

		ooo_exit_disappear,
		ooo_exit_fly_out,
		ooo_exit_venetian_blinds,
		ooo_exit_box,
		ooo_exit_checkerboard,
		ooo_exit_circle,
		ooo_exit_crawl_out,
		ooo_exit_diamond,
		ooo_exit_dissolve,
		ooo_exit_fade_out,
		ooo_exit_flash_once,
		ooo_exit_peek_out,
		ooo_exit_plus,
		ooo_exit_random_bars,
		ooo_exit_spiral_out,
		ooo_exit_split,
		ooo_exit_collapse,
		ooo_exit_diagonal_squares,
		ooo_exit_swivel,
		ooo_exit_wedge,
		ooo_exit_wheel,
		ooo_exit_wipe,
		ooo_exit_zoom,
		ooo_exit_random,
		ooo_exit_boomerang,
		ooo_exit_bounce,
		ooo_exit_colored_lettering,
		ooo_exit_movie_credits,
		ooo_exit_ease_out,
		ooo_exit_float,
		ooo_exit_turn_and_grow,
		ooo_exit_breaks,
		ooo_exit_pinwheel,
		ooo_exit_sink_down,
		ooo_exit_swish,
		ooo_exit_thread,
		ooo_exit_unfold,
		ooo_exit_whip,
		ooo_exit_descend,
		ooo_exit_center_revolve,
		ooo_exit_fade_out_and_swivel,
		ooo_exit_ascend,
		ooo_exit_sling,
		ooo_exit_fade_out_and_zoom,
		ooo_exit_contract,
		ooo_exit_spin_out,
		ooo_exit_stretchy,
		ooo_exit_magnify,
		ooo_exit_curve_down,
		ooo_exit_glide,
		ooo_exit_flip,
		ooo_exit_fold,

		ooo_motionpath_4_point_star,
		ooo_motionpath_5_point_star,
		ooo_motionpath_6_point_star,
		ooo_motionpath_8_point_star,
		ooo_motionpath_circle,
		ooo_motionpath_crescent_moon,
		ooo_motionpath_diamond,
		ooo_motionpath_equal_triangle,
		ooo_motionpath_oval,
		ooo_motionpath_heart,
		ooo_motionpath_hexagon,
		ooo_motionpath_octagon,
		ooo_motionpath_parallelogram,
		ooo_motionpath_pentagon,
		ooo_motionpath_right_triangle,
		ooo_motionpath_square,
		ooo_motionpath_teardrop,
		ooo_motionpath_trapezoid,
		ooo_motionpath_arc_down,
		ooo_motionpath_arc_left,
		ooo_motionpath_arc_right,
		ooo_motionpath_arc_up,
		ooo_motionpath_bounce_left,
		ooo_motionpath_bounce_right,
		ooo_motionpath_curvy_left,
		ooo_motionpath_curvy_right,
		ooo_motionpath_decaying_wave,
		ooo_motionpath_diagonal_down_right,
		ooo_motionpath_diagonal_up_right,
		ooo_motionpath_down,
		ooo_motionpath_funnel,
		ooo_motionpath_spring,
		ooo_motionpath_stairs_down,
		ooo_motionpath_turn_down,
		ooo_motionpath_turn_down_right,
		ooo_motionpath_turn_up,
		ooo_motionpath_turn_up_right,
		ooo_motionpath_up,
		ooo_motionpath_wave,
		ooo_motionpath_zigzag,
		ooo_motionpath_bean,
		ooo_motionpath_buzz_saw,
		ooo_motionpath_curved_square,
		ooo_motionpath_curved_x,
		ooo_motionpath_curvy_star,
		ooo_motionpath_figure_8_four,
		ooo_motionpath_horizontal_figure_8,
		ooo_motionpath_inverted_square,
		ooo_motionpath_inverted_triangle,
		ooo_motionpath_loop_de_loop,
		ooo_motionpath_neutron,
		ooo_motionpath_peanut,
		ooo_motionpath_clover,
		ooo_motionpath_pointy_star,
		ooo_motionpath_swoosh,
		ooo_motionpath_vertical_figure_8,
		ooo_motionpath_left,
		ooo_motionpath_right,
		ooo_motionpath_spiral_left,
		ooo_motionpath_spiral_right,
		ooo_motionpath_sine_wave,
		ooo_motionpath_s_curve_1,
		ooo_motionpath_s_curve_2,
		ooo_motionpath_heartbeat,

		libo_motionpath_curve,
		libo_motionpath_polygon,
		libo_motionpath_freeform_line
	};

	preset_id() {}

	preset_id(type _Type) : type_(_Type)
	{}

	type get_type() const
	{
		return type_;
	};

	static preset_id parse(const std::wstring& Str);

private:
	type type_;

};
std::wostream& operator << (std::wostream& _Wostream, const preset_id& _Val);

} // namespace odf_types

APPLY_PARSE_XML_ATTRIBUTES(odf_types::preset_id);

} // namespace cpdoccore
