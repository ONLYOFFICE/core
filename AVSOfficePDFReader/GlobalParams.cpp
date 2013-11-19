#include "stdafx.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <shlobj.h>

#include "RSA/XmlDecoder.h"

#include "Resource.h"

#include "MemoryUtils.h"
#include "StringExt.h"
#include "Constants.h"
#include "List.h"
#include "Hash.h"
#include "File.h"
#include "NameToCharCode.h"
#include "CharCodeToUnicode.h"
#include "UnicodeMap.h"
#include "CMap.h"
#include "BuiltinFontTables.h"
#include "EncodingTables.h"
#include "GlobalParams.h"
#include "NameToUnicodeTable.h"
#include "UnicodeMapTables.h"
#include "UTF8.h"


#  define lockGlobalParams            CTemporaryCS *pCS = new CTemporaryCS( &m_oCS )
#  define lockUnicodeMapCache         CTemporaryCS *pCS = new CTemporaryCS( &m_oUnicodeMapCacheCS )
#  define lockCMapCache               CTemporaryCS *pCS = new CTemporaryCS( &m_oCMapCacheCS )
#  define unlockGlobalParams          RELEASEOBJECT( pCS )
#  define unlockUnicodeMapCache       RELEASEOBJECT( pCS )
#  define unlockCMapCache             RELEASEOBJECT( pCS )

//-------------------------------------------------------------------------------------------------------------------------------

#define cidToUnicodeCacheSize     4
#define unicodeToUnicodeCacheSize 4

//-------------------------------------------------------------------------------------------------------------------------------

static struct 
{
	char *sName;
	char *sT1FileName;
	char *sTTFileName;
	int   nResID;
} c_arroDisplayFontTable[] = 
{
	{"Courier",               "n022003l.pfb", "cour.ttf",    IDR_Courier              },
	{"Courier-Bold",          "n022004l.pfb", "courbd.ttf",  IDR_CourierBold          },
	{"Courier-BoldOblique",   "n022024l.pfb", "courbi.ttf",  IDR_CourierBoldOblique   },
	{"Courier-Oblique",       "n022023l.pfb", "couri.ttf",   IDR_CourierOblique       },
	{"Helvetica",             "n019003l.pfb", "arial.ttf",   IDR_Helvetica            },
	{"Helvetica-Bold",        "n019004l.pfb", "arialbd.ttf", IDR_HelveticaBold        },
	{"Helvetica-BoldOblique", "n019024l.pfb", "arialbi.ttf", IDR_HelveticaBoldOblique },
	{"Helvetica-Oblique",     "n019023l.pfb", "ariali.ttf",  IDR_HelveticaOblique     },
	{"Symbol",                "s050000l.pfb", NULL,          IDR_Symbol               },
	{"Times-Bold",            "n021004l.pfb", "timesbd.ttf", IDR_TimesBold            },
	{"Times-BoldItalic",      "n021024l.pfb", "timesbi.ttf", IDR_TimesBoldItalic      },
	{"Times-Italic",          "n021023l.pfb", "timesi.ttf",  IDR_TimesItalic          },
	{"Times-Roman",           "n021003l.pfb", "times.ttf",   IDR_TimesRoman           },
	{"ZapfDingbats",          "d050000l.pfb", NULL,          IDR_ZapfDingbats         },
	{NULL}
};

static char *c_arrsDisplayFontDirs[] = 
{
	"c:/windows/fonts",
	"c:/winnt/fonts",
	NULL
};

//-------------------------------------------------------------------------------------------------------------------------------
// DisplayFontParam
//-------------------------------------------------------------------------------------------------------------------------------

DisplayFontParam::DisplayFontParam(StringExt *seName, DisplayFontParamType eType, BOOL bDeleteFile) 
{
	m_seName      = seName;
	m_eType       = eType;
	m_bDeleteFile = bDeleteFile;

	switch ( m_eType ) 
	{
	case displayFontT1:
		m_oT1.pwsFileName = NULL;
		break;
	case displayFontTT:
		m_oTT.pwsFileName = NULL;
		break;
	}
}

