#include "agg_svg_renderer_rgba.h"    

namespace agg
{
namespace svg
{
	//-------------------------------------------------------------------------
    class gradient_opacity_adaptor
    {
    public:
		//---------------------------------------------------------------------
        gradient_opacity_adaptor(const gradient_lut_type& lut, double opacity) 
			:	m_lut(lut)
			,	m_opacity(unsigned(opacity * color_type::base_scale + 0.5))
        {}
		//---------------------------------------------------------------------
        static unsigned size() 
		{ 
			return gradient_lut_type::size(); 
		}
		//---------------------------------------------------------------------
        color_type operator[] (unsigned i) const
        {
            color_type c = m_lut[i];
            c.a = (color_type::value_type)((c.a * m_opacity) >> color_type::base_shift);
            return c;
        }
 		//---------------------------------------------------------------------
    private:
        const gradient_lut_type& m_lut;
        unsigned                 m_opacity;
    };
	//-------------------------------------------------------------------------
    renderer_rgba::renderer_rgba(rasterizer& ras, frame_buffer_type& fb) 
		:	m_rasterizer(ras)
		,	m_frame_buffer(&fb)
		,	m_fill_gradient(0)
		,	m_stroke_gradient(0)
	{}
	//-------------------------------------------------------------------------
	template<class GradientFunc, class InterpolatorType>
	void renderer_rgba::render_gradient_func(const GradientFunc&             gradient_func, 
											  	   InterpolatorType&         span_interpolator,
											 const gradient_opacity_adaptor& gradient_colors)
    {
		typedef agg::span_gradient
		<
			color_type, 
            InterpolatorType, 
            GradientFunc, 
            gradient_opacity_adaptor
		> 
		span_gradient_type;

		span_gradient_type span_gradient(span_interpolator, gradient_func, gradient_colors, 0, 100);

        agg::renderer_scanline_aa
		<
			renderer_base_type, 
			rasterizer::span_allocator_rgba_type, 
			span_gradient_type
		> 
        ren_gradient(m_frame_buffer->ren_base(), m_rasterizer.allocator_rgba(), span_gradient);
    
        m_rasterizer.render(ren_gradient);
    }
	//-------------------------------------------------------------------------
    void renderer_rgba::fill(const attributes& attr)
    {
        if (!m_frame_buffer->is_valid()) return;

        m_rasterizer.fill_rule(attr.fill_rule());

        switch (attr.fill_type())
        {
        case paint_none: 
            break;

        case paint_color:
        case paint_currentColor:
            m_frame_buffer->ren_solid().color(attr.fill_color());
            m_rasterizer.render(m_frame_buffer->ren_solid());
            break;

        case paint_gradient:
            render_gradient(attr, 
                            *m_fill_gradient, 
                            m_fill_gradient->lut(),
                            attr.fill_opacity());
            break;
        }
    }
    //-------------------------------------------------------------------------
    void renderer_rgba::stroke(const attributes& attr)
    {
        if(!m_frame_buffer->is_valid()) return;

        m_rasterizer.fill_rule(fill_rule_nonzero);

        switch (attr.stroke_type())
        {
        case paint_none: 
            break;

        case paint_color:
        case paint_currentColor:
            m_frame_buffer->ren_solid().color(attr.stroke_color());
            m_rasterizer.render(m_frame_buffer->ren_solid());
            break;

        case paint_gradient:
            render_gradient(attr, 
                            *m_stroke_gradient, 
                            m_stroke_gradient->lut(),
                            attr.stroke_opacity());
            break;
        }
    }
    //-------------------------------------------------------------------------
    void renderer_rgba::render_gradient(const attributes&        attr, 
                                        const gradient&          g, 
                                        const gradient_lut_type& lut, 
                                        double                   opacity)
    {
        enum gradient_type
        {
            linear_pad           = 0 | 0,
            linear_reflect       = 4 | 0,
            linear_repeat        = 8 | 0,
            radial_pad           = 0 | 1,
            radial_reflect       = 4 | 1,
            radial_repeat        = 8 | 1,
            radial_focus_pad     = 0 | 2,
            radial_focus_reflect = 4 | 2,
            radial_focus_repeat  = 8 | 2
        };

        // Deduce gradient type and calculate the transformation matrix
        //------------------
        agg::trans_affine mtx;
        int gradient_type = 0;

        double fx = 0;
        double fy = 0;

        if (g.type() == gradient_linear)
        {
            mtx = agg::trans_affine_line_segment(g.x1(), g.y1(), g.x2(), g.y2(), 100.0);
        }
        else
        {
            gradient_type = (g.fx() == g.cx() && g.fy() == g.cy()) ? 1 : 2;
            mtx  = agg::trans_affine_scaling(g.r() / 100.0);
            mtx *= agg::trans_affine_translation(g.cx(), g.cy());
            fx = g.fx();
            fy = g.fy();
            mtx.inverse_transform(&fx, &fy);
        }

        switch (g.spreadMethod())
        {
            default: break;
            case spreadMethod_reflect: gradient_type |= 4; break;
            case spreadMethod_repeat:  gradient_type |= 8; break;
        }

        if (g.gradientUnits() == objectUnits_objectBoundingBox)
        {
            double parl[6] = { 0,0, 1,0, 1,1 };
            double x1 = attr.object_x1();
            double y1 = attr.object_y1();
            double x2 = attr.object_x2();
            double y2 = attr.object_y2(); 

            if (y1 == y2)
                y2 += .1;
            if (x1 == x2)
                x2 += .1;

            mtx *= agg::trans_affine(parl, x1, y1, x2, y2);
        }

        mtx *= g.gradientTransform();
        mtx *= attr.transform().transform();
        mtx.invert();

        gradient_opacity_adaptor        gradient_colors(lut, opacity);
        agg::span_interpolator_linear<> span_interpolator(mtx);

        switch (gradient_type)
        {
            default: break;
            case linear_pad:
            {
                agg::gradient_x gradient_func;
                render_gradient_func(gradient_func, span_interpolator, gradient_colors);
            }
            break;

            case linear_reflect:
            {
                agg::gradient_x gradient_func;
                agg::gradient_reflect_adaptor<agg::gradient_x> gradient_adaptor(gradient_func);
                render_gradient_func(gradient_adaptor, span_interpolator, gradient_colors);
            }
            break;

            case linear_repeat:
            {
                agg::gradient_x gradient_func;
                agg::gradient_repeat_adaptor<agg::gradient_x> gradient_adaptor(gradient_func);
                render_gradient_func(gradient_adaptor, span_interpolator, gradient_colors);
            }
            break;

            case radial_pad:
            {
                agg::gradient_radial gradient_func;
                render_gradient_func(gradient_func, span_interpolator, gradient_colors);
            }
            break;

            case radial_reflect:
            {
                agg::gradient_radial gradient_func;
                agg::gradient_reflect_adaptor<agg::gradient_radial> gradient_adaptor(gradient_func);
                render_gradient_func(gradient_adaptor, span_interpolator, gradient_colors);
            }
            break;

            case radial_repeat:
            {
                agg::gradient_radial gradient_func;
                agg::gradient_repeat_adaptor<agg::gradient_radial> gradient_adaptor(gradient_func);
                render_gradient_func(gradient_adaptor, span_interpolator, gradient_colors);
            }
            break;

            case radial_focus_pad:
            {
                agg::gradient_radial_focus gradient_func(100.0, fx, fy);
                render_gradient_func(gradient_func, span_interpolator, gradient_colors);
            }
            break;

            case radial_focus_reflect:
            {
                agg::gradient_radial_focus gradient_func(100.0, fx, fy);
                agg::gradient_reflect_adaptor<agg::gradient_radial_focus> gradient_adaptor(gradient_func);
                render_gradient_func(gradient_adaptor, span_interpolator, gradient_colors);
            }
            break;

            case radial_focus_repeat:
            {
                agg::gradient_radial_focus gradient_func(100.0, fx, fy);
                agg::gradient_repeat_adaptor<agg::gradient_radial_focus> gradient_adaptor(gradient_func);
                render_gradient_func(gradient_adaptor, span_interpolator, gradient_colors);
            }
            break;
        }
    }

} // namespace svg
} // namespace agg