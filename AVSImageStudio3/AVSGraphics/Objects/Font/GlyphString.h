#ifndef _GLYPH_STRING_H
#define _GLYPH_STRING_H

#include "FontConsts.h"

class CGlyphString
{
public:

	CGlyphString()
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
	}

	CGlyphString(CStringW wsString, float fX = 0, float fY = 0)
	{
		m_nGlyphIndex   = 0;
		m_nGlyphsCount  = wsString.GetLength();

		if ( m_nGlyphsCount > 0 )
			m_pGlyphsBuffer = new TGlyph[m_nGlyphsCount];
		else
			m_pGlyphsBuffer = NULL;

		for ( int nIndex = 0; nIndex < m_nGlyphsCount; nIndex++ )
		{
			//memset( (void *)(m_pGlyphsBuffer + nIndex), 0x00, sizeof( TGlyph ) );
			m_pGlyphsBuffer[nIndex].lUnicode = wsString.GetAt( nIndex );
			m_pGlyphsBuffer[nIndex].bBitmap  = false;
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

	~CGlyphString()
	{
		if ( m_pGlyphsBuffer )
			delete []m_pGlyphsBuffer;
	}


	void SetString(CStringW wsString, float fX = 0, float fY = 0)
	{
		m_fX = fX + m_fTransX;
		m_fY = fY + m_fTransY;

		if ( m_pGlyphsBuffer )
			delete []m_pGlyphsBuffer;

		m_nGlyphsCount = wsString.GetLength();
		m_nGlyphIndex  = 0;

		m_pGlyphsBuffer = new TGlyph[m_nGlyphsCount];
		for ( int nIndex = 0; nIndex < m_nGlyphsCount; nIndex++ )
		{
			//memset( (void *)(m_pGlyphsBuffer + nIndex), 0x00, sizeof( TGlyph ) );
			m_pGlyphsBuffer[nIndex].lUnicode = wsString.GetAt( nIndex );
			m_pGlyphsBuffer[nIndex].bBitmap  = false;
		}
	}

	void Reset()
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

	int  GetLength()
	{
		return m_nGlyphsCount;
	}
	void SetBBox(int nIndex, float fLeft, float fTop, float fRight, float fBottom)
	{
		if ( m_nGlyphsCount <= 0 )
			return;

		int nCurIndex = min( m_nGlyphsCount - 1, max( 0, nIndex ) );

		m_pGlyphsBuffer[nCurIndex].fLeft   = fLeft;
		m_pGlyphsBuffer[nCurIndex].fTop    = fTop;
		m_pGlyphsBuffer[nCurIndex].fRight  = fRight;
		m_pGlyphsBuffer[nCurIndex].fBottom = fBottom;
	}
	void SetMetrics(int nIndex, float fWidth, float fHeight, float fHoriAdvance, float fHoriBearingX, float fHoriBearingY, float fVertAdvance, float fVertBearingX, float fVertBearingY)
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
	void SetStartPoint(int nIndex, float fX, float fY)
	{
		if ( m_nGlyphsCount <= 0 )
			return;

		int nCurIndex = min( m_nGlyphsCount - 1, max( 0, nIndex ) );

		m_pGlyphsBuffer[nCurIndex].fX = fX;
		m_pGlyphsBuffer[nCurIndex].fY = fY;
	}
	void SetState(int nIndex, EGlyphState eState)
	{
		if ( m_nGlyphsCount <= 0 )
			return;

		int nCurIndex = min( m_nGlyphsCount - 1, max( 0, nIndex ) );

		m_pGlyphsBuffer[nCurIndex].eState = eState;
	}

	void GetBBox(float *pfLeft, float *pfTop, float *pfRight, float *pfBottom, int nIndex = -1, int nType = 0)
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

	void GetBBox2(float *pfLeft, float *pfTop, float *pfRight, float *pfBottom)
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

	void SetCTM(float fA, float fB, float fC, float fD, float fE ,float fF)
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
	void ResetCTM()
	{
		m_arrCTM[0] = 1;
		m_arrCTM[1] = 0;
		m_arrCTM[2] = 0;
		m_arrCTM[3] = 1;
		m_arrCTM[4] = 0;
		m_arrCTM[5] = 0;

		m_dIDet      = 1;
	}

	void Transform(float *pfX, float *pfY)
	{
		float fX = *pfX, fY = *pfY;

		*pfX = (float) ( fX * m_arrCTM[0] + fY * m_arrCTM[2] + m_arrCTM[4] );
		*pfY = (float) ( fX * m_arrCTM[1] + fY * m_arrCTM[3] + m_arrCTM[5] );
	}
	void SetTrans(float fX, float fY)
	{
		m_fTransX = (float) ( m_dIDet * ( fX * m_arrCTM[3] - m_arrCTM[2] * fY ) );
		m_fTransY = (float) ( m_dIDet * ( fY * m_arrCTM[0] - m_arrCTM[1] * fX ) );
	}
	TGlyph *GetAt(int nIndex)
	{
		if ( m_nGlyphsCount <= 0 )
		{
			return NULL;
		}

		int nCurIndex = min( m_nGlyphsCount - 1, max( 0, nIndex ) );

		return &(m_pGlyphsBuffer[nCurIndex]);
	}

	BOOL GetNext(TGlyph *pGlyph)
	{
		if ( m_nGlyphIndex >= m_nGlyphsCount || m_nGlyphIndex < 0 )
		{
			pGlyph = NULL;
			return FALSE;
		}

		*pGlyph = m_pGlyphsBuffer[m_nGlyphIndex];
		m_nGlyphIndex++;

		return TRUE;
	}

public:

	float   m_fTransX;
	float   m_fTransY;

	float   m_fX; // Координаты начальной точки для рисования
	float   m_fY; //

	float   m_fEndX; // Координаты конечной точки
	float   m_fEndY; //

	double  m_arrCTM[6];     // Глобальная матрица преобразования
	double  m_dIDet;         // (Детерминант матрицы преобразования)^(-1)
	
private:

	TGlyph *m_pGlyphsBuffer; // Символы в данной строке
	int     m_nGlyphsCount;  // Количество символов в строке
	int     m_nGlyphIndex;   // Номер текущего символа

};

#endif /* _GLYPH_STRING_H */