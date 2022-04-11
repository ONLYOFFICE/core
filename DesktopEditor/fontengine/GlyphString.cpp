/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "GlyphString.h"
#include <stdlib.h>

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef WIN32
#endif

TGlyphBitmap::TGlyphBitmap()
{
	nX			= 0;
	nY			= 0;
	nWidth		= 0;
	nHeight		= 0;
	bAA			= FALSE;
	pData		= NULL;
    bFreeData	= TRUE;
}

TGlyphBitmap::~TGlyphBitmap()
{
	if (bFreeData && pData != NULL)
	{
        free(pData);
		pData = NULL;
	}
}

TGlyph::TGlyph()
{
}

TGlyph::~TGlyph()
{
}

////////////////////////////////
CGlyphString::CGlyphString()
{
	m_fX = 0;
	m_fY = 0;

	m_fEndX = 0;
	m_fEndY = 0;

	m_nGlyphIndex   = -1;
	m_nGlyphsCount  = 0;
	m_pGlyphsBuffer = NULL;

	m_arrCTM[0] = 1;
	m_arrCTM[1] = 0;
	m_arrCTM[2] = 0;
	m_arrCTM[3] = 1;
	m_arrCTM[4] = 0;
	m_arrCTM[5] = 0;

	m_fTransX = 0;
	m_fTransY = 0;

	m_dIDet	= 1.0;
}

CGlyphString::CGlyphString(const std::wstring& wsString, float fX, float fY)
{
	const wchar_t* pWchars = wsString.c_str();
	m_nGlyphIndex   = 0;

	m_nGlyphsCount  = wsString.length();

	if ( m_nGlyphsCount > 0 )
	{
		m_pGlyphsBuffer = new TGlyph[m_nGlyphsCount];

		if (sizeof(wchar_t) == 2)
		{
			int nEmpty = 0;
			for (int nIndex = 0, nGlyphIndex = 0; nIndex < m_nGlyphsCount; ++nIndex, ++nGlyphIndex)
			{
                unsigned int code = (unsigned int)pWchars[nIndex];
				if (code >= 0xD800 && code <= 0xDFFF && (nIndex + 1) < m_nGlyphsCount)
				{
					++nIndex;
					++nEmpty;
					code = 0x10000 + (((code & 0x3FF) << 10) | (0x03FF & pWchars[nIndex]));
				}

				m_pGlyphsBuffer[nGlyphIndex].lUnicode = code;
				m_pGlyphsBuffer[nGlyphIndex].bBitmap  = false;
			}
			m_nGlyphsCount -= nEmpty;
		}
		else
		{
			for ( int nIndex = 0; nIndex < m_nGlyphsCount; ++nIndex )
			{
                m_pGlyphsBuffer[nIndex].lUnicode = (unsigned int)pWchars[nIndex];
				m_pGlyphsBuffer[nIndex].bBitmap  = false;
			}
		}
	}
	else
	{
		m_pGlyphsBuffer = NULL;
	}

	m_fX = fX;
	m_fY = fY;

	m_arrCTM[0] = 1;
	m_arrCTM[1] = 0;
	m_arrCTM[2] = 0;
	m_arrCTM[3] = 1;
	m_arrCTM[4] = 0;
	m_arrCTM[5] = 0;

	m_fTransX = 0;
	m_fTransY = 0;
}

CGlyphString::~CGlyphString()
{
	if ( m_pGlyphsBuffer )
		delete []m_pGlyphsBuffer;
}

void CGlyphString::SetString(const unsigned int* pGids, const unsigned int& nGidsCount, float fX, float fY)
{
    m_fX = fX + m_fTransX;
    m_fY = fY + m_fTransY;

    if ( m_pGlyphsBuffer )
        delete []m_pGlyphsBuffer;

    m_nGlyphIndex   = 0;
    m_nGlyphsCount  = nGidsCount;

    if ( m_nGlyphsCount > 0 )
    {
        m_pGlyphsBuffer = new TGlyph[m_nGlyphsCount];

        for ( int nIndex = 0; nIndex < m_nGlyphsCount; ++nIndex )
        {
            m_pGlyphsBuffer[nIndex].lUnicode = pGids[nIndex];
            m_pGlyphsBuffer[nIndex].bBitmap  = false;
        }
    }
    else
    {
        m_pGlyphsBuffer = NULL;
    }
}

