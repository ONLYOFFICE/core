#ifndef _SFONT_H
#define _SFONT_H

#include "STypes.h"

struct SGlyphBitmap;
struct SFontCacheTag;
class SFontFile;
class SPath;

//-------------------------------------------------------------------------------------------------------------------------------

#define sfontFractionBits 2
#define sfontFraction     (1 << sfontFractionBits)
#define sfontFractionMul  ((double)1 / (double)sfontFraction)

//-------------------------------------------------------------------------------------------------------------------------------
// SFont
//-------------------------------------------------------------------------------------------------------------------------------

class SFont 
{
public:

	SFont(SFontFile *pFontFile, double *pFontMatrix, double *pTextMatrix, BOOL bAntiAliasing);

	// Функцию надо вызыватьп осле конструктора, чтобы у конструктор подкласса мог вычислить BBox.
	void InitCache();

	virtual ~SFont();

	SFontFile *GetFontFile() 
	{ 
		return m_pFontFile; 
	}

	// Проверяем идентичность по FontFile и матрицам.
	BOOL Matches(SFontFile *pFontFile, double *pFontMatrix, double *pTextMatrix) 
	{
		return ( pFontFile == m_pFontFile && 
			     pFontMatrix[0] == m_arrdFontMatrix[0] && pFontMatrix[1] == m_arrdFontMatrix[1] && pFontMatrix[2] == m_arrdFontMatrix[2] && pFontMatrix[3] == m_arrdFontMatrix[3] &&
				 pTextMatrix[0] == m_arrdTextMatrix[0] && pTextMatrix[1] == m_arrdTextMatrix[1] && pTextMatrix[2] == m_arrdTextMatrix[2] && pTextMatrix[3] == m_arrdTextMatrix[3] );
	}

	// Забираем символ (как bitmap) - сначала пытаемся найти его в кэше, если не нашли создаем новый Bitmap и добавляем его в кэш.
	// Величины nFracX и nFracY, представляют числитель некоторого значения из [0, 1), где знаменатель равен sfontFraction = 1 << sfontFractionBits.
	// Производные классы, не поддерживающие, рациональные коодринаты должны считать данное значение 0.
	virtual BOOL GetGlyph(int nCode, int nFracX, int nFracY, SGlyphBitmap *pBitmap);

	// Создаем из Glyph битмап. Значения nFracX и nFracY имеют тот же смысл, что и в GetGlyph.
	virtual BOOL MakeGlyph(int nCode, int nFracX, int nFracY, SGlyphBitmap *pBitmap) = 0;

	// Функция фозвращает Path для данного символа.
	virtual SPath *GetGlyphPath(int nCode) = 0;

	// Функция фозвращает FontMatrix.
	double *GetMatrix() 
	{ 
		return m_arrdFontMatrix; 
	}

	// Функция фозвращает BBox для Glyph.
	void GetBBox(int *pnMinX, int *pnMinY, int *pnMaxX, int *pnMaxY)
	{ 
		*pnMinX = m_nMinX; 
		*pnMinY = m_nMinY; 
		*pnMaxX = m_nMaxX; 
		*pnMaxY = m_nMaxY; 
	}

protected:

	SFontFile     *m_pFontFile;
	double         m_arrdFontMatrix[4]; // FontMatrix (Text space -> Device space)
	double         m_arrdTextMatrix[4]; // TextMatrix (Text space -> User space)
	BOOL           m_bAntiAliasing;     // Anti-aliasing

	int            m_nMinX;        // 
	int            m_nMinY;        // Glyph BBox
	int            m_nMaxX;        //
	int            m_nMaxY;        //

	unsigned char *m_pCache;       // Glyph bitmap cache
	SFontCacheTag *m_pCacheTags;   // Cache Tags

	int            m_nGlyphWidth;  // Ширина Glyph bitmaps   
	int            m_nGlyphHeight; // Высота Glyph bitmaps
	int            m_nGlyphSize;   // Рзамер Glyph bitmaps в байтах

	int            m_nCacheSets;   // Количество Sets в кэше
	int            m_nCacheAssoc;  // Количество Glyphs в Set
};

#endif /* _SFONT_H */
