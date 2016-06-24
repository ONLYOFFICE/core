#ifndef AGG_SVG_RENDERER_RGBA_INCLUDED
#define AGG_SVG_RENDERER_RGBA_INCLUDED
//-----------------------------------------------------------------------------
#include "agg_svg_attributes.h"
#include "agg_svg_rasterizer.h"
#include "agg_svg_frame_buffer_rgba.h"
#include "agg_span_interpolator_linear.h"
#include "agg_span_gradient.h"
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    //-------------------------------------------------------------------------
	class gradient_opacity_adaptor;
    //-------------------------------------------------------------------------
    // Main rgba renderer
    //
    // Used to incapsulate AGG rendering functionality 
    //      
	//-------------------------------------------------------------------------
    class renderer_rgba
    {
    public:
		//---------------------------------------------------------------------
        typedef frame_buffer_rgba                      frame_buffer_type;
        typedef frame_buffer_type::pixfmt_type         pixfmt_type;
        typedef frame_buffer_type::renderer_base_type  renderer_base_type;
        typedef frame_buffer_type::renderer_solid_type renderer_solid_type;
		//---------------------------------------------------------------------
        enum { pixel_size = sizeof(pixel_type) };
		//---------------------------------------------------------------------
        renderer_rgba(rasterizer&, frame_buffer_type&);
		//---------------------------------------------------------------------
        void attach(frame_buffer_type&);
		//---------------------------------------------------------------------
        void fill_gradient(const gradient&);
        void stroke_gradient(const gradient&);
		//---------------------------------------------------------------------

        void reset();
        void clip_box(double x1, double y1, double x2, double y2);
        void add_vertex(double x, double y, unsigned cmd);

		//---------------------------------------------------------------------

        void fill(const attributes& attr);
        void stroke(const attributes& attr);

        //---------------------------------------------------------------------
		bool is_valid()const;
        //---------------------------------------------------------------------
    private:
		//---------------------------------------------------------------------
        renderer_rgba(const renderer_rgba&);
        const renderer_rgba& operator = (const renderer_rgba&);
		//---------------------------------------------------------------------
        void render_gradient(const attributes&, 
                             const gradient&, 
                             const gradient_lut_type&, 
                             double opacity);
		//---------------------------------------------------------------------
        template <class GradientFunc, class InterpolatorType>
        void render_gradient_func(const GradientFunc&             gradient_func, 
                                        InterpolatorType&         span_interpolator,
                                  const gradient_opacity_adaptor& gradient_colors);
		//---------------------------------------------------------------------
        rasterizer&         m_rasterizer;
        frame_buffer_type*  m_frame_buffer;
        const gradient*     m_fill_gradient;
        const gradient*     m_stroke_gradient;
    };
	//-------------------------------------------------------------------------
	inline void renderer_rgba::attach(frame_buffer_type& fb)      
	{ 
		m_frame_buffer = &fb;   
	}
	//-------------------------------------------------------------------------
	inline void renderer_rgba::fill_gradient(const gradient& g)   
	{ 
		m_fill_gradient = &g;   
	}
	//-------------------------------------------------------------------------
	inline void renderer_rgba::stroke_gradient(const gradient& g) 
	{ 
		m_stroke_gradient = &g; 
	}
	//-------------------------------------------------------------------------
    inline void renderer_rgba::reset()
    {
        m_rasterizer.reset();
    }
	//---------------------------------------------------------------------
    inline void renderer_rgba::clip_box(double x1, double y1, double x2, double y2)
    {
        m_rasterizer.clip_box(x1, y1, x2, y2);
    }
	//---------------------------------------------------------------------
    inline void renderer_rgba::add_vertex(double x, double y, unsigned cmd)
    {
        m_rasterizer.add_vertex(x, y, cmd);
    }
	//---------------------------------------------------------------------
	inline bool renderer_rgba::is_valid()const 
	{
		return m_frame_buffer->is_valid(); 
	}
	//---------------------------------------------------------------------

} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_RENDERER_RGBA_INCLUDED