void CGlyphString::SetString(const std::wstring& wsString, float fX, float fY)
{
	m_fX = fX + m_fTransX;
	m_fY = fY + m_fTransY;

	if ( m_pGlyphsBuffer )
		delete []m_pGlyphsBuffer;

	const wchar_t* pWchars = wsString.c_str();
	m_nGlyphIndex   = 0;

	m_nGlyphsCount  = wsString.length();

	if ( m_nGlyphsCount > 0 )
	{
		m_pGlyphsBuffer = new TGlyph[m_nGlyphsCount];

		if (sizeof(wchar_t) == 2)
		{
			int nEmpty = 0;
			for (int nIndex = 0, nGlyphIndex = 0; nIndex < m_nGlyphsCount; ++nIndex, ++nGlyphIndex)				 
			{
				int code = (int)pWchars[nIndex];
				if (code >= 0xD800 && code <= 0xDFFF && (nIndex + 1) < m_nGlyphsCount)
				{
					++nIndex;
					++nEmpty;
					code = 0x10000 + (((code & 0x3FF) << 10) | (0x03FF & pWchars[nIndex]));
				}

				m_pGlyphsBuffer[nGlyphIndex].lUnicode = code;
				m_pGlyphsBuffer[nGlyphIndex].bBitmap  = false;
			}
			m_nGlyphsCount -= nEmpty;
		}
		else
		{
			for ( int nIndex = 0; nIndex < m_nGlyphsCount; ++nIndex )
			{
                m_pGlyphsBuffer[nIndex].lUnicode = (unsigned int)pWchars[nIndex];
				m_pGlyphsBuffer[nIndex].bBitmap  = false;
			}
		}
	}
	else
	{
		m_pGlyphsBuffer = NULL;
	}
}

void CGlyphString::SetStringGID(const unsigned int& gid, float fX, float fY)
{
	m_fX = fX + m_fTransX;
	m_fY = fY + m_fTransY;

	m_nGlyphsCount = 1;
	m_nGlyphIndex  = 0;

	if (NULL == m_pGlyphsBuffer)
		m_pGlyphsBuffer = new TGlyph[1];

	// TODO:
	// m_pGlyphsBuffer[0].Clear();

	m_pGlyphsBuffer[0].lUnicode = gid;
	m_pGlyphsBuffer[0].bBitmap  = false;
}

void CGlyphString::Reset()
{
	if ( m_pGlyphsBuffer )
		delete []m_pGlyphsBuffer;

	m_fX = 0;
	m_fY = 0;

	m_fEndX = 0;
	m_fEndY = 0;

	m_nGlyphIndex   = -1;
	m_nGlyphsCount  = 0;
	m_pGlyphsBuffer = NULL;

	m_arrCTM[0] = 1;
	m_arrCTM[1] = 0;
	m_arrCTM[2] = 0;
	m_arrCTM[3] = 1;
	m_arrCTM[4] = 0;
	m_arrCTM[5] = 0;

	m_fTransX = 0;
	m_fTransY = 0;
}

int CGlyphString::GetLength()
{
	return m_nGlyphsCount;
}

void CGlyphString::SetBBox(int nIndex, float fLeft, float fTop, float fRight, float fBottom)
{
	if ( m_nGlyphsCount <= 0 )
		return;

	int nCurIndex = min( m_nGlyphsCount - 1, max( 0, nIndex ) );

	m_pGlyphsBuffer[nCurIndex].fLeft   = fLeft;
	m_pGlyphsBuffer[nCurIndex].fTop    = fTop;
	m_pGlyphsBuffer[nCurIndex].fRight  = fRight;
	m_pGlyphsBuffer[nCurIndex].fBottom = fBottom;
}
void CGlyphString::SetMetrics(int nIndex, float fWidth, float fHeight, float fHoriAdvance, float fHoriBearingX, float fHoriBearingY, float fVertAdvance, float fVertBearingX, float fVertBearingY)
{
	if ( m_nGlyphsCount <= 0 )
		return;

	int nCurIndex = min( m_nGlyphsCount - 1, max( 0, nIndex ) );

	m_pGlyphsBuffer[nCurIndex].oMetrics.fHeight       = fHeight;
	m_pGlyphsBuffer[nCurIndex].oMetrics.fHoriAdvance  = fHoriAdvance;
	m_pGlyphsBuffer[nCurIndex].oMetrics.fHoriBearingX = fHoriBearingX;
	m_pGlyphsBuffer[nCurIndex].oMetrics.fHoriBearingY = fHoriBearingY;
	m_pGlyphsBuffer[nCurIndex].oMetrics.fVertAdvance  = fVertAdvance;
	m_pGlyphsBuffer[nCurIndex].oMetrics.fVertBearingX = fVertBearingX;
	m_pGlyphsBuffer[nCurIndex].oMetrics.fVertBearingY = fVertBearingY;
	m_pGlyphsBuffer[nCurIndex].oMetrics.fWidth        = fWidth;
}
void CGlyphString::SetStartPoint(int nIndex, float fX, float fY)
{
	if ( m_nGlyphsCount <= 0 )
		return;

	int nCurIndex = min( m_nGlyphsCount - 1, max( 0, nIndex ) );

	m_pGlyphsBuffer[nCurIndex].fX = fX;
	m_pGlyphsBuffer[nCurIndex].fY = fY;
}
void CGlyphString::SetState(int nIndex, EGlyphState eState)
{
	if ( m_nGlyphsCount <= 0 )
		return;

	int nCurIndex = min( m_nGlyphsCount - 1, max( 0, nIndex ) );

	m_pGlyphsBuffer[nCurIndex].eState = eState;
}

