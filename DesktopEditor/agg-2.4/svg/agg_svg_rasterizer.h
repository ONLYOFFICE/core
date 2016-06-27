#ifndef AGG_SVG_RASTERIZER_INCLUDED
#define AGG_SVG_RASTERIZER_INCLUDED

#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"
#include "agg_span_allocator.h"
#include "agg_svg_attributes.h" // for g_fill_rule_lut

namespace agg
{
namespace svg
{
    //-------------------------------------------------------------------------
    //
    // This class incapsulates AGG scanline rasterizer, AGG scanline container
    // and RGBA span allocators. 
    //
    //---------------------------------------------------------------------------
    class rasterizer
    {
    public:
        typedef agg::span_allocator<color_type> span_allocator_rgba_type;
		typedef agg::rasterizer_scanline_aa<> rasterizer_type;

        rasterizer() 
			:	m_scanline()
			,	m_rasterizer()
			,	m_allocator_rgba()
        {}
        //-----------------------------------------------------------------------
		void fill_rule(fill_rule_e fr)
		{
			m_rasterizer.filling_rule(g_fill_rule_lut[fr]);
		}

		void gamma(double g)
		{
			m_rasterizer.gamma(agg::gamma_power(g));
		}
		void gamma_multi(double g)
		{
			m_rasterizer.gamma(agg::gamma_multiply(g));
		}

		void clip_box(double x1, double y1, double x2, double y2)
		{
			m_rasterizer.clip_box(x1, y1, x2, y2);
		}

		void reset_clipping()
		{
			m_rasterizer.reset_clipping();
		}

        //-----------------------------------------------------------------------
        // Rasterizer Interface
        void reset()
        {
            m_rasterizer.reset();
        }

        void add_vertex(double x, double y, unsigned cmd)
        {
            m_rasterizer.add_vertex(x, y, cmd);
        }

        template<class Renderer> void render(Renderer& ren)
        {
            agg::render_scanlines(m_rasterizer, m_scanline, ren);
        }

        span_allocator_rgba_type&    allocator_rgba()  { return m_allocator_rgba; }
		rasterizer_type&             get_rasterizer()  { return m_rasterizer; }
		agg::scanline_u8&            get_scanline()    { return m_scanline; }

    private:
        agg::scanline_u8              m_scanline;
        rasterizer_type               m_rasterizer;
        span_allocator_rgba_type      m_allocator_rgba;
    };

} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_RASTERIZER_INCLUDED


