#include "stdafx.h"

#include <stdlib.h>
#include <string.h>
#include "MemoryUtils.h"
#include "SMathExt.h"
#include "SPath.h"
#include "SXPath.h"

//-------------------------------------------------------------------------------------------------------------------------------

struct SXPathAdjust 
{
	int    nFirstPoint;  // Диапазон точек
	int    nLastPoint;   // 
	BOOL   bVertical;    // Вертикальный ли сегмент
	
	double dX0a;        //
	double dX0b;        //
	double dXma;        //  Границы
	double dXmb;        //
	double dX1a;        //
	double dX1b;        //

	double dX0;         //
	double dX1;         //
	double dXm;	        // 
};

//-------------------------------------------------------------------------------------------------------------------------------

inline void SXPath::Transform(double *pMatrix, double dUserX, double dUserY, double *pdDeviceX, double *pdDeviceY) 
{
	*pdDeviceX = dUserX * pMatrix[0] + dUserY * pMatrix[2] + pMatrix[4];
	*pdDeviceY = dUserX * pMatrix[1] + dUserY * pMatrix[3] + pMatrix[5];
}

//-------------------------------------------------------------------------------------------------------------------------------
// SXPath
//-------------------------------------------------------------------------------------------------------------------------------

SXPath::SXPath() 
{
	m_pSegments = NULL;
	m_nSegmentsCount = m_nSize = 0;
}

