#ifndef _FONT_UTILS_H
#define _FONT_UTILS_H

#include FT_SFNT_NAMES_H

#define fabs(X) ( X >= 0 ? X : -X )

namespace FontConstants
{
	enum FontStyle
	{
		FontStyleRegular    = 0,
		FontStyleBold       = 1,
		FontStyleItalic     = 2,
		FontStyleBoldItalic = 3,
		FontStyleUnderline  = 4,
		FontStyleStrikeout  = 8
	};
}


#define UNKNOWN_CHARSET 3 // для случаев, когда задано значение DEFAULT_CHARSET, но 
                          // на самом деле charset не учитывается

//---------------------------------------------------------------------------------------------------

#define MAX_FONT_CACHE_SIZE 16
#define MAX_FONT_NAME_LEN   50
#define MAX_FONT_STYLE_LEN  40

static long GetNextNameValue(HKEY key, LPCTSTR pszSubkey, LPTSTR pszName, LPTSTR pszData)
{
	static HKEY hkey = NULL;	// registry handle, kept open between calls
	static DWORD dwIndex = 0;	// count of values returned
	LONG retval;

	// if all parameters are NULL then close key
	if (pszSubkey == NULL && pszName == NULL && pszData == NULL)
	{
		if (hkey)
			RegCloseKey(hkey);
		hkey = NULL;
		return ERROR_SUCCESS;
	}

	// if subkey is specified then open key (first time)
	if (pszSubkey && pszSubkey[0] != 0)
	{
		retval = RegOpenKeyEx(key, pszSubkey, 0, KEY_READ, &hkey);
		if (retval != ERROR_SUCCESS)
		{
			return retval;
		}
		dwIndex = 0;
	}
	else
	{
		dwIndex++;
	}

	_ASSERTE(pszName != NULL && pszData != NULL);

	*pszName = 0;
	*pszData = 0;

	TCHAR szValueName[MAX_PATH];
	DWORD dwValueNameSize = sizeof(szValueName)-1;
	BYTE szValueData[MAX_PATH];
	DWORD dwValueDataSize = sizeof(szValueData)-1;
	DWORD dwType = 0;

	retval = RegEnumValue(hkey, dwIndex, szValueName, &dwValueNameSize, NULL,
		&dwType, szValueData, &dwValueDataSize);
	if (retval == ERROR_SUCCESS)
	{
		lstrcpy(pszName, (LPTSTR)szValueName);
		lstrcpy(pszData, (LPTSTR)szValueData);
	}

	return retval;
}



static FT_Error FT_New_FaceW(FT_Library pLibrary, wchar_t *wsFilePath, FT_Long lIndex, FT_Face *pFace)
{
	USES_CONVERSION;
	FT_Open_Args oOpenArgs;
	oOpenArgs.flags    = FT_OPEN_PATHNAME;
	oOpenArgs.pathname = W2A( wsFilePath );

	FT_Parameter *pParams = (FT_Parameter *)::malloc( sizeof(FT_Parameter) * 4 );
	pParams[0].tag  = FT_MAKE_TAG( 'i', 'g', 'p', 'f' );
	pParams[0].data = NULL;
	pParams[1].tag  = FT_MAKE_TAG( 'i', 'g', 'p', 's' );
	pParams[1].data = NULL; 
	pParams[2].tag  = FT_PARAM_TAG_IGNORE_PREFERRED_FAMILY;
	pParams[2].data = NULL; 
	pParams[3].tag  = FT_PARAM_TAG_IGNORE_PREFERRED_SUBFAMILY;
	pParams[3].data = NULL; 

	oOpenArgs.num_params = 4;
	oOpenArgs.params     = pParams;

	int nError  = FT_Open_Face( pLibrary, &oOpenArgs, lIndex, pFace );

	::free( pParams );
	return nError;
}

