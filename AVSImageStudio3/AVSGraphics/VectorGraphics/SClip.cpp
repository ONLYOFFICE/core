#include "stdafx.h"

#include <stdlib.h>
#include <string.h>
#include "MemoryUtils.h"
#include "SErrorCodes.h"
#include "SPath.h"
#include "SXPath.h"
#include "SXPathScanner.h"
#include "SBitmap.h"
#include "SClip.h"

//-------------------------------------------------------------------------------------------------------------------------------
// SClip.nFlags
//-------------------------------------------------------------------------------------------------------------------------------

#define ClipEOFlag       0x01	// Используем правило Even-odd

//-------------------------------------------------------------------------------------------------------------------------------
// SClip
//-------------------------------------------------------------------------------------------------------------------------------

SClip::SClip(double dMinX, double dMinY, double dMaxX, double dMaxY, BOOL bAntialias) 
{
	m_bAntiAlias = bAntialias;
	if ( dMinX < dMaxX ) 
	{
		m_dMinX = dMinX;
		m_dMaxX = dMaxX;
	} 
	else 
	{
		m_dMinX = dMaxX;
		m_dMaxX = dMinX;
	}
	if ( dMinY < dMaxY ) 
	{
		m_dMinY = dMinY;
		m_dMaxY = dMaxY;
	} 
	else 
	{
		m_dMinY = dMaxY;
		m_dMaxY = dMinY;
	}

	m_nMinX = (int)floor( m_dMinX );
	m_nMinY = (int)floor( m_dMinY );
	m_nMaxX = (int)floor( m_dMaxX );
	m_nMaxY = (int)floor( m_dMaxY );

	m_ppPaths   = NULL;
	m_pFlags    = NULL;
	m_pScanners = NULL;

	m_nPathsCount = m_nSize = 0;
}

SClip::SClip(SClip *pClip) 
{
	m_bAntiAlias = pClip->m_bAntiAlias;

	m_dMinX = pClip->m_dMinX;
	m_dMinY = pClip->m_dMinY;
	m_dMaxX = pClip->m_dMaxX;
	m_dMaxY = pClip->m_dMaxY;
	m_nMinX = pClip->m_nMinX;
	m_nMinY = pClip->m_nMinY;
	m_nMaxX = pClip->m_nMaxX;
	m_nMaxY = pClip->m_nMaxY;

	m_nPathsCount = pClip->m_nPathsCount;
	m_nSize = pClip->m_nSize;

	m_ppPaths   = (SXPath        **)MemUtilsMallocArray( m_nSize, sizeof(SXPath *));
	m_pFlags    = (unsigned char  *)MemUtilsMallocArray( m_nSize, sizeof(unsigned char));
	m_pScanners = (SXPathScanner **)MemUtilsMallocArray( m_nSize, sizeof(SXPathScanner *));

	for ( int nIndex = 0; nIndex < m_nPathsCount; ++nIndex ) 
	{
		m_ppPaths[nIndex]   = pClip->m_ppPaths[nIndex]->Copy();
		m_pFlags[nIndex]    = pClip->m_pFlags[nIndex];
		m_pScanners[nIndex] = new SXPathScanner( m_ppPaths[nIndex], m_pFlags[nIndex] & ClipEOFlag );
	}
}

SClip::~SClip() 
{
	for ( int nIndex = 0; nIndex < m_nPathsCount; ++nIndex ) 
	{
		if ( m_ppPaths[nIndex] )
			delete m_ppPaths[nIndex];
		if ( m_pScanners[nIndex] )
			delete m_pScanners[nIndex];
	}
	
	MemUtilsFree( m_ppPaths );
	MemUtilsFree( m_pFlags );
	MemUtilsFree( m_pScanners );
}

void SClip::Resize(int nPathsCount) 
{
	if ( m_nPathsCount + nPathsCount > m_nSize ) 
	{
		if ( m_nSize == 0 ) 
		{
			m_nSize = 32;
		}
		while ( m_nSize < m_nPathsCount + nPathsCount ) 
		{
			m_nSize *= 2;
		}

		m_ppPaths   = (SXPath        **)MemUtilsReallocArray( m_ppPaths,   m_nSize, sizeof(SXPath *));
		m_pFlags    = (unsigned char  *)MemUtilsReallocArray( m_pFlags,    m_nSize, sizeof(unsigned char));
		m_pScanners = (SXPathScanner **)MemUtilsReallocArray( m_pScanners, m_nSize, sizeof(SXPathScanner *));
	}
}

