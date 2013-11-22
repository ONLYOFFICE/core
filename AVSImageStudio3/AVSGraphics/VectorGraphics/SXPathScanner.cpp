#include "stdafx.h"

#include <stdlib.h>
#include <string.h>
#include "MemoryUtils.h"
#include "SMathExt.h"
#include "SXPath.h"
#include "SBitmap.h"
#include "SXPathScanner.h"
#include "..\Common\Pixels.h"

//-------------------------------------------------------------------------------------------------------------------------------

struct SIntersect 
{
	int nX0;     // Пересечение сегмента с полуотрезком [ y, y + 1 )
	int nX1;     //
	int nCount;  // EO/NZWN счетчик
};

static int CompareIntersect(const void *pFirstInter, const void *pSecondInter) 
{
	return ((SIntersect *)pFirstInter)->nX0 - ((SIntersect *)pSecondInter)->nX0;
}

//-------------------------------------------------------------------------------------------------------------------------------
// SXPathScanner
//-------------------------------------------------------------------------------------------------------------------------------

SXPathScanner::SXPathScanner(SXPath *pXPath, BOOL bEO) 
{
	m_pXPath = pXPath;
	m_bEO    = bEO;

	// Вычислим BBox
	if ( m_pXPath->m_nSegmentsCount == 0 ) 
	{
		m_nMinX = m_nMinY = 1;
		m_nMaxX = m_nMaxY = 0;
	} 
	else 
	{
		double dMinX, dMaxX, dMinY, dMaxY;
		SXPathSegment *pSegment = &m_pXPath->m_pSegments[0];
		if ( pSegment->dFirstX <= pSegment->dSecondX ) 
		{
			dMinX = pSegment->dFirstX;
			dMaxX = pSegment->dSecondX;
		} 
		else 
		{
			dMinX = pSegment->dSecondX;
			dMaxX = pSegment->dFirstX;
		}
		if ( pSegment->unFlags & XPathFlip ) 
		{
			dMinY = pSegment->dSecondY;
			dMaxY = pSegment->dFirstY;
		} 
		else 
		{
			dMinY = pSegment->dFirstY;
			dMaxY = pSegment->dSecondY;
		}

		for ( int nIndex = 1; nIndex < m_pXPath->m_nSegmentsCount; ++nIndex ) 
		{
			pSegment = &m_pXPath->m_pSegments[nIndex];
			if ( pSegment->dFirstX < dMinX ) 
			{
				dMinX = pSegment->dFirstX;
			} 
			else if ( pSegment->dFirstX > dMaxX ) 
			{
				dMaxX = pSegment->dFirstX;
			}

			if ( pSegment->dSecondX < dMinX ) 
			{
				dMinX = pSegment->dSecondX;
			} 
			else if ( pSegment->dSecondX > dMaxX ) 
			{
				dMaxX = pSegment->dSecondX;
			}

			if ( pSegment->unFlags & XPathFlip ) 
			{
				if ( pSegment->dFirstY > dMaxY ) 
				{
					dMaxY = pSegment->dFirstY;
				}
			} 
			else 
			{
				if (pSegment->dSecondY > dMaxY) 
				{
					dMaxY = pSegment->dSecondY;
				}
			}
		}
		m_nMinX = (int)floor(dMinX);
		m_nMaxX = (int)floor(dMaxX);
		m_nMinY = (int)floor(dMinY);
		m_nMaxY = (int)floor(dMaxY);
	}

	m_nInterY     = m_nMinY - 1;
	m_nXPathIndex = 0;

	m_pInter    = NULL;
	m_nInterLen = m_nInterSize = 0;
}

SXPathScanner::~SXPathScanner() 
{
	MemUtilsFree( m_pInter );
}

void SXPathScanner::GetBBoxAA(int *pnMinX, int *pnMinY, int *pnMaxX, int *pnMaxY) 
{
	*pnMinX = m_nMinX / AntiAliasingSize;
	*pnMinY = m_nMinY / AntiAliasingSize;
	*pnMaxX = m_nMaxX / AntiAliasingSize;
	*pnMaxY = m_nMaxY / AntiAliasingSize;
}

