#include "stdafx.h"

#include <string.h>
#include <math.h>
#include "File.h"
#include "GlobalParams.h"
#include "Error.h"
#include "Object.h"
#include "GFont.h"
#include "Link.h"
#include "CharCodeToUnicode.h"
#include "EncodingTables.h"
#include "FontFileTrueType.h"
#include "SBitmap.h"
#include "SGlyphBitmap.h"
#include "SPattern.h"
#include "SScreen.h"
#include "SPath.h"
#include "SState.h"
#include "SErrorCodes.h"
#include "SFontEngine.h"
#include "SFont.h"
#include "SFontFile.h"
#include "SFontFileID.h"
#include "SImage.h"
#include "ImageOutputDev.h"

//-------------------------------------------------------------------------------------------------------------------------------

// Делим 16-битное значение [0, 255*255] на 255, возвращаем 8-битное значение.
static inline unsigned char Div255(int nValue) 
{
	return (unsigned char)( ( nValue + ( nValue >> 8 ) + 0x80 ) >> 8 );
}

//-------------------------------------------------------------------------------------------------------------------------------
// Blend functions
//-------------------------------------------------------------------------------------------------------------------------------

static void RGBToHSV(unsigned char unR, unsigned char unG, unsigned char unB, int *pnH, int *pnS, int *pnV) 
{
	int nCmax, nCmid, nCmin, nValue;

	if ( unR >= unG ) 
	{
		if      ( unG >= unB ) { nValue = 0; nCmax = unR; nCmid = unG; nCmin = unB; }
		else if ( unB >= unR ) { nValue = 4; nCmax = unB; nCmid = unR; nCmin = unG; }
		else                   { nValue = 5; nCmax = unR; nCmid = unB; nCmin = unG; }
	} 
	else 
	{
		if      ( unR >= unB ) { nValue = 1; nCmax = unG; nCmid = unR; nCmin = unB; }
		else if ( unG >= unB ) { nValue = 2; nCmax = unG; nCmid = unB; nCmin = unR; }
		else                   { nValue = 3; nCmax = unB; nCmid = unG; nCmin = unR; }
	}
	if ( nCmax == nCmin ) 
	{
		*pnH = *pnS = 0;
	} 
	else 
	{
		*pnH = nValue * 60;
		if ( nValue & 1 ) 
		{
			*pnH += ( ( nCmax - nCmid ) * 60 ) / ( nCmax - nCmin );
		} 
		else 
		{
			*pnH += ( ( nCmid - nCmin ) * 60 ) / ( nCmax - nCmin );
		}
		*pnS = ( 255 * ( nCmax - nCmin ) ) / nCmax;
	}
	*pnV = nCmax;
}

static void HSVToRGB(int nH, int nS, int nV, unsigned char *punR, unsigned char *punG, unsigned char *punB) 
{
	if ( nS == 0 ) 
	{
		*punR = *punG = *punB = nV;
	} 
	else 
	{
		int nInt   = nH / 60;
		int nFloat = nH % 60;
		int nCmax = nV, nCmid;
		if ( nInt & 1 ) 
		{
			nCmid = Div255( nV * 255 - ( ( nS * nFloat ) / 60 ) );
		} 
		else 
		{
			nCmid = Div255( nV * ( 255 - ( ( nS * ( 60 - nFloat ) ) / 60 ) ) );
		}
		int nCmin = Div255( nV * ( 255 - nS ) );
		switch (nInt) 
		{
		case 0: *punR = nCmax; *punG = nCmid; *punB = nCmin; break;
		case 1: *punG = nCmax; *punR = nCmid; *punB = nCmin; break;
		case 2: *punG = nCmax; *punB = nCmid; *punR = nCmin; break;
		case 3: *punB = nCmax; *punG = nCmid; *punR = nCmin; break;
		case 4: *punB = nCmax; *punR = nCmid; *punG = nCmin; break;
		case 5: *punR = nCmax; *punB = nCmid; *punG = nCmin; break;
		}
	}
}


static void BlendMultiply  (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		pBlend[nIndex] = ( pDst[nIndex] * pSrc[nIndex] ) / 255;
	}
}

static void BlendScreen    (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		pBlend[nIndex] = pDst[nIndex] + pSrc[nIndex] - ( pDst[nIndex] * pSrc[nIndex] ) / 255;
	}
}

static void BlendOverlay   (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		pBlend[nIndex] = pDst[nIndex] < 0x80 ? ( pSrc[nIndex] * 2 * pDst[nIndex] ) / 255 : 255 - 2 * ( ( 255 - pSrc[nIndex] ) * ( 255 - pDst[nIndex] ) ) / 255;
	}
}

static void BlendDarken    (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		pBlend[nIndex] = pDst[nIndex] < pSrc[nIndex] ? pDst[nIndex] : pSrc[nIndex];
	}
}

static void BlendLighten   (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		pBlend[nIndex] = pDst[nIndex] > pSrc[nIndex] ? pDst[nIndex] : pSrc[nIndex];
	}
}

static void BlendColorDodge(SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		if ( pSrc[nIndex] == 255 ) 
		{
			pBlend[nIndex] = 255;
		} 
		else 
		{
			int nValue = ( pDst[nIndex] * 255 ) / ( 255 - pSrc[nIndex] );
			pBlend[nIndex] = nValue <= 255 ? nValue : 255;
		}
	}
}

static void BlendColorBurn (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		if ( pSrc[nIndex] == 0 ) 
		{
			pBlend[nIndex] = 0;
		} 
		else 
		{
			int nValue = ( ( 255 - pDst[nIndex] ) * 255 ) / pSrc[nIndex];
			pBlend[nIndex] = nValue <= 255 ? 255 - nValue : 0;
		}
	}
}

static void BlendHardLight (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		pBlend[nIndex] = pSrc[nIndex] < 0x80 ? ( pDst[nIndex] * 2 * pSrc[nIndex] ) / 255 : 255 - 2 * ( ( 255 - pDst[nIndex] ) * ( 255 - pSrc[nIndex] ) ) / 255;
	}
}

static void BlendSoftLight (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		if ( pSrc[nIndex] < 0x80 ) 
		{
			pBlend[nIndex] = pDst[nIndex] - (255 - 2 * pSrc[nIndex]) * pDst[nIndex] * (255 - pDst[nIndex]) / ( 255 * 255 );
		} 
		else 
		{
			int nValue;
			if ( pDst[nIndex] < 0x40 ) 
			{
				nValue = ( ( ( ( ( 16 * pDst[nIndex] - 12 * 255 ) * pDst[nIndex] ) / 255 ) + 4 * 255 ) * pDst[nIndex] ) / 255;
			} 
			else 
			{
				nValue = (int)sqrt( 255.0 * pDst[nIndex] );
			}
			pBlend[nIndex] = pDst[nIndex] + ( 2 * pSrc[nIndex] - 255 ) * ( nValue - pDst[nIndex] ) / 255;
		}
	}
}

static void BlendDifference(SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		pBlend[nIndex] = pDst[nIndex] < pSrc[nIndex] ? pSrc[nIndex] - pDst[nIndex] : pDst[nIndex] - pSrc[nIndex];
	}
}

static void BlendExclusion (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		pBlend[nIndex] = pDst[nIndex] + pSrc[nIndex] - ( 2 * pDst[nIndex] * pSrc[nIndex] ) / 255;
	}
}

static void BlendHue       (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	int nSrcH, nSrcS, nSrcV, nDstH, nDstS, nDstV;

	switch (eColorMode) 
	{
	case colorModeMono1:
	case colorModeMono8:
		pBlend[0] = pDst[0];
		break;
	case colorModeRGB8:
	case colorModeBGR8:
		RGBToHSV( pSrc[0], pSrc[1], pSrc[2], &nSrcH, &nSrcS, &nSrcV );
		RGBToHSV( pDst[0], pDst[1], pDst[2], &nDstH, &nDstS, &nDstV );
		HSVToRGB( nSrcH, nDstS, nDstV, &pBlend[0], &pBlend[1], &pBlend[2] );
		break;
	}
}

static void BlendSaturation(SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	int nSrcH, nSrcS, nSrcV, nDstH, nDstS, nDstV;

	switch (eColorMode) 
	{
	case colorModeMono1:
	case colorModeMono8:
		pBlend[0] = pDst[0];
		break;
	case colorModeRGB8:
	case colorModeBGR8:
		RGBToHSV( pSrc[0], pSrc[1], pSrc[2], &nSrcH, &nSrcS, &nSrcV );
		RGBToHSV( pDst[0], pDst[1], pDst[2], &nDstH, &nDstS, &nDstV );
		HSVToRGB( nDstH, nSrcS, nDstV, &pBlend[0], &pBlend[1], &pBlend[2] );
		break;
	}
}

static void BlendColor     (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	int nSrcH, nSrcS, nSrcV, nDstH, nDstS, nDstV;

	switch (eColorMode) 
	{
	case colorModeMono1:
	case colorModeMono8:
		pBlend[0] = pDst[0];
		break;
	case colorModeRGB8:
	case colorModeBGR8:
		RGBToHSV( pSrc[0], pSrc[1], pSrc[2], &nSrcH, &nSrcS, &nSrcV );
		RGBToHSV( pDst[0], pDst[1], pDst[2], &nDstH, &nDstS, &nDstV );
		HSVToRGB( nSrcH, nSrcS, nDstV, &pBlend[0], &pBlend[1], &pBlend[2] );
		break;
	}
}

static void BlendLuminosity(SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	int nSrcH, nSrcS, nSrcV, nDstH, nDstS, nDstV;

	switch (eColorMode) 
	{
	case colorModeMono1:
	case colorModeMono8:
		pBlend[0] = pDst[0];
		break;
	case colorModeRGB8:
	case colorModeBGR8:
		RGBToHSV( pSrc[0], pSrc[1], pSrc[2], &nSrcH, &nSrcS, &nSrcV );
		RGBToHSV( pDst[0], pDst[1], pDst[2], &nDstH, &nDstS, &nDstV );
		HSVToRGB( nDstH, nDstS, nSrcV, &pBlend[0], &pBlend[1], &pBlend[2] );
		break;
	}
}


// Должно совпадать с перечислением GrBlendMode из GState.h.
SBlendFunc c_arrBlendFunctions[] = 
{
	NULL,
	&BlendMultiply,
	&BlendScreen,
	&BlendOverlay,
	&BlendDarken,
	&BlendLighten,
	&BlendColorDodge,
	&BlendColorBurn,
	&BlendHardLight,
	&BlendSoftLight,
	&BlendDifference,
	&BlendExclusion,
	&BlendHue,
	&BlendSaturation,
	&BlendColor,
	&BlendLuminosity
};

//-------------------------------------------------------------------------------------------------------------------------------
// Font substitutions
//-------------------------------------------------------------------------------------------------------------------------------

struct SImageOutFontSubst 
{
	char  *sName;
	double dAvrWidth;
};

// нумерация: { symbolic:12, fixed:8, serif:4, sans-serif:0 } + bold * 2 + italic
static SImageOutFontSubst c_arrSImageOutSubstFonts[16] = 
{
	{"Helvetica",             0.833},
	{"Helvetica-Oblique",     0.833},
	{"Helvetica-Bold",        0.889},
	{"Helvetica-BoldOblique", 0.889},
	{"Times-Roman",           0.788},
	{"Times-Italic",          0.722},
	{"Times-Bold",            0.833},
	{"Times-BoldItalic",      0.778},
	{"Courier",               0.600},
	{"Courier-Oblique",       0.600},
	{"Courier-Bold",          0.600},
	{"Courier-BoldOblique",   0.600},
	{"Symbol",                0.576},
	{"Symbol",                0.576},
	{"Symbol",                0.576},
	{"Symbol",                0.576}
};

//-------------------------------------------------------------------------------------------------------------------------------
// SImageOutFontFileID
//-------------------------------------------------------------------------------------------------------------------------------

class SImageOutFontFileID: public SFontFileID 
{
public:

	SImageOutFontFileID(Ref *oRef) 
	{ 
		m_oRef        = *oRef; 
		m_nSubstIndex = -1; 
	}

	~SImageOutFontFileID() 
	{
	}

	BOOL Matches(SFontFileID *pID) 
	{
		return ((SImageOutFontFileID *)pID)->m_oRef.nNum == m_oRef.nNum && ((SImageOutFontFileID *)pID)->m_oRef.nGen == m_oRef.nGen;
	}

	void SetSubstIndex(int nSubstIndex) 
	{ 
		m_nSubstIndex = nSubstIndex; 
	}
	int  GetSubstIndex() 
	{ 
		return m_nSubstIndex; 
	}

private:

	Ref m_oRef;
	int m_nSubstIndex;
};

//-------------------------------------------------------------------------------------------------------------------------------
// T3FontCache
//-------------------------------------------------------------------------------------------------------------------------------

struct T3FontCacheTag 
{
	unsigned short unCode;
	unsigned short unMRU;  // Valid bit (0x8000) и номер MRU
};

class T3FontCache 
{
public:

	T3FontCache(Ref *pFontID, double dMatrix11, double dMatrix12, double dMatrix21, double dMAtrix22, int nGlyphX, int nGlyphY, int nGlyphWidth, int nGlyphHeight, BOOL bValidBBox, BOOL bAA) 
	{
		m_oFontID = *pFontID;

		m_dMatrix11 = dMatrix11;
		m_dMatrix12 = dMatrix12;
		m_dMatrix21 = dMatrix21;
		m_dMatrix22 = dMAtrix22;

		m_nGlyphX = nGlyphX;
		m_nGlyphY = nGlyphY;
		m_nGlyphWidth  = nGlyphWidth;
		m_nGlyphHeight = nGlyphHeight;

		m_bValidBBox = bValidBBox;
		if ( bAA ) 
		{
			m_nGlyphSize = m_nGlyphWidth * m_nGlyphHeight;
		} 
		else 
		{
			m_nGlyphSize = (( m_nGlyphWidth + 7 ) >> 3) * m_nGlyphHeight;
		}
		m_nCacheAssoc = 8;
		if ( m_nGlyphSize <= 256 ) 
		{
			m_nCacheSets = 8;
		} 
		else if ( m_nGlyphSize <= 512 ) 
		{
			m_nCacheSets = 4;
		} 
		else if ( m_nGlyphSize <= 1024 ) 
		{
			m_nCacheSets = 2;
		} 
		else 
		{
			m_nCacheSets = 1;
		}
		m_pCacheData = (unsigned char  *)MemUtilsMallocArray( m_nCacheSets * m_nCacheAssoc, m_nGlyphSize );
		m_pCacheTags = (T3FontCacheTag *)MemUtilsMallocArray( m_nCacheSets * m_nCacheAssoc, sizeof(T3FontCacheTag) );

		for ( int nIndex = 0; nIndex < m_nCacheSets * m_nCacheAssoc; ++nIndex ) 
		{
			m_pCacheTags[nIndex].unMRU = nIndex & (m_nCacheAssoc - 1);
		}
	}
	~T3FontCache() 
	{
		MemUtilsFree( m_pCacheData );
		MemUtilsFree( m_pCacheTags );
	}
	BOOL Matches(Ref *pFontID, double dMatrix11, double dMatrix12, double dMatrix21, double dMAtrix22)
	{ 
		return m_oFontID.nNum == pFontID->nNum && m_oFontID.nGen  == pFontID->nGen && m_dMatrix11 == dMatrix11 && m_dMatrix12 == dMatrix12 && m_dMatrix21 == dMatrix21 && m_dMatrix22 == dMAtrix22; 
	}

public:

	Ref    m_oFontID;      // ID данного шрифта (номера данного объекта в PDF-файле)
	double m_dMatrix11;    //
	double m_dMatrix12;    // Матрица преобразования
	double m_dMatrix21;    //
	double m_dMatrix22;    //
	int    m_nGlyphX;      // Сдвиг в пикселях Glyph Bitmap
	int    m_nGlyphY;      //
	int    m_nGlyphWidth;  // Ширина Glyph Bitmap в пикселях 
	int    m_nGlyphHeight; // Высота Glyph Bitmap в пикселях 
	BOOL   m_bValidBBox;   // False, только если BBox был [0 0 0 0]
	int    m_nGlyphSize;   // Размер Glyph bitmaps в байтах
	
	int    m_nCacheSets;   // Количество Sets кэше
	int    m_nCacheAssoc;  // Количестов символов в одном Set

	unsigned char  *m_pCacheData; // Glyph pixmap 
	T3FontCacheTag *m_pCacheTags; // Коды символов (тэги)
};

struct T3GlyphStack 
{
	unsigned short  unCode;          // Код символа

	// Информация о кэше
	T3FontCache    *pCache;          // FontCache для текущего шрифта
	T3FontCacheTag *pCacheTag;       // Указатель на структуру храняющую тэг данного символа
	unsigned char  *pCacheData;      // Укзатель на битмап для данного символа

	// Сохраненное состояние
	SBitmap        *pOriginalBitmap; // Искходный битмап
	SImage         *pOriginalSImage; // Исходный объект SImage
	double          dOriginalCTM4;   // 4-ый элемент в исходной матрице CTM
	double          dOriginalCTM5;   // 5-ый элемент в исходной матрице CTM

	T3GlyphStack   *pNext;           // Указатель на следующий элемент
};

//-------------------------------------------------------------------------------------------------------------------------------
// ImageData
//-------------------------------------------------------------------------------------------------------------------------------
struct ImageMaskData 
{
	ImageStream *pImageStream;
	BOOL         bInvert;
	int          nWidth;
	int          nHeight;
	int          nY;
};

struct MaskedImageData 
{
	ImageStream     *pImageStream;
	GrImageColorMap *pColorMap;
	SBitmap         *pMask;
	SColorPointer    pLookup;
	SColorMode       eColorMode;
	int              nWidth;
	int              nHeight;
	int              nY;
};

struct ImageData 
{
	ImageStream     *pStream;
	GrImageColorMap *pColorMap;
	SColorPointer    pLookup;
	int             *pMaskColors;
	SColorMode       eColorMode;
	int              nWidth;
	int              nHeight;
	int              nY;
};

//-------------------------------------------------------------------------------------------------------------------------------
// SImageTransparencyGroup
//-------------------------------------------------------------------------------------------------------------------------------

struct TransparencyGroup 
{
	int           nTransX;  // Координаты переноса
	int           nTransY;  // 

	SBitmap      *pBitmap;  // Битмап для transparency group
	GrColorSpace *pBlendColorSpace;
	BOOL          bIsolated;

	// Сохраненное состояние
	SBitmap      *pOriginalBitmap;  // Искходный битмап
	SImage       *pOriginalSImage;  // Исходный объект SImage

	TransparencyGroup *pNext; // Указатель на следующий элемент 
};

//-------------------------------------------------------------------------------------------------------------------------------
// SImageOutputDev
//-------------------------------------------------------------------------------------------------------------------------------

SImageOutputDev::SImageOutputDev(GlobalParams *pGlobalParams, SColorMode eColorMode, int nBitmapRowPad, BOOL bReverseVideo, SColorPointer pPaperColor, BOOL bBitmapTopDown, BOOL bAllowAA) 
{
	m_pGlobalParams = pGlobalParams;

	m_eColorMode = eColorMode;

	m_nBitmapRowPad  = nBitmapRowPad;
	m_bBitmapTopDown = bBitmapTopDown;

	m_bAllowAA  = bAllowAA;

	if ( m_pGlobalParams )
	{
		m_bVectorAA = m_bAllowAA && m_pGlobalParams->GetVectorAntialias() && m_eColorMode != colorModeMono1;
	}
	else
	{
		m_bVectorAA = m_bAllowAA && m_eColorMode != colorModeMono1;
	}

	SetupScreenParams( 72.0, 72.0 );

	m_bReverseVideo = bReverseVideo;
	ColorCopy( m_pPaperColor, pPaperColor );

	m_pXref = NULL;

	m_pBitmap = new SBitmap( 1, 1, m_nBitmapRowPad, m_eColorMode, m_eColorMode != colorModeMono1, m_bBitmapTopDown );
	m_pSImage = new SImage( m_pBitmap, m_bVectorAA, &m_pScreenParams );
	m_pSImage->Clear( m_pPaperColor, 0 );

	m_pFontEngine = NULL;

	m_nT3FontsCount = 0;
	m_pT3GlyphStack = NULL;

	m_pFont           = NULL;
	m_bNeedFontUpdate = FALSE;
	m_pTextClipPath   = NULL;

	m_pTranspGroupStack = NULL;
}

void SImageOutputDev::SetupScreenParams(double dHorDPI, double dVerDPI) 
{
	if ( m_pGlobalParams )
	{
		m_pScreenParams.nSize           = m_pGlobalParams->GetScreenSize();
		m_pScreenParams.nDotRadius      = m_pGlobalParams->GetScreenDotRadius();
		m_pScreenParams.dGamma          = (double)m_pGlobalParams->GetScreenGamma();
		m_pScreenParams.dBlackThreshold = (double)m_pGlobalParams->GetScreenBlackThreshold();
		m_pScreenParams.dWhiteThreshold = (double)m_pGlobalParams->GetScreenWhiteThreshold();

		switch ( m_pGlobalParams->GetScreenType() ) 
		{
		case screenDispersed:
			m_pScreenParams.eType = sscreenDispersed;
			if ( m_pScreenParams.nSize < 0 ) 
			{
				m_pScreenParams.nSize = 4;
			}
			break;
		case screenClustered:
			m_pScreenParams.eType = sscreenClustered;
			if ( m_pScreenParams.nSize < 0 ) 
			{
				m_pScreenParams.nSize = 10;
			}
			break;
		case screenStochasticClustered:
			m_pScreenParams.eType = sscreenStochasticClustered;
			if ( m_pScreenParams.nSize < 0 ) 
			{
				m_pScreenParams.nSize = 100;
			}
			if ( m_pScreenParams.nDotRadius < 0 ) 
			{
				m_pScreenParams.nDotRadius = 2;
			}
			break;
		case screenUnset:
		default:
			// Используем Clustered Dithering для разрешения >= 300 dpi
			if ( dHorDPI > 299.9 && dVerDPI > 299.9 ) 
			{
				m_pScreenParams.eType = sscreenStochasticClustered;
				if ( m_pScreenParams.nSize < 0 ) 
				{
					m_pScreenParams.nSize = 100;
				}
				if ( m_pScreenParams.nDotRadius < 0 ) 
				{
					m_pScreenParams.nDotRadius = 2;
				}
			} 
			else 
			{
				m_pScreenParams.eType = sscreenDispersed;
				if ( m_pScreenParams.nSize < 0 ) 
				{
					m_pScreenParams.nSize = 4;
				}
			}
		}
	}
	else
	{
		m_pScreenParams.nSize           = -1;
		m_pScreenParams.nDotRadius      = -1;
		m_pScreenParams.dGamma          = 1.0;
		m_pScreenParams.dBlackThreshold = 0.0;
		m_pScreenParams.dWhiteThreshold = 1.0;

		if ( dHorDPI > 299.9 && dVerDPI > 299.9 ) 
		{
			m_pScreenParams.eType = sscreenStochasticClustered;
			if ( m_pScreenParams.nSize < 0 ) 
			{
				m_pScreenParams.nSize = 100;
			}
			if ( m_pScreenParams.nDotRadius < 0 ) 
			{
				m_pScreenParams.nDotRadius = 2;
			}
		} 
		else 
		{
			m_pScreenParams.eType = sscreenDispersed;
			if ( m_pScreenParams.nSize < 0 ) 
			{
				m_pScreenParams.nSize = 4;
			}
		}

	}
}

SImageOutputDev::~SImageOutputDev() 
{
	for ( int nIndex = 0; nIndex < m_nT3FontsCount; ++nIndex ) 
	{
		if ( m_pT3FontCache[nIndex] ) 
			delete m_pT3FontCache[nIndex];
	}
	if ( m_pFontEngine ) 
	{
		delete m_pFontEngine;
	}
	if ( m_pSImage ) 
	{
		delete m_pSImage;
	}
	if ( m_pBitmap ) 
	{
		delete m_pBitmap;
	}
}

void SImageOutputDev::NewPDF(XRef *pXref) 
{
	m_pXref = pXref;
	if ( m_pFontEngine ) 
	{
		delete m_pFontEngine;
	}
	m_pFontEngine = new SFontEngine( ( m_pGlobalParams ? m_pGlobalParams->GetEnableGlyphLib() : FALSE ), m_bAllowAA && m_pGlobalParams->GetAntialias() && m_eColorMode != colorModeMono1 );
	for ( int nIndex = 0; nIndex < m_nT3FontsCount; ++nIndex ) 
	{
		if ( m_pT3FontCache[nIndex] )
			delete m_pT3FontCache[nIndex];
	}
	m_nT3FontsCount = 0;
}