void SClip::ResetToRect(double dX0, double dY0, double dX1, double dY1) 
{
	for ( int nIndex = 0; nIndex < m_nPathsCount; ++nIndex ) 
	{
		if ( m_ppPaths[nIndex] )
			delete m_ppPaths[nIndex];
		if ( m_pScanners[nIndex] )
			delete m_pScanners[nIndex];
	}

	MemUtilsFree( m_ppPaths );
	MemUtilsFree( m_pFlags );
	MemUtilsFree( m_pScanners );

	m_ppPaths   = NULL;
	m_pFlags    = NULL;
	m_pScanners = NULL;

	m_nPathsCount = m_nSize = 0;

	if ( dX0 < dX1 ) 
	{
		m_dMinX = dX0;
		m_dMaxX = dX1;
	} 
	else 
	{
		m_dMinX = dX1;
		m_dMaxX = dX0;
	}

	if ( dY0 < dY1 ) 
	{
		m_dMinY = dY0;
		m_dMaxY = dY1;
	} 
	else 
	{
		m_dMinY = dY1;
		m_dMaxY = dY0;
	}

	m_nMinX = (int)floor(m_dMinX);
	m_nMinY = (int)floor(m_dMinY);
	m_nMaxX = (int)floor(m_dMaxX);
	m_nMaxY = (int)floor(m_dMaxY);
}

int SClip::СlipToRect(double dX0, double dY0, double dX1, double dY1)
{
	if ( dX0 < dX1 ) 
	{
		if ( dX0 > m_dMinX ) 
		{
			m_dMinX = dX0;
			m_nMinX = (int)floor(m_dMinX);
		}
		if ( dX1 < m_dMaxX ) 
		{
			m_dMaxX = dX1;
			m_nMaxX = (int)floor(m_dMaxX);
		}
	} 
	else 
	{
		if ( dX1 > m_dMinX ) 
		{
			m_dMinX = dX1;
			m_nMinX = (int)floor(m_dMinX);
		}
		if ( dX0 < m_dMaxX ) 
		{
			m_dMaxX = dX0;
			m_nMaxX = (int)floor(m_dMaxX);
		}
	}

	if ( dY0 < dY1 ) 
	{
		if ( dY0 > m_dMinY ) 
		{
			m_dMinY = dY0;
			m_nMinY = (int)floor(m_dMinY);
		}
		if ( dY1 < m_dMaxY ) 
		{
			m_dMaxY = dY1;
			m_nMaxY = (int)floor(m_dMaxY);
		}
	} 
	else 
	{
		if ( dY1 > m_dMinY ) 
		{
			m_dMinY = dY1;
			m_nMinY = (int)floor(m_dMinY);
		}
		if ( dY0 < m_dMaxY ) 
		{
			m_dMaxY = dY0;
			m_nMaxY = (int)floor(m_dMaxY);
		}
	}
	return SNoError;
}

