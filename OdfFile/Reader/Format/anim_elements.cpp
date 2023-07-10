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

#include <xml/xmlchar.h>
#include <xml/simple_xml_writer.h>
#include <boost/algorithm/string.hpp>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {



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
			 if (common_attlist_.presentation_node_type_.value() == L"timing-root")			presentationNodeType = L"tmRoot";
		else if (common_attlist_.presentation_node_type_.value() == L"on-click")				presentationNodeType = L"clickEffect";
		else if (common_attlist_.presentation_node_type_.value() == L"after-previous")			presentationNodeType = L"afterEffect";
		else if (common_attlist_.presentation_node_type_.value() == L"with-previous")			presentationNodeType = L"withEffect";
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
		clockvalue delayClockvalue = clockvalue::parse(common_attlist_.smil_begin_.get());
		if (delayClockvalue.get_value() != -1)
			delay = boost::lexical_cast<std::wstring>(delayClockvalue.get_value());
		else if (common_attlist_.smil_begin_.get() == L"next")
			delay = L"indefinite";
	}
	
	// NOTE: Юзлес штука
	// TODO: Figure out correct value
	end = boost::none;

	if (par_attlist_.presentation_preset_class_)
	{
		if (par_attlist_.presentation_preset_class_.value() == L"entrance")
		{
			presentationPresetClass = L"entr";
			presentationPresetId = convert_entrance_preset_id();
		}
		else if (par_attlist_.presentation_preset_class_.value() == L"exit")
		{
			presentationPresetClass = L"exit";
			presentationPresetId = convert_exit_preset_id();
		}
		else if (par_attlist_.presentation_preset_class_.value() == L"emphasis")
		{
			presentationPresetClass = L"emph";
			presentationPresetId = convert_emphasis_preset_id();
		}
		else if (par_attlist_.presentation_preset_class_.value() == L"motion-path")
		{
			presentationPresetClass = L"path";
			presentationPresetId = convert_motion_path_preset_id();
		}
		else if (par_attlist_.presentation_preset_class_.value() == L"ole-action")	
			presentationPresetClass = L"verb";
		else if (par_attlist_.presentation_preset_class_.value() == L"media-call")	
			presentationPresetClass = L"mediacall";
		else
			presentationPresetClass = L"custom";
	}

	if (par_attlist_.presentation_preset_class_)
	{
		std::wstring presetClass = par_attlist_.presentation_preset_class_.value();
		if(presetClass == L"ent")
		presentationPresetId = boost::none;
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

_CP_OPT(int) anim_par::convert_entrance_preset_id()
{
	if (par_attlist_.presentation_preset_id_)
	{
		const std::wstring& presetId = par_attlist_.presentation_preset_id_.value();

			 if (presetId == L"ooo-entrance-appear")					return 1;
		else if (presetId == L"ooo-entrance-fly-in") 					return 2;
		else if (presetId == L"ooo-entrance-venetian-blinds")			return 3;
		else if (presetId == L"ooo-entrance-box") 						return 4;
		else if (presetId == L"ooo-entrance-checkerboard") 				return 5;
		else if (presetId == L"ooo-entrance-circle") 					return 6;
		else if (presetId == L"ooo-entrance-fly-in-slow") 				return 7;
		else if (presetId == L"ooo-entrance-diamond") 					return 8;
		else if (presetId == L"ooo-entrance-dissolve-in")				return 9;
		else if (presetId == L"ooo-entrance-fade-in") 					return 10;
		else if (presetId == L"ooo-entrance-flash-once")				return 11;
		else if (presetId == L"ooo-entrance-peek-in") 					return 12;
		else if (presetId == L"ooo-entrance-plus") 						return 13;
		else if (presetId == L"ooo-entrance-random-bars")				return 14;
		else if (presetId == L"ooo-entrance-spiral-in")					return 15;
		else if (presetId == L"ooo-entrance-split") 					return 16;
		else if (presetId == L"ooo-entrance-stretchy") 					return 17;
		else if (presetId == L"ooo-entrance-diagonal-squares") 			return 18;
		else if (presetId == L"ooo-entrance-swivel") 					return 19;
		else if (presetId == L"ooo-entrance-wedge") 					return 20;
		else if (presetId == L"ooo-entrance-wheel") 					return 21;
		else if (presetId == L"ooo-entrance-wipe") 						return 22;
		else if (presetId == L"ooo-entrance-zoom") 						return 23;
		else if (presetId == L"ooo-entrance-random") 					return 24;
		else if (presetId == L"ooo-entrance-boomerang") 				return 25;
		else if (presetId == L"ooo-entrance-bounce") 					return 26;
		else if (presetId == L"ooo-entrance-colored-lettering") 		return 27;
		else if (presetId == L"ooo-entrance-movie-credits") 			return 28;
		else if (presetId == L"ooo-entrance-ease-in") 					return 29;
		else if (presetId == L"ooo-entrance-float") 					return 30;
		else if (presetId == L"ooo-entrance-turn-and-grow") 			return 31;
		else if (presetId == L"ooo-entrance-breaks") 					return 34;
		else if (presetId == L"ooo-entrance-pinwheel") 					return 35;
		else if (presetId == L"ooo-entrance-rise-up") 					return 37;
		else if (presetId == L"ooo-entrance-falling-in") 				return 38;
		else if (presetId == L"ooo-entrance-thread") 					return 39;
		else if (presetId == L"ooo-entrance-unfold") 					return 40;
		else if (presetId == L"ooo-entrance-whip") 						return 41;
		else if (presetId == L"ooo-entrance-ascend") 					return 42;
		else if (presetId == L"ooo-entrance-center-revolve") 			return 43;
		else if (presetId == L"ooo-entrance-fade-in-and-swivel") 		return 45;
		else if (presetId == L"ooo-entrance-descend") 					return 47;
		else if (presetId == L"ooo-entrance-sling") 					return 48;
		else if (presetId == L"ooo-entrance-spin-in") 					return 49;
		else if (presetId == L"ooo-entrance-compress") 					return 50;
		else if (presetId == L"ooo-entrance-magnify") 					return 51;
		else if (presetId == L"ooo-entrance-curve-up") 					return 52;
		else if (presetId == L"ooo-entrance-fade-in-and-zoom") 			return 53;
		else if (presetId == L"ooo-entrance-glide") 					return 54;
		else if (presetId == L"ooo-entrance-expand") 					return 55;
		else if (presetId == L"ooo-entrance-flip") 						return 56;
		else if (presetId == L"ooo-entrance-fold") 						return 58;
	}

	return boost::none;
}

_CP_OPT(int) anim_par::convert_emphasis_preset_id()
{
	if (par_attlist_.presentation_preset_id_)
	{
		const std::wstring& presetId = par_attlist_.presentation_preset_id_.value();

			 if (presetId == L"ooo-emphasis-fill-color") 					return 1;
		else if (presetId == L"ooo-emphasis-font") 							return 2;
		else if (presetId == L"ooo-emphasis-font-color") 					return 3;
		else if (presetId == L"ooo-emphasis-font-size") 					return 4;
		else if (presetId == L"ooo-emphasis-font-style") 					return 5;
		else if (presetId == L"ooo-emphasis-grow-and-shrink") 				return 6;
		else if (presetId == L"ooo-emphasis-line-color") 					return 7;
		else if (presetId == L"ooo-emphasis-spin") 							return 8;
		else if (presetId == L"ooo-emphasis-transparency") 					return 9;
		else if (presetId == L"ooo-emphasis-bold-flash") 					return 10;
		else if (presetId == L"ooo-emphasis-blast") 						return 14;
		else if (presetId == L"ooo-emphasis-bold-reveal") 					return 15;
		else if (presetId == L"ooo-emphasis-color-over-by-word") 			return 16;
		else if (presetId == L"ooo-emphasis-reveal-underline") 				return 18;
		else if (presetId == L"ooo-emphasis-color-blend") 					return 19;
		else if (presetId == L"ooo-emphasis-color-over-by-letter") 			return 20;
		else if (presetId == L"ooo-emphasis-complementary-color") 			return 21;
		else if (presetId == L"ooo-emphasis-complementary-color-2") 		return 22;
		else if (presetId == L"ooo-emphasis-contrasting-color") 			return 23;
		else if (presetId == L"ooo-emphasis-darken") 						return 24;
		else if (presetId == L"ooo-emphasis-desaturate") 					return 25;
		else if (presetId == L"ooo-emphasis-flash-bulb") 					return 26;
		else if (presetId == L"ooo-emphasis-flicker") 						return 27;
		else if (presetId == L"ooo-emphasis-grow-with-color") 				return 28;
		else if (presetId == L"ooo-emphasis-lighten") 						return 30;
		else if (presetId == L"ooo-emphasis-style-emphasis") 				return 31;
		else if (presetId == L"ooo-emphasis-teeter") 						return 32;
		else if (presetId == L"ooo-emphasis-vertical-highlight") 			return 33;
		else if (presetId == L"ooo-emphasis-wave") 							return 34;
		else if (presetId == L"ooo-emphasis-blink") 						return 35;
		else if (presetId == L"ooo-emphasis-shimmer") 						return 36;
	}

	return boost::none;
}

_CP_OPT(int) anim_par::convert_exit_preset_id()
{
	if (par_attlist_.presentation_preset_id_)
	{
		const std::wstring& presetId = par_attlist_.presentation_preset_id_.value();

		if (presetId == L"ooo-exit-disappear")					return 1;
		else if (presetId == L"ooo-exit-fly-out")				return 2;
		else if (presetId == L"ooo-exit-venetian-blinds")		return 3;
		else if (presetId == L"ooo-exit-box")					return 4;
		else if (presetId == L"ooo-exit-checkerboard")			return 5;
		else if (presetId == L"ooo-exit-circle")				return 6;
		else if (presetId == L"ooo-exit-crawl-out")				return 7;
		else if (presetId == L"ooo-exit-diamond")				return 8;
		else if (presetId == L"ooo-exit-dissolve")				return 9;
		else if (presetId == L"ooo-exit-fade-out")				return 10;
		else if (presetId == L"ooo-exit-flash-once")			return 11;
		else if (presetId == L"ooo-exit-peek-out")				return 12;
		else if (presetId == L"ooo-exit-plus")					return 13;
		else if (presetId == L"ooo-exit-random-bars")			return 14;
		else if (presetId == L"ooo-exit-spiral-out")			return 15;
		else if (presetId == L"ooo-exit-split")					return 16;
		else if (presetId == L"ooo-exit-collapse")				return 17;
		else if (presetId == L"ooo-exit-diagonal-squares")		return 18;
		else if (presetId == L"ooo-exit-swivel")				return 19;
		else if (presetId == L"ooo-exit-wedge")					return 20;
		else if (presetId == L"ooo-exit-wheel")					return 21;
		else if (presetId == L"ooo-exit-wipe")					return 22;
		else if (presetId == L"ooo-exit-zoom")					return 23;
		else if (presetId == L"ooo-exit-random")				return 24;
		else if (presetId == L"ooo-exit-boomerang")				return 25;
		else if (presetId == L"ooo-exit-bounce")				return 26;
		else if (presetId == L"ooo-exit-colored-lettering")		return 27;
		else if (presetId == L"ooo-exit-movie-credits")			return 28;
		else if (presetId == L"ooo-exit-ease-out")				return 29;
		else if (presetId == L"ooo-exit-float")					return 30;
		else if (presetId == L"ooo-exit-turn-and-grow")			return 31;
		else if (presetId == L"ooo-exit-breaks")				return 34;
		else if (presetId == L"ooo-exit-pinwheel")				return 35;
		else if (presetId == L"ooo-exit-sink-down")				return 37;
		else if (presetId == L"ooo-exit-swish")					return 38;
		else if (presetId == L"ooo-exit-thread")				return 39;
		else if (presetId == L"ooo-exit-unfold")				return 40;
		else if (presetId == L"ooo-exit-whip")					return 41;
		else if (presetId == L"ooo-exit-descend")				return 42;
		else if (presetId == L"ooo-exit-center-revolve")		return 43;
		else if (presetId == L"ooo-exit-fade-out-and-swivel")	return 45;
		else if (presetId == L"ooo-exit-ascend")				return 47;
		else if (presetId == L"ooo-exit-sling")					return 48;
		else if (presetId == L"ooo-exit-fade-out-and-zoom")		return 53;
		else if (presetId == L"ooo-exit-contract")				return 55;
		else if (presetId == L"ooo-exit-spin-out")				return 49;
		else if (presetId == L"ooo-exit-stretchy")				return 50;
		else if (presetId == L"ooo-exit-magnify")				return 51;
		else if (presetId == L"ooo-exit-curve-down")			return 52;
		else if (presetId == L"ooo-exit-glide")					return 54;
		else if (presetId == L"ooo-exit-flip")					return 56;
		else if (presetId == L"ooo-exit-fold")					return 58;
	}

	return boost::none;
}

_CP_OPT(int) anim_par::convert_motion_path_preset_id()
{
	if (par_attlist_.presentation_preset_id_)
	{
		const std::wstring& presetId = par_attlist_.presentation_preset_id_.value();

			 if (presetId == L"ooo-motionpath-4-point-star")			return 16;
		else if (presetId == L"ooo-motionpath-5-point-star")			return 5;
		else if (presetId == L"ooo-motionpath-6-point-star")			return 11;
		else if (presetId == L"ooo-motionpath-8-point-star")			return 17;
		else if (presetId == L"ooo-motionpath-circle")					return 1;
		else if (presetId == L"ooo-motionpath-crescent-moon")			return 6;
		else if (presetId == L"ooo-motionpath-diamond")					return 3;
		else if (presetId == L"ooo-motionpath-equal-triangle")			return 13;
		else if (presetId == L"ooo-motionpath-oval")					return 12;
		else if (presetId == L"ooo-motionpath-heart")					return 9;
		else if (presetId == L"ooo-motionpath-hexagon")					return 4;
		else if (presetId == L"ooo-motionpath-octagon")					return 10;
		else if (presetId == L"ooo-motionpath-parallelogram")			return 14;
		else if (presetId == L"ooo-motionpath-pentagon")				return 15;
		else if (presetId == L"ooo-motionpath-right-triangle")			return 2;
		else if (presetId == L"ooo-motionpath-square")					return 7;
		else if (presetId == L"ooo-motionpath-teardrop")				return 18;
		else if (presetId == L"ooo-motionpath-trapezoid")				return 8;
		else if (presetId == L"ooo-motionpath-arc-down")				return 37;
		else if (presetId == L"ooo-motionpath-arc-left")				return 51;
		else if (presetId == L"ooo-motionpath-arc-right")				return 58;
		else if (presetId == L"ooo-motionpath-arc-up")					return 44;
		else if (presetId == L"ooo-motionpath-bounce-left")				return 41;
		else if (presetId == L"ooo-motionpath-bounce-right")			return 54;
		else if (presetId == L"ooo-motionpath-curvy-left")				return 48;
		else if (presetId == L"ooo-motionpath-curvy-right")				return 61;
		else if (presetId == L"ooo-motionpath-decaying-wave")			return 60;
		else if (presetId == L"ooo-motionpath-diagonal-down-right")		return 49;
		else if (presetId == L"ooo-motionpath-diagonal-up-right")		return 56;
		else if (presetId == L"ooo-motionpath-down")					return 42;
		else if (presetId == L"ooo-motionpath-funnel")					return 52;
		else if (presetId == L"ooo-motionpath-spring")					return 53;
		else if (presetId == L"ooo-motionpath-stairs-down")				return 62;
		else if (presetId == L"ooo-motionpath-turn-down")				return 50;
		else if (presetId == L"ooo-motionpath-turn-down-right")			return 36;
		else if (presetId == L"ooo-motionpath-turn-up")					return 43;
		else if (presetId == L"ooo-motionpath-turn-up-right")			return 57;
		else if (presetId == L"ooo-motionpath-up")						return 64;
		else if (presetId == L"ooo-motionpath-wave")					return 47;
		else if (presetId == L"ooo-motionpath-zigzag")					return 38;
		else if (presetId == L"ooo-motionpath-bean")					return 31;
		else if (presetId == L"ooo-motionpath-buzz-saw")				return 25;
		else if (presetId == L"ooo-motionpath-curved-square")			return 20;
		else if (presetId == L"ooo-motionpath-curved-x")				return 21;
		else if (presetId == L"ooo-motionpath-curvy-star")				return 23;
		else if (presetId == L"ooo-motionpath-figure-8-four")			return 28;
		else if (presetId == L"ooo-motionpath-horizontal-figure-8")		return 26;
		else if (presetId == L"ooo-motionpath-inverted-square")			return 34;
		else if (presetId == L"ooo-motionpath-inverted-triangle")		return 33;
		else if (presetId == L"ooo-motionpath-loop-de-loop")			return 24;
		else if (presetId == L"ooo-motionpath-neutron")					return 29;
		else if (presetId == L"ooo-motionpath-peanut")					return 27;
		else if (presetId == L"ooo-motionpath-clover")					return 32;
		else if (presetId == L"ooo-motionpath-pointy-star")				return 19;
		else if (presetId == L"ooo-motionpath-swoosh")					return 30;
		else if (presetId == L"ooo-motionpath-vertical-figure-8")		return 22;
		else if (presetId == L"ooo-motionpath-left")					return 35;
		else if (presetId == L"ooo-motionpath-right")					return 63;
		else if (presetId == L"ooo-motionpath-spiral-left")				return 55;
		else if (presetId == L"ooo-motionpath-spiral-right")			return 46;
		else if (presetId == L"ooo-motionpath-sine-wave")				return 40;
		else if (presetId == L"ooo-motionpath-s-curve-1")				return 59;
		else if (presetId == L"ooo-motionpath-s-curve-2")				return 39;
		else if (presetId == L"ooo-motionpath-heartbeat")				return 45;
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
	oox::pptx_animation_context& animationContext = Context.get_slide_context().get_animation_context();

	animationContext.start_seq_animation();

	if (attlist_.presentation_node_type_)	animationContext.set_seq_animation_presentation_node_type(attlist_.presentation_node_type_.value());
	if (attlist_.smil_direction_)			animationContext.set_seq_animation_direction(attlist_.smil_direction_.value());
	if (attlist_.smil_restart_)				animationContext.set_seq_animation_restart(attlist_.smil_restart_.value());
	if (attlist_.smil_dur_)					animationContext.set_seq_animation_dur(attlist_.smil_dur_.value().get_value());
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
    CP_APPLY_ATTR(L"smil:subtype",		smil_subtype_);
    CP_APPLY_ATTR(L"smil:type",			smil_type_);
	CP_APPLY_ATTR(L"smil:fadeColor",	smil_fadeColor_);
	CP_APPLY_ATTR(L"smil:mode",			smil_mode_);
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
	std::wstring shapeId = L"2";

	if (filter_attlist_.smil_mode_)
	{
		if (filter_attlist_.smil_mode_.value() == L"out")
			transition = L"out";
	}

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
	_CP_OPT(std::wstring)		target_element;
	_CP_OPT(std::wstring)		attribute_name;
	_CP_OPT(std::wstring)		to_value;

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
		clockvalue delayClockvalue = clockvalue::parse(common_attlist_.smil_begin_.get());
		if (delayClockvalue.get_value() != -1)
			delay = boost::lexical_cast<std::wstring>(delayClockvalue.get_value());
		else
			delay = boost::none;
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
		// TODO: Figure out correct value
		target_element = L"2";
	}

	if (set_attlist_.smil_attribute_name_)
	{
		if (set_attlist_.smil_attribute_name_.value() == L"visibility")
			attribute_name = L"style.visibility";
	}

	if (set_attlist_.smil_to_)
	{
		if (set_attlist_.smil_to_.value() == L"visible")
			to_value = L"visible";
		else if(set_attlist_.smil_to_.value() == L"hidden")
			to_value = L"hidden";
	}

	oox::pptx_animation_context& animationContext = Context.get_slide_context().get_animation_context();

	animationContext.start_set();
	if (direction)			animationContext.set_set_direction(direction.value());
	if (restart)			animationContext.set_set_restart(restart.value());
	if (duration)			animationContext.set_set_duration(duration.value());
	if (delay)				animationContext.set_set_delay(delay.value());
	if (end)				animationContext.set_set_end(end.value());
	if (fill)				animationContext.set_set_fill(fill.value());
	if (target_element)		animationContext.set_set_target_element(target_element.value());
	if (attribute_name)		animationContext.set_set_attribute_name(attribute_name.value());
	if (to_value)			animationContext.set_set_to_value(to_value.value());
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

	animationContext.start_animate_motion();

	if (common_attlist_.presentation_node_type_)			animationContext.set_animate_motion_presentation_node_type(common_attlist_.presentation_node_type_.value());
	if (common_attlist_.smil_direction_)					animationContext.set_animate_motion_direction(common_attlist_.smil_direction_.value());
	if (common_attlist_.smil_restart_)						animationContext.set_animate_motion_restart(common_attlist_.smil_restart_.value());
	if (common_attlist_.smil_dur_)							animationContext.set_animate_motion_dur(common_attlist_.smil_dur_.value().get_value());
	if (common_attlist_.smil_begin_)						animationContext.set_animate_motion_delay(common_attlist_.smil_begin_.value());
	if (common_attlist_.smil_end_)							animationContext.set_animate_motion_end(common_attlist_.smil_end_.value());

	if (animate_motion_attlist_.smil_fill_)					animationContext.set_animate_motion_fill(animate_motion_attlist_.smil_fill_.value());
	if (animate_motion_attlist_.smil_target_element_)		animationContext.set_animate_motion_target_element(L"-1");
	if (animate_motion_attlist_.svg_path_)					animationContext.set_animate_motion_svg_path(animate_motion_attlist_.svg_path_.value());
	
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
	CP_APPLY_ATTR(L"presentation:master-elemen",			presentation_master_element_);
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

	colorSpace = L"rgb";
	
	if (common_attlist_.smil_dur_)
		duration = common_attlist_.smil_dur_->get_value();
	else
		duration = 1;

	if (common_attlist_.smil_begin_)
	{
		clockvalue delayClockvalue = clockvalue::parse(common_attlist_.smil_begin_.get());
		if (delayClockvalue.get_value() != -1)
			delay = boost::lexical_cast<std::wstring>(delayClockvalue.get_value());
		else
			delay = L"0";
	}

	attributeName = L"ppt_c";

	if (animate_color_attlist_.smil_to_)
	{
		toValue = animate_color_attlist_.smil_to_.value();
		boost::algorithm::erase_all(toValue.value(), L"#");
	}

	oox::pptx_animation_context& animationContext = Context.get_slide_context().get_animation_context();

	animationContext.start_animate_color();
	if (colorSpace)			animationContext.set_animate_color_color_space(colorSpace.value());
	if (duration)			animationContext.set_animate_color_duration(duration.value());
	if (delay)				animationContext.set_animate_color_delay(delay.value());
	if (attributeName)		animationContext.set_animate_color_attribute_name(attributeName.value());
	if (toValue)			animationContext.set_animate_color_to_value(toValue.value());
	animationContext.end_animate_color();
}

void anim_animate_color::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
	common_attlist_.add_attributes(Attributes);
	animate_color_attlist_.add_attributes(Attributes);
}

