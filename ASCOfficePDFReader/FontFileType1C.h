#ifndef _FONT_FILE_TYPE1C_H
#define _FONT_FILE_TYPE1C_H

#include "FontFileBase.h"

class StringExt;

//------------------------------------------------------------------------

struct Type1CIndex 
{
	int nPos;         // Позиция в файле от начала файла
	int nCount;       // Количество вхождений
	int nOffsetSize;  // Offset size
	int nStartPos;    // Начальная позиция index data - 1
	int nEndPos;      // Позиция следующего байта после Type1CIndex
};

struct Type1CIndexVal 
{
	int nPos;         // Позиция в файле от начала файла
	int nLen;         // Длина в байтах
};

struct Type1CTopDict 
{
	int    nFirstOperator;

	int    nVersionSID;
	int    nNoticeSID;
	int    nCopyrightSID;
	int    nFullNameSID;
	int    nFamilyNameSID;
	int    nWeightSID;

	int    nIsFixedPitch;
	double dItalicAngle;
	double dUnderlinePosition;
	double dUnderlineThickness;
	
	int    nPaintType;
	int    nCharStringType;
	double arrdFontMatrix[6];
	BOOL   bHasFontMatrix;	// В CID фонтах возможно матрица фонта лежит в FD, а не в верхнем словаре
	int    nUniqueID;
	double arrdFontBBox[4];
	double dStrokeWidth;
	int    nCharsetOffset;
	int    nEncodingOffset;
	int    nCharStringsOffset;
	int    nPrivateSize;
	int    nPrivateOffset;

	// CIDFont entries
	int    nRegistrySID;
	int    nOrderingSID;
	int    nSupplement;
	int    nFDArrayOffset;
	int    nFDSelectOffset;
};



#define type1CMaxBlueValues 14
#define type1CMaxOtherBlues 10
#define type1CMaxStemSnap   12

struct Type1CPrivateDict 
{
	double arrdFontMatrix[6];
	BOOL   bHasFontMatrix;
	int    arrnBlueValues[type1CMaxBlueValues];
	int    nBlueValues;
	int    arrnOtherBlues[type1CMaxOtherBlues];
	int    nOtherBlues;
	int    arrnFamilyBlues[type1CMaxBlueValues];
	int    nFamilyBlues;
	int    arrnFamilyOtherBlues[type1CMaxOtherBlues];
	int    nFamilyOtherBlues;
	double dBlueScale;
	int    nBlueShift;
	int    nBlueFuzz;
	double dStdHW;
	BOOL   bHasStdHW;
	double dStdVW;
	BOOL   bHasStdVW;
	double arrdStemSnapH[type1CMaxStemSnap];
	int    nStemSnapH;
	double arrdStemSnapV[type1CMaxStemSnap];
	int    nStemSnapV;
	BOOL   bForceBold;
	BOOL   bHasForceBold;
	double dForceBoldThreshold;
	int    nLanguageGroup;
	double dExpansionFactor;
	int    nInitialRandomSeed;
	int    nSubrsOffset;
	double dDefaultWidthX;
	BOOL   bDefaultWidthXFP;
	double dNominalWidthX;
	BOOL   bNominalWidthXFP;
};

struct Type1COperator 
{
	BOOL bIsNumber;   // true -> number, false -> operator
	BOOL bIsFloat;    // true -> floating point number, false -> int
	union 
	{
		double dNumber;
		int    nOperator;
	};
};

struct Type1CEexecBuf 
{
	FontFileOutputFunc pOutputFunc;
	void              *pOutputStream;
	BOOL               bASKII;          // ASCII кодировка?
	unsigned short     unEncryptionKey; // eexec encryption key
	int                nLine;           // количество eexec-символов, оставшихся на текущей строке
};

//------------------------------------------------------------------------
// CFontFileType1C
//------------------------------------------------------------------------

class CFontFileType1C: public CFontFileBase 
{

public:

	static CFontFileType1C *LoadFromBuffer(char *sBuffer, int nLen);

	static CFontFileType1C *LoadFromFile(wchar_t *wsFileName);

	virtual ~CFontFileType1C();

	char *GetName();