DisplayFontParam::~DisplayFontParam() 
{
	if ( m_seName )
		delete m_seName;

	switch ( m_eType ) 
	{
	case displayFontT1:
		//if ( m_bDeleteFile )
		//	_wunlink( m_oT1.pwsFileName->GetBuffer() );
		if ( m_oT1.pwsFileName ) 
			delete m_oT1.pwsFileName;
		break;
	case displayFontTT:
		//if ( m_bDeleteFile )
		//	_wunlink( m_oTT.pwsFileName->GetBuffer() );
		if ( m_oTT.pwsFileName )
			delete m_oTT.pwsFileName;
		break;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------
// WinFontInfo
//-------------------------------------------------------------------------------------------------------------------------------

class WinFontInfo: public DisplayFontParam 
{
public:

	static WinFontInfo *Make(StringExt *seName, BOOL bBold, BOOL bItalic, HKEY regKey, wchar_t *wsWinFontDir) 
	{
		// Ищем FontFile
		CStringW *pwsFileName = NULL;
		CStringW wsRegName = CStringW( seName->GetBuffer() );
		if ( bBold ) 
		{
			wsRegName.Append( _T(" Bold") );
		}
		if ( bItalic ) 
		{
			wsRegName.Append( _T(" Italic") );
		}
		wsRegName.Append( _T(" (TrueType)") );

		wchar_t wsBuffer[_MAX_PATH];
		DWORD dwLen = sizeof( wsBuffer ) * sizeof(wchar_t);
		if ( RegQueryValueExW( regKey, wsRegName.GetBuffer(), NULL, NULL, (LPBYTE)wsBuffer, &dwLen ) == ERROR_SUCCESS ) 
		{
			pwsFileName = new CString( wsWinFontDir );
			pwsFileName->Append( _T("\\") );
			pwsFileName->Append( wsBuffer );
		}

		if ( !pwsFileName ) 
		{
			if ( seName )
				delete seName;
			return NULL;
		}

		// Убираем пробелы, запятые и минусы(тире).
		int nIndex = 0;
		while ( nIndex < seName->GetLength() ) 
		{
			char nChar = seName->GetAt(nIndex);
			if ( nChar == ' ' || nChar == ',' || nChar == '-' ) 
			{
				seName->Delete(nIndex);
			} 
			else 
			{
				++nIndex;
			}
		}

		return new WinFontInfo( seName, bBold, bItalic, pwsFileName );
	}


	WinFontInfo(StringExt *seName, BOOL bBold, BOOL bItalic, CStringW *pwsFileName):
	DisplayFontParam( seName, displayFontTT)
	{
		m_bBold           = bBold;
		m_bItalic         = bItalic;
		m_oTT.pwsFileName = pwsFileName;
	}
	virtual ~WinFontInfo() 
	{
	}
	BOOL Equals(WinFontInfo *pFontInfo) 
	{
		return !m_seName->Compare( pFontInfo->m_seName ) && m_bBold == pFontInfo->m_bBold && m_bItalic == pFontInfo->m_bItalic;
	}

public:

	BOOL m_bBold;
	BOOL m_bItalic;
};
//-------------------------------------------------------------------------------------------------------------------------------
// WinFontList
//-------------------------------------------------------------------------------------------------------------------------------

class WinFontList 
{
public:

	WinFontList(wchar_t *wsWinFontDir) 
	{
		m_pFonts = new CList();
		m_pDC = GetDC(NULL);
		m_wsWinFontDir = wsWinFontDir;

		OSVERSIONINFO oVersion;
		oVersion.dwOSVersionInfoSize = sizeof(oVersion);
		GetVersionEx( &oVersion );

		char *sPath;
		if ( oVersion.dwPlatformId == VER_PLATFORM_WIN32_NT ) 
		{
			sPath = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts\\";
		} 
		else 
		{
			sPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Fonts\\";
		}
		if ( RegOpenKeyExA( HKEY_LOCAL_MACHINE, sPath, 0, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, &m_hKey ) == ERROR_SUCCESS ) 
		{
			EnumFontsA( m_pDC, NULL, &WinFontList::EnumerateFunction1, (LPARAM)this );
			RegCloseKey( m_hKey );
		}
		ReleaseDC( NULL, m_pDC );
	}

	~WinFontList() 
	{
		DeleteCList( m_pFonts, WinFontInfo );
	}

	WinFontInfo *Find(StringExt *seFontName) 
	{
		StringExt *seName = seFontName->Copy();

		// Убираем в названии пробелы, запятые и минусы(они же тире).
		int nIndex = 0;
		while ( nIndex < seName->GetLength() ) 
		{
			char nChar = seName->GetAt(nIndex);
			if ( nChar == ' ' || nChar == ',' || nChar == '-' ) 
			{
				seName->Delete( nIndex );
			} 
			else 
			{
				++nIndex;
			}
		}
		int nLen = seName->GetLength();

		// Удаляем "MT"
		if ( !strcmp(seName->GetBuffer() + nLen - 2, "MT") ) 
		{
			seName->Delete(nLen - 2, 2);
			nLen -= 2;
		}

		// Ищем "Italic"
		BOOL bItalic = FALSE;
		if ( !strcmp(seName->GetBuffer() + nLen - 6, "Italic") ) 
		{
			seName->Delete(nLen - 6, 6);
			bItalic = TRUE;
			nLen -= 6;
		} 
		else 
		{
			bItalic = FALSE;
		}

		// Ищем "Bold"
		BOOL bBold = FALSE;
		if ( !strcmp(seName->GetBuffer() + nLen - 4, "Bold") ) 
		{
			seName->Delete( nLen - 4, 4 );
			bBold = TRUE;
			nLen -= 4;
		} 
		else 
		{
			bBold = FALSE;
		}

		// Удаляем "MT"
		if ( !strcmp(seName->GetBuffer() + nLen - 2, "MT") ) 
		{
			seName->Delete( nLen - 2, 2 );
			nLen -= 2;
		}

		// Удаляем "PS"
		if ( !strcmp(seName->GetBuffer() + nLen - 2, "PS") ) 
		{
			seName->Delete( nLen - 2, 2 );
			nLen -= 2;
		}

		// Ищем фонт
		WinFontInfo *pFontInfo = NULL;
		for ( int nIndex = 0; nIndex < m_pFonts->GetLength(); ++nIndex ) 
		{
			pFontInfo = (WinFontInfo *)m_pFonts->GetByIndex(nIndex);
			if ( !pFontInfo->m_seName->Compare(seName) && pFontInfo->m_bBold == bBold && pFontInfo->m_bItalic == bItalic ) 
			{
				break;
			}
			pFontInfo = NULL;
		}

		if ( seName )
			delete seName;
		return pFontInfo;
	}

private:

	void Add(WinFontInfo *pFontInfo) 
	{
		for ( int nIndex = 0; nIndex < m_pFonts->GetLength(); ++nIndex ) 
		{
			if ( ((WinFontInfo *)m_pFonts->GetByIndex(nIndex))->Equals( pFontInfo ) ) 
			{
				if ( pFontInfo )
					delete pFontInfo;
				return;
			}
		}
		m_pFonts->Append( pFontInfo );
	}

	static int CALLBACK EnumerateFunction1(CONST LOGFONTA *pFont, CONST TEXTMETRICA *pMetrics, DWORD nType, LPARAM pData) 
	{
		WinFontList *pFontList = (WinFontList *)pData;

		EnumFontsA( pFontList->m_pDC, pFont->lfFaceName, &WinFontList::EnumerateFunction2, (LPARAM)pFontList );
		return 1;
	}
	static int CALLBACK EnumerateFunction2(CONST LOGFONTA *pFont, CONST TEXTMETRICA *pMetrics, DWORD nType, LPARAM pData) 
	{
		WinFontList *pFontList = (WinFontList *)pData;
		WinFontInfo *pFontInfo = NULL;

		if ( nType & TRUETYPE_FONTTYPE ) 
		{
			if ( ( pFontInfo = WinFontInfo::Make( new StringExt(pFont->lfFaceName), pFont->lfWeight >= 600, pFont->lfItalic ? TRUE : FALSE, pFontList->m_hKey, pFontList->m_wsWinFontDir ) ) ) 
			{
				pFontList->Add( pFontInfo );
			}
		}
		return 1;
	}

private:

	CList   *m_pFonts;      // [WinFontInfo]
	HDC      m_pDC;         // (only used during enumeration)
	HKEY     m_hKey;        // (only used during enumeration)
	wchar_t *m_wsWinFontDir; // (only used during enumeration)
};




//-------------------------------------------------------------------------------------------------------------------------------
// PSFontParam
//-------------------------------------------------------------------------------------------------------------------------------

PSFontParam::PSFontParam(StringExt *sePDFFontName, int nWMode, StringExt *sePSFontName, StringExt *seEncoding) 
{
	m_sePDFFontName = sePDFFontName;
	m_nWMode        = nWMode;
	m_sePSFontName  = sePSFontName;
	m_sePSFontName  = seEncoding;
}

PSFontParam::~PSFontParam() 
{
	if ( m_sePDFFontName )
		delete m_sePDFFontName;
	if ( m_sePSFontName )
		delete m_sePSFontName;
	if ( m_sePSFontName )
		delete m_sePSFontName;
}


//-------------------------------------------------------------------------------------------------------------------------------
// Парсим конфигурационный файл
//-------------------------------------------------------------------------------------------------------------------------------

GlobalParams::GlobalParams(char *sConfigFileName) 
{
	InitializeCriticalSection( &m_oCS );
	InitializeCriticalSection( &m_oUnicodeMapCacheCS );
	InitializeCriticalSection( &m_oCMapCacheCS );

	// Просматриваем кодироваку в обратном порядке, чтобы присвоить символу наименьше возможный номер
	// (если у символа их несколько, например, как у 'space')
	m_pMacRomanReverseMap = new NameToCharCode();
	for ( int nIndex = 255; nIndex >= 0; --nIndex ) 
	{
		if ( c_arrMacRomanEncoding[nIndex] ) 
		{
			m_pMacRomanReverseMap->Add( c_arrMacRomanEncoding[nIndex], (CharCode)nIndex );
		}
	}

	// baseDir установим с помощью функции SetBaseDir
	baseDir = new StringExt();


	// Выделяем память под таблицы и списки
	m_pNameToUnicode        = new NameToCharCode();
	m_pCidToUnicodes        = new CHash(TRUE);
	m_pUnicodeToUnicodes    = new CHash(TRUE);
	m_pResidentUnicodeMaps  = new CHash();
	m_pUnicodeMaps          = new CHash(TRUE);
	m_pCMapDirs             = new CHash(TRUE);
	m_pToUnicodeDirs        = new CList();
	m_pDisplayFonts         = new CHash();
	m_pDisplayCIDFonts      = new CHash();
	m_pDisplayNamedCIDFonts = new CHash();

	// Настрока для PSoutput
	m_nPSPaperWidth   = DefaultPaperWidth;
	m_nPSPaperHeight  = DefaultPaperHeight;
	m_nPSImageableBLX = m_nPSImageableBLY = 0;
	m_nPSImageableTRX = m_nPSPaperWidth;
	m_nPSImageableTRY = m_nPSPaperHeight;
	m_bPSCrop = TRUE;
	m_bPSExpandSmaller = FALSE;
	m_bPSShrinkLarger  = TRUE;
	m_bPSCenter = TRUE;
	m_bPSDuplex = FALSE;
	m_ePSLevel = psLevel2;
	m_sePSFile = NULL;
	m_pPSFonts = new CHash();
	m_pPSNamedFonts16 = new CList();
	m_pPSFonts16 = new CList();

	m_bPSEmbedType1         = TRUE;
	m_bPSEmbedTrueType      = TRUE;
	m_bPSEmbedCIDPostScript = TRUE;
	m_bPSEmbedCIDTrueType   = TRUE;

	m_bPSPreload  = FALSE;
	m_bPSOPI      = FALSE;
	m_bPSASCIIHex = FALSE;

	// Настройка для TextOutput
	m_seTextEncoding     = new StringExt("Latin1");
	m_eTextEOL           = eolDOS; // Windows
	m_bTextPageBreaks    = TRUE;
	m_bTextKeepTinyChars = FALSE;

	//
	m_pFontDirs        = new CList();
	m_seInitialZoom    = new StringExt("125");
	m_bContinuousView  = FALSE;
	m_bEnableGlyphLib  = TRUE;
	m_bAntialias       = TRUE;
	m_bVectorAntialias = TRUE;
	m_bStrokeAdjust    = TRUE;
	m_eScreenType      = screenUnset;
	m_nScreenSize      = -1;
	m_nScreenDotRadius = -1;
	m_dScreenGamma     = 1.0;
	m_dScreenBlackThreshold = 0.0;
	m_dScreenWhiteThreshold = 1.0;
	m_seURLCommand   = NULL;
	m_seMovieCommand = NULL;
	m_bMapNumericCharNames = TRUE;
	m_bMapUnknownCharNames = FALSE;

	m_bPrintCommands = FALSE;
	m_bErrQuiet      = FALSE;

	//
	m_pCidToUnicodeCache     = new CharCodeToUnicodeCache( cidToUnicodeCacheSize );
	m_pUnicodeToUnicodeCache = new CharCodeToUnicodeCache(unicodeToUnicodeCacheSize);
	m_pUnicodeMapCache = new UnicodeMapCache();
	m_pCMapCache       = new CMapCache();

	m_pWinFontList = NULL;


	// Инициализируем талицу m_pNameToUnicode
	for ( int nIndex = 0; c_arrNameToUnicodeTable[nIndex].sName; ++nIndex ) 
	{
		m_pNameToUnicode->Add( c_arrNameToUnicodeTable[nIndex].sName, c_arrNameToUnicodeTable[nIndex].nUnicode );
	}

	// ResidentUnicodeMaps
	UnicodeMap *pMap = NULL;

	pMap = new UnicodeMap("Latin1", FALSE, c_arrLatin1UnicodeMapRanges, Latin1UnicodeMapLen);
	m_pResidentUnicodeMaps->Add( pMap->GetEncodingName(), pMap );

	pMap = new UnicodeMap("ASCII7", FALSE, c_arrASCII7UnicodeMapRanges, ASCII7UnicodeMapLen);
	m_pResidentUnicodeMaps->Add( pMap->GetEncodingName(), pMap );

	pMap = new UnicodeMap("Symbol", FALSE, c_arrSymbolUnicodeMapRanges, SymbolUnicodeMapLen);
	m_pResidentUnicodeMaps->Add( pMap->GetEncodingName(), pMap );

	pMap = new UnicodeMap("ZapfDingbats", FALSE, c_arrZapfDingbatsUnicodeMapRanges, ZapfDingbatsUnicodeMapLen);
	m_pResidentUnicodeMaps->Add( pMap->GetEncodingName(), pMap );

	pMap = new UnicodeMap("UTF-8", TRUE, &MapUTF8);
	m_pResidentUnicodeMaps->Add( pMap->GetEncodingName(), pMap );

	pMap = new UnicodeMap("UCS-2", TRUE, &MapUCS2);
	m_pResidentUnicodeMaps->Add( pMap->GetEncodingName(), pMap );

	// Ищем конфигурационный файл
	FILE *pFile = NULL;
	StringExt *seFileName = NULL;
	if ( sConfigFileName && sConfigFileName[0] ) 
	{
		seFileName = new StringExt( sConfigFileName );
		if ( !(pFile = fopen( seFileName->GetBuffer(), "r") ) ) 
		{
			delete seFileName;
		}
	}
	if ( !pFile ) 
	{
		seFileName = AppendToPath(GetHomeDir(), UserConfigFile);
		if ( !( pFile = fopen( seFileName->GetBuffer(), "r") ) ) 
		{
			delete seFileName;
		}
	}
	if (!pFile) 
	{
		char sBuffer[512];
		int nLen = GetModuleFileNameA( NULL, sBuffer, sizeof( sBuffer ) );
		if ( nLen <= 0 || nLen >= sizeof( sBuffer ) ) 
		{
			sBuffer[0] = '\0';
		}
		seFileName = GrabPath( sBuffer );
		AppendToPath( seFileName, SysConfigFile );
		if ( !( pFile = fopen( seFileName->GetBuffer(), "r") ) ) 
		{
			delete seFileName;
		}
	}
	if ( pFile ) 
	{
		ParseFile( seFileName, pFile );
		delete seFileName;
		fclose( pFile );
	}

	m_sTempDirectory = _T("");
}


static int GetDigit(WCHAR c)
{
	if ( c >= '0' && c <= '9' )
		return (int)( c - '0' );

	return -1;
}

static int GetTempFileNum(CStringW strMask, CStringW strDirPath)
{
	int nIndex = strDirPath.Find(strMask);
	if (0 != nIndex)
		return -1;

	CStringW strNum = strDirPath.Mid(strMask.GetLength());

	int nLen = strNum.GetLength();
	int nResult = 0;
	int nIndexNum = 0;

	while (nLen > 0)
	{
		int nDig = GetDigit(strNum[nIndexNum]);

		if (-1 == nDig)
			return -1;

		nResult *= 10;
		nResult += nDig;

		++nIndexNum;
		--nLen;
	}

	return nResult;
}

void GlobalParams::CreateTempFolder(BSTR bstrPathSrc, CString sTempFolder)
{
	// Сначала удалим темповую папку, если она была
	DeleteTempFolder();

	m_sTempDirectory = L"";

	CStringW sTempPath;

	if ( _T("") == sTempFolder )
	{
		GetTempPathW( 4096, sTempPath.GetBuffer(4096) );
		sTempPath.ReleaseBuffer();
	}
	else
	{
		sTempPath = sTempFolder + _T("\\");
	}

	CStringW strSrcFileName = (CStringW)bstrPathSrc;
	int nFindCatalogPos = strSrcFileName.ReverseFind('\\');

	if (-1 != nFindCatalogPos)
	{
		strSrcFileName = strSrcFileName.Mid(nFindCatalogPos + 1);
	}

	CStringW sFileMask = sTempPath + L"*";

	CStringW strMaskTemp = (L"Temp_" + strSrcFileName);
	int nResultNumber = -1;


	WIN32_FIND_DATAW winFileData;
	HANDLE hFile;

	hFile = FindFirstFileW(sFileMask, &winFileData);
	if ( hFile != INVALID_HANDLE_VALUE )
	{ 
		CStringW strFilePath = sFileMask + (CStringW)winFileData.cFileName;
		DWORD dwAttr = GetFileAttributesW( strFilePath );
		if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
		{
			int nNum = GetTempFileNum(strMaskTemp, winFileData.cFileName);
			nResultNumber = max( nNum, nResultNumber );
		}

		while (FALSE != FindNextFileW(hFile, &winFileData))
		{
			CStringW strFilePathNext = sFileMask + (CStringW)winFileData.cFileName;
			DWORD dwAttrNext = GetFileAttributesW(strFilePathNext);
			if (dwAttrNext & FILE_ATTRIBUTE_DIRECTORY)
			{
				int nNumNext = GetTempFileNum(strMaskTemp, winFileData.cFileName);
				nResultNumber = max(nNumNext, nResultNumber);
			}
		}

		FindClose(hFile);
	}

	++nResultNumber;

	if ( 0 == nResultNumber  )
	{
		m_sTempDirectory = sTempPath + strMaskTemp;
	}
	else
	{
		m_sTempDirectory.Format( sTempPath + strMaskTemp + L"%d", nResultNumber );
	}

	if ( !CreateDirectoryW( m_sTempDirectory, NULL ) )
	{
		m_sTempDirectory = L"";
	}
}
void GlobalParams::SetTempFolder(CString sTempFolder)
{
	m_sTempDirectory = sTempFolder;
}
void GlobalParams::DeleteTempFolder()
{
	int nResult = 0;
	if ( _T("") == m_sTempDirectory )
		return;

	CStringW sLocalFileName = m_sTempDirectory + L"\\*.*";
	LPCWSTR sCur = sLocalFileName.GetBuffer();
	WIN32_FIND_DATAW oFindData;
	HANDLE hFindFile = FindFirstFileW( sLocalFileName.GetBuffer(), &oFindData );
	do
	{
		CStringW sCurName = m_sTempDirectory;
		sCurName += L"\\";
		sCurName += oFindData.cFileName;
		sCur = sCurName.GetBuffer();
		nResult = DeleteFileW( sCur );
	}while ( FindNextFileW( hFindFile, &oFindData ) );
	nResult = FindClose( hFindFile );


	//SHFILEOPSTRUCTW shfos;
	//ZeroMemory(&shfos, sizeof(shfos));
	//shfos.wFunc = FO_DELETE;
	//CStringW _local = m_strTempDirectory + L"\\*.*";
	//_local.AppendChar(0);
	//_local.AppendChar(0);
	//shfos.pFrom = _local.GetString();
	//shfos.fFlags = FOF_SILENT + FOF_NOCONFIRMATION;

	//SHFileOperationW(&shfos);
	nResult = RemoveDirectoryW( m_sTempDirectory );

	m_sTempDirectory = _T("");
}
void GlobalParams::ParseFile(StringExt *seFileName, FILE *pFile) 
{
	char sBuffer[512];
	int nLine = 1;
	while ( GetLine( sBuffer, sizeof( sBuffer ) - 1, pFile ) ) 
	{
		ParseLine( sBuffer, seFileName, nLine );
		++nLine;
	}
}

void GlobalParams::ParseLine(char *sBuffer, StringExt *seFileName, int nLine) 
{
	// Разбиваем строчку на элементы
	CList *pTokens = new CList();
	char *pFirst = sBuffer, *pLast = NULL;
	while (*pFirst) 
	{
		for (; *pFirst && isspace(*pFirst); ++pFirst );
		if ( !*pFirst )
		{
			break;
		}
		if ( *pFirst == '"' || *pFirst == '\'' ) 
		{
			for ( pLast = pFirst + 1; *pLast && *pLast != *pFirst; ++pLast );
			++pFirst;
		} 
		else 
		{
			for ( pLast = pFirst + 1; *pLast && !isspace(*pLast); ++pLast );
		}
		pTokens->Append( new StringExt( pFirst, pLast - pFirst ) );
		pFirst = *pLast ? pLast + 1 : pLast;
	}

	if ( pTokens->GetLength() > 0 && ((StringExt *)pTokens->GetByIndex(0))->GetAt(0) != '#' ) 
	{
		StringExt *seCommand = (StringExt *)pTokens->GetByIndex(0);
		if ( !seCommand->Compare("include") ) 
		{
			if ( pTokens->GetLength() == 2 ) 
			{
				StringExt *seIncludeFile = (StringExt *)pTokens->GetByIndex(1);
				FILE *pIncludeFile = NULL;
				if ( ( pIncludeFile = fopen( seIncludeFile->GetBuffer(), "r") ) ) 
				{
					ParseFile( seIncludeFile, pIncludeFile);
					fclose( pIncludeFile );
				} 
				else 
				{
					// TO DO: Error "Couldn't find included config file"
				}
			} 
			else 
			{
				// TO DO: Error "Bad 'include' config file command"
			}
		} 
		else if ( !seCommand->Compare("nameToUnicode") ) 
			ParseNameToUnicode( pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("cidToUnicode") ) 
			ParseCIDToUnicode( pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("unicodeToUnicode") ) 
			ParseUnicodeToUnicode( pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("unicodeMap") ) 
			ParseUnicodeMap( pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("cMapDir") ) 
			ParseCMapDir( pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("toUnicodeDir") ) 
			ParseToUnicodeDir( pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("displayFontT1") ) 
			ParseDisplayFont( pTokens, m_pDisplayFonts, displayFontT1, seFileName, nLine);
		else if ( !seCommand->Compare("displayFontTT") ) 
			ParseDisplayFont( pTokens, m_pDisplayFonts, displayFontTT, seFileName, nLine);
		else if ( !seCommand->Compare("displayNamedCIDFontT1") ) 
			ParseDisplayFont( pTokens, m_pDisplayNamedCIDFonts, displayFontT1, seFileName, nLine);
		else if ( !seCommand->Compare("displayCIDFontT1") ) 
			ParseDisplayFont( pTokens, m_pDisplayCIDFonts, displayFontT1, seFileName, nLine);
		else if ( !seCommand->Compare("displayNamedCIDFontTT") ) 
			ParseDisplayFont( pTokens, m_pDisplayNamedCIDFonts, displayFontTT, seFileName, nLine);
		else if ( !seCommand->Compare("displayCIDFontTT") ) 
			ParseDisplayFont( pTokens, m_pDisplayCIDFonts, displayFontTT, seFileName, nLine);
		else if ( !seCommand->Compare("psFile") ) 
			ParsePSFile( pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("psFont") ) 
			ParsePSFont( pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("psNamedFont16") ) 
			ParsePSFont16("psNamedFont16", m_pPSNamedFonts16,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("psFont16") ) 
			ParsePSFont16("psFont16", m_pPSFonts16,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("psPaperSize") ) 
			ParsePSPaperSize( pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("psImageableArea") ) 
			ParsePSImageableArea( pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("psCrop") ) 
			ParseYesNo("psCrop", &m_bPSCrop,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("psExpandSmaller") ) 
			ParseYesNo("psExpandSmaller", &m_bPSExpandSmaller,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("psShrinkLarger") ) 
			ParseYesNo("psShrinkLarger", &m_bPSShrinkLarger,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("psCenter") ) 
			ParseYesNo("psCenter", &m_bPSCenter,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("psDuplex") ) 
			ParseYesNo("psDuplex", &m_bPSDuplex,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("psLevel") ) 
			ParsePSLevel( pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("psEmbedType1Fonts") ) 
			ParseYesNo("psEmbedType1", &m_bPSEmbedType1,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("psEmbedTrueTypeFonts") ) 
			ParseYesNo("psEmbedTrueType", &m_bPSEmbedTrueType,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("psEmbedCIDPostScriptFonts") ) 
			ParseYesNo("psEmbedCIDPostScript", &m_bPSEmbedCIDPostScript,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("psEmbedCIDTrueTypeFonts") ) 
			ParseYesNo("psEmbedCIDTrueType", &m_bPSEmbedCIDTrueType,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("psPreload") ) 
			ParseYesNo("psPreload", &m_bPSPreload,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("psOPI") ) 
			ParseYesNo("psOPI", &m_bPSOPI,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("psASCIIHex") ) 
			ParseYesNo("psASCIIHex", &m_bPSASCIIHex,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("textEncoding") ) 
			ParseTextEncoding( pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("textEOL") ) 
			ParseTextEOL( pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("textPageBreaks") )
			ParseYesNo("textPageBreaks", &m_bTextPageBreaks,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("textKeepTinyChars") ) 
			ParseYesNo("textKeepTinyChars", &m_bTextKeepTinyChars,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("fontDir") ) 
			ParseFontDir( pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("initialZoom") ) 
			ParseInitialZoom( pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("continuousView") )
			ParseYesNo("continuousView", &m_bContinuousView,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("enableGlyphLib") )
			ParseYesNo("enableGlyphLib", &m_bEnableGlyphLib,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("antialias") )
			ParseYesNo("antialias", &m_bAntialias,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("vectorAntialias") )
			ParseYesNo("vectorAntialias", &m_bVectorAntialias,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("strokeAdjust") )
			ParseYesNo("strokeAdjust", &m_bStrokeAdjust,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("screenType") )
			ParseScreenType( pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("screenSize") )
			ParseInteger("screenSize", &m_nScreenSize,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("screenDotRadius") )
			ParseInteger("screenDotRadius", &m_nScreenDotRadius,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("screenGamma") )
			ParseFloat("screenGamma", &m_dScreenGamma,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("screenBlackThreshold") )
			ParseFloat("screenBlackThreshold", &m_dScreenBlackThreshold,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("screenWhiteThreshold") )
			ParseFloat("screenWhiteThreshold", &m_dScreenWhiteThreshold,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("urlCommand") )
			ParseCommand("urlCommand", &m_seURLCommand,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("movieCommand") )
			ParseCommand("movieCommand", &m_seMovieCommand,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("mapNumericCharNames") )
			ParseYesNo("mapNumericCharNames", &m_bMapNumericCharNames,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("mapUnknownCharNames") )
			ParseYesNo("mapUnknownCharNames", &m_bMapUnknownCharNames,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("printCommands") ) 
			ParseYesNo("printCommands", &m_bPrintCommands,  pTokens, seFileName, nLine);
		else if ( !seCommand->Compare("errQuiet") )
			ParseYesNo("errQuiet", &m_bErrQuiet,  pTokens, seFileName, nLine);
		else
		{
			// TO DO: Error "Unknown config file command"
		}
	}

	DeleteCList( pTokens, StringExt);
}

void GlobalParams::ParseNameToUnicode(CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 2 ) 
	{
		// TO DO: Error "Bad 'nameToUnicode' config file command"
		return;
	}
	StringExt *seName = (StringExt *)pTokens->GetByIndex(1);
	FILE *pFile = NULL;
	if ( !( pFile = fopen(seName->GetBuffer(), "r") ) ) 
	{
		// TO DO: Error "Couldn't open 'nameToUnicode' file"
		return;
	}
	int nCurLine = 1;
	char sBuffer[256];
	while ( GetLine( sBuffer, sizeof( sBuffer ), pFile ) ) 
	{
		char *sToken1 = strtok( sBuffer, " \t\r\n" );
		char *sToken2 = strtok( NULL, " \t\r\n" );
		if ( sToken1 && sToken2 ) 
		{
			Unicode nUnicode;
			sscanf( sToken1, "%x", &nUnicode );
			m_pNameToUnicode->Add( sToken2, nUnicode );
		} 
		else 
		{
			// TO DO: Error "Bad line in 'nameToUnicode' file"
		}
		++nCurLine;
	}
	fclose( pFile );
}

void GlobalParams::ParseCIDToUnicode(CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 3 ) 
	{
		// TO DO: Error "Bad 'cidToUnicode' config file command"
		return;
	}
	StringExt *seCollection = (StringExt *)pTokens->GetByIndex(1);
	StringExt *seName       = (StringExt *)pTokens->GetByIndex(2);

	StringExt *seOld = NULL;
	if ( ( seOld = (StringExt *)m_pCidToUnicodes->Remove( seCollection ) ) ) 
	{
		delete seOld;
	}
	m_pCidToUnicodes->Add( seCollection->Copy(), seName->Copy());
}

