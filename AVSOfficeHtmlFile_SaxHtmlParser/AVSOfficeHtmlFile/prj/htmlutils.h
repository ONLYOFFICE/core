#pragma once
#include <algorithm>
#include "..\src\strings.h"

namespace HtmlUtils
{	
	static bool FileExist (CString *sFilePath)
	{
		if ( 0 == sFilePath->Find( _T("file://") ) )
			*sFilePath = sFilePath->Mid( 7/*sizeof( _T("file://") )*/ );

		return ( -1 != _taccess ( sFilePath->GetBuffer(), 0 ) );
	}

	static std::wstring url_by_path( const std::wstring & path )
	{
		std::wstring res = path;

		if ( std::wstring::npos == path.find( L"://" ) )
		{
			wchar_t      lpBuffer [ 1024 ] = { 0 };

			GetFullPathNameW( path.c_str(), 1024, lpBuffer, NULL );
			res = L"file://localhost/";
			res += lpBuffer;

			std::replace( res.begin(), res.end(), L'\\', L'/' );
		}

		return res;
	}

	static std::wstring full_from_relative( const std::wstring & doc_path,  const std::wstring & rel_path)
	{
	    
		std::wstring res = L"";

		if ( ( rel_path.find( L"http" ) != std::wstring::npos ) && ( rel_path.find( L"www" ) != std::wstring::npos ) )
		{
			// HTTP adress; skip.
		}
		else if ( rel_path.find( L"file:" ) == 0 )
		{
			std::wstring file_path = rel_path;
			std::replace( file_path.begin(), file_path.end(), L'/', L'\\' );
			std::wstring::size_type pos = file_path.substr(5, file_path.length() - 5).find_first_not_of(L'\\');
			if (pos != std::wstring::npos)
				res = file_path.substr(pos + 5, file_path.length() - pos - 5);
			else
				res = file_path;
		}
		else if ( rel_path.find( L":" ) != std::wstring::npos )
		{
			// Full path. Leave path string as is.
			res = rel_path;
		}	
		else
		{
			// Relative path. Modify path string.
			wchar_t                   buff     [ 1024 ] = { 0 };
			wchar_t                 * fname             = NULL;
			std::wstring              tmp               = L"";
			std::wstring::size_type   name_pos          = std::wstring::npos;

			GetFullPathNameW( doc_path.c_str(), 1024, buff, &fname );
			tmp = buff;
			name_pos = tmp.find( fname );
			if ( name_pos != std::wstring::npos )
			{
				tmp.erase( name_pos );
			}
			name_pos = tmp.length();
			if ( ( tmp[ tmp.length() - 1 ] != L'\\' ) && ( tmp[ tmp.length() - 1 ] != L'/' ) )
			{
				tmp += L'\\';
			}
			std::wstring file_path = rel_path;
			std::replace( file_path.begin(), file_path.end(), L'/', L'\\' );
			std::replace( tmp.begin(), tmp.end(), L'/', L'\\' );
			tmp = tmp.substr(0, tmp.find_last_of(L"\\"));

			while (file_path.find(L"..\\") != std::wstring::npos)
			{
				std::wstring::size_type pos = file_path.find(L"..\\");
				file_path = file_path.substr(0, pos) + file_path.substr(pos + 3, file_path.length() - pos - 3);

				pos = tmp.find_last_of(L"\\");
				tmp = tmp.substr(0, pos);
			}
			if (file_path.find(L".\\") != std::wstring::npos)
			{
				std::wstring::size_type pos = file_path.find(L".\\");
				file_path = file_path.substr(0, pos) + file_path.substr(pos + 2, file_path.length() - pos - 2);
			}

			res = tmp + L"\\" + file_path;
		}

	   std::replace( res.begin(), res.end(), L'/', L'\\' );

	   return res;
	}



	static std::wstring set_slashes( const std::wstring& path)
	{
		std::wstring new_path = path;
		return strings::replace_all(new_path, L"/", L"\\");
	}

	

