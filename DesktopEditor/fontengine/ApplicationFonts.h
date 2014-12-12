#ifndef _BUILD_APPLICATIONFONTS_H_
#define _BUILD_APPLICATIONFONTS_H_

#define UNKNOWN_CHARSET 3 // для случаев, когда задано значение DEFAULT_CHARSET, но 
                          // на самом деле charset не учитывается

#include "FontManager.h"

class CFontSelectFormat
{
public:
	std::wstring*	wsName;
	std::wstring*	wsAltName;

	std::wstring*	wsFamilyClass;
	SHORT*			sFamilyClass;

    INT*			bBold;
    INT*			bItalic;

    INT*			bFixedWidth;

	BYTE*			pPanose;

	ULONG*			ulRange1;
	ULONG*			ulRange2;
	ULONG*			ulRange3;
	ULONG*			ulRange4;
	ULONG*			ulCodeRange1;
	ULONG*			ulCodeRange2;

	USHORT*			usWeight;
	USHORT*			usWidth;

	int*			nFontFormat;
	BYTE*			unCharset;

	SHORT*			shAvgCharWidth;
	SHORT*			shAscent;
	SHORT*			shDescent;
	SHORT*			shLineGap;
	SHORT*			shXHeight;
	SHORT*			shCapHeight;

public:
	CFontSelectFormat();
	~CFontSelectFormat();
	void CreateDuplicate(CFontSelectFormat& oFormat);
	void Destroy();
};

//-------------------------------------------------------------------------------------------------------------------------------
// CWinFontInfo
//-------------------------------------------------------------------------------------------------------------------------------
enum EFontFormat
{
	fontWindowsFNT = 0, // *.fon
	fontTrueType   = 1, // *.ttf
	fontOpenType   = 2, // *.ttf, *.otf (CFF формат)
	fontUnknown    = 3
};

class CFontInfo
{
public:
	CFontInfo(const std::wstring& wsFontName, 
		const std::wstring& wsStyle, 
		const std::wstring& wsFontPath, 
		long lIndex,
        INT bBold,
        INT bItalic,
        INT bFixedWidth,
		BYTE *pPanose, 
        ULONG ulRange1,
        ULONG ulRange2,
        ULONG ulRange3,
        ULONG ulRange4,
        ULONG ulCodeRange1,
        ULONG ulCodeRange2,
		USHORT usWeigth, 
		USHORT usWidth, 
		SHORT sFamilyClass, 
		EFontFormat eFormat, 
		SHORT shAvgCharWidth, 
		SHORT shAscent, 
		SHORT shDescent, 
		SHORT shLineGap, 
		SHORT shXHeight, 
		SHORT shCapHeight);

	~CFontInfo();
    INT Equals(const CFontInfo *pFontInfo);
    static CFontInfo* FromBuffer(BYTE*& pBuffer, std::wstring strDir);

public:
	std::wstring m_wsFontName;   // Имя шрифта
	std::wstring m_wsFontPath;   // Путь к файлу с шрифтом
	long         m_lIndex;       // Номер шрифта в файле(если в файле больше 1 шрифта)
	std::wstring m_wsStyle;

    INT         m_bBold;            // Bold text
    INT         m_bItalic;          // Italic text
    INT         m_bIsFixed;         // Моноширинный шрифт?

	BYTE         m_aPanose[10];
	ULONG	     m_ulUnicodeRange1;  // Bits 0-31
	ULONG        m_ulUnicodeRange2;  // Bits 32-63
	ULONG        m_ulUnicodeRange3;  // Bits 64-95
	ULONG        m_ulUnicodeRange4;  // Bits 96-127

	ULONG        m_ulCodePageRange1; // Bits 0-31
	ULONG        m_ulCodePageRange2; // Bits 32-63

	USHORT       m_usWeigth;         
	USHORT       m_usWidth;

	SHORT        m_sFamilyClass;
	EFontFormat  m_eFontFormat;

