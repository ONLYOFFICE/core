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

	preset_id preset_id::parse(const std::wstring& Str)
	{
		std::wstring tmp = Str;
		boost::algorithm::to_lower(tmp);

		if (boost::algorithm::starts_with(tmp, L"ooo-entrance-"))
		{
			std::wstring preset = tmp.substr(std::wstring(L"ooo-entrance-").size());

			if (preset == L"appear") 					return preset_id::type::ooo_entrance_appear;
			else if (preset == L"fly-in")				return preset_id::type::ooo_entrance_fly_in;
			else if (preset == L"venetian-blinds")		return preset_id::type::ooo_entrance_venetian_blinds;
			else if (preset == L"box")					return preset_id::type::ooo_entrance_box;
			else if (preset == L"checkerboard")			return preset_id::type::ooo_entrance_checkerboard;
			else if (preset == L"circle")				return preset_id::type::ooo_entrance_circle;
			else if (preset == L"fly-in-slow")			return preset_id::type::ooo_entrance_fly_in_slow;
			else if (preset == L"diamond")				return preset_id::type::ooo_entrance_diamond;
			else if (preset == L"dissolve-in")			return preset_id::type::ooo_entrance_dissolve_in;
			else if (preset == L"fade-in")				return preset_id::type::ooo_entrance_fade_in;
			else if (preset == L"flash-once")			return preset_id::type::ooo_entrance_flash_once;
			else if (preset == L"peek-in")				return preset_id::type::ooo_entrance_peek_in;
			else if (preset == L"plus")					return preset_id::type::ooo_entrance_plus;
			else if (preset == L"random-bars")			return preset_id::type::ooo_entrance_random_bars;
			else if (preset == L"spiral-in")			return preset_id::type::ooo_entrance_spiral_in;
			else if (preset == L"split")				return preset_id::type::ooo_entrance_split;
			else if (preset == L"stretchy")				return preset_id::type::ooo_entrance_stretchy;
			else if (preset == L"diagonal-squares")		return preset_id::type::ooo_entrance_diagonal_squares;
			else if (preset == L"swivel")				return preset_id::type::ooo_entrance_swivel;
			else if (preset == L"wedge")				return preset_id::type::ooo_entrance_wedge;
			else if (preset == L"wheel")				return preset_id::type::ooo_entrance_wheel;
			else if (preset == L"wipe")					return preset_id::type::ooo_entrance_wipe;
			else if (preset == L"zoom")					return preset_id::type::ooo_entrance_zoom;
			else if (preset == L"random")				return preset_id::type::ooo_entrance_random;
			else if (preset == L"boomerang")			return preset_id::type::ooo_entrance_boomerang;
			else if (preset == L"bounce")				return preset_id::type::ooo_entrance_bounce;
			else if (preset == L"colored-lettering")	return preset_id::type::ooo_entrance_colored_lettering;
			else if (preset == L"movie-credits")		return preset_id::type::ooo_entrance_movie_credits;
			else if (preset == L"ease-in")				return preset_id::type::ooo_entrance_ease_in;
			else if (preset == L"float")				return preset_id::type::ooo_entrance_float;
			else if (preset == L"turn-and-grow")		return preset_id::type::ooo_entrance_turn_and_grow;
			else if (preset == L"breaks")				return preset_id::type::ooo_entrance_breaks;
			else if (preset == L"pinwheel")				return preset_id::type::ooo_entrance_pinwheel;
			else if (preset == L"rise-up")				return preset_id::type::ooo_entrance_rise_up;
			else if (preset == L"falling-in")			return preset_id::type::ooo_entrance_falling_in;
			else if (preset == L"thread")				return preset_id::type::ooo_entrance_thread;
			else if (preset == L"unfold")				return preset_id::type::ooo_entrance_unfold;
			else if (preset == L"whip")					return preset_id::type::ooo_entrance_whip;
			else if (preset == L"ascend")				return preset_id::type::ooo_entrance_ascend;
			else if (preset == L"center-revolve")		return preset_id::type::ooo_entrance_center_revolve;
			else if (preset == L"fade-in-and-swivel")	return preset_id::type::ooo_entrance_fade_in_and_swivel;
			else if (preset == L"descend")				return preset_id::type::ooo_entrance_descend;
			else if (preset == L"sling")				return preset_id::type::ooo_entrance_sling;
			else if (preset == L"spin-in")				return preset_id::type::ooo_entrance_spin_in;
			else if (preset == L"compress")				return preset_id::type::ooo_entrance_compress;
			else if (preset == L"magnify")				return preset_id::type::ooo_entrance_magnify;
			else if (preset == L"curve-up")				return preset_id::type::ooo_entrance_curve_up;
			else if (preset == L"fade-in-and-zoom")		return preset_id::type::ooo_entrance_fade_in_and_zoom;
			else if (preset == L"glide")				return preset_id::type::ooo_entrance_glide;
			else if (preset == L"expand")				return preset_id::type::ooo_entrance_expand;
			else if (preset == L"flip")					return preset_id::type::ooo_entrance_flip;
			else if (preset == L"fold")					return preset_id::type::ooo_entrance_fold;
		}
		else if (boost::algorithm::starts_with(tmp, L"ooo-emphasis-"))
		{
			std::wstring preset = tmp.substr(std::wstring(L"ooo-emphasis-").size());

			if (preset == L"fill-color")						return preset_id::type::ooo_emphasis_fill_color;
			else if (preset == L"font")							return preset_id::type::ooo_emphasis_font;
			else if (preset == L"font-size")					return preset_id::type::ooo_emphasis_font_size;
			else if (preset == L"font-style")					return preset_id::type::ooo_emphasis_font_style;
			else if (preset == L"grow-and-shrink")				return preset_id::type::ooo_emphasis_grow_and_shrink;
			else if (preset == L"line-color")					return preset_id::type::ooo_emphasis_line_color;
			else if (preset == L"spin")							return preset_id::type::ooo_emphasis_spin;
			else if (preset == L"transparency")					return preset_id::type::ooo_emphasis_transparency;
			else if (preset == L"bold-flash")					return preset_id::type::ooo_emphasis_bold_flash;
			else if (preset == L"blast")						return preset_id::type::ooo_emphasis_blast;
			else if (preset == L"bold-reveal")					return preset_id::type::ooo_emphasis_bold_reveal;
			else if (preset == L"color-over-by-word")			return preset_id::type::ooo_emphasis_color_over_by_word;
			else if (preset == L"reveal-underline")				return preset_id::type::ooo_emphasis_reveal_underline;
			else if (preset == L"color-blend")					return preset_id::type::ooo_emphasis_color_blend;
			else if (preset == L"color-over-by-letter")			return preset_id::type::ooo_emphasis_color_over_by_letter;
			else if (preset == L"complementary-color")			return preset_id::type::ooo_emphasis_complementary_color;
			else if (preset == L"complementary-color-2")		return preset_id::type::ooo_emphasis_complementary_color_2;
			else if (preset == L"contrasting-color")			return preset_id::type::ooo_emphasis_contrasting_color;
			else if (preset == L"darken")						return preset_id::type::ooo_emphasis_darken;
			else if (preset == L"desaturate")					return preset_id::type::ooo_emphasis_desaturate;
			else if (preset == L"flash-bulb")					return preset_id::type::ooo_emphasis_flash_bulb;
			else if (preset == L"flicker")						return preset_id::type::ooo_emphasis_flicker;
			else if (preset == L"grow-with-color")				return preset_id::type::ooo_emphasis_grow_with_color;
			else if (preset == L"lighten")						return preset_id::type::ooo_emphasis_lighten;
			else if (preset == L"style-emphasis")				return preset_id::type::ooo_emphasis_style_emphasis;
			else if (preset == L"teeter")						return preset_id::type::ooo_emphasis_teeter;
			else if (preset == L"vertical-highlight")			return preset_id::type::ooo_emphasis_vertical_highlight;
			else if (preset == L"wave")							return preset_id::type::ooo_emphasis_wave;
			else if (preset == L"blink")						return preset_id::type::ooo_emphasis_blink;
			else if (preset == L"shimmer")						return preset_id::type::ooo_emphasis_shimmer;
		}
		else if (boost::algorithm::starts_with(tmp, L"ooo-exit-"))
		{
			std::wstring preset = tmp.substr(std::wstring(L"ooo-exit-").size());

			if (preset == L"disappear")						return preset_id::type::ooo_exit_disappear;
			else if (preset == L"fly-out")					return preset_id::type::ooo_exit_fly_out;
			else if (preset == L"venetian-blinds")			return preset_id::type::ooo_exit_venetian_blinds;
			else if (preset == L"box")						return preset_id::type::ooo_exit_box;
			else if (preset == L"checkerboard")				return preset_id::type::ooo_exit_checkerboard;
			else if (preset == L"circle")					return preset_id::type::ooo_exit_circle;
			else if (preset == L"crawl-out")				return preset_id::type::ooo_exit_crawl_out;
			else if (preset == L"diamond")					return preset_id::type::ooo_exit_diamond;
			else if (preset == L"dissolve")					return preset_id::type::ooo_exit_dissolve;
			else if (preset == L"fade-out")					return preset_id::type::ooo_exit_fade_out;
			else if (preset == L"flash-once")				return preset_id::type::ooo_exit_flash_once;
			else if (preset == L"peek-out")					return preset_id::type::ooo_exit_peek_out;
			else if (preset == L"plus")						return preset_id::type::ooo_exit_plus;
			else if (preset == L"random-bars")				return preset_id::type::ooo_exit_random_bars;
			else if (preset == L"spiral-out")				return preset_id::type::ooo_exit_spiral_out;
			else if (preset == L"split")					return preset_id::type::ooo_exit_split;
			else if (preset == L"collapse")					return preset_id::type::ooo_exit_collapse;
			else if (preset == L"diagonal-squares")			return preset_id::type::ooo_exit_diagonal_squares;
			else if (preset == L"swivel")					return preset_id::type::ooo_exit_swivel;
			else if (preset == L"wedge")					return preset_id::type::ooo_exit_wedge;
			else if (preset == L"wheel")					return preset_id::type::ooo_exit_wheel;
			else if (preset == L"wipe")						return preset_id::type::ooo_exit_wipe;
			else if (preset == L"zoom")						return preset_id::type::ooo_exit_zoom;
			else if (preset == L"random")					return preset_id::type::ooo_exit_random;
			else if (preset == L"boomerang")				return preset_id::type::ooo_exit_boomerang;
			else if (preset == L"bounce")					return preset_id::type::ooo_exit_bounce;
			else if (preset == L"colored-lettering")		return preset_id::type::ooo_exit_colored_lettering;
			else if (preset == L"movie-credits")			return preset_id::type::ooo_exit_movie_credits;
			else if (preset == L"ease-out")					return preset_id::type::ooo_exit_ease_out;
			else if (preset == L"float")					return preset_id::type::ooo_exit_float;
			else if (preset == L"turn-and-grow")			return preset_id::type::ooo_exit_turn_and_grow;
			else if (preset == L"breaks")					return preset_id::type::ooo_exit_breaks;
			else if (preset == L"pinwheel")					return preset_id::type::ooo_exit_pinwheel;
			else if (preset == L"sink-down")				return preset_id::type::ooo_exit_sink_down;
			else if (preset == L"swish")					return preset_id::type::ooo_exit_swish;
			else if (preset == L"thread")					return preset_id::type::ooo_exit_thread;
			else if (preset == L"unfold")					return preset_id::type::ooo_exit_unfold;
			else if (preset == L"whip")						return preset_id::type::ooo_exit_whip;
			else if (preset == L"descend")					return preset_id::type::ooo_exit_descend;
			else if (preset == L"center-revolve")			return preset_id::type::ooo_exit_center_revolve;
			else if (preset == L"fade-out-and-swivel")		return preset_id::type::ooo_exit_fade_out_and_swivel;
			else if (preset == L"ascend")					return preset_id::type::ooo_exit_ascend;
			else if (preset == L"sling")					return preset_id::type::ooo_exit_sling;
			else if (preset == L"fade-out-and-zoom")		return preset_id::type::ooo_exit_fade_out_and_zoom;
			else if (preset == L"contract")					return preset_id::type::ooo_exit_contract;
			else if (preset == L"spin-out")					return preset_id::type::ooo_exit_spin_out;
			else if (preset == L"stretchy")					return preset_id::type::ooo_exit_stretchy;
			else if (preset == L"magnify")					return preset_id::type::ooo_exit_magnify;
			else if (preset == L"curve-down")				return preset_id::type::ooo_exit_curve_down;
			else if (preset == L"glide")					return preset_id::type::ooo_exit_glide;
			else if (preset == L"flip")						return preset_id::type::ooo_exit_flip;
			else if (preset == L"fold")						return preset_id::type::ooo_exit_fold;
		}
		else if (boost::algorithm::starts_with(tmp, L"ooo-motionpath-"))
		{
			std::wstring preset = tmp.substr(std::wstring(L"ooo-motionpath-").size());

			if (preset == L"4-point-star")					return preset_id::type::ooo_motionpath_4_point_star;
			else if (preset == L"5-point-star")				return preset_id::type::ooo_motionpath_5_point_star;
			else if (preset == L"6-point-star")				return preset_id::type::ooo_motionpath_6_point_star;
			else if (preset == L"8-point-star")				return preset_id::type::ooo_motionpath_8_point_star;
			else if (preset == L"circle")					return preset_id::type::ooo_motionpath_circle;
			else if (preset == L"crescent-moon")			return preset_id::type::ooo_motionpath_crescent_moon;
			else if (preset == L"diamond")					return preset_id::type::ooo_motionpath_diamond;
			else if (preset == L"equal-triangle")			return preset_id::type::ooo_motionpath_equal_triangle;
			else if (preset == L"oval")						return preset_id::type::ooo_motionpath_oval;
			else if (preset == L"heart")					return preset_id::type::ooo_motionpath_heart;
			else if (preset == L"hexagon")					return preset_id::type::ooo_motionpath_hexagon;
			else if (preset == L"octagon")					return preset_id::type::ooo_motionpath_octagon;
			else if (preset == L"parallelogram")			return preset_id::type::ooo_motionpath_parallelogram;
			else if (preset == L"pentagon")					return preset_id::type::ooo_motionpath_pentagon;
			else if (preset == L"right-triangle")			return preset_id::type::ooo_motionpath_right_triangle;
			else if (preset == L"square")					return preset_id::type::ooo_motionpath_square;
			else if (preset == L"teardrop")					return preset_id::type::ooo_motionpath_teardrop;
			else if (preset == L"trapezoid")				return preset_id::type::ooo_motionpath_trapezoid;
			else if (preset == L"arc-down")					return preset_id::type::ooo_motionpath_arc_down;
			else if (preset == L"arc-left")					return preset_id::type::ooo_motionpath_arc_left;
			else if (preset == L"arc-right")				return preset_id::type::ooo_motionpath_arc_right;
			else if (preset == L"arc-up")					return preset_id::type::ooo_motionpath_arc_up;
			else if (preset == L"bounce-left")				return preset_id::type::ooo_motionpath_bounce_left;
			else if (preset == L"bounce-right")				return preset_id::type::ooo_motionpath_bounce_right;
			else if (preset == L"curvy-left")				return preset_id::type::ooo_motionpath_curvy_left;
			else if (preset == L"curvy-right")				return preset_id::type::ooo_motionpath_curvy_right;
			else if (preset == L"decaying-wave")			return preset_id::type::ooo_motionpath_decaying_wave;
			else if (preset == L"diagonal-down-right")		return preset_id::type::ooo_motionpath_diagonal_down_right;
			else if (preset == L"diagonal-up-right")		return preset_id::type::ooo_motionpath_diagonal_up_right;
			else if (preset == L"down")						return preset_id::type::ooo_motionpath_down;
			else if (preset == L"funnel")					return preset_id::type::ooo_motionpath_funnel;
			else if (preset == L"spring")					return preset_id::type::ooo_motionpath_spring;
			else if (preset == L"stairs-down")				return preset_id::type::ooo_motionpath_stairs_down;
			else if (preset == L"turn-down")				return preset_id::type::ooo_motionpath_turn_down;
			else if (preset == L"turn-down-right")			return preset_id::type::ooo_motionpath_turn_down_right;
			else if (preset == L"turn-up")					return preset_id::type::ooo_motionpath_turn_up;
			else if (preset == L"turn-up-right")			return preset_id::type::ooo_motionpath_turn_up_right;
			else if (preset == L"up")						return preset_id::type::ooo_motionpath_up;
			else if (preset == L"wave")						return preset_id::type::ooo_motionpath_wave;
			else if (preset == L"zigzag")					return preset_id::type::ooo_motionpath_zigzag;
			else if (preset == L"bean")						return preset_id::type::ooo_motionpath_bean;
			else if (preset == L"buzz-saw")					return preset_id::type::ooo_motionpath_buzz_saw;
			else if (preset == L"curved-square")			return preset_id::type::ooo_motionpath_curved_square;
			else if (preset == L"curved-x")					return preset_id::type::ooo_motionpath_curved_x;
			else if (preset == L"curvy-star")				return preset_id::type::ooo_motionpath_curvy_star;
			else if (preset == L"figure-8-four")			return preset_id::type::ooo_motionpath_figure_8_four;
			else if (preset == L"horizontal-figure-8")		return preset_id::type::ooo_motionpath_horizontal_figure_8;
			else if (preset == L"inverted-square")			return preset_id::type::ooo_motionpath_inverted_square;
			else if (preset == L"inverted-triangle")		return preset_id::type::ooo_motionpath_inverted_triangle;
			else if (preset == L"loop-de-loop")				return preset_id::type::ooo_motionpath_loop_de_loop;
			else if (preset == L"neutron")					return preset_id::type::ooo_motionpath_neutron;
			else if (preset == L"peanut")					return preset_id::type::ooo_motionpath_peanut;
			else if (preset == L"clover")					return preset_id::type::ooo_motionpath_clover;
			else if (preset == L"pointy-star")				return preset_id::type::ooo_motionpath_pointy_star;
			else if (preset == L"swoosh")					return preset_id::type::ooo_motionpath_swoosh;
			else if (preset == L"vertical-figure-8")		return preset_id::type::ooo_motionpath_vertical_figure_8;
			else if (preset == L"left")						return preset_id::type::ooo_motionpath_left;
			else if (preset == L"right")					return preset_id::type::ooo_motionpath_right;
			else if (preset == L"spiral-left")				return preset_id::type::ooo_motionpath_spiral_left;
			else if (preset == L"spiral-right")				return preset_id::type::ooo_motionpath_spiral_right;
			else if (preset == L"sine-wave")				return preset_id::type::ooo_motionpath_sine_wave;
			else if (preset == L"s-curve-1")				return preset_id::type::ooo_motionpath_s_curve_1;
			else if (preset == L"s-curve-2")				return preset_id::type::ooo_motionpath_s_curve_2;
			else if (preset == L"heartbeat")				return preset_id::type::ooo_motionpath_heartbeat;
		}
		else if (tmp == L"libo-motionpath-curve")			return preset_id::type::libo_motionpath_curve;
		else if (tmp == L"libo-motionpath-polygon")			return preset_id::type::libo_motionpath_polygon;
		else if (tmp == L"libo-motionpath-freeform-line")	return preset_id::type::libo_motionpath_freeform_line;
		

		return preset_id::type::none;
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
		case preset_id::type::libo_motionpath_curve				: _Wostream << L"libo-motionpath-curve"				; break;
		case preset_id::type::libo_motionpath_polygon			: _Wostream << L"libo-motionpath-polygon"			; break;
		case preset_id::type::libo_motionpath_freeform_line		: _Wostream << L"libo-motionpath-freeform-line"		; break;
		}

		return _Wostream;
	}

} // namespace odf_types
} // namespace cpdoccore