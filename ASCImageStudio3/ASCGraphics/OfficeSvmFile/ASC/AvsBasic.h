#pragma once
#include "OpenOfficeBasic.h"
//#include <atlbase.h>
//#include <atlstr.h>
//
//typedef CStringW XubString;
//typedef CStringW String;
//typedef CStringA ByteString;

#include <stdlib.h>
#include <string>
#include < algorithm >

enum StringCompare { COMPARE_LESS = -1, COMPARE_EQUAL = 0, COMPARE_GREATER = 1 };

template <class T, class Char>
class TString
{
protected: T m_oString;
public: TString(){}
public: TString( const Char* pBuffer ):m_oString(pBuffer){}
public: TString( const TString& oStr, int nStart, int nLength ):m_oString(oStr.m_oString.substr( nStart, nLength )){}
public: unsigned int Len() const { return m_oString.length(); }
public: Char * GetBuffer() const
		{ 
			const Char * pcBuffer = m_oString.c_str();
			return const_cast<Char *>(pcBuffer);
		}
public: Char * AllocBuffer( unsigned short nSize ) 
		{ 
			int nCurSize = m_oString.size();
			if( nCurSize < nSize )
				m_oString.resize( nSize );
			return GetBuffer();
		}
public: Char GetChar( int nIndex ) const 
	{ 
		if( nIndex >= 0 && nIndex < m_oString.length() )
			return m_oString[ nIndex ];
		else
			return Char();
	}
public: void SetChar( int nIndex, Char cNewChar ) 
	{ 
		if( nIndex >= 0 && nIndex < m_oString.length() )
			m_oString.insert( nIndex, 1, cNewChar );
	}
		
public: void Append( Char cChar ) 
	{
		m_oString.push_back( cChar );
	}
public: void Append( TString sString ) 
	{ 
		m_oString.append( sString.m_oString );
	}
public: TString& Erase( int nIndex = 0, int nCount = STRING_LEN )
	{
		m_oString.erase( nIndex, nCount );
		return (*this);
	}
public: TString Copy( int nIndex, int nLen ) const 
	{ 
		return TString( m_oString.substr( nIndex, nLen).c_str() );
	}
public: bool Equals( TString sText, int nIndex, int nLen ) const 
	{ 
		if( Len() != sText.Len() )
			return false;
		for( int i = nIndex; i < m_oString.length() && i < sText.m_oString.length() && i < nLen; i++ )
			if( m_oString[i] != sText.m_oString[i] )
				return false;
		return true;
	}
public: bool EqualsIgnoreCaseAscii( TString sText ) const 
	{ 
		TString oTemp1 = (*this);
		TString oTemp2 = sText;
		std::transform( oTemp1.m_oString.begin(), oTemp1.m_oString.end(), oTemp1.m_oString.begin(), tolower);
		std::transform( oTemp2.m_oString.begin(), oTemp2.m_oString.end(), oTemp2.m_oString.begin(), tolower);

		return oTemp1.Equals( oTemp2, 0, oTemp2.Len() );
	}
public: StringCompare CompareTo( TString oStr ) const
		{
			int nRes = m_oString.compare( oStr.m_oString );
			if( nRes > 0 )
				return COMPARE_GREATER;
			else if( nRes < 0 )
				return COMPARE_LESS;
			else
				return COMPARE_EQUAL;
		}
public: void operator=( TString sText ) 
	{ 
		m_oString = sText.m_oString;
	}
public: bool operator==( TString sText ) const 
	{ 
		return m_oString == sText.m_oString;
	}
public: bool operator!=( TString sText ) const 
	{ 
		return !( (*this) == sText );
	}
public: void operator+=( Char cChar ) 
	{ 
		m_oString.append( cChar, 1 );
	}
public: bool operator<( TString sText ) const 
	{ 

		bool bRes = m_oString < sText.m_oString;
		return bRes;
	}
public: operator size_t() const 
	{ 
		return Len();
	}
protected: TString GetToken( int nToken, Char cTok, int& rIndex ) const
{
	const Char*	pStr			= GetBuffer();
	int		nLen			= m_oString.length();
	int		nTok			= 0;
	int		nFirstChar		= rIndex;
	int		i				= nFirstChar;

	// Bestimme die Token-Position und Laenge
	pStr += i;
	while ( i < nLen )
	{
		// Stimmt das Tokenzeichen ueberein, dann erhoehe TokCount
		if ( *pStr == cTok )
		{
			++nTok;

			if ( nTok == nToken )
				nFirstChar = i+1;
			else
			{
				if ( nTok > nToken )
					break;
			}
		}

		++pStr,
		++i;
	}

	if ( nTok >= nToken )
	{
		if ( i < nLen )
			rIndex = i+1;
		else
			rIndex = -1;
		return Copy( nFirstChar, i-nFirstChar );
	}
	else
	{
		rIndex = -1;
		return TString();
	}
}

};

class ByteString;

class XubString : public TString< std::wstring, wchar_t >
{
	public: XubString( ){}
	public: XubString( const wchar_t* pBuffer ):TString< std::wstring, wchar_t >(pBuffer){}
	public: XubString( ByteString pBuffer, unsigned short nCharSet );
	public: XubString( ByteString pBuffer, int nLen, unsigned short nCharSet );
	public: XubString( wchar_t* pBuffer, int nLength )
			{
				m_oString.append( pBuffer, nLength );
			}
	public: XubString( XubString pBuffer, int nStart, int nLength ):TString< std::wstring, wchar_t >(pBuffer, nStart, nLength){}
	public: XubString GetToken( int nToken, wchar_t cTok = ';' ) const
			{
				int nTemp = 0;
				TString< std::wstring, wchar_t > oTempString = TString< std::wstring, wchar_t >::GetToken( nToken, cTok, nTemp );
				XubString oRes = XubString( oTempString.GetBuffer() );
				return oRes;
			}
	public: bool EqualsAscii( ByteString sText ) const;
	public: bool EqualsAscii( ByteString sText, int nIndex, int nLen ) const;
	public: int CompareIgnoreCaseToAscii( char* pcStr, int nLen ) const;
};

class ByteString : public TString< std::string, char >
{
	public: ByteString( ){}
	public: ByteString( const char* pBuffer ):TString< std::string, char >(pBuffer){}
	public: ByteString( XubString pBuffer, unsigned short nCharSet );
	public: ByteString( char* pBuffer, int nLength )
			{
				m_oString.append( pBuffer, nLength );
			}
	public: ByteString( ByteString pBuffer, int nStart, int nLength ):TString< std::string, char >(pBuffer, nStart, nLength){}
};

typedef XubString String;
typedef XubString UniString;

unsigned short gsl_getSystemTextEncoding(void);