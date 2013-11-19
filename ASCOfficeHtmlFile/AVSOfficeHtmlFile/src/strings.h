#include "stdafx.h"
#include <string>
#include "comutil.h"

namespace strings
{
    _bstr_t & make_lower           ( _bstr_t & string );	
    std::wstring replace           ( const std::wstring & str,
                                     const std::wstring & start,
                                     const std::wstring & end,
                                     const std::wstring & replace );
    std::wstring replace           ( const std::wstring & str,
                                     const std::wstring & what,
                                     const std::wstring & with );
    std::wstring replace_all       ( const std::wstring & str,
                                     const std::wstring & start,
                                     const std::wstring & end,
                                     const std::wstring & replace );
    std::wstring replace_all       ( const std::wstring & str,
                                     const std::wstring & what,
                                     const std::wstring & with );


    std::wstring remove            ( const std::wstring & str,
                                     const std::wstring & remove );
    std::wstring remove_all        ( const std::wstring & str,
                                     const std::wstring & remove );
	std::string remove             ( const std::string & str,
                                     const std::string & remove );
    std::string remove_all         ( const std::string & str,
                                     const std::string & remove );


    std::wstring cut               ( const std::wstring & str,
                                     const std::wstring & start   = L"",
                                     const std::wstring & end     = L"" );
    std::wstring cut_all           ( const std::wstring & str,
                                     const std::wstring & start   = L"",
                                     const std::wstring & end     = L"");
    std::wstring substring_between ( const std::wstring & str,
                                     const std::wstring & start,
                                     const std::wstring & end );
	std::wstring substring_between ( const std::wstring & str,
                                     const std::wstring::size_type & start,
                                     const std::wstring::size_type & end );
    std::wstring substring_before  ( const std::wstring & str,
                                     const std::wstring & end );
    std::wstring substring_after   ( const std::wstring & str,
                                     const std::wstring & start );
	std::wstring substring_before  ( const std::wstring & str,
                                     const std::wstring::size_type & end  );
	std::wstring substring_after   ( const std::wstring & str,
                                     const std::wstring::size_type & start  );
    std::wstring trim              ( const std::wstring & str );

	int			 toInt			   ( const std::wstring& wstr);
	
	wchar_t	strFromHex ( const std::wstring& str);
	char	strFromHex ( const std::string& str);
	int	   charFromHex ( const wchar_t wchar);
};