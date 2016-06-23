/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#ifndef UTILITY_ENCODING_INCLUDE_H_
#define UTILITY_ENCODING_INCLUDE_H_

#include "../Base/Base.h"
#include "../Base/ASCString.h"

#if !defined(_WIN32) && !defined (_WIN64)

    #ifndef CP_ACP
        #define CP_ACP 0   //
    #endif
    #ifndef CP_UTF8
        #define CP_UTF8 1
    #endif
#endif
class Encoding
{
public:

    static AVSINLINE const CStringA ansi2utf8     (const CStringA &sLine)
	{
		return wstring2string( string2wstring( sLine, CP_ACP ), CP_UTF8 );
	}


    static AVSINLINE const CStringW ansi2unicode  (const CStringA &sLine)
	{
		return string2wstring( sLine, CP_ACP );
	}


    static AVSINLINE const CStringA utf82ansi     (const CStringA &sLine)
	{
		return wstring2string( string2wstring( sLine, CP_UTF8 ), CP_ACP );
	}


    static AVSINLINE const CStringW utf82unicode  (const CStringA &sLine)
	{
		return string2wstring( sLine, CP_UTF8 );
	}


    static AVSINLINE const CStringA unicode2ansi  (const CStringW &sLine)
	{
		return wstring2string( sLine, CP_ACP );
	}


    static AVSINLINE const CStringA unicode2utf8  (const CStringW &sLine)
	{
		return wstring2string( sLine, CP_UTF8 );
	}




private:
    static AVSINLINE const CStringA wstring2string(const CStringW &sLine, const unsigned int unCodePage)
	{
#if defined(_WIN32) || defined (_WIN64)
		const int nSize = WideCharToMultiByte( unCodePage, 0, sLine.GetString(), sLine.GetLength(), NULL, 0, NULL, NULL );
		char *sTemp = new char[nSize];
		if ( !sTemp )
			return "";

		WideCharToMultiByte( unCodePage, 0, sLine.GetString(), sLine.GetLength(), sTemp, nSize, NULL, NULL );

		CStringA sResult( sTemp );
		delete []sTemp;

		return sResult;
#else
        return stringWstingToUtf8String (sLine);
#endif


	}


    static AVSINLINE const CStringW string2wstring(const CStringA &sLine, const unsigned int unCodePage)
	{
#if defined(_WIN32) || defined (_WIN64)
		const int nSize = MultiByteToWideChar( unCodePage, 0, sLine.GetString(), sLine.GetLength(), NULL, 0 );

		wchar_t *sTemp = new wchar_t[nSize];
		if ( !sTemp )
			return _T("");

		MultiByteToWideChar( unCodePage, 0, sLine.GetString(), sLine.GetLength(), sTemp, nSize );

		CStringW sResult( sTemp );
		delete []sTemp;

        return sResult;
#else
        return stringUtf8ToWString (sLine);
#endif
	}


};	


#endif // UTILITY_ENCODING_INCLUDE_H_
