#pragma once
#include "CommandsUtils.h"

namespace NSGeometry
{
	enum TrackType
	{
		ttNone		= 0x00,
		ttRotate	= 0x01,
		ttMove		= 0x02,
		ttBound		= 0x04
	};

	enum TrackTypeEx
	{
		tteNone			= 0,
		tteLeft			= 1,
		tteTop			= 2,
		tteRight		= 3,
		tteBottom		= 4,

		tteLeftTop		= 5,
		tteLeftBottom	= 6,
		tteRightTop		= 7,
		tteRightBottom	= 8,

		tteMove			= 9,
		tteRotate		= 10,
	};


	double GetDistance(double dX1, double dY1, double dX2, double dY2, float fAngle)
	{
		double dAngleRad = (M_PI * fAngle) / 180.0;
		double dA = cos(dAngleRad);
		double dB = sin(dAngleRad);
		double dC = -(dX2 * dA + dY2 * dB);

		return abs(dA * dX1 + dB * dY1 + dC) / _hypot(dA, dB);
	}
	BOOL IsInRect(PointF* pPoints, long x, long y)
	{
		HRGN hRgn;
		POINT pAr[4];
		
		pAr[0].x = (LONG)pPoints[0].X;
		pAr[0].y = (LONG)pPoints[0].Y;

		pAr[1].x = (LONG)pPoints[1].X;
		pAr[1].y = (LONG)pPoints[1].Y;

		pAr[2].x = (LONG)pPoints[3].X;
		pAr[2].y = (LONG)pPoints[3].Y;

		pAr[3].x = (LONG)pPoints[2].X;
		pAr[3].y = (LONG)pPoints[2].Y;

		hRgn = CreatePolygonRgn(pAr, 4, WINDING);
		BOOL bRes = (0 != PtInRegion(hRgn, int(x), int(y)));
		DeleteObject(hRgn);

		return bRes;
	}

	double GetDistanseToBorder(const double& x1, const double& y1, const double& x2, const double& y2, 
								const double& x3, const double& y3, const double& x4, const double& y4)
	{
		// расстояние между (x4, y4) и точкой пересечения прямых [(x1, y1)-(x2, y2)], [(x3, y3)-(x4, y4)]
		
		// возвращаем 0, если ничего пересчитывать не надо, иначе > 0
		double dZnam = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
		
		if (0 == dZnam)
			return 0;

		double dCisl1 = (x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3);
		double dCisl2 = (x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3);

		double dT1 = dCisl1 / dZnam;
		double dT2 = dCisl2 / dZnam;

		if ((0 >= dT1) || (dT1 >= 1) || (0 >= dT2) || (dT2 >= 1))
		{
			return 0;
		}

		// теперь берем точку пересечения...
		double dX = x1 + dT1 * (x2 - x1);
		double dY = y1 + dT1 * (y2 - y1);

		return _hypot(x4 - dX, y4 - dY);
	}

	double GetDistanseToBorder2(const double& x1, const double& y1, const double& x2, const double& y2, 
								const double& x3, const double& y3, const double& x4, const double& y4)
	{
		// расстояние между (x4, y4) и точкой пересечения прямых [(x1, y1)-(x2, y2)], [(x3, y3)-(x4, y4)]
		
		// возвращаем 0, если ничего пересчитывать не надо, иначе > 0
		double dZnam = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
		
		if (0 == dZnam)
			return 0;

		double dCisl1 = (x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3);
		double dCisl2 = (x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3);

		double dT1 = dCisl1 / dZnam;
		double dT2 = dCisl2 / dZnam;

		if ((0 >= dT1) || (dT1 > 1) || (0 >= dT2) || (dT2 > 1))
		{
			return 0;
		}

		// теперь берем точку пересечения...
		double dX = x1 + dT1 * (x2 - x1);
		double dY = y1 + dT1 * (y2 - y1);

		return _hypot(x4 - dX, y4 - dY);
	}

	bool PointOnSegment(const REAL& x1, const REAL& y1, const REAL& x2, const REAL& y2, 
			const REAL& ptx, const REAL& pty, const REAL& epsilon)
	{
		// point is inside the bundle - detect distance
		double dA = (y2-y1);
		double dB = (x1-x2);
		double dC = y1*(x2-x1)-x1*(y2-y1);

		double dDist = (dA*ptx + dB*pty + dC)/_hypot(dA, dB);

		if (fabs(dDist) < epsilon)
			return true;

		return false;
	}

	bool PointOnSegment(const double& x1, const double& y1, const double& x2, const double& y2, 
			const double& ptx, const double& pty)
	{
		// point is inside the bundle - detect distance
		double dChange = (x1 - ptx) * (x2 - ptx);
		if (0 < dChange)
			return false;
		dChange = (y1 - pty) * (y2 - pty);
		if (0 < dChange)
			return false;
		
		double dA = (y2-y1);
		double dB = (x1-x2);
		double dC = y1*(x2-x1)-x1*(y2-y1);

		double dDist = (dA*ptx + dB*pty + dC)/_hypot(dA, dB);

		if (0 == fabs(dDist))
			return true;

		return false;
	}
}

namespace NSGeometry
{
	class CDoublePoint
	{
	public:
		double x;
		double y;
	public:
		CDoublePoint()
		{
			x = 0;
			y = 0;
		}
		CDoublePoint(double dx, double dy)
		{
			x = dx;
			y = dy;
		}
		CDoublePoint& operator=(const CDoublePoint& oSrc)
		{
			x = oSrc.x;
			y = oSrc.y;

			return *this;
		}
		CDoublePoint(const CDoublePoint& oSrc)
		{
			*this = oSrc;
		}
	};

	bool PointOnSegment(const CDoublePoint& p1, const CDoublePoint& p2, 
			const double& ptx, const double& pty)
	{
		return PointOnSegment(p1.x, p1.y, p2.x, p2.y, ptx, pty);
	}

	double GetDistanseToBorder(const CDoublePoint& point1, const CDoublePoint& point2, 
							   const CDoublePoint& point3, const CDoublePoint& point4)
	{
		return GetDistanseToBorder(point1.x, point1.y, point2.x, point2.y,
									point3.x, point3.y, point4.x, point4.y);
	}
	double GetDistanseToBorder2(const CDoublePoint& point1, const CDoublePoint& point2, 
							   const CDoublePoint& point3, const CDoublePoint& point4)
	{
		return GetDistanseToBorder2(point1.x, point1.y, point2.x, point2.y,
									point3.x, point3.y, point4.x, point4.y);
	}

