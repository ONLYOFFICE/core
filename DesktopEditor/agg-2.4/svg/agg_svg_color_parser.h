//-----------------------------------------------------------------------------
#ifndef AGG_SVG_COLOR_PARSER_INCLUDED
#define AGG_SVG_COLOR_PARSER_INCLUDED
//-----------------------------------------------------------------------------
#if defined(_MSC_VER) && _MSC_VER < 1300
#pragma warning( disable : 4786 ) // ident trunc to '255' chars in debug info
#endif
//-----------------------------------------------------------------------------
#include "agg_svg_basics.h"
#include <string>
#include <vector>
#include <cassert>
#include <utility>
//-----------------------------------------------------------------------------
#define RGB_KEYWORD         "rgb"
#define RGB_KEYWORD_LEN     sizeof(RGB_KEYWORD)-1    
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    //-------------------------------------------------------------------------
    typedef std::vector<std::pair<const char*,agg::rgba8> >    VEC_COLOR_RGBA;
    typedef std::vector<std::pair<unsigned long,const char*> > VEC_ULONG_COLOR;  
    //-------------------------------------------------------------------------
    class color_parser  
    {
    public:
        static agg::rgba8 parse_str(const char* str);
        
        //-- parses input string and fills agg::rgba8 structure.
        //-- returns pointer where the parser stopped.    
        static const char* parse_color( const char* str, agg::rgba8& color );
        static const wchar_t* parse_color( const wchar_t* str, agg::rgba8& color ); 

        //-- from given numbers (r,g,b,a) composes the ASCII-string of the color 
        static std::string compose_color_string(unsigned char r, 
                                                unsigned char g, 
                                                unsigned char b, 
                                                unsigned char a);        
        //-- from given numbers (r,g,b,a) composes the ASCII-string of the color 
        static std::string compose_color_string(unsigned char r, 
                                                unsigned char g, 
                                                unsigned char b);
        
    private:
        
        //-- parses color string represented in hex format ( #FFF or #FFFFFF )    
        static const char* parse_color_hex( const char* str, agg::rgba8& color );
        
        //-- parses color string represented in rgb format( ex: rgba(255,255,255,1.0) )    
        static const char* parse_color_rgb( const char* str, agg::rgba8& color );
        
        //-- reads next RGB value from str, using stop_sign as delimiter    
        static unsigned char get_next_rgb_value( const char*& str, char stop_sign );
        
        //-- reads opacity value from str
        static float get_opacity_value( const char*& str );
        
        //-- extracts next value from str ( R, G, B or Opacity )
        template<class T>       
        static T extract_next_value( 
                    const char*& str, char stop_sign, bool& value_is_percent, 
                    T dummy, const char* fmt="%3d" );
        
        static const char* init();  
        
    private:
        static const char* m_input_str;    
        
        static VEC_COLOR_RGBA  m_colors_to_rgba;
        static VEC_ULONG_COLOR m_ulong_to_colors;

    };//color_parser class
    
} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_COLOR_PARSER_INCLUDED

