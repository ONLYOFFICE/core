#include "stdafx.h"
#include "strings.h"
//#include "Encoding.h"

namespace strings
{
	_bstr_t & make_lower( _bstr_t & string )
	{
		std::locale loc ( "English_England" );
		std::use_facet< std::ctype< wchar_t > > ( loc ).tolower( string.GetBSTR(),
			&string.GetBSTR()[ string.length() ] );
		return string;
	}


	std::wstring replace( const std::wstring & str,
		const std::wstring & what,
		const std::wstring & with )
	{
		std::wstring            res = str;
		std::wstring::size_type s   = res.find( what );

		if ( std::wstring::npos != s )
		{
			res = res.substr( 0, s ) + with + res.substr( s + what.length() );
		}

		return res;
	}


	std::wstring replace( const std::wstring & str,
						  const std::wstring & start,
						  const std::wstring & end,
						  const std::wstring & replace )
	{
		std::wstring            res = str;
		std::wstring::size_type s   = res.find( start );
		std::wstring::size_type e   = res.find( end, s );

		if ( ( std::wstring::npos != s ) && ( std::wstring::npos != e ) )
		{
			res = res.substr( 0, s ) + replace + res.substr( e + end.length() );
		}


		return res;
	}


	std::wstring replace_all( const std::wstring & str,
							  const std::wstring & what,
						      const std::wstring & with )
	{
		std::wstring            res = str;    
		std::wstring::size_type s   = res.find( what );

		while ( std::wstring::npos != s )
		{
			res = res.substr( 0, s ) + with + res.substr( s + what.length() );
			s   = res.find( what );
		}

		return res;
	}


	std::wstring replace_all( const std::wstring & str,
							  const std::wstring & start,
							  const std::wstring & end,
							  const std::wstring & replace )
	{
		std::wstring            res = str;
		std::wstring::size_type s   = res.find( start );
		std::wstring::size_type e   = res.find( end, s );

		while ( ( std::wstring::npos != s ) && ( std::wstring::npos != e ) )
		{
			res = res.substr( 0, s ) + replace + res.substr( e + end.length() );
			s   = res.find( start );
			e   = res.find( end, s );
		}

		return res;
	}


	std::wstring remove( const std::wstring & str, const std::wstring & remove )
	{
		std::wstring            res = str;
		std::wstring::size_type s   = res.find( remove );

		if ( std::wstring::npos != s )		
			res.erase( s, remove.length() );

		return res;
	}


	std::wstring remove_all( const std::wstring & str, const std::wstring & remove )
	{
		std::wstring            res = str;
		std::wstring::size_type s   = res.find( remove );

		while ( std::wstring::npos != s )
		{
			res.erase( s, remove.length() );
			s = res.find( remove );
		}

		return res;
	}


	std::string remove( const std::string & str, const std::string & remove )
	{
		std::string            res = str;
		std::string::size_type s   = res.find( remove );

		if ( std::string::npos != s )		
			res.erase( s, remove.length() );

		return res;
	}


	std::string remove_all( const std::string & str, const std::string & remove )
	{
		std::string            res = str;
		std::string::size_type s   = res.find( remove );

		while ( std::string::npos != s )
		{
			res.erase( s, remove.length() );
			s = res.find( remove );
		}

		return res;
	}

	std::wstring cut( const std::wstring & str,
                      const std::wstring & start,
					  const std::wstring & end )
	{
		std::wstring            res = str;
		std::wstring::size_type s   = start.length() ? res.find( start ) : 0;
		std::wstring::size_type e   = end.length() ? res.find( end, s ) : res.length();

		if ( ( std::wstring::npos != s ) && ( std::wstring::npos != e ) )
		{
			res.erase( s, e + end.length() - s );
		}

		return res;
	}