	int CrossLineAndSegment(const double& xLine1, const double& yLine1, const double& xLine2, const double& yLine2,
								const CDoublePoint& pointSegment1, const CDoublePoint& pointSegment2, CDoublePoint& oCrossPoint)
	{
		double x3 = pointSegment1.x;
		double y3 = pointSegment1.y;
		double x4 = pointSegment2.x;
		double y4 = pointSegment2.y;
		
		double dZnam = (y4 - y3) * (xLine2 - xLine1) - (x4 - x3) * (yLine2 - yLine1);
		
		double dCisl1 = (x4 - x3)		  * (yLine1 - y3) - (y4 - y3)		  * (xLine1 - x3);
		double dCisl2 = (xLine2 - xLine1) * (yLine1 - y3) - (yLine2 - yLine1) * (xLine1 - x3);

		if (0 == dZnam)
		{
			if ((0 == dCisl1) || (0 == dCisl2))
			{
				// совпадают
				return 0;
			}
			else 
			{
				// параллельны
				return -1;
			}
		}

		double dT1 = dCisl1 / dZnam;
		double dT2 = dCisl2 / dZnam;

		if ((0 > dT2) || (dT2 > 1))
		{
			// пересекаются, но не внутри отрезка
			return 2;
		}

		// теперь берем точку пересечения...
		oCrossPoint.x = x3 + dT2 * (x4 - x3);
		oCrossPoint.y = y3 + dT2 * (y4 - y3);

		return 1;
	}

	class CDoubleRect
	{
	public:
		double left;
		double top;
		double right;
		double bottom;

	public:
		CDoubleRect()
		{
			left	= 0;
			top		= 0;
			right	= 0;
			bottom	= 0;
		}
		CDoubleRect& operator=(const CDoubleRect& oSrc)
		{
			left	= oSrc.left;
			top		= oSrc.top;
			right	= oSrc.right;
			bottom	= oSrc.bottom;

			return *this;
		}
		CDoubleRect& operator=(const RectF& oSrc)
		{
			left	= (double)oSrc.X;
			top		= (double)oSrc.Y;
			right	= (double)oSrc.GetRight();
			bottom	= (double)oSrc.GetBottom();

			return *this;
		}
		CDoubleRect& operator=(const CRect& oSrc)
		{
			left	= (double)oSrc.left;
			top		= (double)oSrc.top;
			right	= (double)oSrc.right;
			bottom	= (double)oSrc.bottom;

			return *this;
		}
		CDoubleRect(const CDoubleRect& oSrc)
		{
			*this = oSrc;
		}

		inline double GetWidth() const
		{
			return right - left;
		}
		inline double GetHeight() const
		{
			return bottom - top;
		}

		inline RectF GetRectF()
		{
			RectF oRect((REAL)left, (REAL)top, (REAL)GetWidth(), (REAL)GetHeight());
			return oRect;
		}

		inline void Offset(double dX, double dY)
		{
			left	+= dX;
			top		+= dY;
			right	+= dX;
			bottom	+= dY;
		}

		inline double GetCentreX() const
		{
			return (left + right) / 2.0;
		}

		inline double GetCentreY() const
		{
			return (top + bottom) / 2.0;
		}

		inline BOOL IsPointInside(const CDoublePoint& oPoint)
		{
			return IsPointInside(oPoint.x, oPoint.y);
		}
		BOOL IsPointInside(const double& pointX, const double& pointY)
		{
			return ((left <= pointX) && (right >= pointX) &&
					(top <= pointY) && (bottom >= pointY));
		}

		void CorrectCentre(CDoublePoint& oPointCentre)
		{
			oPointCentre.x = max(oPointCentre.x, left);
			oPointCentre.x = min(oPointCentre.x, right);

			oPointCentre.y = max(oPointCentre.y, top);
			oPointCentre.y = min(oPointCentre.y, bottom);
		}
	};

	class COffsetInfo
	{
	public:
		double m_dLeftMin;
		double m_dRightMax;
		double m_dTopMin;
		double m_dBottomMax;

		double m_dLeftMinAspect;
		double m_dRightMaxAspect;
		double m_dTopMinAspect;
		double m_dBottomMaxAspect;

		CDoubleRect m_oCentresRect;

	public:
		COffsetInfo() : m_oCentresRect()
		{
			m_dLeftMin			= 0;
			m_dRightMax			= 0;
			m_dTopMin			= 0;
			m_dBottomMax		= 0;

			m_dLeftMinAspect	= 0;
			m_dRightMaxAspect	= 0;
			m_dTopMinAspect		= 0;
			m_dBottomMaxAspect	= 0;
		}

		COffsetInfo& operator=(const COffsetInfo& oSrc)
		{
			m_dLeftMin			= oSrc.m_dLeftMin;
			m_dRightMax			= oSrc.m_dRightMax;
			m_dTopMin			= oSrc.m_dTopMin;
			m_dBottomMax		= oSrc.m_dBottomMax;

			m_dLeftMinAspect	= oSrc.m_dLeftMinAspect;
			m_dRightMaxAspect	= oSrc.m_dRightMaxAspect;
			m_dTopMinAspect		= oSrc.m_dTopMinAspect;
			m_dBottomMaxAspect	= oSrc.m_dBottomMaxAspect;

			m_oCentresRect		= oSrc.m_oCentresRect;

			return *this;
		}

		COffsetInfo(const COffsetInfo& oSrc)
		{
			*this = oSrc;
		}
	};

	class CDoubleRectRotate
	{
	private:
		CDoublePoint m_arPoints[4];

