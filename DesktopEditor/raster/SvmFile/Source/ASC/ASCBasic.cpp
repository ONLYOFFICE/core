#include "ASCBasic.h"

//11111
//#include <windows.h>
//#include <tchar.h>
///1111

#include "../vcl/saldata.hxx"

XubString::XubString( ByteString pBuffer, unsigned short nCharSet )
{
	int nWinCodePage = SVMCore::rtl_getWindowsCodePageFromTextEncoding( nCharSet );
	int nBufSize = ::MultiByteToWideChar(nWinCodePage, 0, pBuffer.GetBuffer(), -1, NULL, NULL);
	nBufSize--;// nBufSize - учитывает ноль литеру
	::MultiByteToWideChar(nWinCodePage, 0, pBuffer.GetBuffer(), -1, AllocBuffer( nBufSize ), nBufSize);
}
XubString::XubString( ByteString pBuffer, int nLen, unsigned short nCharSet )
{
	(*this) = XubString( pBuffer, nCharSet );
	m_oString = m_oString.substr( 0, nLen);
}
bool XubString::EqualsAscii( ByteString sText ) const
{
	XubString sUniText( sText, GetACP() );
	if( sUniText == (*this) )
		return true;
	else
		return false;
}
bool XubString::EqualsAscii( ByteString sText, int nIndex, int nLen ) const
{
	XubString sUniText( sText, GetACP() );
	if( 0 == m_oString.compare( nIndex, nLen, sUniText.m_oString ) )
		return true;
	else
		return false;
}
int XubString::CompareIgnoreCaseToAscii( char* pcStr, int nLen ) const 
	{ 
		XubString oTemp1 = (*this);
		XubString oTemp2( ByteString( pcStr ), GetACP() );
		std::transform( oTemp1.m_oString.begin(), oTemp1.m_oString.end(), oTemp1.m_oString.begin(), tolower);
		std::transform( oTemp2.m_oString.begin(), oTemp2.m_oString.end(), oTemp2.m_oString.begin(), tolower);
		return oTemp1.m_oString.compare( 0, nLen, oTemp2.m_oString );
	}
ByteString::ByteString( XubString pBuffer, unsigned short nCharSet ) 
{

}

unsigned short gsl_getSystemTextEncoding(void)
{
	//вычисляем стандартный default ansi codepage для системы
	TCHAR codepage[7];
	int nRes = GetLocaleInfo( LOCALE_SYSTEM_DEFAULT, LOCALE_IDEFAULTANSICODEPAGE, codepage, 6 );
	int nWinCodePage = CP_ACP;
	if( nRes != 0 )
		nWinCodePage = _wtoi(codepage);
	return SVMCore::rtl_getTextEncodingFromWindowsCodePage( nWinCodePage );
	
}
