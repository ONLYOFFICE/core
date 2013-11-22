#pragma once
#include "..\stdafx.h"
#include "math.h"

#ifndef M_PI 
#define M_PI       3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2     1.57079632679489661923
#endif
#ifndef M_PI_4
#define M_PI_4     0.785398163397448309616
#endif

#include "..\VectorGraphics\SPath.h"
#include "..\Interfaces\IAVSPen.h"
#include "..\Interfaces\IAVSBrush.h"
#include "..\Objects\AVSSolidPattern.h"

class CGraphicsPath : public SPath
{
public:

	CGraphicsPath();
	CGraphicsPath(const CGraphicsPath& oSrc);

	CGraphicsPath& operator=(const CGraphicsPath& oSrc);

	~CGraphicsPath();

public:

	// методы, которые просто будем сводить к трем основным
	inline BOOL AddBezier(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
	{
		int nRet = 0;
		if (m_bStartSubPath)
			nRet += MoveTo(x1, y1);
		else 
			nRet += LineTo(x1, y1);

		nRet += CurveTo(x2, y2, x3, y3, x4, y4);
		return (0 == nRet);
	}
	inline BOOL AddBeziers(double* pPoints, int nCount)
	{
		if (6 > nCount)
		{
			return FALSE;
		}
		int nRet = 0;

		int nIndex = 0;
		if (m_bStartSubPath)
		{
			nRet += PathCommandMoveTo(pPoints[0], pPoints[1]);
			nIndex = 2;
		}

		int n = (nCount - nIndex) / 6;

		for (int i = 0; i < n; ++i)
		{
			double* points = &pPoints[nIndex + i * 6];
			nRet += CurveTo(points[0], points[1], points[2], points[3], points[4], points[5]);
		}
		return (0 == nRet);
	}
	inline BOOL AddCurve(double* pPoints, int nCount)
	{
		// этим мы не пользуемся. Понадобится - реализую.
		return AddBeziers(pPoints, nCount);
	}

	inline BOOL AddLine(double x1, double y1, double x2, double y2)
	{
		if (m_bStartSubPath)
			PathCommandMoveTo(x1, y1);
		else
			PathCommandLineTo(x1, y1);

		return PathCommandLineTo(x2, y2);
	}
	inline BOOL AddLines(double* pPoints, int nCount)
	{
		if (2 > nCount)
		{
			return FALSE;
		}
		int nRet = 0;

		int nIndex = 0;
		if (m_bStartSubPath)
		{
			nRet += PathCommandMoveTo(pPoints[0], pPoints[1]);
			nIndex = 2;
		}

		int n = (nCount - nIndex) / 2;

		for (int i = 0; i < n; ++i)
		{
			double* points = &pPoints[nIndex + i * 2];
			nRet += LineTo(points[0], points[1]);
		}
		return (0 == nRet);
	}
	inline BOOL AddEllipse(double x, double y, double width, double height)
	{
		double dRadX = width  / 2.0;
		double dRadY = height / 2.0;
		return (0 == Ellipse(x - dRadX, y - dRadY, dRadX, dRadY));
	}
	inline BOOL AddRectangle(double x, double y, double width, double height)
	{
		PathCommandMoveTo(x, y);
		PathCommandLineTo(x + width, y);
		PathCommandLineTo(x + width, y + height);
		PathCommandLineTo(x, y + height);
		PathCommandLineTo(x, y);

		return TRUE;
	}
	inline BOOL AddPolygon(double* pPoints, int nCount)
	{
		if (2 > nCount)
		{
			return FALSE;
		}
		int nRet = 0;

		int nIndex = 0;
		if (m_bStartSubPath)
		{
			nRet += PathCommandMoveTo(pPoints[0], pPoints[1]);
			nIndex = 2;
		}

		int n = (nCount - nIndex) / 2;

		if (0 < n)
		{
			for (int i = 0; i < n; ++i)
			{
				double* points = &pPoints[nIndex + i * 2];
				nRet += LineTo(points[0], points[1]);
			}
			nRet += LineTo(pPoints[0], pPoints[1]);
		}
		return (0 == nRet);
	}
	inline BOOL AddPath(const CGraphicsPath& oPath)
	{
		SPath::Append((SPath*)&oPath);
		return TRUE;
	}
	inline BOOL AddArc(double x, double y, double width, double height, double startAngle, double sweepAngle)
	{
		return PathCommandArcTo(x, y, width, height, startAngle, sweepAngle);
	}

	inline BOOL PathCommandStart()
	{
		m_bStartSubPath = true;
		return TRUE;
	}

	inline BOOL PathCommandEnd()
	{
		return TRUE;
	}

	inline BOOL PathCommandClose()
	{
		return (0 == SPath::Close());
	}

	inline BOOL PathCommandMoveTo(double dX, double dY)
	{
		m_bStartSubPath = FALSE;
		return (0 == MoveTo(dX, dY));
	}

	inline BOOL PathCommandLineTo(double fX, double fY)
	{
		return (0 == LineTo(fX, fY));
	}

	inline BOOL PathCommandLinesTo(double* pPoints, int nCount)
	{
		return AddLines(pPoints, nCount);
	}

	inline BOOL PathCommandCurvesTo(double* pPoints, int nCount)
	{
		return AddBeziers(pPoints, nCount);
	}

	inline BOOL PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
	{
		if ( m_bStartSubPath )
		{
			double dStartAngle = fStartAngle * M_PI / 180;
			double fStartX = fX + fWidth / 2.0 + fWidth / 2 * cos( AngToEllPrm( dStartAngle, fWidth / 2, fHeight / 2 ) );
			double fStartY = fY + fHeight / 2.0 + fHeight / 2 * sin( AngToEllPrm ( dStartAngle, fWidth / 2, fHeight / 2 ) );

			// В случае, когда эллипс рисуется целиком используется команда AppendEllipse, в которой команда MoveTo уже есть
			if ( fSweepAngle < 360 )
				if ( !PathCommandMoveTo( fStartX, fStartY ) )
					return FALSE;			
		}

		BOOL bClockDirection = FALSE;
		double fEndAngle = 360 - ( fSweepAngle + fStartAngle );
		double fSrtAngle = 360 - fStartAngle;
		if( fSweepAngle > 0 )
			bClockDirection = TRUE;

		if( fSweepAngle >= 360 ) // Целый эллипс
		{
			return (0 == Ellipse(fX + fWidth / 2, fY + fHeight / 2, fWidth / 2, fHeight / 2));
		}
		else // Дуга эллипса
		{
			return (0 == EllipseArc(fX + fWidth / 2, fY + fHeight / 2, fWidth / 2, fHeight / 2, fSrtAngle, fEndAngle, bClockDirection));
		}
	}


	inline BOOL GetPoints(double** pPoints)
	{
		if (NULL == *pPoints || 0 == m_nPointsCount)
			return FALSE;

		*pPoints = new double[2 * m_nPointsCount];

		for (int i = 0; i < m_nPointsCount; ++i)
		{
			(*pPoints)[2 * i]		= m_pPoints[i].dX;
			(*pPoints)[2 * i + 1]	= m_pPoints[i].dY;
		}

		return TRUE;
	}
	inline BOOL GetPoints(double* pPoints)
	{
		if (NULL == pPoints || 0 == m_nPointsCount)
			return FALSE;

		for (int i = 0; i < m_nPointsCount; ++i)
		{
			pPoints[2 * i]		= m_pPoints[i].dX;
			pPoints[2 * i + 1]	= m_pPoints[i].dY;
		}

		return TRUE;
	}


protected:
	int EllipseArc(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, BOOL bClockDirection);
	static double AngToEllPrm(double fAngle, double fXRad, double fYRad);
	int EllipseArc2(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, BOOL bClockDirection);
	int EllipseArc3(double fX, double fY, double fXRad, double fYRad, double dAngle1, double dAngle2, double *pfXCur, double *pfYCur, BOOL bClockDirection);
	int Ellipse(double fX, double fY, double fXRad, double fYRad);

public:
	STDMETHOD(Pattern)(IAVSGraphicsBase* pObject, IAVSPattern** ppPattern)
	{
		// пока сделаем солид.
		if (NULL == pObject || NULL == ppPattern)
			return S_FALSE;

		IAVSPen* pPen = NULL;
		pObject->QueryInterface(__uuidof(IAVSPen), (void**)&pPen);

		if (NULL != pPen)
		{
			LONG lColor = 0;
			pPen->get_Color(&lColor);

			CoCreateInstance(__uuidof(CAVSSolidPattern), NULL, CLSCTX_INPROC, __uuidof(IAVSPattern), (void**)ppPattern);
			if (NULL != *ppPattern)
			{
				((CAVSSolidPattern*)(*ppPattern))->SetColor(lColor);
			}
			return S_OK;
		}

		IAVSBrush* pBrush = NULL;
		pObject->QueryInterface(__uuidof(IAVSBrush), (void**)&pBrush);

		if (NULL != pBrush)
		{
			LONG lColor = 0;
			pBrush->get_Color1(&lColor);

			CoCreateInstance(__uuidof(CAVSSolidPattern), NULL, CLSCTX_INPROC, __uuidof(IAVSPattern), (void**)ppPattern);
			if (NULL != *ppPattern)
			{
				((CAVSSolidPattern*)(*ppPattern))->SetColor(lColor);
			}
			return S_OK;
		}
		
		return S_FALSE;
	}

private:

	bool		   m_bStartSubPath;

	LONG		   m_lMode;

	friend class   SXPath;
	friend class   SImage;
	friend class   CAVSGraphicsPath;
	friend class   CAVSGraphics;
};
