#ifndef _BUILD_FONT_ENGINE_FONTFILE_H_
#define _BUILD_FONT_ENGINE_FONTFILE_H_

// freetype
#include <ft2build.h>
#include FT_OUTLINE_H
#include FT_SIZES_H
#include FT_GLYPH_H
#include FT_TRUETYPE_IDS_H
#include FT_TRUETYPE_TABLES_H
#include FT_XFREE86_H
#include FT_ADVANCES_H

#include "FontPath.h"
#include "GlyphString.h"

//-------------------------------------------------------------------------------------------------------------------------------
// TODO: RasterHeep
//-------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------
// TODO: TFontCacheSizes
//-------------------------------------------------------------------------------------------------------------------------------
class TFontCacheSizes
{
public:
	int			ushUnicode; // Значение символа в юникоде
	EGlyphState eState;     // Есть ли символ в шрифте/стандартном шрифте
	int         nCMapIndex; // Номер таблицы 'cmap', в которой был найден данный символ 

	USHORT		ushGID;     

	float		fAdvanceX; 
	float       fAdvanceY;

	TBBox		oBBox;
	TMetrics	oMetrics;

	bool         bBitmap;
	TGlyphBitmap oBitmap;

public:
	TFontCacheSizes()
	{
		fAdvanceX = 0;
	}
	~TFontCacheSizes()
	{
	}
};

class CFontCacheSizes
{
public:
	CFontCacheSizes()
	{
	}
	~CFontCacheSizes()
	{
	}
	inline void Init()
	{
		m_arrSizes.RemoveAll();
	}
	inline void Clear(bool bIsFree = false)
	{
		if (bIsFree)
		{
			int nCount = m_arrSizes.GetCount();
			for (int i = 0; i < nCount; ++i)
			{
				m_arrSizes[i].oBitmap.bFreeData = TRUE;
			}
		}

		m_arrSizes.RemoveAll();
	}

	int Add(const TFontCacheSizes& oSizes)
	{
		m_arrSizes.Add( oSizes );
		return m_arrSizes.GetSize() - 1;
	}

	inline TFontCacheSizes& Get(int nIndex)
	{
		return m_arrSizes[nIndex];
	}
	const TFontCacheSizes& operator[] (int nIndex) const
	{
		return m_arrSizes[nIndex];
	}
	TFontCacheSizes& operator[] (int nIndex)
	{		
		return m_arrSizes[nIndex];
	}

private:
	CArray<TFontCacheSizes> m_arrSizes;
};

#define LOAD_MODE FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT | FT_LOAD_NO_BITMAP | FT_LOAD_LINEAR_DESIGN
#define REND_MODE FT_RENDER_MODE_NORMAL

#define FONT_ITALIC_ANGLE  0.3090169943749 // Синус 18 градусов (подбиралось под Word 2007)

#define MAX_UNICODE_VALUE 0x10FFFF
#define MAX_UNICODE_COUNT  0x110000
#define MAX_UNICODE_COUNT2 0x220000

#define FONT_CASHE_SIZES_SIZE 255
#define FONT_CACHE_SIZES_INDEXES_SIZE   MAX_UNICODE_COUNT
#define FONT_CACHE_SIZES_INDEXES_SIZE_2 MAX_UNICODE_COUNT2 // MAX_UNICODE_COUNT * sizeof(unsigned short)

class CFontStream;
class CFontManager;
class CFontFile
{
public:
	double m_arrdFontMatrix[6];
    double m_arrdTextMatrix[6];

    bool m_bAntiAliasing;
    bool m_bUseKerning;

    double m_dSize;			// Размер шрифта
    UINT m_unHorDpi;		// Горизонтальное разрешение
    UINT m_unVerDpi;		// Вертикальное разрешение

    INT m_bNeedDoItalic;
    INT m_bNeedDoBold;

    double m_dCharSpacing;

    int m_nMinX;		//
    int m_nMinY;        // Glyph BBox
    int m_nMaxX;        //
    int m_nMaxY;        //
    
