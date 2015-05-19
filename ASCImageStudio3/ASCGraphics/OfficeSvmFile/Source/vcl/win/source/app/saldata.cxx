/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Sun Microsystems, Inc.
 *
 * OpenOffice.org - a multi-platform office productivity suite
 *
 * $RCSfile: saldata.cxx,v $
 * $Revision: 1.9 $
 *
 * This file is part of OpenOffice.org.
 *
 * OpenOffice.org is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3
 * only, as published by the Free Software Foundation.
 *
 * OpenOffice.org is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Lesser General Public License
 * version 3 along with OpenOffice.org.  If not, see
 * <http://www.openoffice.org/license.html>
 * for a copy of the LGPLv3 License.
 *
 ************************************************************************/
#ifdef AVS
// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_vcl.hxx"
#include <tools/svwin.h>
#include "rtl/tencinfo.h"
#include <saldata.hxx>
#include <vcl/svapp.hxx>
#endif
#include "vcl/saldata.hxx"
#include "rtl/textenc.h"

namespace SVMCore{
rtl_TextEncoding rtl_getTextEncodingFromWindowsCodePage(sal_uInt32 nCodePage)
{
    switch (nCodePage)
    {
    case 437: return RTL_TEXTENCODING_IBM_437;
    case 708: return RTL_TEXTENCODING_ISO_8859_6;
    case 737: return RTL_TEXTENCODING_IBM_737;
    case 775: return RTL_TEXTENCODING_IBM_775;
    case 850: return RTL_TEXTENCODING_IBM_850;
    case 852: return RTL_TEXTENCODING_IBM_852;
    case 855: return RTL_TEXTENCODING_IBM_855;
    case 857: return RTL_TEXTENCODING_IBM_857;
    case 860: return RTL_TEXTENCODING_IBM_860;
    case 861: return RTL_TEXTENCODING_IBM_861;
    case 862: return RTL_TEXTENCODING_IBM_862;
    case 863: return RTL_TEXTENCODING_IBM_863;
    case 864: return RTL_TEXTENCODING_IBM_864;
    case 865: return RTL_TEXTENCODING_IBM_865;
    case 866: return RTL_TEXTENCODING_IBM_866;
    case 869: return RTL_TEXTENCODING_IBM_869;
    case 874: return RTL_TEXTENCODING_MS_874;
    case 932: return RTL_TEXTENCODING_MS_932;
    case 936: return RTL_TEXTENCODING_MS_936;
    case 949: return RTL_TEXTENCODING_MS_949;
    case 950: return RTL_TEXTENCODING_MS_950;
    case 1250: return RTL_TEXTENCODING_MS_1250;
    case 1251: return RTL_TEXTENCODING_MS_1251;
    case 1252: return RTL_TEXTENCODING_MS_1252;
    case 1253: return RTL_TEXTENCODING_MS_1253;
    case 1254: return RTL_TEXTENCODING_MS_1254;
    case 1255: return RTL_TEXTENCODING_MS_1255;
    case 1256: return RTL_TEXTENCODING_MS_1256;
    case 1257: return RTL_TEXTENCODING_MS_1257;
    case 1258: return RTL_TEXTENCODING_MS_1258;
    case 1361: return RTL_TEXTENCODING_MS_1361;
    case 10000: return RTL_TEXTENCODING_APPLE_ROMAN;
    case 10001: return RTL_TEXTENCODING_APPLE_JAPANESE;
    case 10002: return RTL_TEXTENCODING_APPLE_CHINTRAD;
    case 10003: return RTL_TEXTENCODING_APPLE_KOREAN;
    case 10004: return RTL_TEXTENCODING_APPLE_ARABIC;
    case 10005: return RTL_TEXTENCODING_APPLE_HEBREW;
    case 10006: return RTL_TEXTENCODING_APPLE_GREEK;
    case 10007: return RTL_TEXTENCODING_APPLE_CYRILLIC;
    case 10008: return RTL_TEXTENCODING_APPLE_CHINSIMP;
    case 10010: return RTL_TEXTENCODING_APPLE_ROMANIAN;
    case 10017: return RTL_TEXTENCODING_APPLE_UKRAINIAN;
    case 10029: return RTL_TEXTENCODING_APPLE_CENTEURO;
    case 10079: return RTL_TEXTENCODING_APPLE_ICELAND;
    case 10081: return RTL_TEXTENCODING_APPLE_TURKISH;
    case 10082: return RTL_TEXTENCODING_APPLE_CROATIAN;
    case 20127: return RTL_TEXTENCODING_ASCII_US;
    case 20866: return RTL_TEXTENCODING_KOI8_R;
    case 21866: return RTL_TEXTENCODING_KOI8_U;
    case 28591: return RTL_TEXTENCODING_ISO_8859_1;
    case 28592: return RTL_TEXTENCODING_ISO_8859_2;
    case 28593: return RTL_TEXTENCODING_ISO_8859_3;
    case 28594: return RTL_TEXTENCODING_ISO_8859_4;
    case 28595: return RTL_TEXTENCODING_ISO_8859_5;
    case 28596: return RTL_TEXTENCODING_ISO_8859_6;
    case 28597: return RTL_TEXTENCODING_ISO_8859_7;
    case 28598: return RTL_TEXTENCODING_ISO_8859_8;
    case 28599: return RTL_TEXTENCODING_ISO_8859_9;
    case 28605: return RTL_TEXTENCODING_ISO_8859_15;
    case 50220: return RTL_TEXTENCODING_ISO_2022_JP;
    case 50225: return RTL_TEXTENCODING_ISO_2022_KR;
    case 51932: return RTL_TEXTENCODING_EUC_JP;
    case 51936: return RTL_TEXTENCODING_EUC_CN;
    case 51949: return RTL_TEXTENCODING_EUC_KR;
    case 65000: return RTL_TEXTENCODING_UTF7;
    case 65001: return RTL_TEXTENCODING_UTF8;
    default: return RTL_TEXTENCODING_DONTKNOW;
    }
}
sal_uInt32 rtl_getWindowsCodePageFromTextEncoding(rtl_TextEncoding nEncoding)
{
    switch ( nEncoding )
    {
    case RTL_TEXTENCODING_IBM_437: return 437;
    case RTL_TEXTENCODING_ISO_8859_6: return 708;
    case RTL_TEXTENCODING_IBM_737: return 737;
    case RTL_TEXTENCODING_IBM_775: return 775;
    case RTL_TEXTENCODING_IBM_850: return 850;
    case RTL_TEXTENCODING_IBM_852: return 852;
    case RTL_TEXTENCODING_IBM_855: return 855;
    case RTL_TEXTENCODING_IBM_857: return 857;
    case RTL_TEXTENCODING_IBM_860: return 860;
    case RTL_TEXTENCODING_IBM_861: return 861;
    case RTL_TEXTENCODING_IBM_862: return 862;
    case RTL_TEXTENCODING_IBM_863: return 863;
    case RTL_TEXTENCODING_IBM_864: return 864;
    case RTL_TEXTENCODING_IBM_865: return 865;
    case RTL_TEXTENCODING_IBM_866: return 866;
    case RTL_TEXTENCODING_IBM_869: return 869;
    case RTL_TEXTENCODING_MS_874: return 874;
    case RTL_TEXTENCODING_MS_932: return 932;
    case RTL_TEXTENCODING_MS_936 : return 936;
    case RTL_TEXTENCODING_MS_949: return 949;
    case RTL_TEXTENCODING_MS_950: return 950;
    case RTL_TEXTENCODING_MS_1250: return 1250;
    case RTL_TEXTENCODING_MS_1251: return 1251;
    case RTL_TEXTENCODING_MS_1252: return 1252;
    case RTL_TEXTENCODING_MS_1253: return 1253;
    case RTL_TEXTENCODING_MS_1254: return 1254;
    case RTL_TEXTENCODING_MS_1255: return 1255;
    case RTL_TEXTENCODING_MS_1256: return 1256;
    case RTL_TEXTENCODING_MS_1257: return 1257;
    case RTL_TEXTENCODING_MS_1258: return 1258;
    case RTL_TEXTENCODING_MS_1361: return 1361;
    case RTL_TEXTENCODING_APPLE_ROMAN: return 10000;
    case RTL_TEXTENCODING_APPLE_JAPANESE: return 10001;
    case RTL_TEXTENCODING_APPLE_CHINTRAD: return 10002;
    case RTL_TEXTENCODING_APPLE_KOREAN: return 10003;
    case RTL_TEXTENCODING_APPLE_ARABIC: return 10004;
    case RTL_TEXTENCODING_APPLE_HEBREW: return 10005;
    case RTL_TEXTENCODING_APPLE_GREEK: return 10006;
    case RTL_TEXTENCODING_APPLE_CYRILLIC: return 10007;
    case RTL_TEXTENCODING_APPLE_CHINSIMP: return 10008;
    case RTL_TEXTENCODING_APPLE_ROMANIAN: return 10010;
    case RTL_TEXTENCODING_APPLE_UKRAINIAN: return 10017;
    case RTL_TEXTENCODING_APPLE_CENTEURO: return 10029;
    case RTL_TEXTENCODING_APPLE_ICELAND: return 10079;
    case RTL_TEXTENCODING_APPLE_TURKISH: return 10081;
    case RTL_TEXTENCODING_APPLE_CROATIAN: return 10082;
    case RTL_TEXTENCODING_ASCII_US: return 20127;
    case RTL_TEXTENCODING_KOI8_R: return 20866;
    case RTL_TEXTENCODING_KOI8_U: return 21866;
    case RTL_TEXTENCODING_ISO_8859_1: return 28591;
    case RTL_TEXTENCODING_ISO_8859_2: return 28592;
    case RTL_TEXTENCODING_ISO_8859_3: return 28593;
    case RTL_TEXTENCODING_ISO_8859_4: return 28594;
    case RTL_TEXTENCODING_ISO_8859_5: return 28595;
    //case RTL_TEXTENCODING_ISO_8859_6: return 28596;
    case RTL_TEXTENCODING_ISO_8859_7: return 28597;
    case RTL_TEXTENCODING_ISO_8859_8: return 28598;
    case RTL_TEXTENCODING_ISO_8859_9: return 28599;
    case RTL_TEXTENCODING_ISO_8859_15: return 28605;
    case RTL_TEXTENCODING_ISO_2022_JP: return 50220;
    case RTL_TEXTENCODING_ISO_2022_KR: return 50225;
    case RTL_TEXTENCODING_EUC_JP: return 51932;
    case RTL_TEXTENCODING_EUC_CN: return 51936;
    case RTL_TEXTENCODING_EUC_KR: return 51949;
    case RTL_TEXTENCODING_UTF7: return 65000;
    case RTL_TEXTENCODING_UTF8: return 65001;
    default: return 1252;
    }
}


// =======================================================================

rtl_TextEncoding ImplSalGetSystemEncoding()
{
	static UINT nOldAnsiCodePage = 0;
	static rtl_TextEncoding eEncoding = RTL_TEXTENCODING_MS_1252;

	UINT nAnsiCodePage = GetACP();
	if ( nAnsiCodePage != nOldAnsiCodePage )
	{
        rtl_TextEncoding nEnc
            = rtl_getTextEncodingFromWindowsCodePage(nAnsiCodePage);
        if (nEnc != RTL_TEXTENCODING_DONTKNOW)
            eEncoding = nEnc;
	}

	return eEncoding;
}

// -----------------------------------------------------------------------

ByteString ImplSalGetWinAnsiString( const UniString& rStr, BOOL bFileName )
{
	rtl_TextEncoding eEncoding = ImplSalGetSystemEncoding();
#ifdef AVS
	if ( bFileName )
	{
		return ByteString( rStr, eEncoding,
						   RTL_UNICODETOTEXT_FLAGS_UNDEFINED_UNDERLINE |
						   RTL_UNICODETOTEXT_FLAGS_INVALID_UNDERLINE |
						   RTL_UNICODETOTEXT_FLAGS_UNDEFINED_REPLACE |
						   RTL_UNICODETOTEXT_FLAGS_UNDEFINED_REPLACESTR |
						   RTL_UNICODETOTEXT_FLAGS_PRIVATE_MAPTO0 );
	}
	else
	{
		return ByteString( rStr, eEncoding,
						   RTL_UNICODETOTEXT_FLAGS_UNDEFINED_DEFAULT |
						   RTL_UNICODETOTEXT_FLAGS_INVALID_DEFAULT |
						   RTL_UNICODETOTEXT_FLAGS_UNDEFINED_REPLACE |
						   RTL_UNICODETOTEXT_FLAGS_UNDEFINED_REPLACESTR |
						   RTL_UNICODETOTEXT_FLAGS_PRIVATE_MAPTO0 );
	}
#endif
		return ByteString( rStr, eEncoding );
}

// -----------------------------------------------------------------------

UniString ImplSalGetUniString( const sal_Char* pStr, xub_StrLen nLen )
{
#ifdef AVS
	return UniString( pStr, nLen, ImplSalGetSystemEncoding(),
					  RTL_TEXTTOUNICODE_FLAGS_UNDEFINED_DEFAULT |
					  RTL_TEXTTOUNICODE_FLAGS_MBUNDEFINED_DEFAULT |
					  RTL_TEXTTOUNICODE_FLAGS_INVALID_DEFAULT );
#endif
	return UniString( pStr, nLen, ImplSalGetSystemEncoding() );
}

// =======================================================================

int ImplSalWICompareAscii( const wchar_t* pStr1, const char* pStr2 )
{
	int 		nRet;
	wchar_t 	c1;
	char	   c2;
	do
	{
		// Ist das Zeichen zwischen 'A' und 'Z' dann umwandeln
		c1 = *pStr1;
		c2 = *pStr2;
		if ( (c1 >= 65) && (c1 <= 90) )
			c1 += 32;
		if ( (c2 >= 65) && (c2 <= 90) )
			c2 += 32;
		nRet = ((sal_Int32)c1)-((sal_Int32)((unsigned char)c2));
		if ( nRet != 0 )
			break;

		pStr1++;
		pStr2++;
	}
	while ( c2 );

	return nRet;
}
}//SVMCore
#ifdef AVS
// =======================================================================

