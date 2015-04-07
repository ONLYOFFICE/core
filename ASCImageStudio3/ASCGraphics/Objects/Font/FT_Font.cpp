#include "stdafx.h"

#include <ft2build.h>
#include FT_OUTLINE_H
#include FT_SIZES_H
#include FT_GLYPH_H
#include FT_TRUETYPE_IDS_H
#include FT_TRUETYPE_TABLES_H
#include FT_XFREE86_H
#include FT_ADVANCES_H

#include <math.h>

#include "FT_FontEngine.h"
#include "FT_FontFile.h"
#include "FT_Font.h"

#include "FontPath.h"
#include "FontConsts.h"
#include "GlyphString.h"

#define LOAD_MODE FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT | FT_LOAD_NO_BITMAP | FT_LOAD_LINEAR_DESIGN
#define REND_MODE FT_RENDER_MODE_NORMAL

//-------------------------------------------------------------------------------------------------------------------------------
struct TFreeTypeFontPath 
{
	CFontPath *pPath;
	//double dTextScale;
	BOOL       bNeedClose;
};

static int GlyphPathMoveTo(const FT_Vector *pPoint, void *pPath) 
{
	TFreeTypeFontPath *pGlyphPath = (TFreeTypeFontPath *)pPath;

	if ( pGlyphPath->bNeedClose ) 
	{
		pGlyphPath->pPath->Close();
		pGlyphPath->bNeedClose = FALSE;
	}
	pGlyphPath->pPath->MoveTo( (double)pPoint->x /** pGlyphPath->dTextScale*/ / 64.0, (double)pPoint->y /** pGlyphPath->dTextScale*/ / 64.0 );
	return 0;
}

static int GlyphPathLineTo(const FT_Vector *pPoint, void *pPath) 
{
	TFreeTypeFontPath *pGlyphPath = (TFreeTypeFontPath *)pPath;

	pGlyphPath->pPath->LineTo( (double)pPoint->x /** pGlyphPath->dTextScale*/ / 64.0, (double)pPoint->y /** pGlyphPath->dTextScale*/ / 64.0 );
	pGlyphPath->bNeedClose = TRUE;
	return 0;
}

static int GlyphPathConicTo(const FT_Vector *pControlPoint, const FT_Vector *pEndPoint, void *pPath) 
{
	TFreeTypeFontPath *pGlyphPath = (TFreeTypeFontPath *)pPath;

	double dX0, dY0;

	if ( !pGlyphPath->pPath->GetCurPoint( &dX0, &dY0 ) ) 
	{
		return 0;
	}

	double dXc = (double)pControlPoint->x /** pGlyphPath->dTextScale*/ / 64.0;
	double dYc = (double)pControlPoint->y /** pGlyphPath->dTextScale*/ / 64.0;
	double dX3 = (double)pEndPoint->x /** pGlyphPath->dTextScale*/ / 64.0;
	double dY3 = (double)pEndPoint->y /** pGlyphPath->dTextScale*/ / 64.0;

	// Строим кривую Безье второго порядка, с помощью кривой Безье третего порядка. Если p0, pC, p3 -
	// начальная, контрольная и конечная точки, соответственно, для кривой Безье второго порядка. Тогда 
	// для этой же кривой, рассматриваемой как кривая Безье третьего порядка, точки p0, p1, p2, p3 будут
	// начальной, две контрольные, конечная точки. Где p1 и p2 рассчитываются по следующим формулам:
	//     p1 = (1/3) * (p0 + 2pС)
	//     p2 = (1/3) * (2pС + p3)

	double dX1 = (double)(1.0 / 3.0) * (dX0 + (double)2 * dXc);
	double dY1 = (double)(1.0 / 3.0) * (dY0 + (double)2 * dYc);
	double dX2 = (double)(1.0 / 3.0) * ((double)2 * dXc + dX3);
	double dY2 = (double)(1.0 / 3.0) * ((double)2 * dYc + dY3);

	pGlyphPath->pPath->CurveTo( dX1, dY1, dX2, dY2, dX3, dY3 );
	pGlyphPath->bNeedClose = TRUE;
	return 0;
}

static int GlyphPathCubicTo(const FT_Vector *pFirstControlPoint, const FT_Vector *pSecondControlPoint, const FT_Vector *pEndPoint, void *pPath) 
{
	TFreeTypeFontPath *pGlyphPath = (TFreeTypeFontPath *)pPath;

	double dX1 = (double)pFirstControlPoint->x /** pGlyphPath->dTextScale*/ / 64.0;
	double dY1 = (double)pFirstControlPoint->y /** pGlyphPath->dTextScale*/ / 64.0;
	double dX2 = (double)pSecondControlPoint->x /** pGlyphPath->dTextScale*/ / 64.0;
	double dY2 = (double)pSecondControlPoint->y /** pGlyphPath->dTextScale*/ / 64.0;
	double dX3 = (double)pEndPoint->x /** pGlyphPath->dTextScale*/ / 64.0;
	double dY3 = (double)pEndPoint->y /** pGlyphPath->dTextScale*/ / 64.0;

	pGlyphPath->pPath->CurveTo( dX1, dY1, dX2, dY2, dX3, dY3 );
	pGlyphPath->bNeedClose = TRUE;
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------
// CFreeTypeFont
//-------------------------------------------------------------------------------------------------------------------------------

CFreeTypeFont::CFreeTypeFont(CFreeTypeFontFile *pFontFile):
CFont(pFontFile, pFontFile->m_pEngine->m_bAntiAliasing, pFontFile->m_pEngine->m_bUseKerning)
{
	m_dUnitsKoef   = 1.0;
	m_nDefaultChar = -1;

	GetDefaultChar();
	m_nSymbolic = GetSymbolicCmapIndex();

	FT_Face pFace = pFontFile->m_pFace;
	if ( FT_New_Size( pFace, &m_pSize ) ) 
	{
		return;
	}
	pFace->size = m_pSize;

	// Выставляем размер шрифта (dSize) и DPI 
	if ( FT_Set_Char_Size( pFace, 0, (int)m_fSize * 64, 0, 0 ) ) 
	{
		return;
	}

	// В конструкторе CFont должно быть m_fSize = 1.0f, поэтому и m_dUnitsKoef = 1.0
	ResetFontMatrix();
	ResetTextMatrix();

	if ( TRUE == m_bUseKerning )
	{
		m_bUseKerning = ( FT_HAS_KERNING( pFace ) > 0 ? TRUE : FALSE );
	}

	InitSizesCache();
	::memset( m_arrCacheSizesIndexs, 0xFF, FONT_CACHE_SIZES_INDEXES_SIZE_2 );

	m_bStringGID = FALSE;
}

CFreeTypeFont::~CFreeTypeFont() 
{
	ClearSizesCache();
}

BOOL CFreeTypeFont::GetGlyph(int nCode, int nFracX, int nFracY, TGlyphBitmap *pBitmap) 
{
	return CFont::GetGlyph( nCode, nFracX, 0, pBitmap );
}

BOOL CFreeTypeFont::MakeGlyph(int nCode, int nFracX, int nFracY, TGlyphBitmap *pBitmap) 
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	FT_Int unGID = SetCMapForCharCode2( nCode );
	if ( unGID <= 0 )
		return FALSE;

	pFontFile->m_pFace->size = m_pSize;

	FT_Vector oOffset;
	oOffset.x = (FT_Pos)(int)( (double)nFracX * FONT_FRACTION_MULT * 64 );
	oOffset.y = 0;

	FT_GlyphSlot pGlyphSlot = pFontFile->m_pFace->glyph;

	// TO DO: Пропустить нулевой (".notdef") символ в TrueType

	if ( m_pDefaultFont )
		((CFreeTypeFont*)m_pDefaultFont)->UpdateMatrix2();
	UpdateMatrix2();

	if ( FT_Load_Glyph( pFontFile->m_pFace, unGID, LOAD_MODE ) ) 
	{
		return FALSE;
	}

	if ( FT_Render_Glyph( pGlyphSlot, REND_MODE /*m_bAntiAliasing ? ft_render_mode_normal : ft_render_mode_mono*/ ) ) 
	{
		return FALSE;
	}

	pBitmap->nX      = pGlyphSlot->bitmap_left;
	pBitmap->nY      = pGlyphSlot->bitmap_top;
	pBitmap->nWidth  = pGlyphSlot->bitmap.width;
	pBitmap->nHeight = pGlyphSlot->bitmap.rows;
	pBitmap->bAA     = m_bAntiAliasing;

	int nRowSize = 0;
	if ( m_bAntiAliasing ) 
	{
		if ( m_bNeedDoBold )
			pBitmap->nWidth++;

		nRowSize = pBitmap->nWidth;
	} 
	else 
	{
		nRowSize = (pBitmap->nWidth + 7) >> 3;
	}

	pBitmap->pData = (unsigned char *)malloc( nRowSize * pBitmap->nHeight );
	pBitmap->bFreeData = TRUE;

	int nIndex;
	unsigned char *pDstBuffer, *pSrcBuffer;

	if ( !m_bNeedDoBold || !m_bAntiAliasing )
	{
		for ( nIndex = 0, pDstBuffer = pBitmap->pData, pSrcBuffer = pGlyphSlot->bitmap.buffer; nIndex < pBitmap->nHeight; ++nIndex, pDstBuffer += nRowSize, pSrcBuffer += pGlyphSlot->bitmap.pitch ) 
		{
			memcpy( pDstBuffer, pSrcBuffer, nRowSize );
		}
	}
	else
	{
		int nY, nX;
		for ( nY = 0, pDstBuffer = pBitmap->pData, pSrcBuffer = pGlyphSlot->bitmap.buffer; nY < pBitmap->nHeight; ++nY, pDstBuffer += nRowSize, pSrcBuffer += pGlyphSlot->bitmap.pitch ) 
		{
			for ( nX = pBitmap->nWidth - 1; nX >= 0; nX-- )
			{
				if ( 0 != nX )
				{
					int nFirstByte, nSecondByte;

					if ( pBitmap->nWidth - 1 == nX )
						nFirstByte = 0;
					else
						nFirstByte = pSrcBuffer[nX];

					nSecondByte = pSrcBuffer[nX - 1];

					pDstBuffer[nX] = min( 255, nFirstByte + nSecondByte);

				}
				else
				{
					pDstBuffer[nX] = pSrcBuffer[nX];
				}
			}
		}

	}

	return TRUE;
}