void GlobalParams::ParseUnicodeToUnicode(CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 3 ) 
	{
		// TO DO: Error "Bad 'unicodeToUnicode' config file command"
		return;
	}
	StringExt *seFont = (StringExt *)pTokens->GetByIndex(1);
	StringExt *seFile = (StringExt *)pTokens->GetByIndex(2);

	StringExt *seOld = NULL;
	if ( ( seOld = (StringExt *)m_pUnicodeToUnicodes->Remove( seFont ) ) ) 
	{
		delete seOld;
	}
	m_pUnicodeToUnicodes->Add( seFont->Copy(), seFile->Copy() );
}

void GlobalParams::ParseUnicodeMap(CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 3 ) 
	{
		// TO DO: Error "Bad 'unicodeMap' config file command"
		return;
	}

	StringExt *seEncodingName = (StringExt *)pTokens->GetByIndex(1);
	StringExt *seName         = (StringExt *)pTokens->GetByIndex(2);

	StringExt *seOld = NULL;
	if ( ( seOld = (StringExt *)m_pUnicodeMaps->Remove( seEncodingName ) ) ) 
	{
		delete seOld;
	}
	m_pUnicodeMaps->Add( seEncodingName->Copy(), seName->Copy() );
}

void GlobalParams::ParseCMapDir(CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 3 ) 
	{
		// TO DO: Error "Bad 'cMapDir' config file command"
		return;
	}
	StringExt *seCollection = (StringExt *)pTokens->GetByIndex(1);
	StringExt *seDir        = (StringExt *)pTokens->GetByIndex(2);

	CList *pList = NULL;
	if ( !( pList = (CList *)m_pCMapDirs->Lookup( seCollection ) ) ) 
	{
		pList = new CList();
		m_pCMapDirs->Add( seCollection->Copy(), pList );
	}
	pList->Append( seDir->Copy() );
}

