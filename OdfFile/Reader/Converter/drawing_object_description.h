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

//#include "oox_drawing_fills.h"

namespace cpdoccore {
namespace oox {

struct _rect
{
	_rect(){}
	_rect(double cx_, double cy_, double x_, double y_) : x(x_), y(y_), cx(cx_), cy(cy_) {}

    double cx = 0;
    double cy = 0;
    double x = 0;
    double y = 0;
};
enum _anchor_rel
{
	anchor_none = 0,
	anchor_start = 1,
	anchor_end = 2
};
struct drawing_object_description
{
	bool									bNotes_;

	oox::_rels_type							type_;
	bool									bInner_;

	std::wstring							name_;	
	std::wstring							descriptor_;
	std::wstring 							xml_id_;

	_CP_OPT(_rect)							svg_rect_;

	_anchor_rel								anchor_rel_;
	std::wstring							anchor_;
    double									anchor_x_;
    double									anchor_y_;

	_CP_OPT(_INT32)							owner_cx_;
	_CP_OPT(_INT32)							owner_cy_;

	_action_desc							action_;
	std::vector<_hlink_desc>				hlinks_;

	bool									hidden_;

	std::vector<odf_reader::_property>		additional_;	//shape properties
	
	std::wstring							xlink_href_;	//link to external object

	_oox_fill								fill_;

	bool									in_group_;
	bool									lined_;
	bool									connector_;
	int										shape_type_;	//default - frame
	_CP_OPT(std::wstring)					start_shape_id;
	_CP_OPT(int)							start_shape_glue_point;
	_CP_OPT(std::wstring)					end_shape_id;
	_CP_OPT(int)							end_shape_glue_point;
	_CP_OPT(std::wstring)					draw_type_;

	std::vector<drawing_object_description>	child_objects_;
};

}
}