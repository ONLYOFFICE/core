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

#include "presetid.h"

#include <boost/algorithm/string.hpp>

#include <ostream>

namespace cpdoccore { namespace odf_types {

	preset_id::id_map preset_id::preset_maping_ = {
		{ L"ooo-entrance-appear",				preset_id::type::ooo_entrance_appear },
		{ L"ooo-entrance-fly-in",				preset_id::type::ooo_entrance_fly_in },
		{ L"ooo-entrance-venetian-blinds",		preset_id::type::ooo_entrance_venetian_blinds },
		{ L"ooo-entrance-box",					preset_id::type::ooo_entrance_box },
		{ L"ooo-entrance-checkerboard",			preset_id::type::ooo_entrance_checkerboard },
		{ L"ooo-entrance-circle",				preset_id::type::ooo_entrance_circle },
		{ L"ooo-entrance-fly-in-slow",			preset_id::type::ooo_entrance_fly_in_slow },
		{ L"ooo-entrance-diamond",				preset_id::type::ooo_entrance_diamond },
		{ L"ooo-entrance-dissolve-in",			preset_id::type::ooo_entrance_dissolve_in },
		{ L"ooo-entrance-fade-in",				preset_id::type::ooo_entrance_fade_in },
		{ L"ooo-entrance-flash-once",			preset_id::type::ooo_entrance_flash_once },
		{ L"ooo-entrance-peek-in",				preset_id::type::ooo_entrance_peek_in },
		{ L"ooo-entrance-plus",					preset_id::type::ooo_entrance_plus },
		{ L"ooo-entrance-random-bars",			preset_id::type::ooo_entrance_random_bars },
		{ L"ooo-entrance-spiral-in",			preset_id::type::ooo_entrance_spiral_in },
		{ L"ooo-entrance-split",				preset_id::type::ooo_entrance_split },
		{ L"ooo-entrance-stretchy",				preset_id::type::ooo_entrance_stretchy },
		{ L"ooo-entrance-diagonal-squares",		preset_id::type::ooo_entrance_diagonal_squares },
		{ L"ooo-entrance-swivel",				preset_id::type::ooo_entrance_swivel },
		{ L"ooo-entrance-wedge",				preset_id::type::ooo_entrance_wedge },
		{ L"ooo-entrance-wheel",				preset_id::type::ooo_entrance_wheel },
		{ L"ooo-entrance-wipe",					preset_id::type::ooo_entrance_wipe },
		{ L"ooo-entrance-zoom",					preset_id::type::ooo_entrance_zoom },
		{ L"ooo-entrance-random",				preset_id::type::ooo_entrance_random },
		{ L"ooo-entrance-boomerang",			preset_id::type::ooo_entrance_boomerang },
		{ L"ooo-entrance-bounce",				preset_id::type::ooo_entrance_bounce },
		{ L"ooo-entrance-colored-lettering",	preset_id::type::ooo_entrance_colored_lettering },
		{ L"ooo-entrance-movie-credits",		preset_id::type::ooo_entrance_movie_credits },
		{ L"ooo-entrance-ease-in",				preset_id::type::ooo_entrance_ease_in },
		{ L"ooo-entrance-float",				preset_id::type::ooo_entrance_float },
		{ L"ooo-entrance-turn-and-grow",		preset_id::type::ooo_entrance_turn_and_grow },
		{ L"ooo-entrance-breaks",				preset_id::type::ooo_entrance_breaks },
		{ L"ooo-entrance-pinwheel",				preset_id::type::ooo_entrance_pinwheel },
		{ L"ooo-entrance-rise-up",				preset_id::type::ooo_entrance_rise_up },
		{ L"ooo-entrance-falling-in",			preset_id::type::ooo_entrance_falling_in },
		{ L"ooo-entrance-thread",				preset_id::type::ooo_entrance_thread },
		{ L"ooo-entrance-unfold",				preset_id::type::ooo_entrance_unfold },
		{ L"ooo-entrance-whip",					preset_id::type::ooo_entrance_whip },
		{ L"ooo-entrance-ascend",				preset_id::type::ooo_entrance_ascend },
		{ L"ooo-entrance-center-revolve",		preset_id::type::ooo_entrance_center_revolve },
		{ L"ooo-entrance-fade-in-and-swivel",	preset_id::type::ooo_entrance_fade_in_and_swivel },
		{ L"ooo-entrance-descend",				preset_id::type::ooo_entrance_descend },
		{ L"ooo-entrance-sling",				preset_id::type::ooo_entrance_sling },
		{ L"ooo-entrance-spin-in",				preset_id::type::ooo_entrance_spin_in },
		{ L"ooo-entrance-compress",				preset_id::type::ooo_entrance_compress },
		{ L"ooo-entrance-magnify",				preset_id::type::ooo_entrance_magnify },
		{ L"ooo-entrance-curve-up",				preset_id::type::ooo_entrance_curve_up },
		{ L"ooo-entrance-fade-in-and-zoom",		preset_id::type::ooo_entrance_fade_in_and_zoom },
		{ L"ooo-entrance-glide",				preset_id::type::ooo_entrance_glide },
		{ L"ooo-entrance-expand",				preset_id::type::ooo_entrance_expand },
		{ L"ooo-entrance-flip",					preset_id::type::ooo_entrance_flip },
		{ L"ooo-entrance-fold",					preset_id::type::ooo_entrance_fold },

		{ L"ooo-emphasis-fill-color",			preset_id::type::ooo_emphasis_fill_color },
		{ L"ooo-emphasis-font",					preset_id::type::ooo_emphasis_font },
		{ L"ooo-emphasis-font-color",			preset_id::type::ooo_emphasis_font_color },
		{ L"ooo-emphasis-font-size",			preset_id::type::ooo_emphasis_font_size },
		{ L"ooo-emphasis-font-style",			preset_id::type::ooo_emphasis_font_style },
		{ L"ooo-emphasis-grow-and-shrink",		preset_id::type::ooo_emphasis_grow_and_shrink },
		{ L"ooo-emphasis-line-color",			preset_id::type::ooo_emphasis_line_color },
		{ L"ooo-emphasis-spin",					preset_id::type::ooo_emphasis_spin },
		{ L"ooo-emphasis-transparency",			preset_id::type::ooo_emphasis_transparency },
		{ L"ooo-emphasis-bold-flash",			preset_id::type::ooo_emphasis_bold_flash },
		{ L"ooo-emphasis-blast",				preset_id::type::ooo_emphasis_blast },
		{ L"ooo-emphasis-bold-reveal",			preset_id::type::ooo_emphasis_bold_reveal },
		{ L"ooo-emphasis-color-over-by-word",	preset_id::type::ooo_emphasis_color_over_by_word },
		{ L"ooo-emphasis-reveal-underline",		preset_id::type::ooo_emphasis_reveal_underline },
		{ L"ooo-emphasis-color-blend",			preset_id::type::ooo_emphasis_color_blend },
		{ L"ooo-emphasis-color-over-by-letter", preset_id::type::ooo_emphasis_color_over_by_letter },
		{ L"ooo-emphasis-complementary-color",	preset_id::type::ooo_emphasis_complementary_color },
		{ L"ooo-emphasis-complementary-color-2", preset_id::type::ooo_emphasis_complementary_color_2 },
		{ L"ooo-emphasis-contrasting-color",	preset_id::type::ooo_emphasis_contrasting_color },
		{ L"ooo-emphasis-darken",				preset_id::type::ooo_emphasis_darken },
		{ L"ooo-emphasis-desaturate",			preset_id::type::ooo_emphasis_desaturate },
		{ L"ooo-emphasis-flash-bulb",			preset_id::type::ooo_emphasis_flash_bulb },
		{ L"ooo-emphasis-flicker",				preset_id::type::ooo_emphasis_flicker },
		{ L"ooo-emphasis-grow-with-color",		preset_id::type::ooo_emphasis_grow_with_color },
		{ L"ooo-emphasis-lighten",				preset_id::type::ooo_emphasis_lighten },
		{ L"ooo-emphasis-style-emphasis",		preset_id::type::ooo_emphasis_style_emphasis },
		{ L"ooo-emphasis-teeter",				preset_id::type::ooo_emphasis_teeter },
		{ L"ooo-emphasis-vertical-highlight",	preset_id::type::ooo_emphasis_vertical_highlight },
		{ L"ooo-emphasis-wave",					preset_id::type::ooo_emphasis_wave },
		{ L"ooo-emphasis-blink",				preset_id::type::ooo_emphasis_blink },
		{ L"ooo-emphasis-shimmer",				preset_id::type::ooo_emphasis_shimmer },

		{ L"ooo-exit-disappear",				preset_id::type::ooo_exit_disappear },
		{ L"ooo-exit-fly-out",					preset_id::type::ooo_exit_fly_out },
		{ L"ooo-exit-venetian-blinds",			preset_id::type::ooo_exit_venetian_blinds },
		{ L"ooo-exit-box",						preset_id::type::ooo_exit_box },
		{ L"ooo-exit-checkerboard",				preset_id::type::ooo_exit_checkerboard },
		{ L"ooo-exit-circle",					preset_id::type::ooo_exit_circle },
		{ L"ooo-exit-crawl-out",				preset_id::type::ooo_exit_crawl_out },
		{ L"ooo-exit-diamond",					preset_id::type::ooo_exit_diamond },
		{ L"ooo-exit-dissolve",					preset_id::type::ooo_exit_dissolve },
		{ L"ooo-exit-fade-out",					preset_id::type::ooo_exit_fade_out },
		{ L"ooo-exit-flash-once",				preset_id::type::ooo_exit_flash_once },
		{ L"ooo-exit-peek-out",					preset_id::type::ooo_exit_peek_out },
		{ L"ooo-exit-plus",						preset_id::type::ooo_exit_plus },
		{ L"ooo-exit-random-bars",				preset_id::type::ooo_exit_random_bars },
		{ L"ooo-exit-spiral-out",				preset_id::type::ooo_exit_spiral_out },
		{ L"ooo-exit-split",					preset_id::type::ooo_exit_split },
		{ L"ooo-exit-collapse",					preset_id::type::ooo_exit_collapse },
		{ L"ooo-exit-diagonal-squares",			preset_id::type::ooo_exit_diagonal_squares },
		{ L"ooo-exit-swivel",					preset_id::type::ooo_exit_swivel },
		{ L"ooo-exit-wedge",					preset_id::type::ooo_exit_wedge },
		{ L"ooo-exit-wheel",					preset_id::type::ooo_exit_wheel },
		{ L"ooo-exit-wipe",						preset_id::type::ooo_exit_wipe },
		{ L"ooo-exit-zoom",						preset_id::type::ooo_exit_zoom },
		{ L"ooo-exit-random",					preset_id::type::ooo_exit_random },
		{ L"ooo-exit-boomerang",				preset_id::type::ooo_exit_boomerang },
		{ L"ooo-exit-bounce",					preset_id::type::ooo_exit_bounce },
		{ L"ooo-exit-colored-lettering",		preset_id::type::ooo_exit_colored_lettering },
		{ L"ooo-exit-movie-credits",			preset_id::type::ooo_exit_movie_credits },
		{ L"ooo-exit-ease-out",					preset_id::type::ooo_exit_ease_out },
		{ L"ooo-exit-float",					preset_id::type::ooo_exit_float },
		{ L"ooo-exit-turn-and-grow",			preset_id::type::ooo_exit_turn_and_grow },
		{ L"ooo-exit-breaks",					preset_id::type::ooo_exit_breaks },
		{ L"ooo-exit-pinwheel",					preset_id::type::ooo_exit_pinwheel },
		{ L"ooo-exit-sink-down",				preset_id::type::ooo_exit_sink_down },
		{ L"ooo-exit-swish",					preset_id::type::ooo_exit_swish },
		{ L"ooo-exit-thread",					preset_id::type::ooo_exit_thread },
		{ L"ooo-exit-unfold",					preset_id::type::ooo_exit_unfold },
		{ L"ooo-exit-whip",						preset_id::type::ooo_exit_whip },
		{ L"ooo-exit-descend",					preset_id::type::ooo_exit_descend },
		{ L"ooo-exit-center-revolve",			preset_id::type::ooo_exit_center_revolve },
		{ L"ooo-exit-fade-out-and-swivel",		preset_id::type::ooo_exit_fade_out_and_swivel },
		{ L"ooo-exit-ascend",					preset_id::type::ooo_exit_ascend },
		{ L"ooo-exit-sling",					preset_id::type::ooo_exit_sling },
		{ L"ooo-exit-fade-out-and-zoom",		preset_id::type::ooo_exit_fade_out_and_zoom },
		{ L"ooo-exit-contract",					preset_id::type::ooo_exit_contract },
		{ L"ooo-exit-spin-out",					preset_id::type::ooo_exit_spin_out },
		{ L"ooo-exit-stretchy",					preset_id::type::ooo_exit_stretchy },
		{ L"ooo-exit-magnify",					preset_id::type::ooo_exit_magnify },
		{ L"ooo-exit-curve-down",				preset_id::type::ooo_exit_curve_down },
		{ L"ooo-exit-glide",					preset_id::type::ooo_exit_glide },
		{ L"ooo-exit-flip",						preset_id::type::ooo_exit_flip },
		{ L"ooo-exit-fold",						preset_id::type::ooo_exit_fold },

		{ L"ooo-motionpath-4-point-star",		preset_id::type::ooo_motionpath_4_point_star },
		{ L"ooo-motionpath-5-point-star",		preset_id::type::ooo_motionpath_5_point_star },
		{ L"ooo-motionpath-6-point-star",		preset_id::type::ooo_motionpath_6_point_star },
		{ L"ooo-motionpath-8-point-star",		preset_id::type::ooo_motionpath_8_point_star },
		{ L"ooo-motionpath-circle",				preset_id::type::ooo_motionpath_circle },
		{ L"ooo-motionpath-crescent-moon",		preset_id::type::ooo_motionpath_crescent_moon },
		{ L"ooo-motionpath-diamond",			preset_id::type::ooo_motionpath_diamond },
		{ L"ooo-motionpath-equal-triangle",		preset_id::type::ooo_motionpath_equal_triangle },
		{ L"ooo-motionpath-oval",				preset_id::type::ooo_motionpath_oval },
		{ L"ooo-motionpath-heart",				preset_id::type::ooo_motionpath_heart },
		{ L"ooo-motionpath-hexagon",			preset_id::type::ooo_motionpath_hexagon },
		{ L"ooo-motionpath-octagon",			preset_id::type::ooo_motionpath_octagon },
		{ L"ooo-motionpath-parallelogram",		preset_id::type::ooo_motionpath_parallelogram },
		{ L"ooo-motionpath-pentagon",			preset_id::type::ooo_motionpath_pentagon },
		{ L"ooo-motionpath-right-triangle",		preset_id::type::ooo_motionpath_right_triangle },
		{ L"ooo-motionpath-square",				preset_id::type::ooo_motionpath_square },
		{ L"ooo-motionpath-teardrop",			preset_id::type::ooo_motionpath_teardrop },
		{ L"ooo-motionpath-trapezoid",			preset_id::type::ooo_motionpath_trapezoid },
		{ L"ooo-motionpath-arc-down",			preset_id::type::ooo_motionpath_arc_down },
		{ L"ooo-motionpath-arc-left",			preset_id::type::ooo_motionpath_arc_left },
		{ L"ooo-motionpath-arc-right",			preset_id::type::ooo_motionpath_arc_right },
		{ L"ooo-motionpath-arc-up",				preset_id::type::ooo_motionpath_arc_up },
		{ L"ooo-motionpath-bounce-left",		preset_id::type::ooo_motionpath_bounce_left },
		{ L"ooo-motionpath-bounce-right",		preset_id::type::ooo_motionpath_bounce_right },
		{ L"ooo-motionpath-curvy-left",			preset_id::type::ooo_motionpath_curvy_left },
		{ L"ooo-motionpath-curvy-right",		preset_id::type::ooo_motionpath_curvy_right },
		{ L"ooo-motionpath-decaying-wave",		preset_id::type::ooo_motionpath_decaying_wave },
		{ L"ooo-motionpath-diagonal-down-right", preset_id::type::ooo_motionpath_diagonal_down_right },
		{ L"ooo-motionpath-diagonal-up-right",	preset_id::type::ooo_motionpath_diagonal_up_right },
		{ L"ooo-motionpath-down",				preset_id::type::ooo_motionpath_down },
		{ L"ooo-motionpath-funnel",				preset_id::type::ooo_motionpath_funnel },
		{ L"ooo-motionpath-spring",				preset_id::type::ooo_motionpath_spring },
		{ L"ooo-motionpath-stairs-down",		preset_id::type::ooo_motionpath_stairs_down },
		{ L"ooo-motionpath-turn-down",			preset_id::type::ooo_motionpath_turn_down },
		{ L"ooo-motionpath-turn-down-right",	preset_id::type::ooo_motionpath_turn_down_right },
		{ L"ooo-motionpath-turn-up",			preset_id::type::ooo_motionpath_turn_up },
		{ L"ooo-motionpath-turn-up-right",		preset_id::type::ooo_motionpath_turn_up_right },
		{ L"ooo-motionpath-up",					preset_id::type::ooo_motionpath_up },
		{ L"ooo-motionpath-wave",				preset_id::type::ooo_motionpath_wave },
		{ L"ooo-motionpath-zigzag",				preset_id::type::ooo_motionpath_zigzag },
		{ L"ooo-motionpath-bean",				preset_id::type::ooo_motionpath_bean },
		{ L"ooo-motionpath-buzz-saw",			preset_id::type::ooo_motionpath_buzz_saw },
		{ L"ooo-motionpath-curved-square",		preset_id::type::ooo_motionpath_curved_square },
		{ L"ooo-motionpath-curved-x",			preset_id::type::ooo_motionpath_curved_x },
		{ L"ooo-motionpath-curvy-star",			preset_id::type::ooo_motionpath_curvy_star },
		{ L"ooo-motionpath-figure-8-four",		preset_id::type::ooo_motionpath_figure_8_four },
		{ L"ooo-motionpath-horizontal-figure-8", preset_id::type::ooo_motionpath_horizontal_figure_8 },
		{ L"ooo-motionpath-inverted-square",	preset_id::type::ooo_motionpath_inverted_square },
		{ L"ooo-motionpath-inverted-triangle",	preset_id::type::ooo_motionpath_inverted_triangle },
		{ L"ooo-motionpath-loop-de-loop",		preset_id::type::ooo_motionpath_loop_de_loop },
		{ L"ooo-motionpath-neutron",			preset_id::type::ooo_motionpath_neutron },
		{ L"ooo-motionpath-peanut",				preset_id::type::ooo_motionpath_peanut },
		{ L"ooo-motionpath-clover",				preset_id::type::ooo_motionpath_clover },
		{ L"ooo-motionpath-pointy-star",		preset_id::type::ooo_motionpath_pointy_star },
		{ L"ooo-motionpath-swoosh",				preset_id::type::ooo_motionpath_swoosh },
		{ L"ooo-motionpath-vertical-figure-8",	preset_id::type::ooo_motionpath_vertical_figure_8 },
		{ L"ooo-motionpath-left",				preset_id::type::ooo_motionpath_left },
		{ L"ooo-motionpath-right",				preset_id::type::ooo_motionpath_right },
		{ L"ooo-motionpath-spiral-left",		preset_id::type::ooo_motionpath_spiral_left },
		{ L"ooo-motionpath-spiral-right",		preset_id::type::ooo_motionpath_spiral_right },
		{ L"ooo-motionpath-sine-wave",			preset_id::type::ooo_motionpath_sine_wave },
		{ L"ooo-motionpath-s-curve-1",			preset_id::type::ooo_motionpath_s_curve_1 },
		{ L"ooo-motionpath-s-curve-2",			preset_id::type::ooo_motionpath_s_curve_2 },
		{ L"ooo-motionpath-heartbeat",			preset_id::type::ooo_motionpath_heartbeat },

	};