	public:
		CDoubleRectRotate()
		{
		}
		CDoubleRectRotate(CDoubleRect& oRect, double dAngle = 0.0)
		{
			double dSin = sin(dAngle);
			double dCos = cos(dAngle);

			double dCx = (oRect.left + oRect.right) / 2.0;
			double dCy = (oRect.top + oRect.bottom) / 2.0;

			oRect.Offset(-dCx, -dCy);
			
			m_arPoints[0].x = (oRect.left  * dCos - oRect.top * dSin);
			m_arPoints[0].y = (oRect.left  * dSin + oRect.top * dCos);

			m_arPoints[1].x = (oRect.right * dCos - oRect.top * dSin);
			m_arPoints[1].y = (oRect.right * dSin + oRect.top * dCos);

			m_arPoints[2].x = (oRect.right * dCos - oRect.bottom * dSin);
			m_arPoints[2].y = (oRect.right * dSin + oRect.bottom * dCos);

			m_arPoints[3].x = (oRect.left  * dCos - oRect.bottom * dSin);
			m_arPoints[3].y = (oRect.left  * dSin + oRect.bottom * dCos);

			oRect.Offset(dCx, dCy);

			Offset(dCx, dCy);
		}
		CDoubleRectRotate(CDoubleRect& oRect, const CDoublePoint& oCentre, double dAngle = 0.0)
		{
			double dSin = sin(dAngle);
			double dCos = cos(dAngle);

			double dCx = oCentre.x;
			double dCy = oCentre.y;

			oRect.Offset(-dCx, -dCy);
			
			m_arPoints[0].x = (oRect.left  * dCos - oRect.top * dSin);
			m_arPoints[0].y = (oRect.left  * dSin + oRect.top * dCos);

			m_arPoints[1].x = (oRect.right * dCos - oRect.top * dSin);
			m_arPoints[1].y = (oRect.right * dSin + oRect.top * dCos);

			m_arPoints[2].x = (oRect.right * dCos - oRect.bottom * dSin);
			m_arPoints[2].y = (oRect.right * dSin + oRect.bottom * dCos);

			m_arPoints[3].x = (oRect.left  * dCos - oRect.bottom * dSin);
			m_arPoints[3].y = (oRect.left  * dSin + oRect.bottom * dCos);

			oRect.Offset(dCx, dCy);

			Offset(dCx, dCy);
		}
		double GetWidth()
		{
			return _hypot(m_arPoints[0].x - m_arPoints[1].x, m_arPoints[0].y - m_arPoints[1].y);
		}
		double GetHeight()
		{
			return _hypot(m_arPoints[1].x - m_arPoints[2].x, m_arPoints[1].y - m_arPoints[2].y);
		}
		CDoublePoint& operator[](int nIndex)
		{
			if (nIndex >= 0 && nIndex < 4)
				return m_arPoints[nIndex];
			return m_arPoints[0];
		}

		void Offset(double dX, double dY)
		{
			for (int i = 0; i < 4; ++i)
			{
				m_arPoints[i].x += dX;
				m_arPoints[i].y	+= dY;
			}
		}

		void Normalize(CDoubleRect& oRect)
		{
			// делаем так, чтобы видна была ВСЯ картинка		
			double dMax = 0;
			double dDist = 0;

			double cX = (oRect.left + oRect.right) / 2.0;
			double cY = (oRect.top + oRect.bottom) / 2.0;

			double dDiag = _hypot(cX - m_arPoints[0].x, cY - m_arPoints[0].y);
			
			dDist = GetDistanseToBorder(oRect.left,  oRect.top,	   oRect.right, oRect.top,    cX, cY, m_arPoints[0].x, m_arPoints[0].y);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder(oRect.left,  oRect.top,	   oRect.left,  oRect.bottom, cX, cY, m_arPoints[0].x, m_arPoints[0].y);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder(oRect.left,  oRect.bottom, oRect.right, oRect.bottom, cX, cY, m_arPoints[0].x, m_arPoints[0].y);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder(oRect.right, oRect.top,    oRect.right, oRect.bottom, cX, cY, m_arPoints[0].x, m_arPoints[0].y);
			dMax = max(dMax, dDist);

			dDist = GetDistanseToBorder(oRect.left,  oRect.top,    oRect.right, oRect.top,    cX, cY, m_arPoints[1].x, m_arPoints[1].y);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder(oRect.left,  oRect.top,    oRect.left,  oRect.bottom, cX, cY, m_arPoints[1].x, m_arPoints[1].y);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder(oRect.left,  oRect.bottom, oRect.right, oRect.bottom, cX, cY, m_arPoints[1].x, m_arPoints[1].y);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder(oRect.right, oRect.top,    oRect.right, oRect.bottom, cX, cY, m_arPoints[1].x, m_arPoints[1].y);
			dMax = max(dMax, dDist);

			double dKoef = dMax / dDiag;

			m_arPoints[0].x = max(oRect.left, min(oRect.right,  m_arPoints[0].x + dKoef * (cX - m_arPoints[0].x)));
			m_arPoints[0].y = max(oRect.top,  min(oRect.bottom, m_arPoints[0].y + dKoef * (cY - m_arPoints[0].y)));

			m_arPoints[1].x = max(oRect.left, min(oRect.right,  m_arPoints[1].x + dKoef * (cX - m_arPoints[1].x)));
			m_arPoints[1].y = max(oRect.top,  min(oRect.bottom, m_arPoints[1].y + dKoef * (cY - m_arPoints[1].y)));
			
			m_arPoints[3].x = 2 * cX - m_arPoints[1].x;
			m_arPoints[3].y = 2 * cY - m_arPoints[1].y;

			m_arPoints[2].x = 2 * cX - m_arPoints[0].x;
			m_arPoints[2].y = 2 * cY - m_arPoints[0].y;
		}

