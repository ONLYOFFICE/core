#ifndef AGG_SVG_FRAME_BUFFER_RGBA_INCLUDED
#define AGG_SVG_FRAME_BUFFER_RGBA_INCLUDED

#include "agg_rendering_buffer.h"
#include "agg_pixfmt_rgba.h"
#include "agg_renderer_base.h"
#include "agg_renderer_scanline.h"

namespace agg
{
namespace svg
{
    //---------------------------------------------------------------------------
    class frame_buffer_rgba
    {
    public:
        typedef agg::blender_rgba<color_type, component_order>                                blender_type;
        typedef agg::pixfmt_alpha_blend_rgba<blender_type, agg::rendering_buffer, pixel_type> pixfmt_type;
        typedef agg::renderer_base<pixfmt_type>                                               renderer_base_type;
        typedef agg::renderer_scanline_aa_solid<renderer_base_type>                           renderer_solid_type;

        enum { pix_size = sizeof(pixel_type) };

        //-----------------------------------------------------------------------
        ~frame_buffer_rgba() 
		{
			if (!m_buf_was_attach)
				delete [] m_internal_buf; 
		}
        //-----------------------------------------------------------------------
        frame_buffer_rgba() 
			:	m_internal_buf(0)
			,	m_is_valid(false)
			,	m_ren_buf()
			,	m_pixfmt()
			,	m_ren_base()
			,	m_ren_solid()
			,   m_buf_was_attach(false)
        {}
        //-----------------------------------------------------------------------
        void create(unsigned width, unsigned height, bool flip, int& stride, void* buff=NULL)
        {
			if (!m_buf_was_attach)
				delete [] m_internal_buf;

            if (!buff)
			{
				stride = flip ? -int(width * pix_size) : int(width * pix_size);
				m_internal_buf = new agg::int8u[width * height * pix_size];
				m_buf_was_attach = false;
			}
			else
			{
				m_internal_buf = (agg::int8u*)buff;
				m_buf_was_attach = true;
			}

            m_ren_buf.attach(m_internal_buf, width, height, stride);

            m_pixfmt.attach(m_ren_buf);
            m_ren_base.attach(m_pixfmt);
            m_ren_solid.attach(m_ren_base);

            m_is_valid = true;
        }

        //-----------------------------------------------------------------------
        void clear()             { m_ren_buf.clear(0); }
        void clear(color_type c) { m_ren_base.clear(c); }

        //-----------------------------------------------------------------------
        const agg::rendering_buffer& ren_buf() const { return m_ren_buf; }
              agg::rendering_buffer& ren_buf()       { return m_ren_buf; }

		bool     is_valid() const { return m_is_valid;         }
		unsigned width()    const { return m_ren_buf.width();  }
        unsigned height()   const { return m_ren_buf.height(); }

        const pixfmt_type&         pixfmt()   const { return m_pixfmt;    }
              pixfmt_type&         pixfmt()         { return m_pixfmt;    }

        const renderer_base_type&  ren_base() const { return m_ren_base;  }
              renderer_base_type&  ren_base()       { return m_ren_base;  }

        const renderer_solid_type& ren_solid()const { return m_ren_solid; }
              renderer_solid_type& ren_solid()      { return m_ren_solid; }

	     const agg::int8u*          get_internal_buf() const {return m_internal_buf;}

    private:
        frame_buffer_rgba(const frame_buffer_rgba&);
        const frame_buffer_rgba& operator = (const frame_buffer_rgba&);

        agg::int8u*            m_internal_buf;
        bool                   m_is_valid;
        agg::rendering_buffer  m_ren_buf;
        pixfmt_type            m_pixfmt;
        renderer_base_type     m_ren_base;
        renderer_solid_type    m_ren_solid;

		bool                   m_buf_was_attach;
    };

} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_FRAME_BUFFER_RGBA_INCLUDED
