#ifndef _FT_FONT_H
#define _FT_FONT_H

#include "Font.h"
#include "GlyphBitmap.h"

class CFreeTypeFontFile;
class CPath;

//-------------------------------------------------------------------------------------------------------------------------------

struct TFontCacheSizes
{
	unsigned short ushUnicode; // Значение символа в юникоде
	EGlyphState    eState;     // Есть ли символ в шрифте/стандартном шрифте
	int            nCMapIndex; // Номер таблицы 'cmap', в которой был найден данный символ 

	unsigned short ushGID;     

	float   fAdvanceX; 

	struct TBBox
	{ 
		float fMinX;
		float fMaxX;
		float fMinY;
		float fMaxY;
	} oBBox;

	struct TMetrics
	{
		float fWidth;
		float fHeight;

		float fHoriBearingX;
		float fHoriBearingY;
		float fHoriAdvance;

		float fVertBearingX;
		float fVertBearingY;
		float fVertAdvance;
	} oMetrics;

	bool         bBitmap;
	TGlyphBitmap oBitmap;
};

#define FONT_CASHE_SIZES_SIZE 255
#define FONT_CACHE_SIZES_INDEXES_SIZE   65536
#define FONT_CACHE_SIZES_INDEXES_SIZE_2 131072 // 65536 * sizeof(unsigned short)

//-------------------------------------------------------------------------------------------------------------------------------
// CFreeTypeFont
//-------------------------------------------------------------------------------------------------------------------------------

class CFreeTypeFont: public CFont 
{
public:

	CFreeTypeFont(CFreeTypeFontFile *pFontFile);

	virtual ~CFreeTypeFont();

	virtual BOOL GetGlyph(int nCode, int nFracX, int nFracY, TGlyphBitmap *pBitmap);

	virtual BOOL MakeGlyph(int nCode, int nFracX, int nFracY, TGlyphBitmap *pBitmap);

	virtual BOOL GetString(CGlyphString *pString);

	virtual BOOL GetString2(CGlyphString *pString);

	virtual CFontPath *GetGlyphPath(int nCode);

	virtual short GetAscender();

	virtual short GetDescender();

	virtual unsigned short GetUnitsPerEm();

	virtual short GetLineSpacing();

	virtual char * GetFamilyName();

	virtual long GetFacesCount();

	virtual long GetFaceIndex();

	virtual long GetGlyphsCount();

	virtual char *GetStyleName();

	virtual short GetUnderlinePosition();

	virtual short GetUnderlineThickness();

	virtual short GetMaxAdvanceWidth();

	virtual short GetMaxAdvanceHeight();

	virtual void GetBBox(long *plMinX, long *plMinY, long *plMaxX, long *plMaxY);

	virtual bool IsCharAvailable(long lUnicode);

	virtual void SetSizeAndDpi(float fSize, unsigned int unHorDpi, unsigned int unVerDpi);

	virtual const char *GetFontFormat();

	virtual int IsUnicodeRangeAvailable(unsigned long ulBit, unsigned int un4ByteIndex);

	virtual unsigned long GetCodeByGID(unsigned short ushGID);

	virtual void GetPanose(char **ppPanose);

	virtual bool IsFixedWidth();

	virtual unsigned short GetNameIndex(char *sName);

	virtual void SetStringGID(BOOL bGID);

	virtual BOOL GetStringGID();

	virtual void ResetFontMatrix();

	virtual void ResetTextMatrix();

	virtual void ApplyTransform(float fA, float fB, float fC, float fD, float fE, float fF);

	virtual void SetFontMatrix(float fA, float fB, float fC, float fD, float fE, float fF);

	virtual void SetTextMatrix(float fA, float fB, float fC, float fD, float fE, float fF);

	int GetSymbolic()
	{
		return m_nSymbolic;
	}

private:

	int SetCMapForCharCode(long lUnicode, int *pnCMapIndex);
	int SetCMapForCharCode2(long lUnicode);
	inline void UpdateMatrix0();
	inline void UpdateMatrix1();
	inline void UpdateMatrix2();
	void InitSizesCache();
	void ClearSizesCache();
	void AddToSizesCache(TFontCacheSizes oSizes);
	__forceinline int FindInSizesCache(int nCode);
	int GetDefaultChar();
	int GetSymbolicCmapIndex();

private:

	FT_Size   m_pSize;
	FT_Matrix m_oFontMatrix;
	FT_Matrix m_oTextMatrix;
	double    m_dTextScale;
	double    m_dUnitsKoef;
	BOOL      m_bStringGID;
	int       m_nDefaultChar; // -1 - не используется, > 0 - номер GID 
	int       m_nSymbolic;    // номер таблицы cmap = Если у числа ulCodePageRange установлен 31 бит, и таблица cmap поддерживает тип (3 0 (Symbol)); -1 = если нет

	class CFontCascheSizes
	{

	public:
		CFontCascheSizes()
		{

		}
		~CFontCascheSizes()
		{
		}
		void Init()
		{
			m_arrSizes.RemoveAll();
		}
		void Clear()
		{
			for ( int nIndex = 0; nIndex < m_arrSizes.GetSize(); nIndex++ )
			{
				unsigned char *pData = m_arrSizes.GetData()[nIndex].oBitmap.pData;
				if ( NULL != pData )
				{
					free( pData );
					pData = NULL;
				}
			}

			m_arrSizes.RemoveAll();
		}

		int Add(TFontCacheSizes oSizes)
		{
			m_arrSizes.Add( oSizes );
			return m_arrSizes.GetSize() - 1;
		}

		TFontCacheSizes Get(int nIndex)
		{
			return m_arrSizes.GetData()[nIndex];
		}


	private:

		CSimpleArray<TFontCacheSizes> m_arrSizes;
	} m_oCacheSizes;
	
	unsigned short m_arrCacheSizesIndexs[FONT_CACHE_SIZES_INDEXES_SIZE];


	friend class CFreeTypeFont;
};

#endif /* _FT_FONT_H */