	std::wstring cut_all( const std::wstring & str,
						  const std::wstring & start,
						  const std::wstring & end )
	{
		std::wstring            res = str;
		std::wstring::size_type s   = ( start.length() ? res.find( start ) : 0 );
		std::wstring::size_type e   = ( end.length() ? res.find( end, s ) : res.length() );

		while ( ( str.length() ) && ( std::wstring::npos != s ) && ( std::wstring::npos != e ) )
		{
			res.erase( s, e + end.length() - s );
			s = start.length() ? res.find( start ) : 0;
			e = end.length() ? res.find( end, s ) : res.length();
		}

		return res;
	}


	std::wstring substring_between( const std::wstring & str,
									const std::wstring & start,
									const std::wstring & end )
	{
		std::wstring            res = L"";
		std::wstring::size_type s   = str.find( start );
		std::wstring::size_type e   = str.find( end, s );

		if ( ( std::wstring::npos != s ) && ( std::wstring::npos != e ) )
		{
			res = str.substr( s + start.length(), e - s - start.length() );
		}

		return res;
	}


	std::wstring substring_between ( const std::wstring & str,
                                     const std::wstring::size_type & start,
                                     const std::wstring::size_type & end )
	{
		std::wstring            res = L"";

		if ( ( std::wstring::npos != start ) && ( std::wstring::npos != end ) )
		{
			res = str.substr( start, end - start + 1 );
		}

		return res;
	}


	std::wstring substring_before  ( const std::wstring & str,
									 const std::wstring & end )
	{
		std::wstring            res = L"";
		std::wstring::size_type e   = str.find( end );

		if ( std::wstring::npos != e )
		{
			res = str.substr( 0, e );
		}

		return res;

	}


	std::wstring substring_before   ( const std::wstring & str,
		const std::wstring::size_type & end  )
	{
		std::wstring  res = L"";
		if ( std::wstring::npos != end )
		{
			res = str.substr(0, end);
		}

		return res;
	}


	std::wstring substring_after   ( const std::wstring & str,
									 const std::wstring & start )
	{
		std::wstring            res = L"";
		std::wstring::size_type s   = str.find( start );

		if ( std::wstring::npos != s )
		{
			res = str.substr( s + start.length() );
		}

		return res;
	}


	std::wstring substring_after   ( const std::wstring & str,
									 const std::wstring::size_type & start  )
	{
		std::wstring  res = L"";
		
		if ( std::wstring::npos != start )
		{
			res = str.substr(start + 1);
		}

		return res;
	}


	std::wstring trim( const std::wstring & str )
	{
		std::wstring res = str;
		for ( size_t i = 0; i < res.length(); ++i )
		{
			if ( ( res[ i ] != L' ' ) && ( res[ i ] != L'\t' ) && ( res[ i ] != 10 ) && ( res[ i ] != 13 ) )
			{
				res.erase( 0, i );
				break;
			}
		}
		for ( int i = res.length() - 1; i >= 0; --i )
		{
			if ( ( res[ i ] != L' ' ) && ( res[ i ] != L'\t' ) && ( res[ i ] != 10 ) && ( res[ i ] != 13 ) )
			{
				res.erase( i + 1 );
				break;
			}
		}

		return res;
	}


	/*
	int	toInt( const std::wstring& wstr)
	{
		std::string str = Encoding::unicode2ansi(wstr);
		return atoi(str.c_str());
	}
	*/


	wchar_t	strFromHex ( const std::wstring& str)
	{
		wchar_t wc = L'';
		int res = 0;

		for(int i = 0; i < str.length(); i++)
		{
			res = res * 16 + charFromHex(str[i]);
		}
		//char p = res;
		wc = res;
		
		
		return wc;
	}


	char	strFromHex ( const std::string& str)
	{
		char wc;
		int res = 0;

		for(int i = 0; i < str.length(); i++)
		{
			res = res * 16 + charFromHex(str[i]);
		}		
		wc = res;	
		
		return wc;
	}


	int	charFromHex ( const wchar_t wchar)
	{
		int p = 0;
		if (wchar >= L'0' && wchar <= L'9')
			p = wchar - L'0';
		else if (wchar >= 'A' && wchar <= 'F')
			p = wchar - L'A' + 10;

		return p;
	}
}