void SXPathScanner::GetSpanBounds(int nY, int *pnSpanMinX, int *pnSpanMaxX)
{
	if ( m_nInterY != nY ) 
	{
		ComputeIntersections( nY );
	}

	if ( m_nInterLen > 0 ) 
	{
		*pnSpanMinX = m_pInter[0].nX0;
		*pnSpanMaxX = m_pInter[m_nInterLen - 1].nX1;
	} 
	else 
	{
		*pnSpanMinX = m_nMaxX + 1;
		*pnSpanMaxX = m_nMaxX;
	}
}

BOOL SXPathScanner::IsInsidePath(int nX, int nY) 
{
	if ( m_nInterY != nY ) 
	{
		ComputeIntersections( nY );
	}

	int nCount = 0;
	for ( int nIndex = 0; nIndex < m_nInterLen && m_pInter[nIndex].nX0 <= nX; ++nIndex ) 
	{
		if ( nX <= m_pInter[nIndex].nX1 ) 
		{
			return TRUE;
		}
		nCount += m_pInter[nIndex].nCount;
	}
	return m_bEO ? (nCount & 1) : (nCount != 0);
}

BOOL SXPathScanner::IsInsidePath(int nX0, int nX1, int nY) 
{
	if ( m_nInterY != nY ) 
	{
		ComputeIntersections(nY);
	}

	int nCount = 0, nIndex = 0;
	for ( nIndex = 0; nIndex < m_nInterLen && m_pInter[nIndex].nX1 < nX0; ++nIndex ) 
	{
		nCount += m_pInter[nIndex].nCount;
	}

	// промежуток [ nX0, nXm ] всегда внутри Path
	int nXm = nX0 - 1;
	while ( nXm < nX1 ) 
	{
		if ( nIndex >= m_nInterLen ) 
		{
			return FALSE;
		}
		
		if ( m_pInter[nIndex].nX0 > nXm + 1 && !(m_bEO ? (nCount & 1) : (nCount != 0) ) ) 
		{
			return FALSE;
		}
		if ( m_pInter[nIndex].nX1 > nXm ) 
		{
			nXm = m_pInter[nIndex].nX1;
		}
		nCount += m_pInter[nIndex].nCount;
		++nIndex;
	}

	return TRUE;
}

BOOL SXPathScanner::GetNextSpan(int nY, int *pnX0, int *pnX1)
{
	if ( m_nInterY != nY ) 
	{
		ComputeIntersections( nY );
	}
	
	if ( m_nInterIndex >= m_nInterLen ) 
	{
		return FALSE;
	}

	int nX0 = m_pInter[m_nInterIndex].nX0;
	int nX1 = m_pInter[m_nInterIndex].nX1;

	m_nInterCount += m_pInter[m_nInterIndex].nCount;
	++m_nInterIndex;

	while ( m_nInterIndex < m_nInterLen && ( m_pInter[m_nInterIndex].nX0 <= nX1 || ( m_bEO ? (m_nInterCount & 1) : (m_nInterCount != 0) ) ) ) 
	{
		if ( m_pInter[m_nInterIndex].nX1 > nX1 ) 
		{
			nX1 = m_pInter[m_nInterIndex].nX1;
		}
		m_nInterCount += m_pInter[m_nInterIndex].nCount;
		++m_nInterIndex;
	}
	*pnX0 = nX0;
	*pnX1 = nX1;
	return TRUE;
}