	// Возвращаем кодировку, как массив 256 имен (некоторые могут быть 
    // NULL). Используется только для 8-битных фонтов.
	char **GetEncoding();

	unsigned short *GetCIDToGIDMap(int *arrCIDs);

	// Convert to a Type 1 font, suitable for embedding in a PostScript
	// file.  This is only useful with 8-bit fonts.  If <newEncoding> is
	// not NULL, it will be used in place of the encoding in the Type 1C
	// font.  If <ascii> is true the eexec section will be hex-encoded,
	// otherwise it will be left as binary data.  If <psName> is non-NULL,
	// it will be used as the PostScript font name.
	void ToType1(char *sPSName, char **ppNewEncoding, BOOL bASKII, FontFileOutputFunc pOutputFunc, void *pOutputStream);

	// Convert to a Type 0 CIDFont, suitable for embedding in a
	// PostScript file.  <psName> will be used as the PostScript font
	// name.
	void ToCIDType0(char *sPSName, FontFileOutputFunc pOutputFunc, void *pOutputStream);

	// Convert to a Type 0 (but non-CID) composite font, suitable for
	// embedding in a PostScript file.  <psName> will be used as the
	// PostScript font name.
	void ToType0(char *sPSName, FontFileOutputFunc pOutputFunc, void *pOutputStream);

private:

	CFontFileType1C(char *sBuffer, int nLen, BOOL bFreeData);
	void EexecConvertGlyph(Type1CEexecBuf *pEexecBuf, char *sGlyphName, int nOffset, int nBytes, Type1CIndex *pSubrIndex, Type1CPrivateDict *pDict);
	void ConvertGlyph(int nOffset, int nBytes, StringExt *seCharBuffer, Type1CIndex *pSubrIndex, Type1CPrivateDict *pDict, BOOL bTop);
	void ConvertGlyphWidth(BOOL bUseOperation, StringExt *seCharBuffer, Type1CPrivateDict *pDict);
	void ConvertNum(double dValue, BOOL bIsFloat, StringExt *seCharBuffer);
	void EexecWrite(Type1CEexecBuf *pEexecBuf, char *sBuffer);
	void EexecWriteCharString(Type1CEexecBuf *pEexecBuf, unsigned char *sBuffer, int nLen);
	BOOL Parse();
	void ReadTopDict();
	void ReadFD(int nOffset, int nLength, Type1CPrivateDict *pDict);
	void ReadPrivateDict(int nOffset, int nLength, Type1CPrivateDict *pDict);
	void ReadFDSelect();
	void BuildEncoding();
	BOOL ReadCharset();
	int GetOperator(int nPos, BOOL bCharString, BOOL *pbSuccess);
	int GetDeltaIntArray(int *pArray, int nMaxLen);
	int GetDeltaDoubleArray(double *pArray, int nMaxLen);
	void GetIndex(int nPos, Type1CIndex *pIndex, BOOL *pbSuccess);
	void GetIndexVal(Type1CIndex *pIndex, int nIndex, Type1CIndexVal *pIndexVal, BOOL *bSuccess);
	char *GetString(int nSID, char *sBuffer, BOOL *pbSuccess);

private:

	StringExt         *m_seName;
	char             **m_arrEncoding;

	Type1CIndex        m_oNameIndex;
	Type1CIndex        m_oTopDictIndex;
	Type1CIndex        m_oStringIndex;
	Type1CIndex        m_oGsubrIndex;
	Type1CIndex        m_oCharStringsIndex;

	Type1CTopDict      m_oTopDict;
    Type1CPrivateDict *m_pPrivateDicts;

	int                m_nGlyphsCount;
	int                m_nFDsCount;
	unsigned char     *m_pnFDSelect;
	unsigned short    *m_pnCharset;
	int                m_nGsubrBias;

	BOOL               m_bSuccessParsed;

	Type1COperator     m_arrOperators[49];
	int                m_nOperatorsCount;
	int                m_nHints;           // для текущего символа
	BOOL               m_bFirstOperator;
	BOOL               m_bOpenPath;		   // true, если есть незакрытый пат
};

#endif /* _FONT_FILE_TYPE1C_H */
