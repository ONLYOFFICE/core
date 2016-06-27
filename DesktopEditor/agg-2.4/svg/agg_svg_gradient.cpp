//-----------------------------------------------------------------------------
#include "agg_svg_gradient.h"
#include "agg_svg_attributes.h"
#include "agg_dda_line.h"
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{

    //-------------------------------------------------------------------------
    class color_interpolator_rgba
    {
    public:
        color_interpolator_rgba(const color_type& c1, 
                                const color_type& c2, 
                                unsigned len) :
            r(c1.r, c2.r, len),
            g(c1.g, c2.g, len),
            b(c1.b, c2.b, len),
            a(c1.a, c2.a, len)
        {}

        void operator ++ ()
        {
            ++r; ++g; ++b; ++a;
        }
        color_type color() const
        {
            return color_type(r.y(), g.y(), b.y(), a.y());
        }

    private:
        agg::dda_line_interpolator<14> r, g, b, a;
    };
    //-------------------------------------------------------------------------
    void gradient::create_gradient_lut(attributes& attr, gradient_lut_cache& cache)
    {
        agg::quick_sort(m_colors, offset_less);
        m_colors.cut_at(agg::remove_duplicates(m_colors, offset_equal));

        m_lut = cache.get_lut(m_colors);
        if (m_lut == 0)
        {
            if(m_colors.size() >= 2)
            {
                gradient_lut_type lut;
                unsigned i;

                unsigned start = unsigned(m_colors[0].offset * lut.size() + 0.5);
                unsigned end;
                color_type c1 = attr.gamma_color(m_colors[0].stop_color);
                color_type c2;
                c1.a = (color_type::value_type)(c1.a * m_colors[0].stop_opacity + 0.5);
                for(i = 0; i < start; i++) 
                {
                    lut[i] = c1;
                }
                for(i = 1; i < m_colors.size(); i++)
                {
                    end  = unsigned(m_colors[i].offset * lut.size() + 0.5);
                    c1   = attr.gamma_color(m_colors[i-1].stop_color);
                    c1.a = (color_type::value_type)(c1.a * m_colors[i-1].stop_opacity + 0.5);
                    c2   = attr.gamma_color(m_colors[i].stop_color);
                    c2.a = (color_type::value_type)(c2.a * m_colors[i].stop_opacity + 0.5);
                    color_interpolator_rgba ci(c1, c2, end - start + 1);
                    while(start < end)
                    {
                        lut[start] = ci.color();
                        ++ci;
                        ++start;
                    }
                }
                c2 = attr.gamma_color(m_colors.last().stop_color);
                c2.a = (color_type::value_type)(c2.a * m_colors.last().stop_opacity + 0.5);
                for(; end < lut.size(); end++)
                {
                    lut[end] = c2;
                }
                m_lut = cache.add(m_colors, lut);
            }
        }
    }

}
}