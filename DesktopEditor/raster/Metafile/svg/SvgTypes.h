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

#ifndef SVGTYPES_H
#define SVGTYPES_H

#include <cstring>
#include <cmath>

#include "../../../Common/3dParty/html/css/src/StyleProperties.h"
#include "SvgUtils.h"

namespace SVG
{
	using MapCI        = std::map<std::wstring, std::wstring>::const_iterator;

	using SvgDigit     = NSCSS::NSProperties::CDigit;
	using SvgString    = NSCSS::NSProperties::CString;
	using SvgColor     = NSCSS::NSProperties::CColor;
	using SvgURL       = NSCSS::NSProperties::CURL;
	using SvgEnum      = NSCSS::NSProperties::CEnum;

	using SvgTransform = NSCSS::NSProperties::CTransform;
	using SvgFont      = NSCSS::NSProperties::CFont;
	using SvgText      = NSCSS::NSProperties::CText;

	using SvgMatrix    = NSCSS::NSProperties::CMatrix;

	#define DEFAULT_FONT_SIZE 16

	struct TStroke
	{
		SvgColor m_oColor;
		SvgDigit m_oWidth;
		std::vector<double> m_arDash;
		SvgDigit m_oDashOffset;
		SvgEnum  m_oLineCap;
		SvgEnum  m_oLineJoin;
		SvgDigit m_oMiterlimit;
	};

	struct TClip
	{
		SvgColor  m_oHref;
		SvgString m_oRule;
	};

	struct TMarkers
	{
		SvgColor m_oStart;
		SvgColor m_oMid;
		SvgColor m_oEnd;
	};

	struct Point
	{
		double dX;
		double dY;

		bool operator==(const Point& oPoint)
		{
			return Equals(dX, oPoint.dX) && Equals(dY,  oPoint.dY);
		}

		Point& operator+=(const Point& oPoint)
		{
			dX += oPoint.dX;
			dY += oPoint.dY;

			return *this;
		}

		Point& operator-=(const Point& oPoint)
		{
			dX -= oPoint.dX;
			dY -= oPoint.dY;

			return *this;
		}

		Point operator+(const Point& oPoint)
		{
			return Point{dX + oPoint.dX, dY + oPoint.dY};
		}

		Point operator-(const Point& oPoint)
		{
			return Point{dX - oPoint.dX, dY - oPoint.dY};
		}

		void Rotate(double dAngle)
		{
			double dOldX = dX;

			dAngle *= 3.1415926535 / 180.;

			dX = dX * std::cos(dAngle) - dY * std::sin(dAngle);
			dY = dOldX * std::sin(dAngle) + dY * std::cos(dAngle);
		}
	};

	struct TRect
	{
		SvgDigit m_oX;
		SvgDigit m_oY;
		SvgDigit m_oWidth;
		SvgDigit m_oHeight;
	};

	struct TBounds
	{
		double m_dLeft;
		double m_dTop;
		double m_dRight;
		double m_dBottom;
	};
}

#endif // SVGTYPES_H
