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

#include "anim_elements.h"

#include "serialize_elements.h"
#include "odfcontext.h"

#include "odf_document.h"

#include "draw_common.h"

#include "../Converter/pptx_animation_context.h"
#include "svg_parser.h"
#include "../Converter/oox_drawing.h"

#include <xml/xmlchar.h>
#include <xml/simple_xml_writer.h>
#include <boost/algorithm/string.hpp>
#include <unordered_map>
#include <vector>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

struct preset_id_maping
{
	odf_types::preset_id::type	ODF_PresetID;
	int							OOX_PresetID;
};

// TODO: replace with unordered_map??
static const preset_id_maping g_preset_id_map[] = {
	{ preset_id::type::ooo_entrance_appear				, 1 },
	{ preset_id::type::ooo_entrance_fly_in				, 2 },
	{ preset_id::type::ooo_entrance_venetian_blinds		, 3 },
	{ preset_id::type::ooo_entrance_box					, 4 },
	{ preset_id::type::ooo_entrance_checkerboard		, 5 },
	{ preset_id::type::ooo_entrance_circle				, 6 },
	{ preset_id::type::ooo_entrance_fly_in_slow			, 7 },
	{ preset_id::type::ooo_entrance_diamond				, 8 },
	{ preset_id::type::ooo_entrance_dissolve_in			, 9 },
	{ preset_id::type::ooo_entrance_fade_in				, 10 },
	{ preset_id::type::ooo_entrance_flash_once			, 11 },
	{ preset_id::type::ooo_entrance_peek_in				, 12 },
	{ preset_id::type::ooo_entrance_plus				, 13 },
	{ preset_id::type::ooo_entrance_random_bars			, 14 },
	{ preset_id::type::ooo_entrance_spiral_in			, 15 },
	{ preset_id::type::ooo_entrance_split				, 16 },
	{ preset_id::type::ooo_entrance_stretchy			, 17 },
	{ preset_id::type::ooo_entrance_diagonal_squares	, 18 },
	{ preset_id::type::ooo_entrance_swivel				, 19 },
	{ preset_id::type::ooo_entrance_wedge				, 20 },
	{ preset_id::type::ooo_entrance_wheel				, 21 },
	{ preset_id::type::ooo_entrance_wipe				, 22 },
	{ preset_id::type::ooo_entrance_zoom				, 23 },
	{ preset_id::type::ooo_entrance_random				, 24 },
	{ preset_id::type::ooo_entrance_boomerang			, 25 },
	{ preset_id::type::ooo_entrance_bounce				, 26 },
	{ preset_id::type::ooo_entrance_colored_lettering	, 27 },
	{ preset_id::type::ooo_entrance_movie_credits		, 28 },
	{ preset_id::type::ooo_entrance_ease_in				, 29 },
	{ preset_id::type::ooo_entrance_float				, 30 },
	{ preset_id::type::ooo_entrance_turn_and_grow		, 31 },
	{ preset_id::type::ooo_entrance_breaks				, 34 },
	{ preset_id::type::ooo_entrance_pinwheel			, 35 },
	{ preset_id::type::ooo_entrance_rise_up				, 37 },
	{ preset_id::type::ooo_entrance_falling_in			, 38 },
	{ preset_id::type::ooo_entrance_thread				, 39 },
	{ preset_id::type::ooo_entrance_unfold				, 40 },
	{ preset_id::type::ooo_entrance_whip				, 41 },
	{ preset_id::type::ooo_entrance_ascend				, 42 },
	{ preset_id::type::ooo_entrance_center_revolve		, 43 },
	{ preset_id::type::ooo_entrance_fade_in_and_swivel	, 45 },
	{ preset_id::type::ooo_entrance_descend				, 47 },
	{ preset_id::type::ooo_entrance_sling				, 48 },
	{ preset_id::type::ooo_entrance_spin_in				, 49 },
	{ preset_id::type::ooo_entrance_compress			, 50 },
	{ preset_id::type::ooo_entrance_magnify				, 51 },
	{ preset_id::type::ooo_entrance_curve_up			, 52 },
	{ preset_id::type::ooo_entrance_fade_in_and_zoom	, 53 },
	{ preset_id::type::ooo_entrance_glide				, 54 },
	{ preset_id::type::ooo_entrance_expand				, 55 },
	{ preset_id::type::ooo_entrance_flip				, 56 },
	{ preset_id::type::ooo_entrance_fold				, 58 },

	{ preset_id::type::ooo_emphasis_fill_color			, 1 },
	{ preset_id::type::ooo_emphasis_font				, 2 },
	{ preset_id::type::ooo_emphasis_font_color			, 3 },
	{ preset_id::type::ooo_emphasis_font_size			, 4 },
	{ preset_id::type::ooo_emphasis_font_style			, 5 },
	{ preset_id::type::ooo_emphasis_grow_and_shrink		, 6 },
	{ preset_id::type::ooo_emphasis_line_color			, 7 },
	{ preset_id::type::ooo_emphasis_spin				, 8 },
	{ preset_id::type::ooo_emphasis_transparency		, 9 },
	{ preset_id::type::ooo_emphasis_bold_flash			, 10 },
	{ preset_id::type::ooo_emphasis_blast				, 14 },
	{ preset_id::type::ooo_emphasis_bold_reveal			, 15 },
	{ preset_id::type::ooo_emphasis_color_over_by_word	, 16 },
	{ preset_id::type::ooo_emphasis_reveal_underline	, 18 },
	{ preset_id::type::ooo_emphasis_color_blend			, 19 },
	{ preset_id::type::ooo_emphasis_color_over_by_letter, 20 },
	{ preset_id::type::ooo_emphasis_complementary_color	, 21 },
	{ preset_id::type::ooo_emphasis_complementary_color_2, 22 },
	{ preset_id::type::ooo_emphasis_contrasting_color	, 23 },
	{ preset_id::type::ooo_emphasis_darken				, 24 },
	{ preset_id::type::ooo_emphasis_desaturate			, 25 },
	{ preset_id::type::ooo_emphasis_flash_bulb			, 26 },
	{ preset_id::type::ooo_emphasis_flicker				, 27 },
	{ preset_id::type::ooo_emphasis_grow_with_color		, 28 },
	{ preset_id::type::ooo_emphasis_lighten				, 30 },
	{ preset_id::type::ooo_emphasis_style_emphasis		, 31 },
	{ preset_id::type::ooo_emphasis_teeter				, 32 },
	{ preset_id::type::ooo_emphasis_vertical_highlight	, 33 },
	{ preset_id::type::ooo_emphasis_wave				, 34 },
	{ preset_id::type::ooo_emphasis_blink				, 35 },
	{ preset_id::type::ooo_emphasis_shimmer				, 36 },

	{ preset_id::type::ooo_exit_disappear				, 1 },
	{ preset_id::type::ooo_exit_fly_out					, 2 },
	{ preset_id::type::ooo_exit_venetian_blinds			, 3 },
	{ preset_id::type::ooo_exit_box						, 4 },
	{ preset_id::type::ooo_exit_checkerboard			, 5 },
	{ preset_id::type::ooo_exit_circle					, 6 },
	{ preset_id::type::ooo_exit_crawl_out				, 7 },
	{ preset_id::type::ooo_exit_diamond					, 8 },
	{ preset_id::type::ooo_exit_dissolve				, 9 },
	{ preset_id::type::ooo_exit_fade_out				, 10 },
	{ preset_id::type::ooo_exit_flash_once				, 11 },
	{ preset_id::type::ooo_exit_peek_out				, 12 },
	{ preset_id::type::ooo_exit_plus					, 13 },
	{ preset_id::type::ooo_exit_random_bars				, 14 },
	{ preset_id::type::ooo_exit_spiral_out				, 15 },
	{ preset_id::type::ooo_exit_split					, 16 },
	{ preset_id::type::ooo_exit_collapse				, 17 },
	{ preset_id::type::ooo_exit_diagonal_squares		, 18 },
	{ preset_id::type::ooo_exit_swivel					, 19 },
	{ preset_id::type::ooo_exit_wedge					, 20 },
	{ preset_id::type::ooo_exit_wheel					, 21 },
	{ preset_id::type::ooo_exit_wipe					, 22 },
	{ preset_id::type::ooo_exit_zoom					, 23 },
	{ preset_id::type::ooo_exit_random					, 24 },
	{ preset_id::type::ooo_exit_boomerang				, 25 },
	{ preset_id::type::ooo_exit_bounce					, 26 },
	{ preset_id::type::ooo_exit_colored_lettering		, 27 },
	{ preset_id::type::ooo_exit_movie_credits			, 28 },
	{ preset_id::type::ooo_exit_ease_out				, 29 },
	{ preset_id::type::ooo_exit_float					, 30 },
	{ preset_id::type::ooo_exit_turn_and_grow			, 31 },
	{ preset_id::type::ooo_exit_breaks					, 34 },
	{ preset_id::type::ooo_exit_pinwheel				, 35 },
	{ preset_id::type::ooo_exit_sink_down				, 37 },
	{ preset_id::type::ooo_exit_swish					, 38 },
	{ preset_id::type::ooo_exit_thread					, 39 },
	{ preset_id::type::ooo_exit_unfold					, 40 },
	{ preset_id::type::ooo_exit_whip					, 41 },
	{ preset_id::type::ooo_exit_descend					, 42 },
	{ preset_id::type::ooo_exit_center_revolve			, 43 },
	{ preset_id::type::ooo_exit_fade_out_and_swivel		, 45 },
	{ preset_id::type::ooo_exit_ascend					, 47 },
	{ preset_id::type::ooo_exit_sling					, 48 },
	{ preset_id::type::ooo_exit_fade_out_and_zoom		, 53 },
	{ preset_id::type::ooo_exit_contract				, 55 },
	{ preset_id::type::ooo_exit_spin_out				, 49 },
	{ preset_id::type::ooo_exit_stretchy				, 50 },
	{ preset_id::type::ooo_exit_magnify					, 51 },
	{ preset_id::type::ooo_exit_curve_down				, 52 },
	{ preset_id::type::ooo_exit_glide					, 54 },
	{ preset_id::type::ooo_exit_flip					, 56 },
	{ preset_id::type::ooo_exit_fold					, 58 },

	{ preset_id::type::ooo_motionpath_4_point_star		, 16 },
	{ preset_id::type::ooo_motionpath_5_point_star		, 5 },
	{ preset_id::type::ooo_motionpath_6_point_star		, 11 },
	{ preset_id::type::ooo_motionpath_8_point_star		, 17 },
	{ preset_id::type::ooo_motionpath_circle			, 1 },
	{ preset_id::type::ooo_motionpath_crescent_moon		, 6 },
	{ preset_id::type::ooo_motionpath_diamond			, 3 },
	{ preset_id::type::ooo_motionpath_equal_triangle	, 13 },
	{ preset_id::type::ooo_motionpath_oval				, 12 },
	{ preset_id::type::ooo_motionpath_heart				, 9 },
	{ preset_id::type::ooo_motionpath_hexagon			, 4 },
	{ preset_id::type::ooo_motionpath_octagon			, 10 },
	{ preset_id::type::ooo_motionpath_parallelogram		, 14 },
	{ preset_id::type::ooo_motionpath_pentagon			, 15 },
	{ preset_id::type::ooo_motionpath_right_triangle	, 2 },
	{ preset_id::type::ooo_motionpath_square			, 7 },
	{ preset_id::type::ooo_motionpath_teardrop			, 18 },
	{ preset_id::type::ooo_motionpath_trapezoid			, 8 },
	{ preset_id::type::ooo_motionpath_arc_down			, 37 },
	{ preset_id::type::ooo_motionpath_arc_left			, 51 },
	{ preset_id::type::ooo_motionpath_arc_right			, 58 },
	{ preset_id::type::ooo_motionpath_arc_up			, 44 },
	{ preset_id::type::ooo_motionpath_bounce_left		, 41 },
	{ preset_id::type::ooo_motionpath_bounce_right		, 54 },
	{ preset_id::type::ooo_motionpath_curvy_left		, 48 },
	{ preset_id::type::ooo_motionpath_curvy_right		, 61 },
	{ preset_id::type::ooo_motionpath_decaying_wave		, 60 },
	{ preset_id::type::ooo_motionpath_diagonal_down_right, 49 },
	{ preset_id::type::ooo_motionpath_diagonal_up_right	, 56 },
	{ preset_id::type::ooo_motionpath_down				, 42 },
	{ preset_id::type::ooo_motionpath_funnel			, 52 },
	{ preset_id::type::ooo_motionpath_spring			, 53 },
	{ preset_id::type::ooo_motionpath_stairs_down		, 62 },
	{ preset_id::type::ooo_motionpath_turn_down			, 50 },
	{ preset_id::type::ooo_motionpath_turn_down_right	, 36 },
	{ preset_id::type::ooo_motionpath_turn_up			, 43 },
	{ preset_id::type::ooo_motionpath_turn_up_right		, 57 },
	{ preset_id::type::ooo_motionpath_up				, 64 },
	{ preset_id::type::ooo_motionpath_wave				, 47 },
	{ preset_id::type::ooo_motionpath_zigzag			, 38 },
	{ preset_id::type::ooo_motionpath_bean				, 31 },
	{ preset_id::type::ooo_motionpath_buzz_saw			, 25 },
	{ preset_id::type::ooo_motionpath_curved_square		, 20 },
	{ preset_id::type::ooo_motionpath_curved_x			, 21 },
	{ preset_id::type::ooo_motionpath_curvy_star		, 23 },
	{ preset_id::type::ooo_motionpath_figure_8_four		, 28 },
	{ preset_id::type::ooo_motionpath_horizontal_figure_8, 26 },
	{ preset_id::type::ooo_motionpath_inverted_square	, 34 },
	{ preset_id::type::ooo_motionpath_inverted_triangle	, 33 },
	{ preset_id::type::ooo_motionpath_loop_de_loop		, 24 },
	{ preset_id::type::ooo_motionpath_neutron			, 29 },
	{ preset_id::type::ooo_motionpath_peanut			, 27 },
	{ preset_id::type::ooo_motionpath_clover			, 32 },
	{ preset_id::type::ooo_motionpath_pointy_star		, 19 },
	{ preset_id::type::ooo_motionpath_swoosh			, 30 },
	{ preset_id::type::ooo_motionpath_vertical_figure_8	, 22 },
	{ preset_id::type::ooo_motionpath_left				, 35 },
	{ preset_id::type::ooo_motionpath_right				, 63 },
	{ preset_id::type::ooo_motionpath_spiral_left		, 55 },
	{ preset_id::type::ooo_motionpath_spiral_right		, 46 },
	{ preset_id::type::ooo_motionpath_sine_wave			, 40 },
	{ preset_id::type::ooo_motionpath_s_curve_1			, 59 },
	{ preset_id::type::ooo_motionpath_s_curve_2			, 39 },
	{ preset_id::type::ooo_motionpath_heartbeat			, 45 },

	{ preset_id::type::none								, 0 },
};