	CFontStream*	m_pStream;
	std::wstring	m_sFileName;
    int				m_lFaceIndex;
	
	FT_Face m_pFace;
	
	double m_dUnitsKoef;
	int m_nDefaultChar;
	int m_nSymbolic;
	double m_dTextScale;

    INT m_bStringGID;
	
	FT_Matrix m_oFontMatrix;
	FT_Matrix m_oTextMatrix;
	
	int m_nNum_charmaps;

    int m_lAscender;
    int m_lDescender;
    int m_lLineHeight;
    int m_lUnits_Per_Em;

    // cache
	CFontCacheSizes m_oCacheSizes;
	// вот так экономим память. нету пока таких шрифтов, в которых глифов больше 0xFFFF
	USHORT m_arrCacheSizesIndexs[FONT_CACHE_SIZES_INDEXES_SIZE];

    INT		m_bUseDefaultFont;
	CFontFile*	m_pDefaultFont;

    INT		m_bIsNeedUpdateMatrix12;

	CFontManager* m_pFontManager;
    INT m_bHintsSupport;

public:

	CFontFile();
	~CFontFile();

	void SetDefaultFont(CFontFile* pDefFont);
	void LoadDefaultCharAndSymbolicCmapIndex();
	
	void ResetFontMatrix();	
	void ResetTextMatrix();

    void CheckTextMatrix();
	
	void UpdateMatrix0();
    void UpdateMatrix1();
    void UpdateMatrix2();
	
	void SetSizeAndDpi(double dSize, UINT unHorDpi, UINT unVerDpi);
	
	void ClearCache();
	void ClearCacheNoAttack(bool bIsFree = false);
    void Destroy();

	bool SetTextMatrix(const double& fA, const double& fB, const double& fC, const double fD, double fE, double fF);
    void SetFontMatrix(const double& fA, const double& fB, const double& fC, const double fD, double fE, double fF);
    
	INT GetString(CGlyphString& oString);
	INT GetString2(CGlyphString& oString);
    INT GetString2C(CGlyphString& oString);
	TFontCacheSizes GetChar(LONG lUnicode);

    bool AddToSizesCache(const TFontCacheSizes& oSizes);
	
	int SetCMapForCharCode(long lUnicode, int *pnCMapIndex);
	int SetCMapForCharCode2(long lUnicode);

    int GetKerning(FT_UInt unPrevGID, FT_UInt unGID);
    void SetStringGID(const INT& bGID);
    INT GetStringGID();
    void SetUseDefaultFont(const INT& bUse);
    INT GetUseDefaultFont();
    void SetCharSpacing(const double& dCharSpacing);
    double GetCharSpacing();
    std::string GetStyleName();
    void GetPanose(BYTE* pData);

    bool IsFixedWidth();

    int IsUnicodeRangeAvailable(unsigned long ulBit, unsigned int un4ByteIndex);
    
	void UpdateStyles(const INT& bBold, const INT& bItalic);
    
	void SetItalic(const INT& value);
    void SetNeedBold(const INT& value);
    
	int GetAscender();
    int GetDescender();
    int GetHeight();
    int Units_Per_Em();

    void CheckHintsSupport();

	std::wstring GetFontFormat() const;
	unsigned int GetNameIndex(const std::wstring& wsName) const;

	// path
	CFontPath* GetGlyphPath(int nCode);
};

//-------------------------------------------------------------------------------------------------------------------------------
struct TFreeTypeFontPath 
{
	CFontPath*	pPath;
	INT		bNeedClose;
};

static int GlyphPathMoveTo(const FT_Vector *pPoint, void *pPath);
static int GlyphPathLineTo(const FT_Vector *pPoint, void *pPath);
static int GlyphPathConicTo(const FT_Vector *pControlPoint, const FT_Vector *pEndPoint, void *pPath);
static int GlyphPathCubicTo(const FT_Vector *pFirstControlPoint, const FT_Vector *pSecondControlPoint, const FT_Vector *pEndPoint, void *pPath);

#endif // _BUILD_FONT_ENGINE_FONTFILE_H_
