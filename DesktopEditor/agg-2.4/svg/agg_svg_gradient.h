//-----------------------------------------------------------------------------
#ifndef AGG_SVG_GRADIENT_INCLUDED
#define AGG_SVG_GRADIENT_INCLUDED
//-----------------------------------------------------------------------------
#include "agg_array.h"
#include "agg_trans_affine.h"
#include "agg_svg_basics.h"
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    class attributes;
    class gradient_lut_cache;
    //-------------------------------------------------------------------------
    class gradient
    {
    public:
        //---------------------------------------------------------------------
        struct stop
        {
            double     offset;
            color_type stop_color;
            double     stop_opacity;

            stop() {}
            stop(double off, const color_type& c, double op) : 
                offset(off), stop_color(c), stop_opacity(op) 
            {
                if(offset       < 0.0) offset       = 0.0;
                if(offset       > 1.0) offset       = 1.0;
                if(stop_opacity < 0.0) stop_opacity = 0.0;
                if(stop_opacity > 1.0) stop_opacity = 1.0;
            }
        };
        typedef agg::pod_bvector<stop, 4> stop_array_type;

        static bool offset_less(const stop& a, const stop& b)
        {
            return a.offset < b.offset;
        }
        static bool offset_equal(const stop& a, const stop& b)
        {
            return a.offset == b.offset;
        }

        //-----------------------------------------------------------------------
        gradient() :
            m_type(gradient_linear),
            m_gradientUnits(objectUnits_objectBoundingBox),
            m_spreadMethod(spreadMethod_pad),
            m_gradientTransform(),
            m_x1(0), m_y1(0), m_x2(1), m_y2(0),
            m_cx(0.5), m_cy(0.5), m_r(0.5), m_fx(0.5), m_fy(0.5), m_lut(0)
        {}
        //-----------------------------------------------------------------------
        void reset()
        { 
            m_type = gradient_linear;
            m_gradientUnits = objectUnits_objectBoundingBox;
            m_spreadMethod = spreadMethod_pad;
            m_gradientTransform.reset();
            m_x1 = 0; m_y1 = 0; m_x2 = 1; m_y2 = 0;
            m_cx = 0.5; m_cy = 0.5; m_r = 0.5; m_fx = 0.5; m_fy = 0.5;
            m_colors.remove_all(); 
            m_lut = 0;
        }

        //-----------------------------------------------------------------------
        void type(gradient_e v)             { m_type = v; }
        void gradientUnits(objectUnits_e v) { m_gradientUnits = v; }
        void spreadMethod(spreadMethod_e v) { m_spreadMethod = v; }

        void linear(double x1, double y1, double x2, double y2) 
        { 
            m_type = gradient_linear;
            m_x1 = x1; m_y1 = y1; 
            m_x2 = x2; m_y2 = y2; 
        }

        void radial(double cx, double cy, double r) 
        { 
            m_type = gradient_radial;
            m_cx = m_fx = cx; 
            m_cy = m_fy = cy; 
            m_r = r;
        }

        void focus(double fx, double fy)  
        { 
            m_fx = fx; 
            m_fy = fy; 
        }

        void x1(double v) { m_x1 = v; }
        void y1(double v) { m_y1 = v; }
        void x2(double v) { m_x2 = v; }
        void y2(double v) { m_y2 = v; }
        void cx(double v) { m_cx = v; }
        void cy(double v) { m_cy = v; }
        void  r(double v) { m_r  = v; }
        void fx(double v) { m_fx = v; }
        void fy(double v) { m_fy = v; }

        void remove_all_stops()
        {
            m_colors.remove_all();
        }

        void add_stop(double offset, const color_type& color, double opacity)
        {
            m_colors.add(stop(offset, color, opacity));
        }

        void create_gradient_lut(attributes& attr, gradient_lut_cache&);

        //-----------------------------------------------------------------------
        void transform(double a0, double a1, double a2,
                       double a3, double a4, double a5)
        {
            m_gradientTransform.premultiply(agg::trans_affine(a0, a1, a2, a3, a4, a5));
        }

        void translate(double dx, double dy)
        {
            m_gradientTransform.premultiply(agg::trans_affine_translation(dx, dy));
        }

        void rotate(double angle)
        {
            m_gradientTransform.premultiply(agg::trans_affine_rotation(angle));
        }

        void rotate(double angle, double cx, double cy)
        {
            agg::trans_affine_translation m(-cx, -cy);
            m *= agg::trans_affine_rotation(angle);
            m *= agg::trans_affine_translation(cx, cy);
            m_gradientTransform.premultiply(m);
        }

        void scale(double s)
        {
            m_gradientTransform.premultiply(agg::trans_affine_scaling(s));
        }

        void scale(double sx, double sy)
        {
            m_gradientTransform.premultiply(agg::trans_affine_scaling(sx, sy));
        }

        void skew(double sx, double sy)
        {
            m_gradientTransform.premultiply(agg::trans_affine_skewing(sx, sy));
        }

        void skew_x(double s)
        {
            m_gradientTransform.premultiply(agg::trans_affine_skewing(s, 0.0));
        }

        void skew_y(double s)
        {
            m_gradientTransform.premultiply(agg::trans_affine_skewing(0.0, s));
        }

        // Accessors
        //-----------------------------------------------------------------------
        gradient_e                            type() const { return  m_type; }
        objectUnits_e                gradientUnits() const { return  m_gradientUnits; }
        spreadMethod_e                spreadMethod() const { return  m_spreadMethod; }
              agg::trans_affine& gradientTransform()       { return  m_gradientTransform; }
        const agg::trans_affine& gradientTransform() const { return  m_gradientTransform; }
        double                                  x1() const { return  m_x1; }
        double                                  y1() const { return  m_y1; }
        double                                  x2() const { return  m_x2; }
        double                                  y2() const { return  m_y2; }
        double                                  cx() const { return  m_cx; }
        double                                  cy() const { return  m_cy; }
        double                                   r() const { return  m_r; }
        double                                  fx() const { return  m_fx; }
        double                                  fy() const { return  m_fy; }
        const gradient_lut_type&               lut() const { return *m_lut; }

    private:
        gradient(const gradient&);
        const gradient& operator = (const gradient&);

        //-----------------------------------------------------------------------
        gradient_e        m_type;
        objectUnits_e     m_gradientUnits;
        spreadMethod_e    m_spreadMethod;
        agg::trans_affine m_gradientTransform;
        double            m_x1, m_y1, m_x2, m_y2;
        double            m_cx, m_cy, m_r, m_fx, m_fy;
        stop_array_type   m_colors;
        gradient_lut_type* m_lut;
    };

} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_GRADIENT_INCLUDED
