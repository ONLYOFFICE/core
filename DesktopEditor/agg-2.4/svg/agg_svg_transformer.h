#ifndef AGG_SVG_TRANSFORMER_INCLUDED
#define AGG_SVG_TRANSFORMER_INCLUDED

#include "agg_trans_affine.h"
#include "agg_trans_viewport.h"

#include "agg_svg_basics.h"
#include "agg_svg_defines.h"



namespace agg
{
namespace svg
{
    //-------------------------------------------------------------------------
    // The transformer class integrates the viewport, additional zoom, and
    // arbitary affine transformations. 
    //
    // USAGE:
    //      transformer tr;
    //      tr.viewBox(. . .);
    //      . . .
    //      tr.transform(&x, &y);
    //    
    //----------------------------------------------------------------------------
    class transformer
    {
    public:
        //-------------------------------------------------------------
        transformer() : 
            m_affine(),
            m_viewport(),
            m_zoom(),
            m_transformer(),
            m_viewBox_level(0),
            m_scale(1.0), 
            m_dx(0),
            m_dy(0),
            m_device_x1(0),
            m_device_y1(0),
            m_device_x2(1),
            m_device_y2(1),
            m_window_x1(0),
            m_window_y1(0),
            m_window_x2(-1),
            m_window_y2(-1),
            m_viewbox_x1(0),
            m_viewbox_y1(0),
            m_viewbox_x2(-1),
            m_viewbox_y2(-1),
            m_viewbox_dx(0),
            m_viewbox_dy(0),
            m_buffer_x1(0),
            m_buffer_y1(0),
            m_buffer_x2(-1),
            m_buffer_y2(-1),
            m_clip_x1(0),
            m_clip_y1(0),
            m_clip_x2(-1),
            m_clip_y2(-1)
        {
            m_viewport.preserve_aspect_ratio(0.5, 0.5, agg::aspect_ratio_meet);
            m_zoom.preserve_aspect_ratio(0.5, 0.5, agg::aspect_ratio_meet);
        }

        //-------------------------------------------------------------
        void reset()
        {
            *this = transformer();
        }

        //-------------------------------------------------------------
        void window(int x1, int y1, int x2, int y2)
        {
            m_viewBox_level = 0;
            m_dx = 0;
            m_dy = 0;
            m_window_x1  = x1;
            m_window_y1  = y1;
            m_window_x2  = x2;
            m_window_y2  = y2;
            m_viewbox_x1 = x1;
            m_viewbox_y1 = y1;
            m_viewbox_x2 = x2;
            m_viewbox_y2 = y2;
            m_viewbox_dx = 0;
            m_viewbox_dy = 0;
            m_buffer_x1  = x1;
            m_buffer_y1  = y1;
            m_buffer_x2  = x2;
            m_buffer_y2  = y2;
            m_clip_x1    = x1;
            m_clip_y1    = y1;
            m_clip_x2    = x2;
            m_clip_y2    = y2;
            m_viewport.device_viewport(x1, y1, x2, y2);
            m_viewport.world_viewport(x1, y1, x2, y2);
            m_zoom.device_viewport(x1, y1, x2, y2);
            m_zoom.world_viewport(x1, y1, x2, y2);
            update();
        }

        //-------------------------------------------------------------
        void viewBox(const agg::trans_viewport& vp);

        void viewBox(double scx, double scy, double scw, double sch,
                     double vbx, double vby, double vbw, double vbh, 
                     uniform_scaling_e align_type,
                     window_fit_logic_e meet_or_slice,
                     bool separate_window);

        //-------------------------------------------------------------
        unsigned viewBox_level() const { return m_viewBox_level; }

        //-------------------------------------------------------------
        bool is_visible(double x1, double y1, double x2, double y2) const
        {
            agg::rect_d cb(m_clip_x1, m_clip_y1, m_clip_x2, m_clip_y2);
            return cb.clip(agg::rect_d(x1, y1, x2, y2));
        }


        //-------------------------------------------------------------
        void zoom(double wx1, double wy1, double wx2, double wy2,
                  double sx1, double sy1, double sx2, double sy2)
        {
            m_zoom.device_viewport(sx1, sy1, sx2, sy2);
            m_zoom.world_viewport(wx1, wy1, wx2, wy2);
            update();
        }

        //-------------------------------------------------------------
        void zoom(double wx1, double wy1, double wx2, double wy2)
        {
            m_zoom.world_viewport(wx1, wy1, wx2, wy2);
            update();
        }

        //-------------------------------------------------------------
        void zoom(double* x1, double* y1, double* x2, double* y2) const
        {
            m_zoom.transform(x1, y1);
            m_zoom.transform(x2, y2);
        }

        //-------------------------------------------------------------
        void inverse_zoom(double* x1, double* y1, double* x2, double* y2) const
        {
            m_zoom.inverse_transform(x1, y1);
            m_zoom.inverse_transform(x2, y2);
        }

        //-------------------------------------------------------------
        void device_viewport(double* x1, double* y1, double* x2, double* y2) const
        {
            m_viewport.device_viewport(x1, y1, x2, y2);
        }

        //-------------------------------------------------------------
        void world_viewport(double* x1, double* y1, double* x2, double* y2) const
        {
            m_viewport.world_viewport(x1, y1, x2, y2);
        }

        //-------------------------------------------------------------
        const agg::trans_viewport& viewport() const { return m_viewport; }
              agg::trans_viewport& viewport()       { return m_viewport; }

