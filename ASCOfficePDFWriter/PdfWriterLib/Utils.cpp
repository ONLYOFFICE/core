#include "Utils.h"
#include <vector>

// Следующие функции нужны для поиска FontFile по названию фонта
#define WUNKNOWNSTR	_T("unknown Windows version")

#define W95STR		_T("Windows 95")
#define W95SP1STR	_T("Windows 95 SP1")
#define W95OSR2STR	_T("Windows 95 OSR2")
#define W98STR		_T("Windows 98")
#define W98SP1STR	_T("Windows 98 SP1")
#define W98SESTR	_T("Windows 98 SE")
#define WMESTR		_T("Windows ME")

#define WNT351STR	_T("Windows NT 3.51")
#define WNT4STR		_T("Windows NT 4")
#define W2KSTR		_T("Windows 2000")
#define WXPSTR		_T("Windows XP")
#define WVSTR       _T("Windows Vista")
#define W7STR       _T("Windows 7")
#define W8STR		_T("Windows 8")

#define WCESTR		_T("Windows CE")


#define WUNKNOWN	0

#define W9XFIRST	1
#define W95			1
#define W95SP1		2
#define W95OSR2		3
#define W98			4
#define W98SP1		5
#define W98SE		6
#define WME			7
#define W9XLAST		99

#define WNTFIRST	101
#define WNT351		101
#define WNT4		102
#define W2K			103
#define WXP			104
#define WVista      105
#define W7          106
#define W8          107
#define WNTLAST		199

#define WCEFIRST	201
#define WCE			201
#define WCELAST		299


int UtilsAToI(const char*  str)
{
	BOOL bFlag = FALSE;
    int  nRes = 0;

    // ищем первый не пустой символ

	while( *str ) 
	{
        if ( IS_WHITE_SPACE(*str) )
            str++;
        else 
		{
            if ( *str == '-') 
			{
                bFlag = TRUE;
                str++;
			}
            break;
        }
    }

    while (*str >= '0' && *str <= '9') 
	{
        nRes *= 10;
        nRes += *str - '0';
        str++;
    }

    if ( bFlag )
        nRes *= -1;

    return nRes;
}