static int  GetDefaultCharset(BOOL bUseDefCharset = TRUE)
{
	if ( !bUseDefCharset )
		return UNKNOWN_CHARSET;

	LOCALESIGNATURE LocSig;
	GetLocaleInfo( GetSystemDefaultLCID(), LOCALE_FONTSIGNATURE, (LPWSTR)&LocSig, sizeof(LocSig) / sizeof(TCHAR) );

	if ( LocSig.lsCsbDefault[0] & 1 )
		return 0;
	else if ( LocSig.lsCsbDefault[0] & 2 )
		return 238;
	else if ( LocSig.lsCsbDefault[0] & 4 )
		return 204;
	else if ( LocSig.lsCsbDefault[0] & 8 )
		return 161;
	else if ( LocSig.lsCsbDefault[0] & 16 )
		return 162;
	else if ( LocSig.lsCsbDefault[0] & 32 )
		return 177;
	else if ( LocSig.lsCsbDefault[0] & 64 )
		return 178;
	else if ( LocSig.lsCsbDefault[0] & 128 )
		return 186;
	else if ( LocSig.lsCsbDefault[0] & 256 )
		return 163;
	else if ( LocSig.lsCsbDefault[0] & 0x10000 )
		return 222;
	else if ( LocSig.lsCsbDefault[0] & 0x20000 )
		return 128;
	else if ( LocSig.lsCsbDefault[0] & 0x40000 )
		return 134;
	else if ( LocSig.lsCsbDefault[0] & 0x80000 )
		return 129;
	else if ( LocSig.lsCsbDefault[0] & 0x100000 )
		return 136;
	else if ( LocSig.lsCsbDefault[0] & 0x200000 )
		return 130;
	else if ( LocSig.lsCsbDefault[0] & 0x20000000 )
		return 77;
	else if ( LocSig.lsCsbDefault[0] & 0x40000000 )
		return 255;
	else if ( LocSig.lsCsbDefault[0] & 0x80000000 )
		return 2;

	return 0;
}

static void GetCodePageByCharset(unsigned char unCharset, unsigned long *pulBit, unsigned int *punLongIndex)
{
	// Данная функция возвращает параметры, которые нужно посылать на вход 
	// функции AVSFontManager::IsUnicodeRangeAvailable


	// Соответствие Charset -> Codepage: http://support.microsoft.com/kb/165478
	// http://msdn.microsoft.com/en-us/library/cc194829.aspx

	//  Charset Name       Charset Value(hex)  Codepage number
	//  ------------------------------------------------------
	//
	//  DEFAULT_CHARSET           1 (x01)
	//  SYMBOL_CHARSET            2 (x02)
	//  OEM_CHARSET             255 (xFF)
	//  ANSI_CHARSET              0 (x00)            1252
	//  RUSSIAN_CHARSET         204 (xCC)            1251
	//  EASTEUROPE_CHARSET      238 (xEE)            1250
	//  GREEK_CHARSET           161 (xA1)            1253
	//  TURKISH_CHARSET         162 (xA2)            1254
	//  BALTIC_CHARSET          186 (xBA)            1257
	//  HEBREW_CHARSET          177 (xB1)            1255
	//  ARABIC _CHARSET         178 (xB2)            1256
	//  SHIFTJIS_CHARSET        128 (x80)             932
	//  HANGEUL_CHARSET         129 (x81)             949
	//  GB2313_CHARSET          134 (x86)             936
	//  CHINESEBIG5_CHARSET     136 (x88)             950
	//  THAI_CHARSET            222 (xDE)             874	
	//  JOHAB_CHARSET	        130 (x82)            1361
	//  VIETNAMESE_CHARSET      163 (xA3)            1258
	//  MAC_CHARSET              77 (x4D)            

	// Соответсвие CodePage -> ulCodePageRange1 : http://www.microsoft.com/Typography/otspec/os2.htm#cpr

	if ( punLongIndex )
		*punLongIndex = 4;

	if ( unCharset == DEFAULT_CHARSET )
		unCharset = GetDefaultCharset();
	
	if ( pulBit )
	{
		switch( unCharset )
		{
		case 0x00: *pulBit =  0; break;
		case 0xEE: *pulBit =  1; break;
		case 0xCC: *pulBit =  2; break;
		case 0xA1: *pulBit =  3; break;
		case 0xA2: *pulBit =  4; break;
		case 0xB1: *pulBit =  5; break;
		case 0xB2: *pulBit =  6; break;
		case 0xBA: *pulBit =  7; break;
		case 0xA3: *pulBit =  8; break;
		case 0xDE: *pulBit = 16; break;
		case 0x80: *pulBit = 17; break;
		case 0x86: *pulBit = 18; break;
		case 0x81: *pulBit = 19; break;
		case 0x88: *pulBit = 20; break;
		case 0x82: *pulBit = 21; break;
		case 0x4D: *pulBit = 29; break;
		case 0x02: *pulBit = 31; break;
		case 0xFF: *pulBit = 30; break;
		default:   *pulBit =  0; break;
		}
	}


}
#endif /* _FONT_UTILS_H */