void anim_animate_attlist::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
	CP_APPLY_ATTR(L"smil:dur",					smil_dur_);
	CP_APPLY_ATTR(L"smil:targetElement",		smil_target_element_);
	CP_APPLY_ATTR(L"smil:attributeName",		smil_attribute_name_);
	CP_APPLY_ATTR(L"smil:values",				smil_values_);
	CP_APPLY_ATTR(L"smil:keyTimes",				smil_key_times_);
	CP_APPLY_ATTR(L"smil:calcMode",				smil_calc_mode_);
}

//////////////////////////////////////////////////////////////////////////
// anim:animate

const wchar_t* anim_animate::ns = L"anim";
const wchar_t* anim_animate::name = L"animate";

void anim_animate::pptx_convert(oox::pptx_conversion_context& Context)
{
	_CP_OPT(std::wstring)						calcmode;
	_CP_OPT(std::wstring)						valueType;
	_CP_OPT(std::wstring)						shapeID;
	_CP_OPT(int)								duration;
	_CP_OPT(std::wstring)						attributeName;

	if (animate_attlist_.smil_calc_mode_)
	{
		if (animate_attlist_.smil_calc_mode_.value() == L"discrete")	calcmode = L"discrete";
		else															calcmode = L"lin";
	}
	else
		calcmode = L"lin";

	valueType = L"num";
	shapeID = L"-1";
	duration = animate_attlist_.smil_dur_ ? animate_attlist_.smil_dur_->get_value() : 1;

	if (animate_attlist_.smil_attribute_name_)
	{
			 if (animate_attlist_.smil_attribute_name_.value() == L"visibility")		attributeName = L"style.visibility";
		else if (animate_attlist_.smil_attribute_name_.value() == L"width")				attributeName = L"ppt_w";
		else if (animate_attlist_.smil_attribute_name_.value() == L"height")			attributeName = L"ppt_h";
		else if (animate_attlist_.smil_attribute_name_.value() == L"x")					attributeName = L"ppt_x";
		else if (animate_attlist_.smil_attribute_name_.value() == L"y")					attributeName = L"ppt_y";
	}
	
	std::vector<std::wstring>	timesOdp;
	std::vector<int>			timesPptx;
	std::vector<std::wstring>	valuesOdp;
	std::vector<std::wstring>	valuesPptx;

	if (animate_attlist_.smil_key_times_)
		boost::split(timesOdp, animate_attlist_.smil_key_times_.value(), boost::is_any_of(";"));
	if (animate_attlist_.smil_values_)
		boost::split(valuesOdp, animate_attlist_.smil_values_.value(), boost::is_any_of(";"));

	for (size_t i = 0; i < timesOdp.size(); i++)
	{
		int keyTime = 0;
		const int pptx_time_mulipier = 100000;
		try
		{
			keyTime = boost::lexical_cast<double>(timesOdp[i]) * pptx_time_mulipier;
		}
		catch (...)
		{
			continue;
		}
		timesPptx.push_back(keyTime);
	}

	for (size_t i = 0; i < valuesOdp.size(); i++)
	{
		std::wstring value = valuesOdp[i];
		boost::replace_all(value, L"x", L"#ppt_x");
		boost::replace_all(value, L"y", L"#ppt_y");
		boost::replace_all(value, L"width", L"#ppt_w");
		boost::replace_all(value, L"height", L"#ppt_h");

		valuesPptx.push_back(value);
	}

	oox::pptx_animation_context& animationContext = Context.get_slide_context().get_animation_context();
	animationContext.start_animate();
	if (calcmode)			animationContext.set_animate_calc_mode(calcmode.value());
	if (valueType)			animationContext.set_animate_value_type(valueType.value());
	if (shapeID)			animationContext.set_animate_shape_id(shapeID.value());
	if (duration)			animationContext.set_animate_duration(duration.value());
	if (attributeName)		animationContext.set_animate_attribute_name(attributeName.value());

	if (timesPptx.size() == valuesPptx.size())
	{
		size_t size = timesPptx.size();

		for (size_t i = 0; i < size; i++)
		{
			animationContext.add_animate_keypoint(timesPptx[i], valuesPptx[i]);
		}
	}

	animationContext.end_animate();
}

void anim_animate::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
	animate_attlist_.add_attributes(Attributes);
}

}
}