SXPath::SXPath(SPath *pPath, double *pMatrix, double dFlatness, BOOL bCloseSubpaths) 
{
	double dX0, dY0, dX1, dY1, dX2, dY2, dX3, dY3, dXSp, dYSp;

	SPathPoint *pPoints = (SPathPoint *)MemUtilsMallocArray( pPath->m_nPointsCount, sizeof(SPathPoint) );
	// Преобразуем точки
	for ( int nIndex = 0; nIndex < pPath->m_nPointsCount; ++nIndex ) 
	{
		Transform( pMatrix, pPath->m_pPoints[nIndex].dX, pPath->m_pPoints[nIndex].dY, &pPoints[nIndex].dX, &pPoints[nIndex].dY );
	}

	SXPathAdjust *pAdjusts;
	if ( pPath->m_pHints ) 
	{
		pAdjusts = (SXPathAdjust *)MemUtilsMallocArray( pPath->m_nHintsCount, sizeof(SXPathAdjust));
		for ( int nIndex = 0; nIndex < pPath->m_nHintsCount; ++nIndex ) 
		{
			SPathHint *pHint = &pPath->m_pHints[nIndex];
			dX0 = pPoints[pHint->nFirstControl     ].dX; dY0 = pPoints[pHint->nFirstControl     ].dY;
			dX1 = pPoints[pHint->nFirstControl  + 1].dX; dY1 = pPoints[pHint->nFirstControl  + 1].dY;
			dX2 = pPoints[pHint->nSecondControl    ].dX; dY2 = pPoints[pHint->nSecondControl    ].dY;
			dX3 = pPoints[pHint->nSecondControl + 1].dX; dY3 = pPoints[pHint->nSecondControl + 1].dY;
			
			double dAdj0 = 0, dAdj1 = 0;
			if ( dX0 == dX1 && dX2 == dX3 ) 
			{
				pAdjusts[nIndex].bVertical = TRUE;
				dAdj0 = dX0;
				dAdj1 = dX2;
			} 
			else if ( dY0 == dY1 && dY2 == dY3 ) 
			{
				pAdjusts[nIndex].bVertical = FALSE;
				dAdj0 = dY0;
				dAdj1 = dY2;
			} 
			else 
			{
				MemUtilsFree( pAdjusts );
				pAdjusts = NULL;
				break;
			}

			if ( dAdj0 > dAdj1 ) 
			{
				dX0 = dAdj0;
				dAdj0 = dAdj1;
				dAdj1 = dX0;
			}
			double dWidth = dAdj1 - dAdj0;
			int nRoundWidth = round( dWidth );
			if ( nRoundWidth == 0 ) 
			{
				nRoundWidth = 1;
			}
			pAdjusts[nIndex].dX0a = dAdj0 - 0.01;
			pAdjusts[nIndex].dX0b = dAdj0 + 0.01;
			pAdjusts[nIndex].dXma = (double)0.5 * ( dAdj0 + dAdj1 ) - 0.01;
			pAdjusts[nIndex].dXmb = (double)0.5 * ( dAdj0 + dAdj1 ) + 0.01;
			pAdjusts[nIndex].dX1a = dAdj1 - 0.01;
			pAdjusts[nIndex].dX1b = dAdj1 + 0.01;
			pAdjusts[nIndex].dX0 = (double)round( dAdj0 );
			pAdjusts[nIndex].dX1 = pAdjusts[nIndex].dX0 + nRoundWidth - 0.01;
			pAdjusts[nIndex].dXm = (double)0.5 * ( pAdjusts[nIndex].dX0 + pAdjusts[nIndex].dX1 );
			pAdjusts[nIndex].nFirstPoint = pHint->nFirstPoint;
			pAdjusts[nIndex].nLastPoint  = pHint->nLastPoint;
		}
	} 
	else 
	{
		pAdjusts = NULL;
	}

	// perform stroke adjustment
	if ( pAdjusts ) 
	{
		int nIndex = 0;
		SXPathAdjust *pCurAdjust;
		for ( nIndex = 0, pCurAdjust = pAdjusts; nIndex < pPath->m_nHintsCount; ++nIndex, ++pCurAdjust ) 
		{
			for ( int nJ = pCurAdjust->nFirstPoint; nJ <= pCurAdjust->nLastPoint; ++nJ ) 
			{
				StrokeAdjust( pCurAdjust, &pPoints[nJ].dX, &pPoints[nJ].dY );
			}
		}
		MemUtilsFree( pAdjusts );
	}

	m_pSegments = NULL;
	m_nSegmentsCount = m_nSize = 0;

	dX0 = dY0 = dXSp = dYSp = 0;
	int nCurSubpath = 0;
	int nCurSubpathX = 0;
	
	int nCount = 0;

	while ( nCount < pPath->m_nPointsCount ) 
	{
		// Пропускаем первую точку в SubPath
		if ( pPath->m_pFlags[nCount] & SPathFirst ) 
		{
			dX0 = pPoints[nCount].dX;
			dY0 = pPoints[nCount].dY;
			dXSp = dX0;
			dYSp = dY0;
			nCurSubpath = nCount;
			nCurSubpathX = m_nSegmentsCount;
			++nCount;
		} 
		else 
		{
			// Сегмент - кривая Безье
			if ( pPath->m_pFlags[nCount] & SPathCurve ) 
			{
				dX1 = pPoints[nCount + 0].dX;
				dY1 = pPoints[nCount + 0].dY;
				dX2 = pPoints[nCount + 1].dX;
				dY2 = pPoints[nCount + 1].dY;
				dX3 = pPoints[nCount + 2].dX;
				dY3 = pPoints[nCount + 2].dY;

				AddCurve( dX0, dY0, dX1, dY1, dX2, dY2, dX3, dY3, dFlatness, ( pPath->m_pFlags[nCount - 1] & SPathFirst) , ( pPath->m_pFlags[nCount + 2] & SPathLast ), !bCloseSubpaths && ( pPath->m_pFlags[nCount - 1] & SPathFirst ) && !( pPath->m_pFlags[nCount - 1] & SPathClosed ), !bCloseSubpaths && ( pPath->m_pFlags[nCount + 2] & SPathLast ) && !( pPath->m_pFlags[nCount + 2] & SPathClosed ) );
				dX0 = dX3;
				dY0 = dY3;
				nCount += 3;
			} 
			else // Сегмент - прямая линия
			{
				dX1 = pPoints[nCount].dX;
				dY1 = pPoints[nCount].dY;

				AddSegment( dX0, dY0, dX1, dY1, pPath->m_pFlags[nCount - 1] & SPathFirst, pPath->m_pFlags[nCount] & SPathLast, !bCloseSubpaths && ( pPath->m_pFlags[nCount - 1] & SPathFirst ) && !( pPath->m_pFlags[nCount - 1] & SPathClosed ), !bCloseSubpaths && ( pPath->m_pFlags[nCount] & SPathLast ) && !( pPath->m_pFlags[nCount] & SPathClosed ) );
				dX0 = dX1;
				dY0 = dY1;
				++nCount;
			}

			// Закрываем SubPath
			if ( bCloseSubpaths && ( pPath->m_pFlags[nCount - 1] & SPathLast ) && ( pPoints[nCount - 1].dX != pPoints[nCurSubpath].dX || pPoints[nCount - 1].dY != pPoints[nCurSubpath].dY ) ) 
			{
				AddSegment( dX0, dY0, dXSp, dYSp, FALSE, TRUE, FALSE, FALSE );
			}

		}
	}

	MemUtilsFree( pPoints );
}

