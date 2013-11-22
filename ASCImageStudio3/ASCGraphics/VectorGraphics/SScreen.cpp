#include "stdafx.h"

#include <stdlib.h>
#include <string.h>
#include "MemoryUtils.h"
#include "SMathExt.h"
#include "SScreen.h"

//-------------------------------------------------------------------------------------------------------------------------------

static SScreenParams DefaultParams = 
{
	sscreenDispersed, // Type
	               2, // Size
	               2, // Dot Radius
	             1.0, // Gamma
	             0.0, // Black Threshold
	             1.0  // White Threshold
};

//-------------------------------------------------------------------------------------------------------------------------------

struct SScreenPoint 
{
	int nX;
	int nY;
	int nDist;
};

static int CompareDistances(const void *pFirstPoint, const void *pSecondPoint) 
{
	return ((SScreenPoint *)pFirstPoint)->nDist - ((SScreenPoint *)pSecondPoint)->nDist;
}

//-------------------------------------------------------------------------------------------------------------------------------
// SScreen
//-------------------------------------------------------------------------------------------------------------------------------

// If <clustered> is true, this generates a 45 degree screen using a circular dot spot function.  DPI = resolution / ((size / 2) *
// sqrt(2)).  If <clustered> is false, this generates an optimal threshold matrix using recursive tesselation.  Gamma correction
// (gamma = 1 / 1.33) is also computed here.
SScreen::SScreen(SScreenParams *pParams) 
{
	if ( !pParams )
	{
		pParams = &DefaultParams;
	}

	switch ( pParams->eType ) 
	{
	case sscreenDispersed:
		// m_nSize должен быть степенью 2
		for ( m_nSize = 1; m_nSize < pParams->nSize; m_nSize <<= 1 ) ;
		m_pMatrix = (unsigned char *)MemUtilsMallocArray( m_nSize * m_nSize, sizeof(unsigned char) );
		BuildDispersedMatrix( m_nSize / 2, m_nSize / 2, 1, m_nSize / 2, 1 );
		break;
	case sscreenClustered:
		// m_nSize должен быть четным
		m_nSize = (pParams->nSize >> 1) << 1;
		if ( m_nSize < 2 ) 
		{
			m_nSize = 2;
		}
		m_pMatrix = (unsigned char *)MemUtilsMallocArray( m_nSize * m_nSize, sizeof(unsigned char) );
		BuildClusteredMatrix();
		break;

	case sscreenStochasticClustered:
		// m_nSize должен быть не меньше, чем 2 * Radius
		if ( pParams->nSize < 2 * pParams->nDotRadius )
		{
			m_nSize = 2 * pParams->nDotRadius;
		} 
		else 
		{
			m_nSize = pParams->nSize;
		}
		m_pMatrix = (unsigned char *)MemUtilsMallocArray( m_nSize * m_nSize, sizeof(unsigned char) );
		BuildSCDMatrix( pParams->nDotRadius );
		break;
	}

	// Выполняем гамма-коррекцию и вычисляем m_unMinValue/m_unMaxValue значения
	m_unMinValue = 255;
	m_unMaxValue = 0;

	unsigned char unBlack = round( (double)255.0 * pParams->dBlackThreshold );
	if ( unBlack < 1 ) 
	{
		unBlack = 1;
	}
	unsigned char unWhite = round( (double)255.0 * pParams->dWhiteThreshold );
	if ( unWhite > 255 ) 
	{
		unWhite = 255;
	}

	for ( int nIndex = 0; nIndex < m_nSize * m_nSize; ++nIndex ) 
	{
		unsigned char unPixel = round( (double)255.0 * pow((double)m_pMatrix[nIndex] / 255.0, pParams->dGamma ) );
		if ( unPixel < unBlack ) 
		{
			unPixel = unBlack;
		} 
		else if ( unPixel >= unWhite ) 
		{
			unPixel = unWhite;
		}
		m_pMatrix[nIndex] = unPixel;

		if ( unPixel < m_unMinValue ) 
		{
			m_unMinValue = unPixel;
		} 
		else if ( unPixel > m_unMaxValue ) 
		{
			m_unMaxValue = unPixel;
		}
	}
}