int SClip::СlipToPath(SPath *pPath, double *pMatrix, double dFlatness, BOOL bEO) 
{
	SXPath *pXPath = new SXPath( pPath, pMatrix, dFlatness, TRUE );

	// check for an empty path
	if ( pXPath->m_nSegmentsCount == 0 ) 
	{
		m_dMaxX = m_dMinX - 1;
		m_dMaxY = m_dMinY - 1;
		m_nMaxX = (int)floor(m_dMaxX);
		m_nMaxY = (int)floor(m_dMaxY);
		delete pXPath;
	} 
	else if ( pXPath->m_nSegmentsCount == 4 &&
		( ( pXPath->m_pSegments[0].dFirstX == pXPath->m_pSegments[0].dSecondX && pXPath->m_pSegments[0].dFirstX == pXPath->m_pSegments[1].dFirstX  && 
		    pXPath->m_pSegments[0].dFirstX == pXPath->m_pSegments[3].dSecondX && pXPath->m_pSegments[2].dFirstX == pXPath->m_pSegments[2].dSecondX &&
			pXPath->m_pSegments[2].dFirstX == pXPath->m_pSegments[1].dSecondX && pXPath->m_pSegments[2].dFirstX == pXPath->m_pSegments[3].dFirstX  &&
			pXPath->m_pSegments[1].dFirstY == pXPath->m_pSegments[1].dSecondY && pXPath->m_pSegments[1].dFirstY == pXPath->m_pSegments[0].dSecondY &&
			pXPath->m_pSegments[1].dFirstY == pXPath->m_pSegments[2].dFirstY  && pXPath->m_pSegments[3].dFirstY == pXPath->m_pSegments[3].dSecondY &&
			pXPath->m_pSegments[3].dFirstY == pXPath->m_pSegments[0].dFirstY  && pXPath->m_pSegments[3].dFirstY == pXPath->m_pSegments[2].dSecondY 
		   ) ||
		  ( pXPath->m_pSegments[0].dFirstY == pXPath->m_pSegments[0].dSecondY && pXPath->m_pSegments[0].dFirstY == pXPath->m_pSegments[1].dFirstY  &&
 		    pXPath->m_pSegments[0].dFirstY == pXPath->m_pSegments[3].dSecondY && pXPath->m_pSegments[2].dFirstY == pXPath->m_pSegments[2].dSecondY &&
			pXPath->m_pSegments[2].dFirstY == pXPath->m_pSegments[1].dSecondY && pXPath->m_pSegments[2].dFirstY == pXPath->m_pSegments[3].dFirstY  &&
			pXPath->m_pSegments[1].dFirstX == pXPath->m_pSegments[1].dSecondX && pXPath->m_pSegments[1].dFirstX == pXPath->m_pSegments[0].dSecondX &&
			pXPath->m_pSegments[1].dFirstX == pXPath->m_pSegments[2].dFirstX  && pXPath->m_pSegments[3].dFirstX == pXPath->m_pSegments[3].dSecondX &&
			pXPath->m_pSegments[3].dFirstX == pXPath->m_pSegments[0].dFirstX  && pXPath->m_pSegments[3].dFirstX == pXPath->m_pSegments[2].dSecondX ) ) ) 
	{
		СlipToRect( pXPath->m_pSegments[0].dFirstX, pXPath->m_pSegments[0].dFirstY, pXPath->m_pSegments[2].dFirstX, pXPath->m_pSegments[2].dFirstY );
		delete pXPath;
	} 
	else 
	{
		Resize(1);
		if ( m_bAntiAlias ) 
		{
			pXPath->AntiAliasingScale();
		}
		pXPath->Sort();

		m_ppPaths[m_nPathsCount] = pXPath;
		m_pFlags[m_nPathsCount]  = ( bEO ? ClipEOFlag : 0 );
		m_pScanners[m_nPathsCount] = new SXPathScanner( pXPath, bEO );
		++m_nPathsCount;
	}

	return SNoError;
}