LONG ImplSetWindowLong( HWND hWnd, int nIndex, DWORD dwNewLong )
{
	if ( aSalShlData.mbWNT )
		return SetWindowLongW( hWnd, nIndex, dwNewLong );
	else
		return SetWindowLongA( hWnd, nIndex, dwNewLong );
}

// -----------------------------------------------------------------------

LONG ImplGetWindowLong( HWND hWnd, int nIndex )
{
	if ( aSalShlData.mbWNT )
		return GetWindowLongW( hWnd, nIndex );
	else
		return GetWindowLongA( hWnd, nIndex );
}

// -----------------------------------------------------------------------

WIN_BOOL ImplPostMessage( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam )
{
	if ( aSalShlData.mbWNT )
		return PostMessageW( hWnd, nMsg, wParam, lParam );
	else
		return PostMessageA( hWnd, nMsg, wParam, lParam );
}

// -----------------------------------------------------------------------

WIN_BOOL ImplSendMessage( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam )
{
    WIN_BOOL bRet;
	if ( aSalShlData.mbWNT )
		bRet = SendMessageW( hWnd, nMsg, wParam, lParam );
	else
		bRet = SendMessageA( hWnd, nMsg, wParam, lParam );

    return bRet;
}

// -----------------------------------------------------------------------

WIN_BOOL ImplGetMessage( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax )
{
	if ( aSalShlData.mbWNT )
		return GetMessageW( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax );
	else
		return GetMessageA( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax );
}

// -----------------------------------------------------------------------

WIN_BOOL ImplPeekMessage( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg )
{
	if ( aSalShlData.mbWNT )
		return PeekMessageW( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg );
	else
		return PeekMessageA( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg );
}

// -----------------------------------------------------------------------

LONG ImplDispatchMessage( CONST MSG *lpMsg )
{
	if ( aSalShlData.mbWNT )
		return DispatchMessageW( lpMsg );
	else
		return DispatchMessageA( lpMsg );
}

#endif