void SScreen::BuildDispersedMatrix(int nX, int nY, int nValue, int nDelta, int nOffset) 
{
	if ( nDelta == 0 ) 
	{
		// [1, m_nSize ^ 2] --> [1, 255]
		m_pMatrix[nX * m_nSize + nY] = 1 + (254 * (nValue - 1)) / (m_nSize * m_nSize - 1);
	} 
	else 
	{
		BuildDispersedMatrix(                          nX,                      nY, nValue              , nDelta / 2, 4 * nOffset );
		BuildDispersedMatrix( (nX +     nDelta) % m_nSize, (nY + nDelta) % m_nSize, nValue +     nOffset, nDelta / 2, 4 * nOffset );
		BuildDispersedMatrix( (nX +     nDelta) % m_nSize,                      nY, nValue + 2 * nOffset, nDelta / 2, 4 * nOffset );
		BuildDispersedMatrix( (nX + 2 * nDelta) % m_nSize, (nY + nDelta) % m_nSize, nValue + 3 * nOffset, nDelta / 2, 4 * nOffset );
	}
}

void SScreen::BuildClusteredMatrix() 
{
	double dDx, dDy;

	int nSize_2 = m_nSize >> 1;

	// Инициализация
	for ( int nY = 0; nY < m_nSize; ++nY ) 
	{
		for ( int nX = 0; nX < m_nSize; ++nX ) 
		{
			m_pMatrix[ nY * m_nSize + nX ] = 0;
		}
	}

	// build the distance matrix
	double *pDistance = (double *)MemUtilsMallocArray( m_nSize * nSize_2, sizeof(double) );

	for ( int nY = 0; nY < nSize_2; ++nY ) 
	{
		for ( int nX = 0; nX < nSize_2; ++nX ) 
		{
			if ( nX + nY < nSize_2 - 1 ) 
			{
				dDx = (double)nX + 0.5 - 0;
				dDy = (double)nY + 0.5 - 0;
			} 
			else 
			{
				dDx = (double)nX + 0.5 - (double)nSize_2;
				dDy = (double)nY + 0.5 - (double)nSize_2;
			}
			pDistance[nY * nSize_2 + nX] = dDx * dDx + dDy * dDy;
		}
	}
	
	for ( int nY = 0; nY < nSize_2; ++nY ) 
	{
		for ( int nX = 0; nX < nSize_2; ++nX ) 
		{
			if ( nX < nY ) 
			{
				dDx = (double)nX + 0.5 - 0;
				dDy = (double)nY + 0.5 - (double)nSize_2;
			} 
			else 
			{
				dDx = (double)nX + 0.5 - (double)nSize_2;
				dDy = (double)nY + 0.5 - 0;
			}
			pDistance[(nSize_2 + nY) * nSize_2 + nX] = dDx * dDx + dDy * dDy;
		}
	}

	// Строим матрицу
	m_unMinValue = 1;
	m_unMaxValue = 0;
	
	int nX1 = 0, nY1 = 0;
	
	for ( int nIndex = 0; nIndex < m_nSize * nSize_2; ++nIndex ) 
	{
		double dDist = -1;

		for ( int nY = 0; nY < m_nSize; ++nY ) 
		{
			for ( int nX = 0; nX < nSize_2; ++nX ) 
			{
				if ( m_pMatrix[nY * m_nSize + nX] == 0 && pDistance[nY * nSize_2 + nX] > dDist ) 
				{
					nX1 = nX;
					nY1 = nY;
					dDist = pDistance[nY1 * nSize_2 + nX1];
				}
			}
		}

		// [0, 2 * m_nSize * nSize_2 - 1] --> [1, 255]
		unsigned char unValue = 1 + (254 * (2 * nIndex)) / (2 * m_nSize * nSize_2 - 1);
		m_pMatrix[nY1 * m_nSize + nX1] = unValue;

		unValue = 1 + (254 * (2 * nIndex + 1)) / (2 * m_nSize * nSize_2 - 1);
		if ( nY1 < nSize_2 ) 
		{
			m_pMatrix[(nY1 + nSize_2) * m_nSize + nX1 + nSize_2] = unValue;
		} 
		else 
		{
			m_pMatrix[(nY1 - nSize_2) * m_nSize + nX1 + nSize_2] = unValue;
		}
	}

	MemUtilsFree( pDistance );
}