void SXPath::StrokeAdjust(SXPathAdjust *pAdjust, double *pdX, double *pdY) 
{
	if ( pAdjust->bVertical ) 
	{
		double dX = *pdX;
		if ( dX > pAdjust->dX0a && dX < pAdjust->dX0b ) 
		{
			*pdX = pAdjust->dX0;
		} 
		else if ( dX > pAdjust->dXma && dX < pAdjust->dXmb ) 
		{
			*pdX = pAdjust->dXm;
		} 
		else if ( dX > pAdjust->dX1a && dX < pAdjust->dX1b ) 
		{
			*pdX = pAdjust->dX1;
		}
	} 
	else 
	{
		double dY = *pdY;
		if ( dY > pAdjust->dX0a && dY < pAdjust->dX0b ) 
		{
			*pdY = pAdjust->dX0;
		} 
		else if ( dY > pAdjust->dXma && dY < pAdjust->dXmb ) 
		{
			*pdY = pAdjust->dXm;
		} 
		else if ( dY > pAdjust->dX1a && dY < pAdjust->dX1b ) 
		{
			*pdY = pAdjust->dX1;
		}
	}
}

SXPath::SXPath(SXPath *pXPath) 
{
	m_nSegmentsCount = pXPath->m_nSegmentsCount;
	m_nSize = pXPath->m_nSize;

	m_pSegments = (SXPathSegment *)MemUtilsMallocArray( m_nSize, sizeof(SXPathSegment) );
	memcpy( m_pSegments, pXPath->m_pSegments, m_nSegmentsCount * sizeof(SXPathSegment) );
}

SXPath::~SXPath()
{
	MemUtilsFree( m_pSegments );
}

void SXPath::Resize(int nSegmentsCount) 
{
	if ( m_nSegmentsCount + nSegmentsCount > m_nSize ) 
	{
		if ( m_nSize == 0 ) 
		{
			m_nSize = 32;
		}
		while ( m_nSize < m_nSegmentsCount + nSegmentsCount ) 
		{
			m_nSize *= 2;
		}
		m_pSegments = (SXPathSegment *)MemUtilsReallocArray( m_pSegments, m_nSize, sizeof(SXPathSegment) );
	}
}