static std::wstring pptx_convert_smil_attribute_name(const odf_types::smil_attribute_name& smil_attribute_name_)
{
	using namespace odf_types;

	switch (smil_attribute_name_.get_type())
	{
	case smil_attribute_name::charColor:		return L"";
	case smil_attribute_name::charFontName:		return L"";
	case smil_attribute_name::charHeight:		return L"";
	case smil_attribute_name::charPosture:		return L"";
	case smil_attribute_name::charUnderline:	return L"";
	case smil_attribute_name::charWeight:		return L"";
	case smil_attribute_name::color:			return L"style.color";
	case smil_attribute_name::fill:				return L"fill.type";
	case smil_attribute_name::fillColor:		return L"fillcolor";
	case smil_attribute_name::fillStyle:		return L"";
	case smil_attribute_name::height:			return L"ppt_h";
	case smil_attribute_name::lineColor:		return L"";
	case smil_attribute_name::lineStyle:		return L"";
	case smil_attribute_name::opacity:			return L"style.opacity";
	case smil_attribute_name::rotate:			return L"r";
	case smil_attribute_name::skewX:			return L"xshear";
	case smil_attribute_name::skewY:			return L"";
	case smil_attribute_name::visibility:		return L"style.visibility";
	case smil_attribute_name::width:			return L"ppt_w";
	case smil_attribute_name::x:				return L"ppt_x";
	case smil_attribute_name::y:				return L"ppt_y";
	case smil_attribute_name::dim:				return L"ppt_c";
	}

	return L"";
}

