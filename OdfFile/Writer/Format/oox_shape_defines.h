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

		std::wstring			odf_type_name;
	};
};