double UtilsAToF(const char  *str)
{
    BOOL bFlag = FALSE;
    int nVal = 0;
    double dRes = 0;
    int nTmp = 1;

    // ищем первый не пустой символ

    while (*str) 
	{
        if ( IS_WHITE_SPACE(*str) )
            str++;
        else 
		{
            if (*str == '-') {
                bFlag = TRUE;
                str++;
            }
            break;
        }
    }

    while (*str >= '0' && *str <= '9') 
	{
        if (nVal > 3276)
            break;

        nVal *= 10;
        nVal += *str - '0';
        str++;
    }

    if (*str == '.') 
	{
        str++;
        while (*str >= '0' && *str <= '9') 
		{
            if (nVal > 214748364)
                break;

            nVal *= 10;
            nVal += *str - '0';
            str++;
            nTmp *= 10;
        }
    }

    dRes = (double) nVal / nTmp;

    if ( bFlag )
        dRes *= -1;

    return dRes;
}
char* UtilsIToA(char *str, int nVal, char *eptr)
{
    char* sTemp;
    char sBuf[INT_LEN + 1];

    if ( nVal < 0) 
	{
        if ( nVal < LIMIT_MIN_INT )
            nVal = LIMIT_MIN_INT;
        *str++ = '-';
        nVal = - nVal;
    } 
	else if ( nVal > LIMIT_MAX_INT ) 
	{
        nVal = LIMIT_MAX_INT;
    }
	else if ( nVal == 0) 
	{
        *str++ = '0';
    }

    sTemp = sBuf + INT_LEN;
    *sTemp-- = 0;

    while ( nVal > 0) 
	{
        *sTemp = (char)( nVal % 10) + '0';
        nVal /= 10;
        sTemp--;
    }

    sTemp++;
    while ( str < eptr && *sTemp != 0)
      *str++ = *sTemp++;
    
	*str = 0;

    return str;
}
char* UtilsIToA2(char  *str, unsigned int nVal, unsigned int nLen)
{
    char* sT;
    char* sU;

    if ( nVal > LIMIT_MAX_INT )
        nVal = LIMIT_MAX_INT;

    sU = str + nLen - 1;
    *sU = 0;
    sT = sU - 1;
    while ( nVal > 0 && sT >= str ) 
	{
        *sT = (char)( nVal % 10) + '0';
        nVal /= 10;
        sT--;
    }

    while (str <= sT)
        *sT-- = '0';

    return str + nLen - 1;
}
char* UtilsFToA(char *str, float fVal, char *eptr)
{
    int nNPartVal = 0;
    int nFPartVal = 0;
    char sBuf[REAL_LEN + 1];
    char* sptr = str;
    char* sTemp;

    unsigned int nIndex = 0;

    if ( fVal > LIMIT_MAX_REAL)
        fVal = LIMIT_MAX_REAL;
    else
    if ( fVal < LIMIT_MIN_REAL)
        fVal = LIMIT_MIN_REAL;

    sTemp = sBuf + REAL_LEN;
    *sTemp-- = 0;

    if ( fVal < 0 ) 
	{
        *str++ = '-';
        fVal = -fVal;
    }

    // разделяем целую и дробную части
    nNPartVal = (int)( fVal + 0.000005 );
    nFPartVal = (int)((float)( fVal - nNPartVal + 0.000005) * 100000);

    // пишем дробную часть
    for ( nIndex = 0; nIndex < 5; nIndex++) 
	{
        *sTemp = (char)( nFPartVal % 10) + '0';
        nFPartVal /= 10;
        sTemp--;
    }

    // пишем целую часть
    *sTemp-- = '.';
    *sTemp = '0';
    if ( nNPartVal == 0 )
        sTemp--;

    while (nNPartVal > 0) 
	{
        *sTemp = (char)(nNPartVal % 10) + '0';
        nNPartVal /= 10;
        sTemp--;
    }

    sTemp++;

    while (str <= eptr && *sTemp != 0)
        *str++ = *sTemp++;
    str--;


	// TODO: при избавлении от нулей при сдвиге конец строки тоже нужно чистить
	// пример число -00.90123 рельузльтат "-0.901234"

    while (str > sptr) 
	{
        if (*str == '0')
            *str = 0;
        else {
            if (*str == '.')
                *str = 0;
            break;
        }
        str--;
    }

    return (*str == 0) ? str : ++str;
}

BYTE* UtilsMemCpy(BYTE* pDst, const BYTE *pSrc, DWORD nLen)
{
	if (nLen > 0)
		memcpy( pDst, pSrc, nLen );

    return pDst;
}

BYTE* UtilsStrCpy(char *sDst, const char *sSrc, char *eptr)
{
    if ( NULL != sSrc ) 
	{
        while ( eptr > sDst && *sSrc != 0)
            *sDst++ = *sSrc++;
    }

    *sDst = 0;

    return (BYTE*)sDst;
}


int UtilsMemCmp (const BYTE *s1, const BYTE   *s2, unsigned int nLen)
{
    if ( nLen == 0 )
		return 0;

    while ( *s1 == *s2 ) 
	{
        nLen--;
        if (nLen == 0)
            return 0;
        s1++;
        s2++;
    }

    return *s1 - *s2;
}

int UtilsStrCmp (const char   *s1, const char   *s2)
{
    if (!s1 || !s2) 
	{
		if (!s1 && !s2)
			return 0;
        if (!s1 && s2)
            return -1;
        else
            return 1;
    }

    while (*s1 == *s2) 
	{
        s1++;
        s2++;
        if (*s1 == 0 || *s2 == 0)
            break;
    }

    return (BYTE)*s1 - (BYTE)*s2;
}


void* UtilsMemSet(void *pBuf, BYTE nChar, DWORD nLen)
{
	BYTE* pBuffer = (BYTE*)pBuf;
	memset( pBuffer, nChar, nLen );

	pBuffer += nLen;

    return pBuffer;
}


DWORD UtilsStrLen(const char *str, int nMaxLen)
{
    int nLen = 0;

    if ( !str )
        return 0;

    while ( *str != 0 && ( nMaxLen < 0 || nLen < nMaxLen) ) 
	{
        str++;
        nLen++;
    }

    return (DWORD)nLen;
}