	preset_id preset_id::parse(const std::wstring& Str)
	{
		std::wstring tmp = Str;
		boost::algorithm::to_lower(tmp);

		id_map::const_iterator id = preset_maping_.find(tmp);
		if (id == preset_maping_.end())
			return preset_id::type::none;

		return id->second;
	}

	std::wostream& operator << (std::wostream& _Wostream, const preset_id& _Val)
	{
		switch (_Val.get_type())
		{
		case preset_id::type::ooo_entrance_appear				: _Wostream << L"ooo-entrance-appear"				; break;
		case preset_id::type::ooo_entrance_fly_in				: _Wostream << L"ooo-entrance-fly-in"				; break;
		case preset_id::type::ooo_entrance_venetian_blinds		: _Wostream << L"ooo-entrance-venetian-blinds"		; break;
		case preset_id::type::ooo_entrance_box					: _Wostream << L"ooo-entrance-box"					; break;
		case preset_id::type::ooo_entrance_checkerboard			: _Wostream << L"ooo-entrance-checkerboard"			; break;
		case preset_id::type::ooo_entrance_circle				: _Wostream << L"ooo-entrance-circle"				; break;
		case preset_id::type::ooo_entrance_fly_in_slow			: _Wostream << L"ooo-entrance-fly-in-slow"			; break;
		case preset_id::type::ooo_entrance_diamond				: _Wostream << L"ooo-entrance-diamond"				; break;
		case preset_id::type::ooo_entrance_dissolve_in			: _Wostream << L"ooo-entrance-dissolve-in"			; break;
		case preset_id::type::ooo_entrance_fade_in				: _Wostream << L"ooo-entrance-fade-in"				; break;
		case preset_id::type::ooo_entrance_flash_once			: _Wostream << L"ooo-entrance-flash-once"			; break;
		case preset_id::type::ooo_entrance_peek_in				: _Wostream << L"ooo-entrance-peek-in"				; break;
		case preset_id::type::ooo_entrance_plus					: _Wostream << L"ooo-entrance-plus"					; break;
		case preset_id::type::ooo_entrance_random_bars			: _Wostream << L"ooo-entrance-random-bars"			; break;
		case preset_id::type::ooo_entrance_spiral_in			: _Wostream << L"ooo-entrance-spiral-in"			; break;
		case preset_id::type::ooo_entrance_split				: _Wostream << L"ooo-entrance-split"				; break;
		case preset_id::type::ooo_entrance_stretchy				: _Wostream << L"ooo-entrance-stretchy"				; break;
		case preset_id::type::ooo_entrance_diagonal_squares		: _Wostream << L"ooo-entrance-diagonal-squares"		; break;
		case preset_id::type::ooo_entrance_swivel				: _Wostream << L"ooo-entrance-swivel"				; break;
		case preset_id::type::ooo_entrance_wedge				: _Wostream << L"ooo-entrance-wedge"				; break;
		case preset_id::type::ooo_entrance_wheel				: _Wostream << L"ooo-entrance-wheel"				; break;
		case preset_id::type::ooo_entrance_wipe					: _Wostream << L"ooo-entrance-wipe"					; break;
		case preset_id::type::ooo_entrance_zoom					: _Wostream << L"ooo-entrance-zoom"					; break;
		case preset_id::type::ooo_entrance_random				: _Wostream << L"ooo-entrance-random"				; break;
		case preset_id::type::ooo_entrance_boomerang			: _Wostream << L"ooo-entrance-boomerang"			; break;
		case preset_id::type::ooo_entrance_bounce				: _Wostream << L"ooo-entrance-bounce"				; break;
		case preset_id::type::ooo_entrance_colored_lettering	: _Wostream << L"ooo-entrance-colored-lettering"	; break;
		case preset_id::type::ooo_entrance_movie_credits		: _Wostream << L"ooo-entrance-movie-credits"		; break;
		case preset_id::type::ooo_entrance_ease_in				: _Wostream << L"ooo-entrance-ease-in"				; break;
		case preset_id::type::ooo_entrance_float				: _Wostream << L"ooo-entrance-float"				; break;
		case preset_id::type::ooo_entrance_turn_and_grow		: _Wostream << L"ooo-entrance-turn-and-grow"		; break;
		case preset_id::type::ooo_entrance_breaks				: _Wostream << L"ooo-entrance-breaks"				; break;
		case preset_id::type::ooo_entrance_pinwheel				: _Wostream << L"ooo-entrance-pinwheel"				; break;
		case preset_id::type::ooo_entrance_rise_up				: _Wostream << L"ooo-entrance-rise-up"				; break;
		case preset_id::type::ooo_entrance_falling_in			: _Wostream << L"ooo-entrance-falling-in"			; break;
		case preset_id::type::ooo_entrance_thread				: _Wostream << L"ooo-entrance-thread"				; break;
		case preset_id::type::ooo_entrance_unfold				: _Wostream << L"ooo-entrance-unfold"				; break;
		case preset_id::type::ooo_entrance_whip					: _Wostream << L"ooo-entrance-whip"					; break;
		case preset_id::type::ooo_entrance_ascend				: _Wostream << L"ooo-entrance-ascend"				; break;
		case preset_id::type::ooo_entrance_center_revolve		: _Wostream << L"ooo-entrance-center-revolve"		; break;
		case preset_id::type::ooo_entrance_fade_in_and_swivel	: _Wostream << L"ooo-entrance-fade-in-and-swivel"	; break;
		case preset_id::type::ooo_entrance_descend				: _Wostream << L"ooo-entrance-descend"				; break;
		case preset_id::type::ooo_entrance_sling				: _Wostream << L"ooo-entrance-sling"				; break;
		case preset_id::type::ooo_entrance_spin_in				: _Wostream << L"ooo-entrance-spin-in"				; break;
		case preset_id::type::ooo_entrance_compress				: _Wostream << L"ooo-entrance-compress"				; break;
		case preset_id::type::ooo_entrance_magnify				: _Wostream << L"ooo-entrance-magnify"				; break;
		case preset_id::type::ooo_entrance_curve_up				: _Wostream << L"ooo-entrance-curve-up"				; break;
		case preset_id::type::ooo_entrance_fade_in_and_zoom		: _Wostream << L"ooo-entrance-fade-in-and-zoom"		; break;
		case preset_id::type::ooo_entrance_glide				: _Wostream << L"ooo-entrance-glide"				; break;
		case preset_id::type::ooo_entrance_expand				: _Wostream << L"ooo-entrance-expand"				; break;
		case preset_id::type::ooo_entrance_flip					: _Wostream << L"ooo-entrance-flip"					; break;
		case preset_id::type::ooo_entrance_fold					: _Wostream << L"ooo-entrance-fold"					; break;
		case preset_id::type::ooo_emphasis_fill_color			: _Wostream << L"ooo-emphasis-fill-color"			; break;
		case preset_id::type::ooo_emphasis_font					: _Wostream << L"ooo-emphasis-font"					; break;
		case preset_id::type::ooo_emphasis_font_color			: _Wostream << L"ooo-emphasis-font-color"			; break;
		case preset_id::type::ooo_emphasis_font_size			: _Wostream << L"ooo-emphasis-font-size"			; break;
		case preset_id::type::ooo_emphasis_font_style			: _Wostream << L"ooo-emphasis-font-style"			; break;
		case preset_id::type::ooo_emphasis_grow_and_shrink		: _Wostream << L"ooo-emphasis-grow-and-shrink"		; break;
		case preset_id::type::ooo_emphasis_line_color			: _Wostream << L"ooo-emphasis-line-color"			; break;
		case preset_id::type::ooo_emphasis_spin					: _Wostream << L"ooo-emphasis-spin"					; break;
		case preset_id::type::ooo_emphasis_transparency			: _Wostream << L"ooo-emphasis-transparency"			; break;
		case preset_id::type::ooo_emphasis_bold_flash			: _Wostream << L"ooo-emphasis-bold-flash"			; break;
		case preset_id::type::ooo_emphasis_blast				: _Wostream << L"ooo-emphasis-blast"				; break;
		case preset_id::type::ooo_emphasis_bold_reveal			: _Wostream << L"ooo-emphasis-bold-reveal"			; break;
		case preset_id::type::ooo_emphasis_color_over_by_word	: _Wostream << L"ooo-emphasis-color-over-by-word"	; break;
		case preset_id::type::ooo_emphasis_reveal_underline		: _Wostream << L"ooo-emphasis-reveal-underline"		; break;
		case preset_id::type::ooo_emphasis_color_blend			: _Wostream << L"ooo-emphasis-color-blend"			; break;
		case preset_id::type::ooo_emphasis_color_over_by_letter	: _Wostream << L"ooo-emphasis-color-over-by-letter"	; break;
		case preset_id::type::ooo_emphasis_complementary_color	: _Wostream << L"ooo-emphasis-complementary-color"	; break;
		case preset_id::type::ooo_emphasis_complementary_color_2: _Wostream << L"ooo-emphasis-complementary-color-2"; break;
		case preset_id::type::ooo_emphasis_contrasting_color	: _Wostream << L"ooo-emphasis-contrasting-color"	; break;
		case preset_id::type::ooo_emphasis_darken				: _Wostream << L"ooo-emphasis-darken"				; break;
		case preset_id::type::ooo_emphasis_desaturate			: _Wostream << L"ooo-emphasis-desaturate"			; break;
		case preset_id::type::ooo_emphasis_flash_bulb			: _Wostream << L"ooo-emphasis-flash-bulb"			; break;
		case preset_id::type::ooo_emphasis_flicker				: _Wostream << L"ooo-emphasis-flicker"				; break;
		case preset_id::type::ooo_emphasis_grow_with_color		: _Wostream << L"ooo-emphasis-grow-with-color"		; break;
		case preset_id::type::ooo_emphasis_lighten				: _Wostream << L"ooo-emphasis-lighten"				; break;
		case preset_id::type::ooo_emphasis_style_emphasis		: _Wostream << L"ooo-emphasis-style-emphasis"		; break;
		case preset_id::type::ooo_emphasis_teeter				: _Wostream << L"ooo-emphasis-teeter"				; break;
		case preset_id::type::ooo_emphasis_vertical_highlight	: _Wostream << L"ooo-emphasis-vertical-highlight"	; break;
		case preset_id::type::ooo_emphasis_wave					: _Wostream << L"ooo-emphasis-wave"					; break;
		case preset_id::type::ooo_emphasis_blink				: _Wostream << L"ooo-emphasis-blink"				; break;
		case preset_id::type::ooo_emphasis_shimmer				: _Wostream << L"ooo-emphasis-shimmer"				; break;
		case preset_id::type::ooo_exit_disappear				: _Wostream << L"ooo-exit-disappear"				; break;
		case preset_id::type::ooo_exit_fly_out					: _Wostream << L"ooo-exit-fly-out"					; break;
		case preset_id::type::ooo_exit_venetian_blinds			: _Wostream << L"ooo-exit-venetian-blinds"			; break;
		case preset_id::type::ooo_exit_box						: _Wostream << L"ooo-exit-box"						; break;
		case preset_id::type::ooo_exit_checkerboard				: _Wostream << L"ooo-exit-checkerboard"				; break;
		case preset_id::type::ooo_exit_circle					: _Wostream << L"ooo-exit-circle"					; break;
		case preset_id::type::ooo_exit_crawl_out				: _Wostream << L"ooo-exit-crawl-out"				; break;
		case preset_id::type::ooo_exit_diamond					: _Wostream << L"ooo-exit-diamond"					; break;
		case preset_id::type::ooo_exit_dissolve					: _Wostream << L"ooo-exit-dissolve"					; break;
		case preset_id::type::ooo_exit_fade_out					: _Wostream << L"ooo-exit-fade-out"					; break;
		case preset_id::type::ooo_exit_flash_once				: _Wostream << L"ooo-exit-flash-once"				; break;
		case preset_id::type::ooo_exit_peek_out					: _Wostream << L"ooo-exit-peek-out"					; break;
		case preset_id::type::ooo_exit_plus						: _Wostream << L"ooo-exit-plus"						; break;
		case preset_id::type::ooo_exit_random_bars				: _Wostream << L"ooo-exit-random-bars"				; break;
		case preset_id::type::ooo_exit_spiral_out				: _Wostream << L"ooo-exit-spiral-out"				; break;
		case preset_id::type::ooo_exit_split					: _Wostream << L"ooo-exit-split"					; break;
		case preset_id::type::ooo_exit_collapse					: _Wostream << L"ooo-exit-collapse"					; break;
		case preset_id::type::ooo_exit_diagonal_squares			: _Wostream << L"ooo-exit-diagonal-squares"			; break;
		case preset_id::type::ooo_exit_swivel					: _Wostream << L"ooo-exit-swivel"					; break;
		case preset_id::type::ooo_exit_wedge					: _Wostream << L"ooo-exit-wedge"					; break;
		case preset_id::type::ooo_exit_wheel					: _Wostream << L"ooo-exit-wheel"					; break;
		case preset_id::type::ooo_exit_wipe						: _Wostream << L"ooo-exit-wipe"						; break;
		case preset_id::type::ooo_exit_zoom						: _Wostream << L"ooo-exit-zoom"						; break;
		case preset_id::type::ooo_exit_random					: _Wostream << L"ooo-exit-random"					; break;
		case preset_id::type::ooo_exit_boomerang				: _Wostream << L"ooo-exit-boomerang"				; break;
		case preset_id::type::ooo_exit_bounce					: _Wostream << L"ooo-exit-bounce"					; break;
		case preset_id::type::ooo_exit_colored_lettering		: _Wostream << L"ooo-exit-colored-lettering"		; break;
		case preset_id::type::ooo_exit_movie_credits			: _Wostream << L"ooo-exit-movie-credits"			; break;
		case preset_id::type::ooo_exit_ease_out					: _Wostream << L"ooo-exit-ease-out"					; break;
		case preset_id::type::ooo_exit_float					: _Wostream << L"ooo-exit-float"					; break;
		case preset_id::type::ooo_exit_turn_and_grow			: _Wostream << L"ooo-exit-turn-and-grow"			; break;
		case preset_id::type::ooo_exit_breaks					: _Wostream << L"ooo-exit-breaks"					; break;
		case preset_id::type::ooo_exit_pinwheel					: _Wostream << L"ooo-exit-pinwheel"					; break;
		case preset_id::type::ooo_exit_sink_down				: _Wostream << L"ooo-exit-sink-down"				; break;
		case preset_id::type::ooo_exit_swish					: _Wostream << L"ooo-exit-swish"					; break;
		case preset_id::type::ooo_exit_thread					: _Wostream << L"ooo-exit-thread"					; break;
		case preset_id::type::ooo_exit_unfold					: _Wostream << L"ooo-exit-unfold"					; break;
		case preset_id::type::ooo_exit_whip						: _Wostream << L"ooo-exit-whip"						; break;
		case preset_id::type::ooo_exit_descend					: _Wostream << L"ooo-exit-descend"					; break;
		case preset_id::type::ooo_exit_center_revolve			: _Wostream << L"ooo-exit-center-revolve"			; break;
		case preset_id::type::ooo_exit_fade_out_and_swivel		: _Wostream << L"ooo-exit-fade-out-and-swivel"		; break;
		case preset_id::type::ooo_exit_ascend					: _Wostream << L"ooo-exit-ascend"					; break;
		case preset_id::type::ooo_exit_sling					: _Wostream << L"ooo-exit-sling"					; break;
		case preset_id::type::ooo_exit_fade_out_and_zoom		: _Wostream << L"ooo-exit-fade-out-and-zoom"		; break;
		case preset_id::type::ooo_exit_contract					: _Wostream << L"ooo-exit-contract"					; break;
		case preset_id::type::ooo_exit_spin_out					: _Wostream << L"ooo-exit-spin-out"					; break;
		case preset_id::type::ooo_exit_stretchy					: _Wostream << L"ooo-exit-stretchy"					; break;
		case preset_id::type::ooo_exit_magnify					: _Wostream << L"ooo-exit-magnify"					; break;
		case preset_id::type::ooo_exit_curve_down				: _Wostream << L"ooo-exit-curve-down"				; break;
		case preset_id::type::ooo_exit_glide					: _Wostream << L"ooo-exit-glide"					; break;
		case preset_id::type::ooo_exit_flip						: _Wostream << L"ooo-exit-flip"						; break;
		case preset_id::type::ooo_exit_fold						: _Wostream << L"ooo-exit-fold"						; break;
		case preset_id::type::ooo_motionpath_4_point_star		: _Wostream << L"ooo-motionpath-4-point-star"		; break;
		case preset_id::type::ooo_motionpath_5_point_star		: _Wostream << L"ooo-motionpath-5-point-star"		; break;
		case preset_id::type::ooo_motionpath_6_point_star		: _Wostream << L"ooo-motionpath-6-point-star"		; break;
		case preset_id::type::ooo_motionpath_8_point_star		: _Wostream << L"ooo-motionpath-8-point-star"		; break;
		case preset_id::type::ooo_motionpath_circle				: _Wostream << L"ooo-motionpath-circle"				; break;
		case preset_id::type::ooo_motionpath_crescent_moon		: _Wostream << L"ooo-motionpath-crescent-moon"		; break;
		case preset_id::type::ooo_motionpath_diamond			: _Wostream << L"ooo-motionpath-diamond"			; break;
		case preset_id::type::ooo_motionpath_equal_triangle		: _Wostream << L"ooo-motionpath-equal-triangle"		; break;
		case preset_id::type::ooo_motionpath_oval				: _Wostream << L"ooo-motionpath-oval"				; break;
		case preset_id::type::ooo_motionpath_heart				: _Wostream << L"ooo-motionpath-heart"				; break;
		case preset_id::type::ooo_motionpath_hexagon			: _Wostream << L"ooo-motionpath-hexagon"			; break;
		case preset_id::type::ooo_motionpath_octagon			: _Wostream << L"ooo-motionpath-octagon"			; break;
		case preset_id::type::ooo_motionpath_parallelogram		: _Wostream << L"ooo-motionpath-parallelogram"		; break;
		case preset_id::type::ooo_motionpath_pentagon			: _Wostream << L"ooo-motionpath-pentagon"			; break;
		case preset_id::type::ooo_motionpath_right_triangle		: _Wostream << L"ooo-motionpath-right-triangle"		; break;
		case preset_id::type::ooo_motionpath_square				: _Wostream << L"ooo-motionpath-square"				; break;
		case preset_id::type::ooo_motionpath_teardrop			: _Wostream << L"ooo-motionpath-teardrop"			; break;
		case preset_id::type::ooo_motionpath_trapezoid			: _Wostream << L"ooo-motionpath-trapezoid"			; break;
		case preset_id::type::ooo_motionpath_arc_down			: _Wostream << L"ooo-motionpath-arc-down"			; break;
		case preset_id::type::ooo_motionpath_arc_left			: _Wostream << L"ooo-motionpath-arc-left"			; break;
		case preset_id::type::ooo_motionpath_arc_right			: _Wostream << L"ooo-motionpath-arc-right"			; break;
		case preset_id::type::ooo_motionpath_arc_up				: _Wostream << L"ooo-motionpath-arc-up"				; break;
		case preset_id::type::ooo_motionpath_bounce_left		: _Wostream << L"ooo-motionpath-bounce-left"		; break;
		case preset_id::type::ooo_motionpath_bounce_right		: _Wostream << L"ooo-motionpath-bounce-right"		; break;
		case preset_id::type::ooo_motionpath_curvy_left			: _Wostream << L"ooo-motionpath-curvy-left"			; break;
		case preset_id::type::ooo_motionpath_curvy_right		: _Wostream << L"ooo-motionpath-curvy-right"		; break;
		case preset_id::type::ooo_motionpath_decaying_wave		: _Wostream << L"ooo-motionpath-decaying-wave"		; break;
		case preset_id::type::ooo_motionpath_diagonal_down_right: _Wostream << L"ooo-motionpath-diagonal-down-right"; break;
		case preset_id::type::ooo_motionpath_diagonal_up_right	: _Wostream << L"ooo-motionpath-diagonal-up-right"	; break;
		case preset_id::type::ooo_motionpath_down				: _Wostream << L"ooo-motionpath-down"				; break;
		case preset_id::type::ooo_motionpath_funnel				: _Wostream << L"ooo-motionpath-funnel"				; break;
		case preset_id::type::ooo_motionpath_spring				: _Wostream << L"ooo-motionpath-spring"				; break;
		case preset_id::type::ooo_motionpath_stairs_down		: _Wostream << L"ooo-motionpath-stairs-down"		; break;
		case preset_id::type::ooo_motionpath_turn_down			: _Wostream << L"ooo-motionpath-turn-down"			; break;
		case preset_id::type::ooo_motionpath_turn_down_right	: _Wostream << L"ooo-motionpath-turn-down-right"	; break;
		case preset_id::type::ooo_motionpath_turn_up			: _Wostream << L"ooo-motionpath-turn-up"			; break;
		case preset_id::type::ooo_motionpath_turn_up_right		: _Wostream << L"ooo-motionpath-turn-up-right"		; break;
		case preset_id::type::ooo_motionpath_up					: _Wostream << L"ooo-motionpath-up"					; break;
		case preset_id::type::ooo_motionpath_wave				: _Wostream << L"ooo-motionpath-wave"				; break;
		case preset_id::type::ooo_motionpath_zigzag				: _Wostream << L"ooo-motionpath-zigzag"				; break;
		case preset_id::type::ooo_motionpath_bean				: _Wostream << L"ooo-motionpath-bean"				; break;
		case preset_id::type::ooo_motionpath_buzz_saw			: _Wostream << L"ooo-motionpath-buzz-saw"			; break;
		case preset_id::type::ooo_motionpath_curved_square		: _Wostream << L"ooo-motionpath-curved-square"		; break;
		case preset_id::type::ooo_motionpath_curved_x			: _Wostream << L"ooo-motionpath-curved-x"			; break;
		case preset_id::type::ooo_motionpath_curvy_star			: _Wostream << L"ooo-motionpath-curvy-star"			; break;
		case preset_id::type::ooo_motionpath_figure_8_four		: _Wostream << L"ooo-motionpath-figure-8-four"		; break;
		case preset_id::type::ooo_motionpath_horizontal_figure_8: _Wostream << L"ooo-motionpath-horizontal-figure-8"; break;
		case preset_id::type::ooo_motionpath_inverted_square	: _Wostream << L"ooo-motionpath-inverted-square"	; break;
		case preset_id::type::ooo_motionpath_inverted_triangle	: _Wostream << L"ooo-motionpath-inverted-triangle"	; break;
		case preset_id::type::ooo_motionpath_loop_de_loop		: _Wostream << L"ooo-motionpath-loop-de-loop"		; break;
		case preset_id::type::ooo_motionpath_neutron			: _Wostream << L"ooo-motionpath-neutron"			; break;
		case preset_id::type::ooo_motionpath_peanut				: _Wostream << L"ooo-motionpath-peanut"				; break;
		case preset_id::type::ooo_motionpath_clover				: _Wostream << L"ooo-motionpath-clover"				; break;
		case preset_id::type::ooo_motionpath_pointy_star		: _Wostream << L"ooo-motionpath-pointy-star"		; break;
		case preset_id::type::ooo_motionpath_swoosh				: _Wostream << L"ooo-motionpath-swoosh"				; break;
		case preset_id::type::ooo_motionpath_vertical_figure_8	: _Wostream << L"ooo-motionpath-vertical-figure-8"	; break;
		case preset_id::type::ooo_motionpath_left				: _Wostream << L"ooo-motionpath-left"				; break;
		case preset_id::type::ooo_motionpath_right				: _Wostream << L"ooo-motionpath-right"				; break;
		case preset_id::type::ooo_motionpath_spiral_left		: _Wostream << L"ooo-motionpath-spiral-left"		; break;
		case preset_id::type::ooo_motionpath_spiral_right		: _Wostream << L"ooo-motionpath-spiral-right"		; break;
		case preset_id::type::ooo_motionpath_sine_wave			: _Wostream << L"ooo-motionpath-sine-wave"			; break;
		case preset_id::type::ooo_motionpath_s_curve_1			: _Wostream << L"ooo-motionpath-s-curve-1"			; break;
		case preset_id::type::ooo_motionpath_s_curve_2			: _Wostream << L"ooo-motionpath-s-curve-2"			; break;
		case preset_id::type::ooo_motionpath_heartbeat			: _Wostream << L"ooo-motionpath-heartbeat"			; break;
		}

		return _Wostream;
	}

} // namespace odf_types
} // namespace cpdoccore