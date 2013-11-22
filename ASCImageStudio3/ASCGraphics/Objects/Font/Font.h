#ifndef _FONT_H
#define _FONT_H

#include "GlyphBitmap.h"
#include "FontConsts.h"

struct TFontCacheTag;
class CFontFile;
class CFontPath;
class CGlyphString;

//-------------------------------------------------------------------------------------------------------------------------------
// CFont
//-------------------------------------------------------------------------------------------------------------------------------

class CFont 
{
public:

	CFont(CFontFile *pFontFile, BOOL bAntiAliasing, BOOL bUseKerning);

	// Функцию надо вызыватьп осле конструктора, чтобы у конструктор подкласса мог вычислить BBox.
	void InitCache();
	void ClearCache();

	virtual ~CFont();

	CFontFile *GetFontFile() 
	{ 
		return m_pFontFile; 
	}

	// Проверяем идентичность по FontFile и матрицам.
	BOOL Matches(CFontFile *pFontFile, double *pFontMatrix, double *pTextMatrix) 
	{
		// TO DO: Изменить проверку по FontFile
		return ( pFontFile == m_pFontFile && 
			     pFontMatrix[0] == m_arrdFontMatrix[0] && pFontMatrix[1] == m_arrdFontMatrix[1] && pFontMatrix[2] == m_arrdFontMatrix[2] && pFontMatrix[3] == m_arrdFontMatrix[3] &&
				 pTextMatrix[0] == m_arrdTextMatrix[0] && pTextMatrix[1] == m_arrdTextMatrix[1] && pTextMatrix[2] == m_arrdTextMatrix[2] && pTextMatrix[3] == m_arrdTextMatrix[3] );
	}
	BOOL Matches(CFontFile *pFontFile) 
	{
		return ( pFontFile == m_pFontFile );
	}

	// Забираем символ (как bitmap) - сначала пытаемся найти его в кэше, если не нашли создаем новый Bitmap и добавляем его в кэш.
	// Величины nFracX и nFracY, представляют числитель некоторого значения из [0, 1), где знаменатель равен sfontFraction = 1 << sfontFractionBits.
	// Производные классы, не поддерживающие, рациональные коодринаты должны считать данное значение 0.
	virtual BOOL GetGlyph(int nCode, int nFracX, int nFracY, TGlyphBitmap *pBitmap);

	// Создаем из Glyph битмап. Значения nFracX и nFracY имеют тот же смысл, что и в GetGlyph.
	virtual BOOL MakeGlyph(int nCode, int nFracX, int nFracY, TGlyphBitmap *pBitmap) = 0;

	// Функция возвращает позиции символов в строке, начиная с точки (0;0)
	virtual BOOL GetString(CGlyphString *pString) = 0;

	// Функция возвращает позиции символов в строке, начиная с точки (0;0) и сразу считывает сами картинки букв
	virtual BOOL GetString2(CGlyphString *pString) = 0;

	// Функция возвращает CFontPath для данного символа.
	virtual CFontPath *GetGlyphPath(int nCode) = 0;

	// Функция устанавливет значение, являюсят ли элементы строки юникодными значениями или номерами глифов
	virtual void SetStringGID(BOOL bGID) = 0;

	// Функция возвращает значение, являюсят ли элементы строки юникодными значениями или номерами глифов
	virtual BOOL GetStringGID() = 0;

//----- Глобальные параметры шрифта ----------------------------------------------------
	// Функция возвращает Ascent в единицах шрифта
	virtual short GetAscender() = 0;

	// Функция возвращает Descent в единицах шрифта
	virtual short GetDescender() = 0;

	// Функция возвращает высоту буквы 'M' в единицах шрифта
	virtual unsigned short GetUnitsPerEm() = 0;

	// Функция возвращает расстояние между линиями в единицах шрифта
	virtual short GetLineSpacing() = 0;

	// Функция возвращает название семейства шрифта
	virtual char * GetFamilyName() = 0;

	// Функция вовзаращает количество шрифтов, хранящихся в файле
	virtual long GetFacesCount() = 0;

	// Функция возвращает номер текущего шрифта в файле
	virtual long GetFaceIndex() = 0;

	// Функция возвращает количество глифов в файле
	virtual long GetGlyphsCount() = 0;

	// Функция возвращает стиль шрифта в текстовой форме ("Regular", "Bold" и т.д.)
	virtual char *GetStyleName() = 0;

	// Функция возвращает позицию линии для подчеркивания (относительно BaseLine)
	virtual short GetUnderlinePosition() = 0;

	// Функция возвращает толщину линии подчеркивания
	virtual short GetUnderlineThickness() = 0;

	// Функция возвращает максимальное расстояние по горизонтали между начальными точками рисования двух соседних символов
	virtual short GetMaxAdvanceWidth() = 0;

	// Функция возвращает максимальное расстояние по вертикали между начальными точками рисования двух соседних символов
	virtual short GetMaxAdvanceHeight() = 0;

	// Функция возвращает наименьший прямоугольник, в котором можно нарисовать любую букву из шрифта
	virtual void GetBBox(long *plMinX, long *plMinY, long *plMaxX, long *plMaxY) = 0;

	// Функция возвращает номер глифа по его имени
	virtual unsigned short GetNameIndex(char *sName) = 0;

//--------------------------------------------------------------------------------------

	// Функция возвращает true, если символ с юникодным значением lUnicode присутствует в шрифте(в каком-нибудь CMap)
	virtual bool IsCharAvailable(long lUnicode) = 0;

