/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "../Common/xml/simple_xml_writer.h"

#include "presetclass.h"
#include "presetid.h"
#include "color.h"
#include "bool.h"
#include "svg_type.h"
#include "smil_transitiontype.h"
#include "smil_attributename.h"
#include "smil_additive.h"
#include "smil_keytimes.h"
#include "smil_values.h"

namespace cpdoccore {
namespace odf_types {

class anim_par_attlist
{
public:
	void add_attributes(const xml::attributes_wc_ptr& Attributes);
	void apply_from(const anim_par_attlist& Other);
	void serialize(CP_ATTR_NODE);

	_CP_OPT(preset_class)			presentation_preset_class_;
	_CP_OPT(preset_id)				presentation_preset_id_;
	_CP_OPT(std::wstring)			presentation_preset_sub_type_;
	_CP_OPT(float)					smil_accelerate_;
	_CP_OPT(float)					smil_decelerate_;
};

class anim_audio_attlist
{
public:
	void add_attributes(const xml::attributes_wc_ptr& Attributes);
	void apply_from(const anim_audio_attlist& Other);
	void serialize(CP_ATTR_NODE);

	_CP_OPT(std::wstring)			xlink_href_;
	_CP_OPT(std::wstring)			anim_audio_level_;
};

class anim_transition_filter_attlist
{
public:
	void add_attributes(const xml::attributes_wc_ptr& Attributes);
	void apply_from(const anim_transition_filter_attlist& Other);
	void serialize(CP_ATTR_NODE);

	_CP_OPT(std::wstring)			smil_subtype_;
	_CP_OPT(smil_transition_type)	smil_type_;
	_CP_OPT(std::wstring)			smil_mode_;
	_CP_OPT(color)					smil_fadeColor_;
	_CP_OPT(std::wstring)			smil_direction_;
};

class anim_set_attlist
{
public:
	void add_attributes(const xml::attributes_wc_ptr& Attributes);
	void apply_from(const anim_set_attlist& Other);
	void serialize(CP_ATTR_NODE);

	_CP_OPT(std::wstring)			smil_to_;
};

class anim_animate_motion_attlist
{
public:
	void add_attributes(const xml::attributes_wc_ptr& Attributes);
	void apply_from(const anim_animate_motion_attlist& Other);
	void serialize(CP_ATTR_NODE);

	_CP_OPT(std::wstring)			svg_path_;
};

class anim_animate_color_attlist
{
public:
	void add_attributes(const xml::attributes_wc_ptr& Attributes);
	void apply_from(const anim_animate_color_attlist& Other);
	void serialize(CP_ATTR_NODE);

	_CP_OPT(std::wstring)			smil_to_;
	_CP_OPT(std::wstring)			smil_by_;
	_CP_OPT(std::wstring)			presentation_master_element_;
	_CP_OPT(std::wstring)			anim_color_interpolation_;
	_CP_OPT(std::wstring)			anim_color_interpolation_direction;
};

class anim_animate_attlist
{
public:
	void add_attributes(const xml::attributes_wc_ptr& Attributes);
	void apply_from(const anim_animate_attlist& Other);
	void serialize(CP_ATTR_NODE);

	_CP_OPT(smil_values)			smil_values_;
	_CP_OPT(smil_key_times)			smil_key_times_;
	_CP_OPT(std::wstring)			anim_formula_;
	_CP_OPT(std::wstring)			smil_calc_mode_;
	_CP_OPT(std::wstring)			smil_from_;
	_CP_OPT(std::wstring)			smil_to_;
	_CP_OPT(std::wstring)			smil_by_;
	_CP_OPT(smil_additive)			smil_additive_;
	_CP_OPT(Bool)					smil_auto_reverse_;
	_CP_OPT(svg_type)				svg_type_;

};

class anim_animate_transform_attlist
{
public:
	void add_attributes(const xml::attributes_wc_ptr& Attributes);
	void apply_from(const anim_animate_transform_attlist& Other);
	void serialize(CP_ATTR_NODE);

	_CP_OPT(Bool)					smil_auto_reverse_;
	_CP_OPT(std::wstring)			smil_from_;
	_CP_OPT(std::wstring)			smil_to_;
	_CP_OPT(std::wstring)			smil_by_;
	_CP_OPT(svg_type)				svg_type_;
};

} // namespace odf_types
} // namespace cpdoccore