int SScreen::Distance(int nX0, int nY0, int nX1, int nY1) 
{
	int nDx0 = abs( nX0 - nX1 );
	int nDx1 = m_nSize - nDx0;
	int nDx = nDx0 < nDx1 ? nDx0 : nDx1;
	int nDy0 = abs( nY0 - nY1 );
	int nDy1 = m_nSize - nDy0;
	int nDy = nDy0 < nDy1 ? nDy0 : nDy1;
	return nDx * nDx + nDy * nDy;
}

void SScreen::BuildSCDMatrix(int nRadius) 
{
	srand( 123 );

	SScreenPoint *pPoints = (SScreenPoint *)MemUtilsMallocArray( m_nSize * m_nSize, sizeof(SScreenPoint) );
	int nIndex = 0;
	for ( int nY = 0; nY < m_nSize; ++nY ) 
	{
		for ( int nX = 0; nX < m_nSize; ++nX ) 
		{
			pPoints[nIndex].nX = nX;
			pPoints[nIndex].nY = nY;
			++nIndex;
		}
	}

	for ( nIndex = 0; nIndex < m_nSize * m_nSize; ++nIndex ) 
	{
		int nTempIndex = nIndex + (int)( (double)( m_nSize * m_nSize - nIndex ) * (double)rand() / ( (double)RAND_MAX + 1.0) );
		int nTempX = pPoints[nIndex].nX;
		int nTempY = pPoints[nIndex].nY;
		pPoints[nIndex].nX = pPoints[nTempIndex].nX;
		pPoints[nIndex].nY = pPoints[nTempIndex].nY;
		pPoints[nTempIndex].nX = nTempX;
		pPoints[nTempIndex].nY = nTempY;
	}

	char *pTemplate = (char *)MemUtilsMallocArray( (nRadius + 1) * (nRadius + 1), sizeof(char) );
	for ( int nY = 0; nY <= nRadius; ++nY ) 
	{
		for ( int nX = 0; nX <= nRadius; ++nX ) 
		{
			pTemplate[nY * (nRadius + 1) + nX] = (nX * nY <= nRadius * nRadius) ? 1 : 0;
		}
	}

	char *pGrid = (char *)MemUtilsMallocArray( m_nSize * m_nSize, sizeof(char) );
	for ( int nY = 0; nY < m_nSize; ++nY ) 
	{
		for ( int nX = 0; nX < m_nSize; ++nX ) 
		{
			pGrid[nY * m_nSize + nX] = 0;
		}
	}

	int nDotsCount = 0;
	int nDotsSize = 32;
	SScreenPoint *pDots = (SScreenPoint *)MemUtilsMallocArray( nDotsSize, sizeof(SScreenPoint) );
	for ( nIndex = 0; nIndex < m_nSize * m_nSize; ++nIndex ) 
	{
		int nX = pPoints[nIndex].nX;
		int nY = pPoints[nIndex].nY;
		if ( !pGrid[nY * m_nSize + nX] ) 
		{
			if ( nDotsCount == nDotsSize ) 
			{
				nDotsSize *= 2;
				pDots = (SScreenPoint *)MemUtilsReallocArray( pDots, nDotsSize, sizeof(SScreenPoint) );
			}
			pDots[nDotsCount++] = pPoints[nIndex];
			for ( int nYY = 0; nYY <= nRadius; ++nYY ) 
			{
				int nY0 = (nY + nYY) % m_nSize;
				int nY1 = (nY - nYY + m_nSize) % m_nSize;
				for ( int nXX = 0; nXX <= nRadius; ++nXX ) 
				{
					if ( pTemplate[nYY * (nRadius + 1) + nXX] ) 
					{
						int nX0 = (nX + nXX) % m_nSize;
						int nX1 = (nX - nXX + m_nSize) % m_nSize;
						pGrid[nY0 * m_nSize + nX0] = 1;
						pGrid[nY0 * m_nSize + nX1] = 1;
						pGrid[nY1 * m_nSize + nX0] = 1;
						pGrid[nY1 * m_nSize + nX1] = 1;
					}
				}
			}
		}
	}

	MemUtilsFree( pTemplate );
	MemUtilsFree( pGrid );

	int *pRegion = (int *)MemUtilsMallocArray( m_nSize * m_nSize, sizeof(int) );
	int *pDist   = (int *)MemUtilsMallocArray( m_nSize * m_nSize, sizeof(int) );
	for ( int nY = 0; nY < m_nSize; ++nY ) 
	{
		for ( int nX = 0; nX < m_nSize; ++nX ) 
		{
			int nMinIndex = 0;
			int nMinDist = Distance(pDots[0].nX, pDots[0].nY, nX, nY);
			for ( nIndex = 1; nIndex < nDotsCount; ++nIndex ) 
			{
				int nDistance = Distance(pDots[nIndex].nX, pDots[nIndex].nY, nX, nY);
				if ( nDistance < nMinDist ) 
				{
					nMinIndex = nIndex;
					nMinDist = nDistance;
				}
			}
			pRegion[nY * m_nSize + nX] = nMinIndex;
			pDist[nY * m_nSize + nX] = nMinDist;
		}
	}

	for ( nIndex = 0; nIndex < nDotsCount; ++nIndex ) 
	{
		int nCount = 0;
		for ( int nY = 0; nY < m_nSize; ++nY ) 
		{
			for ( int nX = 0; nX < m_nSize; ++nX ) 
			{
				if ( pRegion[nY * m_nSize + nX] == nIndex ) 
				{
					pPoints[nCount].nX = nX;
					pPoints[nCount].nY = nY;
					pPoints[nCount].nDist = Distance(pDots[nIndex].nX, pDots[nIndex].nY, nX, nY);
					++nCount;
				}
			}
		}
		qsort( pPoints, nCount, sizeof(SScreenPoint), &CompareDistances );
		for ( int nJ = 0; nJ < nCount; ++nJ ) 
		{
			// [0 .. n - 1] --> [255 .. 1]
			m_pMatrix[pPoints[nJ].nY * m_nSize + pPoints[nJ].nX] = 255 - (254 * nJ) / (nCount - 1);
		}
	}

	MemUtilsFree( pPoints );
	MemUtilsFree( pRegion );
	MemUtilsFree( pDist );
	MemUtilsFree( pDots );
}