BOOL SClip::IsInsideClip(int dX, int dY) 
{
	// Проверяем попала ли точка в общий рект Clip'а.
	if ( dX < m_nMinX || dX > m_nMaxX || dY < m_nMinY || dY > m_nMaxY ) 
	{
		return FALSE;
	}

	// Проверяем для каждого Path.
	if ( m_bAntiAlias ) 
	{
		for ( int nIndex = 0; nIndex < m_nPathsCount; ++nIndex ) 
		{
			if ( !m_pScanners[nIndex]->IsInsidePath( dX * AntiAliasingSize, dY * AntiAliasingSize ) ) 
			{
				return FALSE;
			}
		}
	} 
	else 
	{
		for ( int nIndex = 0; nIndex < m_nPathsCount; ++nIndex ) 
		{
			if ( !m_pScanners[nIndex]->IsInsidePath( dX, dY ) ) 
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

SClipResult SClip::CheckRectangle(int dRectMinX, int dRectMinY, int dRectMaxX, int dRectMaxY) 
{
	if ( (double)(dRectMaxX + 1) <= m_dMinX || (double)dRectMinX > m_dMaxX || (double)(dRectMaxY + 1) <= m_dMinY || (double)dRectMinY > m_dMaxY ) 
	{
		return clipAllOutside;
	}

	if ( (double)dRectMinX >= m_dMinX && (double)(dRectMaxX + 1) <= m_dMaxX && (double)dRectMinY >= m_dMinY && (double)(dRectMaxY + 1) <= m_dMaxY && m_nPathsCount == 0 ) 
	{
		return clipAllInside;
	}

	return clipPartial;
}

SClipResult SClip::CheckSpan(int nSpanMinX, int nSpanMaxX, int nSpanY) 
{
	if ( (double)(nSpanMaxX + 1) <= m_dMinX || (double)nSpanMinX > m_dMaxX || (double)(nSpanY + 1) <= m_dMinY || (double)nSpanY > m_dMaxY ) 
	{
		return clipAllOutside;
	}
	if ( !( (double)nSpanMinX >= m_dMinX && (double)(nSpanMaxX + 1) <= m_dMaxX && (double)nSpanY >= m_dMinY && (double)(nSpanY + 1) <= m_dMaxY ) ) 
	{
		return clipPartial;
	}

	if ( m_bAntiAlias ) 
	{
		for ( int nIndex = 0; nIndex < m_nPathsCount; ++nIndex ) 
		{
			if ( !m_pScanners[nIndex]->IsInsidePath( nSpanMinX * AntiAliasingSize, nSpanMaxX * AntiAliasingSize + ( AntiAliasingSize - 1 ), nSpanY * AntiAliasingSize ) ) 
			{
				return clipPartial;
			}
		}
	} 
	else 
	{
		for ( int nIndex = 0; nIndex < m_nPathsCount; ++nIndex ) 
		{
			if ( !m_pScanners[nIndex]->IsInsidePath( nSpanMinX, nSpanMaxX, nSpanY ) ) 
			{
				return clipPartial;
			}
		}
	}
	return clipAllInside;
}

void SClip::ClipAALine(SBitmap *pAABuffer, int *pnX0, int *pnX1, int nY) 
{
	int nXX, nYY;

	// Зануляем пиксели для которых nX < m_dMinX
	int nX0 = *pnX0 * AntiAliasingSize;
	int nX1 = (int)floor(m_dMinX * AntiAliasingSize);
	if ( nX1 > pAABuffer->GetWidth() ) 
	{
		nX1 = pAABuffer->GetWidth();
	}
	if ( nX0 < nX1 ) 
	{
		nX0 &= ~7;
		for ( nYY = 0; nYY < AntiAliasingSize; ++nYY ) 
		{
			SColorPointer pColor = pAABuffer->GetData() + nYY * pAABuffer->GetStride() + (nX0 >> 3);
			for ( nXX = nX0; nXX + 7 < nX1; nXX += 8 ) 
			{
				*pColor++ = 0;
			}
			if ( nXX < nX1 ) 
			{
				*pColor &= 0xff >> (nX1 & 7);
			}
		}
		*pnX0 = (int)floor(m_dMinX);
	}

	// Зануляем пиксели для которых nX > m_dMaxX
	nX0 = (int)floor(m_dMaxX * AntiAliasingSize) + 1;
	if ( nX0 < 0 ) 
	{
		nX0 = 0;
	}
	nX1 = (*pnX1 + 1) * AntiAliasingSize;
	if ( nX0 < nX1 ) 
	{
		for ( nYY = 0; nYY < AntiAliasingSize; ++nYY ) 
		{
			SColorPointer pColor = pAABuffer->GetData() + nYY * pAABuffer->GetStride() + (nX0 >> 3);
			nXX = nX0;
			if ( nXX & 7 ) 
			{
				*pColor &= 0xff00 >> (nXX & 7);
				nXX = (nXX & ~7) + 8;
				++pColor;
			}
			for (; nXX < nX1; nXX += 8) 
			{
				*pColor++ = 0;
			}
		}
		*pnX1 = (int)floor(m_dMaxX);
	}

	for ( int nIndex = 0; nIndex < m_nPathsCount; ++nIndex ) 
	{
		m_pScanners[nIndex]->ClipAALine( pAABuffer, pnX0, pnX1, nY );
	}
}