void GlobalParams::ParseToUnicodeDir(CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 2 ) 
	{
		// TO DO: Error "Bad 'toUnicodeDir' config file command"
		return;
	}
	m_pToUnicodeDirs->Append( ((StringExt *)pTokens->GetByIndex(1))->Copy() );
}

void GlobalParams::ParseDisplayFont(CList *pTokens, CHash *pFontHash, DisplayFontParamType eType, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() < 2 ) 
	{
		// TO DO: Error "Bad 'display*Font*' config file command"
		return;
	}
	DisplayFontParam *pFontParameters = new DisplayFontParam( ((StringExt *)pTokens->GetByIndex(1))->Copy(), eType );

	switch (eType) 
	{
	case displayFontT1:
		if ( pTokens->GetLength() != 3 ) 
		{
			delete pFontParameters;
			// TO DO: Error "Bad 'display*Font*' config file command"
			return;
		}
		//pFontParameters->m_oT1.pwsFileName = ((CStringW *)pTokens->GetByIndex(2))->Сopy();
		break;
	case displayFontTT:
		if ( pTokens->GetLength() != 3 ) 
		{
			delete pFontParameters;
			// TO DO: Error "Bad 'display*Font*' config file command"
			return;
		}
		//pFontParameters->m_oTT.pwsFileName = ((CStringW *)pTokens->GetByIndex(2))->Сopy();
		break;
	}

	DisplayFontParam *pOld = NULL;
	if ( ( pOld = (DisplayFontParam *)pFontHash->Remove( pFontParameters->m_seName ) ) ) 
	{
		delete pOld;
	}
	pFontHash->Add( pFontParameters->m_seName, pFontParameters);
	return;
}

void GlobalParams::ParsePSPaperSize(CList *pTokens, StringExt *seFileName, int nLine) 
{
	StringExt *seToken = NULL;

	if ( pTokens->GetLength() == 2 ) 
	{
		seToken = (StringExt *)pTokens->GetByIndex(1);
		if ( !SetPSPaperSize( seToken->GetBuffer() ) ) 
		{
			// TO DO: Error "Bad 'psPaperSize' config file command"
		}
	} 
	else if ( pTokens->GetLength() == 3 ) 
	{
		seToken = (StringExt *)pTokens->GetByIndex(1);
		m_nPSPaperWidth  = atoi( seToken->GetBuffer() );
		seToken = (StringExt *)pTokens->GetByIndex(2);
		m_nPSPaperHeight = atoi( seToken->GetBuffer() );
		m_nPSImageableBLX = m_nPSImageableBLY = 0;
		m_nPSImageableTRX = m_nPSPaperWidth;
		m_nPSImageableTRY = m_nPSPaperHeight;
	} 
	else 
	{
		// TO DO: Error "Bad 'psPaperSize' config file command"
	}
}

void GlobalParams::ParsePSImageableArea(CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 5 ) 
	{
		// TO DO: Error "Bad 'psImageableArea' config file command"
		return;
	}
	m_nPSImageableBLX = atoi( ((StringExt *)pTokens->GetByIndex(1))->GetBuffer() );
	m_nPSImageableBLY = atoi( ((StringExt *)pTokens->GetByIndex(2))->GetBuffer() );
	m_nPSImageableTRX = atoi( ((StringExt *)pTokens->GetByIndex(3))->GetBuffer() );
	m_nPSImageableTRY = atoi( ((StringExt *)pTokens->GetByIndex(4))->GetBuffer() );
}

void GlobalParams::ParsePSLevel(CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 2 ) 
	{
		// TO DO: Error "Bad 'psLevel' config file command"
		return;
	}
	StringExt *seLevel = (StringExt *)pTokens->GetByIndex(1);
	if ( !seLevel->Compare("level1") ) 
	{
		m_ePSLevel = psLevel1;
	} 
	else if ( !seLevel->Compare("level1sep") ) 
	{
		m_ePSLevel = psLevel1Sep;
	} 
	else if ( !seLevel->Compare("level2") ) 
	{
		m_ePSLevel = psLevel2;
	} 
	else if ( !seLevel->Compare("level2sep") ) 
	{
		m_ePSLevel = psLevel2Sep;
	} 
	else if ( !seLevel->Compare("level3") ) 
	{
		m_ePSLevel = psLevel3;
	} 
	else if ( !seLevel->Compare("level3Sep") ) 
	{
		m_ePSLevel = psLevel3Sep;
	} 
	else 
	{
		// TO DO: Error "Bad 'psLevel' config file command"
	}
}

void GlobalParams::ParsePSFile(CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 2 ) 
	{
		// TO DO: Error "Bad 'psFile' config file command"
		return;
	}
	if ( m_sePSFile ) 
	{
		delete m_sePSFile;
	}
	m_sePSFile = ((StringExt *)pTokens->GetByIndex(1))->Copy();
}

void GlobalParams::ParsePSFont(CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 3 ) 
	{
		// TO DO: Error "Bad 'psFont' config file command"
		return;
	}
	PSFontParam *pFontParam = new PSFontParam( ((StringExt *)pTokens->GetByIndex(1))->Copy(), 0, ((StringExt *)pTokens->GetByIndex(2))->Copy(), NULL );
	m_pPSFonts->Add( pFontParam->m_sePDFFontName, pFontParam );
}

void GlobalParams::ParsePSFont16(char *sCommandName, CList *pFontList, CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 5 ) 
	{
		// TO DO: Error "Bad config file command"
		return;
	}
	StringExt *seWritingMode = (StringExt *)pTokens->GetByIndex(2);
	int nWMode = 0;
	if ( !seWritingMode->Compare("H") ) 
	{
		nWMode = 0;
	} 
	else if ( !seWritingMode->Compare("V") ) 
	{
		nWMode = 1;
	} 
	else 
	{
		// TO DO: Error "Bad config file command"
		return;
	}
	PSFontParam *pFontParam = new PSFontParam(((StringExt *)pTokens->GetByIndex(1))->Copy(), nWMode, ((StringExt *)pTokens->GetByIndex(3))->Copy(), ((StringExt *)pTokens->GetByIndex(4))->Copy());
	pFontList->Append( pFontParam );
}

void GlobalParams::ParseTextEncoding(CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 2 ) 
	{
		// TO DO: Error "Bad 'textEncoding' config file command"
		return;
	}
	if ( m_seTextEncoding )
		delete m_seTextEncoding;
	m_seTextEncoding = ((StringExt *)pTokens->GetByIndex(1))->Copy();
}

void GlobalParams::ParseTextEOL(CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 2 ) 
	{
		// TO DO: Error "Bad 'textEOL' config file command"
		return;
	}
	StringExt *seEOL = (StringExt *)pTokens->GetByIndex(1);
	if ( !seEOL->Compare("unix") ) 
	{
		m_eTextEOL = eolUnix;
	} 
	else if ( !seEOL->Compare("dos") ) 
	{
		m_eTextEOL = eolDOS;
	} 
	else if ( !seEOL->Compare("mac") ) 
	{
		m_eTextEOL = eolMac;
	} 
	else 
	{
		// TO DO: Error "Bad 'textEOL' config file command"
	}
}

void GlobalParams::ParseFontDir(CList *pTokens, StringExt *seFileName, int nLine)
{
	if ( pTokens->GetLength() != 2 ) 
	{
		// TO DO: Error "Bad 'fontDir' config file command"
		return;
	}
	m_pFontDirs->Append( ((StringExt *)pTokens->GetByIndex(1))->Copy() );
}