static std::wstring pptx_convert_presentation_node_type(const odf_types::presentation_node_type& presentation_node_type_)
{
	using namespace odf_types;

	switch (presentation_node_type_.get_type())
	{
	case presentation_node_type::default				: return L"clickEffect";
	case presentation_node_type::after_previous			: return L"afterEffect";
	case presentation_node_type::interactive_sequence	: return L"interactiveSeq";
	case presentation_node_type::main_sequence			: return L"mainSeq";
	case presentation_node_type::on_click				: return L"clickEffect";
	case presentation_node_type::timing_root			: return L"tmRoot";
	case presentation_node_type::with_previous			: return L"withEffect";
	}

	return L"clickEffect";
}

static std::wstring pptx_convert_animation_function(std::wstring animation_function)
{
	boost::replace_all(animation_function, L"x", L"#ppt_x");
	boost::replace_all(animation_function, L"y", L"#ppt_y");
	boost::replace_all(animation_function, L"width", L"#ppt_w");
	boost::replace_all(animation_function, L"height", L"#ppt_h");

	return animation_function;
}

static std::wstring pptx_convert_smil_begin(const std::wstring& smil_begin)
{
	if(smil_begin == L"next")
		return L"indefinite";

	std::wstring delay;
	clockvalue delayClockvalue = clockvalue::parse(smil_begin);
	if (delayClockvalue.get_value() != -1)
		delay = boost::lexical_cast<std::wstring>(delayClockvalue.get_value());

	return delay;
}

static std::wstring pptx_convert_svg_path(const std::vector<::svg_path::_polyline>& polylines)
{
	using namespace ::svg_path;

	std::wstringstream result;

	for (size_t i = 0; i < polylines.size(); i++)
	{
		const _polyline& polyline = polylines[i];
		
		if (polyline.command == L"a:close")
			result << L"Z ";
		else if (polyline.command == L"a:moveTo")
			result << L"M ";
		else if (polyline.command == L"a:lnTo")
			result << L"L ";
		else if (polyline.command == L"a:cubicBezTo")
			result << L"C ";
		else if (polyline.command == L"a:ArcTo")
			result << L"G ";

		for (size_t pointIndex = 0; pointIndex < polyline.points.size(); pointIndex++)
		{
			if (polyline.points[pointIndex].x)
				result << polyline.points[pointIndex].x << L" ";
			if (polyline.points[pointIndex].y)
				result << polyline.points[pointIndex].y << L" "; 
		}
	}

	return result.str();
}

static std::vector<int> pptx_convert_smil_key_times(const odf_types::smil_key_times& key_times)
{
	std::vector<int> result;

	const int pptx_key_time_multiplier = 100000;
	const std::vector<float> values = key_times.get_values();

	for (size_t i = 0; i < values.size(); i++)
	{
		result.push_back(values[i] * pptx_key_time_multiplier);
	}

	return result;
}

static std::vector<std::wstring> pptx_convert_smil_values(const odf_types::smil_values& smil_values_)
{
	std::vector<std::wstring> result;

	const std::vector<std::wstring>& values = smil_values_.get_values();

	for (size_t i = 0; i < values.size(); i++)
	{
		std::wstring value = values[i];
		
		boost::replace_all(value, L"x", L"#ppt_x");
		boost::replace_all(value, L"y", L"#ppt_y");
		boost::replace_all(value, L"width", L"#ppt_w");
		boost::replace_all(value, L"height", L"#ppt_h");

		result.push_back(value);
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t* anim_par::ns = L"anim";
const wchar_t* anim_par::name = L"par";

void anim_par_attlist::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
	CP_APPLY_ATTR(L"presentation:preset-class",		presentation_preset_class_);
	CP_APPLY_ATTR(L"presentation:preset-id",		presentation_preset_id_);
	CP_APPLY_ATTR(L"presentation:preset-sub-type",	presentation_preset_sub_type_);
	CP_APPLY_ATTR(L"smil:accelerate",				smil_accelerate_);
	CP_APPLY_ATTR(L"smil:decelerate",				smil_decelerate_);
}

void anim_par::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	common_attlist_.add_attributes(Attributes);
	par_attlist_.add_attributes(Attributes);
}

void anim_par::pptx_convert(oox::pptx_conversion_context & Context)
{
	oox::pptx_animation_context & animationContext = Context.get_slide_context().get_animation_context();

	_CP_OPT(std::wstring)	presentationNodeType;
	_CP_OPT(std::wstring)	direction;
	_CP_OPT(std::wstring)	restart;
	_CP_OPT(int)			duration;
	_CP_OPT(std::wstring)	delay;		// NOTE: Comes from smil:begin
	_CP_OPT(std::wstring)	end;

	_CP_OPT(std::wstring)	presentationPresetClass;
	_CP_OPT(int)			presentationPresetId;
	_CP_OPT(std::wstring)	presentationPresetPresetSubType;

	// NOTE: в pptx нет атрибутов accelerate/decelerate. Там надо менять svg path ???
	//_CP_OPT(std::wstring)	accelerate; 
	//_CP_OPT(std::wstring)	decelerate;
	
	if (common_attlist_.presentation_node_type_)
	{
		switch (common_attlist_.presentation_node_type_.value().get_type())
		{
		case odf_types::presentation_node_type::timing_root:
			presentationNodeType = L"tmRoot";
			break;
		case odf_types::presentation_node_type::on_click:
			presentationNodeType = L"clickEffect";
			break;
		case odf_types::presentation_node_type::after_previous:
			presentationNodeType = L"afterEffect";
			break;
		case odf_types::presentation_node_type::with_previous:
			presentationNodeType = L"withEffect";
			break;
		}

		presentationNodeType = pptx_convert_presentation_node_type(common_attlist_.presentation_node_type_.value());
	}

	if (common_attlist_.smil_direction_)
	{
		if (common_attlist_.smil_direction_.value() == L"reverse")
			direction = L"reverse";
	}
	
	if (common_attlist_.smil_restart_)
	{
		// smil:restart = "never", "always", "whenNotActive" or "default".
		// NOTE: Hardcode for now
		// TODO: Figure out correct value
		restart = boost::none;
	}
	
	if (common_attlist_.smil_dur_)
	{
		duration = common_attlist_.smil_dur_->get_value();
	}

	if (common_attlist_.smil_begin_)
	{
		delay = pptx_convert_smil_begin(common_attlist_.smil_begin_.value());
	}

	// TODO: Figure out correct value
	end = boost::none;

	if (par_attlist_.presentation_preset_class_)
	{
		switch (par_attlist_.presentation_preset_class_.value().get_type())
		{
		case preset_class::entrance:
			presentationPresetClass = L"entr";
			presentationPresetId = pptx_convert_preset_id();
			break;
		case preset_class::exit:
			presentationPresetClass = L"exit";
			presentationPresetId = pptx_convert_preset_id();
			break;
		case preset_class::emphasis:
			presentationPresetClass = L"emph";
			presentationPresetId = pptx_convert_preset_id();
			break;
		case preset_class::motion_path:
			presentationPresetClass = L"path";
			presentationPresetId = pptx_convert_preset_id();
			break;
		case preset_class::ole_action:
			presentationPresetClass = L"verb";
			break;
		case preset_class::media_call:
			presentationPresetClass = L"mediacall";
			break;
		default:
			presentationPresetClass = L"custom";
		}			
	}
	
	animationContext.start_par_animation();

	if (presentationNodeType)		animationContext.set_par_animation_presentation_node_type	(presentationNodeType.value());
	if (direction)					animationContext.set_par_animation_direction				(direction.value());
	if (restart)					animationContext.set_par_animation_restart					(restart.value());
	if (duration)					animationContext.set_par_animation_duration					(duration.value());
	if (delay)						animationContext.set_par_animation_delay					(delay.value());
	if (end)						animationContext.set_par_animation_end						(end.value());
	if (presentationPresetClass)	animationContext.set_par_animation_preset_class				(presentationPresetClass.value());
	if (presentationPresetId)		animationContext.set_par_animation_preset_id				(presentationPresetId.value());


	if (anim_par_array_.size())
	{
		Context.get_slide_context().start_slide_animation();
		for(size_t i = 0; i < anim_par_array_.size(); i++)
			anim_par_array_[i]->pptx_convert(Context); // это для самого слайда (то что и нужно)
		Context.get_slide_context().end_slide_animation();
	}
	for (size_t i = 0; i < anim_seq_array_.size(); i++)
    {
		anim_seq_array_[i]->pptx_convert(Context);
	}
/////////////////////////////////////////////////////////////////
//внутренние эффекты - те что внутри одной последовательности
	for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->pptx_convert(Context);
	}

	animationContext.end_par_animation();
}
void anim_par::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if CP_CHECK_NAME(L"anim", L"par") 
		CP_CREATE_ELEMENT(anim_par_array_);
	else if	CP_CHECK_NAME(L"anim", L"seq") 
		CP_CREATE_ELEMENT(anim_seq_array_);//более 1 элемента- взаимосвязанная анимация (между фигурами)
	else
		CP_CREATE_ELEMENT(content_);
}