void SXPathScanner::ComputeIntersections(int nY)
{
	double dSegMinX, dSegMaxX, dSegMinY, dSegMaxY, dX0, dX1;

	// Ищем первый сегмент, пересекающийся с [ nY, nY + 1 )
	int nFirst = ( nY >= m_nInterY ) ? m_nXPathIndex : 0;  

	while ( nFirst < m_pXPath->m_nSegmentsCount && m_pXPath->m_pSegments[nFirst].dFirstY < nY && m_pXPath->m_pSegments[nFirst].dSecondY < nY ) 
	{
		++nFirst;
	}
	m_nXPathIndex = nFirst;

	// Ищем все сегменты, пересекающиеся с [ nY, nY + 1 ), и создаем элемент Intersect для каждого
	m_nInterLen = 0;
	for ( int nIndex = nFirst; nIndex < m_pXPath->m_nSegmentsCount; ++nIndex ) 
	{
		SXPathSegment *pSegment = &m_pXPath->m_pSegments[nIndex];
		if ( pSegment->unFlags & XPathFlip ) 
		{
			dSegMinY = pSegment->dSecondY;
			dSegMaxY = pSegment->dFirstY;
		} 
		else 
		{
			dSegMinY = pSegment->dFirstY;
			dSegMaxY = pSegment->dSecondY;
		}

		//       ySegMin < nY + 1
		// nY <= ySegMax
		if ( dSegMinY >= nY + 1 ) 
		{
			break;
		}
		if ( dSegMaxY < nY ) 
		{
			continue;
		}

		if ( m_nInterLen == m_nInterSize ) 
		{
			if ( m_nInterSize == 0 ) 
			{
				m_nInterSize = 16;
			} 
			else 
			{
				m_nInterSize *= 2;
			}
			m_pInter = (SIntersect *)MemUtilsReallocArray( m_pInter, m_nInterSize, sizeof(SIntersect) );
		}

		if ( pSegment->unFlags & XPathHoriz ) 
		{
			dX0 = pSegment->dFirstX;
			dX1 = pSegment->dSecondX;
		} 
		else if ( pSegment->unFlags & XPathVert ) 
		{
			dX0 = dX1 = pSegment->dFirstX;
		} 
		else 
		{
			if ( pSegment->dFirstX < pSegment->dSecondX ) 
			{
				dSegMinX = pSegment->dFirstX;
				dSegMaxX = pSegment->dSecondX;
			} 
			else 
			{
				dSegMinX = pSegment->dSecondX;
				dSegMaxX = pSegment->dFirstX;
			}

			// Пересечение с верхней вершиной
			dX0 = pSegment->dFirstX + ((double)nY - pSegment->dFirstY) * pSegment->dDxDy;
			// Пересечение с нижней вершиной
			dX1 = pSegment->dFirstX + ((double)nY + 1 - pSegment->dFirstY) * pSegment->dDxDy;

			if ( dX0 < dSegMinX )
			{
				dX0 = dSegMinX;
			} 
			else if ( dX0 > dSegMaxX ) 
			{
				dX0 = dSegMaxX;
			}
			if ( dX1 < dSegMinX ) 
			{
				dX1 = dSegMinX;
			} 
			else if ( dX1 > dSegMaxX ) 
			{
				dX1 = dSegMaxX;
			}
		}
		if ( dX0 < dX1 )
		{
			m_pInter[m_nInterLen].nX0 = (int)floor(dX0);
			m_pInter[m_nInterLen].nX1 = (int)floor(dX1);
		} 
		else 
		{
			m_pInter[m_nInterLen].nX0 = (int)floor(dX1);
			m_pInter[m_nInterLen].nX1 = (int)floor(dX0);
		}
		if ( dSegMinY <= nY && (double)nY < dSegMaxY && !(pSegment->unFlags & XPathHoriz) ) 
		{
			m_pInter[m_nInterLen].nCount = m_bEO ? 1 : (pSegment->unFlags & XPathFlip) ? 1 : -1;
		} 
		else 
		{
			m_pInter[m_nInterLen].nCount = 0;
		}
		++m_nInterLen;
	}

	qsort( m_pInter, m_nInterLen, sizeof(SIntersect), &CompareIntersect );

	m_nInterY = nY;
	m_nInterIndex = 0;
	m_nInterCount = 0;
}

