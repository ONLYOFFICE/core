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