BOOL CFreeTypeFont::GetString(CGlyphString *pString)
{
	if ( pString->GetLength() <= 0 )
		return TRUE;

	unsigned int unPrevGID = 0;
	float fPenX = 0, fPenY = 0;

	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)(m_pFontFile);
	FT_Face pSrcFace = pFontFile->m_pFace;

	FT_Face pDefFace = NULL;
	CFreeTypeFont *pDefFont = NULL;
	if ( m_pDefaultFont )
	{
		pDefFont = (CFreeTypeFont*)m_pDefaultFont;
		pDefFace = ((CFreeTypeFontFile *)(pDefFont->m_pFontFile))->m_pFace;
	}

	// Сначала мы все рассчитываем исходя только из матрицы шрифта FontMatrix
	if ( pDefFont )
		pDefFont->UpdateMatrix1();
	UpdateMatrix1();

	for ( int nIndex = 0; nIndex < pString->GetLength(); nIndex++ )
	{
		FT_Face pFace = pSrcFace;
		TGlyph *pCurGlyph = pString->GetAt( nIndex );

		unsigned short ushUnicode = (unsigned short)pCurGlyph->lUnicode;
		int nCacheIndex = FindInSizesCache( ushUnicode );
		unsigned int unGID = 0;

		if ( 0xFFFF == nCacheIndex )
		{
			int nCMapIndex = 0;
			unGID = (unsigned int)SetCMapForCharCode( ushUnicode, &nCMapIndex );
			TFontCacheSizes oSizes;
			oSizes.ushUnicode = ushUnicode;

			if ( !( ( unGID > 0 ) || ( -1 != m_nSymbolic && ( ushUnicode < 0xF000 )  && 0 < ( unGID = (unsigned int)SetCMapForCharCode( ushUnicode + 0xF000, &nCMapIndex ) ) ) ) )
			{
				// Пробуем загрузить через стандартный шрифт
				if ( FALSE == m_bUseDefaultFont || NULL == pDefFont || 0 >= ( unGID = pDefFont->SetCMapForCharCode( ushUnicode, &nCMapIndex ) )  )
				{
					if ( m_nDefaultChar < 0 )
					{
						oSizes.ushGID    = -1;
						oSizes.eState    = glyphstateMiss;
						oSizes.fAdvanceX = (float)(pSrcFace->size->metrics.max_advance >> 6) / 2.f;

						// В стандартном шрифте данный символ тоже не рисуется
						pString->SetStartPoint( nIndex, fPenX, fPenY );
						pString->SetBBox( nIndex, 0, 0, 0, 0 );
						pString->SetState( nIndex, glyphstateMiss );

						FT_Fixed lAdv = 0;

						fPenX += oSizes.fAdvanceX + m_fCharSpacing;
						unPrevGID = 0;

						continue;
					}
					else
					{
						unGID = m_nDefaultChar;
						oSizes.eState = glyphstateNormal;
						pString->SetState( nIndex, glyphstateNormal );
						pFace = pSrcFace;
					}
				}
				else
				{
					oSizes.eState = glyphstateDeafault;

					pString->SetState( nIndex, glyphstateDeafault );
					pFace = pDefFace;
				}
			}
			else
			{
				oSizes.eState = glyphstateNormal;

				pString->SetState( nIndex, glyphstateNormal );
				pFace = pSrcFace;
			}
			oSizes.ushGID     = unGID;
			oSizes.nCMapIndex = nCMapIndex;

			if ( m_bUseKerning && unPrevGID && ( nIndex >= 0 && pString->GetAt( nIndex )->eState == pString->GetAt( nIndex - 1 )->eState ) )
			{
				FT_Vector oDelta;
				FT_Get_Kerning( pFace, unPrevGID, unGID, FT_KERNING_DEFAULT, &oDelta );
				fPenX += (float)(oDelta.x >> 6);
			}

			float fX = pString->m_fX + fPenX;
			float fY = pString->m_fY + fPenY;

			// Начальную точку рассчитываем сразу исходя из глобальной матрицы 
			float fXX = (float)(pString->m_arrCTM[4] + fX * pString->m_arrCTM[0] + fY * pString->m_arrCTM[2] - pString->m_fX );
			float fYY = (float)(pString->m_arrCTM[5] + fX * pString->m_arrCTM[1] + fY * pString->m_arrCTM[3] - pString->m_fY );

			pString->SetStartPoint( nIndex, fXX, fYY);

			if ( FT_Load_Glyph( pFace, unGID, LOAD_MODE ) )
			{
				pString->SetStartPoint( nIndex, -0xFFFF, -0xFFFF );
				pString->SetState( nIndex, glyphstateMiss );
				continue;
			}

			FT_Glyph pGlyph = NULL;
			if ( FT_Get_Glyph( pFace->glyph, &pGlyph ) )
			{
				pString->SetStartPoint( nIndex, -0xFFFF, -0xFFFF );
				pString->SetState( nIndex, glyphstateMiss );
				continue;
			}

			FT_BBox oBBox;
			FT_Glyph_Get_CBox( pGlyph, ft_glyph_bbox_gridfit, &oBBox );
			FT_Done_Glyph( pGlyph );

			oSizes.fAdvanceX = (float)(pFace->glyph->linearHoriAdvance * m_dUnitsKoef / pFace->units_per_EM);
			oSizes.oBBox.fMinX = (float)(oBBox.xMin >> 6);
			oSizes.oBBox.fMaxX = (float)(oBBox.xMax >> 6);
			oSizes.oBBox.fMinY = (float)(oBBox.yMin >> 6);
			oSizes.oBBox.fMaxY = (float)(oBBox.yMax >> 6);

			oSizes.oMetrics.fHeight       = (float)(pFace->glyph->metrics.height       >> 6);
			oSizes.oMetrics.fHoriAdvance  = (float)(pFace->glyph->metrics.horiAdvance  >> 6);
			oSizes.oMetrics.fHoriBearingX = (float)(pFace->glyph->metrics.horiBearingX >> 6);
			oSizes.oMetrics.fHoriBearingY = (float)(pFace->glyph->metrics.horiBearingY >> 6);
			oSizes.oMetrics.fVertAdvance  = (float)(pFace->glyph->metrics.vertAdvance  >> 6);
			oSizes.oMetrics.fVertBearingX = (float)(pFace->glyph->metrics.vertBearingX >> 6);
			oSizes.oMetrics.fVertBearingY = (float)(pFace->glyph->metrics.vertBearingY >> 6);
			oSizes.oMetrics.fWidth        = (float)(pFace->glyph->metrics.width        >> 6);

			oSizes.bBitmap = false;
			oSizes.oBitmap.nX        = 0;
			oSizes.oBitmap.nY        = 0;
			oSizes.oBitmap.nHeight   = 0;
			oSizes.oBitmap.nWidth    = 0;
			oSizes.oBitmap.bFreeData = FALSE;
			oSizes.oBitmap.pData     = NULL;
			oSizes.oBitmap.bAA       = FALSE;

			pString->SetMetrics( nIndex, oSizes.oMetrics.fWidth, oSizes.oMetrics.fHeight, oSizes.oMetrics.fHoriAdvance, oSizes.oMetrics.fHoriBearingX, oSizes.oMetrics.fHoriBearingY, oSizes.oMetrics.fVertAdvance, oSizes.oMetrics.fVertBearingX, oSizes.oMetrics.fVertBearingY );
			pString->SetBBox( nIndex, oSizes.oBBox.fMinX, oSizes.oBBox.fMaxY, oSizes.oBBox.fMaxX, oSizes.oBBox.fMinY );
			//pString->SetBBox( nIndex, (float)(oBBox.xMin >> 6), (float)(oBBox.yMax >> 6), (float)(oBBox.xMax >> 6), (float)(oBBox.yMin >> 6) );

			fPenX    += oSizes.fAdvanceX + m_fCharSpacing;//(float)(pFace->glyph->advance.x >> 6);
			if ( m_bNeedDoBold )
			{
				// Когда текст делаем жирным сами, то мы увеличиваем расстояние на 1 пиксель в ширину (независимо от DPI и размера текста всегда 1 пиксель)
				fPenX += 1;
			}
			AddToSizesCache( oSizes );
		}
		else
		{
			TFontCacheSizes oSizes = m_oCacheSizes.Get(nCacheIndex);

			int nCMapIndex     = oSizes.nCMapIndex;
			unGID              = oSizes.ushGID;
			EGlyphState eState = oSizes.eState;

			if ( glyphstateMiss == eState )
			{
				pString->SetStartPoint( nIndex, fPenX, fPenY );
				pString->SetBBox( nIndex, 0, 0, 0, 0 );
				pString->SetState( nIndex, glyphstateMiss );

				FT_Fixed lAdv = 0;

				fPenX += oSizes.fAdvanceX + m_fCharSpacing;		
				unPrevGID = 0;

				continue;
			}
			else if ( glyphstateDeafault == eState )
			{
				pString->SetState( nIndex, glyphstateDeafault );
				pFace = pDefFace;
			}
			else // if ( glyphstateNormal == eState )
			{
				pString->SetState( nIndex, glyphstateNormal );
				pFace = pSrcFace;
			}

			if ( 0 != pFace->num_charmaps )
			{
				int nCurCMapIndex = FT_Get_Charmap_Index( pFace->charmap );
				if ( nCurCMapIndex != nCMapIndex )
				{
					nCMapIndex = max( 0, nCMapIndex );
					FT_Set_Charmap( pFace, pFace->charmaps[nCMapIndex] );
				}
			}

			if ( m_bUseKerning && unPrevGID && ( nIndex >= 0 && pString->GetAt( nIndex )->eState == pString->GetAt( nIndex - 1 )->eState ) )
			{
				FT_Vector oDelta;
				FT_Get_Kerning( pFace, unPrevGID, unGID, FT_KERNING_DEFAULT, &oDelta );
				fPenX += (float)(oDelta.x >> 6);
			}

			float fX = pString->m_fX + fPenX;
			float fY = pString->m_fY + fPenY;

			// Начальную точку рассчитываем сразу исходя из глобальной матрицы 
			float fXX = (float)(pString->m_arrCTM[4] + fX * pString->m_arrCTM[0] + fY * pString->m_arrCTM[2] - pString->m_fX );
			float fYY = (float)(pString->m_arrCTM[5] + fX * pString->m_arrCTM[1] + fY * pString->m_arrCTM[3] - pString->m_fY );

			pString->SetStartPoint( nIndex, fXX, fYY);

			pString->SetMetrics( nIndex, oSizes.oMetrics.fWidth, oSizes.oMetrics.fHeight, oSizes.oMetrics.fHoriAdvance, oSizes.oMetrics.fHoriBearingX, oSizes.oMetrics.fHoriBearingY, oSizes.oMetrics.fVertAdvance, oSizes.oMetrics.fVertBearingX, oSizes.oMetrics.fVertBearingY );
			pString->SetBBox( nIndex, oSizes.oBBox.fMinX, oSizes.oBBox.fMaxY, oSizes.oBBox.fMaxX, oSizes.oBBox.fMinY );
			fPenX += oSizes.fAdvanceX + m_fCharSpacing;

			if ( m_bNeedDoBold )
			{
				// Когда текст делаем жирным сами, то мы увеличиваем расстояние на 1 пиксель в ширину (независимо от DPI и размера текста всегда 1 пиксель)
				fPenX += 1;
			}

			oSizes.bBitmap = false;
			oSizes.oBitmap.nX        = 0;
			oSizes.oBitmap.nY        = 0;
			oSizes.oBitmap.nHeight   = 0;
			oSizes.oBitmap.nWidth    = 0;
			oSizes.oBitmap.bFreeData = FALSE;
			oSizes.oBitmap.pData     = NULL;
			oSizes.oBitmap.bAA       = FALSE;
			//pCurGlyph->bBitmap = m_oCacheSizes.Get(nCacheIndex).bBitmap;//m_arrCacheSizes[nCacheIndex].bBitmap;
			//pCurGlyph->oBitmap = m_oCacheSizes.Get(nCacheIndex).oBitmap;//m_arrCacheSizes[nCacheIndex].oBitmap;

		}
		unPrevGID = unGID;
	}

	pString->m_fEndX = fPenX + pString->m_fX;
	pString->m_fEndY = fPenY + pString->m_fY;

	if ( pDefFont )
		pDefFont->UpdateMatrix2();
	UpdateMatrix2();

	return TRUE;
}
BOOL CFreeTypeFont::GetString2(CGlyphString *pString)
{
	if ( pString->GetLength() <= 0 )
		return TRUE;

	unsigned int unPrevGID = 0;
	float fPenX = 0, fPenY = 0;

	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)(m_pFontFile);
	FT_Face pSrcFace = pFontFile->m_pFace;

	FT_Face pDefFace = NULL;
	CFreeTypeFont *pDefFont = NULL;
	if ( m_pDefaultFont )
	{
		pDefFont = (CFreeTypeFont*)m_pDefaultFont;
		pDefFace = ((CFreeTypeFontFile *)(pDefFont->m_pFontFile))->m_pFace;
	}

	for ( int nIndex = 0; nIndex < pString->GetLength(); nIndex++ )
	{
		// Сначала мы все рассчитываем исходя только из матрицы шрифта FontMatrix
		if ( pDefFont )
			pDefFont->UpdateMatrix1();
		UpdateMatrix1();

		FT_Face pFace = pSrcFace;
		TGlyph *pCurGlyph = pString->GetAt( nIndex );

		unsigned short ushUnicode = (unsigned short)pCurGlyph->lUnicode;
		int nCacheIndex = FindInSizesCache( ushUnicode );
		unsigned int unGID = 0;
		
		if ( 0xFFFF == nCacheIndex )
		{
			int nCMapIndex = 0;
			unGID = (unsigned int)SetCMapForCharCode( ushUnicode, &nCMapIndex );
			TFontCacheSizes oSizes;
			oSizes.ushUnicode = ushUnicode;

			if ( !( ( unGID > 0 ) || ( -1 != m_nSymbolic && ( ushUnicode < 0xF000 )  && 0 < ( unGID = (unsigned int)SetCMapForCharCode( ushUnicode + 0xF000, &nCMapIndex ) ) ) ) )
			{
				// Пробуем загрузить через стандартный шрифт
				if ( FALSE == m_bUseDefaultFont || NULL == pDefFont || 0 >= ( unGID = pDefFont->SetCMapForCharCode( ushUnicode, &nCMapIndex ) )  )
				{
					if ( m_nDefaultChar < 0 )
					{
						oSizes.ushGID    = -1;
						oSizes.eState    = glyphstateMiss;
						oSizes.fAdvanceX = (float)(pSrcFace->size->metrics.max_advance >> 6) / 2.f;

						// В стандартном шрифте данный символ тоже не рисуется
						pString->SetStartPoint( nIndex, fPenX, fPenY );
						pString->SetBBox( nIndex, 0, 0, 0, 0 );
						pString->SetState( nIndex, glyphstateMiss );

						FT_Fixed lAdv = 0;

						fPenX += oSizes.fAdvanceX + m_fCharSpacing;		
						continue;
					}
					else
					{
						unGID = m_nDefaultChar;
						oSizes.eState = glyphstateNormal;

						pString->SetState( nIndex, glyphstateNormal );
						pFace = pSrcFace;
					}
				}
				else
				{
					oSizes.eState = glyphstateDeafault;

					pString->SetState( nIndex, glyphstateDeafault );
					pFace = pDefFace;
				}
			}
			else
			{
				oSizes.eState = glyphstateNormal;

				pString->SetState( nIndex, glyphstateNormal );
				pFace = pSrcFace;
			}
			oSizes.ushGID     = unGID;
			oSizes.nCMapIndex = nCMapIndex;

			if ( m_bUseKerning && unPrevGID && ( nIndex >= 0 && pString->GetAt( nIndex )->eState == pString->GetAt( nIndex - 1 )->eState ) )
			{
				FT_Vector oDelta;
				FT_Get_Kerning( pFace, unPrevGID, unGID, FT_KERNING_DEFAULT, &oDelta );
				fPenX += (float)(oDelta.x >> 6);
			}

			float fX = pString->m_fX + fPenX;
			float fY = pString->m_fY + fPenY;

			// Начальную точку рассчитываем сразу исходя из глобальной матрицы 
			float fXX = (float)(pString->m_arrCTM[4] + fX * pString->m_arrCTM[0] + fY * pString->m_arrCTM[2] - pString->m_fX );
			float fYY = (float)(pString->m_arrCTM[5] + fX * pString->m_arrCTM[1] + fY * pString->m_arrCTM[3] - pString->m_fY );

			pString->SetStartPoint( nIndex, fXX, fYY);

			if ( pDefFont )
				pDefFont->UpdateMatrix2();
			UpdateMatrix2();

			if ( FT_Load_Glyph( pFace, unGID, LOAD_MODE ) )
			{
				pString->SetStartPoint( nIndex, -0xFFFF, -0xFFFF );
				pString->SetState( nIndex, glyphstateMiss );
				continue;
			}

			FT_Glyph pGlyph = NULL;
			if ( FT_Get_Glyph( pFace->glyph, &pGlyph ) )
			{
				pString->SetStartPoint( nIndex, -0xFFFF, -0xFFFF );
				pString->SetState( nIndex, glyphstateMiss );
				continue;
			}

			FT_BBox oBBox;
			FT_Glyph_Get_CBox( pGlyph, ft_glyph_bbox_gridfit, &oBBox );
			FT_Done_Glyph( pGlyph );

			float fAdvX = (float)(pFace->glyph->linearHoriAdvance * m_dUnitsKoef / pFace->units_per_EM );
			oSizes.fAdvanceX = fAdvX;
			oSizes.oBBox.fMinX = (float)(oBBox.xMin >> 6);
			oSizes.oBBox.fMaxX = (float)(oBBox.xMax >> 6);
			oSizes.oBBox.fMinY = (float)(oBBox.yMin >> 6);
			oSizes.oBBox.fMaxY = (float)(oBBox.yMax >> 6);

			oSizes.oMetrics.fHeight       = (float)(pFace->glyph->metrics.height       >> 6);
			oSizes.oMetrics.fHoriAdvance  = (float)(pFace->glyph->metrics.horiAdvance  >> 6);
			oSizes.oMetrics.fHoriBearingX = (float)(pFace->glyph->metrics.horiBearingX >> 6);
			oSizes.oMetrics.fHoriBearingY = (float)(pFace->glyph->metrics.horiBearingY >> 6);
			oSizes.oMetrics.fVertAdvance  = (float)(pFace->glyph->metrics.vertAdvance  >> 6);
			oSizes.oMetrics.fVertBearingX = (float)(pFace->glyph->metrics.vertBearingX >> 6);
			oSizes.oMetrics.fVertBearingY = (float)(pFace->glyph->metrics.vertBearingY >> 6);
			oSizes.oMetrics.fWidth        = (float)(pFace->glyph->metrics.width        >> 6);

			pString->SetMetrics( nIndex, oSizes.oMetrics.fWidth, oSizes.oMetrics.fHeight, oSizes.oMetrics.fHoriAdvance, oSizes.oMetrics.fHoriBearingX, oSizes.oMetrics.fHoriBearingY, oSizes.oMetrics.fVertAdvance, oSizes.oMetrics.fVertBearingX, oSizes.oMetrics.fVertBearingY );
			pString->SetBBox( nIndex, oSizes.oBBox.fMinX, oSizes.oBBox.fMaxY, oSizes.oBBox.fMaxX, oSizes.oBBox.fMinY );

			fPenX  += oSizes.fAdvanceX + m_fCharSpacing;
			if ( m_bNeedDoBold )
			{
				// Когда текст делаем жирным сами, то мы увеличиваем расстояние на 1 пиксель в ширину (независимо от DPI и размера текста всегда 1 пиксель)
				fPenX += 1;
			}

			pCurGlyph->bBitmap  = true;
			FT_GlyphSlot pGlyphSlot = pFace->glyph;
			if ( FT_Render_Glyph( pGlyphSlot, REND_MODE/*m_bAntiAliasing ? ft_render_mode_normal : ft_render_mode_mono*/ ) ) 
			{
				return FALSE;
			}

			TGlyphBitmap *pBitmap = &(pCurGlyph->oBitmap);
			pBitmap->nX      = pGlyphSlot->bitmap_left;
			pBitmap->nY      = pGlyphSlot->bitmap_top;
			pBitmap->nWidth  = pGlyphSlot->bitmap.width;
			pBitmap->nHeight = pGlyphSlot->bitmap.rows;
			pBitmap->bAA     = m_bAntiAliasing;

			int nRowSize = 0;
			if ( m_bAntiAliasing ) 
			{
				if ( m_bNeedDoBold )
					pBitmap->nWidth++;

				nRowSize = pBitmap->nWidth;
			} 
			else 
			{
				nRowSize = (pBitmap->nWidth + 7) >> 3;
			}

			if (0 != (nRowSize * pBitmap->nHeight))
				pBitmap->pData = (unsigned char *)malloc( nRowSize * pBitmap->nHeight );
			else
				pBitmap->pData = NULL;

			pBitmap->bFreeData = FALSE; // Все удаляется в кэше (во время очистки или замены)

			int nIndex2;
			unsigned char *pDstBuffer, *pSrcBuffer;

			if (NULL != pBitmap->pData)
			{
				//double dKoef = ( 255 + 10 ) / (double)255;
				if ( !m_bNeedDoBold || !m_bAntiAliasing )
				{
					for ( nIndex2 = 0, pDstBuffer = pBitmap->pData, pSrcBuffer = pGlyphSlot->bitmap.buffer; nIndex2 < pBitmap->nHeight; ++nIndex2, pDstBuffer += nRowSize, pSrcBuffer += pGlyphSlot->bitmap.pitch ) 
					{
						//for ( int nX = 0; nX < nRowSize; nX++ )
						//{
						//	pDstBuffer[nX] = min( 255, ( pSrcBuffer[nX]) * dKoef );
						//}
						memcpy( pDstBuffer, pSrcBuffer, nRowSize );
					}
				}
				else
				{
					int nY, nX;
					for ( nY = 0, pDstBuffer = pBitmap->pData, pSrcBuffer = pGlyphSlot->bitmap.buffer; nY < pBitmap->nHeight; ++nY, pDstBuffer += nRowSize, pSrcBuffer += pGlyphSlot->bitmap.pitch ) 
					{
						for ( nX = pBitmap->nWidth - 1; nX >= 0; nX-- )
						{
							if ( 0 != nX )
							{
								int nFirstByte, nSecondByte;

								if ( pBitmap->nWidth - 1 == nX )
									nFirstByte = 0;
								else
									nFirstByte = pSrcBuffer[nX];

								nSecondByte = pSrcBuffer[nX - 1];

								pDstBuffer[nX] = min( 255, nFirstByte + nSecondByte);

							}
							else
							{
								pDstBuffer[nX] = pSrcBuffer[nX];
							}
						}
					}

				}
			}

			oSizes.bBitmap			 = pCurGlyph->bBitmap;
			oSizes.oBitmap.bAA       = pBitmap->bAA;
			oSizes.oBitmap.bFreeData = pBitmap->bFreeData;
			oSizes.oBitmap.nX        = pBitmap->nX;
			oSizes.oBitmap.nY        = pBitmap->nY;
			oSizes.oBitmap.nWidth    = pBitmap->nWidth;
			oSizes.oBitmap.nHeight   = pBitmap->nHeight;
			oSizes.oBitmap.pData     = pBitmap->pData;

			AddToSizesCache( oSizes );
		}
		else
		{
			TFontCacheSizes oSizes = m_oCacheSizes.Get(nCacheIndex);

			int nCMapIndex     = oSizes.nCMapIndex;
			unGID              = oSizes.ushGID;
			EGlyphState eState = oSizes.eState;

			if ( glyphstateMiss == eState )
			{
				pString->SetStartPoint( nIndex, fPenX, fPenY );
				pString->SetBBox( nIndex, 0, 0, 0, 0 );
				pString->SetState( nIndex, glyphstateMiss );

				FT_Fixed lAdv = 0;

				fPenX += oSizes.fAdvanceX + m_fCharSpacing;		
				unPrevGID = 0;

				continue;
			}
			else if ( glyphstateDeafault == eState )
			{
				pString->SetState( nIndex, glyphstateDeafault );
				pFace = pDefFace;
			}
			else // if ( glyphstateNormal == eState )
			{
				pString->SetState( nIndex, glyphstateNormal );
				pFace = pSrcFace;
			}

			if ( 0 != pFace->num_charmaps )
			{
				int nCurCMapIndex = FT_Get_Charmap_Index( pFace->charmap );
				if ( nCurCMapIndex != nCMapIndex )
				{
					nCMapIndex = max( 0, nCMapIndex );
					FT_Set_Charmap( pFace, pFace->charmaps[nCMapIndex] );
				}
			}

			if ( m_bUseKerning && unPrevGID && ( nIndex >= 0 && pString->GetAt( nIndex )->eState == pString->GetAt( nIndex - 1 )->eState ) )
			{
				FT_Vector oDelta;
				FT_Get_Kerning( pFace, unPrevGID, unGID, FT_KERNING_DEFAULT, &oDelta );
				fPenX += (float)(oDelta.x >> 6);
			}

			float fX = pString->m_fX + fPenX;
			float fY = pString->m_fY + fPenY;

			// Начальную точку рассчитываем сразу исходя из глобальной матрицы 
			float fXX = (float)(pString->m_arrCTM[4] + fX * pString->m_arrCTM[0] + fY * pString->m_arrCTM[2] - pString->m_fX );
			float fYY = (float)(pString->m_arrCTM[5] + fX * pString->m_arrCTM[1] + fY * pString->m_arrCTM[3] - pString->m_fY );

			pString->SetStartPoint( nIndex, fXX, fYY);

			pString->SetMetrics( nIndex, oSizes.oMetrics.fWidth, oSizes.oMetrics.fHeight, oSizes.oMetrics.fHoriAdvance, oSizes.oMetrics.fHoriBearingX, oSizes.oMetrics.fHoriBearingY, oSizes.oMetrics.fVertAdvance, oSizes.oMetrics.fVertBearingX, oSizes.oMetrics.fVertBearingY );
			pString->SetBBox( nIndex, oSizes.oBBox.fMinX, oSizes.oBBox.fMaxY, oSizes.oBBox.fMaxX, oSizes.oBBox.fMinY );
			fPenX  += oSizes.fAdvanceX + m_fCharSpacing;

			if ( m_bNeedDoBold )
			{
				// Когда текст делаем жирным сами, то мы увеличиваем расстояние на 1 пиксель в ширину (независимо от DPI и размера текста всегда 1 пиксель)
				fPenX += 1;
			}

			pCurGlyph->bBitmap = oSizes.bBitmap;// m_oCacheSizes.Get(nCacheIndex).bBitmap;
			pCurGlyph->oBitmap = oSizes.oBitmap;// m_oCacheSizes.Get(nCacheIndex).oBitmap;
		}
		unPrevGID = unGID;
	}

	pString->m_fEndX = fPenX + pString->m_fX;
	pString->m_fEndY = fPenY + pString->m_fY;

	if ( pDefFont )
		pDefFont->UpdateMatrix2();
	UpdateMatrix2();


	return TRUE;
}
CFontPath *CFreeTypeFont::GetGlyphPath(int nCode) 
{ 
	FT_UInt unGID = SetCMapForCharCode2( nCode );
	if ( unGID <= 0 )
		return NULL;

	static FT_Outline_Funcs pOutlineFuncs = 
	{
#if FREETYPE_MINOR <= 1
		(int (*)(FT_Vector *, void *))&GlyphPathMoveTo,
		(int (*)(FT_Vector *, void *))&GlyphPathLineTo,
		(int (*)(FT_Vector *, FT_Vector *, void *))&GlyphPathConicTo,
		(int (*)(FT_Vector *, FT_Vector *, FT_Vector *, void *))&GlyphPathCubicTo,
#else
		&GlyphPathMoveTo,
		&GlyphPathLineTo,
		&GlyphPathConicTo,
		&GlyphPathCubicTo,
#endif
		0, 0
	};

	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;
	pFontFile->m_pFace->size = m_pSize;

	//FT_Set_Transform( pFontFile->m_pFace, &m_oTextMatrix, NULL );
	FT_GlyphSlot oSlot = pFontFile->m_pFace->glyph;

	// TO DO: Пропустить нулевой (".notdef") символ в TrueType

	if ( FT_Load_Glyph( pFontFile->m_pFace, unGID, FT_LOAD_NO_BITMAP ) ) 
	{
		return NULL;
	}

	FT_Glyph oGlyph;
	if ( FT_Get_Glyph( oSlot, &oGlyph ) ) 
	{
		return NULL;
	}

	TFreeTypeFontPath oGlyphPath;
	oGlyphPath.pPath      = new CFontPath();
	//oGlyphPath.dTextScale = m_dTextScale;
	oGlyphPath.bNeedClose = FALSE;

	FT_Outline_Decompose( &((FT_OutlineGlyph)oGlyph)->outline, &pOutlineFuncs, &oGlyphPath );

	if ( oGlyphPath.bNeedClose ) 
	{
		oGlyphPath.pPath->Close();
	}

	FT_Done_Glyph( oGlyph );
	return oGlyphPath.pPath;
}
short CFreeTypeFont::GetAscender()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	return pFontFile->m_pFace->ascender;
}