const char* UtilsStrStr(const char *s1, const char   *s2, DWORD  nMaxLen)
{
    DWORD nLen = UtilsStrLen(s2, -1);

    if ( !s1 )
        return NULL;

    if ( nLen == 0 )
        return s1;

    if ( nMaxLen == 0 )
        nMaxLen = UtilsStrLen (s1, -1);

    if ( nMaxLen < nLen)
        return NULL;

    nMaxLen -= nLen;
    nMaxLen++;

    while ( nMaxLen > 0 ) 
	{
        if ( UtilsMemCmp ((const BYTE*)s1, (const BYTE*)s2, nLen) == 0 )
            return s1;

        s1++;
        nMaxLen--;
    }

    return NULL;
}


void UtilsUIntArrayClearRepeat(unsigned short** ppArray, int *pnCount, unsigned short unEmptySymbol)
{
	int nCount = *pnCount;
	unsigned short *pArray = *ppArray;
	unsigned short *pClearArray = new unsigned short[nCount];
	int nCurCount = 0;

	for ( int nIndex = 0; nIndex < nCount; nIndex++ )
	{
		BOOL bRepeat = FALSE;
		for ( int nClearIndex = 0; nClearIndex < nCurCount; nClearIndex++  )
		{
			if ( pClearArray[nClearIndex] == pArray[nIndex] )
			{
				bRepeat = TRUE;
				break;
			}
		}

		if ( !bRepeat )
		{
			pClearArray[nCurCount] = pArray[nIndex];
			nCurCount++;
		}
	}
	for ( int nIndex = 0; nIndex < nCurCount; nIndex++ )
	{
		pArray[nIndex] = pClearArray[nIndex];
	}
	for ( int nIndex = nCurCount; nIndex < nCount; nIndex++ )
	{
		pArray[nIndex] = unEmptySymbol;
	}
	*pnCount = nCurCount;
	delete []pClearArray;
}
Box ToBox(short  nLeft, short nBottom, short nRight, short nTop)
{
    Box oBox;

	oBox.fLeft   = nLeft;
	oBox.fBottom = nBottom;
	oBox.fRight  = nRight;
	oBox.fTop    = nTop;

    return oBox;
}
Box         ToBox(float  fLeft, float fBottom, float fRight, float fTop)
{
    Box oBox;

	oBox.fLeft   = fLeft;
	oBox.fBottom = fBottom;
	oBox.fRight  = fRight;
	oBox.fTop    = fTop;

    return oBox;
}
TPoint ToPoint(short x, short y)
{
    TPoint oPoint;

	oPoint.fX = x;
	oPoint.fY = y;

    return oPoint;
}

TRect ToRect(float   fLeft, float fBottom, float fRight, float fTop)
{
    TRect oRect;

	oRect.fLeft   = fLeft;
	oRect.fBottom = fBottom;
	oRect.fRight  = fRight;
	oRect.fTop    = fTop;

    return oRect;
}


void UInt16Swap(unsigned short *pnValue)
{

	BYTE aTemp[2];

    UtilsMemCpy( aTemp, (BYTE*)pnValue, 2);
    *pnValue = (unsigned short)((BYTE)aTemp[0] << 8 | (BYTE)aTemp[1]);
}