void SImageOutputDev::StartPage(int nPageIndex, GrState *pGState) 
{
	int nWidth, nHeight;

	if ( pGState ) 
	{
		SetupScreenParams( pGState->GetHorDPI(), pGState->GetVerDPI() );
		nWidth = (int)(pGState->GetPageWidth() + 0.5);
		if ( nWidth <= 0 ) 
		{
			nWidth = 1;
		}
		nHeight = (int)(pGState->GetPageHeight() + 0.5);
		if ( nHeight <= 0 ) 
		{
			nHeight = 1;
		}
	} 
	else 
	{
		nWidth = nHeight = 1;
	}
	if ( m_pSImage ) 
	{
		delete m_pSImage;
	}
	if ( !m_pBitmap || nWidth != m_pBitmap->GetWidth() || nHeight != m_pBitmap->GetHeight() ) 
	{
		if ( m_pBitmap ) 
		{
			delete m_pBitmap;
		}
		m_pBitmap = new SBitmap( nWidth, nHeight, m_nBitmapRowPad, m_eColorMode, m_eColorMode != colorModeMono1, m_bBitmapTopDown );
	}
	m_pSImage = new SImage( m_pBitmap, m_bVectorAA, &m_pScreenParams );

	if ( pGState ) 
	{
		double *pCTM = pGState->GetCTM();
		double arrMatrix[6];
		arrMatrix[0] = (double)pCTM[0];
		arrMatrix[1] = (double)pCTM[1];
		arrMatrix[2] = (double)pCTM[2];
		arrMatrix[3] = (double)pCTM[3];
		arrMatrix[4] = (double)pCTM[4];
		arrMatrix[5] = (double)pCTM[5];
		m_pSImage->SetMatrix( arrMatrix );
	}

	SColor pColor;
	switch ( m_eColorMode ) 
	{
	case colorModeMono1:
	case colorModeMono8:
		pColor[0] = 0;
		break;
	case colorModeRGB8:
	case colorModeBGR8:
		pColor[0] = pColor[1] = pColor[2] = 0;
		break;
	}

	// Устанавливаем стандартные параметры
	m_pSImage->SetStrokePattern( new SSolidColor(pColor) );
	m_pSImage->SetFillPattern( new SSolidColor(pColor) );
	m_pSImage->SetLineCap( LineCapButt );
	m_pSImage->SetLineJoin( LineJoinMiter );
	m_pSImage->SetLineDash( NULL, 0, 0 );
	m_pSImage->SetMiterLimit( 10 );
	m_pSImage->SetFlatness( 1 );
	// параметр SA по умолчанию должен быть False, но Acrobat иногда выставляет его True
	m_pSImage->SetStrokeAdjust( ( m_pGlobalParams ? m_pGlobalParams->GetStrokeAdjust() : FALSE ) );
	m_pSImage->Clear( m_pPaperColor, 0 );
}

void SImageOutputDev::EndPage() 
{
	if ( m_eColorMode != colorModeMono1 ) 
	{
		m_pSImage->CompositeBackground( m_pPaperColor );
	}
}

void SImageOutputDev::SaveGState(GrState *pGState) 
{
	m_pSImage->SaveState();
}

void SImageOutputDev::RestoreGState(GrState *pGState) 
{
	m_pSImage->RestoreState();
	m_bNeedFontUpdate = TRUE;
}

void SImageOutputDev::UpdateAll(GrState *pGState) 
{
	UpdateLineDash( pGState );
	UpdateLineJoin( pGState );
	UpdateLineCap( pGState );
	UpdateLineWidth( pGState );
	UpdateFlatness( pGState );
	UpdateMiterLimit( pGState );
	UpdateStrokeAdjust( pGState );
	UpdateFillColor( pGState );
	UpdateStrokeColor( pGState );
	m_bNeedFontUpdate = TRUE;
}

void SImageOutputDev::UpdateCTM(GrState *pGState, double dMatrix11, double dMatrix12, double dMatrix21, double dMatrix22, double dMatrix31, double dMatrix32) 
{
	double *pCTM = pGState->GetCTM();

	double arrMatrix[6];
	arrMatrix[0] = (double)pCTM[0];
	arrMatrix[1] = (double)pCTM[1];
	arrMatrix[2] = (double)pCTM[2];
	arrMatrix[3] = (double)pCTM[3];
	arrMatrix[4] = (double)pCTM[4];
	arrMatrix[5] = (double)pCTM[5];
	m_pSImage->SetMatrix( arrMatrix );
}

void SImageOutputDev::UpdateLineDash(GrState *pGState) 
{
	double *pDashPattern;
	int nDashLength;
	double dDashStart;
	double arrDash[20];

	pGState->GetLineDash( &pDashPattern, &nDashLength, &dDashStart );
	if ( nDashLength > 20 ) 
	{
		nDashLength = 20;
	}
	for ( int nIndex = 0; nIndex < nDashLength; ++nIndex ) 
	{
		arrDash[nIndex] = (double)pDashPattern[nIndex];
		if ( arrDash[nIndex] < 0 ) 
		{
			arrDash[nIndex] = 0;
		}
	}
	m_pSImage->SetLineDash( arrDash, nDashLength, (double)dDashStart );
}

void SImageOutputDev::UpdateFlatness(GrState *pGState) 
{
	m_pSImage->SetFlatness( pGState->GetFlatness() );
}

void SImageOutputDev::UpdateLineJoin(GrState *pGState) 
{
	m_pSImage->SetLineJoin( pGState->GetLineJoin() );
}

void SImageOutputDev::UpdateLineCap(GrState *pGState) 
{
	m_pSImage->SetLineCap( pGState->GetLineCap() );
}

void SImageOutputDev::UpdateMiterLimit(GrState *pGState) 
{
	m_pSImage->SetMiterLimit( pGState->GetMiterLimit() );
}

void SImageOutputDev::UpdateLineWidth(GrState *pGState) 
{
	m_pSImage->SetLineWidth( pGState->GetLineWidth() );
}

void SImageOutputDev::UpdateStrokeAdjust(GrState *pGState) 
{
#if 0 
	// параметр SA по умолчанию должен быть False, но Acrobat иногда выставляет его True
	m_pSImage->SetStrokeAdjust( pGState->GetStrokeAdjust() );
#endif
}

void SImageOutputDev::UpdateFillColor(GrState *pGState) 
{
	GrGray oGray;
	GrRGB oRGB;

	pGState->GetFillGray( &oGray );
	pGState->GetFillRGB( &oRGB );
	m_pSImage->SetFillPattern( GetColor( oGray, &oRGB ) );
}

void SImageOutputDev::UpdateStrokeColor(GrState *pGState) 
{
	GrGray oGray;
	GrRGB oRGB;

	pGState->GetStrokeGray( &oGray );
	pGState->GetStrokeRGB( &oRGB );
	m_pSImage->SetStrokePattern( GetColor( oGray, &oRGB ) );
}

SPattern *SImageOutputDev::GetColor(GrGray oGray, GrRGB *pRGB) 
{
	GrColorComp nR, nG, nB;

	if ( m_bReverseVideo ) 
	{
		oGray = GrColorComp1 - oGray;
		nR = GrColorComp1 - pRGB->r;
		nG = GrColorComp1 - pRGB->g;
		nB = GrColorComp1 - pRGB->b;
	} 
	else 
	{
		nR = pRGB->r;
		nG = pRGB->g;
		nB = pRGB->b;
	}

	SPattern *pPattern = NULL;
	SColor pColor;
	switch ( m_eColorMode ) 
	{
	case colorModeMono1:
	case colorModeMono8:
		pColor[0] = ColorToByte( oGray );
		pPattern = new SSolidColor( pColor );
		break;
	case colorModeRGB8:
	case colorModeBGR8:
		pColor[0] = ColorToByte( nR );
		pColor[1] = ColorToByte( nG );
		pColor[2] = ColorToByte( nB );
		pPattern = new SSolidColor( pColor );
		break;
	}

	return pPattern;
}

void SImageOutputDev::UpdateBlendMode(GrState *pGState) 
{
	m_pSImage->SetBlendFunc( c_arrBlendFunctions[pGState->GetBlendMode()] );
}

void SImageOutputDev::UpdateFillOpacity(GrState *pGState) 
{
	m_pSImage->SetFillAlpha( (double)pGState->GetFillOpacity() );
}

void SImageOutputDev::UpdateStrokeOpacity(GrState *pGState) 
{
	m_pSImage->SetStrokeAlpha( (double)pGState->GetStrokeOpacity() );
}

void SImageOutputDev::UpdateFont(GrState *pGState) 
{
	m_bNeedFontUpdate = TRUE;
}

