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
	#define SvgURL       NSCSS::NSProperties::CURL
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