		void Normalize2(CDoubleRect& oRect)
		{
			double dMax = 0;
			double dDist = 0;

			double cX = (oRect.left + oRect.right) / 2.0;
			double cY = (oRect.top + oRect.bottom) / 2.0;

			double dDiag = _hypot(cX - oRect.left, cY - oRect.top);

			CDoublePoint pointCentre;
			pointCentre.x = cX;
			pointCentre.y = cY;

			CDoublePoint point1;
			point1.x = oRect.left;
			point1.y = oRect.top;

			CDoublePoint point2;
			point2.x = oRect.right;
			point2.y = oRect.top;

			CDoublePoint point3;
			point3.x = oRect.right;
			point3.y = oRect.bottom;

			CDoublePoint point4;
			point4.x = oRect.left;
			point4.y = oRect.bottom;

	
			dDist = GetDistanseToBorder(m_arPoints[0], m_arPoints[1], pointCentre, point1);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder(m_arPoints[1], m_arPoints[2], pointCentre, point1);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder(m_arPoints[2], m_arPoints[3], pointCentre, point1);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder(m_arPoints[3], m_arPoints[0], pointCentre, point1);
			dMax = max(dMax, dDist);

			dDist = GetDistanseToBorder(m_arPoints[0], m_arPoints[1], pointCentre, point2);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder(m_arPoints[1], m_arPoints[2], pointCentre, point2);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder(m_arPoints[2], m_arPoints[3], pointCentre, point2);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder(m_arPoints[3], m_arPoints[0], pointCentre, point2);
			dMax = max(dMax, dDist);

			dDist = GetDistanseToBorder(m_arPoints[0], m_arPoints[1], pointCentre, point3);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder(m_arPoints[1], m_arPoints[2], pointCentre, point3);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder(m_arPoints[2], m_arPoints[3], pointCentre, point3);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder(m_arPoints[3], m_arPoints[0], pointCentre, point3);
			dMax = max(dMax, dDist);

			dDist = GetDistanseToBorder(m_arPoints[0], m_arPoints[1], pointCentre, point4);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder(m_arPoints[1], m_arPoints[2], pointCentre, point4);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder(m_arPoints[2], m_arPoints[3], pointCentre, point4);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder(m_arPoints[3], m_arPoints[0], pointCentre, point4);
			dMax = max(dMax, dDist);

			if (0 == dMax)
				return;

			double dKoef = 1.0 - dMax / dDiag;

			double dNewWidth	= dKoef * oRect.GetWidth();
			double dNewHeight	= dKoef * oRect.GetHeight();

			oRect.left		= cX - dNewWidth  / 2.0;
			oRect.right		= cX + dNewWidth  / 2.0;
			oRect.top		= cY - dNewHeight / 2.0;
			oRect.bottom	= cY + dNewHeight / 2.0;
		}

		BOOL InRectInside(CDoubleRect& oTrackRect)
		{
			BOOL bRes = IsPointInside(oTrackRect.left,  oTrackRect.top)		&&
						IsPointInside(oTrackRect.left,  oTrackRect.bottom)	&&
						IsPointInside(oTrackRect.right, oTrackRect.top)		&&
						IsPointInside(oTrackRect.right, oTrackRect.bottom);

			return bRes;
		}
		inline BOOL IsPointInside(const CDoublePoint& oPoint)
		{
			return IsPointInside(oPoint.x, oPoint.y);
		}
		BOOL IsPointInside(const double& pointX, const double& pointY)
		{
			int      yflag0, yflag1, inside_flag;
			double   vtx0, vty0, vtx1, vty1;

			int num_points = 4;

			vtx0 = m_arPoints[num_points - 1].x;
			vty0 = m_arPoints[num_points - 1].y;

			yflag0 = (vty0 >= pointY);

			vtx1 = m_arPoints[0].x;
			vty1 = m_arPoints[0].y;

			inside_flag = 0;
			for (int j = 1; j <= num_points; ++j) 
			{
				yflag1 = (vty1 >= pointY);
				if (yflag0 != yflag1) 
				{
					if ( ((vty1-pointY) * (vtx0-vtx1) >= (vtx1-pointX) * (vty0-vty1)) == yflag1 ) 
					{
						inside_flag ^= 1;
					}
				}

				yflag0 = yflag1;
				vtx0 = vtx1;
				vty0 = vty1;

				int k = (j >= num_points) ? j - num_points : j;
				vtx1 = m_arPoints[k].x;
				vty1 = m_arPoints[k].y;
			}
			
			bool bOnLine = (PointOnSegment(m_arPoints[0], m_arPoints[1], pointX, pointY) ||
						    PointOnSegment(m_arPoints[1], m_arPoints[2], pointX, pointY) ||
						    PointOnSegment(m_arPoints[2], m_arPoints[3], pointX, pointY) ||
						    PointOnSegment(m_arPoints[3], m_arPoints[0], pointX, pointY));

			return (inside_flag != 0 || bOnLine);
		}

		PointF* GetPoints()
		{
			PointF* points = new PointF[4];

			points[0].X = (REAL)m_arPoints[0].x;
			points[0].Y = (REAL)m_arPoints[0].y;

			points[1].X = (REAL)m_arPoints[1].x;
			points[1].Y = (REAL)m_arPoints[1].y;

			points[2].X = (REAL)m_arPoints[3].x;
			points[2].Y = (REAL)m_arPoints[3].y;

			points[3].X = (REAL)m_arPoints[2].x;
			points[3].Y = (REAL)m_arPoints[2].y;

			return points;
		}

		double GetMaxDistanceHor()
		{
			double dLeft	= m_arPoints[0].x;
			double dRight	= m_arPoints[0].x;

			for (int i = 1; i < 4; ++i)
			{
				dLeft	= min(dLeft,  m_arPoints[i].x);
				dRight	= max(dRight, m_arPoints[i].x);
			}

			return dRight - dLeft;
		}
		double GetMaxDistanceVer()
		{
			double dTop		= m_arPoints[0].y;
			double dBottom	= m_arPoints[0].y;

			for (int i = 1; i < 4; ++i)
			{
				dTop	= min(dTop,		m_arPoints[i].y);
				dBottom	= max(dBottom,	m_arPoints[i].y);
			}

			return dBottom - dTop;
		}

		inline double GetDistanceToBorders(const CDoublePoint& oPointCentre, CDoublePoint& oPoint)
		{
			double dDist = 0.0;
			double dMax  = 0.0;

			dDist = GetDistanseToBorder2(m_arPoints[0], m_arPoints[1], oPointCentre, oPoint);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder2(m_arPoints[1], m_arPoints[2], oPointCentre, oPoint);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder2(m_arPoints[2], m_arPoints[3], oPointCentre, oPoint);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder2(m_arPoints[3], m_arPoints[0], oPointCentre, oPoint);
			dMax = max(dMax, dDist);

			return dMax;
		}
		inline void CorrectDistanceToBorders(const CDoublePoint& oPointCentre, CDoublePoint& oPoint)
		{
			double dDist = 0.0;
			double dMax  = 0.0;

			dDist = GetDistanseToBorder2(m_arPoints[0], m_arPoints[1], oPointCentre, oPoint);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder2(m_arPoints[1], m_arPoints[2], oPointCentre, oPoint);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder2(m_arPoints[2], m_arPoints[3], oPointCentre, oPoint);
			dMax = max(dMax, dDist);
			dDist = GetDistanseToBorder2(m_arPoints[3], m_arPoints[0], oPointCentre, oPoint);
			dMax = max(dMax, dDist);

			if (0 != dMax)
			{
				double dX = oPoint.x - oPointCentre.x;
				double dY = oPoint.y - oPointCentre.y;

				double dKoef = 1.0 - dMax / _hypot(dX, dY);
				oPoint.x = oPointCentre.x + dKoef * dX;
				oPoint.y = oPointCentre.y + dKoef * dY;
			}
		}
		