void SImageOutputDev::DoUpdateFont(GrState *pGState) 
{
	m_bNeedFontUpdate = FALSE;
	m_pFont = NULL;

	CStringW wsTempFileName = _T("");

	GrFont *pGrFont;
	if ( !( pGrFont = pGState->GetFont() ) ) 
	{
		return;
	}
	GrFontType eFontType = pGrFont->GetType();
	if ( eFontType == fontType3 ) // Type3-шрифты обрабатываются отдельной командой
	{
		return;
	}

	// Проверяем, возможно нужный FontFile еще лежит в кэше
	SImageOutFontFileID *pID = new SImageOutFontFileID( pGrFont->GetID() );
	if ( !pID )
		return;

	CFontFileTrueType *pTTFontFile = NULL;
	unsigned short *pCodeToGID = NULL;
	int nLen = 0;

	CStringW wsFileName = _T("");
	DisplayFontParam *pDisplayFontParam = NULL;
	SFontFile *pFontFile = NULL;
	if ( ( pFontFile = m_pFontEngine->GetFontFile( pID ) ) ) 
	{
		delete pID;
	} 
	else 
	{
		// Если шрифт включен в PDF-файл, записываем его на диск
		Ref oEmbRef;
		if ( pGrFont->GetEmbeddedFontFileRef( &oEmbRef ) ) 
		{
			FILE *pTempFile = NULL;
			if ( !OpenTempFile( &wsTempFileName, &pTempFile, _T("wb"), NULL, m_pGlobalParams->GetTempFolder() ) ) 
			{
				// TO DO: Eerror "Couldn't create temporary font file"
				delete pID;

				if ( _T("") != wsTempFileName ) 
				{
					_wunlink( wsTempFileName.GetBuffer() );
				}
				return;
			}
			Object oReferenceObject, oStreamObject;
			oReferenceObject.InitRef( oEmbRef.nNum, oEmbRef.nGen );
			oReferenceObject.Fetch( m_pXref, &oStreamObject );
			oReferenceObject.Free();
			if ( !oStreamObject.IsStream() ) 
			{
				// TO DO: Error "Embedded font object is wrong type"
				oStreamObject.Free();
				fclose( pTempFile );
				delete pID;

				if ( _T("") != wsTempFileName ) 
				{
					_wunlink( wsTempFileName.GetBuffer() );
				}
				return;
			}
			oStreamObject.StreamReset();
			int nChar;
			while ( ( nChar = oStreamObject.StreamGetChar() ) != EOF ) 
			{
				fputc( nChar, pTempFile );
			}
			oStreamObject.StreamClose();
			oStreamObject.Free();
			fclose( pTempFile );
			wsFileName = wsTempFileName;
		} 
		else if ( _T("") == ( wsFileName = pGrFont->GetExternalFontFilePath() ) ) // Если есть внешний FontFile, тогда используем его
		{
			// Ищем DisplayFont
			if ( pGrFont->IsCIDFont() ) 
			{
				if ( m_pGlobalParams && ((GrCIDFont *)pGrFont)->GetCollection() ) 
				{
					pDisplayFontParam = m_pGlobalParams->GetDisplayCIDFont( pGrFont->GetBaseName(), ((GrCIDFont *)pGrFont)->GetCollection() );
				}
			} 
			else 
			{
				if ( m_pGlobalParams && pGrFont->GetBaseName() ) 
				{
					pDisplayFontParam = m_pGlobalParams->GetDisplayFont( pGrFont->GetBaseName() );
				}
				if ( m_pGlobalParams && !pDisplayFontParam ) 
				{
					int nSubstIndex = -1;

					if ( pGrFont->IsFixedWidth() ) 
					{
						nSubstIndex = 8;
					} 
					else if ( pGrFont->IsSerif() ) 
					{
						nSubstIndex = 4;
					} 
					else 
					{
						nSubstIndex = 0;
					}
					if ( pGrFont->IsBold() ) 
					{
						nSubstIndex += 2;
					}
					if ( pGrFont->IsItalic() ) 
					{
						nSubstIndex += 1;
					}
					StringExt *seSubstName = new StringExt( c_arrSImageOutSubstFonts[nSubstIndex].sName );
					pDisplayFontParam = m_pGlobalParams->GetDisplayFont( seSubstName );
					delete seSubstName;
					pID->SetSubstIndex( nSubstIndex );
				}
			}
			if ( !pDisplayFontParam ) 
			{
				// TO DO: Error "Couldn't find a font"
				delete pID;

				if ( _T("") != wsTempFileName ) 
				{
					_wunlink( wsTempFileName.GetBuffer() );
				}
				return;
			}
			switch ( pDisplayFontParam->m_eType ) 
			{
			case displayFontT1:
				wsFileName = *(pDisplayFontParam->m_oT1.pwsFileName);
				eFontType  = pGrFont->IsCIDFont() ? fontCIDType0 : fontType1;
				break;
			case displayFontTT:
				wsFileName = *(pDisplayFontParam->m_oTT.pwsFileName);
				eFontType  = pGrFont->IsCIDFont() ? fontCIDType2 : fontTrueType;
				break;
			}
		}
		
		// Загружаем FontFile
		switch ( eFontType ) 
		{
		case fontType1:
			if ( !( pFontFile = m_pFontEngine->LoadType1Font( pID, wsFileName.GetBuffer(), wsFileName == wsTempFileName, ((Gr8BitFont *)pGrFont)->GetEncoding() ) ) ) 
			{
				// TO DO: Error "Couldn't create a font"
				delete pID;

				if ( _T("") != wsTempFileName ) 
				{
					_wunlink( wsTempFileName.GetBuffer() );
				}
				return;
			}
			break;
		case fontType1C:
			if ( !( pFontFile = m_pFontEngine->LoadType1CFont( pID, wsFileName.GetBuffer(), wsFileName == wsTempFileName, ((Gr8BitFont *)pGrFont)->GetEncoding() ) ) ) 
			{
				// TO DO: Error "Couldn't create a font"
				delete pID;

				if ( _T("") != wsTempFileName ) 
				{
					_wunlink( wsTempFileName.GetBuffer() );
				}
				return;
			}
			break;
		case fontType1COT:
			if ( !( pFontFile = m_pFontEngine->LoadOpenTypeT1CFont( pID, wsFileName.GetBuffer(), wsFileName == wsTempFileName, ((Gr8BitFont *)pGrFont)->GetEncoding() ) ) ) 
			{
				// TO DO: Error "Couldn't create a font"
				delete pID;

				if ( _T("") != wsTempFileName ) 
				{
					_wunlink( wsTempFileName.GetBuffer() );
				}
				return;
			}
			break;
		case fontTrueType:
		case fontTrueTypeOT:
			if ( ( pTTFontFile = CFontFileTrueType::LoadFromFile( wsFileName.GetBuffer() ) ) ) 
			{
				pCodeToGID = ((Gr8BitFont *)pGrFont)->GetCodeToGIDMap( pTTFontFile );
				nLen = 256;
				delete pTTFontFile;
			} 
			else 
			{
				pCodeToGID = NULL;
				nLen = 0;
			}
			if ( !( pFontFile = m_pFontEngine->LoadTrueTypeFont( pID, wsFileName.GetBuffer(), wsFileName == wsTempFileName, pCodeToGID, nLen, m_pGlobalParams->GetTempFolder() ) ) ) 
			{
				// TO DO: Error "Couldn't create a font"
				delete pID;

				if ( _T("") != wsTempFileName ) 
				{
					_wunlink( wsTempFileName.GetBuffer() );
				}
				return;
			}
			break;
		case fontCIDType0:
		case fontCIDType0C:
			if ( !( pFontFile = m_pFontEngine->LoadCIDFont( pID, wsFileName.GetBuffer(), wsFileName == wsTempFileName ) ) ) 
			{
				// TO DO: Error "Couldn't create a font"
				delete pID;

				if ( _T("") != wsTempFileName ) 
				{
					_wunlink( wsTempFileName.GetBuffer() );
				}
				return;
			}
			break;
		case fontCIDType0COT:
			if ( !( pFontFile = m_pFontEngine->LoadOpenTypeCFFFont( pID, wsFileName.GetBuffer(), wsFileName == wsTempFileName ) ) ) 
			{
				// TO DO: Error "Couldn't create a font"
				delete pID;

				if ( _T("") != wsTempFileName ) 
				{
					_wunlink( wsTempFileName.GetBuffer() );
				}
				return;
			}
			break;
		case fontCIDType2:
		case fontCIDType2OT:
			pCodeToGID = NULL;
			nLen = 0;
			if ( pDisplayFontParam ) 
			{
				// Создаем карту CID-to-GID
				CharCodeToUnicode *pCodeToUnicode = NULL;
				if ( ( pCodeToUnicode = ((GrCIDFont *)pGrFont)->GetToUnicode() ) ) 
				{
					if ( ( pTTFontFile = CFontFileTrueType::LoadFromFile( wsFileName.GetBuffer() ) ) ) 
					{
						// Ищем Unicode Cmap
						int nCMapIndex;
						for ( nCMapIndex = 0; nCMapIndex < pTTFontFile->GetCmapsCount(); ++nCMapIndex )  
						{
							if ( ( pTTFontFile->GetCmapPlatform(nCMapIndex) == 3 && pTTFontFile->GetCmapEncoding(nCMapIndex) == 1 ) || pTTFontFile->GetCmapPlatform(nCMapIndex) == 0 ) 
							{
								break;
							}
						}
						if ( nCMapIndex < pTTFontFile->GetCmapsCount() ) 
						{
							// CID -> Unicode -> GID
							nLen = pCodeToUnicode->GetLength();
							pCodeToGID = (unsigned short *)MemUtilsMallocArray( nLen, sizeof(unsigned short) );
							for ( int nCode = 0; nCode < nLen; ++nCode ) 
							{
								Unicode arrUnicodeBuffer[8];
								if ( pCodeToUnicode->MapToUnicode( nCode, arrUnicodeBuffer, 8 ) > 0 ) 
								{
									pCodeToGID[nCode] = pTTFontFile->MapCodeToGID( nCMapIndex, arrUnicodeBuffer[0] );
								} 
								else 
								{
									pCodeToGID[nCode] = 0;
								}
							}
						}
						delete pTTFontFile;
					}
					pCodeToUnicode->Release();
				} 
				else 
				{
					// TO DO: Error "Couldn't find a mapping to Unicode"
				}
			} 
			else 
			{
				if ( ((GrCIDFont *)pGrFont)->GetCIDToGID() ) 
				{
					nLen = ((GrCIDFont *)pGrFont)->GetCIDToGIDLen();
					pCodeToGID = (unsigned short *)MemUtilsMallocArray( nLen, sizeof(unsigned short));
					memcpy( pCodeToGID, ((GrCIDFont *)pGrFont)->GetCIDToGID(), nLen * sizeof(unsigned short) );
				}
			}
			if ( !( pFontFile = m_pFontEngine->LoadTrueTypeFont( pID, wsFileName.GetBuffer(), wsFileName == wsTempFileName, pCodeToGID, nLen, m_pGlobalParams->GetTempFolder() ) ) ) 
			{
				// TO DO: Error "Couldn't create a font"
				delete pID;

				if ( _T("") != wsTempFileName ) 
				{
					_wunlink( wsTempFileName.GetBuffer() );
				}
				return;
			}
			break;
		default:
			// Такого не должно произойти
			delete pID;

			if ( _T("") != wsTempFileName ) 
			{
				_wunlink( wsTempFileName.GetBuffer() );
			}
			return;
		}
	}

	// Считываем матрицу данного шрифта
	double *pTextMatrix = pGState->GetTextMatrix();
	double dFontSize = pGState->GetFontSize();
	double dMatrix11 = pTextMatrix[0] * dFontSize * pGState->GetHorizScaling();
	double dMatrix12 = pTextMatrix[1] * dFontSize * pGState->GetHorizScaling();
	double dMatrix21 = pTextMatrix[2] * dFontSize;
	double dMatrix22 = pTextMatrix[3] * dFontSize;

	// Для Заменяемых шрифтов: применяем матрицу шрифта и сравниваем ширину символа 'm' в исходном шрифте и в замененном шрифте
	int nSubstIndex = ((SImageOutFontFileID *)pFontFile->GetFontFileID())->GetSubstIndex();
	if ( nSubstIndex >= 0 ) 
	{
		int nCodeM;
		for ( nCodeM = 0; nCodeM < 256; ++nCodeM ) 
		{
			char *sName = NULL;
			if ( ( sName = ((Gr8BitFont *)pGrFont)->GetCharName(nCodeM)) && sName[0] == 'm' && sName[1] == '\0' ) 
			{
				break;
			}
		}
		if ( nCodeM < 256 ) 
		{
			double dWidth1 = ((Gr8BitFont *)pGrFont)->GetWidth( nCodeM );
			double dWidth2 = c_arrSImageOutSubstFonts[nSubstIndex].dAvrWidth;
			if ( !pGrFont->IsSymbolic() ) 
			{
				// Если реальный шрифт существенно уже, чем шрифт которым мы его заменяем, тогда уменьшаем ширину букв соответственно
				if ( dWidth1 > 0.01 && dWidth1 < 0.9 * dWidth2 ) 
				{
					dWidth1 /= dWidth2;
					dMatrix11 *= dWidth1;
					dMatrix21 *= dWidth1;
				}
			}
		}
	}

	// Создаем шрифт
	double arrMatrix[4];
	arrMatrix[0] = dMatrix11; arrMatrix[1] = dMatrix12;
	arrMatrix[2] = dMatrix21; arrMatrix[3] = dMatrix22;
	m_pFont = m_pFontEngine->GetFont( pFontFile, arrMatrix, m_pSImage->GetMatrix() );

	return;
}

void SImageOutputDev::Stroke(GrState *pGState) 
{
	if ( pGState->GetStrokeColorSpace()->IsNonMarking() ) 
	{
		return;
	}
	SPath *pPath = ConvertPath( pGState, pGState->GetPath() );
	if ( !pPath )
		return;
	m_pSImage->Stroke( pPath );
	delete pPath;
}

void SImageOutputDev::Fill(GrState *pGState) 
{
	if ( pGState->GetFillColorSpace()->IsNonMarking() ) 
	{
		return;
	}
	SPath *pPath = ConvertPath( pGState, pGState->GetPath() );
	if ( !pPath )
		return;
	m_pSImage->Fill( pPath, FALSE );
	delete pPath;
}

void SImageOutputDev::EoFill(GrState *pGState) 
{
	if ( pGState->GetFillColorSpace()->IsNonMarking() ) 
	{
		return;
	}
	SPath *pPath = ConvertPath( pGState, pGState->GetPath() );
	if ( !pPath )
		return;
	m_pSImage->Fill( pPath, TRUE );
	delete pPath;
}

void SImageOutputDev::Clip(GrState *pGState) 
{
	SPath *pPath = ConvertPath( pGState, pGState->GetPath() );
	m_pSImage->ClipToPath( pPath, FALSE );
	delete pPath;
}

void SImageOutputDev::EoClip(GrState *pGState) 
{
	SPath *pPath = ConvertPath( pGState, pGState->GetPath() );
	if ( !pPath )
		return;
	m_pSImage->ClipToPath( pPath, TRUE );
	delete pPath;
}

void SImageOutputDev::ClipToStrokePath(GrState *pGState) 
{
	SPath *pPath = ConvertPath( pGState, pGState->GetPath() );
	SPath *pStrokePath = m_pSImage->MakeStrokePath( pPath );
	if ( !pPath || ! pStrokePath )
		return;
	
	delete pPath;
	m_pSImage->ClipToPath( pStrokePath, FALSE );
	delete pStrokePath;
}

SPath *SImageOutputDev::ConvertPath(GrState *pGState, GrPath *pPath) 
{
	SPath *pSPath = new SPath();
	for ( int nSubPathIndex = 0; nSubPathIndex < pPath->GetSubpathsCount(); ++nSubPathIndex ) 
	{
		GrSubpath *pSubPath = pPath->GetSubpath(nSubPathIndex);
		if ( pSubPath->GetPointsCount() > 0 ) 
		{
			pSPath->MoveTo( (double)pSubPath->GetX(0), (double)pSubPath->GetY(0) );
			int nPointsIndex = 1;
			while ( nPointsIndex < pSubPath->GetPointsCount() ) 
			{
				if ( pSubPath->GetCurve(nPointsIndex) ) 
				{
					pSPath->CurveTo( (double)pSubPath->GetX(nPointsIndex), (double)pSubPath->GetY(nPointsIndex), (double)pSubPath->GetX(nPointsIndex + 1), (double)pSubPath->GetY(nPointsIndex + 1), (double)pSubPath->GetX(nPointsIndex + 2), (double)pSubPath->GetY(nPointsIndex + 2) );
					nPointsIndex += 3;
				} 
				else 
				{
					pSPath->LineTo( (double)pSubPath->GetX(nPointsIndex), (double)pSubPath->GetY(nPointsIndex) );
					++nPointsIndex;
				}
			}
			if ( pSubPath->IsClosed() ) 
			{
				pSPath->Close();
			}
		}
	}
	return pSPath;
}

void SImageOutputDev::DrawChar(GrState *pGState, double dX, double dY, double dDx, double dDy, double dOriginX, double dOriginY, CharCode nCode, int nBytesCount, Unicode *pUnicode, int nUnicodeLen) 
{
	// Проверяем, возможно текст должен быть невидимым
	int nRenderMode = pGState->GetRenderMode();
	if ( nRenderMode == 3 ) 
	{
		return;
	}

	// Обновляем шрифт, если надо
	if ( m_bNeedFontUpdate ) 
	{
		DoUpdateFont( pGState );
	}
	if ( !m_pFont ) 
	{
		return;
	}

	dX -= dOriginX;
	dY -= dOriginY;

	// Fill
	if ( !( nRenderMode & 1 ) ) 
	{
		if ( !pGState->GetFillColorSpace()->IsNonMarking() ) 
		{
			m_pSImage->FillChar( (double)dX, (double)dY, nCode, m_pFont );
		}
	}

	// Stroke
	if ( ( nRenderMode & 3 ) == 1 || ( nRenderMode & 3 ) == 2 ) 
	{
		if ( !pGState->GetStrokeColorSpace()->IsNonMarking() ) 
		{
			SPath *pPath = NULL;
			if ( ( pPath = m_pFont->GetGlyphPath( nCode ) ) ) 
			{
				pPath->Offset( (double)dX, (double)dY ); 
				m_pSImage->Stroke( pPath );
				delete pPath;
			}
		}
	}

	// Clip
	if ( nRenderMode & 4 ) 
	{
		SPath *pPath = NULL;
		if ( ( pPath = m_pFont->GetGlyphPath( nCode ) ) ) 
		{
			pPath->Offset( (double)dX, (double)dY );
			if ( m_pTextClipPath ) 
			{
				m_pTextClipPath->Append( pPath );
				delete pPath;
			} 
			else 
			{
				m_pTextClipPath = pPath;
			}
		}
	}
}