        //-------------------------------------------------------------
        const agg::trans_viewport& zoom() const { return m_zoom; }
              agg::trans_viewport& zoom()       { return m_zoom; }

        //-------------------------------------------------------------
        void transform(const agg::trans_affine& mtx)
        {
            m_affine.premultiply(mtx);
            update();
        }

        //-------------------------------------------------------------
        void transform(double a0, double a1, double a2,
                       double a3, double a4, double a5)
        {
            m_affine.premultiply(agg::trans_affine(a0, a1, a2, a3, a4, a5));
            update();
        }

        //-------------------------------------------------------------
        void translate(double dx, double dy)
        {
            m_affine.premultiply(agg::trans_affine_translation(dx, dy));
            update();
        }

        //-------------------------------------------------------------
        void rotate(double angle)
        {
            m_affine.premultiply(agg::trans_affine_rotation(angle));
            update();
        }

        //-------------------------------------------------------------
        void rotate(double angle, double cx, double cy)
        {
            agg::trans_affine_translation m(-cx, -cy);
            m *= agg::trans_affine_rotation(angle);
            m *= agg::trans_affine_translation(cx, cy);
            m_affine.premultiply(m);
            update();
        }

        //-------------------------------------------------------------
        void scale(double s)
        {
            m_affine.premultiply(agg::trans_affine_scaling(s));
            update();
        }

        //-------------------------------------------------------------
        void scale(double sx, double sy)
        {
            m_affine.premultiply(agg::trans_affine_scaling(sx, sy));
            update();
        }

        //-------------------------------------------------------------
        void skew(double sx, double sy)
        {
            m_affine.premultiply(agg::trans_affine_skewing(sx, sy));
            update();
        }

        //-------------------------------------------------------------
        void skew_x(double s)
        {
            m_affine.premultiply(agg::trans_affine_skewing(s, 0.0));
            update();
        }

        //-------------------------------------------------------------
        void skew_y(double s)
        {
            m_affine.premultiply(agg::trans_affine_skewing(0.0, s));
            update();
        }

        //-------------------------------------------------------------
        unsigned byte_size_affine() const
        {
            return 6 * sizeof(double);
        }

        //-------------------------------------------------------------
        void serialize_affine(agg::int8u* ptr) const
        {
            double mtx[6];
            m_affine.store_to(mtx);
            memcpy(ptr, mtx, sizeof(mtx));
        }

        //-------------------------------------------------------------
        void deserialize_affine(const agg::int8u* ptr)
        {
            double mtx[6];
            memcpy(mtx, ptr, sizeof(mtx));
            m_affine.load_from(mtx);
            update();
        }

        //-------------------------------------------------------------
        void transform(double* x, double* y) const
        {
            m_transformer.transform(x, y);
        }

        //-------------------------------------------------------------
        void inverse_transform(double* x, double* y) const
        {
            m_transformer.inverse_transform(x, y);
        }
        
        //-------------------------------------------------------------
        bool is_valid() const
        {
            return m_viewport.is_valid() && 
                   m_zoom.is_valid() &&
                   m_buffer_x2 > m_buffer_x1 && 
                   m_buffer_y2 > m_buffer_y1;
        }

        //-------------------------------------------------------------
        double scale() const 
        { 
            return m_scale; 
        }

        //-------------------------------------------------------------
        const agg::trans_affine& transform() const
        {
            return m_transformer;
        }

        //-------------------------------------------------------------------
        double device_x1() const { return m_device_x1; }
        double device_y1() const { return m_device_y1; }
        double device_x2() const { return m_device_x2; }
        double device_y2() const { return m_device_y2; }

        int    window_x1() const { return m_window_x1; }
        int    window_y1() const { return m_window_y1; }
        int    window_x2() const { return m_window_x2; }
        int    window_y2() const { return m_window_y2; }

        double clip_x1()   const { return m_clip_x1; }
        double clip_y1()   const { return m_clip_y1; }
        double clip_x2()   const { return m_clip_x2; }
        double clip_y2()   const { return m_clip_y2; }

        int    buffer_x1() const { return m_buffer_x1; }
        int    buffer_y1() const { return m_buffer_y1; }
        int    buffer_x2() const { return m_buffer_x2; }
        int    buffer_y2() const { return m_buffer_y2; }

    private:
        void update();
        void invalidate();

        agg::trans_affine   m_affine;
        agg::trans_viewport m_viewport;
        agg::trans_viewport m_zoom;
        agg::trans_affine   m_transformer;
        unsigned            m_viewBox_level;
        double              m_scale;
        double              m_dx;
        double              m_dy;
        double              m_device_x1;
        double              m_device_y1;
        double              m_device_x2;
        double              m_device_y2;
        int                 m_window_x1;
        int                 m_window_y1;
        int                 m_window_x2;
        int                 m_window_y2;
        double              m_viewbox_x1;
        double              m_viewbox_y1;
        double              m_viewbox_x2;
        double              m_viewbox_y2;
        double              m_viewbox_dx;
        double              m_viewbox_dy;
        double              m_clip_x1;
        double              m_clip_y1;
        double              m_clip_x2;
        double              m_clip_y2;
        int                 m_buffer_x1;
        int                 m_buffer_y1;
        int                 m_buffer_x2;
        int                 m_buffer_y2;
    };
}
}

#endif