		COffsetInfo GetCritical(const CDoubleRect& oCropTrack)
		{
			const double dCropMinSize = 10.0;
			
			double dLeft	= oCropTrack.left;
			double dTop		= oCropTrack.top;
			double dRight	= oCropTrack.right;
			double dBottom	= oCropTrack.bottom;
			
			// все смещения в системе координат, связанной с кроп-ректом

			COffsetInfo oInfo;
			oInfo.m_dLeftMin	= -1000000;
			oInfo.m_dTopMin		= -1000000;
			oInfo.m_dRightMax	=  1000000;
			oInfo.m_dBottomMax	=  1000000;
				
			CDoublePoint oCross;
			int flag = 0;
			for (int i = 0; i < 4; ++i)
			{
				int j = (i == 3) ? 0 : i + 1;

				// top line
				flag = CrossLineAndSegment(dLeft, dTop, dRight, dTop, m_arPoints[i], m_arPoints[j], oCross);
				if (flag == 0)
				{
					// совпадение. Нужно смотреть на вершины
					double d1 = m_arPoints[i].x - dLeft;
					double d2 = m_arPoints[j].x - dLeft;
					double d3 = m_arPoints[i].x - dRight;
					double d4 = m_arPoints[j].x - dRight;

					oInfo.m_dLeftMin  = max(oInfo.m_dLeftMin, min(d1, d2));
					oInfo.m_dRightMax = min(oInfo.m_dRightMax, max(d3, d4));
				}
				else if (flag == 1)
				{
					// пересечение
					if (oCross.x <= dLeft)
					{
						oInfo.m_dLeftMin  = max(oInfo.m_dLeftMin, oCross.x - dLeft);
					}
					else if (oCross.x >= dRight)
					{
						oInfo.m_dRightMax = min(oInfo.m_dRightMax, oCross.x - dRight);
					}
				}
				// bottom line
				flag = CrossLineAndSegment(dLeft, dBottom, dRight, dBottom, m_arPoints[i], m_arPoints[j], oCross);
				if (flag == 0)
				{
					// совпадение. Нужно смотреть на вершины
					double d1 = m_arPoints[i].x - dLeft;
					double d2 = m_arPoints[j].x - dLeft;
					double d3 = m_arPoints[i].x - dRight;
					double d4 = m_arPoints[j].x - dRight;

					oInfo.m_dLeftMin  = max(oInfo.m_dLeftMin, min(d1, d2));
					oInfo.m_dRightMax = min(oInfo.m_dRightMax, max(d3, d4));
				}
				else if (flag == 1)
				{
					// пересечение
					if (oCross.x <= dLeft)
					{
						oInfo.m_dLeftMin  = max(oInfo.m_dLeftMin, oCross.x - dLeft);
					}
					else if (oCross.x >= dRight)
					{
						oInfo.m_dRightMax = min(oInfo.m_dRightMax, oCross.x - dRight);
					}
				}
				// left line
				flag = CrossLineAndSegment(dLeft, dTop, dLeft, dBottom, m_arPoints[i], m_arPoints[j], oCross);
				if (flag == 0)
				{
					// совпадение. Нужно смотреть на вершины
					double d1 = m_arPoints[i].y - dTop;
					double d2 = m_arPoints[j].y - dTop;
					double d3 = m_arPoints[i].y - dBottom;
					double d4 = m_arPoints[j].y - dBottom;

					oInfo.m_dTopMin    = max(oInfo.m_dTopMin, min(d1, d2));
					oInfo.m_dBottomMax = min(oInfo.m_dBottomMax, max(d3, d4));
				}
				else if (flag == 1)
				{
					// пересечение
					if (oCross.y <= dTop)
					{
						oInfo.m_dTopMin  = max(oInfo.m_dTopMin, oCross.y - dTop);
					}
					else if (oCross.y >= dBottom)
					{
						oInfo.m_dBottomMax = min(oInfo.m_dBottomMax, oCross.y - dBottom);
					}
				}
				// right line
				flag = CrossLineAndSegment(dRight, dTop, dRight, dBottom, m_arPoints[i], m_arPoints[j], oCross);
				if (flag == 0)
				{
					// совпадение. Нужно смотреть на вершины
					double d1 = m_arPoints[i].y - dTop;
					double d2 = m_arPoints[j].y - dTop;
					double d3 = m_arPoints[i].y - dBottom;
					double d4 = m_arPoints[j].y - dBottom;

					oInfo.m_dTopMin    = max(oInfo.m_dTopMin, min(d1, d2));
					oInfo.m_dBottomMax = min(oInfo.m_dBottomMax, max(d3, d4));
				}
				else if (flag == 1)
				{
					// пересечение
					if (oCross.y <= dTop)
					{
						oInfo.m_dTopMin  = max(oInfo.m_dTopMin, oCross.y - dTop);
					}
					else if (oCross.y >= dBottom)
					{
						oInfo.m_dBottomMax = min(oInfo.m_dBottomMax, oCross.y - dBottom);
					}
				}
			}

			// теперь расчитываем по аспекту.
			double dAspect = oCropTrack.GetWidth() / oCropTrack.GetHeight();
			double dWidthMax  = GetWidth() + GetHeight();
			double dHeightMax = dWidthMax / dAspect;

			double dDiagHor1 = _hypot(dWidthMax, dHeightMax / 2.0); 
			double dDiagHor2 = dHeightMax / 2.0; 
			double dDiagVer1 = _hypot(dWidthMax / 2.0, dHeightMax);
			double dDiagVer2 = dWidthMax / 2.0; 

			CDoublePoint oCentre;

			CDoublePoint point1;
			CDoublePoint point2;
			CDoublePoint point3;
			CDoublePoint point4;

			double dMemX = 0;
			double dMemY = 0;

			double d1 = 0;
			double d2 = 0;
			double d3 = 0;
			double d4 = 0;

			double d = 0;
			double dKoef = 1.0;

			double dWidth	= 0.0;
			double dHeight	= 0.0;

			// left --------------------------------------------------------
			oCentre.x = oCropTrack.right;
			oCentre.y = oCropTrack.GetCentreY();

			point1.x = oCentre.x;
			point1.y = oCentre.y - dHeightMax / 2.0;

			point2.x = oCentre.x;
			point2.y = oCentre.y + dHeightMax / 2.0;

			point3.x = oCentre.x - dWidthMax;
			point3.y = point1.y;

			point4.x = point3.x;
			point4.y = point2.y;

			d1 = GetDistanceToBorders(oCentre, point1) / dDiagHor2;
			d2 = GetDistanceToBorders(oCentre, point2) / dDiagHor2;
			d3 = GetDistanceToBorders(oCentre, point3) / dDiagHor1;
			d4 = GetDistanceToBorders(oCentre, point4) / dDiagHor1;

			d1 = max(d1, d2);
			d3 = max(d3, d4);

			d = max(d1, d3);
			
			dKoef = 1.0 - d;

			dWidth  = dKoef * dWidthMax;
			dHeight = dKoef * dHeightMax;

			oInfo.m_dLeftMinAspect = min(oCropTrack.GetWidth() - dWidth, 0);
			// -------------------------------------------------------------

			// right -------------------------------------------------------
			oCentre.x = oCropTrack.left;
			oCentre.y = oCropTrack.GetCentreY();

			point1.x = oCentre.x;
			point1.y = oCentre.y - dHeightMax / 2.0;

			point2.x = oCentre.x;
			point2.y = oCentre.y + dHeightMax / 2.0;

			point3.x = oCentre.x + dWidthMax;
			point3.y = point1.y;

			point4.x = point3.x;
			point4.y = point2.y;

			d1 = GetDistanceToBorders(oCentre, point1) / dDiagHor2;
			d2 = GetDistanceToBorders(oCentre, point2) / dDiagHor2;
			d3 = GetDistanceToBorders(oCentre, point3) / dDiagHor1;
			d4 = GetDistanceToBorders(oCentre, point4) / dDiagHor1;

			d1 = max(d1, d2);
			d3 = max(d3, d4);

			d = max(d1, d3);
			
			dKoef = 1.0 - d;

			dWidth  = dKoef * dWidthMax;
			dHeight = dKoef * dHeightMax;

			oInfo.m_dRightMaxAspect = max(0, dWidth - oCropTrack.GetWidth());
			// -------------------------------------------------------------

			// top -------------------------------------------------------
			oCentre.x = oCropTrack.GetCentreX();
			oCentre.y = oCropTrack.bottom;

			point1.x = oCentre.x - dWidthMax / 2.0;
			point1.y = oCentre.y;

			point2.x = oCentre.x + dWidthMax / 2.0;
			point2.y = oCentre.y;

			point3.x = point1.x;
			point3.y = oCentre.y - dHeightMax;

			point4.x = point2.x;
			point4.y = point3.y;

			d1 = GetDistanceToBorders(oCentre, point1) / dDiagVer2;
			d2 = GetDistanceToBorders(oCentre, point2) / dDiagVer2;
			d3 = GetDistanceToBorders(oCentre, point3) / dDiagVer1;
			d4 = GetDistanceToBorders(oCentre, point4) / dDiagVer1;

			d1 = max(d1, d2);
			d3 = max(d3, d4);

			d = max(d1, d3);
			
			dKoef = 1.0 - d;

			dWidth  = dKoef * dWidthMax;
			dHeight = dKoef * dHeightMax;

			oInfo.m_dTopMinAspect = min(0, oCropTrack.GetHeight() - dHeight);
			// -------------------------------------------------------------

			// bottom -------------------------------------------------------
			oCentre.x = oCropTrack.GetCentreX();
			oCentre.y = oCropTrack.top;

			point1.x = oCentre.x - dWidthMax / 2.0;
			point1.y = oCentre.y;

			point2.x = oCentre.x + dWidthMax / 2.0;
			point2.y = oCentre.y;

			point3.x = point1.x;
			point3.y = oCentre.y + dHeightMax;

			point4.x = point2.x;
			point4.y = point3.y;

			d1 = GetDistanceToBorders(oCentre, point1) / dDiagVer2;
			d2 = GetDistanceToBorders(oCentre, point2) / dDiagVer2;
			d3 = GetDistanceToBorders(oCentre, point3) / dDiagVer1;
			d4 = GetDistanceToBorders(oCentre, point4) / dDiagVer1;

			d1 = max(d1, d2);
			d3 = max(d3, d4);

			d = max(d1, d3);
			
			dKoef = 1.0 - d;

			dWidth  = dKoef * dWidthMax;
			dHeight = dKoef * dHeightMax;

			oInfo.m_dBottomMaxAspect = max(0, dHeight - oCropTrack.GetHeight());
			// -------------------------------------------------------------

			// теперь расчитываем рект возвожных центров
			oInfo.m_oCentresRect = GetCentresRect(oCropTrack);
			
			return oInfo;
		}

