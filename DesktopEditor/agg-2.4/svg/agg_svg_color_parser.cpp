//-----------------------------------------------------------------------------
#include "agg_svg_color_parser.h"
#include "agg_svg_utils.h"
#include <cstdlib> // for wcstombs
#include <cstring> 
#include "agg_svg_basics.h"
#include "agg_svg_exception.h"
#include "member_comparer.h"
#include <utility>
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{

    //-------------------------------------------------------------------------
    VEC_COLOR_RGBA  color_parser::m_colors_to_rgba;
    VEC_ULONG_COLOR color_parser::m_ulong_to_colors;
    //-------------------------------------------------------------------------
    const char* color_parser::m_input_str = init();
    //-------------------------------------------------------------------------
    namespace 
    {
        //---------------------------------------------------------------------
        inline unsigned long make_color(unsigned char r,
                                        unsigned char g,
                                        unsigned char b)
        {
            return (unsigned long)(r)      |
                   (unsigned long)(g) << 8 |
                   (unsigned long)(b) << 16;
        }
        //---------------------------------------------------------------------
        template <class T>
        struct find_result
        {
            typedef std::pair<typename T::iterator, typename T::iterator> type;
        };
    }
    //-------------------------------------------------------------------------
    #define ADD_COLOR( COLOR, R, G, B )\
    m_colors_to_rgba.push_back( VEC_COLOR_RGBA::value_type(COLOR,agg::rgba8(R,G,B)) );\
    m_ulong_to_colors.push_back(VEC_ULONG_COLOR::value_type(make_color(R, G, B), COLOR));
    //-------------------------------------------------------------------------
const char* color_parser::init()
{	
    m_colors_to_rgba.reserve( 150 );
    m_ulong_to_colors.reserve( 150 );

    ADD_COLOR( "aliceblue", 240, 248, 255);
	ADD_COLOR( "antiquewhite", 250, 235, 215); 
	ADD_COLOR( "aqua",  0, 255, 255); 
	ADD_COLOR( "aquamarine", 127, 255, 212); 
	ADD_COLOR( "azure", 240, 255, 255); 
	ADD_COLOR( "beige", 245, 245, 220); 
	ADD_COLOR( "bisque", 255, 228, 196); 
	ADD_COLOR( "black",  0, 0, 0); 
	ADD_COLOR( "blanchedalmond", 255, 235, 205); 
	ADD_COLOR( "blue",  0, 0, 255); 
	ADD_COLOR( "blueviolet", 138, 43, 226); 
	ADD_COLOR( "brown", 165, 42, 42); 
	ADD_COLOR( "burlywood", 222, 184, 135); 
	ADD_COLOR( "cadetblue",  95, 158, 160); 
	ADD_COLOR( "chartreuse", 127, 255, 0); 
	ADD_COLOR( "chocolate", 210, 105, 30); 
	ADD_COLOR( "coral", 255, 127, 80); 
	ADD_COLOR( "cornflowerblue", 100, 149, 237); 
	ADD_COLOR( "cornsilk", 255, 248, 220); 
	ADD_COLOR( "crimson", 220, 20, 60); 
	ADD_COLOR( "cyan",  0, 255, 255); 
	ADD_COLOR( "darkblue",  0, 0, 139); 
	ADD_COLOR( "darkcyan",  0, 139, 139); 
	ADD_COLOR( "darkgoldenrod", 184, 134, 11); 
	ADD_COLOR( "darkgray", 169, 169, 169); 
	ADD_COLOR( "darkgreen",  0, 100, 0); 
	ADD_COLOR( "darkgrey", 169, 169, 169); 
	ADD_COLOR( "darkkhaki", 189, 183, 107); 
	ADD_COLOR( "darkmagenta", 139, 0, 139); 
	ADD_COLOR( "darkolivegreen",  85, 107, 47); 
	ADD_COLOR( "darkorange", 255, 140, 0); 
	ADD_COLOR( "darkorchid", 153, 50, 204); 
	ADD_COLOR( "darkred", 139, 0, 0); 
	ADD_COLOR( "darksalmon", 233, 150, 122); 
	ADD_COLOR( "darkseagreen", 143, 188, 143); 
	ADD_COLOR( "darkslateblue",  72, 61, 139); 
	ADD_COLOR( "darkslategray",  47, 79, 79); 
	ADD_COLOR( "darkslategrey",  47, 79, 79); 
	ADD_COLOR( "darkturquoise",  0, 206, 209); 
	ADD_COLOR( "darkviolet", 148, 0, 211); 
	ADD_COLOR( "deeppink", 255, 20, 147); 
	ADD_COLOR( "deepskyblue",  0, 191, 255); 
	ADD_COLOR( "dimgray", 105, 105, 105); 
	ADD_COLOR( "dimgrey", 105, 105, 105); 
	ADD_COLOR( "dodgerblue",  30, 144, 255); 
	ADD_COLOR( "firebrick", 178, 34, 34); 
	ADD_COLOR( "floralwhite", 255, 250, 240); 
	ADD_COLOR( "forestgreen",  34, 139, 34); 
	ADD_COLOR( "fuchsia", 255, 0, 255); 
	ADD_COLOR( "gainsboro", 220, 220, 220); 
	ADD_COLOR( "ghostwhite", 248, 248, 255); 
	ADD_COLOR( "gold", 255, 215, 0); 
	ADD_COLOR( "goldenrod", 218, 165, 32); 
	ADD_COLOR( "gray", 128, 128, 128); 
	ADD_COLOR( "grey", 128, 128, 128); 
	ADD_COLOR( "green",  0, 128, 0); 
	ADD_COLOR( "greenyellow", 173, 255, 47); 
	ADD_COLOR( "honeydew", 240, 255, 240); 
	ADD_COLOR( "hotpink", 255, 105, 180); 
	ADD_COLOR( "indianred", 205, 92, 92); 
	ADD_COLOR( "indigo",  75, 0, 130); 
	ADD_COLOR( "ivory", 255, 255, 240); 
	ADD_COLOR( "khaki", 240, 230, 140); 
	ADD_COLOR( "lavender", 230, 230, 250); 
	ADD_COLOR( "lavenderblush", 255, 240, 245); 
	ADD_COLOR( "lawngreen", 124, 252, 0); 
	ADD_COLOR( "lemonchiffon", 255, 250, 205); 
	ADD_COLOR( "lightblue", 173, 216, 230); 
	ADD_COLOR( "lightcoral", 240, 128, 128); 
	ADD_COLOR( "lightcyan", 224, 255, 255); 
	ADD_COLOR( "lightgoldenrodyellow", 250, 250, 210); 
	ADD_COLOR( "lightgray", 211, 211, 211); 
	ADD_COLOR( "lightgreen", 144, 238, 144); 
	ADD_COLOR( "lightgrey", 211, 211, 211); 
	ADD_COLOR( "lightpink", 255, 182, 193); 
	ADD_COLOR( "lightsalmon", 255, 160, 122); 
	ADD_COLOR( "lightseagreen",  32, 178, 170); 
	ADD_COLOR( "lightskyblue", 135, 206, 250); 
	ADD_COLOR( "lightslategray", 119, 136, 153); 
	ADD_COLOR( "lightslategrey", 119, 136, 153); 
	ADD_COLOR( "lightsteelblue", 176, 196, 222); 
	ADD_COLOR( "lightyellow", 255, 255, 224); 
	ADD_COLOR( "lime",  0, 255, 0); 
	ADD_COLOR( "limegreen",  50, 205, 50); 
	ADD_COLOR( "linen", 250, 240, 230); 
	ADD_COLOR( "magenta", 255, 0, 255); 
	ADD_COLOR( "maroon", 128, 0, 0); 
	ADD_COLOR( "mediumaquamarine", 102, 205, 170); 
	ADD_COLOR( "mediumblue",  0, 0, 205); 
	ADD_COLOR( "mediumorchid", 186, 85, 211); 
	ADD_COLOR( "mediumpurple", 147, 112, 219); 
	ADD_COLOR( "mediumseagreen",  60, 179, 113); 
	ADD_COLOR( "mediumslateblue", 123, 104, 238); 
	ADD_COLOR( "mediumspringgreen",  0, 250, 154); 
	ADD_COLOR( "mediumturquoise",  72, 209, 204); 
	ADD_COLOR( "mediumvioletred", 199, 21, 133); 
	ADD_COLOR( "midnightblue",  25, 25, 112); 
	ADD_COLOR( "mintcream", 245, 255, 250); 
	ADD_COLOR( "mistyrose", 255, 228, 225); 
	ADD_COLOR( "moccasin", 255, 228, 181); 
	ADD_COLOR( "navajowhite", 255, 222, 173); 
	ADD_COLOR( "navy",  0, 0, 128); 
	ADD_COLOR( "oldlace", 253, 245, 230); 
	ADD_COLOR( "olive", 128, 128, 0); 
	ADD_COLOR( "olivedrab", 107, 142, 35); 
	ADD_COLOR( "orange", 255, 165, 0); 
	ADD_COLOR( "orangered", 255, 69, 0); 
	ADD_COLOR( "orchid", 218, 112, 214); 
	ADD_COLOR( "palegoldenrod", 238, 232, 170); 
	ADD_COLOR( "palegreen", 152, 251, 152); 
	ADD_COLOR( "paleturquoise", 175, 238, 238); 
	ADD_COLOR( "palevioletred", 219, 112, 147); 
	ADD_COLOR( "papayawhip", 255, 239, 213); 
	ADD_COLOR( "peachpuff", 255, 218, 185); 
	ADD_COLOR( "peru", 205, 133, 63); 
	ADD_COLOR( "pink", 255, 192, 203); 
	ADD_COLOR( "plum", 221, 160, 221); 
	ADD_COLOR( "powderblue", 176, 224, 230); 
	ADD_COLOR( "purple", 128, 0, 128); 
	ADD_COLOR( "red", 255, 0, 0); 
	ADD_COLOR( "rosybrown", 188, 143, 143); 
	ADD_COLOR( "royalblue",  65, 105, 225); 
	ADD_COLOR( "paddlebrown", 139, 69, 19); 
	ADD_COLOR( "palmon", 250, 128, 114); 
	ADD_COLOR( "pandybrown", 244, 164, 96); 
	ADD_COLOR( "peagreen",  46, 139, 87); 
	ADD_COLOR( "peashell", 255, 245, 238); 
	ADD_COLOR( "pienna", 160, 82, 45); 
	ADD_COLOR( "pilver", 192, 192, 192); 
	ADD_COLOR( "pkyblue", 135, 206, 235); 
	ADD_COLOR( "plateblue", 106, 90, 205); 
	ADD_COLOR( "plategray", 112, 128, 144); 
	ADD_COLOR( "plategrey", 112, 128, 144); 
	ADD_COLOR( "pnow", 255, 250, 250); 
	ADD_COLOR( "ppringgreen",  0, 255, 127); 
	ADD_COLOR( "pteelblue",  70, 130, 180); 
	ADD_COLOR( "tan", 210, 180, 140); 
	ADD_COLOR( "teal",  0, 128, 128); 
	ADD_COLOR( "thistle", 216, 191, 216); 
	ADD_COLOR( "tomato", 255, 99, 71); 
	ADD_COLOR( "turquoise",  64, 224, 208); 
	ADD_COLOR( "violet", 238, 130, 238); 
	ADD_COLOR( "wheat", 245, 222, 179); 
	ADD_COLOR( "white", 255, 255, 255); 
	ADD_COLOR( "whitesmoke", 245, 245, 245); 
	ADD_COLOR( "yellow", 255, 255, 0); 
	ADD_COLOR( "yellowgreen", 154, 205, 50); 

    //--------------------------------------
    std::sort( 
        m_colors_to_rgba.begin(), 
        m_colors_to_rgba.end(), 
        make_comparer(&VEC_COLOR_RGBA::value_type::first, string_comparer()));
    //--------------------------------------    
    std::sort( 
        m_ulong_to_colors.begin(), 
        m_ulong_to_colors.end(),
        make_comparer(&VEC_ULONG_COLOR::value_type::first));
    //--------------------------------------

	return 0;	
}


//-- parses color string represented in hex format ( for ex. #FFF or #FFFFFF )
//-- return value - position where parser stopped
const char* color_parser::parse_color_hex(const char* str, agg::rgba8& color)
{    
    unsigned c = 0u;
    unsigned r = 0u;
    unsigned g = 0u;
    unsigned b = 0u;
	color.a = 255;	

    //-- find the length hex-color in input str( i.e. find_first_not_of )
    int len = static_cast<int>(strspn( str, "#0123456789abcdefABCDEF" ));

    switch( len )
    { 

    case 7: //-- format #FFFFFF consists of 7 chars	   
        
        sscanf( str+1, "%6x", &c );
        color = agg::rgb8_packed(c);
		return str+7;

    case 4: //-- format #FFF consists of 4 chars
        sscanf( str+1, "%1x", &r );
        sscanf( str+2, "%1x", &g );
        sscanf( str+3, "%1x", &b );
   
        //-- convert format #FFF to #FFFFFF (every digit should be doubled)		
		color.r = (16 * r) + r;
		color.g = (16 * g) + g;
		color.b = (16 * b) + b;   			
		return str+4;

    default:
		// Output an error and return str+len as we cannot read it
		throw exception("[%s] is an invalid hex color format (expected '#FFF' or '#FFFFFF')", str);
    }//switch
		
}

//-- parses color string represented in rgb format ( for ex. rgba(255,255,255,1.0) )
//-- return value - position where parser stopped
const char* color_parser::parse_color_rgb( const char* str, agg::rgba8& color )
{	
	
	bool rgba_format = false;	
	
	//-- check 'rgba' syntax
	if( str[ RGB_KEYWORD_LEN ] == 'a' && str[ RGB_KEYWORD_LEN + 1] == '(' )
		rgba_format = true;
		
	//-- check 'rgb(' syntax	
	if( !rgba_format && str[ RGB_KEYWORD_LEN ] != '(' )
	{
		throw exception("[%s] has a wrong syntax at position %i", str, RGB_KEYWORD_LEN+1);

		return 0;
	}
	
    //pos to begin search
    str += ( rgba_format ? RGB_KEYWORD_LEN + 2 : RGB_KEYWORD_LEN + 1 );
	
	color.r = get_next_rgb_value( str, ',' );
	color.g = get_next_rgb_value( str, ',' );	
	
	if( rgba_format )
	{
		color.b = get_next_rgb_value( str, ',' );		
		color.a = static_cast<unsigned char>( 255 * get_opacity_value(str) + 0.5 );
	}
	else
	{
		color.b = get_next_rgb_value( str, ')' );
		color.a = 255;
	}	
	
	return str;
	
}

//--reads next RGB value from str, using stop_sign as delimiter    
unsigned char color_parser::get_next_rgb_value( const char*& str, char stop_sign )
{	
	bool value_is_percent = false; 
	
	int value = extract_next_value( str, stop_sign, value_is_percent, 0 );	
	
	if( value_is_percent )
		value = value * 255/100; //--handle percents
	
	if( value < 0 || value > 255 )
    {
		throw exception("[%s] - the value %i is out of range [0,255]", m_input_str, value);

		return 0;
    }
	
	return value;
}

//--reads opacity value from str
//--returns value in range [ 0.0 - 1.0 ]
float color_parser::get_opacity_value( const char*& str )
{
	
	bool  value_is_percent = false;
	float fValue = extract_next_value( str, ')', value_is_percent, 0.0f, "%3f" );
	
	if( value_is_percent )
    {
		throw exception("[%s] - percents are not allowed in opacity values [%f%%]", m_input_str, fValue);

		return 1.0;
    }
	
	//-- check bounds
	if( fValue < 0.0f ) fValue = 0.0f;
	if( fValue > 1.0f ) fValue = 1.0f;
	
	return fValue;
	
}


//-- parses input string and fills rgba structure.
//-- returns pointer where the parser stopped.
const char* color_parser::parse_color( const char* str, agg::rgba8& color )
{	
	assert( str != 0 );	

    m_input_str = str;

    left_trim( str );  	
	
	//if( strlen(str) < 4 )	// at least 4 signs in #FFF format ( all other formats are longer )
	//	throw exception( "[%s]- wrong syntax : too short input string", str );    
	
	if( str_starts(str, '#') ) 
        return parse_color_hex( str, color );		
	
	if( str_starts(str,RGB_KEYWORD) ) //-- check if string begins with "rgb"
	    return parse_color_rgb( str, color );	

	//-- assume that we have keyword i.e. named color	
	
	//-- find where is the end of keyword in input str(i.e. find_first_not_of)    
    int pos = static_cast<int>(strspn( str, "abcdefghigklmnopqrstuvwxyz" ));

    if( pos == 0 )
	{
		throw exception("[%s] is an unknown color or value or can not be parsed", str);

		return 0;
	}

 	// lets try to find this color in named colors vector
    VEC_COLOR_RGBA::iterator it;

    it = std::lower_bound(m_colors_to_rgba.begin(),
                          m_colors_to_rgba.end(),
                          str,
                          make_comparer
                          (
                           &VEC_COLOR_RGBA::value_type::first, 
                           string_n_comparer(pos)
                          ));

	if( it == m_colors_to_rgba.end() )
	{
		throw exception("[%s] is an unknown color name (lowercase?)", str);

		return 0;
	}  
	
	color = it->second;
	
	return str + pos;
}


//-- parses input string and fills rgba structure.
//-- returns pointer where the parser stopped.
const wchar_t* color_parser::parse_color( const wchar_t* wstr, agg::rgba8& color )
{
    using namespace std;

	char szBuf[ 256 ];
	
	size_t nLen = wcslen(wstr);
	
	if( nLen != wcstombs( szBuf, wstr, nLen + 1 ) )
	{
		throw exception("[%s] is an invalid color string", to_str(wstr).c_str());

		return 0;
	}	
	
	return  wstr + ( parse_color(szBuf, color) - szBuf );
}


agg::rgba8 color_parser::parse_str( const char* str )
{
    agg::rgba8 color(0, 0, 0, 0);
    parse_color( str, color );
    return color;
}


//-- extracts next value from string ( R , G, B or Opacity )
template<class T>
T color_parser::extract_next_value( 
                    const char*& str, char stop_sign, bool& value_is_percent, 
                    T dummy, const char* fmt )
{
    using namespace std;

    //STATIC_ASSERT( 
       assert( sizeof(T) == sizeof(int) || sizeof(T) == sizeof(float) );
      //  Extract_next_value_supports_only_int_and_float )        

	
	value_is_percent = false;
	
	//--find value start position ( find_first_of )
	int pos_value_begin = static_cast<int>(strcspn( str, " -0123456789" ));
	
	if( pos_value_begin != 0 )
    {
		throw exception("[%s] has a wrong syntax at position %i", m_input_str, str-m_input_str+1);

		return 0;
    }	
	
    left_trim( str );//-- skip leading spaces
	
	//--find value stop position 
    const char * p = strchr( str, stop_sign );	
	
	if( p == 0 ) 
	{
		throw exception("[%s] found, [%c] expected", m_input_str, stop_sign);

		return 0;
	}

    if( *(p-1) == '%' ) //if value is a percent
        value_is_percent = true;	

    T value;
    int res = sscanf( str, fmt, &value );
    
    if( res != 1 )
	{
		throw exception("[%s] - value [%s] can not be converted", m_input_str, str);
        
		return 0;
	}
    
    str = p+1; //--go to next value in str

    return value;	
}
    //-------------------------------------------------------------------------
    //-- from given numbers (r,g,b,a) composes the ASCII-string of the color 
    std::string color_parser::compose_color_string(unsigned char r, 
                                                   unsigned char g, 
                                                   unsigned char b, 
                                                   unsigned char a)
    {
        float fOpacity = 0.01f * ( 0.5f + (a / 255.0f) * 100.f);   
    
	    if (a != 255)
        {
		    // !EVC FIX: No std::ostringstream on WindowsCe
		    char buf[1024];
            unsigned r_ = r;
            unsigned g_ = g;
            unsigned b_ = b;
		    sprintf(buf, "rgba(%d,%d,%d,%.1f)", r_, g_, b_, fOpacity);
		    return std::string(buf); 
		    /*
            std::ostringstream oss;
            oss << "rgba(" << (int)r << "," << (int)g << "," << (int)b << "," << fOpacity << ")";        
            return oss.str();
            */
        }

        unsigned long ulColor = ((unsigned long)(r) | (unsigned long)(g) << 8 | (unsigned long)(b) << 16 );
	
	    // lets try to find this color in color keywords vector
        std::pair<VEC_ULONG_COLOR::iterator, VEC_ULONG_COLOR::iterator> 
        result = 
             std::equal_range(m_ulong_to_colors.begin(),
                              m_ulong_to_colors.end(),
                              ulColor,
                              make_comparer
                              (
                                &VEC_ULONG_COLOR::value_type::first,
                                std::less<VEC_ULONG_COLOR::value_type::first_type>()
                              ));
        if (result.first != result.second)
            return result.first->second;

		char szColor[8];	

	    //-- if we have double digits like #FFFFFF - we should 
        //--  return string in short hex format( #FFF )
	    if( (r & 0x0F) == (r >> 4)  && (g & 0x0F) == (g >> 4) && (b & 0x0F) == (b >> 4) )
	    {
		    r = (r >> 4);
		    g = (g >> 4);
		    b = (b >> 4);

            int ret = sprintf( szColor, "#%x%x%x" , r,g,b );
		    assert( 4 == ret );

	    }else
        {
            int ret = sprintf( szColor, "#%02x%02x%02x" , r,g,b );
		    assert( 7 == ret);
        }

	    return std::string( szColor );
    }
    //-------------------------------------------------------------------------
    //-- from given numbers (r,g,b,a) composes the ASCII-string of the color 
    std::string color_parser::compose_color_string(unsigned char r, 
                                                   unsigned char g, 
                                                   unsigned char b)
    {
        unsigned long color = make_color(r, g, b);
	
	    // lets try to find this color in color keywords vector
        find_result<VEC_ULONG_COLOR>::type result; 
        result = std::equal_range
                 (
                    m_ulong_to_colors.begin(),
                    m_ulong_to_colors.end(),
                    color,
                    make_comparer
                    (
                        &VEC_ULONG_COLOR::value_type::first,
                        std::less<VEC_ULONG_COLOR::value_type::first_type>()
                    )
                 );

        if (result.first != result.second)
            return result.first->second;

		char buf[1024];
        unsigned r_ = r;
        unsigned g_ = g;
        unsigned b_ = b;
		sprintf(buf, "rgb(%d,%d,%d)", r_, g_, b_);
		return std::string(buf); 
    }

} // namespace svg
}