boost::optional<int> anim_par::pptx_convert_preset_id()
{
	if (par_attlist_.presentation_preset_id_)
	{
		preset_id::type presetID = par_attlist_.presentation_preset_id_.value().get_type();

		for (size_t i = 0; g_preset_id_map[i].ODF_PresetID != preset_id::type::none; i++)
		{
			if (g_preset_id_map[i].ODF_PresetID == presetID)
				return g_preset_id_map[i].OOX_PresetID;
		}
	}
	

	return boost::none;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * anim_seq::ns = L"anim";
const wchar_t * anim_seq::name = L"seq";

void anim_seq::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	attlist_.add_attributes(Attributes);
}

void anim_seq::pptx_convert(oox::pptx_conversion_context & Context)
{
	_CP_OPT(std::wstring)	presentationNodeType;
	_CP_OPT(int)			duration;

	if (attlist_.presentation_node_type_)
		presentationNodeType = pptx_convert_presentation_node_type(attlist_.presentation_node_type_.value());

	if (attlist_.smil_dur_)
		duration = attlist_.smil_dur_->get_value();

	oox::pptx_animation_context& animationContext = Context.get_slide_context().get_animation_context();

	animationContext.start_seq_animation();

	if (presentationNodeType)				animationContext.set_seq_animation_presentation_node_type(presentationNodeType.value());
	if (attlist_.smil_direction_)			animationContext.set_seq_animation_direction(attlist_.smil_direction_.value());
	if (attlist_.smil_restart_)				animationContext.set_seq_animation_restart(attlist_.smil_restart_.value());
	if (duration)							animationContext.set_seq_animation_dur(duration.value());
	if (attlist_.smil_begin_)				animationContext.set_seq_animation_delay(attlist_.smil_begin_.value());
	if (attlist_.smil_end_)					animationContext.set_seq_animation_end(attlist_.smil_end_.value());

	for (size_t i = 0; i < anim_par_array_.size(); i++)
    {
		anim_par_array_[i]->pptx_convert(Context);
	}
	animationContext.end_seq_animation();
}
void anim_seq::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{ 
	if	CP_CHECK_NAME(L"anim", L"par") 
		CP_CREATE_ELEMENT(anim_par_array_);
}
////////////////////////////////////////////////////////////////
void anim_transition_filter_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"smil:subtype",			smil_subtype_);
    CP_APPLY_ATTR(L"smil:type",				smil_type_);
	CP_APPLY_ATTR(L"smil:fadeColor",		smil_fadeColor_);
	CP_APPLY_ATTR(L"smil:mode",				smil_mode_);
	CP_APPLY_ATTR(L"smil:targetElement",	smil_target_element_);
}
void anim_audio_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"xlink:href",		xlink_href_);
    CP_APPLY_ATTR(L"anim:audio-level",	anim_audio_level_);
}

const wchar_t * anim_transitionFilter::ns	= L"anim";
const wchar_t * anim_transitionFilter::name = L"transitionFilter";

void anim_transitionFilter::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	common_attlist_.add_attributes(Attributes);
	filter_attlist_.add_attributes(Attributes);
}

