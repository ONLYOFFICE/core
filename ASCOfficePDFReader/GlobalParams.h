#ifndef _GLOBAL_PARAMS_H
#define _GLOBAL_PARAMS_H

#include <stdio.h>
#include "CharTypes.h"
#include "TemporaryCS.h"

class StringExt;
class CList;
class CHash;
class NameToCharCode;
class CharCodeToUnicode;
class CharCodeToUnicodeCache;
class UnicodeMap;
class UnicodeMapCache;
class CMap;
class CMapCache;
class GlobalParams;
class WinFontList;

//-------------------------------------------------------------------------------------------------------------------------------

enum DisplayFontParamType 
{
	displayFontT1,
	displayFontTT
};

struct DisplayFontParamT1 
{
	CStringW *pwsFileName;
};

struct DisplayFontParamTT 
{
	CStringW *pwsFileName;
};

class DisplayFontParam 
{
public:

	DisplayFontParam(StringExt *seName, DisplayFontParamType eType, BOOL bDeleteFile = FALSE);
	virtual ~DisplayFontParam();

public: 

	StringExt           *m_seName;
	DisplayFontParamType m_eType;
	BOOL                 m_bDeleteFile;
	union 
	{
		DisplayFontParamT1 m_oT1;
		DisplayFontParamTT m_oTT;
	};


};

//-------------------------------------------------------------------------------------------------------------------------------

class PSFontParam 
{
public:

	PSFontParam(StringExt *sePDFFontName, int nWMode, StringExt *sePSFontName, StringExt *seEncoding);
	~PSFontParam();

public:

	StringExt *m_sePDFFontName;   // Для 8-битных фонтов и именных 16-битных фонтов; название коллекции для 16-битных фонтов
	int        m_nWMode;          // Writing mode (0 = по горизонтали, 1 = по вертикали) для 16-битных фонтов
	StringExt *m_sePSFontName;    // PostScript'ое название фонта
	StringExt *m_seEncoding;      // Кодировка ( только для 16-битных фонтов )

};

//-------------------------------------------------------------------------------------------------------------------------------

enum PSLevel 
{
	psLevel1,
	psLevel1Sep,
	psLevel2,
	psLevel2Sep,
	psLevel3,
	psLevel3Sep
};

//-------------------------------------------------------------------------------------------------------------------------------

enum EndOfLineType
{
	eolUnix, // LF
	eolDOS,  // CR+LF
	eolMac   // CR
};

//-------------------------------------------------------------------------------------------------------------------------------

enum ScreenType 
{
	screenUnset,
	screenDispersed,
	screenClustered,
	screenStochasticClustered
};


class GlobalParams 
{
public:

	GlobalParams(char *sConfigFileName);

	~GlobalParams();

	void SetBaseDir(char *sDir);
	void SetupBaseFonts(wchar_t *wsDir);
	void SetCMapsDir(wchar_t *wsDir = NULL);

	void ParseLine(char *sBuffer, StringExt *seFileName, int nLine);

	// Считываем глобальные параметры

	CharCode GetMacRomanCharCode(char *sCharName);

	StringExt        *GetBaseDir();
	Unicode           MapNameToUnicode(char *sCharName);
	UnicodeMap       *GetResidentUnicodeMap(StringExt *seEncodingName);
	FILE             *GetUnicodeMapFile(StringExt *seEncodingName);
	FILE             *FindCMapFile(StringExt *seCollection, StringExt *seCMapName);
	FILE             *FindToUnicodeFile(StringExt *seName);
	DisplayFontParam *GetDisplayFont(StringExt *seFontName);
	DisplayFontParam *GetDisplayCIDFont(StringExt *seFontName, StringExt *seCollection);

	StringExt   *GetPSFile();
	int          GetPSPaperWidth();
	int          GetPSPaperHeight();
	void         GetPSImageableArea(int *pnBottomLeftX, int *pnBottomLeftY, int *pnTopRightX, int *pnTopRightY);
	BOOL         GetPSDuplex();
	BOOL         GetPSCrop();
	BOOL         GetPSExpandSmaller();
	BOOL         GetPSShrinkLarger();
	BOOL         GetPSCenter();
	PSLevel      GetPSLevel();
	PSFontParam *GetPSFont(StringExt *seFontName);
	PSFontParam *GetPSFont16(StringExt *seFontName, StringExt *seCollection, int nWMode);
	BOOL         GetPSEmbedType1();
	BOOL         GetPSEmbedTrueType();
	BOOL         GetPSEmbedCIDPostScript();
	BOOL         GetPSEmbedCIDTrueType();
	BOOL         GetPSPreload();
	BOOL         GetPSOPI();
	BOOL         GetPSASCIIHex();

