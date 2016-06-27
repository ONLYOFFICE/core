#include "agg_svg_transformer.h"


namespace agg
{
namespace svg
{


    //-------------------------------------------------------------------------
    void viewbox_to_viewport(agg::trans_viewport& tr,
                             double x, double y, double w, double h,
                             uniform_scaling_e align_type,
                             window_fit_logic_e meet_or_slice)
    {
        if(w > 0.0 && h > 0.0)
        {
            tr.world_viewport(x, y, x + w, y + h);
        }

        agg::aspect_ratio_e ar =
            (meet_or_slice == window_meet) ? agg::aspect_ratio_meet :
                                             agg::aspect_ratio_slice;
        switch(align_type)
        {
            case usc_none:     tr.preserve_aspect_ratio(0.0, 0.0, agg::aspect_ratio_stretch); break;
            case usc_xMinYMin: tr.preserve_aspect_ratio(0.0, 0.0, ar); break;
            case usc_xMidYMin: tr.preserve_aspect_ratio(0.5, 0.0, ar); break;
            case usc_xMaxYMin: tr.preserve_aspect_ratio(1.0, 0.0, ar); break;
            case usc_xMinYMid: tr.preserve_aspect_ratio(0.0, 0.5, ar); break;
            case usc_xMidYMid: tr.preserve_aspect_ratio(0.5, 0.5, ar); break;
            case usc_xMaxYMid: tr.preserve_aspect_ratio(1.0, 0.5, ar); break;
            case usc_xMinYMax: tr.preserve_aspect_ratio(0.0, 1.0, ar); break;
            case usc_xMidYMax: tr.preserve_aspect_ratio(0.5, 1.0, ar); break;
            case usc_xMaxYMax: tr.preserve_aspect_ratio(1.0, 1.0, ar); break;
        }
    }



    //-------------------------------------------------------------
    void transformer::update()
    {
        if(is_valid())
        {
            m_transformer = m_affine;
            m_transformer *= m_viewport.to_affine();
            m_transformer *= m_zoom.to_affine();
            m_transformer *= agg::trans_affine_translation(-m_dx, -m_dy);
            m_scale = m_transformer.scale();
            m_viewport.device_viewport(&m_device_x1, &m_device_y1, 
                                       &m_device_x2, &m_device_y2);
            m_clip_x1 = m_viewbox_x1;
            m_clip_y1 = m_viewbox_y1;
            m_clip_x2 = m_viewbox_x2;
            m_clip_y2 = m_viewbox_y2;
            double w = m_buffer_x2 - m_buffer_x1;
            double h = m_buffer_y2 - m_buffer_y1;
            if(m_clip_x1 < 0) m_clip_x1 = 0;
            if(m_clip_x2 > w) m_clip_x2 = w;
            if(m_clip_y1 < 0) m_clip_y1 = 0;
            if(m_clip_y2 > h) m_clip_y2 = h;
        }
        else
        {
            invalidate();
        }
    }

    //-------------------------------------------------------------
    void transformer::invalidate()
    {
        m_transformer = m_affine;
        m_scale = m_affine.scale(); 
        m_dx = 0;
        m_dy = 0;
        m_device_x1  = 0;
        m_device_y1  = 0;
        m_device_x2  = 1;
        m_device_y2  = 1;
        m_window_x1  = 0;
        m_window_y1  = 0;
        m_window_x2  = -1;
        m_window_y2  = -1;
        m_viewbox_x1 = 0;
        m_viewbox_y1 = 0;
        m_viewbox_x2 = -1;
        m_viewbox_y2 = -1;
        m_viewbox_dx = 0;
        m_viewbox_dy = 0;
        m_buffer_x1  = 0;
        m_buffer_y1  = 0;
        m_buffer_x2  = -1;
        m_buffer_y2  = -1;
        m_clip_x1    = 0;
        m_clip_y1    = 0;
        m_clip_x2    = -1;
        m_clip_y2    = -1;
    }

    //-------------------------------------------------------------
    void transformer::viewBox(const agg::trans_viewport& vp)
    {
        m_viewport = vp;
        m_viewport.device_viewport(&m_device_x1, &m_device_y1, 
                                   &m_device_x2, &m_device_y2);
        m_buffer_x1  = int(floor(m_device_x1));
        m_buffer_y1  = int(floor(m_device_y1));
        m_buffer_x2  = int( ceil(m_device_x2));
        m_buffer_y2  = int( ceil(m_device_y2));
        m_window_x1  = int(floor(m_device_x1));
        m_window_y1  = int(floor(m_device_y1));
        m_window_x2  = int( ceil(m_device_x2));
        m_window_y2  = int( ceil(m_device_y2));
        m_dx = 0;
        m_dy = 0;
        m_viewbox_x1 = m_device_x1;
        m_viewbox_y1 = m_device_y1;
        m_viewbox_x2 = m_device_x2;
        m_viewbox_y2 = m_device_y2;
        m_viewbox_dx = 0;
        m_viewbox_dy = 0;
        m_clip_x1    = m_device_x1;
        m_clip_y1    = m_device_y1;
        m_clip_x2    = m_device_x2;
        m_clip_y2    = m_device_y2;
        m_viewBox_level = 1;
        update();
    }

