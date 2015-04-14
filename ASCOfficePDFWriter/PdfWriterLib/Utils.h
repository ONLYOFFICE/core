#ifndef _UTILS_H
#define _UTILS_H

#pragma once

#if defined(_WIN32) || defined (_WIN64)
	#include <windows.h>
    #include <atlbase.h>
    #include <atlstr.h>
#else
    #include "../../Common/DocxFormat/Source/Base/ASCString.h"
    #include "../../DesktopEditor/common/ASCVariant.h"

 //  #include <stdio.h>
    #include <stdarg.h>
    #include <wchar.h>

#endif

#include <string>

static std::wstring string_format(const std::wstring fmt, ...)
{
    int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
    std::wstring str;
    va_list ap;
    while (1) {     // Maximum two passes on a POSIX system...
        str.resize(size);
        va_start(ap, fmt);
        int n = vswprintf((wchar_t *)str.data(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {  // Everything worked
            str.resize(n);
            return str;
        }
        if (n > -1)  // Needed size returned
            size = n + 1;   // For null char
        else
            size *= 2;      // Guess at a larger size (OS specific)
    }
    return str;
}
static void string_replaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to)
{
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::wstring::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}
#include "../../DesktopEditor/fontengine/ApplicationFonts.h"

#include "Types.h"

#define NEEDS_ESCAPE(c)    (c < 0x21 || \
                            c > 0x7e || \
                            c == '\\' || \
                            c == '%' || \
                            c == '#' || \
                            c == '/' || \
                            c == '(' || \
                            c == ')' || \
                            c == '<' || \
                            c == '>' || \
                            c == '[' || \
                            c == ']' || \
                            c == '{' || \
                            c == '}' )  \

#define IS_WHITE_SPACE(c)   (c == 0x00 || \
                             c == 0x09 || \
                             c == 0x0A || \
                             c == 0x0C || \
                             c == 0x0D || \
                             c == 0x20 ) \


namespace NSStructures
{
	class CFont;
}

int         UtilsAToI	(const char*  str);
double      UtilsAToF	(const char  *str);
char*       UtilsIToA	(char *str, int nVal, char *eptr);
char*       UtilsIToA2	(char  *str, unsigned int nVal, unsigned int nLen);
char*       UtilsFToA	(char *str, float fVal, char *eptr);
BYTE*       UtilsMemCpy	(BYTE* pDst, const BYTE *pSrc, DWORD nLen);
BYTE*       UtilsStrCpy	(char *sDst, const char *sSrc, char *eptr);
int         UtilsMemCmp (const BYTE *s1, const BYTE   *s2, unsigned int nLen);
int         UtilsStrCmp (const char   *s1, const char   *s2);
void*       UtilsMemSet	(void *pBuf, BYTE nChar, DWORD nLen);
DWORD       UtilsStrLen	(const char *str, int nMaxLen);

const char* UtilsStrStr(const char *s1, const char   *s2, DWORD  nMaxLen);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void     UtilsUIntArrayClearRepeat(unsigned short** ppArray, int *pnCount, unsigned short unEmptySymbol = 0xFFFF);
Box		ToBox		(short  nLeft, short nBottom, short nRight, short nTop);
Box		ToBox		(float  fLeft, float fBottom, float fRight, float fTop);
TPoint	ToPoint		(short x, short y);
TRect	ToRect		(float   fLeft, float fBottom, float fRight, float fTop);
void	UInt16Swap	(unsigned short *pnValue);
void	UIntChangeBit(unsigned int *pnValue, short nBit);

void	UtilsStrClear(CString		& sString);
void	UtilsStrClear(std::wstring	& sString);

#if defined(_WIN32) || defined (_WIN64)
    long	GetNextNameValue(HKEY key, LPCTSTR pszSubkey, LPTSTR pszName, LPTSTR pszData);
#endif

bool	GetWinVer(LPTSTR lpszVersion, int nVersionSize, int *pnVersion);
bool	GetFontFile (NSStructures::CFont *pFont, std::wstring & wsFontName, std::wstring & wsDisplayName, std::wstring & wsFontFile, bool *bBold, bool *bItalic);

bool	GetFontFile2(NSStructures::CFont *pFont, std::wstring & wsFontName, std::wstring & wsDisplayName, std::wstring & wsFontFile, bool *bBold, bool *bItalic, CFontManager *pFontManager = NULL);

//-----------------------------------------------------------------------------------------------------
//  CRC 32
//-----------------------------------------------------------------------------------------------------
class CRC32
{

public:   

	CRC32()
	{
		const unsigned CRC_POLY = 0xEDB88320;

		for ( unsigned int i = 0; i < 256; i++ )
		{
			unsigned int r, j;
			for ( r = i, j = 8; j; j--)
				r = r & 1? (r >> 1) ^ CRC_POLY: r >> 1;

			m_pTable[i] = r;
		}

		m_nCRC32 = 0;
	}
	void Init(unsigned int nCRC = 0)
	{
		m_nCRC32 = nCRC;
	}
	void ProcessCRC(void* pData, int nLen)
	{
		const unsigned CRC_MASK = 0xD202EF8D;
		register unsigned char *sData = reinterpret_cast<unsigned char*>(pData);
		register unsigned int nCRC = m_nCRC32;

		while ( nLen-- )
		{
			nCRC = m_pTable[static_cast<unsigned char>(nCRC) ^ *sData++] ^ nCRC >> 8;
			nCRC ^= CRC_MASK;
		}

		m_nCRC32 = nCRC;
	}

protected:

	unsigned int m_pTable[256];

public:

	unsigned int m_nCRC32;   

};



#endif /* _UTILS_H */