void GlobalParams::ParseInitialZoom(CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 2 ) 
	{
		// TO DO: Error "Bad 'initialZoom' config file command"
		return;
	}
	if ( m_seInitialZoom )
		delete m_seInitialZoom;
	m_seInitialZoom = ((StringExt *)pTokens->GetByIndex(1))->Copy();
}

void GlobalParams::ParseScreenType(CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 2 ) 
	{
		// TO DO: Error "Bad 'screenType' config file command"
		return;
	}
	StringExt *seSreenType = (StringExt *)pTokens->GetByIndex(1);
	if ( !seSreenType->Compare("dispersed") ) 
	{
		m_eScreenType = screenDispersed;
	} 
	else if ( !seSreenType->Compare("clustered") ) 
	{
		m_eScreenType = screenClustered;
	} 
	else if ( !seSreenType->Compare("stochasticClustered") ) 
	{
		m_eScreenType = screenStochasticClustered;
	} 
	else 
	{
		// TO DO: Error "Bad 'screenType' config file command"
	}
}

void GlobalParams::ParseCommand(char *sCommandName, StringExt **pseValue, CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 2 ) 
	{
		// TO DO: Error "Bad config file command"
		return;
	}
	if ( *pseValue )
	{
		delete *pseValue;
	}
	*pseValue = ((StringExt *)pTokens->GetByIndex(1))->Copy();
}

void GlobalParams::ParseYesNo(char *sCommandName, BOOL *pbFlag, CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 2 ) 
	{
		// TO DO: Error "Bad config file command"
		return;
	}
	StringExt *seToken = (StringExt *)pTokens->GetByIndex(1);
	if ( !ParseYesNo2(seToken->GetBuffer(), pbFlag) ) 
	{
		// TO DO: Error "Bad config file command"
	}
}

BOOL GlobalParams::ParseYesNo2(char *sToken, BOOL *pbFlag) 
{
	if ( !strcmp(sToken, "yes") ) 
	{
		*pbFlag = TRUE;
	} 
	else if ( !strcmp(sToken, "no") ) 
	{
		*pbFlag = FALSE;
	} 
	else 
	{
		return FALSE;
	}
	return TRUE;
}

void GlobalParams::ParseInteger(char *sCommandName, int *pnValue, CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 2 ) 
	{
		// TO DO: Error "Bad config file command"
		return;
	}
	StringExt *seValue = (StringExt *)pTokens->GetByIndex(1);
	if ( seValue->GetLength() == 0 ) 
	{
		// TO DO: Error "Bad config file command"
		return;
	}
	int nLen = 0;
	if ( seValue->GetAt(0) == '-' ) 
	{
		nLen = 1;
	} 
	else 
	{
		nLen = 0;
	}
	for (; nLen < seValue->GetLength(); ++nLen) 
	{
		if ( seValue->GetAt(nLen) < '0' || seValue->GetAt(nLen) > '9' ) 
		{
			// TO DO: Error "Bad config file command"
			return;
		}
	}
	*pnValue = atoi( seValue->GetBuffer() );
}

void GlobalParams::ParseFloat(char *sCommandName, double *pdValue, CList *pTokens, StringExt *seFileName, int nLine) 
{
	if ( pTokens->GetLength() != 2 ) 
	{
		// TO DO: Error "Bad config file command"
		return;
	}
	StringExt *seValue = (StringExt *)pTokens->GetByIndex(1);
	if ( seValue->GetLength() == 0 ) 
	{
		// TO DO: Error "Bad config file command"
		return;
	}
	int nLen = 0;
	if ( seValue->GetAt(0) == '-' ) 
	{
		nLen = 1;
	} 
	else 
	{
		nLen = 0;
	}
	for (; nLen < seValue->GetLength(); ++nLen) 
	{
		if ( !( (seValue->GetAt(nLen) >= '0' && seValue->GetAt(nLen) <= '9') || seValue->GetAt(nLen) == '.') ) 
		{
			// TO DO: Error "Bad config file command"
			return;
		}
	}
	*pdValue = atof( seValue->GetBuffer() );
}

GlobalParams::~GlobalParams() 
{
	//DeleteTempFolder();

	if ( m_pMacRomanReverseMap )
		delete m_pMacRomanReverseMap;

	if ( baseDir )
		delete baseDir;

	if ( m_pNameToUnicode )
		delete m_pNameToUnicode;

	DeleteCHash( m_pCidToUnicodes,        StringExt );
	DeleteCHash( m_pUnicodeToUnicodes,    StringExt );
	DeleteCHash( m_pResidentUnicodeMaps,  UnicodeMap );
	DeleteCHash( m_pUnicodeMaps,          StringExt );
	DeleteCList( m_pToUnicodeDirs,        StringExt );
	DeleteCHash( m_pDisplayFonts,         DisplayFontParam );
	DeleteCHash( m_pDisplayCIDFonts,      DisplayFontParam );
	DeleteCHash( m_pDisplayNamedCIDFonts, DisplayFontParam );

	if ( m_pWinFontList )
		delete m_pWinFontList;
	if ( m_sePSFile )
		delete m_sePSFile;

	DeleteCHash( m_pPSFonts,        PSFontParam );
	DeleteCList( m_pPSNamedFonts16, PSFontParam );
	DeleteCList( m_pPSFonts16,      PSFontParam );

	if ( m_seTextEncoding )
		delete m_seTextEncoding;
	DeleteCList( m_pFontDirs, StringExt );
	if ( m_seInitialZoom )
		delete m_seInitialZoom;
	if ( m_seURLCommand )
		delete m_seURLCommand;
	if ( m_seMovieCommand )
		delete m_seMovieCommand;

	THashIter *pIter = NULL;
	StringExt *seKey = NULL;
	CList *pList = NULL;
	m_pCMapDirs->StartIter( &pIter );
	while ( m_pCMapDirs->GetNext( &pIter, &seKey, (void **)&pList) )
	{
		DeleteCList( pList, StringExt );
	}

	if ( m_pCMapDirs )
		delete m_pCMapDirs;

	if ( m_pCidToUnicodeCache )
		delete m_pCidToUnicodeCache;
	if ( m_pUnicodeToUnicodeCache )
		delete m_pUnicodeToUnicodeCache;
	if ( m_pUnicodeMapCache )
		delete m_pUnicodeMapCache;
	if ( m_pCMapCache )
		delete m_pCMapCache;

	DeleteCriticalSection( &m_oCS );
	DeleteCriticalSection( &m_oUnicodeMapCacheCS );
	DeleteCriticalSection( &m_oCMapCacheCS );
}

//-------------------------------------------------------------------------------------------------------------------------------

void GlobalParams::SetBaseDir(char *sDir) 
{
	lockGlobalParams;
	if ( baseDir )
		delete baseDir;
	baseDir = new StringExt( sDir );
	unlockGlobalParams;
}

void GlobalParams::SetupBaseFonts(wchar_t *wsDir)
{
	// Ищем директорию с фонтами (обычно это C:\Windows\Fonts)
	wchar_t wsWinFontDir[_MAX_PATH];
	wsWinFontDir[0] = '\0';
	if ( !SHGetSpecialFolderPathW( NULL, wsWinFontDir, CSIDL_FONTS, FALSE ) )
		wsWinFontDir[0] = '\0';

	for (int nIndex = 0; c_arroDisplayFontTable[nIndex].sName; ++nIndex) 
	{
		StringExt *seFontName = new StringExt( c_arroDisplayFontTable[nIndex].sName );
		if ( GetDisplayFont( seFontName ) ) 
		{
			if ( seFontName )
				delete seFontName;
			continue;
		}

		CStringW wsFileName = _T("");
		BOOL bNeedDelete = FALSE;
		DisplayFontParamType eType = displayFontT1;
		FILE *pFile = NULL;

		// Сначала пытаемся достать фонты из ресурсов:
		HMODULE hModule = _AtlBaseModule.GetModuleInstance();
		if ( hModule )
		{
			HRSRC hResInfo = ::FindResource( hModule, MAKEINTRESOURCE(c_arroDisplayFontTable[nIndex].nResID), RT_RCDATA );
			if ( hResInfo )
			{
				DWORD dwResourceSize = ::SizeofResource( hModule, hResInfo );
				HGLOBAL hGlobal = ::LoadResource( hModule, hResInfo );
				if ( hGlobal )
				{
					LPVOID pData = ::LockResource( hGlobal );
					FILE *pTempFile = NULL;
					USES_CONVERSION;
					wchar_t wsTempName[MAX_PATH];
					_wsplitpath( A2W(c_arroDisplayFontTable[nIndex].sT1FileName), NULL, NULL, wsTempName, NULL );
					CString wsTempN = _T(".") + CString(wsTempName);

					if ( OpenTempFile( &wsFileName, &pTempFile, _T("wb"), (wchar_t *)wsTempN.GetBuffer()/*_T(".pfb")*//*NULL*/, m_sTempDirectory.GetBuffer(), NULL/*A2W(c_arroDisplayFontTable[nIndex].sT1FileName)*/ ) ) 
					{
						bool bResult = DecryptResource( (BYTE*)pData, dwResourceSize, pTempFile );
						::fclose( pTempFile );
						bNeedDelete = TRUE;

						if ( !bResult )
						{
							bNeedDelete = FALSE;
							_wunlink( wsFileName.GetBuffer() );
							wsFileName = _T("");
						}
					}
				}
			}
		}

		if ( _T("") == wsFileName && wsDir ) 
		{
			USES_CONVERSION;
			wsFileName = AppendToPath( CString( wsDir ), A2W( c_arroDisplayFontTable[nIndex].sT1FileName ) );
			//seFileName = AppendToPath(new StringExt( sDir ), c_arroDisplayFontTable[nIndex].sT1FileName );
			eType = displayFontT1;
			if ( ( pFile = _wfopen( wsFileName.GetBuffer(), _T("rb") ) ) ) 
			{
				fclose(pFile);
			} 
			else 
			{
				wsFileName = _T("");
			}
		}
		if ( _T("") == wsFileName && wsWinFontDir[0] && c_arroDisplayFontTable[nIndex].sTTFileName ) 
		{
			USES_CONVERSION;
			wsFileName = AppendToPath( CString( wsWinFontDir ), A2W( c_arroDisplayFontTable[nIndex].sTTFileName ) );
			//seFileName = AppendToPath(new StringExt( sWinFontDir ), c_arroDisplayFontTable[nIndex].sTTFileName);
			eType = displayFontTT;
			if ( ( pFile = _wfopen( wsFileName.GetBuffer(), _T("rb") ) ) ) 
			{
				fclose(pFile);
			} 
			else 
			{
				wsFileName = _T("");
			}
		}

		// Если мы так и не нашли фонт, тогда ищем его в стандартных директориях.
		if ( c_arroDisplayFontTable[nIndex].sTTFileName ) 
		{
			for ( int nDirIndex = 0; _T("") == wsFileName && c_arrsDisplayFontDirs[nDirIndex]; ++nDirIndex ) 
			{
				USES_CONVERSION;
				wsFileName = AppendToPath( CString( c_arrsDisplayFontDirs[nDirIndex] ), A2W( c_arroDisplayFontTable[nIndex].sTTFileName ) );
				//seFileName = AppendToPath(new StringExt(c_arrsDisplayFontDirs[nDirIndex]), c_arroDisplayFontTable[nIndex].sTTFileName );
				eType = displayFontTT;
				if ( ( pFile = _wfopen( wsFileName.GetBuffer(), _T("rb") ) ) ) 
				{
					fclose(pFile);
				} 
				else 
				{
					wsFileName = _T("");
				}
			}
		}
		if ( _T("") == wsFileName ) 
		{
			// TO DO: Error "No display font"
			if ( seFontName )
				delete seFontName;
			continue;
		}
		DisplayFontParam *pDisplayFontParam = new DisplayFontParam( seFontName, eType, bNeedDelete );

		CStringW *pwsFileName = new CStringW( wsFileName );
		pDisplayFontParam->m_oT1.pwsFileName = pwsFileName;

		this->AddDisplayFont( pDisplayFontParam );
	}

	if ( wsWinFontDir[0] ) 
	{
		m_pWinFontList = new WinFontList( wsWinFontDir );
	}
}