void SXPath::AddCurve(double dX0, double dY0, double dX1, double dY1, double dX2, double dY2, double dX3, double dY3, double dFlatness, BOOL bFirst, BOOL bLast, BOOL bEnd0, BOOL bEnd1) 
{
	double arrSegX[MaxCurveSplits + 1][3];
	double arrSegY[MaxCurveSplits + 1][3];
	int arrNext[MaxCurveSplits + 1];
	double xl0, xl1, xl2, xr0, xr1, xr2, xr3, xx1, xx2, xh;
	double yl0, yl1, yl2, yr0, yr1, yr2, yr3, yy1, yy2, yh;
	double dx, dy, mx, my, d1, d2;

	double dFlatness_2 = dFlatness * dFlatness;

	// Начальный сегмент
	int nPart1 = 0, nPart2 = MaxCurveSplits;
	arrSegX[nPart1][0] = dX0; arrSegY[nPart1][0] = dY0;
	arrSegX[nPart1][1] = dX1; arrSegY[nPart1][1] = dY1;
	arrSegX[nPart1][2] = dX2; arrSegY[nPart1][2] = dY2;
	arrSegX[nPart2][0] = dX3; arrSegY[nPart2][0] = dY3;
	arrNext[nPart1] = nPart2;

	while ( nPart1 < MaxCurveSplits ) 
	{
		// Следующий сегмент
		xl0 = arrSegX[nPart1][0];  yl0 = arrSegY[nPart1][0];
		xx1 = arrSegX[nPart1][1];  yy1 = arrSegY[nPart1][1];
		xx2 = arrSegX[nPart1][2];  yy2 = arrSegY[nPart1][2];
		nPart2 = arrNext[nPart1];
		xr3 = arrSegX[nPart2][0];  yr3 = arrSegY[nPart2][0];

		// Вычисляем расстояние от контрольных точек до средних точек прямой линии. (Вычисление не совсем точное, но быстрое)
		mx = (xl0 + xr3) * 0.5;
		my = (yl0 + yr3) * 0.5;
		dx = xx1 - mx;
		dy = yy1 - my;
		d1 = dx*dx + dy*dy;
		dx = xx2 - mx;
		dy = yy2 - my;
		d2 = dx*dx + dy*dy;

		// Если сегмент уже достаточно плоский или больше делений невозомжно сделать, добавляем прямую линию
		if ( nPart2 - nPart1 == 1 || ( d1 <= dFlatness_2 && d2 <= dFlatness_2 ) ) 
		{
			AddSegment( xl0, yl0, xr3, yr3, nPart1 == 0 && bFirst, nPart2 == MaxCurveSplits && bLast, nPart1 == 0 && bEnd0, nPart2 == MaxCurveSplits && bEnd1 ); 
			nPart1 = nPart2;
		} 
		else // в противном случае, разделяем кривую на части
		{
			xl1 = (xl0 + xx1) * 0.5;
			yl1 = (yl0 + yy1) * 0.5;
			xh = (xx1 + xx2) * 0.5;
			yh = (yy1 + yy2) * 0.5;
			xl2 = (xl1 + xh) * 0.5;
			yl2 = (yl1 + yh) * 0.5;
			xr2 = (xx2 + xr3) * 0.5;
			yr2 = (yy2 + yr3) * 0.5;
			xr1 = (xh + xr2) * 0.5;
			yr1 = (yh + yr2) * 0.5;
			xr0 = (xl2 + xr1) * 0.5;
			yr0 = (yl2 + yr1) * 0.5;

			int nPart3 = ( nPart1 + nPart2 ) / 2;
			arrSegX[nPart1][1] = xl1;  arrSegY[nPart1][1] = yl1;
			arrSegX[nPart1][2] = xl2;  arrSegY[nPart1][2] = yl2;
			arrNext[nPart1] = nPart3;

			arrSegX[nPart3][0] = xr0;  arrSegY[nPart3][0] = yr0;
			arrSegX[nPart3][1] = xr1;  arrSegY[nPart3][1] = yr1;
			arrSegX[nPart3][2] = xr2;  arrSegY[nPart3][2] = yr2;
			arrNext[nPart3] = nPart2;
		}
	}
}