short CFreeTypeFont::GetDescender()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	return pFontFile->m_pFace->descender;
}

unsigned short CFreeTypeFont::GetUnitsPerEm()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	return pFontFile->m_pFace->units_per_EM;
}

short CFreeTypeFont::GetLineSpacing()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	return pFontFile->m_pFace->height;
}

char *CFreeTypeFont::GetFamilyName()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	return pFontFile->m_pFace->family_name;
}

long CFreeTypeFont::GetFacesCount()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	return pFontFile->m_pFace->num_faces;
}

long CFreeTypeFont::GetFaceIndex()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	return pFontFile->m_pFace->face_index;
}

long CFreeTypeFont::GetGlyphsCount()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	return pFontFile->m_pFace->num_glyphs;
}

char *CFreeTypeFont::GetStyleName()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	return pFontFile->m_pFace->style_name;
}

short CFreeTypeFont::GetUnderlinePosition()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	return pFontFile->m_pFace->underline_position;	
}
short CFreeTypeFont::GetUnderlineThickness()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	return pFontFile->m_pFace->underline_thickness;	
}
short CFreeTypeFont::GetMaxAdvanceWidth()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	return pFontFile->m_pFace->max_advance_width;
}
short CFreeTypeFont::GetMaxAdvanceHeight()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	return pFontFile->m_pFace->max_advance_height;
}
void CFreeTypeFont::GetBBox(long *plMinX, long *plMinY, long *plMaxX, long *plMaxY)
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	*plMinX = pFontFile->m_pFace->bbox.xMin;
	*plMinY = pFontFile->m_pFace->bbox.yMin;
	*plMaxX = pFontFile->m_pFace->bbox.xMax;
	*plMaxY = pFontFile->m_pFace->bbox.yMax;
}
int CFreeTypeFont::SetCMapForCharCode(long lUnicode, int *pnCMapIndex)
{
	*pnCMapIndex = -1;

	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;
	FT_Face pFace = pFontFile->m_pFace;

	if ( m_bStringGID || 0 == pFace->num_charmaps )
		return lUnicode;

	int nCharIndex = 0;

	if ( !pFace )
		return nCharIndex;

	for ( int nIndex = 0; nIndex < pFace->num_charmaps; nIndex++ )
	{
		FT_CharMap pCharMap = pFace->charmaps[nIndex];

		if ( FT_Set_Charmap( pFace, pCharMap ) )
			continue;

		FT_Encoding pEncoding = pCharMap->encoding;

		if ( FT_ENCODING_UNICODE == pEncoding )
		{
			if ( nCharIndex = FT_Get_Char_Index( pFace, lUnicode ) )
			{
				*pnCMapIndex = nIndex;
				return nCharIndex;
			}
		}
		else if ( FT_ENCODING_NONE == pEncoding || FT_ENCODING_MS_SYMBOL == pEncoding || FT_ENCODING_APPLE_ROMAN == pEncoding )
		{
			FT_ULong  charcode;
			FT_UInt   gindex;


			charcode = FT_Get_First_Char( pFace, &gindex );                   
			while ( gindex != 0 )                                            
			{                                                               
				charcode = FT_Get_Next_Char( pFace, charcode, &gindex );       
				if ( charcode == lUnicode )
				{
					nCharIndex = gindex;
					*pnCMapIndex = nIndex;
					break;
				}
			}                                                               

			if ( nCharIndex = FT_Get_Char_Index( pFace, lUnicode ) )
			{
				*pnCMapIndex = nIndex;
			}
		}

		//else if ( FT_ENCODING_ADOBE_STANDARD == pEncoding )
		//{
		//	if ( nCharIndex = FT_Get_Char_Index( pFace, lUnicode ) )
		//		return nCharIndex;

		//}
		//else if ( FT_ENCODING_ADOBE_CUSTOM == pEncoding )
		//{
		//	if ( nCharIndex = FT_Get_Char_Index( pFace, lUnicode ) )
		//		return nCharIndex;

		//}
		//else if ( FT_ENCODING_ADOBE_EXPERT == pEncoding )
		//{
		//	if ( nCharIndex = FT_Get_Char_Index( pFace, lUnicode ) )
		//		return nCharIndex;

		//}
	}

	return nCharIndex;
}