BOOL SImageOutputDev::BeginType3Char(GrState *pGState, double dX, double dY, double dDx, double dDy, CharCode nCode, Unicode *pUnicode, int nUnicodeLen) 
{
	T3FontCache *pT3Font = NULL;

	GrFont *pGFront;
	if ( !( pGFront = pGState->GetFont() ) ) 
	{
		return FALSE;
	}
	Ref *pFontID = pGFront->GetID();
	double *pCTM = pGState->GetCTM();
	double dTransX, dTransY;
	pGState->Transform(0, 0, &dTransX, &dTransY);

	// Проверяем первый ли это шрифт из кэша(MRU MostRecentUsed)?
	if ( !( m_nT3FontsCount > 0 && m_pT3FontCache[0]->Matches( pFontID, pCTM[0], pCTM[1], pCTM[2], pCTM[3] ) ) ) 
	{
		// Проверяем оставшиеся фонты в кэше?
		int nFontIndex;
		for ( nFontIndex = 1; nFontIndex < m_nT3FontsCount; ++nFontIndex ) 
		{
			if ( m_pT3FontCache[nFontIndex]->Matches( pFontID, pCTM[0], pCTM[1], pCTM[2], pCTM[3] ) ) 
			{
				pT3Font = m_pT3FontCache[nFontIndex];
				for ( int nJ = nFontIndex; nJ > 0; --nJ ) 
				{
					m_pT3FontCache[nJ] = m_pT3FontCache[nJ - 1];
				}
				m_pT3FontCache[0] = pT3Font;
				break;
			}
		}
		if ( nFontIndex >= m_nT3FontsCount ) 
		{
			// Создаем новую запись в кэше
			BOOL bValidBBox = FALSE;	
			double dMinX, dMinY, dMaxX, dMaxY;
			if ( m_nT3FontsCount == T3FontCacheSize ) 
			{
				delete m_pT3FontCache[m_nT3FontsCount - 1];
				--m_nT3FontsCount;
			}
			for ( int nJ = m_nT3FontsCount; nJ > 0; --nJ ) 
			{
				m_pT3FontCache[nJ] = m_pT3FontCache[nJ - 1];
			}
			++m_nT3FontsCount;
			double *pBBox = pGFront->GetFontBBox();
			if ( pBBox[0] == 0 && pBBox[1] == 0 && pBBox[2] == 0 && pBBox[3] == 0 ) 
			{
				// BBox неопределен, пытаемся угадать его :)
				dMinX = dTransX - 5;
				dMaxX = dMinX + 30;
				dMaxY = dTransY + 15;
				dMinY = dMaxY - 45;
				bValidBBox = FALSE;
			} 
			else 
			{
				double dX1, dY1;
				pGState->Transform( pBBox[0], pBBox[1], &dX1, &dY1);
				dMinX = dMaxX = dX1;
				dMinY = dMaxY = dY1;
				pGState->Transform( pBBox[0], pBBox[3], &dX1, &dY1);
				if ( dX1 < dMinX ) 
				{
					dMinX = dX1;
				} 
				else if ( dX1 > dMaxX ) 
				{
					dMaxX = dX1;
				}
				if ( dY1 < dMinY ) 
				{
					dMinY = dY1;
				} 
				else if ( dY1 > dMaxY ) 
				{
					dMaxY = dY1;
				}
				pGState->Transform( pBBox[2], pBBox[1], &dX1, &dY1 );
				if ( dX1 < dMinX ) 
				{
					dMinX = dX1;
				}
				else if ( dX1 > dMaxX ) 
				{
					dMaxX = dX1;
				}
				if ( dY1 < dMinY ) 
				{
					dMinY = dY1;
				} 
				else if ( dY1 > dMaxY ) 
				{
					dMaxY = dY1;
				}
				pGState->Transform( pBBox[2], pBBox[3], &dX1, &dY1 );
				if ( dX1 < dMinX ) 
				{
					dMinX = dX1;
				} 
				else if ( dX1 > dMaxX ) 
				{
					dMaxX = dX1;
				}
				if ( dY1 < dMinY ) 
				{
					dMinY = dY1;
				} 
				else if ( dY1 > dMaxY ) 
				{
					dMaxY = dY1;
				}
				bValidBBox = TRUE;
			}
			m_pT3FontCache[0] = new T3FontCache( pFontID, pCTM[0], pCTM[1], pCTM[2], pCTM[3], (int)floor(dMinX - dTransX), (int)floor(dMinY - dTransY), (int)ceil(dMaxX) - (int)floor(dMinX) + 3, (int)ceil(dMaxY) - (int)floor(dMinY) + 3, bValidBBox, m_eColorMode != colorModeMono1 );
		}
	}
	pT3Font = m_pT3FontCache[0];

	// Проверяем, возможно данный символ уже есть в кэше.
	int nSetIndex = (nCode & (pT3Font->m_nCacheSets - 1)) * pT3Font->m_nCacheAssoc;
	for ( int nGlyphIndex = 0; nGlyphIndex < pT3Font->m_nCacheAssoc; ++nGlyphIndex ) 
	{
		if ( ( pT3Font->m_pCacheTags[nSetIndex + nGlyphIndex].unMRU & 0x8000 ) && pT3Font->m_pCacheTags[nSetIndex + nGlyphIndex].unCode == nCode ) 
		{
			DrawType3Glyph( pT3Font, &pT3Font->m_pCacheTags[nSetIndex + nGlyphIndex], pT3Font->m_pCacheData + (nSetIndex + nGlyphIndex) * pT3Font->m_nGlyphSize );
			return TRUE;
		}
	}

	// Добавляем новый Type3-символ в стек
	T3GlyphStack *pT3GlyphStack = new T3GlyphStack();
	pT3GlyphStack->pNext = m_pT3GlyphStack;
	m_pT3GlyphStack             = pT3GlyphStack;
	m_pT3GlyphStack->unCode     = nCode;
	m_pT3GlyphStack->pCache     = pT3Font;
	m_pT3GlyphStack->pCacheTag  = NULL;
	m_pT3GlyphStack->pCacheData = NULL;

	return FALSE;
}

void SImageOutputDev::EndType3Char(GrState *pGState) 
{
	if ( m_pT3GlyphStack->pCacheTag ) 
	{
		memcpy( m_pT3GlyphStack->pCacheData, m_pBitmap->GetData(), m_pT3GlyphStack->pCache->m_nGlyphSize );
		delete m_pBitmap;
		delete m_pSImage;
		m_pBitmap = m_pT3GlyphStack->pOriginalBitmap;
		m_pSImage = m_pT3GlyphStack->pOriginalSImage;
		double *pCTM = pGState->GetCTM();
		pGState->SetCTM( pCTM[0], pCTM[1], pCTM[2], pCTM[3], m_pT3GlyphStack->dOriginalCTM4, m_pT3GlyphStack->dOriginalCTM5 );
		UpdateCTM( pGState, 0, 0, 0, 0, 0, 0 );
		DrawType3Glyph( m_pT3GlyphStack->pCache, m_pT3GlyphStack->pCacheTag, m_pT3GlyphStack->pCacheData );
	}
	T3GlyphStack *pT3GlyphStack = m_pT3GlyphStack;
	m_pT3GlyphStack = pT3GlyphStack->pNext;
	delete pT3GlyphStack;
}

void SImageOutputDev::Type3D0(GrState *pGState, double dWx, double dWy) 
{
}

void SImageOutputDev::Type3D1(GrState *pGState, double dWx, double dWy, double dBLx, double dBLy, double dTRx, double dTRy) 
{
	T3FontCache *pT3Font = m_pT3GlyphStack->pCache;

	// Проверяем корректность BBox
	double dTransX, dTransY;
	pGState->Transform( 0, 0, &dTransX, &dTransY );
	double dX1, dY1;
	pGState->Transform( dBLx, dBLy, &dX1, &dY1 );
	double dMinX = dX1, dMaxX = dX1, dMinY = dY1, dMaxY = dY1;
	pGState->Transform( dBLx, dTRy, &dX1, &dY1);
	if ( dX1 < dMinX ) 
	{
		dMinX = dX1;
	} 
	else if ( dX1 > dMaxX ) 
	{
		dMaxX = dX1;
	}
	if ( dY1 < dMinY ) 
	{
		dMinY = dY1;
	} 
	else if ( dY1 > dMaxY ) 
	{
		dMaxY = dY1;
	}
	pGState->Transform( dTRx, dBLy, &dX1, &dY1);
	if ( dX1 < dMinX ) 
	{
		dMinX = dX1;
	} 
	else if ( dX1 > dMaxX ) 
	{
		dMaxX = dX1;
	}
	if ( dY1 < dMinY ) 
	{
		dMinY = dY1;
	} 
	else if ( dY1 > dMaxY ) 
	{
		dMaxY = dY1;
	}
	pGState->Transform( dTRx, dTRy, &dX1, &dY1 );
	if ( dX1 < dMinX ) 
	{
		dMinX = dX1;
	} 
	else if ( dX1 > dMaxX ) 
	{
		dMaxX = dX1;
	}
	if ( dY1 < dMinY ) 
	{
		dMinY = dY1;
	} 
	else if ( dY1 > dMaxY ) 
	{
		dMaxY = dY1;
	}
	if ( dMinX - dTransX < pT3Font->m_nGlyphX || dMinY - dTransY < pT3Font->m_nGlyphY || dMaxX - dTransX > pT3Font->m_nGlyphX + pT3Font->m_nGlyphWidth || dMaxY - dTransY > pT3Font->m_nGlyphY + pT3Font->m_nGlyphHeight ) 
	{
		if ( pT3Font->m_bValidBBox ) 
		{
			// TO DO: Error "Bad bounding box in Type 3 glyph"
		}
		return;
	}

	int nSetIndex = ( m_pT3GlyphStack->unCode & (pT3Font->m_nCacheSets - 1)) * pT3Font->m_nCacheAssoc;
	for ( int nGlyphIndex = 0; nGlyphIndex < pT3Font->m_nCacheAssoc; ++nGlyphIndex ) 
	{
		if ( (pT3Font->m_pCacheTags[nSetIndex + nGlyphIndex].unMRU & 0x7fff) == pT3Font->m_nCacheAssoc - 1 ) 
		{
			pT3Font->m_pCacheTags[nSetIndex + nGlyphIndex].unMRU = 0x8000;
			pT3Font->m_pCacheTags[nSetIndex + nGlyphIndex].unCode = m_pT3GlyphStack->unCode;
			m_pT3GlyphStack->pCacheTag  = &pT3Font->m_pCacheTags[nSetIndex + nGlyphIndex];
			m_pT3GlyphStack->pCacheData =  pT3Font->m_pCacheData + (nSetIndex + nGlyphIndex) * pT3Font->m_nGlyphSize;
		} 
		else 
		{
			++pT3Font->m_pCacheTags[nSetIndex + nGlyphIndex].unMRU;
		}
	}

	// Сохраняем состояние
	m_pT3GlyphStack->pOriginalBitmap = m_pBitmap;
	m_pT3GlyphStack->pOriginalSImage = m_pSImage;
	double *pCTM = pGState->GetCTM();
	m_pT3GlyphStack->dOriginalCTM4 = pCTM[4];
	m_pT3GlyphStack->dOriginalCTM5 = pCTM[5];

	// Создаем временный битмап
	SColor pColor;
	if ( m_eColorMode == colorModeMono1 ) 
	{
		m_pBitmap = new SBitmap( pT3Font->m_nGlyphWidth, pT3Font->m_nGlyphHeight, 1, colorModeMono1, FALSE );
		m_pSImage = new SImage ( m_pBitmap, FALSE, m_pT3GlyphStack->pOriginalSImage->GetScreen() );
		pColor[0] = 0;
		m_pSImage->Clear( pColor );
		pColor[0] = 1;
	} 
	else 
	{
		m_pBitmap = new SBitmap( pT3Font->m_nGlyphWidth, pT3Font->m_nGlyphHeight, 1, colorModeMono8, FALSE );
		m_pSImage = new SImage ( m_pBitmap, m_bVectorAA, m_pT3GlyphStack->pOriginalSImage->GetScreen() );
		pColor[0] = 0x00;
		m_pSImage->Clear( pColor );
		pColor[0] = 0xff;
	}
	m_pSImage->SetFillPattern( new SSolidColor( pColor ) );
	m_pSImage->SetStrokePattern( new SSolidColor( pColor ) );

	pGState->SetCTM( pCTM[0], pCTM[1], pCTM[2], pCTM[3], -pT3Font->m_nGlyphX, -pT3Font->m_nGlyphY );
	UpdateCTM( pGState, 0, 0, 0, 0, 0, 0 );
}

