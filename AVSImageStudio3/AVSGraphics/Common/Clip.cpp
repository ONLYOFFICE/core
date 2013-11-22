#include "stdafx.h"
#include "Clip.h"
#include "..\VectorGraphics\MemoryUtils.h"

#include "..\VectorGraphics\SPath.h"
#include "..\VectorGraphics\SXPath.h"
#include "..\VectorGraphics\SXPathScanner.h"
#include "..\VectorGraphics\SBitmap.h"

#include "..\Objects\Structures.h"
#include "Pixels.h"

void CClip::Reset(double dMinX, double dMinY, double dMaxX, double dMaxY, BOOL bAntialias)
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
	m_nPathsCount = m_nSize = 0;

	m_dLeft		= dMinX;
	m_dTop		= dMinY;
	m_dRight	= dMaxX;
	m_dBottom	= dMaxY;		
}

void CClip::Reset()
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
	m_nPathsCount = m_nSize = 0;
}

void CClip::ClipAALine(CPixels* pAABuffer, int *pnX0, int *pnX1, int nY)
{
	int nXX = 0, nYY = 0;

	LONG lStride = pAABuffer->m_lStride;

	// Зануляем пиксели для которых nX < m_dMinX
	int nX0 = *pnX0 * AntiAliasingSize;
	int nX1 = (int)floor(m_dMinX * AntiAliasingSize);
	if ( nX1 > pAABuffer->m_lWidth ) 
	{
		nX1 = pAABuffer->m_lWidth;
	}
	if ( nX0 < nX1 ) 
	{
		nX0 &= ~7;
		for ( nYY = 0; nYY < AntiAliasingSize; ++nYY ) 
		{
			SColorPointer pColor = pAABuffer->m_pPixels + nYY * lStride + (nX0 >> 3);
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
			SColorPointer pColor = pAABuffer->m_pPixels + nYY * lStride + (nX0 >> 3);
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