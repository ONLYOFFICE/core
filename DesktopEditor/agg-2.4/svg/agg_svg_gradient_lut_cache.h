//-----------------------------------------------------------------------------
#ifndef AGG_SVG_GRADIENT_LUT_CACHE_INCLUDED
#define AGG_SVG_GRADIENT_LUT_CACHE_INCLUDED
//-----------------------------------------------------------------------------
#include "agg_array.h"
#include "agg_svg_basics.h"
#include "agg_svg_gradient.h"
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    class gradient_lut_cache
    {
        typedef gradient::stop_array_type             stop_array_type;
        typedef agg::pod_bvector<gradient_lut_type,8> cache_type;
        typedef agg::pod_bvector<stop_array_type>     stop_array_key_type;
        
        cache_type           m_cache;
        stop_array_key_type  m_colors_key;

        static bool color_equal(const color_type& lhs,
                                const color_type& rhs)
        {
            return lhs.r == rhs.r &&
                   lhs.g == rhs.g &&
                   lhs.b == rhs.b &&
                   lhs.a == rhs.a;
        }

        static bool stop_equal(const gradient::stop& lhs, 
                               const gradient::stop& rhs)
        {
            return lhs.offset       == rhs.offset       && 
                   lhs.stop_opacity == rhs.stop_opacity &&
                   color_equal(lhs.stop_color, rhs.stop_color);
        }

        static bool stop_array_equal(const stop_array_type& lhs,
                                     const stop_array_type& rhs)
        {
            if (lhs.size() != rhs.size())
                return false;
            
            for (unsigned i = 0u, size = lhs.size(); i < size; ++i)
            {
                if (!stop_equal(lhs[i], rhs[i]))
                    return false;
            }
            return true;
        }

        unsigned find_lut_index(const stop_array_type& colors)
        {
            for (unsigned i = 0u, size = m_colors_key.size(); i < size; ++i)
                if (stop_array_equal(m_colors_key[i], colors))
                    return i + 1;

            return 0;
        }

    public:
        gradient_lut_type* get_lut(const stop_array_type& colors) 
        {
            unsigned idx = find_lut_index(colors);
            return idx ? &m_cache[idx - 1] : 0;
        }

        gradient_lut_type* add(const stop_array_type& colors, const gradient_lut_type& lut)
        {
            m_cache.add(lut);
            m_colors_key.add(colors);

            return &m_cache.last();
        }
    };

} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_GRADIENT_LUT_CACHE_INCLUDED