int CFreeTypeFont::SetCMapForCharCode2(long lUnicode)
{
	if ( m_bStringGID )
		return lUnicode;

	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	FT_Int unGID;
	int nCMapIndex = 0;
	int nCacheIndex = m_arrCacheSizesIndexs[(unsigned short)lUnicode];
	if ( 0xFFFF == nCacheIndex )
	{
		return unGID = SetCMapForCharCode( lUnicode, &nCMapIndex );
	}
	else
	{
		FT_Face pFace = pFontFile->m_pFace;
		TFontCacheSizes oSizes = m_oCacheSizes.Get(nCacheIndex);
		unGID = oSizes.ushGID;
		nCMapIndex = oSizes.nCMapIndex;
		if ( 0 != pFace->num_charmaps )
		{
			int nCurCMapIndex = FT_Get_Charmap_Index( pFace->charmap );
			if ( nCurCMapIndex != nCMapIndex )
			{
				nCMapIndex = max( 0, nCMapIndex );
				FT_Set_Charmap( pFace, pFace->charmaps[nCMapIndex] );
			}
		}
	}

	return unGID;
}
unsigned long CFreeTypeFont::GetCodeByGID(unsigned short unGID)
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;
	FT_Face pFace = pFontFile->m_pFace;

	if ( 0 == pFace->num_charmaps )
		return unGID;

	int nCharCode = 0;

	if ( !pFace )
		return nCharCode;

	for ( int nIndex = 0; nIndex < pFace->num_charmaps; nIndex++ )
	{
		FT_CharMap pCharMap = pFace->charmaps[nIndex];

		if ( FT_Set_Charmap( pFace, pCharMap ) )
			continue;

		FT_ULong unCharCode;
        FT_UInt  unCurGID;         
        
		unCharCode = FT_Get_First_Char( pFace, &unCurGID );

        while ( unCurGID != 0 )                                            
        {                                                                
			if ( unGID == unCurGID )
				return unCharCode;

			unCharCode = FT_Get_Next_Char( pFace, unCharCode, &unCurGID );        
        }                                                                
	}

	return 0;
}
void CFreeTypeFont::GetPanose(char **ppPanose)
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;
	FT_Face pFace = pFontFile->m_pFace;

	TT_OS2 *pTable = (TT_OS2 *)FT_Get_Sfnt_Table( pFace, ft_sfnt_os2 );

	::memset( *ppPanose, 0x00, 10 );
	if ( NULL == pTable )
		return;

	::memcpy( *ppPanose, pTable->panose, 10 );
}
bool CFreeTypeFont::IsFixedWidth()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	return FT_IS_FIXED_WIDTH( pFontFile->m_pFace ) != 0;
}
bool CFreeTypeFont::IsCharAvailable(long lUnicode)
{
	return ( SetCMapForCharCode2( lUnicode ) > 0 );
}
void CFreeTypeFont::SetSizeAndDpi(float fSize, unsigned int unHorDpi, unsigned int unVerDpi)
{
	if ( m_pDefaultFont )
	{
		m_pDefaultFont->SetSizeAndDpi( fSize, unHorDpi, unVerDpi );
	}

	CFont::ClearCache();
	ClearSizesCache();

	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;
	FT_Face pFace = pFontFile->m_pFace;

	float fOldSize = m_fSize;
	float fNewSize = fSize;
	float fKoef    = fNewSize / fOldSize;

	if ( fKoef > 1.001 || fKoef < 0.999 || unHorDpi != m_unHorDpi || unVerDpi != m_unVerDpi )
	{
		m_unHorDpi = unHorDpi;
		m_unVerDpi = unVerDpi;

		if ( fKoef > 1.001 || fKoef < 0.999 )
		{
			m_fSize    = fNewSize;
			UpdateMatrix0();
		}

		m_dUnitsKoef = m_unHorDpi / 72.0 * m_fSize;

		// Выставляем размер шрифта (dSize) и DPI 
		if ( FT_Set_Char_Size( pFace, 0, (int)(fNewSize * 64), unHorDpi, unVerDpi ) ) 
		{
			return;
		}
	}
}
const char *CFreeTypeFont::GetFontFormat()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;
	FT_Face pFace = pFontFile->m_pFace;

	return FT_Get_X11_Font_Format( pFace );
}

