//-----------------------------------------------------------------------------
#include "agg_svg_attributes_map.h"
#include "agg_svg_utils.h"
#include "agg_svg_basics.h"
#include "member_comparer.h"
#include <vector>
#include <utility>
#include <algorithm>
#include <cassert>
#include <cstring>
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{

    namespace
    {
        typedef std::vector<std::pair<const char*, attr_e> > map_type;
        map_type m_map;
        unsigned m_names_positions[end_of_attr];
    }

    #define ADD_ATTR(name, value) \
    m_map.push_back(map_type::value_type(name, value));
    //-------------------------------------------------------------------------
    attributes_map::attributes_map()
    {
        memset(m_names_positions, 0, sizeof(unsigned)*end_of_attr);
        m_map.reserve(end_of_attr);
        int index = 0;

        ADD_ATTR( "color",                        attr_color );
        ADD_ATTR( "cx",                           attr_cx );
        ADD_ATTR( "cy",                           attr_cy );
        ADD_ATTR( "d",                            attr_d );
        ADD_ATTR( "fill",                         attr_fill );
        ADD_ATTR( "fill-opacity",                 attr_fill_opacity );
        ADD_ATTR( "fill-rule",                    attr_fill_rule );
        ADD_ATTR( "fx",                           attr_fx );
        ADD_ATTR( "fy",                           attr_fy );
        ADD_ATTR( "gradientTransform",            attr_gradientTransform );
        ADD_ATTR( "gradientUnits",                attr_gradientUnits );
        ADD_ATTR( "height",                       attr_height );
        ADD_ATTR( "id",                           attr_id );
        ADD_ATTR( "offset",                       attr_offset );
        ADD_ATTR( "opacity",                      attr_opacity );
        ADD_ATTR( "points",                       attr_points );
        ADD_ATTR( "preserveAspectRatio",          attr_preserveAspectRatio );
        ADD_ATTR( "r",                            attr_r );
        ADD_ATTR( "rx",                           attr_rx );
        ADD_ATTR( "ry",                           attr_ry );
        ADD_ATTR( "spreadMethod",                 attr_spreadMethod );
        ADD_ATTR( "stop-color",                   attr_stop_color );
        ADD_ATTR( "stop-opacity",                 attr_stop_opacity );
        ADD_ATTR( "stroke",                       attr_stroke );
        ADD_ATTR( "stroke-linecap",               attr_stroke_linecap );
        ADD_ATTR( "stroke-linejoin",              attr_stroke_linejoin );
        ADD_ATTR( "stroke-miterlimit",            attr_stroke_miterlimit );
        ADD_ATTR( "stroke-opacity",               attr_stroke_opacity );
        ADD_ATTR( "stroke-width",                 attr_stroke_width );
        ADD_ATTR( "style",                        attr_style );
        ADD_ATTR( "transform",                    attr_transform );
        ADD_ATTR( "viewBox",                      attr_viewBox );
        ADD_ATTR( "width",                        attr_width );
        ADD_ATTR( "x",                            attr_x );
        ADD_ATTR( "x1",                           attr_x1 );
        ADD_ATTR( "x2",                           attr_x2 );
        ADD_ATTR( "xlink:href",                   attr_xlink_href );
        ADD_ATTR( "y",                            attr_y );
        ADD_ATTR( "y1",                           attr_y1 );
        ADD_ATTR( "y2",                           attr_y2 );
        
//--------------------------------------
        std::sort(m_map.begin(), 
                  m_map.end(),
                  make_comparer
                  (
                    &map_type::value_type::first, 
                    string_comparer()
                  )
                 );

        map_type::iterator it  = m_map.begin();
        map_type::iterator end = m_map.end(); 
        for (unsigned i = 0; it != end; ++it)
            m_names_positions[it->second] = i++;
    }
#undef ADD_ATTR

    //-------------------------------------------------------------------------
    attr_e attributes_map::get_type(const char * name) 
    {
        typedef std::pair<const char*, attr_e>* iterator;
        std::pair<map_type::iterator,map_type::iterator> range;

        range = std::equal_range(m_map.begin(), m_map.end(), name, 
                                 make_comparer
                                 (
                                 &std::pair<const char*, attr_e>::first, 
                                   string_comparer()
                                 ));

        return range.first != range.second ? range.first->second : end_of_attr;
    }
    //-------------------------------------------------------------------------
    const char * attributes_map::get_name(unsigned attr) 
    {
        assert(attr >= 0);
        assert(attr < end_of_attr);

        return m_map[m_names_positions[attr]].first;
    }

} // namespace svg
} // namespace agg