SScreen::SScreen(SScreen *pScreen) 
{
	m_nSize = pScreen->m_nSize;
	m_pMatrix = (unsigned char *)MemUtilsMallocArray( m_nSize * m_nSize, sizeof(unsigned char) );
	memcpy( m_pMatrix, pScreen->m_pMatrix, m_nSize * m_nSize * sizeof(unsigned char) );
	m_unMinValue = pScreen->m_unMinValue;
	m_unMaxValue = pScreen->m_unMaxValue;
}

SScreen::~SScreen() 
{
	MemUtilsFree( m_pMatrix );
}

int SScreen::GetGrayPixel(int nX, int nY, unsigned char unValue) 
{
	int nXX, nYY;

	if ( unValue < m_unMinValue ) 
	{
		return 0;
	}
	if ( unValue >= m_unMaxValue ) 
	{
		return 1;
	}
	
	if ( ( nXX = nX % m_nSize ) < 0 ) 
	{
		nXX = -nXX;
	}
	if ( ( nYY = nY % m_nSize ) < 0 ) 
	{
		nYY = -nYY;
	}
	return unValue < m_pMatrix[nYY * m_nSize + nXX] ? 0 : 1;
}

BOOL SScreen::IsStatic(unsigned char unValue) 
{
	return ( unValue < m_unMinValue || unValue >= m_unMaxValue );
}