void SXPath::AddSegment(double dX0, double dY0, double dX1, double dY1, BOOL bFirst, BOOL bLast, BOOL bEnd0, BOOL bEnd1) 
{
	Resize(1);

	m_pSegments[m_nSegmentsCount].dFirstX  = dX0;
	m_pSegments[m_nSegmentsCount].dFirstY  = dY0;
	m_pSegments[m_nSegmentsCount].dSecondX = dX1;
	m_pSegments[m_nSegmentsCount].dSecondY = dY1;
	m_pSegments[m_nSegmentsCount].unFlags  = 0;

	if ( bFirst ) 
	{
		m_pSegments[m_nSegmentsCount].unFlags |= XPathFirst;
	}
	if ( bLast ) 
	{
		m_pSegments[m_nSegmentsCount].unFlags |= XPathLast;
	}
	if ( bEnd0 ) 
	{
		m_pSegments[m_nSegmentsCount].unFlags |= XPathEnd0;
	}
	if ( bEnd1 ) 
	{
		m_pSegments[m_nSegmentsCount].unFlags |= XPathEnd1;
	}

	if ( dY1 == dY0 ) 
	{
		m_pSegments[m_nSegmentsCount].dDxDy = m_pSegments[m_nSegmentsCount].dDyDx = 0;
		m_pSegments[m_nSegmentsCount].unFlags |= XPathHoriz;
		if ( dX1 == dX0 ) 
		{
			m_pSegments[m_nSegmentsCount].unFlags |= XPathVert;
		}
	} 
	else if ( dX1 == dX0 ) 
	{
		m_pSegments[m_nSegmentsCount].dDxDy = m_pSegments[m_nSegmentsCount].dDyDx = 0;
		m_pSegments[m_nSegmentsCount].unFlags |= XPathVert;
	} 
	else 
	{
		m_pSegments[m_nSegmentsCount].dDxDy = ( dX1 - dX0 ) / ( dY1 - dY0 );
		m_pSegments[m_nSegmentsCount].dDyDx = (double)1 / m_pSegments[m_nSegmentsCount].dDxDy;
	}
	if ( dY0 > dY1 ) 
	{
		m_pSegments[m_nSegmentsCount].unFlags |= XPathFlip;
	}
	++m_nSegmentsCount;
}

static int CompareXPathSegments(const void *pFirstSegment, const void *pSecondSegment) 
{
	SXPathSegment *pSegment0 = (SXPathSegment *)pFirstSegment;
	SXPathSegment *pSegment1 = (SXPathSegment *)pSecondSegment;

	double dX0, dY0, dX1, dY1;

	if ( pSegment0->unFlags & XPathFlip ) 
	{
		dX0 = pSegment0->dSecondX;
		dY0 = pSegment0->dSecondY;
	} 
	else 
	{
		dX0 = pSegment0->dFirstX;
		dY0 = pSegment0->dFirstY;
	}

	if ( pSegment1->unFlags & XPathFlip ) 
	{
		dX1 = pSegment1->dSecondX;
		dY1 = pSegment1->dSecondY;
	} 
	else 
	{
		dX1 = pSegment1->dFirstX;
		dY1 = pSegment1->dFirstY;
	}

	if ( dY0 != dY1 ) 
	{
		return ( dY0 > dY1 ) ? 1 : -1;
	}

	if ( dX0 != dX1 ) 
	{
		return ( dX0 > dX1 ) ? 1 : -1;
	}
	return 0;
}

void SXPath::AntiAliasingScale() 
{
	SXPathSegment *pSegment = NULL;
	int nIndex;

	for ( nIndex = 0, pSegment = m_pSegments; nIndex < m_nSegmentsCount; ++nIndex, ++pSegment ) 
	{
		pSegment->dFirstX  *= AntiAliasingSize;
		pSegment->dFirstY  *= AntiAliasingSize;
		pSegment->dSecondX *= AntiAliasingSize;
		pSegment->dSecondY *= AntiAliasingSize;
	}
}

void SXPath::Sort() 
{
	qsort( m_pSegments, m_nSegmentsCount, sizeof(SXPathSegment), &CompareXPathSegments );
}
