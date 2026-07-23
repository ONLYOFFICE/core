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

#include <vector>
#include <string>

#include <CPSharedPtr.h>
#include <CPOptional.h>

namespace cpdoccore
{

	class oox_shape;
	typedef _CP_PTR(oox_shape) oox_shape_ptr;

	class oox_shape
	{
	public:
		static oox_shape_ptr create(int ooxPrstGeomType);

		oox_shape();

		struct _equation
		{
			std::wstring name;
			std::wstring formula;

		};
		struct _handle
		{		
			_CP_OPT(std::wstring) position;	

			_CP_OPT(std::wstring) y_minimum;
			_CP_OPT(std::wstring) y_maximum;
			_CP_OPT(std::wstring) x_minimum;
			_CP_OPT(std::wstring) x_maximum;
			_CP_OPT(std::wstring) r_minimum;
			_CP_OPT(std::wstring) r_maximum;

			_CP_OPT(bool) handle_swiched;
		};

        void add(std::wstring name,std::wstring frmla);

		std::vector<_equation>	equations;
		std::vector<_handle>	handles;		
		
		std::wstring			enhanced_path;
		std::wstring			modifiers;
		std::wstring			text_areas;

		_CP_OPT(std::wstring)	view_box;
		_CP_OPT(std::wstring)	sub_view_size;
		_CP_OPT(std::wstring)	glue_points;
		_CP_OPT(std::wstring)	glue_points_leaving_directions;

		_CP_OPT(std::wstring)	path_stretchpoint_x;
		_CP_OPT(std::wstring)	path_stretchpoint_y;

		std::wstring			odf_type_name;
	};
};