		void NormalizeNoAspect(TrackTypeEx lType, const CDoubleRect& oRect, double& dWidth, double& dHeight, COffsetInfo& oInfo)
		{
			if (tteRotate == lType)
			{	
				double dMax = 0;
				double dDist = 0;

				double cX = (oRect.left + oRect.right) / 2.0;
				double cY = (oRect.top + oRect.bottom) / 2.0;

				double dDiag = _hypot(cX - oRect.left, cY - oRect.top);

				CDoublePoint pointCentre;
				pointCentre.x = cX;
				pointCentre.y = cY;

				CDoublePoint point1;
				point1.x = oRect.left;
				point1.y = oRect.top;

				CDoublePoint point2;
				point2.x = oRect.right;
				point2.y = oRect.top;

				CDoublePoint point3;
				point3.x = oRect.right;
				point3.y = oRect.bottom;

				CDoublePoint point4;
				point4.x = oRect.left;
				point4.y = oRect.bottom;
			
				dDist = GetDistanseToBorder(m_arPoints[0], m_arPoints[1], pointCentre, point1);
				dMax = max(dMax, dDist);
				dDist = GetDistanseToBorder(m_arPoints[1], m_arPoints[2], pointCentre, point1);
				dMax = max(dMax, dDist);
				dDist = GetDistanseToBorder(m_arPoints[2], m_arPoints[3], pointCentre, point1);
				dMax = max(dMax, dDist);
				dDist = GetDistanseToBorder(m_arPoints[3], m_arPoints[0], pointCentre, point1);
				dMax = max(dMax, dDist);

				dDist = GetDistanseToBorder(m_arPoints[0], m_arPoints[1], pointCentre, point2);
				dMax = max(dMax, dDist);
				dDist = GetDistanseToBorder(m_arPoints[1], m_arPoints[2], pointCentre, point2);
				dMax = max(dMax, dDist);
				dDist = GetDistanseToBorder(m_arPoints[2], m_arPoints[3], pointCentre, point2);
				dMax = max(dMax, dDist);
				dDist = GetDistanseToBorder(m_arPoints[3], m_arPoints[0], pointCentre, point2);
				dMax = max(dMax, dDist);

				dDist = GetDistanseToBorder(m_arPoints[0], m_arPoints[1], pointCentre, point3);
				dMax = max(dMax, dDist);
				dDist = GetDistanseToBorder(m_arPoints[1], m_arPoints[2], pointCentre, point3);
				dMax = max(dMax, dDist);
				dDist = GetDistanseToBorder(m_arPoints[2], m_arPoints[3], pointCentre, point3);
				dMax = max(dMax, dDist);
				dDist = GetDistanseToBorder(m_arPoints[3], m_arPoints[0], pointCentre, point3);
				dMax = max(dMax, dDist);

				dDist = GetDistanseToBorder(m_arPoints[0], m_arPoints[1], pointCentre, point4);
				dMax = max(dMax, dDist);
				dDist = GetDistanseToBorder(m_arPoints[1], m_arPoints[2], pointCentre, point4);
				dMax = max(dMax, dDist);
				dDist = GetDistanseToBorder(m_arPoints[2], m_arPoints[3], pointCentre, point4);
				dMax = max(dMax, dDist);
				dDist = GetDistanseToBorder(m_arPoints[3], m_arPoints[0], pointCentre, point4);
				dMax = max(dMax, dDist);

				if (0 == dMax)
					return;

				double dKoef = 1.0 - dMax / dDiag;

				dWidth	= dKoef * oRect.GetWidth();
				dHeight	= dKoef * oRect.GetHeight();

				return;
			}
			else if (tteLeft	== lType)
			{
				dWidth	= min(dWidth, oRect.GetWidth() - oInfo.m_dLeftMin);
				return;
			}
			else if (tteRight	== lType)
			{
				dWidth	= min(dWidth, oRect.GetWidth() + oInfo.m_dRightMax);
				return;
			}
			else if (tteTop		== lType)
			{
				dHeight = min(dHeight, oRect.GetHeight() - oInfo.m_dTopMin);
				return;
			}
			else if (tteBottom	== lType)
			{
				dHeight	= min(dHeight, oRect.GetHeight() + oInfo.m_dBottomMax);
				return;
			}

			CDoubleRect oRectNew = oRect;
			if (tteLeftTop == lType)
			{
				oRectNew.left	+= (oRect.GetWidth()  - dWidth);
				oRectNew.top	+= (oRect.GetHeight() - dHeight);
			}
			else if (tteLeftBottom == lType)
			{
				oRectNew.left	+= (oRect.GetWidth()  - dWidth);
				oRectNew.bottom	-= (oRect.GetHeight() - dHeight);
			}
			else if (tteRightTop == lType)
			{
				oRectNew.right	-= (oRect.GetWidth()  - dWidth);
				oRectNew.top	+= (oRect.GetHeight() - dHeight);
			}
			else if (tteRightBottom == lType)
			{
				oRectNew.right	-= (oRect.GetWidth()  - dWidth);
				oRectNew.bottom	-= (oRect.GetHeight() - dHeight);
			}
			
			CDoublePoint pointLeftTop(oRectNew.left, oRectNew.top);
			CDoublePoint pointLeftBottom(oRectNew.left, oRectNew.bottom);
			CDoublePoint pointRightTop(oRectNew.right, oRectNew.top);
			CDoublePoint pointRightBottom(oRectNew.right, oRectNew.bottom);
			
			if (tteLeftTop == lType)
			{
				if (!IsPointInside(pointLeftBottom))
				{
					CorrectDistanceToBorders(pointRightBottom, pointLeftBottom);
				}
				if (!IsPointInside(pointRightTop))
				{
					CorrectDistanceToBorders(pointRightBottom, pointRightTop);
				}
				pointLeftTop.x = pointLeftBottom.x;
				pointLeftTop.y = pointRightTop.y;

				if (!IsPointInside(pointLeftTop))
				{
					CorrectDistanceToBorders(pointRightBottom, pointLeftTop);
				}

				dWidth  = abs(pointRightBottom.x - pointLeftTop.x);
				dHeight = abs(pointRightBottom.y - pointLeftTop.y);				
			}
			else if (tteLeftBottom == lType)
			{
				if (!IsPointInside(pointLeftTop))
				{
					CorrectDistanceToBorders(pointRightTop, pointLeftTop);
				}
				if (!IsPointInside(pointRightBottom))
				{
					CorrectDistanceToBorders(pointRightTop, pointRightBottom);
				}
				pointLeftBottom.x = pointLeftTop.x;
				pointLeftBottom.y = pointRightBottom.y;

				if (!IsPointInside(pointLeftBottom))
				{
					CorrectDistanceToBorders(pointRightTop, pointLeftBottom);
				}

				dWidth  = abs(pointLeftBottom.x - pointRightTop.x);
				dHeight = abs(pointLeftBottom.y - pointRightTop.y);
			}
			else if (tteRightTop == lType)
			{
				if (!IsPointInside(pointRightBottom))
				{
					CorrectDistanceToBorders(pointLeftBottom, pointRightBottom);
				}
				if (!IsPointInside(pointLeftTop))
				{
					CorrectDistanceToBorders(pointLeftBottom, pointLeftTop);
				}
				pointRightTop.x = pointRightBottom.x;
				pointRightTop.y = pointLeftTop.y;

				if (!IsPointInside(pointRightTop))
				{
					CorrectDistanceToBorders(pointLeftBottom, pointRightTop);
				}

				dWidth  = abs(pointLeftBottom.x - pointRightTop.x);
				dHeight = abs(pointLeftBottom.y - pointRightTop.y);
			}
			else if (tteRightBottom == lType)
			{
				if (!IsPointInside(pointRightTop))
				{
					CorrectDistanceToBorders(pointLeftTop, pointRightTop);
				}
				if (!IsPointInside(pointLeftBottom))
				{
					CorrectDistanceToBorders(pointLeftTop, pointLeftBottom);
				}
				pointRightBottom.x = pointRightTop.x;
				pointRightBottom.y = pointLeftBottom.y;

				if (!IsPointInside(pointRightBottom))
				{
					CorrectDistanceToBorders(pointLeftTop, pointRightBottom);
				}

				dWidth  = abs(pointRightBottom.x - pointLeftTop.x);
				dHeight = abs(pointRightBottom.y - pointLeftTop.y);		
			}
		}