void GlobalParams::SetCMapsDir(wchar_t *wsCMapDir)
{
	HKEY	hKey;
	wchar_t wsCMapsDir[_MAX_PATH];
	wchar_t wsDllPath[_MAX_PATH];		
	wchar_t wsDrive[_MAX_PATH];
	wchar_t wsDir[_MAX_PATH];

	DWORD dwType          = REG_SZ;
	DWORD dwMaxLengthSize = sizeof( wsDllPath );

	// Ищем в реестре, где зарегистрирована библиотека AVSPDFReader.dll
	if ( ERROR_SUCCESS == RegOpenKeyEx( HKEY_CLASSES_ROOT, _T("CLSID\\{A329E783-2E52-4C96-A032-ADD2A5DADD0D}\\InprocServer32"), 0, KEY_READ, &hKey ) )
	{
		if ( ERROR_SUCCESS == RegQueryValueEx( hKey, NULL, 0, &dwType, (LPBYTE)&wsDllPath, &dwMaxLengthSize ) )
		{
			_tsplitpath( wsDllPath, wsDrive, wsDir, NULL, NULL );
			_tcscpy( wsCMapsDir, wsDrive );
			_tcscat( wsCMapsDir, wsDir );
			_tcscat( wsCMapsDir, _T("CMaps\\") );

			m_sCMapDirectory = CString( wsCMapsDir );
		}
		RegCloseKey( hKey );
	} 
	else
		return;

}
bool GlobalParams::DecryptResource(BYTE *pData, DWORD dwDataLen, FILE *pFile)
{
	if ( dwDataLen  < 4 )
		return false;
	LONG lKeyLen = pData[3];
	lKeyLen = ( lKeyLen << 8 ) | pData[2];
	lKeyLen = ( lKeyLen << 8 ) | pData[1];
	lKeyLen = ( lKeyLen << 8 ) | pData[0];

	if ( lKeyLen <= 1 || dwDataLen - 4 - lKeyLen <= 0 )
		return false;

	LONG lFirstKeyLen = lKeyLen  / 2;
	LONG lLastKeyLen  = lKeyLen - lFirstKeyLen;

	BYTE *pCryptKey = new BYTE[lKeyLen];
	if ( !pCryptKey )
		return false;
	::memcpy( pCryptKey, pData + 4, lFirstKeyLen );
	::memcpy( pCryptKey + lFirstKeyLen, pData + dwDataLen - lLastKeyLen, lLastKeyLen  );

	ULONG64 nDataBufferLen = dwDataLen - 4 - lKeyLen;
	BYTE* pDataBuffer = new BYTE[nDataBufferLen + 1];
	if ( !pDataBuffer )
	{
		delete []pCryptKey;
		return false;
	}
	::memcpy( pDataBuffer, pData + 4 + lFirstKeyLen, nDataBufferLen );

	CXMLDecoder oDecoder;

	MediaCore::IAVSMediaData *pKey = NULL;
	::CoCreateInstance(MediaCore::CLSID_CAVSMediaData, NULL, CLSCTX_ALL, MediaCore::IID_IAVSMediaData, (void**)&pKey);
	if ( !pKey )
	{
		delete []pDataBuffer;
		delete []pCryptKey;
		return false;
	}
		
	BYTE *pKeyBuffer = NULL;
	pKey->AllocateBuffer( lKeyLen );
	pKey->get_Buffer( &pKeyBuffer );
	::memcpy( pKeyBuffer, pCryptKey, lKeyLen );
	delete []pCryptKey;

	IUnknown* punkKey = NULL;
	pKey->QueryInterface( IID_IUnknown, (void**)&punkKey );
	RELEASEINTERFACE( pKey );
	if ( !punkKey )
	{
		delete []pCryptKey;
		return false;
	}

	oDecoder.SetBlowfishKey( punkKey );
	oDecoder.DecryptXML( (BYTE**)&pDataBuffer, (DWORD*)&nDataBufferLen );

	RELEASEINTERFACE( punkKey );


	::fwrite( pDataBuffer, 1, nDataBufferLen, pFile );
	delete []pDataBuffer;	

	return true;
}
//-------------------------------------------------------------------------------------------------------------------------------
// Доступ к глобальным переменным
//-------------------------------------------------------------------------------------------------------------------------------

CharCode           GlobalParams::GetMacRomanCharCode(char *sCharName) 
{
	return m_pMacRomanReverseMap->Lookup( sCharName );
}

StringExt         *GlobalParams::GetBaseDir() 
{
	lockGlobalParams;
	StringExt *seBase = baseDir->Copy();
	unlockGlobalParams;
	return seBase;
}

Unicode            GlobalParams::MapNameToUnicode(char *sCharName) 
{
	return m_pNameToUnicode->Lookup( sCharName );
}

UnicodeMap        *GlobalParams::GetResidentUnicodeMap(StringExt *seEncodingName) 
{
	lockGlobalParams;
	UnicodeMap *pMap = (UnicodeMap *)m_pResidentUnicodeMaps->Lookup( seEncodingName );
	unlockGlobalParams;
	
	if ( pMap ) 
	{
		pMap->AddRef();
	}
	return pMap;
}

FILE              *GlobalParams::GetUnicodeMapFile(StringExt *seEncodingName) 
{
	StringExt *seFileName;
	FILE *pFile = NULL;

	lockGlobalParams;
	if ( ( seFileName = (StringExt *)m_pUnicodeMaps->Lookup(seEncodingName) ) ) 
	{
		pFile = fopen( seFileName->GetBuffer(), "r" );
	} 
	else 
	{
		pFile = NULL;
	}
	unlockGlobalParams;

	return pFile;
}

FILE              *GlobalParams::FindCMapFile(StringExt *seCollection, StringExt *seCMapName) 
{
	lockGlobalParams;
	
	if ( _T("") != m_sCMapDirectory )
	{
		CString sFileName = m_sCMapDirectory + CString( seCMapName->GetBuffer() );
		FILE *pFile = _wfopen( sFileName.GetBuffer(), _T("r") );
		if ( pFile ) 
		{
			unlockGlobalParams;
			return pFile;
		}
	}

	CList *pList = NULL;

	if ( !(pList = (CList *)m_pCMapDirs->Lookup(seCollection)) ) 
	{
		unlockGlobalParams;
		return NULL;
	}
	for ( int nIndex = 0; nIndex < pList->GetLength(); ++nIndex ) 
	{
		StringExt *seDir = (StringExt *)pList->GetByIndex(nIndex);
		StringExt *seFileName = AppendToPath( seDir->Copy(), seCMapName->GetBuffer() );
		FILE *pFile = fopen( seFileName->GetBuffer(), "r" );
		delete seFileName;

		if ( pFile ) 
		{
			unlockGlobalParams;
			return pFile;
		}
	}
	unlockGlobalParams;
	return NULL;
}

FILE              *GlobalParams::FindToUnicodeFile(StringExt *seName) 
{
	lockGlobalParams;
	for ( int nIndex = 0; nIndex < m_pToUnicodeDirs->GetLength(); ++nIndex ) 
	{
		StringExt *seDir = (StringExt *)m_pToUnicodeDirs->GetByIndex(nIndex);
		StringExt *seFileName = AppendToPath( seDir->Copy(), seName->GetBuffer() );
		FILE *pFile = fopen( seFileName->GetBuffer(), "r" );
		delete seFileName;

		if ( pFile ) 
		{
			unlockGlobalParams;
			return pFile;
		}
	}
	unlockGlobalParams;
	return NULL;
}

DisplayFontParam  *GlobalParams::GetDisplayFont(StringExt *seFontName) 
{
	lockGlobalParams;
	DisplayFontParam *pDisplayFont = (DisplayFontParam *)m_pDisplayFonts->Lookup( seFontName );
	if ( !pDisplayFont && m_pWinFontList ) 
	{
		pDisplayFont = m_pWinFontList->Find( seFontName );
	}
	unlockGlobalParams;
	return pDisplayFont;
}

DisplayFontParam  *GlobalParams::GetDisplayCIDFont(StringExt *seFontName, StringExt *seCollection) 
{
	DisplayFontParam *pDisplayFont = NULL;

	lockGlobalParams;
	if ( !seFontName || !(pDisplayFont = (DisplayFontParam *)m_pDisplayNamedCIDFonts->Lookup( seFontName )) ) 
	{
		pDisplayFont = (DisplayFontParam *)m_pDisplayCIDFonts->Lookup( seCollection );
	}
	unlockGlobalParams;
	return pDisplayFont;
}


StringExt         *GlobalParams::GetPSFile() 
{
	lockGlobalParams;
	StringExt *seFileName = m_sePSFile ? m_sePSFile->Copy() : (StringExt *)NULL;
	unlockGlobalParams;
	return seFileName;
}

int                GlobalParams::GetPSPaperWidth() 
{
	lockGlobalParams;
	int nWidth = m_nPSPaperWidth;
	unlockGlobalParams;
	return nWidth;
}

int                GlobalParams::GetPSPaperHeight() 
{
	lockGlobalParams;
	int nHeight = m_nPSPaperHeight;
	unlockGlobalParams;
	return nHeight;
}

void               GlobalParams::GetPSImageableArea(int *pnBottomLeftX, int *pnBottomLeftY, int *pnTopRightX, int *pnTopRightY) 
{
	lockGlobalParams;
	*pnBottomLeftX = m_nPSImageableBLX;
	*pnBottomLeftY = m_nPSImageableBLY;
	*pnTopRightX   = m_nPSImageableTRX;
	*pnTopRightY   = m_nPSImageableTRY;
	unlockGlobalParams;
}

BOOL               GlobalParams::GetPSCrop() 
{
	lockGlobalParams;
	BOOL bCrop = m_bPSCrop;
	unlockGlobalParams;
	return bCrop;
}