void SXPathScanner::RenderAALine(SBitmap *pAABuffer, int *pnX0, int *pnX1, int nY) 
{
	memset( pAABuffer->GetData(), 0, pAABuffer->GetStride() * pAABuffer->GetHeight() );
	int nMinX = pAABuffer->GetWidth();
	int nMaxX = -1;
	for ( int nIndexY = 0; nIndexY < AntiAliasingSize; ++nIndexY ) 
	{
		ComputeIntersections( AntiAliasingSize * nY + nIndexY );

		while ( m_nInterIndex < m_nInterLen ) 
		{
			int nX0 = m_pInter[m_nInterIndex].nX0;
			int nX1 = m_pInter[m_nInterIndex].nX1;
			m_nInterCount += m_pInter[m_nInterIndex].nCount;
			++m_nInterIndex;

			while ( m_nInterIndex < m_nInterLen && ( m_pInter[m_nInterIndex].nX0 <= nX1 || ( m_bEO ? (m_nInterCount & 1) : (m_nInterCount != 0) ) ) ) 
			{
				if ( m_pInter[m_nInterIndex].nX1 > nX1 ) 
				{
					nX1 = m_pInter[m_nInterIndex].nX1;
				}
				m_nInterCount += m_pInter[m_nInterIndex].nCount;
				++m_nInterIndex;
			}
			if ( nX0 < 0 ) 
			{
				nX0 = 0;
			}
			++nX1;
			if ( nX1 > pAABuffer->GetWidth() ) 
			{
				nX1 = pAABuffer->GetWidth();
			}

			if ( nX0 < nX1 ) 
			{
				int nTempX = nX0;
				SColorPointer pColor = pAABuffer->GetData() + nIndexY * pAABuffer->GetStride() + (nTempX >> 3);

				if ( nTempX & 7 ) 
				{
					unsigned char unMask = 0xff >> (nTempX & 7);
					if ( ( nTempX & ~7 ) == ( nX1 & ~7 ) ) 
					{
						unMask &= (unsigned char)(0xff00 >> (nX1 & 7));
					}
					*pColor++ |= unMask;
					nTempX = (nTempX & ~7) + 8;
				}
				for (; nTempX + 7 < nX1; nTempX += 8) 
				{
					*pColor++ |= 0xff;
				}
				if ( nTempX < nX1 ) 
				{
					*pColor |= (unsigned char)(0xff00 >> (nX1 & 7));
				}
			}
			if ( nX0 < nMinX ) 
			{
				nMinX = nX0;
			}
			if ( nX1 > nMaxX ) 
			{
				nMaxX = nX1;
			}
		}
	}
	*pnX0 = nMinX / AntiAliasingSize;
	*pnX1 = (nMaxX - 1) / AntiAliasingSize;
}