void UIntChangeBit(unsigned int *pnValue, short nBit)
{
	// работаем только с 4-байтовыми числами
	if ( nBit < 0 || nBit > 31 )
		return;

	BYTE aBits[32];

	for ( int nIndex = 0, nCurBit = 1; nIndex < 32; nIndex++, nCurBit *= 2 )
	{
		aBits[nIndex] = (*pnValue) & nCurBit ? 1 : 0;
	}
	aBits[nBit] = 1 - aBits[nBit];
	*pnValue = 0;
	for ( int nIndex = 0, nCurBit = 1; nIndex < 32; nIndex++, nCurBit *= 2 )
	{
		*pnValue += nCurBit * aBits[nIndex];
	}
}
void UtilsStrClear(std::wstring & sString)
{
	// Убираем пробелы, запятые и минусы(тире).
	int nIndex = 0;
	while ( nIndex < sString.length() ) 
	{
		char nChar = sString[nIndex];
		if ( nChar == ' ' || nChar == ',' || nChar == '-' ) 
		{
			sString.erase(sString.begin() + nIndex);
		} 
		else 
		{
			++nIndex;
		}
	}

}
void UtilsStrClear(CString & sString)
{
	// Убираем пробелы, запятые и минусы(тире).
	int nIndex = 0;
	while ( nIndex < sString.GetLength() ) 
	{
		char nChar = sString[nIndex];
		if ( nChar == ' ' || nChar == ',' || nChar == '-' ) 
		{
			sString.Delete(nIndex);
		} 
		else 
		{
			++nIndex;
		}
	}

}
#if defined(_WIN32) || defined (_WIN64)
long   GetNextNameValue(HKEY key, LPCTSTR pszSubkey, LPTSTR pszName, LPTSTR pszData)
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

	///_ASSERTE(pszName != NULL && pszData != NULL); todooo

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
#endif
//						dwPlatFormID  dwMajorVersion  dwMinorVersion  dwBuildNumber
// 95						1              4               0             950
// 95 SP1					1              4               0        >950 && <=1080
// 95 OSR2					1              4             <10           >1080
// 98						1              4              10            1998
// 98 SP1					1              4              10       >1998 && <2183
// 98 SE					1              4              10          >=2183
// ME						1              4              90            3000 

// Windows NT 3.51			2              3              51
// Windows NT 4				2              4               0            1381
// Windows 2000				2              5               0            2195
// Windows XP				2              5               1            2600
// Windows Vista			2              6               0			6000
// Windows Vista SP1		2              6               0			6001
// Windows Vista SP2		2              6               0			6002
// Windows 7				2              6               1			7600
// Windows 7 SP1			2              6               1			7601
// Windows Server 2012		2              6               2			9200
// Windows 8				2              6               2			9200
// Windows 8.1				2              6               3			9431 

// CE						3

