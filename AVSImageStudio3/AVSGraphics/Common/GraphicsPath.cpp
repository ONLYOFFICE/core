#include "stdafx.h"

#include <string.h>
#include "MemoryUtils.h"
#include "..\VectorGraphics\SErrorCodes.h"
#include "GraphicsPath.h"
#include "..\VectorGraphics\SMathExt.h"


CGraphicsPath::CGraphicsPath() : SPath()
{
	m_bStartSubPath = false;
	m_lMode = 0;
}
CGraphicsPath::CGraphicsPath(const CGraphicsPath &oSrc) : SPath((SPath*)&oSrc)
{
	m_bStartSubPath = oSrc.m_bStartSubPath;
	m_lMode			= oSrc.m_lMode;
}
CGraphicsPath& CGraphicsPath::operator=(const CGraphicsPath& oSrc)
{
	return *this;
}

CGraphicsPath::~CGraphicsPath()
{
}


// ------------------------------------------------
int CGraphicsPath::EllipseArc(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, BOOL bClockDirection)
{
	int nRet = 0;
	
	while ( fAngle1 < 0 )
		fAngle1 += 360;

	while ( fAngle1 > 360 )
		fAngle1 -= 360;

	while ( fAngle2 < 0 )
		fAngle2 += 360;

	while ( fAngle2 >= 360 )
		fAngle2 -= 360;

	if ( !bClockDirection )
	{
		if ( fAngle1 <= fAngle2 )
			nRet = EllipseArc2( fX, fY, fXRad, fYRad, fAngle1, fAngle2, FALSE );
		else
		{
			nRet += EllipseArc2( fX, fY, fXRad, fYRad, fAngle1, 360, FALSE );
			nRet += EllipseArc2( fX, fY, fXRad, fYRad, 0, fAngle2, FALSE );
		}
	}
	else
	{
		if ( fAngle1 >= fAngle2 )
			nRet = EllipseArc2( fX, fY, fXRad, fYRad, fAngle1, fAngle2, TRUE );
		else
		{
			nRet += EllipseArc2( fX, fY, fXRad, fYRad, fAngle1, 0, TRUE );
			nRet += EllipseArc2( fX, fY, fXRad, fYRad, 360, fAngle2, TRUE );
		}
	}
    return nRet;
}

double CGraphicsPath::AngToEllPrm(double fAngle, double fXRad, double fYRad)
{
	// Функция для перевода реального угла в параметрическое задание эллписа
	// т.е. x= a cos(t) y = b sin(t) - параметрическое задание эллписа.
	// x = r cos(p), y = r sin(p) => t = atan2( sin(p) / b, cos(p) / a );
	return atan2( sin( fAngle ) / fYRad,  cos( fAngle ) / fXRad );
}

