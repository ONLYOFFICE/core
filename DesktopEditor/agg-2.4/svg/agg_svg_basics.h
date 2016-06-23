#ifndef AGG_SVG_BASICS_INCLUDED
#define AGG_SVG_BASICS_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <string>
#include "agg_array.h"
#include "agg_pixfmt_rgba.h"
#include "platform/agg_platform_support.h"
#include "agg_svg_defines.h"

namespace agg
{
namespace svg
{
    //-------------------------------------------------------------------------
    // The main container used to store binary SVG data
	typedef agg::pod_bvector<agg::int8u,14> data_container;
    //-------------------------------------------------------------------------
    template<class Container> class data_accessor;
    //-------------------------------------------------------------------------
    typedef data_accessor<data_container> data_accessor_type;
    //-------------------------------------------------------------------------
    typedef double coord_type;
    //-------------------------------------------------------------------------
    typedef char char_type;
    //-------------------------------------------------------------------------
	const pix_format_e pix_format = agg::pix_format_bgra32; 
    //-------------------------------------------------------------------------
    typedef agg::rgba8            color_type;               
    typedef agg::pixel32_type     pixel_type;               
    typedef agg::order_bgra       component_order;          
    typedef agg::rect_d           rectangle;                
    //-------------------------------------------------------------------------
    struct length
    {
        coord_type value;
        units2_e   unit;
    };
    //-------------------------------------------------------------------------
    typedef length coordinate;
    //-------------------------------------------------------------------------
    typedef agg::pod_auto_array<color_type, 256> gradient_lut_type; 
    //-------------------------------------------------------------------------
    struct string_comparer
    {
        bool operator()(const char* lhs, const char* rhs) const
        {
            using namespace std;
            return strcmp(lhs, rhs) < 0;
        }
    };
    //-------------------------------------------------------------------------
    struct string_n_comparer
    {
        string_n_comparer(unsigned len) : m_len(len) {}
        
        bool operator()(const char* lhs, const char* rhs) const
        {
            using namespace std;
            return strncmp(lhs, rhs, m_len) < 0;
        }
        
        unsigned m_len;
    };

} // namespace svg
} // namespace agg

#endif // ifndef AGG_SVG_BASICS_INCLUDED

