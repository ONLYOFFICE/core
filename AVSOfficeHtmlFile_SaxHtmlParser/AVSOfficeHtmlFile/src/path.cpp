#include "stdafx.h"
#include "path.h"
#include <algorithm>
#include "strings.h"

std::wstring url_by_path( const std::wstring & path )
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

std::wstring full_from_relative( const std::wstring & doc_path,  const std::wstring & rel_path)
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
	else if (0 != doc_path.length())
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

		if (file_path.length() > 0 && file_path[0] != L'\\')	// check '\' file\path delimiter
			tmp += L'\\';

        res = tmp + file_path;
    }

   std::replace( res.begin(), res.end(), L'/', L'\\' );

   return res;
}



std::wstring set_slashes( const std::wstring& path)
{
	std::wstring new_path = path;
	return strings::replace_all(new_path, L"/", L"\\");
}

std::wstring file_from_path ( const std::wstring & path )
{
	std::wstring::size_type indexSlash;

	indexSlash = path.rfind (L"\\");
	if (std::wstring::npos == indexSlash)
		return path;

	return path.substr (indexSlash + 1);
}