	static CStringW AnsiToUnicode (CStringA &sUTF8String, UINT aCodePage)
	{
		LPCSTR pStr = sUTF8String.GetBuffer();
		const int nStrSize = sUTF8String.GetLength();

		const int nWideCharSize = MultiByteToWideChar(aCodePage,         // code page
			  0,														// character-type options
			  pStr,		// string to map
			  nStrSize,       // number of bytes in string
			  NULL,  // wide-character buffer
			  0        // size of buffer
			);

			if (0 == nWideCharSize || ERROR_NO_UNICODE_TRANSLATION == nWideCharSize)
			{
				ATLTRACE2 ("AnsiToUnicode() failed! (MultiByteToWideChar 0x%x)", GetLastError());
				sUTF8String.ReleaseBuffer();
				return L"";
			}
			wchar_t *aWideChars = new wchar_t [nWideCharSize];
			if (NULL == aWideChars)
			{
				ATLTRACE2 ("AnsiToUnicode() failed! (no memory 1)");
				sUTF8String.ReleaseBuffer();
				return L"";
			}

			const int nMultiByteToWideCharResult = MultiByteToWideChar(aCodePage,         // code page
			  0,										// character-type options
			  pStr, // string to map
			  nStrSize,       // number of bytes in string
			  aWideChars,  // wide-character buffer
			  nWideCharSize// size of buffer
			);
			if (0 == nWideCharSize || ERROR_NO_UNICODE_TRANSLATION == nWideCharSize)
			{
				ATLTRACE2 ("AnsiToUnicode() failed! (0x%x)", GetLastError());
				delete [] aWideChars;
				sUTF8String.ReleaseBuffer();
				return L"";
			}
			sUTF8String.ReleaseBuffer();

			CStringW sStr;
			sStr.SetString (aWideChars, nWideCharSize);
			delete [] aWideChars;
			return sStr;
	}

	static CStringW Utf8ToUnicode (CStringA &sAnsiString)
	{
		return AnsiToUnicode (sAnsiString, CP_UTF8);
	}
	
	const int HexChar2Int(const char value)
	{
		if (value >= '0' && value <= '9')
			return value - '0';
		if (value >= 'a' && value <= 'f')
			return 10 + value - 'a';
		if (value >= 'A' && value <= 'F')
			return 10 + value - 'A';
		return 0;
	}

	static void DecodeUrl (CString &psString)
	{
		// В некоторых версиях Htmlayout строка Test%20test может прийти как Test%2520test
		psString.Replace( _T("%25"), _T("%") );

		CStringA sTemp;
		CString sResult;
		int nLen = psString.GetLength();

		bool bPreviousPerc = false;
		for ( int nPos = 0; nPos < nLen; nPos++ )
		{
			if ( '%' != psString.GetAt(nPos) )
			{
				if ( bPreviousPerc )
				{
					sResult += Utf8ToUnicode (sTemp);
					sTemp.Empty();
				}

				sResult += psString.GetAt(nPos);
				bPreviousPerc = false;
			}
			else
			{
				bPreviousPerc = true;
				if ( nPos + 2 < nLen )
				{
					unsigned char unChar1 = HexChar2Int ((char) psString.GetAt(nPos + 1));
					unsigned char unChar2 = HexChar2Int ((char) psString.GetAt(nPos + 2));

					unsigned char unChar = (unChar1 << 4) + unChar2;
					sTemp += (unsigned char)unChar;
				}
				else
					break;

				nPos += 2;
			}
		}

		if (bPreviousPerc && !sTemp.IsEmpty())
			sResult += Utf8ToUnicode (sTemp);

		psString = sResult;
	}

	/*
	static void DecodeUrl (CString *psString)
	{
		// строка Test%20test может быть Test%2520test
		psString->Replace( _T("%25"), _T("%") );

		CStringA sTemp;
		CString sResult;
		int nLen = psString->GetLength();

		bool bPreviousPerc = false;
		for ( int nPos = 0; nPos < nLen; nPos++ )
		{
			if ( '%' != psString->GetAt(nPos) )
			{
				if ( bPreviousPerc )
				{
					sResult += CString( Encoding::utf82unicode( sTemp.GetBuffer() ).c_str() );
					sTemp.Empty();
				}

				sResult += psString->GetAt(nPos);
				bPreviousPerc = false;
			}
			else
			{
				bPreviousPerc = true;
				if ( nPos + 2 < nLen )
				{
					unsigned char unChar1 = HexChar2Int( psString->GetAt(nPos + 1) );
					unsigned char unChar2 = HexChar2Int( psString->GetAt(nPos + 2) );

					unsigned char unChar = (unChar1 << 4) + unChar2;
					sTemp += (unsigned char)unChar;
				}
				else
					break;

				nPos += 2;
			}
		}

		if ( bPreviousPerc && !sTemp.IsEmpty() )
			sResult += CString( Encoding::utf82unicode( sTemp.GetBuffer() ).c_str() );

		*psString = sResult;
	}
	*/
}