std::wstring anim_transitionFilter::convert_filter()
{
	std::wstring filter;
	const _CP_OPT(std::wstring)& subtype = filter_attlist_.smil_subtype_;
	_CP_OPT(std::wstring) pptx_subtype;

	if (filter_attlist_.smil_type_)
	{
		switch (filter_attlist_.smil_type_.value().get_type())
		{
		case smil_transition_type::barWipe:
			filter = L"wipe";
			if (subtype)
			{
				if (subtype.value() == L"topToBottom")	pptx_subtype = L"down";
				else									pptx_subtype = L"up";
			}
			else
				pptx_subtype = L"up";
			break;
		case smil_transition_type::boxWipe:
			filter = L"slide";
			if (subtype)
			{
					 if (subtype.value() == L"topRight")		pptx_subtype = L"fromTop";
				else if (subtype.value() == L"bottomRight")		pptx_subtype = L"fromBottom";
				else if (subtype.value() == L"bottomLeft")		pptx_subtype = L"fromBottom";
				else if (subtype.value() == L"topCenter")		pptx_subtype = L"fromTop";
				else if (subtype.value() == L"rightCenter")		pptx_subtype = L"fromRight";
				else if (subtype.value() == L"bottomCenter")	pptx_subtype = L"fromBottom";
				else if (subtype.value() == L"leftCenter")		pptx_subtype = L"fromLeft";
				else											pptx_subtype = L"fromTop";
			}
			else 
				pptx_subtype = L"fromTop";
			break;
		case smil_transition_type::fourBoxWipe:
			filter = L"plus";
			if (subtype)
			{
				if (subtype.value() == L"cornersOut")		pptx_subtype = L"out";
				else										pptx_subtype = L"in";
			}
			else
				pptx_subtype = L"in";
			break;
		case smil_transition_type::barnDoorWipe:
			filter = L"barn";
			if (subtype)
			{
				if (subtype.value() == L"horizontal")		pptx_subtype = L"inHorizontal";
				else										pptx_subtype = L"inVertical";
			}
			else
				pptx_subtype = L"inHorizontal";
			break;
		case smil_transition_type::irisWipe:              
			if (subtype)
			{
				if (subtype.value() == L"rectangle")
				{
					filter = L"box";
					pptx_subtype = L"in";
				}
				else if (subtype.value() == L"diamond")
				{
					filter = L"diamond";
					pptx_subtype = L"in";
				}
				else
				{
					filter = L"box";
					pptx_subtype = L"in";
				}
			}
			else
			{
				filter = L"box";
				pptx_subtype = L"in";
			}
			break;
		case smil_transition_type::ellipseWipe:
			filter = L"circle";
			pptx_subtype = L"in";
			break;
		case smil_transition_type::pinWheelWipe:
			filter = L"wheel";
			if (subtype)
			{
				if (subtype.value() == L"oneBlade")					pptx_subtype = L"1";
				else if (subtype.value() == L"twoBladeVertical")	pptx_subtype = L"2";
				else if (subtype.value() == L"fourBlade")			pptx_subtype = L"4";
				else												pptx_subtype = L"2";
			}
			else
				pptx_subtype = L"TODO";
			break;
		case smil_transition_type::fanWipe:
			filter = L"wedge";
			break;
		case smil_transition_type::waterfallWipe:
			filter = L"strips";
			if (subtype)
			{
					 if (subtype.value() == L"horizontalLeft")		pptx_subtype = L"downRight";
				else if (subtype.value() == L"horizontalRight")		pptx_subtype = L"downLeft";
				else if (subtype.value() == L"verticalLeft")		pptx_subtype = L"upRight";
				else if (subtype.value() == L"verticalRight")		pptx_subtype = L"upLeft";
				else												pptx_subtype = L"upRight";
			}
			else
				pptx_subtype = L"upRight";
			break;
		case smil_transition_type::slideWipe:
			filter = L"slide";
			if (subtype)
			{
				if (subtype.value() == L"fromRight")			pptx_subtype = L"fromRight";
				else											pptx_subtype = L"fromLeft";
			}
			break;
		case smil_transition_type::fade:
			filter = L"fade";
			break;
		case smil_transition_type::checkerBoardWipe:
			filter = L"checkerboard";
			if (subtype)
			{
				if (subtype.value() == L"across")		pptx_subtype = L"across";
				else									pptx_subtype = L"across";
			}
			else 
				pptx_subtype = L"across";
			break;
		case smil_transition_type::blindsWipe:            
			filter = L"blinds";
			if (subtype)
			{
				if (subtype.value() == L"horizontal")		pptx_subtype = L"horizontal";
				else										pptx_subtype = L"vertical";
			}
			else
				pptx_subtype = L"vertical";
			break;
		case smil_transition_type::dissolve:
			filter = L"dissolve";
			break;
		case smil_transition_type::randomBarWipe:
			filter = L"randombar";
			if (subtype)
			{
				if (subtype.value() == L"horizontal")		pptx_subtype = L"horizontal";
				else										pptx_subtype = L"vertical";
			}
			break;
		case smil_transition_type::pushWipe:            
		case smil_transition_type::doubleFanWipe:       
		case smil_transition_type::doubleSweepWipe:     
		case smil_transition_type::saloonDoorWipe:      
		case smil_transition_type::windshieldWipe:      
		case smil_transition_type::snakeWipe:           
		case smil_transition_type::spiralWipe:          
		case smil_transition_type::parallelSnakesWipe:  
		case smil_transition_type::boxSnakesWipe:       
		case smil_transition_type::singleSweepWipe:     
		case smil_transition_type::eyeWipe:             
		case smil_transition_type::roundRectWipe:       
		case smil_transition_type::starWipe:            
		case smil_transition_type::miscShapeWipe:       
		case smil_transition_type::clockWipe:           
		case smil_transition_type::triangleWipe:        
		case smil_transition_type::arrowHeadWipe:       
		case smil_transition_type::pentagonWipe:        
		case smil_transition_type::hexagonWipe:         
		case smil_transition_type::diagonalWipe:        
		case smil_transition_type::bowTieWipe:          
		case smil_transition_type::miscDiagonalWipe:    
		case smil_transition_type::veeWipe:             
		case smil_transition_type::barnVeeWipe:         
		case smil_transition_type::zigZagWipe:          
		case smil_transition_type::barnZigZagWipe:       
			// NOTE: Not implemented yet. Set "fade" as default animation 
			filter = L"fade"; 
			break;
		default:

			break;
		}
	}

	if (pptx_subtype)
		filter += L"(" + pptx_subtype.value() + L")";

	return filter;
}