void SImageOutputDev::DrawType3Glyph(T3FontCache *pT3Font, T3FontCacheTag *pT3Tag, unsigned char *pData) 
{
	SGlyphBitmap oGlyphBitmap;

	oGlyphBitmap.nX        = -pT3Font->m_nGlyphX;
	oGlyphBitmap.nY        = -pT3Font->m_nGlyphY;
	oGlyphBitmap.nWidth    =  pT3Font->m_nGlyphWidth;
	oGlyphBitmap.nHeight   =  pT3Font->m_nGlyphHeight;
	oGlyphBitmap.bAA       = m_eColorMode != colorModeMono1;
	oGlyphBitmap.pData     = pData;
	oGlyphBitmap.bFreeData = FALSE;

	m_pSImage->FillGlyph( 0, 0, &oGlyphBitmap );
}

void SImageOutputDev::EndTextObject(GrState *pGState) 
{
	if ( m_pTextClipPath ) 
	{
		m_pSImage->ClipToPath( m_pTextClipPath, FALSE );
		delete m_pTextClipPath;
		m_pTextClipPath = NULL;
	}
}

BOOL SImageOutputDev::ImageMaskSrc(void *pData, SColorPointer pLine) 
{
	ImageMaskData *pImageMaskData = (ImageMaskData *)pData;

	if ( pImageMaskData->nY == pImageMaskData->nHeight ) 
	{
		return FALSE;
	}
	int nX;
	unsigned char *pMask;
	SColorPointer pImageMask;
	for ( nX = 0, pMask = pImageMaskData->pImageStream->GetNextLine(), pImageMask = pLine; nX < pImageMaskData->nWidth; ++nX ) 
	{
		*pImageMask++ = *pMask++ ^ pImageMaskData->bInvert;
	}
	++pImageMaskData->nY;
	return TRUE;
}

void SImageOutputDev::DrawImageMask(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, BOOL bInvert, BOOL bInlineImage) 
{
	if ( pGState->GetFillColorSpace()->IsNonMarking() ) 
	{
		return;
	}

	double arrMatrix[6];
	double *pCTM = pGState->GetCTM();
	arrMatrix[0] =  pCTM[0];
	arrMatrix[1] =  pCTM[1];
	arrMatrix[2] = -pCTM[2];
	arrMatrix[3] = -pCTM[3];
	arrMatrix[4] =  pCTM[2] + pCTM[4];
	arrMatrix[5] =  pCTM[3] + pCTM[5];

	ImageMaskData oImageMaskData;
	oImageMaskData.pImageStream = new ImageStream( pStream, nWidth, 1, 1 );
	oImageMaskData.pImageStream->Reset();
	oImageMaskData.bInvert = bInvert ? 0 : 1;
	oImageMaskData.nWidth  = nWidth;
	oImageMaskData.nHeight = nHeight;
	oImageMaskData.nY      = 0;

	m_pSImage->FillImageMask( &ImageMaskSrc, &oImageMaskData, nWidth, nHeight, arrMatrix, m_pT3GlyphStack != NULL );
	if ( bInlineImage ) 
	{
		while ( oImageMaskData.nY < nHeight ) 
		{
			oImageMaskData.pImageStream->GetNextLine();
			++oImageMaskData.nY;
		}
	}

	delete oImageMaskData.pImageStream;
	pStream->Close();
}

BOOL SImageOutputDev::ImageSrc(void *pData, SColorPointer pColorLine, unsigned char *pAlphaLine) 
{
	ImageData *pImageData = (ImageData *)pData;
	if ( pImageData->nY == pImageData->nHeight ) 
	{
		return FALSE;
	}

	int nComponentsCount = pImageData->pColorMap->GetComponentsCount();

	int nX;
	unsigned char *pSrc;	
	SColorPointer pRes, col;
	if ( pImageData->pLookup ) 
	{
		switch (pImageData->eColorMode) 
		{
		case colorModeMono1:
		case colorModeMono8:
			for ( nX = 0, pSrc = pImageData->pStream->GetNextLine(), pRes = pColorLine; nX < pImageData->nWidth; ++nX, ++pSrc ) 
			{
				*pRes++ = pImageData->pLookup[*pSrc];
			}
			break;
		case colorModeRGB8:
		case colorModeBGR8:
			for ( nX = 0, pSrc = pImageData->pStream->GetNextLine(), pRes = pColorLine; nX < pImageData->nWidth; ++nX, ++pSrc ) 
			{
				col = &pImageData->pLookup[3 * *pSrc];
				*pRes++ = col[0];
				*pRes++ = col[1];
				*pRes++ = col[2];
			}
			break;
		}
	} 
	else 
	{
		switch (pImageData->eColorMode) 
		{
		case colorModeMono1:
		case colorModeMono8:
			for ( nX = 0, pSrc = pImageData->pStream->GetNextLine(), pRes = pColorLine; nX < pImageData->nWidth; ++nX, pSrc += nComponentsCount ) 
			{
				GrGray oGray;
				pImageData->pColorMap->GetGray( pSrc, &oGray );
				*pRes++ = ColorToByte( oGray );
			}
			break;
		case colorModeRGB8:
		case colorModeBGR8:
			for ( nX = 0, pSrc = pImageData->pStream->GetNextLine(), pRes = pColorLine; nX < pImageData->nWidth; ++nX, pSrc += nComponentsCount ) 
			{
				GrRGB oRGB;
				pImageData->pColorMap->GetRGB( pSrc, &oRGB );
				*pRes++ = ColorToByte( oRGB.r );
				*pRes++ = ColorToByte( oRGB.g );
				*pRes++ = ColorToByte( oRGB.b );
			}
			break;
		}
	}

	++pImageData->nY;
	return TRUE;
}

BOOL SImageOutputDev::AlphaImageSrc(void *pData, SColorPointer pColorLine, unsigned char *pAlphaLine) 
{
	ImageData *pImageData = (ImageData *)pData;

	if ( pImageData->nY == pImageData->nHeight ) 
	{
		return FALSE;
	}

	int nComponentsCount = pImageData->pColorMap->GetComponentsCount();

	int nX;
	unsigned char *pSrc, *pAlphaRes;
	SColorPointer pColorRes, pColor;
	for ( nX = 0, pSrc = pImageData->pStream->GetNextLine(), pColorRes = pColorLine, pAlphaRes = pAlphaLine; nX < pImageData->nWidth; ++nX, pSrc += nComponentsCount ) 
	{
		unsigned char unAlpha = 0;
		for ( int nIndex = 0; nIndex < nComponentsCount; ++nIndex ) 
		{
			if ( pSrc[nIndex] < pImageData->pMaskColors[2 * nIndex] || pSrc[nIndex] > pImageData->pMaskColors[2 * nIndex + 1] ) 
			{
				unAlpha = 0xff;
				break;
			}
		}
		
		GrGray oGray;
		GrRGB oRGB;
		if ( pImageData->pLookup ) 
		{
			switch ( pImageData->eColorMode ) 
			{
			case colorModeMono1:
			case colorModeMono8:
				*pColorRes++ = pImageData->pLookup[*pSrc];
				*pAlphaRes++ = unAlpha;
				break;
			case colorModeRGB8:
			case colorModeBGR8:
				pColor = &pImageData->pLookup[3 * *pSrc];
				*pColorRes++ = pColor[0];
				*pColorRes++ = pColor[1];
				*pColorRes++ = pColor[2];
				*pAlphaRes++ = unAlpha;
				break;
			}
		} 
		else 
		{
			switch ( pImageData->eColorMode ) 
			{
			case colorModeMono1:
			case colorModeMono8:
				pImageData->pColorMap->GetGray( pSrc, &oGray );
				*pColorRes++ = ColorToByte( oGray );
				*pAlphaRes++ = unAlpha;
				break;
			case colorModeRGB8:
			case colorModeBGR8:
				pImageData->pColorMap->GetRGB( pSrc, &oRGB );
				*pColorRes++ = ColorToByte( oRGB.r );
				*pColorRes++ = ColorToByte( oRGB.g );
				*pColorRes++ = ColorToByte( oRGB.b );
				*pAlphaRes++ = unAlpha;
				break;
			}
		}
	}

	++pImageData->nY;
	return TRUE;
}

BOOL SImageOutputDev::MaskedImageSrc(void *pData, SColorPointer pColorLine, unsigned char *pAlphaLine) 
{
	MaskedImageData *pImageData = (MaskedImageData *)pData;
	
	if ( pImageData->nY == pImageData->nHeight )
	{
		return FALSE;
	}

	int nComponentsCount = pImageData->pColorMap->GetComponentsCount();
	int nX;
	unsigned char *pSrc, *pAlphaRes;
	SColorPointer pColorRes, pColor;
	for ( nX = 0, pSrc = pImageData->pImageStream->GetNextLine(), pColorRes = pColorLine, pAlphaRes = pAlphaLine; nX < pImageData->nWidth; ++nX, pSrc += nComponentsCount ) 
	{
		SColor pMaskColor;
		pImageData->pMask->GetPixel( nX, pImageData->nY, pMaskColor );
		unsigned char unAlpha = pMaskColor[0] ? 0xff : 0x00;

		GrGray oGray;
		GrRGB oRGB;
		if ( pImageData->pLookup ) 
		{
			switch ( pImageData->eColorMode ) 
			{
			case colorModeMono1:
			case colorModeMono8:
				*pColorRes++ = pImageData->pLookup[*pSrc];
				*pAlphaRes++ = unAlpha;
				break;
			case colorModeRGB8:
			case colorModeBGR8:
				pColor = &pImageData->pLookup[3 * *pSrc];
				*pColorRes++ = pColor[0];
				*pColorRes++ = pColor[1];
				*pColorRes++ = pColor[2];
				*pAlphaRes++ = unAlpha;
				break;
			}
		} 
		else 
		{
			switch ( pImageData->eColorMode ) 
			{
			case colorModeMono1:
			case colorModeMono8:
				pImageData->pColorMap->GetGray( pSrc, &oGray );
				*pColorRes++ = ColorToByte( oGray );
				*pAlphaRes++ = unAlpha;
				break;
			case colorModeRGB8:
			case colorModeBGR8:
				pImageData->pColorMap->GetRGB( pSrc, &oRGB );
				*pColorRes++ = ColorToByte( oRGB.r );
				*pColorRes++ = ColorToByte( oRGB.g );
				*pColorRes++ = ColorToByte( oRGB.b );
				*pAlphaRes++ = unAlpha;
				break;
			}
		}
	}

	++pImageData->nY;
	return TRUE;
}

void SImageOutputDev::DrawImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, int *pMaskColors, BOOL bInlineImage) 
{
	double arrMatrix[6];
	double *pCTM = pGState->GetCTM();
	arrMatrix[0] =  pCTM[0];
	arrMatrix[1] =  pCTM[1];
	arrMatrix[2] = -pCTM[2];
	arrMatrix[3] = -pCTM[3];
	arrMatrix[4] =  pCTM[2] + pCTM[4];
	arrMatrix[5] =  pCTM[3] + pCTM[5];

	ImageData oImageData;
	oImageData.pStream = new ImageStream( pStream, nWidth, pColorMap->GetComponentsCount(), pColorMap->GetBitsPerComponent() );
	oImageData.pStream->Reset();
	oImageData.pColorMap   = pColorMap;
	oImageData.pMaskColors = pMaskColors;
	oImageData.eColorMode  = m_eColorMode;
	oImageData.nWidth      = nWidth;
	oImageData.nHeight     = nHeight;
	oImageData.nY          = 0;
	oImageData.pLookup     = NULL;

	if ( pColorMap->GetComponentsCount() == 1 ) 
	{
		int nBits = 1 << pColorMap->GetBitsPerComponent();
		switch ( m_eColorMode ) 
		{
		case colorModeMono1:
		case colorModeMono8:
			oImageData.pLookup = (SColorPointer)MemUtilsMalloc(nBits);
			for ( int nIndex = 0; nIndex < nBits; ++nIndex ) 
			{
				unsigned char unPixel = (unsigned char)nIndex;
				GrGray oGray;
				pColorMap->GetGray( &unPixel, &oGray );
				oImageData.pLookup[nIndex] = ColorToByte( oGray );
			}
			break;
		case colorModeRGB8:
		case colorModeBGR8:
			oImageData.pLookup = (SColorPointer)MemUtilsMalloc(3 * nBits);
			for ( int nIndex = 0; nIndex < nBits; ++nIndex ) 
			{
				unsigned char unPixel = (unsigned char)nIndex;
				GrRGB oRGB;
				pColorMap->GetRGB( &unPixel, &oRGB );
				oImageData.pLookup[3 * nIndex + 0] = ColorToByte( oRGB.r );
				oImageData.pLookup[3 * nIndex + 1] = ColorToByte( oRGB.g );
				oImageData.pLookup[3 * nIndex + 2] = ColorToByte( oRGB.b );
			}
			break;
		}
	}

	SColorMode eSrcMode;
	if ( m_eColorMode == colorModeMono1 ) 
	{
		eSrcMode = colorModeMono8;
	} 
	else 
	{
		eSrcMode = m_eColorMode;
	}
	SImageSource pImageSrc = pMaskColors ? &AlphaImageSrc : &ImageSrc;
	m_pSImage->DrawImage( pImageSrc, &oImageData, eSrcMode, pMaskColors ? TRUE : FALSE, nWidth, nHeight, arrMatrix );
	if ( bInlineImage ) 
	{
		while ( oImageData.nY < nHeight ) 
		{
			oImageData.pStream->GetNextLine();
			++oImageData.nY;
		}
	}

	MemUtilsFree( oImageData.pLookup );
	delete oImageData.pStream;
	pStream->Close();
}

