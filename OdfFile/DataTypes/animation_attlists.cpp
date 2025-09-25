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

#include <iostream>
#include "animation_attlists.h"

#define _CP_APPLY_PROP(A, B) \
    if (B) \
        A = B;

#define _CP_APPLY_PROP2(A) \
    if (Other.A) \
        A = Other.A;

#define _CP_APPLY_PROP3(A) \
    if (Other->A) \
        A = Other->A;

namespace cpdoccore {
namespace odf_types {


	void anim_par_attlist::add_attributes(const xml::attributes_wc_ptr& Attributes)
	{
		CP_APPLY_ATTR(L"presentation:preset-class", presentation_preset_class_);
		CP_APPLY_ATTR(L"presentation:preset-id", presentation_preset_id_);
		CP_APPLY_ATTR(L"presentation:preset-sub-type", presentation_preset_sub_type_);
		CP_APPLY_ATTR(L"smil:accelerate", smil_accelerate_);
		CP_APPLY_ATTR(L"smil:decelerate", smil_decelerate_);
	}

	void anim_par_attlist::apply_from(const anim_par_attlist& Other)
	{
		_CP_APPLY_PROP2(presentation_preset_class_);
		_CP_APPLY_PROP2(presentation_preset_id_);
		_CP_APPLY_PROP2(presentation_preset_sub_type_);
		_CP_APPLY_PROP2(smil_accelerate_);
		_CP_APPLY_PROP2(smil_decelerate_);
	}

	void anim_par_attlist::serialize(CP_ATTR_NODE)
	{
		CP_XML_ATTR_OPT(L"presentation:preset-class",		presentation_preset_class_);
		CP_XML_ATTR_OPT(L"presentation:preset-id",			presentation_preset_id_);
		CP_XML_ATTR_OPT(L"presentation:preset-sub-type",	presentation_preset_sub_type_);
		CP_XML_ATTR_OPT(L"smil:accelerate",					smil_accelerate_);
		CP_XML_ATTR_OPT(L"smil:decelerate",					smil_decelerate_);
	}

	void anim_audio_attlist::add_attributes(const xml::attributes_wc_ptr& Attributes)
	{
		CP_APPLY_ATTR(L"xlink:href", xlink_href_);
		CP_APPLY_ATTR(L"anim:audio-level", anim_audio_level_);
	}

	void anim_audio_attlist::apply_from(const anim_audio_attlist& Other)
	{
		_CP_APPLY_PROP2(xlink_href_);
		_CP_APPLY_PROP2(anim_audio_level_);
	}

	void anim_audio_attlist::serialize(CP_ATTR_NODE)
	{
		CP_XML_ATTR_OPT(L"xlink:href",			xlink_href_);
		CP_XML_ATTR_OPT(L"anim:audio-level",	anim_audio_level_);
	}

	void anim_transition_filter_attlist::add_attributes(const xml::attributes_wc_ptr& Attributes)
	{
		CP_APPLY_ATTR(L"smil:subtype", smil_subtype_);
		CP_APPLY_ATTR(L"smil:type", smil_type_);
		CP_APPLY_ATTR(L"smil:fadeColor", smil_fadeColor_);
		CP_APPLY_ATTR(L"smil:mode", smil_mode_);
		CP_APPLY_ATTR(L"smil:direction", smil_direction_);
	}

	void anim_transition_filter_attlist::apply_from(const anim_transition_filter_attlist& Other)
	{
		_CP_APPLY_PROP2(smil_subtype_);
		_CP_APPLY_PROP2(smil_type_);
		_CP_APPLY_PROP2(smil_mode_);
		_CP_APPLY_PROP2(smil_fadeColor_);
		_CP_APPLY_PROP2(smil_direction_);
	}

	void anim_transition_filter_attlist::serialize(CP_ATTR_NODE)
	{
		CP_XML_ATTR_OPT(L"smil:subtype",			smil_subtype_);
		CP_XML_ATTR_OPT(L"smil:type",				smil_type_);
		CP_XML_ATTR_OPT(L"smil:mode",				smil_mode_);
		CP_XML_ATTR_OPT(L"smil:fadeColor",			smil_fadeColor_);
		CP_XML_ATTR_OPT(L"smil:direction",			smil_direction_);
	}

	void anim_set_attlist::add_attributes(const xml::attributes_wc_ptr& Attributes)
	{
		CP_APPLY_ATTR(L"smil:to", smil_to_);
	}

	void anim_set_attlist::apply_from(const anim_set_attlist& Other)
	{
		_CP_APPLY_PROP2(smil_to_);
	}

	void anim_set_attlist::serialize(CP_ATTR_NODE)
	{
		CP_XML_ATTR_OPT(L"smil:to",					smil_to_);
	}

	void anim_animate_motion_attlist::add_attributes(const xml::attributes_wc_ptr& Attributes)
	{
		CP_APPLY_ATTR(L"svg:path", svg_path_);
	}

	void anim_animate_motion_attlist::apply_from(const anim_animate_motion_attlist& Other)
	{
		_CP_APPLY_PROP2(svg_path_);
	}

	void anim_animate_motion_attlist::serialize(CP_ATTR_NODE)
	{
		CP_XML_ATTR_OPT(L"svg:path",		svg_path_);
	}