	SHORT        m_shAvgCharWidth;   // Средняя ширина символов
	SHORT        m_shAscent;         // Ascent
	SHORT        m_shDescent;        // Descent
	SHORT        m_shLineGap;        // Межсимвольный интервал
	SHORT        m_shXHeight;        // Высота буквы 'x' (в нижнем регистре)
	SHORT        m_shCapHeight;      // Высота буквы 'H' (в верхнем регистре)
};

namespace NSCharsets
{
	static void GetCodePageByCharset(unsigned char unCharset, unsigned long *pulBit, unsigned int *punLongIndex);
    static int  GetDefaultCharset(INT bUseDefCharset = TRUE);
}

class CFontList
{
private:
	CArray<CFontInfo*>	m_pList;
	std::wstring		m_sDirectory;
	LONG			    m_lDefIndex;	// Номер стандартного шрифта (-1, если не задан)

public:
	CFontList()
	{
		m_lDefIndex = -1;
	}
	~CFontList()
	{
		int nCount = m_pList.GetCount();
		for ( int nIndex = 0; nIndex < nCount; ++nIndex ) 
		{
			CFontInfo* pTemp = m_pList[nIndex];
			RELEASEOBJECT(pTemp);
		}
		m_pList.RemoveAll();
	}

private:
	int GetCharsetPenalty(ULONG ulCandRanges[6], unsigned char unReqCharset);
	int GetSigPenalty(ULONG ulCandRanges[6], ULONG ulReqRanges[6], double dRangeWeight = 1, double dRangeWeightSuferflouous = 0);
    int GetFixedPitchPenalty(INT bCandFixed, INT bReqFixed);
	int GetFaceNamePenalty(std::wstring sCandName, std::wstring sReqName);
	int GetFamilyUnlikelyPenalty(SHORT nCandFamilyClass, SHORT nReqFamilyClass);
	int GetFamilyUnlikelyPenalty(int nCandFamilyClass, std::wstring sReqFamilyClass);
	int GetWidthPenalty(USHORT usCandWidth, USHORT usReqWidth);
	int GetWeightPenalty(USHORT usCandWeight, USHORT usReqWeight);
    int GetItalicPenalty(INT bCandItalic, INT bReqItalic);
    int GetBoldPenalty(INT bCandBold, INT bReqBold);
	int GetFontFormatPenalty(EFontFormat eCandFormat, EFontFormat eReqFormat);
	int GetPanosePenalty(BYTE *pCandPanose, BYTE *pReqPanose);
	int GetAvgWidthPenalty(SHORT shCandWidth, SHORT shReqWidth);
	int GetAscentPenalty(SHORT shCandAscent, SHORT shReqAscent);
	int GetDescentPenalty(SHORT shCandDescent, SHORT shReqDescent);
	int GetLineGapPenalty(SHORT shCandLineGap, SHORT shReqLineGap);
	int GetXHeightPenalty(SHORT shCandXHeight, SHORT shReqXHeight);
	int GetCapHeightPenalty(SHORT shCandCapHeight, SHORT shReqCapHeight);

public:
	static EFontFormat GetFontFormat(FT_Face pFace);

public:
	void LoadFromArrayFiles(CArray<std::wstring>& arrFiles);
	void LoadFromFolder(const std::wstring& strDirectory);
	bool CheckLoadFromFolderBin(const std::wstring& strDirectory);
	void Add(CFontInfo* pInfo);
	CFontInfo* GetByParams(CFontSelectFormat& oSelect);
	CArray<CFontInfo*> GetAllByName(const std::wstring& strFontName);
	void SetDefaultFont(std::wstring& sName);
};

class CApplicationFonts
{
private:
	CApplicationFontStreams	m_oStreams;
	CFontsCache				m_oCache;
	CFontList				m_oList;

public:
	CApplicationFonts();
	~CApplicationFonts();

public:
	CFontsCache*				GetCache();
	CFontList*					GetList();
	CApplicationFontStreams*	GetStreams();

	void InitializeFromFolder(std::wstring strFolder);
	void Initialize();

#ifdef WIN32
	void InitFromReg();
#endif

	CFontManager* GenerateFontManager();
};

#endif
