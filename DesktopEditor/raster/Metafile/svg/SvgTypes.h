/*
 * (c) Copyright Ascensio System SIA 2010-2024
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

#ifndef SVGTYPES_H
#define SVGTYPES_H

#include <cstring>
#include <cmath>

#include "../../../Common/3dParty/html/css/src/StyleProperties.h"
#include "SvgUtils.h"

namespace SVG
{
	#define MapCI std::map<std::wstring, std::wstring>::const_iterator

	#define SvgDigit     NSCSS::NSProperties::CDigit
	#define SvgString    NSCSS::NSProperties::CString
	#define SvgColor     NSCSS::NSProperties::CColor
	#define SvgEnum      NSCSS::NSProperties::CEnum

	#define SvgTransform NSCSS::NSProperties::CTransform
	#define SvgFont      NSCSS::NSProperties::CFont
	#define SvgText      NSCSS::NSProperties::CText

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