    //-------------------------------------------------------------
    void transformer::viewBox(double scx, double scy, double scw, double sch,
                              double vbx, double vby, double vbw, double vbh,
                              uniform_scaling_e align_type,
                              window_fit_logic_e meet_or_slice,
                              bool separate_buffer)
    {
        if(!is_valid())
        {
            invalidate();
            return;
        }

        double x0, y0, x1, y1, x2, y2;
        x2 = scx + scw;
        y2 = scy + sch;
        m_viewport.transform(&scx, &scy);
        m_viewport.transform(&x2,  &y2);
        if(separate_buffer)
        {
            x0 = 0;
            y0 = 0;
            m_viewport.transform(&x0, &y0);
            if(m_viewBox_level > 0)
            {
                x1 = m_zoom.device_dx() - m_viewbox_dx;
                y1 = m_zoom.device_dy() - m_viewbox_dy;
                m_zoom.inverse_transform_scale_only(&x1, &y1);
                x0 += x1;
                y0 += y1;
            }
            
            x1 = m_viewbox_x1;
            y1 = m_viewbox_y1;
            m_zoom.inverse_transform_scale_only(&x1, &y1);
            x0  -= x1;
            y0  -= y1;
            scx -= x0;
            scy -= y0;
            x2  -= x0;
            y2  -= y0;
        }
        else
        {
            x0 = m_dx;
            y0 = m_dy;
            m_zoom.inverse_transform_scale_only(&x0, &y0);
            scx -= x0;
            scy -= y0;
            x2  -= x0;
            y2  -= y0;
        }

        m_viewport.device_viewport(scx, scy, x2, y2);
        viewbox_to_viewport(m_viewport, vbx, vby, vbw, vbh, align_type, meet_or_slice);
        if(!m_viewport.is_valid())
        {
            invalidate();
            return;
        }

        m_dx = 0;
        m_dy = 0;
        if(separate_buffer)
        {
            bool wflag = m_window_x1 < m_window_x2 && m_window_y1 < m_window_y2;
            if(wflag)
            {
                m_viewport.world_viewport(&x1, &y1, &x2, &y2);
                m_viewport.transform(&x1, &y1);
                m_viewport.transform(&x2, &y2);
                x0 = x1;
                y0 = y1;
                m_zoom.transform(&x1, &y1);
                m_zoom.transform(&x2, &y2);
                m_viewbox_x1 = x1;
                m_viewbox_y1 = y1;
                m_viewbox_x2 = x2;
                m_viewbox_y2 = y2;

                m_viewbox_dx = 0;
                m_viewbox_dy = 0;
                if(m_viewbox_x1 < m_window_x1) 
                {
                    m_viewbox_dx = m_viewbox_x1 - m_window_x1;
                    m_viewbox_x1 = m_window_x1;
                }
                if(m_viewbox_y1 < m_window_y1) 
                {
                    m_viewbox_dy = m_viewbox_y1 - m_window_y1;
                    m_viewbox_y1 = m_window_y1;
                }
                if(m_viewbox_x2 > m_window_x2) m_viewbox_x2 = m_window_x2;
                if(m_viewbox_y2 > m_window_y2) m_viewbox_y2 = m_window_y2;

                m_buffer_x1 = int(floor(m_viewbox_x1));
                m_buffer_y1 = int(floor(m_viewbox_y1));
                m_buffer_x2 = int( ceil(m_viewbox_x2));
                m_buffer_y2 = int( ceil(m_viewbox_y2));

                m_viewbox_x1 -= m_buffer_x1;
                m_viewbox_y1 -= m_buffer_y1;
                m_viewbox_x2 -= m_buffer_x1;
                m_viewbox_y2 -= m_buffer_y1;

                m_viewport.transform(&m_dx, &m_dy);
                m_zoom.transform(&m_dx, &m_dy);
                m_dx -= m_viewbox_x1 + m_viewbox_dx;
                m_dy -= m_viewbox_y1 + m_viewbox_dy;
            }
            update();
            if(!wflag)
            {
                m_buffer_x1 = int(floor(m_device_x1));
                m_buffer_y1 = int(floor(m_device_y1));
                m_buffer_x2 = int(ceil(m_device_x2));
                m_buffer_y2 = int(ceil(m_device_y2));
            }
        }
        else
        {
            update();
            m_viewbox_x1 = vbx;
            m_viewbox_y1 = vby;
            m_viewbox_x2 = vbx + vbw;
            m_viewbox_y2 = vby + vbh;
            m_viewport.transform(&m_viewbox_x1, &m_viewbox_y1);
            m_viewport.transform(&m_viewbox_x2, &m_viewbox_y2);
            m_zoom.transform(&m_viewbox_x1, &m_viewbox_y1);
            m_zoom.transform(&m_viewbox_x2, &m_viewbox_y2);
            m_clip_x1 = m_viewbox_x1;
            m_clip_y1 = m_viewbox_y1;
            m_clip_x2 = m_viewbox_x2;
            m_clip_y2 = m_viewbox_y2;
            double w = m_buffer_x2 - m_buffer_x1;
            double h = m_buffer_y2 - m_buffer_y1;
            if(m_clip_x1 < 0) m_clip_x1 = 0;
            if(m_clip_x2 > w) m_clip_x2 = w;
            if(m_clip_y1 < 0) m_clip_y1 = 0;
            if(m_clip_y2 > h) m_clip_y2 = h;
        }
        ++m_viewBox_level;
    }

}
}