#if defined(_WIN32) || defined(_WIN64)
	BOOL GetFontFile (NSStructures::CFont *pFont, std::wstring & wsFontName, std::wstring & wsDisplayName, std::wstring & wsFontFile, BOOL *bBold, BOOL *bItalic)
	{
		TCHAR szName[2 * MAX_PATH];
		TCHAR szData[2 * MAX_PATH];

		int nVersion;
		TCHAR szVersion[100];
		GetWinVer(szVersion, sizeof(szVersion)/sizeof(TCHAR)-1, &nVersion);

		TCHAR szFontPath[1000];

		if ( (nVersion >= WNTFIRST ) && ( nVersion <= WNTLAST) )
			_tcscpy(szFontPath, _T("Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts"));
		else
			_tcscpy(szFontPath, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Fonts"));

		BOOL bResult = FALSE;

	// Сначала пробежимся по списку фонтов и узнаем сколько из них начинаются с имени lpszFontName
		string_replaceAll(wsFontName, _T("Adobe"), _T("A"));
		UtilsStrClear( wsFontName );

		int nFontCount = 0;
		while (GetNextNameValue(HKEY_LOCAL_MACHINE, szFontPath, szName, szData) == ERROR_SUCCESS) 
		{
			std::wstring sName(szName);			
			UtilsStrClear( sName );

			if (sName.find(wsFontName) >=0 )
			{
				nFontCount++;
			}

			szFontPath[0] = _T('\0');	// this will get next value, same key
		}
		GetNextNameValue(HKEY_LOCAL_MACHINE, NULL, NULL, NULL);	// close the registry key

	// Восстанавливаем Путь в реестре
		if ( (nVersion >= WNTFIRST ) && ( nVersion <= WNTLAST) )
			_tcscpy(szFontPath, _T("Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts"));
		else
			_tcscpy(szFontPath, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Fonts"));

		//_ASSERTE(lpszFontName && lpszFontName[0] != 0);
		if (wsFontName.length() < 1)
			return FALSE;

		//_ASSERTE(lpszDisplayName);
		if (wsDisplayName.length() < 1)
			return FALSE;

		//_ASSERTE(lpszFontFile);
		if (wsFontFile.length() < 1)
			return FALSE;

		wsDisplayName	= _T("");
		wsFontFile		= _T("");

		BOOL blBold   = *bBold;
		BOOL blItalic = *bItalic;
	
		string_replaceAll(wsFontName, _T("Adobe"), _T("A"));
		UtilsStrClear( wsFontName );
		
		std::vector<std::wstring>	arr_wsDisplayName;
		std::vector<std::wstring>	arr_wsFontFile;
		while (GetNextNameValue(HKEY_LOCAL_MACHINE, szFontPath, szName, szData) == ERROR_SUCCESS) 
		{
			std::wstring sName(szName);
			
			UtilsStrClear( sName );

			if ( sName.find(wsFontName) >=0 )//todooo проверить
			{
				bResult = TRUE;
				arr_wsDisplayName.push_back(sName);
				arr_wsFontFile.push_back(szData);
			}

			//if (_tcsnicmp(lpszFontName, szName, _tcslen(lpszFontName)) == 0)
			//{
			//	bResult = TRUE;
			//	_tcsncpy( arrlpszDisplayName[nCurFontIndex], szName, nDisplayNameSize - 1);
			//	_tcsncpy( arrlpszFontFile[nCurFontIndex],    szData, nFontFileSize - 1);
			//	nCurFontIndex++;
			//}

			szName[0]		= _T('\0');	
			szFontPath[0]	= _T('\0');	// this will get next value, same key
		}
		GetNextNameValue(HKEY_LOCAL_MACHINE, NULL, NULL, NULL);	// close the registry key

	// в массив пишем номер фонта в массиве arrlpszDisplayName, с соответствующими настройками
		// { font, font bold, font italic, font bold italic}
		int arrFont[4] = { -1, -1, -1, -1 };	

		std::wstring sFontRTT  = wsFontName + _T("Regular") + _T("(TrueType)\0"); 
		std::wstring sFontBTT  = wsFontName + _T("Bold")    + _T("(TrueType)\0");
		std::wstring sFontITT  = wsFontName + _T("Italic")  + _T("(TrueType)\0");
		// oblique = Italic
		std::wstring sFontOTT  = wsFontName + _T("Oblique")     + _T("(TrueType)\0");
		std::wstring sFontBITT = wsFontName + _T("BoldItalic")  + _T("(TrueType)\0");
		std::wstring sFontBOTT = wsFontName + _T("BoldOblique") + _T("(TrueType)\0");
		std::wstring sFontTT   = wsFontName + _T("(TrueType)\0");

		std::wstring sFontROT  = wsFontName + _T("Regular") + _T("(OpenType)\0");
		std::wstring sFontBOT  = wsFontName + _T("Bold")    + _T("(OpenType)\0");
		std::wstring sFontIOT  = wsFontName + _T("Italic")  + _T("(OpenType)\0");
		// oblique = Italic
		std::wstring sFontOOT  = wsFontName + _T("Oblique")     + _T("(OpenType)\0");
		std::wstring sFontBIOT = wsFontName + _T("BoldItalic")  + _T("(OpenType)\0");
		std::wstring sFontBOOT = wsFontName + _T("BoldOblique") + _T("(OpenType)\0");
		std::wstring sFontOT   = wsFontName + _T("(OpenType)\0");

		nFontCount = arr_wsDisplayName.size();
		for ( unsigned int nIndex = 0; nIndex < nFontCount; nIndex++ )
		{
		// самое главное в начале проверить именно Bold Italic и Bold Oblique
			if (( arr_wsDisplayName[nIndex].find(sFontBITT) >= 0 ) || 
				( arr_wsDisplayName[nIndex].find(sFontBOTT) >= 0 ) ||
				( arr_wsDisplayName[nIndex].find(sFontBIOT) >= 0 ) || 
				( arr_wsDisplayName[nIndex].find(sFontBOOT) >= 0 ) )

			{
				if ( arrFont[3] < 0 )
					arrFont[3] = nIndex;
			}
			if (( arr_wsDisplayName[nIndex].find(sFontITT) >= 0 ) || 
				( arr_wsDisplayName[nIndex].find(sFontOTT) >= 0 ) ||
				( arr_wsDisplayName[nIndex].find(sFontIOT) >= 0 ) || 
				( arr_wsDisplayName[nIndex].find(sFontOOT) >= 0 ))

			{
				if ( arrFont[2] < 0 )
					arrFont[2] = nIndex;
			}
			if ( arr_wsDisplayName[nIndex].find(sFontBTT) >= 0 ||
				 arr_wsDisplayName[nIndex].find(sFontBOT) >= 0 )
			{
				if ( arrFont[1] < 0 )
					arrFont[1] = nIndex;
			}
			if ( arr_wsDisplayName[nIndex].find(sFontTT) >= 0 ||
				 arr_wsDisplayName[nIndex].find(sFontOT) >= 0 ||
				 arr_wsDisplayName[nIndex].find(sFontRTT) >= 0 ||
				 arr_wsDisplayName[nIndex].find(sFontROT) >= 0 )
			{
				if ( arrFont[0] < 0 )
					arrFont[0] = nIndex;				   
			}
		}
		if ( bResult )
		{
			if ( blBold && blItalic && arrFont[3] >= 0 )
			{
				wsDisplayName	= arr_wsDisplayName[arrFont[3]];
				wsFontFile		= arr_wsFontFile[arrFont[3]];
				blBold   = FALSE;
				blItalic = FALSE;
			}
			else if ( blBold && blItalic && arrFont[3] < 0 && arrFont[1] >= 0 )
			{
				wsDisplayName	= arr_wsDisplayName[arrFont[1]];
				wsFontFile		= arr_wsFontFile[arrFont[1]];
				blBold = FALSE;
			}
			else if ( blBold && blItalic && arrFont[3] < 0 && arrFont[2] >= 0 )
			{
				wsDisplayName	= arr_wsDisplayName[arrFont[2]];
				wsFontFile		= arr_wsFontFile[arrFont[2]];
				blItalic = FALSE;
			}
			else if ( blBold && blItalic && arrFont[3] < 0 && arrFont[0] >= 0 )
			{
				wsDisplayName	= arr_wsDisplayName[arrFont[0]];
				wsFontFile		= arr_wsFontFile[arrFont[0]];
			}
			else if ( blBold && !blItalic && arrFont[1] >= 0 )
			{
				wsDisplayName	= arr_wsDisplayName[arrFont[1]];
				wsFontFile		= arr_wsFontFile[arrFont[1]];
				blBold = FALSE;
			}
			else if ( blBold && !blItalic && arrFont[1] < 0 && arrFont[0] >= 0 )
			{
				wsDisplayName	= arr_wsDisplayName[arrFont[0]];
				wsFontFile		= arr_wsFontFile[arrFont[0]];
			}
			else if ( !blBold && blItalic && arrFont[2] >= 0 )
			{
				wsDisplayName	= arr_wsDisplayName[arrFont[2]];
				wsFontFile		= arr_wsFontFile[arrFont[2]];
				blItalic = FALSE;
			}
			else if ( !blBold && blItalic && arrFont[2] < 0 && arrFont[0] >= 0 )
			{
				wsDisplayName	= arr_wsDisplayName[arrFont[0]];
				wsFontFile		= arr_wsFontFile[arrFont[0]];
			}
			else if ( !blBold && !blItalic && arrFont[0] >= 0 )
			{
				wsDisplayName	= arr_wsDisplayName[arrFont[0]];
				wsFontFile		= arr_wsFontFile[arrFont[0]];
			}
			else
			{
			// если не один из предыдущих вариантов не прошел, тогда берем первый попавшийся фонт
			// запоминая, нужно ли в будущем делать Bold и Italic
				int nIndex = 0;
				for ( ; nIndex < 4; nIndex++ )
					if ( arrFont[nIndex] >= 0 )
						break;
				if ( nIndex >= 4 )
					bResult = FALSE;

				if ( bResult )
				{
					switch ( nIndex )
					{
						case 0: break;
						case 1: blBold = FALSE; break;
						case 2: blItalic = FALSE; break;
						case 3: blItalic = FALSE; blBold = FALSE; break;
					}
					wsDisplayName	=  arr_wsDisplayName[arrFont[nIndex]];
					wsFontFile		= arr_wsFontFile[arrFont[nIndex]];
				}
			}

		}
		
		*bBold   = blBold;
		*bItalic = blItalic;

		arr_wsDisplayName.clear();
		arr_wsFontFile.clear();

		return bResult;
	}



#elif defined(__linux__)
BOOL GetFontFile (NSStructures::CFont *pFont, std::wstring & wsFontName, std::wstring & wsDisplayName, std::wstring & wsFontFile, BOOL *bBold, BOOL *bItalic)
{
	//todooo linux
	return FALSE;
}

#else
BOOL GetFontFile (NSStructures::CFont *pFont, std::wstring & wsFontName, std::wstring & wsDisplayName, std::wstring & wsFontFile, BOOL *bBold, BOOL *bItalic)
{
	//todooo mac
	return FALSE;
}
#endif
#ifdef BUILD_CONFIG_OPENSOURCE_VERSION
	BOOL GetFontFile2(NSStructures::CFont *pFont, LPCTSTR lpszFontName, LPTSTR lpszDisplayName, int nDisplayNameSize, LPTSTR lpszFontFile, int nFontFileSize, BOOL *bBold, BOOL *bItalic, CFontManager *pFontManager)
	{
		if ( pFontManager )
		{
			long lStyle = *bBold + 2 * *bItalic;

			CString sParams;
			sParams.Format( _T("<FontProperties><Name value='%ls'/><Style bold='%d' italic='%d'/></FontProperties>"), lpszFontName, ( *bBold ? 1 : 0 ), ( *bItalic ? 1 : 0 ) );
			BSTR bsParams = sParams.AllocSysString();
			
			BSTR bsFontName = NULL;
			BSTR bsFontPath = NULL;
			BSTR bsFontStyle = NULL;
			LONG lIndex = 0;
//todooo 
            //if ( S_OK != ( pFontManager->GetWinFontByParams( bsParams, &bsFontName, &bsFontPath, &bsFontStyle, &lIndex ) ) )
			//{
			//	::SysFreeString( bsParams );
			//	::SysFreeString( bsFontName );
			//	::SysFreeString( bsFontPath );
			//	::SysFreeString( bsFontStyle );
			//	return FALSE;
			//}

			::SysFreeString( bsParams );

			::memset( lpszFontFile,    0x00, nFontFileSize    );
			::memset( lpszDisplayName, 0x00, nDisplayNameSize );

			_tcsncpy( lpszFontFile, bsFontPath, min( nFontFileSize - 1, wcslen( bsFontPath ) ) );
			::SysFreeString( bsFontPath );

			_tcsncpy( lpszDisplayName, bsFontName, min( nDisplayNameSize - 1, wcslen( bsFontName ) ) );
			::SysFreeString( bsFontName );

			// pFontManager->raw_GetAdditionalParam( _T("GetFontIndex"), &vTemp );

			CString sStyle( bsFontStyle );
			::SysFreeString( bsFontStyle );

			if ( *bBold && -1 != sStyle.Find( _T("Bold") ) )
				*bBold = FALSE;

			if ( *bItalic && ( -1 != sStyle.Find( _T("Italic") ) || -1 != sStyle.Find( _T("Oblique") ) ) )
				*bItalic = FALSE;

			return TRUE;
		}
		else
		{
			return GetFontFile( pFont, lpszFontName, lpszDisplayName, nDisplayNameSize, lpszFontFile, nFontFileSize, bBold, bItalic );
		}
	}
#else
BOOL GetFontFile2(NSStructures::CFont *pFont, std::wstring & wsFontName, std::wstring & wsDisplayName, std::wstring & wsFontFile, BOOL *bBold, BOOL *bItalic, CFontManager *pFontManager)
	{
		if ( pFontManager )
		{
			long lStyle = *bBold + 2 * *bItalic;
            CFontSelectFormat oFontSelectFormat;
            oFontSelectFormat.wsName = new std::wstring(wsFontName);
            oFontSelectFormat.bBold = new INT(*bBold);
            oFontSelectFormat.bItalic = new INT(*bItalic);
            CFontInfo* pFontInfo = pFontManager->GetFontInfoByParams(oFontSelectFormat);
            if(NULL == pFontInfo)
                return FALSE;
            wsFontFile = pFontInfo->m_wsFontPath;
            wsDisplayName = pFontInfo->m_wsFontName;
            
            if (*bBold && pFontInfo->m_bBold)
                *bBold = FALSE;
            
            if (*bItalic && pFontInfo->m_bItalic)
                *bItalic = FALSE;
            
            return TRUE;
		}
		else
		{
			return GetFontFile( pFont, wsFontName, wsDisplayName, wsFontFile, bBold, bItalic );
		}
	}
#endif


BOOL  GetWinVer(LPTSTR lpszVersion, int nVersionSize, int *pnVersion)
{
#if defined (_WIN32) || defined(_WIN64)
    _tcsncpy(lpszVersion, WUNKNOWNSTR, nVersionSize-1);
	*pnVersion = WUNKNOWN;

	TCHAR *cp = NULL;

	OSVERSIONINFO osinfo;
	osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if ( !GetVersionEx(&osinfo) )
		return FALSE;

	DWORD dwPlatformId   = osinfo.dwPlatformId;
	DWORD dwMinorVersion = osinfo.dwMinorVersion;
	DWORD dwMajorVersion = osinfo.dwMajorVersion;
	DWORD dwBuildNumber  = osinfo.dwBuildNumber & 0xFFFF;	// Win 95 needs this

	if ((dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && (dwMajorVersion == 4))
	{
		if ((dwMinorVersion < 10) && (dwBuildNumber == 950))
		{
			cp = W95STR;
			*pnVersion = W95;
		}
		else if ((dwMinorVersion < 10) &&
				((dwBuildNumber > 950) && (dwBuildNumber <= 1080)))
		{
			cp = W95SP1STR;
			*pnVersion = W95SP1;
		}
		else if ((dwMinorVersion < 10) && (dwBuildNumber > 1080))
		{
			cp = W95OSR2STR;
			*pnVersion = W95OSR2;
		}
		else if ((dwMinorVersion == 10) && (dwBuildNumber == 1998))
		{
			cp = W98STR;
			*pnVersion = W98;
		}
		else if ((dwMinorVersion == 10) &&
				((dwBuildNumber > 1998) && (dwBuildNumber < 2183)))
		{
			cp = W98SP1STR;
			*pnVersion = W98SP1;
		}
		else if ((dwMinorVersion == 10) && (dwBuildNumber >= 2183))
		{
			cp = W98SESTR;
			*pnVersion = W98SE;
		}
		else if (dwMinorVersion == 90)
		{
			cp = WMESTR;
			*pnVersion = WME;
		}
	}
	else if (dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if ((dwMajorVersion == 3) && (dwMinorVersion == 51))
		{
			cp = WNT351STR;
			*pnVersion = WNT351;
		}
		else if ((dwMajorVersion == 4) && (dwMinorVersion == 0))
		{
			cp = WNT4STR;
			*pnVersion = WNT4;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 0))
		{
			cp = W2KSTR;
			*pnVersion = W2K;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 1))
		{
			cp = WXPSTR;
			*pnVersion = WXP;
		}
		else if ((dwMajorVersion == 6) && (dwMinorVersion == 0))
		{
			cp = WVSTR;
			*pnVersion = WVista;
		}
		else if ((dwMajorVersion == 6) && (dwMinorVersion == 1))
		{
			cp = W7STR;
			*pnVersion = W7;
		}
		else if ((dwMajorVersion == 6) && (dwMinorVersion == 2))
		{
			cp = W8STR;
			*pnVersion = W8;
		}
	}
	//else if (dwPlatformId == VER_PLATFORM_WIN32_CE)///????? todooo
	//{
	//	cp = WCESTR;
	//	*pnVersion = WCE;
	//}

	_tcsncpy(lpszVersion, cp, nVersionSize-1);

	return TRUE;
#else
    return FALSE;
#endif
}