void SXPathScanner::RenderAALine(CPixels *pAABuffer, int *pnX0, int *pnX1, int nY) 
{
	int nWidth = pAABuffer->m_lWidth;
	int nStride = pAABuffer->m_lStride;
	
	memset( pAABuffer->m_pPixels, 0, nStride * pAABuffer->m_lHeight );
	int nMinX = nWidth;
	int nMaxX = -1;
	for ( int nIndexY = 0; nIndexY < AntiAliasingSize; ++nIndexY ) 
	{
		ComputeIntersections( AntiAliasingSize * nY + nIndexY );

		while ( m_nInterIndex < m_nInterLen ) 
		{
			int nX0 = m_pInter[m_nInterIndex].nX0;
			int nX1 = m_pInter[m_nInterIndex].nX1;
			m_nInterCount += m_pInter[m_nInterIndex].nCount;
			++m_nInterIndex;

			while ( m_nInterIndex < m_nInterLen && ( m_pInter[m_nInterIndex].nX0 <= nX1 || ( m_bEO ? (m_nInterCount & 1) : (m_nInterCount != 0) ) ) ) 
			{
				if ( m_pInter[m_nInterIndex].nX1 > nX1 ) 
				{
					nX1 = m_pInter[m_nInterIndex].nX1;
				}
				m_nInterCount += m_pInter[m_nInterIndex].nCount;
				++m_nInterIndex;
			}
			if ( nX0 < 0 ) 
			{
				nX0 = 0;
			}
			++nX1;
			if ( nX1 > nWidth ) 
			{
				nX1 = nWidth;
			}

			if ( nX0 < nX1 ) 
			{
				int nTempX = nX0;
				SColorPointer pColor = pAABuffer->m_pPixels + nIndexY * nStride + (nTempX >> 3);

				if ( nTempX & 7 ) 
				{
					unsigned char unMask = 0xff >> (nTempX & 7);
					if ( ( nTempX & ~7 ) == ( nX1 & ~7 ) ) 
					{
						unMask &= (unsigned char)(0xff00 >> (nX1 & 7));
					}
					*pColor++ |= unMask;
					nTempX = (nTempX & ~7) + 8;
				}
				for (; nTempX + 7 < nX1; nTempX += 8) 
				{
					*pColor++ |= 0xff;
				}
				if ( nTempX < nX1 ) 
				{
					*pColor |= (unsigned char)(0xff00 >> (nX1 & 7));
				}
			}
			if ( nX0 < nMinX ) 
			{
				nMinX = nX0;
			}
			if ( nX1 > nMaxX ) 
			{
				nMaxX = nX1;
			}
		}
	}
	*pnX0 = nMinX / AntiAliasingSize;
	*pnX1 = (nMaxX - 1) / AntiAliasingSize;
}

void SXPathScanner::ClipAALine(SBitmap *pAABuffer, int *pnX0, int *pnX1, int nY) 
{
	int nX0, nX1;

	for ( int nIndexY = 0; nIndexY < AntiAliasingSize; ++nIndexY ) 
	{
		int nXX = *pnX0 * AntiAliasingSize;
		ComputeIntersections( AntiAliasingSize * nY + nIndexY );
		while ( m_nInterIndex < m_nInterLen && nXX < (*pnX1 + 1) * AntiAliasingSize ) 
		{
			nX0 = m_pInter[m_nInterIndex].nX0;
			nX1 = m_pInter[m_nInterIndex].nX1;
			m_nInterCount += m_pInter[m_nInterIndex].nCount;
			++m_nInterIndex;

			while ( m_nInterIndex < m_nInterLen && ( m_pInter[m_nInterIndex].nX0 <= nX1 || ( m_bEO ? (m_nInterCount & 1) : (m_nInterCount != 0) ) ) ) 
			{
				if ( m_pInter[m_nInterIndex].nX1 > nX1 ) 
				{
					nX1 = m_pInter[m_nInterIndex].nX1;
				}
				m_nInterCount += m_pInter[m_nInterIndex].nCount;
				++m_nInterIndex;
			}
			if ( nX0 > pAABuffer->GetWidth() ) 
			{
				nX0 = pAABuffer->GetWidth();
			}

			// Устанавливаем 0
			if ( nXX < nX0 ) 
			{
				SColorPointer pColor = pAABuffer->GetData() + nIndexY * pAABuffer->GetStride() + (nXX >> 3);
				if ( nXX & 7 ) 
				{
					unsigned char unMask = (unsigned char)(0xff00 >> (nXX & 7));
					if ( (nXX & ~7) == (nX0 & ~7) ) 
					{
						unMask |= 0xff >> (nX0 & 7);
					}
					*pColor++ &= unMask;
					nXX = (nXX & ~7) + 8;
				}
				for (; nXX + 7 <= nX0; nXX += 8) 
				{
					*pColor++ = 0x00;
				}
				if ( nXX <= nX0 ) 
				{
					*pColor &= 0xff >> (nX0 & 7);
				}
			}
			if ( nX1 >= nXX ) 
			{
				nXX = nX1 + 1;
			}
		}
		nX0 = (*pnX1 + 1) * AntiAliasingSize;

		// Устанавливаем 0
		if ( nXX < nX0 ) 
		{
			SColorPointer pColor = pAABuffer->GetData() + nIndexY * pAABuffer->GetStride() + (nXX >> 3);
			if ( nXX & 7 ) 
			{
				unsigned char unMask = (unsigned char)(0xff00 >> (nXX & 7));
				if ( (nXX & ~7) == (nX0 & ~7) ) 
				{
					unMask &= 0xff >> (nX0 & 7);
				}
				*pColor++ &= unMask;
				nXX = (nXX & ~7) + 8;
			}
			for (; nXX + 7 <= nX0; nXX += 8) 
			{
				*pColor++ = 0x00;
			}
			if ( nXX <= nX0 ) 
			{
				*pColor &= 0xff >> (nX0 & 7);
			}
		}
	}
}

