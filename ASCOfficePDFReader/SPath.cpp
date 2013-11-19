#include "stdafx.h"

#include <string.h>
#include "MemoryUtils.h"
#include "SErrorCodes.h"
#include "SPath.h"

//-------------------------------------------------------------------------------------------------------------------------------
// SPath
//-------------------------------------------------------------------------------------------------------------------------------

// Path может быть в одном из трех следующий состо€ний:
//
// 1. Ќет текущей точки -- ни одного или более законченных SubPaths
//    [m_nCurSubpath == m_nPointsCount]
//
// 2. ќдна точка в SubPaths
//    [m_nCurSubpath == m_nPointsCount - 1]
//
// 3. ќткрытый SubPaths c двум€ или более точками
//    [m_nCurSubpath < m_nPointsCount - 1]

SPath::SPath() 
{
	m_pPoints = NULL;
	m_pFlags = NULL;
	m_nPointsCount = m_nSize = 0;
	m_nCurSubpath = 0;

	m_pHints = NULL;
	m_nHintsCount = m_nHintsSize = 0;
}

SPath::SPath(SPath *pPath) 
{
	m_nPointsCount = pPath->m_nPointsCount;
	m_nSize = pPath->m_nSize;
	m_pPoints = (SPathPoint    *)MemUtilsMallocArray( m_nSize, sizeof(SPathPoint) );
	m_pFlags  = (unsigned char *)MemUtilsMallocArray( m_nSize, sizeof(unsigned char) );

	memcpy( m_pPoints, pPath->m_pPoints, m_nPointsCount * sizeof(SPathPoint) );
	memcpy( m_pFlags, pPath->m_pFlags, m_nPointsCount * sizeof(unsigned char) );
	m_nCurSubpath = pPath->m_nCurSubpath;

	if ( pPath->m_pHints ) 
	{
		m_nHintsCount = m_nHintsSize = pPath->m_nHintsCount;
		m_pHints = (SPathHint *)MemUtilsMallocArray( m_nHintsSize, sizeof(SPathHint) );
		memcpy( m_pHints, pPath->m_pHints, m_nHintsCount * sizeof(SPathHint) );
	} 
	else 
	{
		m_pHints = NULL;
	}
}

SPath::~SPath() 
{
	MemUtilsFree( m_pPoints );
	MemUtilsFree( m_pFlags );
	MemUtilsFree( m_pHints );
}

void SPath::Resize(int nPointsCount) 
{
	if ( m_nPointsCount + nPointsCount > m_nSize ) 
	{
		if ( m_nSize == 0 ) 
		{
			m_nSize = 32;
		}
		while ( m_nSize < m_nPointsCount + nPointsCount ) 
		{
			m_nSize *= 2;
		}
		m_pPoints = (SPathPoint    *)MemUtilsReallocArray( m_pPoints, m_nSize, sizeof(SPathPoint) );
		m_pFlags  = (unsigned char *)MemUtilsReallocArray( m_pFlags, m_nSize, sizeof(unsigned char) );
	}
}

void SPath::Append(SPath *pPath) 
{
	m_nCurSubpath = m_nPointsCount + pPath->m_nCurSubpath;
	Resize( pPath->m_nPointsCount );

	for ( int nIndex = 0; nIndex < pPath->m_nPointsCount; ++nIndex ) 
	{
		m_pPoints[m_nPointsCount] = pPath->m_pPoints[nIndex];
		m_pFlags[m_nPointsCount]  = pPath->m_pFlags[nIndex];
		++m_nPointsCount;
	}
}

int SPath::MoveTo(double dX, double dY) 
{
	if ( OnePointSubpath() ) 
	{
		return SErrorWrongPath;
	}
	Resize(1);
	m_pPoints[m_nPointsCount].dX = dX;
	m_pPoints[m_nPointsCount].dY = dY;
	m_pFlags[m_nPointsCount] = SPathFirst | SPathLast;
	m_nCurSubpath = m_nPointsCount++;
	return SNoError;
}

int SPath::LineTo(double dX, double dY) 
{
	if ( NoCurrentPoint() ) 
	{
		return SErrorNoCurrentPoint;
	}
	m_pFlags[m_nPointsCount - 1] &= ~SPathLast;
	Resize(1);

	m_pPoints[m_nPointsCount].dX = dX;
	m_pPoints[m_nPointsCount].dY = dY;
	m_pFlags[m_nPointsCount] = SPathLast;
	++m_nPointsCount;
	return SNoError;
}

int SPath::CurveTo(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3) 
{
	if ( NoCurrentPoint() )
	{
		return SErrorNoCurrentPoint;
	}

	m_pFlags[m_nPointsCount-1] &= ~SPathLast;
	Resize(3);

	m_pPoints[m_nPointsCount].dX = dX1;
	m_pPoints[m_nPointsCount].dY = dY1;
	m_pFlags[m_nPointsCount] = SPathCurve;
	++m_nPointsCount;

	m_pPoints[m_nPointsCount].dX = dX2;
	m_pPoints[m_nPointsCount].dY = dY2;
	m_pFlags[m_nPointsCount] = SPathCurve;
	++m_nPointsCount;

	m_pPoints[m_nPointsCount].dX = dX3;
	m_pPoints[m_nPointsCount].dY = dY3;
	m_pFlags[m_nPointsCount] = SPathLast;
	++m_nPointsCount;

	return SNoError;
}

int SPath::Close() 
{
	if ( NoCurrentPoint() ) 
	{
		return SErrorNoCurrentPoint;
	}
	if ( m_nCurSubpath == m_nPointsCount - 1 || m_pPoints[m_nPointsCount - 1].dX != m_pPoints[m_nCurSubpath].dX || m_pPoints[m_nPointsCount - 1].dY != m_pPoints[m_nCurSubpath].dY ) 
	{
		LineTo( m_pPoints[m_nCurSubpath].dX, m_pPoints[m_nCurSubpath].dY );
	}

	m_pFlags[m_nCurSubpath] |= SPathClosed;
	m_pFlags[m_nPointsCount - 1] |= SPathClosed;
	m_nCurSubpath = m_nPointsCount;
	return SNoError;
}

void SPath::AddStrokeAdjustHint(int nFirstControl, int nSecondControl, int nFirstPoint, int nLastPoint) 
{
	if ( m_nHintsCount == m_nHintsSize ) 
	{
		m_nHintsSize = m_nHintsCount ? 2 * m_nHintsCount : 8;
		m_pHints = (SPathHint *)MemUtilsReallocArray( m_pHints, m_nHintsSize, sizeof(SPathHint) );
	}
	m_pHints[m_nHintsCount].nFirstControl  = nFirstControl;
	m_pHints[m_nHintsCount].nSecondControl = nSecondControl;
	m_pHints[m_nHintsCount].nFirstPoint = nFirstPoint;
	m_pHints[m_nHintsCount].nLastPoint  = nLastPoint;
	
	++m_nHintsCount;
}

void SPath::Offset(double dDx, double dDy) 
{
	for ( int nIndex = 0; nIndex < m_nPointsCount; ++nIndex ) 
	{
		m_pPoints[nIndex].dX += dDx;
		m_pPoints[nIndex].dY += dDy;
	}
}

BOOL SPath::GetCurPoint(double *pdX, double *pdY) 
{
	if ( NoCurrentPoint() ) 
	{
		return FALSE;
	}
	*pdX = m_pPoints[m_nPointsCount - 1].dX;
	*pdY = m_pPoints[m_nPointsCount - 1].dY;
	return TRUE;
}