		void NormalizeWithAspect(TrackTypeEx lType, const CDoubleRect& oRect, double& dWidth, double& dHeight, double dAspect, COffsetInfo& oInfo)
		{
			/*if (tteLeft		== lType)
			{
				dHeight = oRect.GetHeight();
				dWidth	= min(dWidth, oRect.GetWidth() - oInfo.m_dLeftMin);
				
				double dHeightNew = dWidth / dAspect;
				double dMem = (dHeightNew - dHeight) / 2.0;

				if (0 < dMem)
				{
					dMem = min(dMem, min(-oInfo.m_dTopMin, oInfo.m_dBottomMax));
					dHeight += (2 * dMem);
					dWidth = dAspect * dHeight;
				}
				else
				{
					dHeight = dHeightNew;
				}
			}
			if (tteRight	== lType)
			{
				dHeight = oRect.GetHeight();				
				dWidth	= min(dWidth, oRect.GetWidth() + oInfo.m_dRightMax);
				
				double dHeightNew = dWidth / dAspect;
				double dMem = (dHeightNew - dHeight) / 2.0;

				if (0 < dMem)
				{
					dMem = min(dMem, min(-oInfo.m_dTopMin, oInfo.m_dBottomMax));
					dHeight += (2 * dMem);
					dWidth = dAspect * dHeight;
				}
				else
				{
					dHeight = dHeightNew;
				}
			}
			if (tteTop		== lType)
			{
				dWidth = oRect.GetWidth();
				dHeight = min(dHeight, oRect.GetHeight() - oInfo.m_dTopMin);

				double dWidthNew = dAspect * dHeight;
				double dMem = (dWidthNew - dWidth) / 2.0;

				if (0 < dMem)
				{
					dMem = min(dMem, min(-oInfo.m_dLeftMin, oInfo.m_dRightMax));
					dWidth += (2 * dMem);
					dHeight = dWidth / dAspect;
				}
				else
				{
					dWidth = dWidthNew;
				}
			}
			if (tteBottom	== lType)
			{
				dWidth = oRect.GetWidth();
				dHeight	= min(dHeight, oRect.GetHeight() + oInfo.m_dBottomMax);

				double dWidthNew = dAspect * dHeight;
				double dMem = (dWidthNew - dWidth) / 2.0;

				if (0 < dMem)
				{
					dMem = min(dMem, min(-oInfo.m_dLeftMin, oInfo.m_dRightMax));
					dWidth += (2 * dMem);
					dHeight = dWidth / dAspect;
				}
				else
				{
					dWidth = dWidthNew;
				}
			}*/
			if (tteLeft			== lType)
			{
				dWidth	= min(dWidth, oRect.GetWidth() - oInfo.m_dLeftMinAspect);
				dHeight = dWidth / dAspect;
			}
			else if (tteRight	== lType)
			{
				dWidth	= min(dWidth, oRect.GetWidth() + oInfo.m_dRightMaxAspect);
				dHeight = dWidth / dAspect;
			}
			else if (tteTop		== lType)
			{
				dHeight = min(dHeight, oRect.GetHeight() - oInfo.m_dTopMinAspect);
				dWidth = dHeight * dAspect;
			}
			else if (tteBottom	== lType)
			{
				dHeight	= min(dHeight, oRect.GetHeight() + oInfo.m_dBottomMaxAspect);
				dWidth = dHeight * dAspect;
			}
		}
		