void anim_transitionFilter::pptx_convert(oox::pptx_conversion_context & Context)
{
	_CP_OPT(std::wstring) color;
	_CP_OPT(std::wstring) dir;
	_CP_OPT(int)		  time;
	std::wstring		  type;

	_CP_OPT(std::wstring) param;

	if (common_attlist_.smil_dur_)
	{
		time = common_attlist_.smil_dur_->get_value();
	}	
	if (filter_attlist_.smil_fadeColor_)
	{
		color = filter_attlist_.smil_fadeColor_->get_hex_value();
	}

	smil_transition_type::type transition_type;

	if (filter_attlist_.smil_type_)
	{
		transition_type = filter_attlist_.smil_type_->get_type();
	}

	switch(transition_type)
	{
		case smil_transition_type::barnVeeWipe: 
			type = L"split";
			break; 
		case smil_transition_type::irisWipe: 
			if ((filter_attlist_.smil_subtype_) && (filter_attlist_.smil_subtype_.get()==L"diamond"))
				type = L"diamond";
			else
				type = L"zoom";
			break; 
		case smil_transition_type::miscDiagonalWipe: 
			if ((filter_attlist_.smil_subtype_) && (filter_attlist_.smil_subtype_.get()==L"doubleDiamond"))
				type = L"diamond";
			else
				type = L"zoom";
			break; 
		case smil_transition_type::ellipseWipe: 
		case smil_transition_type::eyeWipe: 
			type = L"circle";
			break; 
		case smil_transition_type::roundRectWipe: 
			type = L"zoom";
			break; 
		case smil_transition_type::fourBoxWipe: 
		case smil_transition_type::triangleWipe: 
		case smil_transition_type::arrowHeadWipe: 
		case smil_transition_type::pentagonWipe: 
		case smil_transition_type::hexagonWipe: 		
		case smil_transition_type::starWipe: 
		case smil_transition_type::miscShapeWipe: 
			type = L"plus";
			break; 
		case smil_transition_type::pinWheelWipe: 
			param = L"2";		
		case smil_transition_type::clockWipe: 
		case smil_transition_type::singleSweepWipe: //
		case smil_transition_type::doubleFanWipe: //
			type = L"wheel";
				 if ((filter_attlist_.smil_subtype_)  && (filter_attlist_.smil_subtype_.get()==L"oneBlade"))	param = L"1";
			else if ((filter_attlist_.smil_subtype_)  && (filter_attlist_.smil_subtype_.get()==L"threeBlade"))	param = L"3";
			else if ((filter_attlist_.smil_subtype_)  && (filter_attlist_.smil_subtype_.get()==L"fourBlade"))	param = L"4";
			else if ((filter_attlist_.smil_subtype_)  && (filter_attlist_.smil_subtype_.get()==L"eightBlade"))	param = L"8";
			break; 
		case smil_transition_type::fanWipe: 
			type = L"wedge";
			break;
		case smil_transition_type::fade:
			type = L"fade";
			param = L"1";
			break;
		case smil_transition_type::checkerBoardWipe:
			type = L"checker";
			if (filter_attlist_.smil_subtype_.get()==L"across")	dir = L"horz";
			if (filter_attlist_.smil_subtype_.get()==L"down")	dir = L"vert";
			break;
		case smil_transition_type::blindsWipe:
			type = L"blinds";
				 if (filter_attlist_.smil_subtype_.get()==L"vertical")		dir = L"vert";
			else if (filter_attlist_.smil_subtype_.get()==L"horizontal")	dir = L"horz";
			break;
		case smil_transition_type::diagonalWipe:
		case smil_transition_type::waterfallWipe:
			type = L"strips";			
			if (filter_attlist_.smil_subtype_)
			{
				if		(filter_attlist_.smil_subtype_.get() == L"horizontalLeft")		dir = L"rd";	
				else if (filter_attlist_.smil_subtype_.get() == L"horizontalRight")		dir = L"lu";	
				else if (filter_attlist_.smil_subtype_.get() == L"verticalRight")		dir = L"ld";
				else dir = L"ru";	
			}
			break;
		case smil_transition_type::dissolve:
			type = L"dissolve";
			break;		
		case smil_transition_type::randomBarWipe:
			type = L"randomBar";
				 if (filter_attlist_.smil_subtype_.get() == L"vertical")	dir = L"vert";
			else if (filter_attlist_.smil_subtype_.get() == L"horizontal")	dir = L"horz";
			break;	
		case smil_transition_type::pushWipe: 
			type = L"push";
				 if (filter_attlist_.smil_subtype_.get() == L"combVertical")	{type = L"comb"; dir = L"vert";}
			else if (filter_attlist_.smil_subtype_.get() == L"combHorizontal")	{type = L"comb"; dir = L"horz";}
			break;	
		case smil_transition_type::slideWipe: 
			type = L"pull";
			break;
		case smil_transition_type::boxWipe: 
			type = L"cover";
			break;
		case smil_transition_type::barnDoorWipe: 
			type = L"split";
			if (filter_attlist_.smil_subtype_.get() == L"vertical")		param = L"vert";
			if (filter_attlist_.smil_subtype_.get() == L"horizontal")	param = L"horz";			
			break;
		case smil_transition_type::barWipe:
			type = L"wipe";
			if (filter_attlist_.smil_subtype_)
			{
					 if (filter_attlist_.smil_subtype_.get()==L"fromTopLeft")		{type = L"strips"; dir = L"rd";}
				else if (filter_attlist_.smil_subtype_.get()==L"fromBottomLeft")	{type = L"strips"; dir = L"ru";}
				else if (filter_attlist_.smil_subtype_.get()==L"fromTopRight")		{type = L"strips"; dir = L"ld";}
				else if (filter_attlist_.smil_subtype_.get()==L"fromBottomRight")	{type = L"strips"; dir = L"lu";}
				
				else if (filter_attlist_.smil_subtype_.get()==L"fadeOverColor")		{type = L"fade"; param = L"0";}
			}
			break;
///////////////////////////////////////////////////////
		case smil_transition_type::bowTieWipe:
		case smil_transition_type::veeWipe: 
		case smil_transition_type::zigZagWipe: 
		case smil_transition_type::barnZigZagWipe: 
		case smil_transition_type::doubleSweepWipe: 
		case smil_transition_type::saloonDoorWipe: 
		case smil_transition_type::windshieldWipe: 
		case smil_transition_type::snakeWipe: 
		case smil_transition_type::spiralWipe: 
		case smil_transition_type::parallelSnakesWipe: 
		case smil_transition_type::boxSnakesWipe: 
			break;
//////////////////////////////////////////////////////
	}
	if (filter_attlist_.smil_subtype_)
	{
		if (!dir)
		{
			if (filter_attlist_.smil_subtype_.get()==L"leftToRight")
			{
				if ((common_attlist_.smil_direction_) && (common_attlist_.smil_direction_.get()==L"reverse"))dir = L"l";
				else dir = L"r";
			}
			if (filter_attlist_.smil_subtype_.get()==L"topToBottom")
			{
				if ((common_attlist_.smil_direction_) && (common_attlist_.smil_direction_.get()==L"reverse"))dir = L"u";
				else dir = L"d";	
			}

				 if (filter_attlist_.smil_subtype_.get()==L"fromTop")		dir = L"d";	
			else if (filter_attlist_.smil_subtype_.get()==L"fromLeft")		dir = L"r";	
			else if (filter_attlist_.smil_subtype_.get()==L"fromRight")		dir = L"l";			
			else if (filter_attlist_.smil_subtype_.get()==L"fromBottom")	dir = L"u";	
			
			else if (filter_attlist_.smil_subtype_.get()==L"topRight")		dir = L"ld";	
			else if (filter_attlist_.smil_subtype_.get()==L"bottomLeft")	dir = L"lu";	
			else if (filter_attlist_.smil_subtype_.get()==L"bottomRight")	dir = L"ru";
			else if (filter_attlist_.smil_subtype_.get()==L"topLeft")		dir = L"rd";	
		
			else if (filter_attlist_.smil_subtype_.get()==L"fromTopLeft")	dir = L"rd";
			else if (filter_attlist_.smil_subtype_.get()==L"fromBottomLeft")dir = L"ru";
			else if (filter_attlist_.smil_subtype_.get()==L"fromTopRight")	dir = L"ld";
			else if (filter_attlist_.smil_subtype_.get()==L"fromBottomRight")dir = L"lu";

		}
		
		if (!dir && (common_attlist_.smil_direction_) && (common_attlist_.smil_direction_.get()==L"reverse"))
			dir = L"in";
	}

	Context.get_slide_context().set_transitionFilter(type , dir, param , time);

	std::wstring filter = convert_filter();
	std::wstring transition = L"in";
	size_t shapeId = 0;

	if (filter_attlist_.smil_mode_)
	{
		if (filter_attlist_.smil_mode_.value() == L"out")
			transition = L"out";
	}

	if (filter_attlist_.smil_target_element_)
		shapeId = Context.get_slide_context().get_id(filter_attlist_.smil_target_element_.value());

	oox::pptx_animation_context& animationContext = Context.get_slide_context().get_animation_context();

	animationContext.start_anim_effect();
	animationContext.set_anim_effect_filter(filter);
	animationContext.set_anim_effect_transition(transition);
	if (time) animationContext.set_anim_effect_duration(time.value());
	animationContext.set_anim_effect_shape_id(shapeId);
	animationContext.end_anim_effect();
}

const wchar_t * anim_audio::ns = L"anim";
const wchar_t * anim_audio::name = L"audio";

void anim_audio::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	common_attlist_.add_attributes(Attributes);
	audio_attlist_.add_attributes(Attributes);
}

void anim_audio::pptx_convert(oox::pptx_conversion_context & Context)
{
}

////////////////////////////////////////////////////////////////
void anim_set_attlist::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
	CP_APPLY_ATTR(L"smil:fill",				smil_fill_);
	CP_APPLY_ATTR(L"smil:targetElement",	smil_target_element_);
	CP_APPLY_ATTR(L"smil:attributeName",	smil_attribute_name_);
	CP_APPLY_ATTR(L"smil:to",				smil_to_);
}

const wchar_t* anim_set::ns = L"anim";
const wchar_t* anim_set::name = L"set";

void anim_set::pptx_convert(oox::pptx_conversion_context& Context)
{
	_CP_OPT(std::wstring)		direction;
	_CP_OPT(std::wstring)		restart;
	_CP_OPT(int)				duration;
	_CP_OPT(std::wstring)		delay;
	_CP_OPT(std::wstring)		end;
	_CP_OPT(std::wstring)		fill;
	_CP_OPT(std::wstring)		attribute_name;
	_CP_OPT(std::wstring)		to_value;
	size_t						shapeID = 0;

	if (common_attlist_.smil_direction_)
	{	
	}

	if (common_attlist_.smil_restart_)
	{
	}

	if (common_attlist_.smil_dur_)
	{
		duration = common_attlist_.smil_dur_->get_value();
	}

	if (common_attlist_.smil_begin_)
	{
		delay = pptx_convert_smil_begin(common_attlist_.smil_begin_.value());
	}

	if (common_attlist_.smil_end_)
	{
	}

	if (set_attlist_.smil_fill_)
	{
		fill = set_attlist_.smil_fill_.value();
	}

	if (set_attlist_.smil_target_element_)
	{
		shapeID = Context.get_slide_context().get_id(set_attlist_.smil_target_element_.value());
	}

	if (set_attlist_.smil_attribute_name_)
	{
		attribute_name = pptx_convert_smil_attribute_name(set_attlist_.smil_attribute_name_.value());
	}

	if (set_attlist_.smil_to_)
	{
		if (set_attlist_.smil_to_.value() == L"visible")
			to_value = L"visible";
		else if(set_attlist_.smil_to_.value() == L"hidden")
			to_value = L"hidden";
		else if (set_attlist_.smil_to_.value() == L"solid")
			to_value = L"solid";
	}

	oox::pptx_animation_context& animationContext = Context.get_slide_context().get_animation_context();

	animationContext.start_set();
	if (direction)			animationContext.set_set_direction(direction.value());
	if (restart)			animationContext.set_set_restart(restart.value());
	if (duration)			animationContext.set_set_duration(duration.value());
	if (delay)				animationContext.set_set_delay(delay.value());
	if (end)				animationContext.set_set_end(end.value());
	if (fill)				animationContext.set_set_fill(fill.value());
	if (attribute_name)		animationContext.set_set_attribute_name(attribute_name.value());
	if (to_value)			animationContext.set_set_to_value(to_value.value());
	animationContext.set_set_shape_id(shapeID);
	animationContext.end_set();
}