void SImageOutputDev::DrawMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, BOOL bMaskInvert) 
{
	// Если у макси разрешение больше, чем у самого изображения, тогда используем функцию DrawSoftMaskedImage().
	if ( nMaskWidth > nWidth || nMaskHeight > nHeight ) 
	{
		Object oDecodeLow, oDecodeHigh, oMaskDecode;
		oDecodeLow.InitInt( bMaskInvert ? 0 : 1 );
		oDecodeHigh.InitInt( bMaskInvert ? 1 : 0 );
		oMaskDecode.InitArray( m_pXref );
		oMaskDecode.ArrayAdd( &oDecodeLow );
		oMaskDecode.ArrayAdd( &oDecodeHigh );
		GrImageColorMap *pMaskColorMap = new GrImageColorMap( 1, &oMaskDecode, new GrDeviceGrayColorSpace() );
		oMaskDecode.Free();
		DrawSoftMaskedImage( pGState, pRef, pStream, nWidth, nHeight, pColorMap, pMaskStream, nMaskWidth, nMaskHeight, pMaskColorMap );
		delete pMaskColorMap;
	} 
	else 
	{
		// Изменяем размеры макси до зразмеров исходной картинки.
		double arrMatrix[6];
		arrMatrix[0] = (double)nWidth;
		arrMatrix[1] = 0;
		arrMatrix[2] = 0;
		arrMatrix[3] = (double)nHeight;
		arrMatrix[4] = 0;
		arrMatrix[5] = 0;

		ImageMaskData oImageMaskData;
		oImageMaskData.pImageStream = new ImageStream( pMaskStream, nMaskWidth, 1, 1);
		oImageMaskData.pImageStream->Reset();
		oImageMaskData.bInvert = bMaskInvert ? 0 : 1;
		oImageMaskData.nWidth  = nMaskWidth;
		oImageMaskData.nHeight = nMaskHeight;
		oImageMaskData.nY      = 0;

		SBitmap *pMaskBitmap = new SBitmap( nWidth, nHeight, 1, colorModeMono1, FALSE);
		SImage  *pMaskSImage = new SImage( pMaskBitmap, FALSE );
		SColor pMaskColor;
		pMaskColor[0] = 0;
		pMaskSImage->Clear( pMaskColor );
		pMaskColor[0] = 0xff;
		pMaskSImage->SetFillPattern( new SSolidColor( pMaskColor ) );
		pMaskSImage->FillImageMask( &ImageMaskSrc, &oImageMaskData, nMaskWidth, nMaskHeight, arrMatrix, FALSE );
		delete oImageMaskData.pImageStream;
		pMaskStream->Close();
		delete pMaskSImage;

		// Рисуем исходную картинку

		double *pCTM = pGState->GetCTM();
		arrMatrix[0] =  pCTM[0];
		arrMatrix[1] =  pCTM[1];
		arrMatrix[2] = -pCTM[2];
		arrMatrix[3] = -pCTM[3];
		arrMatrix[4] =  pCTM[2] + pCTM[4];
		arrMatrix[5] =  pCTM[3] + pCTM[5];

		MaskedImageData oImageData;
		oImageData.pImageStream = new ImageStream( pStream, nWidth, pColorMap->GetComponentsCount(), pColorMap->GetBitsPerComponent() );
		oImageData.pImageStream->Reset();
		oImageData.pColorMap  = pColorMap;
		oImageData.pMask      = pMaskBitmap;
		oImageData.eColorMode = m_eColorMode;
		oImageData.nWidth     = nWidth;
		oImageData.nHeight    = nHeight;
		oImageData.nY         = 0;

		// Специальный случай для одноканальных изображений: строим таблицу поиска здесь
		oImageData.pLookup = NULL;
		if ( pColorMap->GetComponentsCount() == 1 ) 
		{
			int nBitsCount = 1 << pColorMap->GetBitsPerComponent();
			switch ( m_eColorMode ) 
			{
			case colorModeMono1:
			case colorModeMono8:
				oImageData.pLookup = (SColorPointer)MemUtilsMalloc( nBitsCount );
				for ( int nIndex = 0; nIndex < nBitsCount; ++nIndex ) 
				{
					unsigned char unPixel = (unsigned char)nIndex;
					GrGray oGray;
					pColorMap->GetGray( &unPixel, &oGray );
					oImageData.pLookup[nIndex] = ColorToByte( oGray );
				}
				break;
			case colorModeRGB8:
			case colorModeBGR8:
				oImageData.pLookup = (SColorPointer)MemUtilsMalloc(3 * nBitsCount);
				for ( int nIndex = 0; nIndex < nBitsCount; ++nIndex ) 
				{
					unsigned char unPixel = (unsigned char)nIndex;
					GrRGB oRGB;
					pColorMap->GetRGB( &unPixel, &oRGB );
					oImageData.pLookup[3 * nIndex + 0] = ColorToByte( oRGB.r );
					oImageData.pLookup[3 * nIndex + 1] = ColorToByte( oRGB.g );
					oImageData.pLookup[3 * nIndex + 2] = ColorToByte( oRGB.b );
				}
				break;
			}
		}

		SColorMode eSourceMode;
		if ( m_eColorMode == colorModeMono1 ) 
		{
			eSourceMode = colorModeMono8;
		} 
		else 
		{
			eSourceMode = m_eColorMode;
		}
		m_pSImage->DrawImage( &MaskedImageSrc, &oImageData, eSourceMode, TRUE, nWidth, nHeight, arrMatrix ); 

		delete pMaskBitmap;
		MemUtilsFree( oImageData.pLookup );
		delete oImageData.pImageStream;
		pStream->Close();
	}
}

void SImageOutputDev::DrawSoftMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GrImageColorMap *pMaskColorMap) 
{
	double *pCTM = pGState->GetCTM();
	double arrMatrix[6];
	arrMatrix[0] =  pCTM[0];
	arrMatrix[1] =  pCTM[1];
	arrMatrix[2] = -pCTM[2];
	arrMatrix[3] = -pCTM[3];
	arrMatrix[4] =  pCTM[2] + pCTM[4];
	arrMatrix[5] =  pCTM[3] + pCTM[5];

	ImageData oImageMaskData;
	oImageMaskData.pStream = new ImageStream( pMaskStream, nMaskWidth, pMaskColorMap->GetComponentsCount(), pMaskColorMap->GetBitsPerComponent() );	
	oImageMaskData.pStream->Reset();
	oImageMaskData.pColorMap   = pMaskColorMap;
	oImageMaskData.pMaskColors = NULL;
	oImageMaskData.eColorMode  = colorModeMono8;
	oImageMaskData.nWidth      = nMaskWidth;
	oImageMaskData.nHeight     = nMaskHeight;
	oImageMaskData.nY          = 0;

	int nBitsCount = 1 << pMaskColorMap->GetBitsPerComponent();
	oImageMaskData.pLookup = (SColorPointer)MemUtilsMalloc(nBitsCount);
	for ( int nIndex = 0; nIndex < nBitsCount; ++nIndex )
	{
		unsigned char unPixel = (unsigned char)nIndex;
		GrGray oGray;
		pMaskColorMap->GetGray( &unPixel, &oGray );
		oImageMaskData.pLookup[nIndex] = ColorToByte( oGray );
	}
	SBitmap *pMaskBitmap = new SBitmap( m_pBitmap->GetWidth(), m_pBitmap->GetHeight(), 1, colorModeMono8, FALSE );
	SImage  *pMaskSImage = new SImage( pMaskBitmap, m_bVectorAA );
	SColor pMaskColor;
	pMaskColor[0] = 0;
	pMaskSImage->Clear( pMaskColor );
	pMaskSImage->DrawImage( &ImageSrc, &oImageMaskData, colorModeMono8, FALSE, nMaskWidth, nMaskHeight, arrMatrix );
	delete oImageMaskData.pStream;
	pMaskStream->Close();
	MemUtilsFree( oImageMaskData.pLookup );
	delete pMaskSImage;
	m_pSImage->SetSoftMask( pMaskBitmap );

	ImageData oImageData;
	oImageData.pStream = new ImageStream( pStream, nWidth, pColorMap->GetComponentsCount(), pColorMap->GetBitsPerComponent() );
	oImageData.pStream->Reset();
	oImageData.pColorMap   = pColorMap;
	oImageData.pMaskColors = NULL;
	oImageData.eColorMode  = m_eColorMode;
	oImageData.nWidth      = nWidth;
	oImageData.nHeight     = nHeight;
	oImageData.nY          = 0;
	oImageData.pLookup     = NULL;
	if ( pColorMap->GetComponentsCount() == 1 ) 
	{
		nBitsCount = 1 << pColorMap->GetBitsPerComponent();
		switch ( m_eColorMode ) 
		{
		case colorModeMono1:
		case colorModeMono8:
			oImageData.pLookup = (SColorPointer)MemUtilsMalloc(nBitsCount);
			for ( int nIndex = 0; nIndex < nBitsCount; ++nIndex ) 
			{
				unsigned char unPixel = (unsigned char)nIndex;
				GrGray oGray;
				pColorMap->GetGray( &unPixel, &oGray );
				oImageData.pLookup[nIndex] = ColorToByte( oGray );
			}
			break;
		case colorModeRGB8:
		case colorModeBGR8:
			oImageData.pLookup = (SColorPointer)MemUtilsMalloc(3 * nBitsCount);
			for ( int nIndex = 0; nIndex < nBitsCount; ++nIndex ) 
			{
				unsigned char unPixel = (unsigned char)nIndex;
				GrRGB oRGB;
				pColorMap->GetRGB( &unPixel, &oRGB );
				oImageData.pLookup[3 * nIndex + 0] = ColorToByte( oRGB.r );
				oImageData.pLookup[3 * nIndex + 1] = ColorToByte( oRGB.g );
				oImageData.pLookup[3 * nIndex + 2] = ColorToByte( oRGB.b );
			}
			break;
		}
	}

	SColorMode eSourceMode;
	if ( m_eColorMode == colorModeMono1 ) 
	{
		eSourceMode = colorModeMono8;
	} 
	else 
	{
		eSourceMode = m_eColorMode;
	}
	m_pSImage->DrawImage( &ImageSrc, &oImageData, eSourceMode, FALSE, nWidth, nHeight, arrMatrix );

	m_pSImage->SetSoftMask(NULL);
	MemUtilsFree( oImageData.pLookup );
	delete oImageData.pStream;
	pStream->Close();
}