void CGlyphString::GetBBox(float *pfLeft, float *pfTop, float *pfRight, float *pfBottom, int nIndex, int nType)
{
	int nCurIndex = 0;
	if ( nIndex < 0 )
	{
		if ( m_nGlyphsCount <= 0 || m_nGlyphIndex < 1 || m_nGlyphIndex > m_nGlyphsCount )
			return;

		nCurIndex = m_nGlyphIndex - 1;
	}
	else
	{
		if ( m_nGlyphsCount <= 0 )
			return;

		nCurIndex = min( m_nGlyphsCount - 1, max( 0, nIndex ) );
	}

	float fBottom = -m_pGlyphsBuffer[nCurIndex].fBottom;
	float fRight  =  m_pGlyphsBuffer[nCurIndex].fRight;
	float fLeft   =  m_pGlyphsBuffer[nCurIndex].fLeft;
	float fTop    = -m_pGlyphsBuffer[nCurIndex].fTop;


	if ( 0 == nType && !( 1 == m_arrCTM[0] && 0 == m_arrCTM[1] && 0 == m_arrCTM[2] && 1 == m_arrCTM[3] && 0 == m_arrCTM[4] && 0 == m_arrCTM[5] ) )
	{
		// Применяем глобальную матрицу преобразования и пересчитываем BBox
		float arrfX[4] = { fLeft, fLeft,   fRight,  fRight };
		float arrfY[4] = { fTop,  fBottom, fBottom, fTop   };

		float fMinX = (float)(arrfX[0] * m_arrCTM[0] + arrfY[0] * m_arrCTM[2]);
		float fMinY = (float)(arrfX[0] * m_arrCTM[1] + arrfY[0] * m_arrCTM[3]);
		float fMaxX = fMinX;
		float fMaxY = fMinY;
		
		for ( int nIndex = 1; nIndex < 4; nIndex++ )
		{
			float fX = (float)(arrfX[nIndex] * m_arrCTM[0] + arrfY[nIndex] * m_arrCTM[2]);
			float fY = (float)(arrfX[nIndex] * m_arrCTM[1] + arrfY[nIndex] * m_arrCTM[3]);

			fMaxX = max( fMaxX, fX );
			fMinX = min( fMinX, fX );

			fMaxY = max( fMaxY, fY );
			fMinY = min( fMinY, fY );
		}

		fLeft   = fMinX;
		fRight  = fMaxX;
		fTop    = fMinY;
		fBottom = fMaxY;
	}

	*pfLeft   = fLeft   + m_pGlyphsBuffer[nCurIndex].fX + m_fX;
	*pfRight  = fRight  + m_pGlyphsBuffer[nCurIndex].fX + m_fX;
	*pfTop    = fTop    + m_pGlyphsBuffer[nCurIndex].fY + m_fY;
	*pfBottom = fBottom + m_pGlyphsBuffer[nCurIndex].fY + m_fY; 
}