void anim_set::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
	common_attlist_.add_attributes(Attributes);
	set_attlist_.add_attributes(Attributes);
}

////////////////////////////////////////////////////////////////
void anim_animate_motion_attlist::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
	CP_APPLY_ATTR(L"smil:fill",				smil_fill_);
	CP_APPLY_ATTR(L"smil:targetElement",	smil_target_element_);
	CP_APPLY_ATTR(L"svg:path",				svg_path_);
}

const wchar_t* anim_animate_motion::ns = L"anim";
const wchar_t* anim_animate_motion::name = L"animateMotion";

void anim_animate_motion::pptx_convert(oox::pptx_conversion_context& Context)
{
	oox::pptx_animation_context& animationContext = Context.get_slide_context().get_animation_context();

	size_t shapeID = 0;
	_CP_OPT(std::wstring) path;

	if (animate_motion_attlist_.smil_target_element_)
		shapeID = Context.get_slide_context().get_id(animate_motion_attlist_.smil_target_element_.value());

	if (animate_motion_attlist_.svg_path_)
	{
		std::vector<::svg_path::_polyline> polylines;
		bool closed, stroked;
		::svg_path::parseSvgD(polylines, animate_motion_attlist_.svg_path_.value(), false, closed, stroked);

		path = pptx_convert_svg_path(polylines);
	}
	

	animationContext.start_animate_motion();

//	if (common_attlist_.presentation_node_type_)			animationContext.set_animate_motion_presentation_node_type(common_attlist_.presentation_node_type_.value());
//	if (common_attlist_.smil_direction_)					animationContext.set_animate_motion_direction(common_attlist_.smil_direction_.value());
//	if (common_attlist_.smil_restart_)						animationContext.set_animate_motion_restart(common_attlist_.smil_restart_.value());
	if (common_attlist_.smil_dur_)							animationContext.set_animate_motion_dur(common_attlist_.smil_dur_.value().get_value());
//	if (common_attlist_.smil_begin_)						animationContext.set_animate_motion_delay(common_attlist_.smil_begin_.value());
//	if (common_attlist_.smil_end_)							animationContext.set_animate_motion_end(common_attlist_.smil_end_.value());

	if (animate_motion_attlist_.smil_fill_)					animationContext.set_animate_motion_fill(animate_motion_attlist_.smil_fill_.value());
	if (path)												animationContext.set_animate_motion_svg_path(path.value());

	animationContext.set_animate_motion_shape_id(shapeID);
	
	animationContext.end_animate_motion();
}

void anim_animate_motion::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
	common_attlist_.add_attributes(Attributes);
	animate_motion_attlist_.add_attributes(Attributes);
}

////////////////////////////////////////////////////////////////
void anim_animate_color_attlist::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
	CP_APPLY_ATTR(L"smil:fill",								smil_fill_);
	CP_APPLY_ATTR(L"smil:targetElement",					smil_target_element_);
	CP_APPLY_ATTR(L"smil:attributeName",					smil_attribute_name_);
	CP_APPLY_ATTR(L"smil:to",								smil_to_);
	CP_APPLY_ATTR(L"presentation:master-element",			presentation_master_element_);
	CP_APPLY_ATTR(L"anim:color-interpolation",				anim_color_interpolation_);
	CP_APPLY_ATTR(L"anim:color-interpolation-direction",	anim_color_interpolation_direction);
}

const wchar_t* anim_animate_color::ns = L"anim";
const wchar_t* anim_animate_color::name = L"animateColor";

void anim_animate_color::pptx_convert(oox::pptx_conversion_context& Context)
{
	_CP_OPT(std::wstring) colorSpace;
	_CP_OPT(int) duration;
	_CP_OPT(std::wstring) delay;
	_CP_OPT(std::wstring) attributeName;
	_CP_OPT(std::wstring) toValue;
	size_t				  shapeID = 0;

	colorSpace = L"rgb";
	
	if (common_attlist_.smil_dur_)
		duration = common_attlist_.smil_dur_->get_value();
	else
		duration = 1;

	if (common_attlist_.smil_begin_)
	{
		delay = pptx_convert_smil_begin(common_attlist_.smil_begin_.value());
	}

	if (animate_color_attlist_.smil_attribute_name_)
	{
		attributeName = pptx_convert_smil_attribute_name(animate_color_attlist_.smil_attribute_name_.value());
	}

	if (animate_color_attlist_.smil_to_)
	{
		toValue = animate_color_attlist_.smil_to_.value();
		boost::algorithm::erase_all(toValue.value(), L"#");
	}

	if (animate_color_attlist_.smil_target_element_)
		shapeID = Context.get_slide_context().get_id(animate_color_attlist_.smil_target_element_.value());

	oox::pptx_animation_context& animationContext = Context.get_slide_context().get_animation_context();

	animationContext.start_animate_color();
	if (colorSpace)			animationContext.set_animate_color_color_space(colorSpace.value());
	if (duration)			animationContext.set_animate_color_duration(duration.value());
	if (delay)				animationContext.set_animate_color_delay(delay.value());
	if (attributeName)		animationContext.set_animate_color_attribute_name(attributeName.value());
	if (toValue)			animationContext.set_animate_color_to_value(toValue.value());
	animationContext.set_animate_color_shape_id(shapeID);
	animationContext.end_animate_color();
}

void anim_animate_color::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
	common_attlist_.add_attributes(Attributes);
	animate_color_attlist_.add_attributes(Attributes);
}

void anim_animate_attlist::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
	CP_APPLY_ATTR(L"smil:targetElement",		smil_target_element_);
	CP_APPLY_ATTR(L"smil:attributeName",		smil_attribute_name_);
	CP_APPLY_ATTR(L"smil:values",				smil_values_);
	CP_APPLY_ATTR(L"smil:keyTimes",				smil_key_times_);
	CP_APPLY_ATTR(L"smil:calcMode",				smil_calc_mode_);
	CP_APPLY_ATTR(L"smil:from",					smil_from_);
	CP_APPLY_ATTR(L"smil:to",					smil_to_);
	CP_APPLY_ATTR(L"smil:by",					smil_by_);
	CP_APPLY_ATTR(L"smil:autoReverse",			smil_auto_reverse_);
	CP_APPLY_ATTR(L"smil:additive",				smil_additive_);
	CP_APPLY_ATTR(L"anim:formula",				anim_formula_);
}

//////////////////////////////////////////////////////////////////////////
// anim:animate

const wchar_t* anim_animate::ns = L"anim";
const wchar_t* anim_animate::name = L"animate";