		void Normalize(TrackTypeEx lType, const CDoubleRect& oOldCrop, double& dWidth, double& dHeight, double dAspect, COffsetInfo& oInfo)
		{
			if (-1 == dAspect)
			{
				NormalizeNoAspect(lType, oOldCrop, dWidth, dHeight, oInfo);
			}
			else
			{
				NormalizeWithAspect(lType, oOldCrop, dWidth, dHeight, dAspect, oInfo);
			}
		}

		CDoubleRect GetCentresRect(const CDoubleRect& oRect)
		{
			double dX = abs(m_arPoints[1].x - m_arPoints[0].x);
			double dY = abs(m_arPoints[1].y - m_arPoints[0].y);

			double dAlpha = 0.0;
			if (0 != dX)
				dAlpha = atan(dY / dX);

			double dW = oRect.GetWidth();
			double dH = oRect.GetHeight();

			double dBetta = 0.0;
			if (0 != dW)
				dBetta = atan(dH / dW);

			double dDiag = _hypot(dW, dH) / 2.0;

			double _x = dDiag * abs(cos(dBetta - dAlpha));
			double _y = dDiag * abs(sin(dBetta + dAlpha));

			double dWidthImage  = GetWidth();
			double dHeightImage = GetHeight();

			CDoubleRect result;
			result.left		= _x;
			result.right	= dWidthImage - _x;
			result.top		= _y;
			result.bottom	= dHeightImage - _y;

			return result;
		}
	};
}
