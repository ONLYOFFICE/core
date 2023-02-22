#ifndef SVGTYPES_H
#define SVGTYPES_H

#include <cstring>
#include <cmath>

#include "../../../Common/3dParty/html/css/src/StyleProperties.h"

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
		SvgEnum m_oLineCap;
		SvgEnum m_oLineJoin;
	};

	struct Point
	{
		double dX;
		double dY;

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

		void Rotate(double dAngle, const Point& oPoint)
		{
			double dOldX = dX;
			double dOldY = dY;

			dAngle *= 3.1415926535 / 180;

			dX = dOldX + cos(dAngle) * (oPoint.dX - dOldX) - sin(dAngle) * (oPoint.dY - dOldY);
			dY = dOldY + sin(dAngle) * (oPoint.dX - dOldX) + cos(dAngle) * (oPoint.dY - dOldY);
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