	// Устанавливаем Dpi
	virtual void SetSizeAndDpi(float fSize, unsigned int unHorDpi, unsigned int unVerDpi) = 0;

	// Функция возвращает тип шрифта
	virtual const char *GetFontFormat() = 0;

	// Функция проверяет присутствует ли данные Unicode Range в шрифте или CodeRange(если параметр un4ByteIndex равен 4 или 5)
	virtual int IsUnicodeRangeAvailable(unsigned long ulBit, unsigned int un4ByteIndex) = 0;

	// Функция возвращает номер кода по номеру глифа
	virtual unsigned long GetCodeByGID(unsigned short ushGID) = 0;

	// Функция возвращает panose (в таблице OS/2)
	virtual void GetPanose(char **ppPanose) = 0;

	// Функция проверяем является ли данный шрифт моноширинным
	virtual bool IsFixedWidth() = 0;
//--------------------------------------------------------------------------------------

	// Сбрасываем матрицу преобразований к единичной
	virtual void ResetFontMatrix() = 0;

	// Сбрасываем глобальную матрицу
	virtual void ResetTextMatrix() = 0;

	// Применяем преобразование (к матрице шрифта)
	virtual void ApplyTransform(float fA, float fB, float fC, float fD, float fE, float fF) = 0;

	// Устанавливаем матрицу шрифта
	virtual void SetFontMatrix(float fA, float fB, float fC, float fD, float fE, float fF) = 0;

	// Устанавливаем глобальную матрицу (общая матрица для всего текста)
	virtual void SetTextMatrix(float fA, float fB, float fC, float fD, float fE, float fF) = 0;

//--------------------------------------------------------------------------------------
// Поскольку не всегда шрифт имеется в нужном стиле, поэтому нужные элементы стиля 
// делаем вручную.
//--------------------------------------------------------------------------------------

	// Устанавливаем, нужно ли делать шрифт жирным(алгоритмически)
	void SetBold(BOOL bBold)
	{
		m_bNeedDoBold = bBold;
	};

	// Считываем, делается ли шрифт жирным
	BOOL GetBold()
	{
		return m_bNeedDoBold;
	}

	// Устанавливаем, нужно ли делать шрифт наклонным(алгоритмически)
	void SetItalic(BOOL bItalic)
	{
		m_bNeedDoItalic = bItalic;
		ResetFontMatrix();
	}

	// Считываем, делается ли шрифт наклонным
	BOOL GetItalic()
	{
		return m_bNeedDoItalic;
	}
//--------------------------------------------------------------------------------------

	// Устанавливаем стандартный шрифт
	void SetDefaultFont(CFont *pDefFont)
	{
		m_pDefaultFont = pDefFont;
	}


	// Функция возвращает стандартный шрифт
	CFont *GetDefaultFont()
	{
		return m_pDefaultFont;
	}

	void SetUseDefaultFont(BOOL bUse)
	{
		m_bUseDefaultFont = bUse;
	}

	BOOL GetUseDefaultFont()
	{
		return m_bUseDefaultFont;
	}


	// Функция фозвращает FontMatrix
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

	// Устанавливаем межсивольный интервал
	void SetCharSpacing(float fCharSpacing)
	{
		m_fCharSpacing = fCharSpacing;
	}

	// Получаем межсимвольный интервал
	float GetCharSpacing()
	{
		return m_fCharSpacing;
	}
//--------------------------------------------------------------------------------------

	float GetFontSize()
	{
		return m_fSize;
	}
	unsigned int GetVerDpi()
	{
		return m_unVerDpi;
	}

	unsigned int GetHorDpi()
	{
		return m_unHorDpi;
	}

protected:

	CFontFile     *m_pFontFile;
	double         m_arrdFontMatrix[6]; // FontMatrix (Text space -> Device space)
	double         m_arrdTextMatrix[6]; // TextMatrix (Text space -> User space)
	BOOL           m_bAntiAliasing;     // Anti-aliasing
	BOOL           m_bUseKerning;       // Использовать и доступен ли Kerning

	float          m_fSize;             // Размер шрифта
	unsigned int   m_unHorDpi;          // Горизонтальное разрешение
	unsigned int   m_unVerDpi;          // Вертикальное разрешение

	BOOL           m_bNeedDoItalic;     // Данный шрифт нужно сделать наклонным | Такое случается в 2-х случаях, либо мы грузим шрифт из файла,
	BOOL           m_bNeedDoBold;       // Данный шрифт нужно сделать жирным    | либо среди установленных шрифтов нет шрифта с нужным стилем.

	float          m_fCharSpacing;      // Межсимвольный интервал

	int            m_nMinX;        // 
	int            m_nMinY;        // Glyph BBox
	int            m_nMaxX;        //
	int            m_nMaxY;        //

	unsigned char *m_pCache;       // Glyph bitmap cache
	TFontCacheTag *m_pCacheTags;   // Cache Tags

	int            m_nGlyphWidth;  // Ширина Glyph bitmaps   
	int            m_nGlyphHeight; // Высота Glyph bitmaps
	int            m_nGlyphSize;   // Рзамер Glyph bitmaps в байтах

	int            m_nCacheSets;   // Количество Sets в кэше
	int            m_nCacheAssoc;  // Количество Glyphs в Set

	CFont         *m_pDefaultFont;    // Стандартный шрифт, который используется, если в исходном какие-то символы не рисуются
	BOOL           m_bUseDefaultFont; // Используем ли стандартный шрифт? Если нет, тогда когда нет символа рисуем стандартный символ
};

#endif /* _FONT_H */
