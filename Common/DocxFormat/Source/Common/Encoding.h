#pragma once
#ifndef UTILITY_ENCODING_INCLUDE_H_
#define UTILITY_ENCODING_INCLUDE_H_

#include "../Base/Base.h"
#include "../Base/ASCString.h"

#ifndef _WIN32
#define CP_ACP 0   //
#define CP_UTF8 1
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
#ifdef _WIN32
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
#ifdef _WIN32
		const int nSize = MultiByteToWideChar( unCodePage, 0, sLine.GetString(), sLine.GetLength(), NULL, 0 );

		wchar_t *sTemp = new wchar_t[nSize];
		if ( !sTemp )
			return _T("");

		MultiByteToWideChar( unCodePage, 0, sLine.GetString(), sLine.GetLength(), sTemp, nSize );

		CStringW sResult( sTemp );
		delete []sTemp;

        return sResult;
#else
#endif
	}


};	


#endif // UTILITY_ENCODING_INCLUDE_H_