	StringExt    *GetTextEncodingName();
	EndOfLineType GetTextEOL();
	BOOL          GetTextPageBreaks();
	BOOL          GetTextKeepTinyChars();

	CStringW   FindFontFile(StringExt *seFontName, wchar_t **pwsExts);
	StringExt *GetInitialZoom();
	wchar_t   *GetTempFolder()
	{
		return m_sTempDirectory.GetBuffer();
	}
	BOOL       GetContinuousView();
	BOOL       GetEnableGlyphLib();
	BOOL       GetAntialias();
	BOOL       GetVectorAntialias();
	BOOL       GetStrokeAdjust();
	ScreenType GetScreenType();
	int        GetScreenSize();
	int        GetScreenDotRadius();
	double     GetScreenGamma();
	double     GetScreenBlackThreshold();
	double     GetScreenWhiteThreshold();
	StringExt *GetURLCommand() 
	{ 
		return m_seURLCommand; 
	}
	StringExt *GetMovieCommand() 
	{ 
		return m_seMovieCommand; 
	}
	BOOL       GetMapNumericCharNames();
	BOOL       GetMapUnknownCharNames();
	BOOL       GetPrintCommands();
	BOOL       GetErrQuiet();

	CharCodeToUnicode *GetCIDToUnicode(StringExt *seCollection);
	CharCodeToUnicode *GetUnicodeToUnicode(StringExt *seFontName);
	UnicodeMap        *GetUnicodeMap(StringExt *seEncodingName);
	CMap              *GetCMap(StringExt *seCollection, StringExt *seCMapName, wchar_t * wsFilePath = NULL);
	UnicodeMap        *GetTextEncoding();

	// Устанавливаем глобальные параметры

	void AddDisplayFont(DisplayFontParam *pFontParam);

	void CreateTempFolder(BSTR bstrPathSrc, CStringW sTempFolder = _T(""));
	void SetTempFolder(CString sTempFolder);
	void DeleteTempFolder();

	void SetPSFile(char *sFileName);
	BOOL SetPSPaperSize(char *sSize);
	void SetPSPaperWidth(int nWidth);
	void SetPSPaperHeight(int nHeight);
	void SetPSImageableArea(int nBottomLeftX, int nBottomLeftY, int nTopRightX, int nTopRightY);
	void SetPSDuplex(BOOL bDuplex);
	void SetPSCrop(BOOL bCrop);
	void SetPSExpandSmaller(BOOL bExpand);
	void SetPSShrinkLarger(BOOL bShrink);
	void SetPSCenter(BOOL bCenter);
	void SetPSLevel(PSLevel eLevel);
	void SetPSEmbedType1(BOOL bEmbed);
	void SetPSEmbedTrueType(BOOL bEmbed);
	void SetPSEmbedCIDPostScript(BOOL bEmbed);
	void SetPSEmbedCIDTrueType(BOOL bEmbed);
	void SetPSPreload(BOOL bPreload);
	void SetPSOPI(BOOL bOpi);
	void SetPSASCIIHex(BOOL bHex);

	void SetTextEncoding(char *sEncodingName);
	BOOL SetTextEOL(char *sEOL);
	void SetTextPageBreaks(BOOL bPageBreaks);
	void SetTextKeepTinyChars(BOOL bKeep);

	void SetInitialZoom(char *sZoom);
	void SetContinuousView(BOOL bContView);
	BOOL SetEnableGlyphLib(char *sGlyphLib);
	BOOL SetAntialias(char *sAntialias);
	BOOL SetVectorAntialias(char *sVectorAA);
	void SetScreenType(ScreenType eType);
	void SetScreenSize(int nSize);
	void SetScreenDotRadius(int nRadius);
	void SetScreenGamma(double dGamma);
	void SetScreenBlackThreshold(double dTreshold);
	void SetScreenWhiteThreshold(double dTreshold);
	void SetMapNumericCharNames(BOOL bMap);
	void SetMapUnknownCharNames(BOOL bMap);
	void SetPrintCommands(BOOL bPrintCommands);
	void SetErrQuiet(BOOL bErrQuiet);

private:

	void ParseFile(StringExt *seFileName, FILE *pFile);
	void ParseNameToUnicode   (CList *pTokens, StringExt *seFileName, int nLine);
	void ParseCIDToUnicode    (CList *pTokens, StringExt *seFileName, int nLine);
	void ParseUnicodeToUnicode(CList *pTokens, StringExt *seFileName, int nLine);
	void ParseUnicodeMap      (CList *pTokens, StringExt *seFileName, int nLine);
	void ParseCMapDir         (CList *pTokens, StringExt *seFileName, int nLine);
	void ParseToUnicodeDir    (CList *pTokens, StringExt *seFileName, int nLine);

	void ParseDisplayFont(CList *pTokens, CHash *pFontHash, DisplayFontParamType eType, StringExt *seFileName, int nLine);

	// Post Script Output
	void ParsePSFile         (CList *pTokens, StringExt *seFileName, int nLine);
	void ParsePSPaperSize    (CList *pTokens, StringExt *seFileName, int nLine);
	void ParsePSImageableArea(CList *pTokens, StringExt *seFileName, int nLine);
	void ParsePSLevel        (CList *pTokens, StringExt *seFileName, int nLine);
	void ParsePSFont         (CList *pTokens, StringExt *seFileName, int nLine);
	void ParsePSFont16       (char *sCommandName, CList *pFontList, CList *pTokens, StringExt *seFileName, int nLine);
	// Text Output
	void ParseTextEncoding(CList *pTokens, StringExt *seFileName, int nLine);
	void ParseTextEOL     (CList *pTokens, StringExt *seFileName, int nLine);
	//
	void ParseFontDir(CList *pTokens, StringExt *seFileName, int nLine);
	void ParseInitialZoom(CList *pTokens, StringExt *seFileName, int nLine);
	void ParseScreenType(CList *pTokens, StringExt *seFileName, int nLine);
	void ParseCommand(char *sCommandName, StringExt **pseValue, CList *pTokens, StringExt *seFileName, int nLine);
	void ParseYesNo(char *sCommandName, BOOL *pbFlag, CList *pTokens, StringExt *seFileName, int nLine);
	BOOL ParseYesNo2(char *sToken, BOOL *pbFlag);
	void ParseInteger(char *sCommandName, int *pnValue, CList *pTokens, StringExt *seFileName, int nLine);
	void ParseFloat(char *sCommandName, double *pdValue, CList *pTokens, StringExt *seFileName, int nLine);

	UnicodeMap *GetUnicodeMap2(StringExt *seEncodingName);

	bool DecryptResource(BYTE *pData, DWORD dwDataLen, FILE *pFile);

private:

	// Таблицы

	NameToCharCode *m_pMacRomanReverseMap; // Char name -> MacRomanEncoding 
	NameToCharCode *m_pNameToUnicode;      // Char name -> Unicode 

	// user-modifiable settings

	StringExt *baseDir;		// base directory - for plugins, etc.

	CHash *m_pCidToUnicodes;        // Файлы: Char collections -> Unicode, нумерованные по имени коллекции
	CHash *m_pUnicodeToUnicodes;    // Файлы: Unicode -> Unicode, нумерованные по имени шрифта
	CHash *m_pResidentUnicodeMaps;  // Файлы: Unicode -> Char codes, нумерованные по имени кодировки
	CHash *m_pUnicodeMaps;          // Файлы: Unicode -> Char codes, нумерованные по имени кодировки
	CHash *m_pCMapDirs;             // Список CMap директорий, нумерованный по имени коллекций
	CList *m_pToUnicodeDirs;        // Список ToUnicode CMap директорий
	CHash *m_pDisplayFonts;         // Display font info, список идет в порядке имени шрифта [DisplayFontParam]

	WinFontList *m_pWinFontList;    // Системные шрифты (TrueType)

	CHash *m_pDisplayCIDFonts;      // Display CID font info, список идет в порядке имени коллекции [DisplayFontParam]
	CHash *m_pDisplayNamedCIDFonts;	// Display CID font info, список идет в порядке имени шрифта [DisplayFontParam]

	// Для PSOutput
	StringExt *m_sePSFile;              // PostScript файл или команда

	int        m_nPSPaperWidth;         // Размер бумаги для PS output,
	int        m_nPSPaperHeight;        // в пиекселях ( 72 dpi )

