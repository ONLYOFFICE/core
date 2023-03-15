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

	_CP_OPT(_rect)							svg_rect_;

	_anchor_rel								anchor_rel_;
	std::wstring							anchor_;
    double									anchor_x_;
    double									anchor_y_;

	_CP_OPT(_INT32)							owner_cx_;
	_CP_OPT(_INT32)							owner_cy_;

	_action_desc							action_;
	std::vector<_hlink_desc>				hlinks_;

	std::vector<odf_reader::_property>		additional_;	//shape properties
	
	std::wstring							xlink_href_;	//ссылка на внешний объект
	std::wstring							clipping_string_;

	_oox_fill								fill_;

	bool									in_group_;
	bool									lined_;
	bool									connector_;
	int										shape_type_;	//default - frame

	std::vector<drawing_object_description>	child_objects_;
};

}
}