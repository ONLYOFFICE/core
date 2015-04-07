#include "Encoding.h"
#include "Utility.h"
#include "../Common/SimpleTypes_Word.h"


AVSINLINE const CStringA Encoding::ansi2utf8     (const CStringA &sLine)
{
	return wstring2string( string2wstring( sLine, CP_ACP ), CP_UTF8 );
}


AVSINLINE const CStringW Encoding::ansi2unicode  (const CStringA &sLine)
{
	return string2wstring( sLine, CP_ACP );
}


AVSINLINE const CStringA Encoding::utf82ansi     (const CStringA &sLine)
{
	return wstring2string( string2wstring( sLine, CP_UTF8 ), CP_ACP );
}


AVSINLINE const CStringW Encoding::utf82unicode  (const CStringA &sLine)
{
	return string2wstring( sLine, CP_UTF8 );
}


AVSINLINE const CStringA Encoding::unicode2ansi  (const CStringW &sLine)
{
	return wstring2string( sLine, CP_ACP );
}


AVSINLINE const CStringA Encoding::unicode2utf8  (const CStringW &sLine)
{
	return wstring2string( sLine, CP_UTF8 );
}


AVSINLINE const CStringA Encoding::wstring2string(const CStringW &sLine, const unsigned int unCodePage)
{
	const int nSize = WideCharToMultiByte( unCodePage, 0, sLine.GetString(), sLine.GetLength(), NULL, 0, NULL, NULL );
	char *sTemp = new char[nSize];
	if ( !sTemp )
		return "";
	
	WideCharToMultiByte( unCodePage, 0, sLine.GetString(), sLine.GetLength(), sTemp, nSize, NULL, NULL );

	CStringA sResult( sTemp );
	delete []sTemp;

	return sResult;
}


AVSINLINE const CStringW Encoding::string2wstring(const CStringA &sLine, const unsigned int unCodePage)
{
	const int nSize = MultiByteToWideChar( unCodePage, 0, sLine.GetString(), sLine.GetLength(), NULL, 0 );

	wchar_t *sTemp = new wchar_t[nSize];
	if ( !sTemp )
		return _T("");

	MultiByteToWideChar( unCodePage, 0, sLine.GetString(), sLine.GetLength(), sTemp, nSize );

	CStringW sResult( sTemp );
	delete []sTemp;

	return sResult;
}