int CGraphicsPath::EllipseArc2(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, BOOL bClockDirection)
{
	// переведем углы в радианы
	int nRet = 0;

	double dAngle1 = fAngle1 * 3.141592 / 180;
	double dAngle2 = fAngle2 * 3.141592 / 180;

	// Выясним в каких четвертях находятся начальная и конечная точки
	unsigned int nFirstPointQuard  = int(fAngle1) / 90 + 1; 
	unsigned int nSecondPointQuard = int(fAngle2) / 90 + 1;
	nSecondPointQuard = min( 4, max( 1, nSecondPointQuard ) );
	nFirstPointQuard  = min( 4, max( 1, nFirstPointQuard ) );
	// Проведем линию в начальную точку дуги
	double fStartX = 0.0, fStartY = 0.0, fEndX = 0.0, fEndY = 0.0;

	fStartX = fX + fXRad * cos( AngToEllPrm( dAngle1, fXRad, fYRad ) );
	fStartY = fY + fYRad * sin( AngToEllPrm( dAngle1, fXRad, fYRad ) );

	LineTo(fStartX, fStartY);

	// Дальше рисуем по четверям

	double fCurX = fStartX, fCurY = fStartY;
	double dStartAngle = dAngle1;
	double dEndAngle = 0;

	if ( !bClockDirection )
	{
		for( unsigned int nIndex = nFirstPointQuard; nIndex <= nSecondPointQuard; nIndex++ ) 
		{
			if ( nIndex == nSecondPointQuard )
				dEndAngle = dAngle2;
			else
				dEndAngle = (90 * (nIndex ) ) * 3.141592f / 180;
			if ( !( nIndex == nFirstPointQuard ) )
				dStartAngle = (90 * (nIndex - 1 ) ) * 3.141592f / 180;

			EllipseArc3(fX, fY, fXRad, fYRad, AngToEllPrm( dStartAngle, fXRad, fYRad ), AngToEllPrm( dEndAngle, fXRad, fYRad ), &fEndX, &fEndY, FALSE);
		}
	}
	else
	{
		for( unsigned int nIndex = nFirstPointQuard; nIndex >= nSecondPointQuard; nIndex-- ) 
		{
			if ( nIndex == nFirstPointQuard )
				dStartAngle = dAngle1;
			else
				dStartAngle = (90 * (nIndex ) ) * 3.141592f / 180;
			if ( !( nIndex == nSecondPointQuard ) )
				dEndAngle = (90 * (nIndex - 1 ) ) * 3.141592f / 180;
			else
				dEndAngle = dAngle2;

			EllipseArc3(fX, fY, fXRad, fYRad, AngToEllPrm( dStartAngle, fXRad, fYRad ), AngToEllPrm( dEndAngle, fXRad, fYRad ), &fEndX, &fEndY, FALSE);
		}
	}

    return nRet;
}

int CGraphicsPath::EllipseArc3(double fX, double fY, double fXRad, double fYRad, double dAngle1, double dAngle2, double *pfXCur, double *pfYCur, BOOL bClockDirection = FALSE)
{
	// Рассчитаем начальную, конечную и контрольные точки
	double fX1  = 0.0, fX2  = 0.0, fY1  = 0.0, fY2  = 0.0;
	double fCX1 = 0.0, fCX2 = 0.0, fCY1 = 0.0, fCY2 = 0.0;

	double fAlpha = sin( dAngle2 - dAngle1 ) * ( sqrt( 4.0 + 3.0 * tan( (dAngle2 - dAngle1) / 2.0 ) * tan( (dAngle2 - dAngle1) / 2.0 ) ) - 1.0 ) / 3.0;

	double fKoef = 1;

	fX1 = fX + fXRad * cos( dAngle1 );
	fY1 = fY + fYRad * sin( dAngle1 );

	fX2 = fX + fXRad * cos( dAngle2 );
	fY2 = fY + fYRad * sin( dAngle2 );

	fCX1 = fX1 - fAlpha * fXRad * sin ( dAngle1 );
	fCY1 = fY1 + fAlpha * fYRad * cos ( dAngle1 );

	fCX2 = fX2 + fAlpha * fXRad * sin ( dAngle2 );
	fCY2 = fY2 - fAlpha * fYRad * cos ( dAngle2 );

	if ( !bClockDirection )
	{
		CurveTo(fCX1, fCY1, fCX2, fCY2, fX2, fY2);

		*pfXCur = fX2; 
		*pfYCur = fY2;
	}
	else
	{
		CurveTo(fCX2, fCY2, fCX1, fCY1, fX1, fY1);

		*pfXCur = fX1; 
		*pfYCur = fY1;
	}
    return 0;
}

int CGraphicsPath::Ellipse(double fX, double fY, double fXRad, double fYRad)
{
	MoveTo(fX - fXRad, fY);

    double c_fKappa = 0.552;
	CurveTo(fX - fXRad, fY + fYRad * c_fKappa, fX - fXRad * c_fKappa, fY + fYRad, fX, fY + fYRad);
	CurveTo(fX + fXRad * c_fKappa, fY + fYRad, fX + fXRad, fY + fYRad * c_fKappa, fX + fXRad, fY);
	CurveTo(fX + fXRad, fY - fYRad * c_fKappa, fX + fXRad * c_fKappa, fY - fYRad, fX, fY - fYRad);
	CurveTo(fX - fXRad * c_fKappa, fY - fYRad, fX - fXRad, fY - fYRad * c_fKappa, fX - fXRad, fY);

    return 0;
}