int CFreeTypeFont::IsUnicodeRangeAvailable(unsigned long ulBit, unsigned int un4ByteIndex)
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;
	FT_Face pFace = pFontFile->m_pFace;

	TT_OS2 *pOs2 = (TT_OS2 *)FT_Get_Sfnt_Table( pFace, ft_sfnt_os2 );
	if ( NULL == pOs2 || 0xFFFF == pOs2->version )
		return -1;

	int nResult = 0;

	unsigned long ulMult = 1;
	for ( unsigned long ulIndex = 0; ulIndex < ulBit; ulIndex++ )
		ulMult <<= 1;

	switch(un4ByteIndex)
	{
	case 0: if ( pOs2->ulUnicodeRange1  & ulMult ) nResult = 1; break;
	case 1: if ( pOs2->ulUnicodeRange2  & ulMult ) nResult = 1; break;
	case 2: if ( pOs2->ulUnicodeRange3  & ulMult ) nResult = 1; break;
	case 3: if ( pOs2->ulUnicodeRange4  & ulMult ) nResult = 1; break;
	case 4: if ( pOs2->ulCodePageRange1 & ulMult ) nResult = 1; break;
	case 5: if ( pOs2->ulCodePageRange2 & ulMult ) nResult = 1; break;
	}

	// Специальная ветка для случаев, когда charset может быть задан не через значения
	// ulCodePageRange, а непосредственно через тип Cmap.
	
	//  Charset Name       Charset Value(hex)  Codepage number   Platform_ID   Encoding_ID   Description
	//  -------------------------------------------------------------------------------------------------
	//
	//  SYMBOL_CHARSET            2 (x02)                             3            0           Symbol
	//  SHIFTJIS_CHARSET        128 (x80)             932             3            2           ShiftJIS
	//  GB2313_CHARSET          134 (x86)             936             3            3           PRC
	//  CHINESEBIG5_CHARSET     136 (x88)             950             3            4           Big5
	//  HANGEUL_CHARSET         129 (x81)             949             3            5           Wansung
	//  JOHAB_CHARSET	        130 (x82)            1361             3            6           Johab

	if ( 4 == un4ByteIndex && 0 == nResult )
	{
		for( int nIndex = 0; nIndex < pFace->num_charmaps; nIndex++ )
		{
			// Symbol
			if ( 31 == ulBit && 0 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
			{
				nResult = 1;
				break;
			}

			// ShiftJIS
			if ( 17 == ulBit && 2 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
			{
				nResult = 1;
				break;
			}

			// PRC
			if ( 18 == ulBit && 3 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
			{
				nResult = 1;
				break;
			}

			// Big5
			if ( 20 == ulBit && 4 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
			{
				nResult = 1;
				break;
			}

			// Wansung
			if ( 19 == ulBit && 5 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
			{
				nResult = 1;
				break;
			}

			// Johab
			if ( 21 == ulBit && 6 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
			{
				nResult = 1;
				break;
			}
		}
	}

	return nResult;
}
unsigned short CFreeTypeFont::GetNameIndex(char *sName)
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;
	FT_Face pFace = pFontFile->m_pFace;

	unsigned int unGID = FT_Get_Name_Index( pFace, sName );
	return unGID;
}
void CFreeTypeFont::ResetFontMatrix()
{
	if ( m_pDefaultFont )
	{
		m_pDefaultFont->ResetFontMatrix();
	}

	if ( m_bNeedDoItalic )
	{
		m_arrdFontMatrix[0] = 1;
		m_arrdFontMatrix[1] = 0;
		m_arrdFontMatrix[2] = FONT_ITALIC_ANGLE;
		m_arrdFontMatrix[3] = 1;
		m_arrdFontMatrix[4] = 0;
		m_arrdFontMatrix[5] = 0;
	}
	else
	{
		m_arrdFontMatrix[0] = 1;
		m_arrdFontMatrix[1] = 0;
		m_arrdFontMatrix[2] = 0;
		m_arrdFontMatrix[3] = 1;
		m_arrdFontMatrix[4] = 0;
		m_arrdFontMatrix[5] = 0;
	}

	UpdateMatrix0();

	//ClearSizesCache();
}
void CFreeTypeFont::ApplyTransform(float fA, float fB, float fC, float fD, float fE, float fF)
{
	if ( m_pDefaultFont )
	{
		m_pDefaultFont->ApplyTransform( fA, fB, fC, fD, fE, fF );
	}

	double arrTemp[6] = { m_arrdFontMatrix[0], m_arrdFontMatrix[1], m_arrdFontMatrix[2], m_arrdFontMatrix[3] };

	m_arrdFontMatrix[0] = arrTemp[0] * fA + arrTemp[1] * fC;
	m_arrdFontMatrix[1] = arrTemp[0] * fB + arrTemp[1] * fD;
	m_arrdFontMatrix[2] = arrTemp[2] * fA + arrTemp[3] * fC;
	m_arrdFontMatrix[3] = arrTemp[2] * fB + arrTemp[3] * fD;
	m_arrdFontMatrix[4] = arrTemp[4] * fA + arrTemp[5] * fC + fE;
	m_arrdFontMatrix[5] = arrTemp[4] * fB + arrTemp[5] * fD + fF;

	UpdateMatrix0();

	//ClearSizesCache();
}
void CFreeTypeFont::SetFontMatrix(float fA, float fB, float fC, float fD, float fE, float fF)
{
	if ( m_pDefaultFont )
	{
		m_pDefaultFont->SetFontMatrix( fA, fB, fC, fD, fE, fF );
	}

	if ( m_bNeedDoItalic ) 
	{
		m_arrdFontMatrix[0] = fA;
		m_arrdFontMatrix[1] = fB;
		m_arrdFontMatrix[2] = fC + fA * FONT_ITALIC_ANGLE;
		m_arrdFontMatrix[3] = fD + fB * FONT_ITALIC_ANGLE;
		m_arrdFontMatrix[4] = fE;
		m_arrdFontMatrix[5] = fF;
	}
	else
	{
		m_arrdFontMatrix[0] = fA;
		m_arrdFontMatrix[1] = fB;
		m_arrdFontMatrix[2] = fC;
		m_arrdFontMatrix[3] = fD;
		m_arrdFontMatrix[4] = fE;
		m_arrdFontMatrix[5] = fF;
	}

	ClearSizesCache();
}

void CFreeTypeFont::SetTextMatrix(float fA, float fB, float fC, float fD, float fE, float fF)
{
	if ( m_pDefaultFont )
	{
		m_pDefaultFont->SetTextMatrix( fA, fB, fC, fD, fE, fF );
	}

	m_arrdTextMatrix[0] =  fA;
	m_arrdTextMatrix[1] = -fB;
	m_arrdTextMatrix[2] = -fC;
	m_arrdTextMatrix[3] =  fD;
	m_arrdTextMatrix[4] =  fE;
	m_arrdTextMatrix[5] =  fF;

	ClearSizesCache();
}

void CFreeTypeFont::ResetTextMatrix()
{
	if ( m_pDefaultFont )
	{
		m_pDefaultFont->ResetTextMatrix();
	}

	m_arrdTextMatrix[0] =  1;
	m_arrdTextMatrix[1] = -0;
	m_arrdTextMatrix[2] = -0; // 
	m_arrdTextMatrix[3] =  1; // 
	m_arrdTextMatrix[4] =  0;
	m_arrdTextMatrix[5] =  0;

	//ClearSizesCache();
}
inline void CFreeTypeFont::UpdateMatrix0()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;
	FT_Face pFace = pFontFile->m_pFace;

	double dSize = m_fSize;

	m_dTextScale = sqrt( m_arrdTextMatrix[2] * m_arrdTextMatrix[2] + m_arrdTextMatrix[3] * m_arrdTextMatrix[3] );

	double dDiv = pFace->bbox.xMax > 20000 ? 65536 : 1;

	// Вычисляем BBox

	if ( pFace->units_per_EM == 0 )
		pFace->units_per_EM = 2048;

	int nX = (int)((m_arrdFontMatrix[0] * pFace->bbox.xMin + m_arrdFontMatrix[2] * pFace->bbox.yMin) * dSize / (dDiv * pFace->units_per_EM));
	m_nMinX = m_nMaxX = nX;

	int nY = (int)((m_arrdFontMatrix[1] * pFace->bbox.xMin + m_arrdFontMatrix[3] * pFace->bbox.yMin) * dSize / (dDiv * pFace->units_per_EM));
	m_nMinY = m_nMaxY = nY;

	nX = (int)((m_arrdFontMatrix[0] * pFace->bbox.xMin + m_arrdFontMatrix[2] * pFace->bbox.yMax) * dSize / (dDiv * pFace->units_per_EM));

	if ( nX < m_nMinX ) 
	{
		m_nMinX = nX;
	} 
	else if ( nX > m_nMaxX ) 
	{
		m_nMaxX = nX;
	}

	nY = (int)((m_arrdFontMatrix[1] * pFace->bbox.xMin + m_arrdFontMatrix[3] * pFace->bbox.yMax) * dSize / (dDiv * pFace->units_per_EM));

	if ( nY < m_nMinY ) 
	{
		m_nMinY = nY;
	} 
	else if ( nY > m_nMaxY ) 
	{
		m_nMaxY = nY;
	}

	nX = (int)((m_arrdFontMatrix[0] * pFace->bbox.xMax + m_arrdFontMatrix[2] * pFace->bbox.yMin) * dSize / (dDiv * pFace->units_per_EM));
	if ( nX < m_nMinX ) 
	{
		m_nMinX = nX;
	} 
	else if ( nX > m_nMaxX ) 
	{
		m_nMaxX = nX;
	}

	nY = (int)((m_arrdFontMatrix[1] * pFace->bbox.xMax + m_arrdFontMatrix[3] * pFace->bbox.yMin) * dSize / (dDiv * pFace->units_per_EM));
	if ( nY < m_nMinY ) 
	{
		m_nMinY = nY;
	} 
	else if ( nY > m_nMaxY ) 
	{
		m_nMaxY = nY;
	}

	nX = (int)((m_arrdFontMatrix[0] * pFace->bbox.xMax + m_arrdFontMatrix[2] * pFace->bbox.yMax) * dSize / (dDiv * pFace->units_per_EM));
	if ( nX < m_nMinX ) 
	{
		m_nMinX = nX;
	} 
	else if ( nX > m_nMaxX ) 
	{
		m_nMaxX = nX;
	}

	nY = (int)((m_arrdFontMatrix[1] * pFace->bbox.xMax + m_arrdFontMatrix[3] * pFace->bbox.yMax) * dSize / (dDiv * pFace->units_per_EM));
	if ( nY < m_nMinY ) 
	{
		m_nMinY = nY;
	} 
	else if ( nY > m_nMaxY ) 
	{
		m_nMaxY = nY;
	}

	// This is a kludge: some buggy PDF generators embed fonts with  zero bounding boxes.
	if ( m_nMaxX == m_nMinX ) 
	{
		m_nMinX = 0;
		m_nMaxX = (int)dSize;
	}

	if ( m_nMaxY == m_nMinY ) 
	{
		m_nMinY = 0;
		m_nMaxY = (int)((double)1.2 * dSize);
	}

	// Вычислим матрицу преобразования (FontMatrix)
	m_oFontMatrix.xx = (FT_Fixed)(m_arrdFontMatrix[0] * 65536);
	m_oFontMatrix.yx = (FT_Fixed)(m_arrdFontMatrix[1] * 65536);
	m_oFontMatrix.xy = (FT_Fixed)(m_arrdFontMatrix[2] * 65536);
	m_oFontMatrix.yy = (FT_Fixed)(m_arrdFontMatrix[3] * 65536);

	m_oTextMatrix.xx = (FT_Fixed)((m_arrdTextMatrix[0] / m_dTextScale) * 65536);
	m_oTextMatrix.yx = (FT_Fixed)((m_arrdTextMatrix[1] / m_dTextScale) * 65536);
	m_oTextMatrix.xy = (FT_Fixed)((m_arrdTextMatrix[2] / m_dTextScale) * 65536);
	m_oTextMatrix.yy = (FT_Fixed)((m_arrdTextMatrix[3] / m_dTextScale) * 65536);

	FT_Set_Transform( pFontFile->m_pFace, &m_oFontMatrix, NULL );
}

inline void CFreeTypeFont::UpdateMatrix1()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	m_oFontMatrix.xx = (FT_Fixed)(m_arrdFontMatrix[0] * 65536);
	m_oFontMatrix.yx = (FT_Fixed)(m_arrdFontMatrix[1] * 65536);
	m_oFontMatrix.xy = (FT_Fixed)(m_arrdFontMatrix[2] * 65536);
	m_oFontMatrix.yy = (FT_Fixed)(m_arrdFontMatrix[3] * 65536);

	FT_Set_Transform( pFontFile->m_pFace, &m_oFontMatrix, NULL );
}

inline void CFreeTypeFont::UpdateMatrix2()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;

	m_oFontMatrix.xx = (FT_Fixed)(( m_arrdFontMatrix[0] * m_arrdTextMatrix[0] + m_arrdFontMatrix[1] * m_arrdTextMatrix[2] ) * 65536);
	m_oFontMatrix.yx = (FT_Fixed)(( m_arrdFontMatrix[0] * m_arrdTextMatrix[1] + m_arrdFontMatrix[1] * m_arrdTextMatrix[3] ) * 65536);
	m_oFontMatrix.xy = (FT_Fixed)(( m_arrdFontMatrix[2] * m_arrdTextMatrix[0] + m_arrdFontMatrix[3] * m_arrdTextMatrix[2] ) * 65536);
	m_oFontMatrix.yy = (FT_Fixed)(( m_arrdFontMatrix[2] * m_arrdTextMatrix[1] + m_arrdFontMatrix[3] * m_arrdTextMatrix[3] ) * 65536);

	FT_Set_Transform( pFontFile->m_pFace, &m_oFontMatrix, NULL );
}

void CFreeTypeFont::InitSizesCache()
{
	m_oCacheSizes.Init();
}
void CFreeTypeFont::ClearSizesCache()
{
	m_oCacheSizes.Clear();
	::memset( m_arrCacheSizesIndexs, 0xFF, FONT_CACHE_SIZES_INDEXES_SIZE_2 );
}
void CFreeTypeFont::AddToSizesCache(TFontCacheSizes oSizes)
{
	m_arrCacheSizesIndexs[oSizes.ushUnicode] = m_oCacheSizes.Add( oSizes );
}
__forceinline int CFreeTypeFont::FindInSizesCache(int nCode)
{
	return m_arrCacheSizesIndexs[nCode];
}
void CFreeTypeFont::SetStringGID(BOOL bGID)
{
	if ( m_bStringGID == bGID )
		return;

	ClearSizesCache();
	m_bStringGID = bGID;
}
BOOL CFreeTypeFont::GetStringGID()
{
	return m_bStringGID;
}
int CFreeTypeFont::GetDefaultChar()
{
	FT_Face pFace = ((CFreeTypeFontFile *)m_pFontFile)->m_pFace;

	TT_OS2 *pTable = (TT_OS2 *)FT_Get_Sfnt_Table( pFace, ft_sfnt_os2 );

	if ( NULL == pTable )
	{
		return (m_nDefaultChar = -1);
	}

	return (m_nDefaultChar = pTable->usDefaultChar);
}
int CFreeTypeFont::GetSymbolicCmapIndex()
{
	CFreeTypeFontFile *pFontFile = (CFreeTypeFontFile *)m_pFontFile;
	FT_Face pFace = pFontFile->m_pFace;

	TT_OS2 *pOs2 = (TT_OS2 *)FT_Get_Sfnt_Table( pFace, ft_sfnt_os2 );
	if ( NULL == pOs2 || 0xFFFF == pOs2->version )
		return -1;

	// Проверяем установлен ли 31 бит
	if ( !( pOs2->ulCodePageRange1 & 0x80000000 ) && !( pOs2->ulCodePageRange1 == 0 && pOs2->ulCodePageRange2 == 0 ) )
		return -1;


	for( int nIndex = 0; nIndex < pFace->num_charmaps; nIndex++ )
	{
		// Symbol
		if ( 0 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
			return nIndex;
	}

	return -1;
}