BOOL               GlobalParams::GetPSExpandSmaller() 
{
	lockGlobalParams;
	BOOL bExpand = m_bPSExpandSmaller;
	unlockGlobalParams;
	return bExpand;
}

BOOL               GlobalParams::GetPSShrinkLarger() 
{
	lockGlobalParams;
	BOOL bShrink = m_bPSShrinkLarger;
	unlockGlobalParams;
	return bShrink;
}

BOOL               GlobalParams::GetPSCenter() 
{
	lockGlobalParams;
	BOOL bCenter = m_bPSCenter;
	unlockGlobalParams;
	return bCenter;
}

BOOL               GlobalParams::GetPSDuplex() 
{
	lockGlobalParams;
	BOOL bDuplex = m_bPSDuplex;
	unlockGlobalParams;
	return bDuplex;
}

PSLevel            GlobalParams::GetPSLevel() 
{
	lockGlobalParams;
	PSLevel eLevel = m_ePSLevel;
	unlockGlobalParams;
	return eLevel;
}

PSFontParam       *GlobalParams::GetPSFont(StringExt *seFontName) 
{
	lockGlobalParams;
	PSFontParam *pFont = (PSFontParam *)m_pPSFonts->Lookup( seFontName );
	unlockGlobalParams;
	return pFont;
}

PSFontParam       *GlobalParams::GetPSFont16(StringExt *seFontName, StringExt *seCollection, int nWMode) 
{
	PSFontParam *pFont = NULL;
	lockGlobalParams;
	if ( seFontName ) 
	{
		for ( int nIndex = 0; nIndex < m_pPSNamedFonts16->GetLength(); ++nIndex ) 
		{
			pFont = (PSFontParam *)m_pPSNamedFonts16->GetByIndex( nIndex );
			if ( !pFont->m_sePDFFontName->Compare(seFontName) && pFont->m_nWMode == nWMode ) 
			{
				break;
			}
			pFont = NULL;
		}
	}
	if ( !pFont && seCollection ) 
	{
		for ( int nIndex = 0; nIndex < m_pPSFonts16->GetLength(); ++nIndex ) 
		{
			pFont = (PSFontParam *)m_pPSFonts16->GetByIndex( nIndex );
			if ( !pFont->m_sePDFFontName->Compare(seCollection) && pFont->m_nWMode == nWMode ) 
			{
				break;
			}
			pFont = NULL;
		}
	}
	unlockGlobalParams;
	return pFont;
}

BOOL               GlobalParams::GetPSEmbedType1() 
{
	lockGlobalParams;
	BOOL bEmbed = m_bPSEmbedType1;
	unlockGlobalParams;
	return bEmbed;
}

BOOL               GlobalParams::GetPSEmbedTrueType() 
{
	lockGlobalParams;
	BOOL bEmbed = m_bPSEmbedTrueType;
	unlockGlobalParams;
	return bEmbed;
}

BOOL               GlobalParams::GetPSEmbedCIDPostScript() 
{
	lockGlobalParams;
	BOOL bEmbed = m_bPSEmbedCIDPostScript;
	unlockGlobalParams;
	return bEmbed;
}

BOOL               GlobalParams::GetPSEmbedCIDTrueType() 
{
	lockGlobalParams;
	BOOL bEmbed = m_bPSEmbedCIDTrueType;
	unlockGlobalParams;
	return bEmbed;
}

BOOL               GlobalParams::GetPSPreload() 
{
	lockGlobalParams;
	BOOL bPreload = m_bPSPreload;
	unlockGlobalParams;
	return bPreload;
}

BOOL               GlobalParams::GetPSOPI() 
{
	lockGlobalParams;
	BOOL bOpi = m_bPSOPI;
	unlockGlobalParams;
	return bOpi;
}

BOOL               GlobalParams::GetPSASCIIHex() 
{
	lockGlobalParams;
	BOOL bASKIIHex = m_bPSASCIIHex;
	unlockGlobalParams;
	return bASKIIHex;
}

StringExt         *GlobalParams::GetTextEncodingName() 
{
	lockGlobalParams;
	StringExt *seName = m_seTextEncoding->Copy();
	unlockGlobalParams;
	return seName;
}

EndOfLineType      GlobalParams::GetTextEOL() 
{
	lockGlobalParams;
	EndOfLineType eEOL = m_eTextEOL;
	unlockGlobalParams;
	return eEOL;
}

BOOL               GlobalParams::GetTextPageBreaks() 
{
	lockGlobalParams;
	BOOL bPageBreaks = m_bTextPageBreaks;
	unlockGlobalParams;
	return bPageBreaks;
}

BOOL               GlobalParams::GetTextKeepTinyChars() 
{
	lockGlobalParams;
	BOOL bTiny = m_bTextKeepTinyChars;
	unlockGlobalParams;
	return bTiny;
}

CStringW           GlobalParams::FindFontFile(StringExt *seFontName, wchar_t **pwsExts) 
{
	lockGlobalParams;
	for ( int nIndex = 0; nIndex < m_pFontDirs->GetLength(); ++nIndex ) 
	{
		StringExt *seDir = (StringExt *)m_pFontDirs->GetByIndex(nIndex);
		wchar_t **pwsCurExt = NULL;
		for ( pwsCurExt = pwsExts; *pwsCurExt; ++pwsCurExt ) 
		{
			USES_CONVERSION;
			CStringW wsFileName = AppendToPath( A2W( seDir->GetBuffer() ), A2W( seFontName->GetBuffer() ) );
			//StringExt *seFileName = AppendToPath( seDir->Сopy(), seFontName->GetBuffer() );
			wsFileName.Append( *pwsCurExt );
			FILE *pFile = NULL;
			if ( ( pFile = _wfopen( wsFileName.GetBuffer(), _T("rb") ) ) ) 
			{
				fclose( pFile );
				unlockGlobalParams;
				return wsFileName;
			}
		}
	}
	unlockGlobalParams;
	return NULL;
}

StringExt         *GlobalParams::GetInitialZoom() 
{
	lockGlobalParams;
	StringExt *seZoom = m_seInitialZoom->Copy();
	unlockGlobalParams;
	return seZoom;
}

BOOL               GlobalParams::GetContinuousView() 
{
	lockGlobalParams;
	BOOL bView = m_bContinuousView;
	unlockGlobalParams;
	return bView;
}

BOOL               GlobalParams::GetEnableGlyphLib() 
{
	lockGlobalParams;
	BOOL bGlyphLib = m_bEnableGlyphLib;
	unlockGlobalParams;
	return bGlyphLib;
}


BOOL               GlobalParams::GetAntialias() 
{
	lockGlobalParams;
	BOOL bAntialias = m_bAntialias;
	unlockGlobalParams;
	return bAntialias;
}

BOOL               GlobalParams::GetVectorAntialias() 
{
	lockGlobalParams;
	BOOL bVectorAA = m_bVectorAntialias;
	unlockGlobalParams;
	return bVectorAA;
}

BOOL               GlobalParams::GetStrokeAdjust() 
{
	lockGlobalParams;
	BOOL bStrokeAdjust = m_bStrokeAdjust;
	unlockGlobalParams;
	return bStrokeAdjust;
}

ScreenType         GlobalParams::GetScreenType() 
{
	lockGlobalParams;
	ScreenType eType = m_eScreenType;
	unlockGlobalParams;
	return eType;
}

int                GlobalParams::GetScreenSize() 
{
	lockGlobalParams;
	int nSize = m_nScreenSize;
	unlockGlobalParams;
	return nSize;
}

int                GlobalParams::GetScreenDotRadius() 
{
	lockGlobalParams;
	int nRadius = m_nScreenDotRadius;
	unlockGlobalParams;
	return nRadius;
}

double             GlobalParams::GetScreenGamma() 
{
	lockGlobalParams;
	double dGamma = m_dScreenGamma;
	unlockGlobalParams;
	return dGamma;
}

double             GlobalParams::GetScreenBlackThreshold() 
{
	lockGlobalParams;
	double dThreshold = m_dScreenBlackThreshold;
	unlockGlobalParams;
	return dThreshold;
}

double             GlobalParams::GetScreenWhiteThreshold() 
{
	lockGlobalParams;
	double dTreshold = m_dScreenWhiteThreshold;
	unlockGlobalParams;
	return dTreshold;
}

BOOL               GlobalParams::GetMapNumericCharNames() 
{
	lockGlobalParams;
	BOOL bMap = m_bMapNumericCharNames;
	unlockGlobalParams;
	return bMap;
}

BOOL               GlobalParams::GetMapUnknownCharNames() 
{
	lockGlobalParams;
	BOOL bMap = m_bMapUnknownCharNames;
	unlockGlobalParams;
	return bMap;
}

BOOL               GlobalParams::GetPrintCommands() 
{
	lockGlobalParams;
	BOOL bPrint = m_bPrintCommands;
	unlockGlobalParams;
	return bPrint;
}

BOOL               GlobalParams::GetErrQuiet() 
{
	return m_bErrQuiet;
}

CharCodeToUnicode *GlobalParams::GetCIDToUnicode(StringExt *seCollection) 
{
	StringExt *seFileName = NULL;
	CharCodeToUnicode *pCharCodeToUnicode = NULL;

	lockGlobalParams;
	if ( !( pCharCodeToUnicode = m_pCidToUnicodeCache->GetCharCodeToUnicode( seCollection ) ) ) 
	{
		if ( ( seFileName = (StringExt *)m_pCidToUnicodes->Lookup( seCollection )) && ( pCharCodeToUnicode = CharCodeToUnicode::ParseCIDToUnicode( seFileName, seCollection) ) ) 
		{
			m_pCidToUnicodeCache->Add( pCharCodeToUnicode );
		}
	}
	unlockGlobalParams;
	return pCharCodeToUnicode;
}

CharCodeToUnicode *GlobalParams::GetUnicodeToUnicode(StringExt *seFontName) 
{
	CharCodeToUnicode *pCharCodeToUnicode;
	lockGlobalParams;

	StringExt *seFileName = NULL, *seFontPattern = NULL;
	THashIter *pIter;
	m_pUnicodeToUnicodes->StartIter(&pIter);
	while ( m_pUnicodeToUnicodes->GetNext(&pIter, &seFontPattern, (void **)&seFileName) ) 
	{
		if ( strstr( seFontName->GetBuffer(), seFontPattern->GetBuffer() ) ) 
		{
			m_pUnicodeToUnicodes->DeleteIter(&pIter);
			break;
		}
		seFileName = NULL;
	}
	if ( seFileName ) 
	{
		if ( !( pCharCodeToUnicode = m_pUnicodeToUnicodeCache->GetCharCodeToUnicode(seFileName) ) ) 
		{
			if ( ( pCharCodeToUnicode = CharCodeToUnicode::ParseUnicodeToUnicode(seFileName) ) ) 
			{
				m_pUnicodeToUnicodeCache->Add( pCharCodeToUnicode );
			}
		}
	}
	else 
	{
		pCharCodeToUnicode = NULL;
	}
	unlockGlobalParams;
	return pCharCodeToUnicode;
}