void SXPathScanner::ClipAALine(CPixels *pAABuffer, int *pnX0, int *pnX1, int nY) 
{
	int nX0, nX1;

	int nWidth = pAABuffer->m_lWidth;
	int nStride = pAABuffer->m_lStride;

	for ( int nIndexY = 0; nIndexY < AntiAliasingSize; ++nIndexY ) 
	{
		int nXX = *pnX0 * AntiAliasingSize;
		ComputeIntersections( AntiAliasingSize * nY + nIndexY );
		while ( m_nInterIndex < m_nInterLen && nXX < (*pnX1 + 1) * AntiAliasingSize ) 
		{
			nX0 = m_pInter[m_nInterIndex].nX0;
			nX1 = m_pInter[m_nInterIndex].nX1;
			m_nInterCount += m_pInter[m_nInterIndex].nCount;
			++m_nInterIndex;

			while ( m_nInterIndex < m_nInterLen && ( m_pInter[m_nInterIndex].nX0 <= nX1 || ( m_bEO ? (m_nInterCount & 1) : (m_nInterCount != 0) ) ) ) 
			{
				if ( m_pInter[m_nInterIndex].nX1 > nX1 ) 
				{
					nX1 = m_pInter[m_nInterIndex].nX1;
				}
				m_nInterCount += m_pInter[m_nInterIndex].nCount;
				++m_nInterIndex;
			}
			if ( nX0 > nWidth ) 
			{
				nX0 = nWidth;
			}

			// Устанавливаем 0
			if ( nXX < nX0 ) 
			{
				SColorPointer pColor = pAABuffer->m_pPixels + nIndexY * nStride + (nXX >> 3);
				if ( nXX & 7 ) 
				{
					unsigned char unMask = (unsigned char)(0xff00 >> (nXX & 7));
					if ( (nXX & ~7) == (nX0 & ~7) ) 
					{
						unMask |= 0xff >> (nX0 & 7);
					}
					*pColor++ &= unMask;
					nXX = (nXX & ~7) + 8;
				}
				for (; nXX + 7 <= nX0; nXX += 8) 
				{
					*pColor++ = 0x00;
				}
				if ( nXX <= nX0 ) 
				{
					*pColor &= 0xff >> (nX0 & 7);
				}
			}
			if ( nX1 >= nXX ) 
			{
				nXX = nX1 + 1;
			}
		}
		nX0 = (*pnX1 + 1) * AntiAliasingSize;

		// Устанавливаем 0
		if ( nXX < nX0 ) 
		{
			SColorPointer pColor = pAABuffer->m_pPixels + nIndexY * nStride + (nXX >> 3);
			if ( nXX & 7 ) 
			{
				unsigned char unMask = (unsigned char)(0xff00 >> (nXX & 7));
				if ( (nXX & ~7) == (nX0 & ~7) ) 
				{
					unMask &= 0xff >> (nX0 & 7);
				}
				*pColor++ &= unMask;
				nXX = (nXX & ~7) + 8;
			}
			for (; nXX + 7 <= nX0; nXX += 8) 
			{
				*pColor++ = 0x00;
			}
			if ( nXX <= nX0 ) 
			{
				*pColor &= 0xff >> (nX0 & 7);
			}
		}
	}
}
