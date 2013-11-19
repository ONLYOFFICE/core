#include "stdafx.h"

#include <ft2build.h>
#include FT_OUTLINE_H
#include FT_SIZES_H
#include FT_GLYPH_H
#include "MemoryUtils.h"
#include "SMathExt.h"
#include "SGlyphBitmap.h"
#include "SPath.h"
#include "SGlyphLibFontEngine.h"
#include "SGlyphLibFontFile.h"
#include "SGlyphLibFont.h"

//-------------------------------------------------------------------------------------------------------------------------------
struct SGlyphLibFontPath 
{
	SPath *pPath;
	double dTextScale;
	BOOL   bNeedClose;
};

static int GlyphPathMoveTo(const FT_Vector *pPoint, void *pPath) 
{
	SGlyphLibFontPath *pGlyphPath = (SGlyphLibFontPath *)pPath;

	if ( pGlyphPath->bNeedClose ) 
	{
		pGlyphPath->pPath->Close();
		pGlyphPath->bNeedClose = FALSE;
	}
	pGlyphPath->pPath->MoveTo( (double)pPoint->x * pGlyphPath->dTextScale / 64.0, (double)pPoint->y * pGlyphPath->dTextScale / 64.0 );
	return 0;
}

static int GlyphPathLineTo(const FT_Vector *pPoint, void *pPath) 
{
	SGlyphLibFontPath *pGlyphPath = (SGlyphLibFontPath *)pPath;

	pGlyphPath->pPath->LineTo( (double)pPoint->x * pGlyphPath->dTextScale / 64.0, (double)pPoint->y * pGlyphPath->dTextScale / 64.0 );
	pGlyphPath->bNeedClose = TRUE;
	return 0;
}