UnicodeMap        *GlobalParams::GetUnicodeMap(StringExt *seEncodingName) 
{
	return GetUnicodeMap2( seEncodingName );
}

UnicodeMap        *GlobalParams::GetUnicodeMap2(StringExt *seEncodingName) 
{
	UnicodeMap *pMap = NULL;

	if ( !( pMap = GetResidentUnicodeMap( seEncodingName ) ) ) 
	{
		lockUnicodeMapCache;
		pMap = m_pUnicodeMapCache->GetUnicodeMap( seEncodingName, this );
		unlockUnicodeMapCache;
	}
	return pMap;
}

CMap              *GlobalParams::GetCMap(StringExt *seCollection, StringExt *seCMapName, wchar_t *wsFilePath) 
{
	lockCMapCache;
	CMap *pCMap = m_pCMapCache->GetCMap( seCollection, seCMapName, this, wsFilePath );
	unlockCMapCache;
	return pCMap;
}

UnicodeMap        *GlobalParams::GetTextEncoding() 
{
	return GetUnicodeMap2( m_seTextEncoding );
}

//-------------------------------------------------------------------------------------------------------------------------------
// Устанавливаем глобальные переменные
//-------------------------------------------------------------------------------------------------------------------------------

void GlobalParams::AddDisplayFont(DisplayFontParam *pFontParam) 
{
	DisplayFontParam *pOld = NULL;

	lockGlobalParams;
	if ( ( pOld = (DisplayFontParam *)m_pDisplayFonts->Remove( pFontParam->m_seName ) ) ) 
	{
		delete pOld;
	}
	m_pDisplayFonts->Add( pFontParam->m_seName, pFontParam );
	unlockGlobalParams;
}

void GlobalParams::SetPSFile(char *sFileName) 
{
	lockGlobalParams;
	if ( m_sePSFile ) 
	{
		delete m_sePSFile;
	}
	m_sePSFile = new StringExt( sFileName );
	unlockGlobalParams;
}

BOOL GlobalParams::SetPSPaperSize(char *sSize) 
{
	lockGlobalParams;
	if ( !strcmp(sSize, "match") ) 
	{
		m_nPSPaperWidth = m_nPSPaperHeight = -1;
	} 
	else if ( !strcmp(sSize, "letter") ) 
	{
		m_nPSPaperWidth = 612;
		m_nPSPaperHeight = 792;
	} 
	else if ( !strcmp(sSize, "legal") ) 
	{
		m_nPSPaperWidth = 612;
		m_nPSPaperHeight = 1008;
	} 
	else if ( !strcmp(sSize, "A4") ) 
	{
		m_nPSPaperWidth = 595;
		m_nPSPaperHeight = 842;
	} 
	else if ( !strcmp(sSize, "A3") ) 
	{
		m_nPSPaperWidth = 842;
		m_nPSPaperHeight = 1190;
	} 
	else 
	{
		unlockGlobalParams;
		return FALSE;
	}
	m_nPSImageableBLX = m_nPSImageableBLY = 0;
	m_nPSImageableTRX = m_nPSPaperWidth;
	m_nPSImageableTRY = m_nPSPaperHeight;
	unlockGlobalParams;
	return TRUE;
}

void GlobalParams::SetPSPaperWidth(int nWidth) 
{
	lockGlobalParams;
	m_nPSPaperWidth = nWidth;
	m_nPSImageableBLX = 0;
	m_nPSImageableTRX = m_nPSPaperWidth;
	unlockGlobalParams;
}

void GlobalParams::SetPSPaperHeight(int nHeight) 
{
	lockGlobalParams;
	m_nPSPaperHeight = nHeight;
	m_nPSImageableBLY = 0;
	m_nPSImageableTRY = m_nPSPaperHeight;
	unlockGlobalParams;
}

void GlobalParams::SetPSImageableArea(int nBottomLeftX, int nBottomLeftY, int nTopRightX, int nTopRightY)
{
	lockGlobalParams;
	m_nPSImageableBLX = nBottomLeftX;
	m_nPSImageableBLY = nBottomLeftY;
	m_nPSImageableTRX = nTopRightX;
	m_nPSImageableTRY = nTopRightY;
	unlockGlobalParams;
}

void GlobalParams::SetPSCrop(BOOL bCrop) 
{
	lockGlobalParams;
	m_bPSCrop = bCrop;
	unlockGlobalParams;
}

void GlobalParams::SetPSExpandSmaller(BOOL bExpand) 
{
	lockGlobalParams;
	m_bPSExpandSmaller = bExpand;
	unlockGlobalParams;
}

void GlobalParams::SetPSShrinkLarger(BOOL bShrink) 
{
	lockGlobalParams;
	m_bPSShrinkLarger = bShrink;
	unlockGlobalParams;
}

void GlobalParams::SetPSCenter(BOOL bCenter) 
{
	lockGlobalParams;
	m_bPSCenter = bCenter;
	unlockGlobalParams;
}

void GlobalParams::SetPSDuplex(BOOL bDuplex) 
{
	lockGlobalParams;
	m_bPSDuplex = bDuplex;
	unlockGlobalParams;
}

void GlobalParams::SetPSLevel(PSLevel eLevel) 
{
	lockGlobalParams;
	m_ePSLevel = eLevel;
	unlockGlobalParams;
}

void GlobalParams::SetPSEmbedType1(BOOL bEmbed) 
{
	lockGlobalParams;
	m_bPSEmbedType1 = bEmbed;
	unlockGlobalParams;
}

void GlobalParams::SetPSEmbedTrueType(BOOL bEmbed) 
{
	lockGlobalParams;
	m_bPSEmbedTrueType = bEmbed;
	unlockGlobalParams;
}

void GlobalParams::SetPSEmbedCIDPostScript(BOOL bEmbed) 
{
	lockGlobalParams;
	m_bPSEmbedCIDPostScript = bEmbed;
	unlockGlobalParams;
}

void GlobalParams::SetPSEmbedCIDTrueType(BOOL bEmbed) 
{
	lockGlobalParams;
	m_bPSEmbedCIDTrueType = bEmbed;
	unlockGlobalParams;
}

void GlobalParams::SetPSPreload(BOOL bPreload) 
{
	lockGlobalParams;
	m_bPSPreload = bPreload;
	unlockGlobalParams;
}

void GlobalParams::SetPSOPI(BOOL bOpi) 
{
	lockGlobalParams;
	m_bPSOPI = bOpi;
	unlockGlobalParams;
}

void GlobalParams::SetPSASCIIHex(BOOL bHex) 
{
	lockGlobalParams;
	m_bPSASCIIHex = bHex;
	unlockGlobalParams;
}

void GlobalParams::SetTextEncoding(char *sEncodingName) 
{
	lockGlobalParams;
	if ( m_seTextEncoding )
		delete m_seTextEncoding;
	m_seTextEncoding = new StringExt( sEncodingName );
	unlockGlobalParams;
}

BOOL GlobalParams::SetTextEOL(char *sEOL) 
{
	lockGlobalParams;
	if ( !strcmp(sEOL, "unix") ) 
	{
		m_eTextEOL = eolUnix;
	} 
	else if ( !strcmp(sEOL, "dos") ) 
	{
		m_eTextEOL = eolDOS;
	} 
	else if ( !strcmp(sEOL, "mac") ) 
	{
		m_eTextEOL = eolMac;
	} 
	else 
	{
		unlockGlobalParams;
		return FALSE;
	}
	unlockGlobalParams;
	return TRUE;
}

void GlobalParams::SetTextPageBreaks(BOOL bPageBreaks) 
{
	lockGlobalParams;
	m_bTextPageBreaks = bPageBreaks;
	unlockGlobalParams;
}

void GlobalParams::SetTextKeepTinyChars(BOOL bKeep) 
{
	lockGlobalParams;
	m_bTextKeepTinyChars = bKeep;
	unlockGlobalParams;
}

void GlobalParams::SetInitialZoom(char *sZoom) 
{
	lockGlobalParams;
	if ( m_seInitialZoom )
		delete m_seInitialZoom;
	m_seInitialZoom = new StringExt( sZoom );
	unlockGlobalParams;
}

void GlobalParams::SetContinuousView(BOOL bContView) 
{
	lockGlobalParams;
	m_bContinuousView = bContView;
	unlockGlobalParams;
}

BOOL GlobalParams::SetEnableGlyphLib(char *sGlyphLib) 
{
	lockGlobalParams;
	BOOL bResult = ParseYesNo2( sGlyphLib, &m_bEnableGlyphLib );
	unlockGlobalParams;
	return bResult;
}


BOOL GlobalParams::SetAntialias(char *sAntialias) 
{
	lockGlobalParams;
	BOOL bResult = ParseYesNo2( sAntialias, &m_bAntialias );
	unlockGlobalParams;
	return bResult;
}

BOOL GlobalParams::SetVectorAntialias(char *sVectorAA) 
{
	lockGlobalParams;
	BOOL bResult = ParseYesNo2( sVectorAA, &m_bVectorAntialias );
	unlockGlobalParams;
	return bResult;
}

void GlobalParams::SetScreenType(ScreenType eType) 
{
	lockGlobalParams;
	m_eScreenType = eType;
	unlockGlobalParams;
}

void GlobalParams::SetScreenSize(int nSize) 
{
	lockGlobalParams;
	m_nScreenSize = nSize;
	unlockGlobalParams;
}

void GlobalParams::SetScreenDotRadius(int nRadius) 
{
	lockGlobalParams;
	m_nScreenDotRadius = nRadius;
	unlockGlobalParams;
}

void GlobalParams::SetScreenGamma(double dGamma) 
{
	lockGlobalParams;
	m_dScreenGamma = dGamma;
	unlockGlobalParams;
}

void GlobalParams::SetScreenBlackThreshold(double dTreshold) 
{
	lockGlobalParams;
	m_dScreenBlackThreshold = dTreshold;
	unlockGlobalParams;
}

void GlobalParams::SetScreenWhiteThreshold(double dTreshold) 
{
	lockGlobalParams;
	m_dScreenWhiteThreshold = dTreshold;
	unlockGlobalParams;
}

void GlobalParams::SetMapNumericCharNames(BOOL bMap) 
{
	lockGlobalParams;
	m_bMapNumericCharNames = bMap;
	unlockGlobalParams;
}

void GlobalParams::SetMapUnknownCharNames(BOOL bMap) 
{
	lockGlobalParams;
	m_bMapUnknownCharNames = bMap;
	unlockGlobalParams;
}

void GlobalParams::SetPrintCommands(BOOL bPrintCommands) 
{
	lockGlobalParams;
	m_bPrintCommands = bPrintCommands;
	unlockGlobalParams;
}

void GlobalParams::SetErrQuiet(BOOL bErrQuiet) 
{
	lockGlobalParams;
	m_bErrQuiet = bErrQuiet;
	unlockGlobalParams;
}