void SImageOutputDev::BeginTransparencyGroup(GrState *pGState, double *pBBox, GrColorSpace *pBlendingColorSpace, BOOL bIsolated, BOOL bKnockout, BOOL bForSoftMask) 
{
	double dX, dY;
	pGState->Transform( pBBox[0], pBBox[1], &dX, &dY );
	double dMinX = dX, dMaxX = dX;
	double dMinY = dY, dMaxY = dY;
	pGState->Transform( pBBox[0], pBBox[3], &dX, &dY );
	if ( dX < dMinX ) 
	{
		dMinX = dX;
	} 
	else if ( dX > dMaxX ) 
	{
		dMaxX = dX;
	}
	if ( dY < dMinY ) 
	{
		dMinY = dY;
	} 
	else if ( dY > dMaxY ) 
	{
		dMaxY = dY;
	}
	pGState->Transform( pBBox[2], pBBox[1], &dX, &dY );
	if ( dX < dMinX ) 
	{
		dMinX = dX;
	} 
	else if ( dX > dMaxX ) 
	{
		dMaxX = dX;
	}
	if ( dY < dMinY ) 
	{
		dMinY = dY;
	} 
	else if ( dY > dMaxY ) 
	{
		dMaxY = dY;
	}
	pGState->Transform( pBBox[2], pBBox[3], &dX, &dY );
	if ( dX < dMinX ) 
	{
		dMinX = dX;
	} 
	else if ( dX > dMaxX ) 
	{
		dMaxX = dX;
	}
	if ( dY < dMinY ) 
	{
		dMinY = dY;
	} 
	else if ( dY > dMaxY ) 
	{
		dMaxY = dY;
	}
	int nTransX = (int)floor( dMinX );
	if ( nTransX < 0 ) 
	{
		nTransX = 0;
	} 
	else if ( nTransX > m_pBitmap->GetWidth() ) 
	{
		nTransX = m_pBitmap->GetWidth();
	}
	int nTransY = (int)floor( dMinY );
	if ( nTransY < 0 ) 
	{
		nTransY = 0;
	} 
	else if ( nTransY > m_pBitmap->GetHeight() ) 
	{
		nTransY = m_pBitmap->GetHeight();
	}
	int nWidth = (int)ceil( dMaxX ) - nTransX + 1;
	if ( nTransX + nWidth > m_pBitmap->GetWidth() ) 
	{
		nWidth = m_pBitmap->GetWidth() - nTransX;
	}
	if ( nWidth < 1 ) 
	{
		nWidth = 1;
	}
	int nHeight = (int)ceil( dMaxY ) - nTransY + 1;
	if ( nTransY + nHeight > m_pBitmap->GetHeight() ) 
	{
		nHeight = m_pBitmap->GetHeight() - nTransY;
	}
	if ( nHeight < 1 ) 
	{
		nHeight = 1;
	}

	// Создаем новый элемент в стеке
	TransparencyGroup *pTranspGroup;
	pTranspGroup = new TransparencyGroup();
	pTranspGroup->nTransX          = nTransX;
	pTranspGroup->nTransY          = nTransY;
	pTranspGroup->pBlendColorSpace = pBlendingColorSpace;
	pTranspGroup->bIsolated        = bIsolated;
	pTranspGroup->pNext            = m_pTranspGroupStack;
	m_pTranspGroupStack = pTranspGroup;

	// Сохраняем состояние
	pTranspGroup->pOriginalBitmap = m_pBitmap;
	pTranspGroup->pOriginalSImage = m_pSImage;

	m_pBitmap = new SBitmap( nWidth, nHeight, m_nBitmapRowPad, m_eColorMode, TRUE, m_bBitmapTopDown ); 
	m_pSImage = new SImage( m_pBitmap, m_bVectorAA, pTranspGroup->pOriginalSImage->GetScreen() );
	if ( bIsolated ) 
	{
		SColor pColor;
		switch ( m_eColorMode ) 
		{
		case colorModeMono1:
		case colorModeMono8:
			pColor[0] = 0;
			break;
		case colorModeRGB8:
		case colorModeBGR8:
			pColor[0] = pColor[1] = pColor[2] = 0;
			break;
		default:
			break;
		}
		m_pSImage->Clear( pColor, 0 );
	} 
	else 
	{
		m_pSImage->BlitTransparent( pTranspGroup->pOriginalBitmap, nTransX, nTransY, 0, 0, nWidth, nHeight );
		m_pSImage->SetInNonIsolatedGroup( pTranspGroup->pOriginalBitmap, nTransX, nTransY );
	}
	pTranspGroup->pBitmap = m_pBitmap;
	pGState->ShiftCTM( -nTransX, -nTransY );
	UpdateCTM( pGState, 0, 0, 0, 0, 0, 0 );
}

void SImageOutputDev::EndTransparencyGroup(GrState *pGState) 
{
	// Восстанавливаем предыдущее состояние
	delete m_pSImage;
	m_pBitmap = m_pTranspGroupStack->pOriginalBitmap;
	m_pSImage = m_pTranspGroupStack->pOriginalSImage;
	double *pCTM = pGState->GetCTM();
	pGState->ShiftCTM( m_pTranspGroupStack->nTransX, m_pTranspGroupStack->nTransY );
	UpdateCTM( pGState, 0, 0, 0, 0, 0, 0 );
}

void SImageOutputDev::PaintTransparencyGroup(GrState *pGState, double *pBBox) 
{
	int nTransX = m_pTranspGroupStack->nTransX;
	int nTransY = m_pTranspGroupStack->nTransY;
	SBitmap *pTempBitmap = m_pTranspGroupStack->pBitmap;
	BOOL bIsolated = m_pTranspGroupStack->bIsolated;

	m_pSImage->Composite( pTempBitmap, 0, 0, nTransX, nTransY, pTempBitmap->GetWidth(), pTempBitmap->GetHeight(), FALSE, !bIsolated );

	// Удаляем элемент из стека
	TransparencyGroup *pTranspGroup = m_pTranspGroupStack;
	m_pTranspGroupStack = pTranspGroup->pNext;
	delete pTranspGroup;
	delete pTempBitmap;
}

void SImageOutputDev::SetSoftMask(GrState *pGState, double *pBBox, BOOL bAlpha, Function *pTransferFunc, GrColor *pBackdropColor) 
{
	int nTransX = m_pTranspGroupStack->nTransX;
	int nTransY = m_pTranspGroupStack->nTransY;
	SBitmap *pTBitmap = m_pTranspGroupStack->pBitmap;

	// Совмещаем с подкладкой (pBackdropColor)
	if ( !bAlpha && m_eColorMode != colorModeMono1 ) 
	{
		SImage *pTSImage = new SImage( pTBitmap, m_bVectorAA, m_pTranspGroupStack->pOriginalSImage->GetScreen() );
		SColor pColor;
		if ( m_pTranspGroupStack->pBlendColorSpace ) 
		{
			GrGray oGray;
			GrRGB oRGB;
			switch ( m_eColorMode ) 
			{
			case colorModeMono1:
				// Прозрачность не поддерживается в случае Mono1
				break;
			case colorModeMono8:
				m_pTranspGroupStack->pBlendColorSpace->GetGray( pBackdropColor, &oGray );
				pColor[0] = ColorToByte( oGray );
				pTSImage->CompositeBackground( pColor );
				break;
			case colorModeRGB8:
			case colorModeBGR8:
				m_pTranspGroupStack->pBlendColorSpace->GetRGB( pBackdropColor, &oRGB );
				pColor[0] = ColorToByte( oRGB.r );
				pColor[1] = ColorToByte( oRGB.g );
				pColor[2] = ColorToByte( oRGB.b );
				pTSImage->CompositeBackground( pColor );
				break;
			}
			delete pTSImage;
		}
	}

	SBitmap *pSoftMask = new SBitmap( m_pBitmap->GetWidth(), m_pBitmap->GetHeight(), 1, colorModeMono8, FALSE );
	memset( pSoftMask->GetData(), 0, pSoftMask->GetStride() * pSoftMask->GetHeight() );
	SColorPointer pLine = pSoftMask->GetData() + nTransY * pSoftMask->GetStride() + nTransX;
	for ( int nY = 0; nY < pTBitmap->GetHeight(); ++nY ) 
	{
		for ( int nX = 0; nX < pTBitmap->GetWidth(); ++nX )  
		{
			SColor pColor;
			pTBitmap->GetPixel( nX, nY, pColor );
			if ( bAlpha ) 
			{
				// Пока не поддерживается
			} 
			else 
			{
				double dLuminosity, dLuminosity2;
				switch ( m_eColorMode ) 
				{
				case colorModeMono1:
				case colorModeMono8:
					dLuminosity = pColor[0] / 255.0;
					break;
				case colorModeRGB8:
				case colorModeBGR8:
					dLuminosity = (0.3 / 255.0) * pColor[0] + (0.59 / 255.0) * pColor[1] + (0.11 / 255.0) * pColor[2];
					break;
				}
				if ( pTransferFunc ) 
				{
					pTransferFunc->Transform( &dLuminosity, &dLuminosity2 );
				} 
				else 
				{
					dLuminosity2 = dLuminosity;
				}
				pLine[nX] = (int)(dLuminosity2 * 255.0 + 0.5);
			}
		}
		pLine += pSoftMask->GetStride();
	}
	m_pSImage->SetSoftMask( pSoftMask );

	// Удаляем элемент из стека
	TransparencyGroup *pTranspGroup = m_pTranspGroupStack;
	m_pTranspGroupStack = pTranspGroup->pNext;
	delete pTranspGroup;

	delete pTBitmap;
}

void SImageOutputDev::ClearSoftMask(GrState *pGState) 
{
	m_pSImage->SetSoftMask(NULL);
}

void SImageOutputDev::SetPaperColor(SColorPointer pPaperColor) 
{
	ColorCopy( m_pPaperColor, pPaperColor );
}

int SImageOutputDev::GetBitmapWidth() 
{
	return m_pBitmap->GetWidth();
}

int SImageOutputDev::GetBitmapHeight() 
{
	return m_pBitmap->GetHeight();
}

SBitmap *SImageOutputDev::TakeBitmap() 
{
	SBitmap *m_pRet = m_pBitmap;
	m_pBitmap = new SBitmap( 1, 1, m_nBitmapRowPad, m_eColorMode, m_eColorMode != colorModeMono1, m_bBitmapTopDown );
	return m_pRet;
}

void SImageOutputDev::GetModRegion(int *pnMinX, int *pnMinY, int *pnMaxX, int *pnMaxY) 
{
	m_pSImage->GetModRegion( pnMinX, pnMinY, pnMaxX, pnMaxY );
}

void SImageOutputDev::ClearModRegion() 
{
	m_pSImage->ClearModRegion();
}

void SImageOutputDev::SetFillColor(int nR, int nG, int nB) 
{
	GrRGB oRGB;
	GrGray oGray;

	oRGB.r = ByteToColor( nR );
	oRGB.g = ByteToColor( nG );
	oRGB.b = ByteToColor( nB );
	oGray = (GrColorComp)(0.299 * oRGB.r + 0.587 * oRGB.g + 0.114 * oRGB.g + 0.5);
	if ( oGray > GrColorComp1 )
	{
		oGray = GrColorComp1;
	}
	m_pSImage->SetFillPattern( GetColor( oGray, &oRGB ) );
}

SFont *SImageOutputDev::GetFont(StringExt *seName, double *pTextMatrix) 
{
	int nIndex;
	for ( nIndex = 0; nIndex < 16; ++nIndex ) 
	{
		if ( !seName->Compare( c_arrSImageOutSubstFonts[nIndex].sName ) ) 
		{
			break;
		}
	}
	if ( nIndex == 16 ) 
	{
		return NULL;
	}
	Ref oRef;
	oRef.nNum = nIndex;
	oRef.nGen = -1;
	SImageOutFontFileID *pID = new SImageOutFontFileID( &oRef );

	// Сначала ищем в кэше
	SFontFile *pFontFile = NULL;
	if ( ( pFontFile = m_pFontEngine->GetFontFile( pID ) ) ) 
	{
		delete pID;
	} 
	else 
	{
		DisplayFontParam *pDisplayFontParam = NULL;
		if ( m_pGlobalParams )
		{
			pDisplayFontParam = m_pGlobalParams->GetDisplayFont( seName );
		}
		if ( pDisplayFontParam && pDisplayFontParam->m_eType == displayFontT1 ) 
		{
			pFontFile = m_pFontEngine->LoadType1Font( pID, pDisplayFontParam->m_oT1.pwsFileName->GetBuffer(), FALSE, c_arrWinAnsiEncoding );
		} 
		else if ( pDisplayFontParam && pDisplayFontParam->m_eType == displayFontTT ) 
		{
			CFontFileTrueType *pTTFontFile = NULL;
			if ( !( pTTFontFile = CFontFileTrueType::LoadFromFile( pDisplayFontParam->m_oTT.pwsFileName->GetBuffer() ) ) ) 
			{
				return NULL;
			}
			int nCMapIndex;
			for ( nCMapIndex = 0; nCMapIndex < pTTFontFile->GetCmapsCount(); ++nCMapIndex ) 
			{
				if ( ( pTTFontFile->GetCmapPlatform( nCMapIndex ) == 3 && pTTFontFile->GetCmapEncoding( nCMapIndex ) == 1) || pTTFontFile->GetCmapPlatform( nCMapIndex ) == 0 ) 
				{
					break;
				}
			}
			if ( nCMapIndex == pTTFontFile->GetCmapsCount() ) 
			{
				delete pTTFontFile;
				return NULL;
			}
			unsigned short *pCodeToGID = (unsigned short *)MemUtilsMallocArray( 256, sizeof(unsigned short) );
			for ( nIndex = 0; nIndex < 256; ++nIndex ) 
			{
				pCodeToGID[nIndex] = 0;
				Unicode nUnicode;
				if ( c_arrWinAnsiEncoding[nIndex] && m_pGlobalParams && ( nUnicode = m_pGlobalParams->MapNameToUnicode(c_arrWinAnsiEncoding[nIndex] ) ) ) 
				{
					pCodeToGID[nIndex] = pTTFontFile->MapCodeToGID( nCMapIndex, nUnicode );
				}
			}
			delete pTTFontFile;
			pFontFile = m_pFontEngine->LoadTrueTypeFont( pID, pDisplayFontParam->m_oTT.pwsFileName->GetBuffer(), FALSE, pCodeToGID, 256, m_pGlobalParams->GetTempFolder() );
		} 
		else 
		{
			return NULL;
		}
	}

	double arrTextMatrix[4];
	arrTextMatrix[0] = (double)pTextMatrix[0];
	arrTextMatrix[1] = (double)pTextMatrix[1];
	arrTextMatrix[2] = (double)pTextMatrix[2];
	arrTextMatrix[3] = (double)pTextMatrix[3];
	SFont *pFontObject = m_pFontEngine->GetFont( pFontFile, arrTextMatrix, m_pSImage->GetMatrix() );

	return pFontObject;
}

BOOL SImageOutputDev::GetVectorAntialias() 
{
	return m_pSImage->GetVectorAA();
}

void SImageOutputDev::SetVectorAntialias(BOOL vaa) 
{
	m_pSImage->SetVectorAA(vaa);
}