	int        m_nPSImageableBLX;       // Координаты левого нижнего,
	int	       m_nPSImageableBLY;       // и правого верхнего углов
	int	       m_nPSImageableTRX;       // прямоугольника, в котором
	int        m_nPSImageableTRY;       // можно рисовать в PSoutput

	BOOL       m_bPSCrop;               // Будем ли обрезать PSoutput до CropBox?
	BOOL       m_bPSExpandSmaller;      // Будем ли мы растягивать маленькие страницы к стандартному размеру?
	BOOL       m_bPSShrinkLarger;       // Будем ли мы сжимать большие страницы к стандартному размеру?
	BOOL       m_bPSCenter;             // Размещать страницу по центру бумаги(т.е. PSoutput)? ( если размеры страницы и бумаги не совпадают )
	BOOL       m_bPSDuplex;             // 
	PSLevel    m_ePSLevel;              // PostScript level
	CHash     *m_pPSFonts;              // PostScript'ие шрифты, сортированные по PDF'им названиям [PSFontParam]
	CList     *m_pPSNamedFonts16;       // 16-битные шрифты с названием [PSFontParam]
	CList     *m_pPSFonts16;            // 16-битные шрифты [PSFontParam]
	BOOL       m_bPSEmbedType1;         // Будем ли мы внедрять Type1-шрифты в Post Script File?
	BOOL       m_bPSEmbedTrueType;      // Будем ли мы внедрять TrueType-шрифты в Post Script File?
	BOOL       m_bPSEmbedCIDPostScript;	// Будем ли мы внедрять CIDPostScript-шрифты в Post Script File?
	BOOL       m_bPSEmbedCIDTrueType;   // Будем ли мы внедрять CIDTrueType-шрифты в Post Script File?
	BOOL       m_bPSPreload;            // Надо ли предварительно загружать картинки и формы в память?
	BOOL       m_bPSOPI;                // Поеддерживаются ли PostScript OPI-комментарии?
	BOOL       m_bPSASCIIHex;           // Использовать ли кодировку ASCIIHex вместо ASCII85?

	// Для TextOutput
	StringExt    *m_seTextEncoding;     // Кодировка (UnicodeMap), которая используется в TextOutput
	EndOfLineType m_eTextEOL;           // Тип маркера конца строки
	BOOL          m_bTextPageBreaks;    // Надо ли вставлять маркеры конца страницы?
	BOOL          m_bTextKeepTinyChars;	// Надо ли хранить все символы в TextOutput
	
	// Глобальные константы
	CList     *m_pFontDirs;             // Список директорий с шрифтами [StringExt]
	StringExt *m_seInitialZoom;         // Начальное значение zoom
	BOOL       m_bContinuousView;       // View mode
	BOOL       m_bEnableGlyphLib;       // флаг использования GlyphLib
	BOOL       m_bAntialias;            // флаг использования Anti-aliasing в шрифтах
	BOOL       m_bVectorAntialias;      // флаг для векторного anti-aliasing
	BOOL       m_bStrokeAdjust;         // Stroke adjustment
	ScreenType m_eScreenType;           // Screen type
	int        m_nScreenSize;           // Screen matrix size
	int        m_nScreenDotRadius;		// Screen dot radius
	double     m_dScreenGamma;          // Screen gamma correction
	double     m_dScreenBlackThreshold; // Screen black clamping threshold
	double     m_dScreenWhiteThreshold; // Screen white clamping threshold
	StringExt *m_seURLCommand;          // Команда для обработки URL ссылок
	StringExt *m_seMovieCommand;        // Команда для обработки Movie annotations
	BOOL       m_bMapNumericCharNames;  // Map numeric char names (from font subsets)?
	BOOL       m_bMapUnknownCharNames;  // Map unknown char names?
	BOOL       m_bPrintCommands;        // Печатать ли графические команды?
	BOOL       m_bErrQuiet;             // 

	CharCodeToUnicodeCache *m_pCidToUnicodeCache;
	CharCodeToUnicodeCache *m_pUnicodeToUnicodeCache;
	UnicodeMapCache        *m_pUnicodeMapCache;
	CMapCache              *m_pCMapCache;

	CString    m_sTempDirectory;
	CString    m_sCMapDirectory;        // Путь к папке с CMap файлами

	CRITICAL_SECTION m_oCS;
	CRITICAL_SECTION m_oUnicodeMapCacheCS;
	CRITICAL_SECTION m_oCMapCacheCS;

};

#endif /* _GLOBAL_PARAMS_H */