void CGlyphString::GetBBox2(float *pfLeft, float *pfTop, float *pfRight, float *pfBottom)
{
	if ( m_nGlyphsCount <= 0 )
	{
		*pfLeft   = 0;
		*pfRight  = 0;
		*pfBottom = 0;
		*pfTop    = 0;
	}

	float fBottom = 0;
	float fRight  = 0;
	float fLeft   = 0;
	float fTop    = 0;

	for ( int nIndex = 0; nIndex < m_nGlyphsCount; nIndex++ )
	{
		fBottom = max( fBottom, -m_pGlyphsBuffer[nIndex].fBottom );
		//fRight  = max( fRight, m_pGlyphsBuffer[nIndex].fRight );
		//fLeft   = min( fLeft, m_pGlyphsBuffer[nIndex].fLeft );
		fTop    = min( fTop, -m_pGlyphsBuffer[nIndex].fTop );
	}

	if ( !( 1 == m_arrCTM[0] && 0 == m_arrCTM[1] && 0 == m_arrCTM[2] && 1 == m_arrCTM[3] && 0 == m_arrCTM[4] && 0 == m_arrCTM[5] ) )
	{
		// Применяем глобальную матрицу преобразования и пересчитываем BBox
		float arrfX[4] = { fLeft, fLeft,   fRight,  fRight };
		float arrfY[4] = { fTop,  fBottom, fBottom, fTop   };

		float fMinX = (float)(arrfX[0] * m_arrCTM[0] + arrfY[0] * m_arrCTM[2]);
		float fMinY = (float)(arrfX[0] * m_arrCTM[1] + arrfY[0] * m_arrCTM[3]);
		float fMaxX = fMinX;
		float fMaxY = fMinY;
		
		for ( int nIndex = 1; nIndex < 4; nIndex++ )
		{
			float fX = (float)(arrfX[nIndex] * m_arrCTM[0] + arrfY[nIndex] * m_arrCTM[2]);
			float fY = (float)(arrfX[nIndex] * m_arrCTM[1] + arrfY[nIndex] * m_arrCTM[3]);

			fMaxX = max( fMaxX, fX );
			fMinX = min( fMinX, fX );

			fMaxY = max( fMaxY, fY );
			fMinY = min( fMinY, fY );
		}

		fLeft   = fMinX;
		fRight  = fMaxX;
		fTop    = fMinY;
		fBottom = fMaxY;
	}

	fLeft   += m_fX;
	fRight  += m_fX;
	fTop    += m_fY;
	fBottom += m_fY; 

	*pfLeft  = min( fLeft, min(m_fX, m_fEndX) );
	*pfRight = max( fRight, max(m_fX, m_fEndX) );
	*pfTop   = min( fTop, min(m_fY, m_fEndY) );
	*pfBottom = max( fBottom, max(m_fY, m_fEndY) );
}

void CGlyphString::SetCTM(float fA, float fB, float fC, float fD, float fE ,float fF)
{
	m_arrCTM[0] = fA;
	m_arrCTM[1] = fB;
	m_arrCTM[2] = fC;
	m_arrCTM[3] = fD;
	m_arrCTM[4] = fE;
	m_arrCTM[5] = fF;

	double dDet      = fA * fD - fB * fC;

	if ( dDet < 0.001 && dDet >= 0 )
		dDet =  0.001;
	else if ( dDet > - 0.001 && dDet < 0 )
		dDet = -0.001;

	m_dIDet = 1 / dDet;

}
void CGlyphString::ResetCTM()
{
	m_arrCTM[0] = 1;
	m_arrCTM[1] = 0;
	m_arrCTM[2] = 0;
	m_arrCTM[3] = 1;
	m_arrCTM[4] = 0;
	m_arrCTM[5] = 0;

	m_dIDet      = 1;
}

void CGlyphString::Transform(float *pfX, float *pfY)
{
	float fX = *pfX, fY = *pfY;

	*pfX = (float) ( fX * m_arrCTM[0] + fY * m_arrCTM[2] + m_arrCTM[4] );
	*pfY = (float) ( fX * m_arrCTM[1] + fY * m_arrCTM[3] + m_arrCTM[5] );
}
void CGlyphString::SetTrans(float fX, float fY)
{
	m_fTransX = (float) ( m_dIDet * ( fX * m_arrCTM[3] - m_arrCTM[2] * fY ) );
	m_fTransY = (float) ( m_dIDet * ( fY * m_arrCTM[0] - m_arrCTM[1] * fX ) );
}
TGlyph* CGlyphString::GetAt(int nIndex)
{
	if ( m_nGlyphsCount <= 0 )
	{
		return NULL;
	}

	int nCurIndex = min( m_nGlyphsCount - 1, max( 0, nIndex ) );

	return &(m_pGlyphsBuffer[nCurIndex]);
}

INT CGlyphString::GetNext(TGlyph*& pGlyph)
{
	if ( m_nGlyphIndex >= m_nGlyphsCount || m_nGlyphIndex < 0 )
	{
		pGlyph = NULL;
		return FALSE;
	}

	pGlyph = &m_pGlyphsBuffer[m_nGlyphIndex];
	m_nGlyphIndex++;

	return TRUE;
}
