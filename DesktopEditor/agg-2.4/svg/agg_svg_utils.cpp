
#include "agg_svg_utils.h"
#include <cstdlib> // for wcstombs

namespace agg
{
namespace svg
{
namespace aux
{
    //-------------------------------------------------------------------------
    const unsigned char is_numeric_helper::s_set[] = "0123456789+-.eE"; 
    //-------------------------------------------------------------------------
    const unsigned char is_wsp_helper::s_set[] = " \t\n\r"; 
    //-------------------------------------------------------------------------
}
std::string to_str(const wchar_t* chars, int len )
{
    using namespace std;

	if( len==0 ) len = (int)wcslen( chars );
	
	std::string result_str;
	if (len <= 512)
	{
		char buf[512];
		wcstombs(buf, chars, len);
		
		return std::string( buf, len );
		
	}else
	{
		std::vector<char> vecch;
		vecch.reserve(len);
		
		wcstombs( &vecch[0], chars, len );
		return std::string( &vecch[0], len );
	}
	
}


std::wstring to_wstr(const char* chars, int len )
{
    using namespace std;

	if( len == 0 ) len = (int)strlen(chars);

	std::wstring result_str;
	if (len <= 512)
	{
		wchar_t buf[512];
		mbstowcs(buf, chars, len);
		
		return std::wstring( buf, len );
		
	}else
	{
		std::vector<wchar_t> vecwch;
		vecwch.reserve(len);
		
		mbstowcs( &vecwch[0], chars, len );
		return std::wstring( &vecwch[0], len );
	}
	
}


}
}