static int GlyphPathConicTo(const FT_Vector *pControlPoint, const FT_Vector *pEndPoint, void *pPath) 
{
	SGlyphLibFontPath *pGlyphPath = (SGlyphLibFontPath *)pPath;

	double dX0, dY0;

	if ( !pGlyphPath->pPath->GetCurPoint( &dX0, &dY0 ) ) 
	{
		return 0;
	}
	
	double dXc = (double)pControlPoint->x * pGlyphPath->dTextScale / 64.0;
	double dYc = (double)pControlPoint->y * pGlyphPath->dTextScale / 64.0;
	double dX3 = (double)pEndPoint->x * pGlyphPath->dTextScale / 64.0;
	double dY3 = (double)pEndPoint->y * pGlyphPath->dTextScale / 64.0;

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
	SGlyphLibFontPath *pGlyphPath = (SGlyphLibFontPath *)pPath;

	double dX1 = (double)pFirstControlPoint->x * pGlyphPath->dTextScale / 64.0;
	double dY1 = (double)pFirstControlPoint->y * pGlyphPath->dTextScale / 64.0;
	double dX2 = (double)pSecondControlPoint->x * pGlyphPath->dTextScale / 64.0;
	double dY2 = (double)pSecondControlPoint->y * pGlyphPath->dTextScale / 64.0;
	double dX3 = (double)pEndPoint->x * pGlyphPath->dTextScale / 64.0;
	double dY3 = (double)pEndPoint->y * pGlyphPath->dTextScale / 64.0;
	
	pGlyphPath->pPath->CurveTo( dX1, dY1, dX2, dY2, dX3, dY3 );
	pGlyphPath->bNeedClose = TRUE;
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------
// SGlyphLibFont
//-------------------------------------------------------------------------------------------------------------------------------

SGlyphLibFont::SGlyphLibFont(SGlyphLibFontFile *pFontFile, double *pFontMatrix, double *pTextMatrix):
SFont(pFontFile, pFontMatrix, pTextMatrix, pFontFile->m_pEngine->m_bAntiAliasing)
{
	FT_Face pFace = pFontFile->m_oFace;
	if ( FT_New_Size( pFace, &m_oSize ) ) 
	{
		return;
	}
	pFace->size = m_oSize;
	double dSize = sqrt( m_arrdFontMatrix[2] * m_arrdFontMatrix[2] + m_arrdFontMatrix[3] * m_arrdFontMatrix[3] );

	if ( FT_Set_Pixel_Sizes( pFace, 0, (int)dSize ) ) 
	{
		return;
	}
	
	m_dTextScale = sqrt( m_arrdTextMatrix[2] * m_arrdTextMatrix[2] + m_arrdTextMatrix[3] * m_arrdTextMatrix[3] ) / dSize;

	double dDiv = pFace->bbox.xMax > 20000 ? 65536 : 1;

	// Вычисляем BBox
	int nX = (int)((m_arrdFontMatrix[0] * pFace->bbox.xMin + m_arrdFontMatrix[2] * pFace->bbox.yMin) / (dDiv * pFace->units_per_EM));
	m_nMinX = m_nMaxX = nX;

	int nY = (int)((m_arrdFontMatrix[1] * pFace->bbox.xMin + m_arrdFontMatrix[3] * pFace->bbox.yMin) / (dDiv * pFace->units_per_EM));
	m_nMinY = m_nMaxY = nY;

	nX = (int)((m_arrdFontMatrix[0] * pFace->bbox.xMin + m_arrdFontMatrix[2] * pFace->bbox.yMax) / (dDiv * pFace->units_per_EM));
	
	if ( nX < m_nMinX ) 
	{
		m_nMinX = nX;
	} 
	else if ( nX > m_nMaxX ) 
	{
		m_nMaxX = nX;
	}

	nY = (int)((m_arrdFontMatrix[1] * pFace->bbox.xMin + m_arrdFontMatrix[3] * pFace->bbox.yMax) / (dDiv * pFace->units_per_EM));
  
	if ( nY < m_nMinY ) 
	{
		m_nMinY = nY;
	} 
	else if ( nY > m_nMaxY ) 
	{
		m_nMaxY = nY;
	}

	nX = (int)((m_arrdFontMatrix[0] * pFace->bbox.xMax + m_arrdFontMatrix[2] * pFace->bbox.yMin) / (dDiv * pFace->units_per_EM));
	if ( nX < m_nMinX ) 
	{
		m_nMinX = nX;
	} 
	else if ( nX > m_nMaxX ) 
	{
		m_nMaxX = nX;
	}

	nY = (int)((m_arrdFontMatrix[1] * pFace->bbox.xMax + m_arrdFontMatrix[3] * pFace->bbox.yMin) / (dDiv * pFace->units_per_EM));
	if ( nY < m_nMinY ) 
	{
		m_nMinY = nY;
	} 
	else if ( nY > m_nMaxY ) 
	{
		m_nMaxY = nY;
	}

	nX = (int)((m_arrdFontMatrix[0] * pFace->bbox.xMax + m_arrdFontMatrix[2] * pFace->bbox.yMax) / (dDiv * pFace->units_per_EM));
	if ( nX < m_nMinX ) 
	{
		m_nMinX = nX;
	} 
	else if ( nX > m_nMaxX ) 
	{
		m_nMaxX = nX;
	}

	nY = (int)((m_arrdFontMatrix[1] * pFace->bbox.xMax + m_arrdFontMatrix[3] * pFace->bbox.yMax) / (dDiv * pFace->units_per_EM));
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

	m_nMinX = max( -1000, min( 1000, m_nMinX ) );
	m_nMinY = max( -1000, min( 1000, m_nMinY ) );
	m_nMaxX = max( -1000, min( 1000, m_nMaxX ) );
	m_nMaxY = max( -1000, min( 1000, m_nMaxY ) );

	// Вычислим матрицу преобразования (FontMatrix)
	m_oFontMatrix.xx = (FT_Fixed)((m_arrdFontMatrix[0] / dSize) * 65536);
	m_oFontMatrix.yx = (FT_Fixed)((m_arrdFontMatrix[1] / dSize) * 65536);
	m_oFontMatrix.xy = (FT_Fixed)((m_arrdFontMatrix[2] / dSize) * 65536);
	m_oFontMatrix.yy = (FT_Fixed)((m_arrdFontMatrix[3] / dSize) * 65536);
	m_oTextMatrix.xx = (FT_Fixed)((m_arrdTextMatrix[0] / (dSize * m_dTextScale)) * 65536);
	m_oTextMatrix.yx = (FT_Fixed)((m_arrdTextMatrix[1] / (dSize * m_dTextScale)) * 65536);
	m_oTextMatrix.xy = (FT_Fixed)((m_arrdTextMatrix[2] / (dSize * m_dTextScale)) * 65536);
	m_oTextMatrix.yy = (FT_Fixed)((m_arrdTextMatrix[3] / (dSize * m_dTextScale)) * 65536);
}

SGlyphLibFont::~SGlyphLibFont() 
{
}

BOOL SGlyphLibFont::GetGlyph(int nCode, int nFracX, int nFracY, SGlyphBitmap *pBitmap) 
{
	return SFont::GetGlyph( nCode, nFracX, 0, pBitmap );
}

BOOL SGlyphLibFont::MakeGlyph(int nCode, int nFracX, int nFracY, SGlyphBitmap *pBitmap) 
{
	SGlyphLibFontFile *pFontFile = (SGlyphLibFontFile *)m_pFontFile;

	pFontFile->m_oFace->size = m_oSize;

	FT_Vector oOffset;
	oOffset.x = (FT_Pos)(int)( (double)nFracX * sfontFractionMul * 64 );
	oOffset.y = 0;

	FT_Set_Transform( pFontFile->m_oFace, &m_oFontMatrix, &oOffset );

	FT_GlyphSlot pGlyphSlot = pFontFile->m_oFace->glyph;

	FT_UInt unGID;
	if ( pFontFile->m_pCodeToGID && nCode < pFontFile->m_nCodeToGIDLen ) 
	{
		unGID = (FT_UInt)pFontFile->m_pCodeToGID[nCode];
	} 
	else 
	{
		unGID = (FT_UInt)nCode;
	}

	if ( pFontFile->m_bTrueType && unGID == 0 ) 
	{
		// Пропустим нулевой (".notdef") символ в TrueType
		return FALSE;
	}

	if ( FT_Load_Glyph( pFontFile->m_oFace, unGID, m_bAntiAliasing ? FT_LOAD_NO_HINTING | FT_LOAD_NO_BITMAP : FT_LOAD_DEFAULT ) ) 
	{
		return FALSE;
	}

	if ( FT_Render_Glyph( pGlyphSlot, m_bAntiAliasing ? ft_render_mode_normal : ft_render_mode_mono ) ) 
	{
		return FALSE;
	}

	pBitmap->nX      = -pGlyphSlot->bitmap_left;
	pBitmap->nY      =  pGlyphSlot->bitmap_top;
	pBitmap->nWidth  =  pGlyphSlot->bitmap.width;
	pBitmap->nHeight =  pGlyphSlot->bitmap.rows;
	pBitmap->bAA     = m_bAntiAliasing;

	int nRowSize = 0;
	if ( m_bAntiAliasing ) 
	{
		nRowSize = pBitmap->nWidth;
	} 
	else 
	{
		nRowSize = (pBitmap->nWidth + 7) >> 3;
	}

	pBitmap->pData = (unsigned char *)MemUtilsMalloc( nRowSize * pBitmap->nHeight );
	pBitmap->bFreeData = TRUE;

	int nIndex;
	unsigned char *pDstBuffer, *pSrcBuffer;
	for ( nIndex = 0, pDstBuffer = pBitmap->pData, pSrcBuffer = pGlyphSlot->bitmap.buffer; nIndex < pBitmap->nHeight; ++nIndex, pDstBuffer += nRowSize, pSrcBuffer += pGlyphSlot->bitmap.pitch ) 
	{
		memcpy( pDstBuffer, pSrcBuffer, nRowSize );
	}

	return TRUE;
}



SPath *SGlyphLibFont::GetGlyphPath(int nCode) 
{
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

	SGlyphLibFontFile *pFontFile = (SGlyphLibFontFile *)m_pFontFile;
	pFontFile->m_oFace->size = m_oSize;

	FT_Set_Transform( pFontFile->m_oFace, &m_oTextMatrix, NULL );
	FT_GlyphSlot oSlot = pFontFile->m_oFace->glyph;

	FT_UInt unGID;
	if ( pFontFile->m_pCodeToGID && nCode < pFontFile->m_nCodeToGIDLen ) 
	{
		unGID = pFontFile->m_pCodeToGID[nCode];
	} 
	else 
	{
		unGID = (FT_UInt)nCode;
	}

	if ( pFontFile->m_bTrueType && unGID == 0 ) 
	{
		// Пропустим нулевой (".notdef") символ в TrueType
		return NULL;
	}

	if ( FT_Load_Glyph( pFontFile->m_oFace, unGID, FT_LOAD_NO_BITMAP ) ) 
	{
		return NULL;
	}

	FT_Glyph oGlyph;
	if ( FT_Get_Glyph( oSlot, &oGlyph ) ) 
	{
		return NULL;
	}

	SGlyphLibFontPath oGlyphPath;
	oGlyphPath.pPath      = new SPath();
	oGlyphPath.dTextScale = m_dTextScale;
	oGlyphPath.bNeedClose = FALSE;

	FT_Outline_Decompose( &((FT_OutlineGlyph)oGlyph)->outline, &pOutlineFuncs, &oGlyphPath );

	if ( oGlyphPath.bNeedClose ) 
	{
		oGlyphPath.pPath->Close();
	}

	FT_Done_Glyph( oGlyph );
	return oGlyphPath.pPath;
}