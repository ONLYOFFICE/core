﻿/*
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
#pragma once   

#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "../../Common/CPOptional.h"

namespace svg_path
{
	struct _point
	{
		_point(double _x, double _y) { x = _x; y = _y; }

		_CP_OPT(double) x;
		_CP_OPT(double) y;
	};
	struct _polyline
	{
		std::wstring command;
		std::vector<_point> points;
	};
	struct _pointS
	{
		_pointS(const std::wstring& _x, const std::wstring& _y) { x = _x; y = _y; }

		_CP_OPT(std::wstring) x;
		_CP_OPT(std::wstring) y;
	};
	struct _polylineS
	{
		std::wstring command;
		std::vector<_pointS> points;
	};

	bool parseVml(std::vector<_polyline>& Polyline, const std::wstring& path);
	bool parseSvgD(std::vector<_polyline>& Polyline, const std::wstring& path, bool bWrongPositionAfterZ, bool& bIsClosed, bool& bIsStroked);
	bool parsePolygon(std::vector<_polyline>& Polyline, const std::wstring& path, bool bWrongPositionAfterZ, bool closed);

	bool parseSvgS(std::vector<_polylineS>& Polyline, const std::wstring& path, bool bWrongPositionAfterZ, bool& bIsClosed, bool& bIsStroked);
}