	void anim_animate_color_attlist::add_attributes(const xml::attributes_wc_ptr& Attributes)
	{
		CP_APPLY_ATTR(L"smil:to", smil_to_);
		CP_APPLY_ATTR(L"smil:by", smil_by_);
		CP_APPLY_ATTR(L"presentation:master-element", presentation_master_element_);
		CP_APPLY_ATTR(L"anim:color-interpolation", anim_color_interpolation_);
		CP_APPLY_ATTR(L"anim:color-interpolation-direction", anim_color_interpolation_direction);
	}

	void anim_animate_color_attlist::apply_from(const anim_animate_color_attlist& Other)
	{
		_CP_APPLY_PROP2(smil_to_);
		_CP_APPLY_PROP2(smil_by_);
		_CP_APPLY_PROP2(presentation_master_element_);
		_CP_APPLY_PROP2(anim_color_interpolation_);
		_CP_APPLY_PROP2(anim_color_interpolation_direction);
	}

	void anim_animate_color_attlist::serialize(CP_ATTR_NODE)
	{
		CP_XML_ATTR_OPT(L"smil:to",								smil_to_);
		CP_XML_ATTR_OPT(L"smil:by",								smil_by_);
		CP_XML_ATTR_OPT(L"presentation:master-element",			presentation_master_element_);
		CP_XML_ATTR_OPT(L"anim:color-interpolation",			anim_color_interpolation_);
		CP_XML_ATTR_OPT(L"anim:color-interpolation-direction",	anim_color_interpolation_direction);
	}

	void anim_animate_attlist::add_attributes(const xml::attributes_wc_ptr& Attributes)
	{
		CP_APPLY_ATTR(L"smil:values", smil_values_);
		CP_APPLY_ATTR(L"smil:keyTimes", smil_key_times_);
		CP_APPLY_ATTR(L"smil:calcMode", smil_calc_mode_);
		CP_APPLY_ATTR(L"smil:from", smil_from_);
		CP_APPLY_ATTR(L"smil:to", smil_to_);
		CP_APPLY_ATTR(L"smil:by", smil_by_);
		CP_APPLY_ATTR(L"smil:autoReverse", smil_auto_reverse_);
		CP_APPLY_ATTR(L"smil:additive", smil_additive_);
		CP_APPLY_ATTR(L"anim:formula", anim_formula_);
		CP_APPLY_ATTR(L"svg:type", svg_type_);
	}

	void anim_animate_attlist::apply_from(const anim_animate_attlist& Other)
	{
		_CP_APPLY_PROP2(smil_values_);
		_CP_APPLY_PROP2(smil_key_times_);
		_CP_APPLY_PROP2(anim_formula_);
		_CP_APPLY_PROP2(smil_calc_mode_);
		_CP_APPLY_PROP2(smil_from_);
		_CP_APPLY_PROP2(smil_to_);
		_CP_APPLY_PROP2(smil_by_);
		_CP_APPLY_PROP2(smil_additive_);
		_CP_APPLY_PROP2(smil_auto_reverse_);
		_CP_APPLY_PROP2(svg_type_);
	}

	void anim_animate_attlist::serialize(CP_ATTR_NODE)
	{
		CP_XML_ATTR_OPT(L"smil:values",					smil_values_);
		CP_XML_ATTR_OPT(L"smil:keyTimes",				smil_key_times_);
		CP_XML_ATTR_OPT(L"anim:formula",				anim_formula_);
		CP_XML_ATTR_OPT(L"smil:calcMode",				smil_calc_mode_);
		CP_XML_ATTR_OPT(L"smil:from",					smil_from_);
		CP_XML_ATTR_OPT(L"smil:to",						smil_to_);
		CP_XML_ATTR_OPT(L"smil:by",						smil_by_);
		CP_XML_ATTR_OPT(L"smil:additive",				smil_additive_);
		CP_XML_ATTR_OPT(L"smil:autoReverse",			smil_auto_reverse_);
		CP_XML_ATTR_OPT(L"svg:type",					svg_type_);
	}

	void anim_animate_transform_attlist::add_attributes(const xml::attributes_wc_ptr& Attributes)
	{
		CP_APPLY_ATTR(L"smil:autoReverse", smil_auto_reverse_);
		CP_APPLY_ATTR(L"smil:from", smil_from_);
		CP_APPLY_ATTR(L"smil:to", smil_to_);
		CP_APPLY_ATTR(L"smil:by", smil_by_);
		CP_APPLY_ATTR(L"svg:type", svg_type_);
	}


	void anim_animate_transform_attlist::apply_from(const anim_animate_transform_attlist& Other)
	{
		_CP_APPLY_PROP2(smil_auto_reverse_);
		_CP_APPLY_PROP2(smil_from_);
		_CP_APPLY_PROP2(smil_to_);
		_CP_APPLY_PROP2(smil_by_);
		_CP_APPLY_PROP2(svg_type_);
	}

	void anim_animate_transform_attlist::serialize(CP_ATTR_NODE)
	{
		CP_XML_ATTR_OPT(L"smil:autoReverse",	smil_auto_reverse_);
		CP_XML_ATTR_OPT(L"smil:from",			smil_from_);
		CP_XML_ATTR_OPT(L"smil:to",				smil_to_);
		CP_XML_ATTR_OPT(L"smil:by",				smil_by_);
		CP_XML_ATTR_OPT(L"svg:type",			svg_type_);
	}

} // namespace odf_types
} // namespace cpdoccore