void anim_animate::pptx_convert(oox::pptx_conversion_context& Context)
{
	_CP_OPT(std::wstring)						calcmode;
	_CP_OPT(std::wstring)						valueType;
	_CP_OPT(int)								duration;
	_CP_OPT(std::wstring)						attributeName;
	size_t										shapeID = 0;
	_CP_OPT(std::wstring)						from;
	_CP_OPT(std::wstring)						to;
	_CP_OPT(std::wstring)						by;
	_CP_OPT(std::wstring)						additive;
	_CP_OPT(bool)								autoRev;
	_CP_OPT(std::wstring)						delay;
	_CP_OPT(std::wstring)						formula;
	std::vector<std::wstring>					values;
	std::vector<int>							keyTimes;

	if (animate_attlist_.smil_calc_mode_)
	{
		if (animate_attlist_.smil_calc_mode_.value() == L"discrete")	calcmode = L"discrete";
		else															calcmode = L"lin";
	}
	else
		calcmode = L"lin";

	valueType = L"num";
	duration = common_attlist_.smil_dur_ ? common_attlist_.smil_dur_->get_value() : 1;

	if (animate_attlist_.smil_attribute_name_)
	{
		attributeName = pptx_convert_smil_attribute_name(animate_attlist_.smil_attribute_name_.value());
	}

	if (animate_attlist_.smil_target_element_)
		shapeID = Context.get_slide_context().get_id(animate_attlist_.smil_target_element_.value());
	
	if (animate_attlist_.smil_values_)
		values = pptx_convert_smil_values(animate_attlist_.smil_values_.value());

	if (animate_attlist_.smil_key_times_)
		keyTimes = pptx_convert_smil_key_times(animate_attlist_.smil_key_times_.value());

	if (animate_attlist_.anim_formula_)
		formula = animate_attlist_.anim_formula_.value();

	if (animate_attlist_.smil_from_)
		from = pptx_convert_animation_function(animate_attlist_.smil_from_.value());

	if (animate_attlist_.smil_to_)
		to = pptx_convert_animation_function(animate_attlist_.smil_to_.value());

	if (animate_attlist_.smil_by_)
		by = pptx_convert_animation_function(animate_attlist_.smil_by_.value());

	if (animate_attlist_.smil_additive_)
	{
		switch (animate_attlist_.smil_additive_.value().get_type())
		{
		case odf_types::smil_additive::replace: additive = L"repl";
		case odf_types::smil_additive::sum: additive = L"sum";
		default: additive = L"repl";
		}
	}
	else
		additive = L"repl";

	if (animate_attlist_.smil_auto_reverse_)
	{
		autoRev = animate_attlist_.smil_auto_reverse_.value().get();
	}

	if (common_attlist_.smil_begin_)
	{
		delay = pptx_convert_smil_begin(common_attlist_.smil_begin_.value());
	}

	oox::pptx_animation_context& animationContext = Context.get_slide_context().get_animation_context();
	animationContext.start_animate();
	if (calcmode)			animationContext.set_animate_calc_mode(calcmode.value());
	if (valueType)			animationContext.set_animate_value_type(valueType.value());
	if (duration)			animationContext.set_animate_duration(duration.value());
	if (attributeName)		animationContext.set_animate_attribute_name(attributeName.value());
	if (from)				animationContext.set_animate_from(from.value());
	if (to)					animationContext.set_animate_to(to.value());
	if (by)					animationContext.set_animate_by(by.value());
	if (additive)			animationContext.set_animate_additive(additive.value());
	if (autoRev)			animationContext.set_animate_auto_reverse(autoRev.value());
	if (delay)				animationContext.set_animate_delay(delay.value());
	animationContext.set_animate_shape_id(shapeID);

	if (keyTimes.size() == values.size())
	{
		size_t size = keyTimes.size();

		for (size_t i = 0; i < size; i++)
		{
			animationContext.add_animate_keypoint(keyTimes[i], values[i], formula);
		}
	}

	animationContext.end_animate();
}

void anim_animate::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
	common_attlist_.add_attributes(Attributes);
	animate_attlist_.add_attributes(Attributes);
}

//////////////////////////////////////////////////////////////////////////
// anim:animateTransform

const wchar_t* anim_animate_transform::ns = L"anim";
const wchar_t* anim_animate_transform::name = L"animateTransform";

void anim_animate_transform::pptx_convert(oox::pptx_conversion_context& Context)
{
	size_t shapeID = 0;
	_CP_OPT(int) duration;
	_CP_OPT(std::wstring) fill;
	_CP_OPT(std::wstring) delay;
	_CP_OPT(bool) autoRev;
	_CP_OPT(int) by;

	if(animate_transform_attlist_.smil_target_element_)
		shapeID = Context.get_slide_context().get_id(animate_transform_attlist_.smil_target_element_.value());

	if (common_attlist_.smil_dur_)
		duration = common_attlist_.smil_dur_.value().get_value();

	if (animate_transform_attlist_.smil_fill_)
		fill = animate_transform_attlist_.smil_fill_.value();

	if (common_attlist_.smil_begin_)
		delay = pptx_convert_smil_begin(common_attlist_.smil_begin_.value());

	if (animate_transform_attlist_.smil_auto_reverse_)
		autoRev = animate_transform_attlist_.smil_auto_reverse_.value().get();
		

	if (animate_transform_attlist_.svg_type_)
	{
		oox::pptx_animation_context& animationContext = Context.get_slide_context().get_animation_context();

		switch (animate_transform_attlist_.svg_type_->get_type())
		{
		case odf_types::svg_type::scale:
		{

			animationContext.start_animate_scale();
			animationContext.set_animate_scale_shape_id(shapeID);
			if (duration)		animationContext.set_animate_scale_duration(duration.value());
			if (fill)			animationContext.set_animate_scale_fill(fill.value());
			if (delay)			animationContext.set_animate_scale_delay(delay.value());
			if (autoRev)		animationContext.set_animate_scale_auto_reverse(autoRev.value());

			if (animate_transform_attlist_.smil_from_)
			{
				const int pptx_mulipier = 100000;
				std::vector<std::wstring> oox_from;
				boost::split(oox_from, animate_transform_attlist_.smil_from_.value(), boost::is_any_of(","));
				if (oox_from.size() >= 2)
				{
					int x = boost::lexical_cast<double>(oox_from[0]) * pptx_mulipier;
					int y = boost::lexical_cast<double>(oox_from[1]) * pptx_mulipier;

					animationContext.set_animate_scale_from(x, y);
				}
			}

			if (animate_transform_attlist_.smil_to_)
			{
				const int pptx_mulipier = 100000;
				std::vector<std::wstring> oox_to;
				boost::split(oox_to, animate_transform_attlist_.smil_to_.value(), boost::is_any_of(","));
				if (oox_to.size() >= 2)
				{
					int x = boost::lexical_cast<double>(oox_to[0]) * pptx_mulipier;
					int y = boost::lexical_cast<double>(oox_to[1]) * pptx_mulipier;

					animationContext.set_animate_scale_to(x, y);
				}				
			}

			animationContext.end_animate_scale();
			break;
		}
		case odf_types::svg_type::rotate:
		{
			if (animate_transform_attlist_.smil_by_)
			{
				try
				{
					const int pptx_muliplier = 60000;
					by = boost::lexical_cast<double>(animate_transform_attlist_.smil_by_.value()) * pptx_muliplier;
				}
				catch (...)
				{
					by = 0;
				}
			}

			animationContext.start_animate_rotate();
			animationContext.set_animate_rotate_shape_id(shapeID);
			if (duration)		animationContext.set_animate_rotate_duration(duration.value());
			if (fill)			animationContext.set_animate_rotate_fill(fill.value());
			if (delay)			animationContext.set_animate_rotate_delay(delay.value());
			if (autoRev)		animationContext.set_animate_rotate_auto_reverse(autoRev.value());
			if (by)				animationContext.set_animate_rotate_by(by.value());

			animationContext.end_animate_rotate();
			break;
			break;
		}
		}
	}
}

void anim_animate_transform::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
	common_attlist_.add_attributes(Attributes);
	animate_transform_attlist_.add_attributes(Attributes);
}

void anim_animate_transform_attlist::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
	CP_APPLY_ATTR(L"smil:fill", smil_fill_);
	CP_APPLY_ATTR(L"smil:autoReverse", smil_auto_reverse_);
	CP_APPLY_ATTR(L"smil:targetElement", smil_target_element_);
	CP_APPLY_ATTR(L"smil:from", smil_from_);
	CP_APPLY_ATTR(L"smil:to", smil_to_);
	CP_APPLY_ATTR(L"smil:by", smil_by_);
	CP_APPLY_ATTR(L"svg:type", svg_type_);
}

}
}
