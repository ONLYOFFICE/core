//-----------------------------------------------------------------------------
#ifndef AGG_SVG_UTILS_INCLUDED
#define AGG_SVG_UTILS_INCLUDED
//-----------------------------------------------------------------------------
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstdlib> // for atof
#include <cstring>
#include "agg_svg_parse_real.h"
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    //-------------------------------------------------------------------------
    namespace aux
    {
        //---------------------------------------------------------------------
        struct is_numeric_helper
        {
            is_numeric_helper() { init_mask(m_set_mask, s_set); }

            bool operator()(unsigned c) const
            {
                return (m_set_mask[(c >> 3) & (256/8-1)] & (1 << (c & 7)))!= 0;
            }

        private:
            static void init_mask(unsigned char*        mask, 
                                  const unsigned char*  char_set);

            unsigned char m_set_mask[256/8];

            static const unsigned char s_set[];
        };
        //---------------------------------------------------------------------
        inline void is_numeric_helper::init_mask(unsigned char*       mask, 
                                                 const unsigned char* char_set)
        {
            using namespace std;
            memset(mask, 0, 256/8);
            while(*char_set)
            {
                unsigned c = unsigned(*char_set++) & 0xFF;
                mask[c >> 3] |= 1 << (c & 7);
            }
        }
        //---------------------------------------------------------------------
        struct is_wsp_helper
        {
            is_wsp_helper() { init_mask(m_set_mask, s_set); }

            bool operator()(unsigned c) const
            {
                return (m_set_mask[(c >> 3) & (256/8-1)] & (1 << (c & 7)))!= 0;
            }

        private:
            static void init_mask(unsigned char*        mask, 
                                  const unsigned char*  char_set);

            unsigned char m_set_mask[256/8];

            static const unsigned char s_set[];
        };
        //---------------------------------------------------------------------
        inline void is_wsp_helper::init_mask(unsigned char*       mask, 
                                             const unsigned char* char_set)
        {
            using namespace std;
            memset(mask, 0, 256/8);
            while(*char_set)
            {
                unsigned c = unsigned(*char_set++) & 0xFF;
                mask[c >> 3] |= 1 << (c & 7);
            }
        }
        //---------------------------------------------------------------------
    } // namespace aux
    //-------------------------------------------------------------------------
    std::string  to_str (const wchar_t* chars, int len = 0);
	std::wstring to_wstr(const char* chars, int len = 0);
	//-------------------------------------------------------------------------
    inline bool is_wsp_or_eol(char c)
    {
        static aux::is_wsp_helper helper;
        return helper(c);
    }
    //-------------------------------------------------------------------------
	inline bool is_wsp( char c)
	{
        return c == ' ' || c == '\t';
	}	
    //-------------------------------------------------------------------------
    inline bool is_numeric(char c)
    {
        static aux::is_numeric_helper helper;
        return helper(c);
    }
	//-------------------------------------------------------------------------
	inline double to_double(const char* str)
	{
        using namespace std;
		while(*str == ' ') ++str;

        double val;
        const char* endptr;
        parse_real pd;
        if (!pd.parse(str, str + strlen(str), &val, &endptr))
            return 0.;
        return val;
	}
	//-------------------------------------------------------------------------
	inline const char* next_number(const char* str)
	{
		while(*str &&  (*str == ' ' || *str == ',' || *str == ';')) ++str;
		while(*str && !(*str == ' ' || *str == ',' || *str == ';')) ++str;
		return str;
	}
	//-------------------------------------------------------------------------
	inline const char* get_token(const char* str, char* buf, unsigned max_len)
	{
		while(*str &&  (*str == ' ' || *str == ',' || *str == ';')) ++str;
		while(*str && !(*str == ' ' || *str == ',' || *str == ';'))
		{
			if(max_len == 0) break;
			*buf++ = *str++;
			--max_len;
		}
		*buf = 0;
		return str;
	}
    //-------------------------------------------------------------------------
    inline void left_trim(const char*& str)
    {
        while( is_wsp(*str) ) ++str;
	}
	//-------------------------------------------------------------------------
	inline void left_trim(char*& str_begin, char* str_end )
	{
        using namespace std;
		while(str_begin < str_end && isspace(*str_begin)) ++str_begin;
	}
	//-------------------------------------------------------------------------
	inline void right_trim(char* str_begin, char*& str_end, 
                           char additionalTokenToSkip = ' ')
	{		
        using namespace std;
		char* ts = str_end;
		while(str_end > str_begin && 
			(*str_end == additionalTokenToSkip || isspace(*str_end))) --str_end;
		if(str_end < ts) ++str_end;
	}
    //-------------------------------------------------------------------------
    inline void right_trim( const char* str )
    {        
        using namespace std;
        char* p = const_cast<char*>(str) + strlen(str)-1;
        while( p >= str && isspace(*p) ) { *p=0x0; --p; }        
	}
    //-------------------------------------------------------------------------
    inline bool str_starts( const char* str1, char ch )
    {
        return ( str1[0] == ch );
    } 
    //-------------------------------------------------------------------------
    inline bool str_starts( const char* str1, const char* str2 )
    {
        return ( strncmp( str1, str2, strlen(str2) )==0 );
    }
    //-------------------------------------------------------------------------
    inline bool str_ends( const char* str1, char ch )
    {
        return ( str1[ strlen(str1)-1 ] == ch );
    }
    //-------------------------------------------------------------------------
    inline bool str_ends( const char* str1, const char* str2 )
    {
        size_t len1 = strlen( str1 );
        size_t len2 = strlen( str2 );

        if( len1 < len2 )
            return false;

        return ( strncmp( str1+len1-len2, str2, len2)==0 );
    }
    //------------------------------------------------------------------------- 
}
}

#endif 

