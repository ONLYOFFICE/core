//-----------------------------------------------------------------------------
#include "agg_svg_attributes.h"
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    //---------------------------------------------------------------------------------
    agg::line_join_e g_stroke_linejoin_lut[] = 
    {
        agg::miter_join_revert,
        agg::round_join,
        agg::bevel_join
    };
    //---------------------------------------------------------------------------------
    agg::line_cap_e g_stroke_linecap_lut[] = 
    {
        agg::butt_cap,
        agg::round_cap,
        agg::square_cap
    };
    //---------------------------------------------------------------------------------
    agg::filling_rule_e g_fill_rule_lut[] = 
    {
        agg::fill_non_zero,
        agg::fill_even_odd
    };
    //--------------------------------------------------------------------
    agg::int8u* attr_stack::push_attribute(unsigned type, unsigned size)
    {
        if(m_sessions.size())
        {
            session& s = m_sessions.last();
            agg::int8u* data = 0;
            
            if(size < attr_heap_block_size)
            {
                int i = m_heap.allocate_continuous_block(size);
                if(i >= 0)
                {
                    data = &m_heap[i];
                }
            }
            else
            {
                data = new agg::int8u[size];
                type |= large_block_flag;
            }

            if(data)
            {
                m_attributes.add(attr(type, data, size));
                s.set_saved_attribute(type);
                ++s.num_attributes;
                return data;
            }
        }
        return 0;
    }
    //--------------------------------------------------------------------
    void attr_stack::end_session()
    {
        const session& s = m_sessions.last();
        for(int i = (int)num_attributes() - 1; i >= 0; --i)
        {
            attr& a = m_attributes[s.start_attr_index + i];
            if(a.type & large_block_flag)
            {
                delete [] a.data;
            }
        }
        m_heap.free_tail(s.start_attr_heap);
        m_attributes.free_tail(s.start_attr_index);
        m_sessions.remove_last();
    }
    //--------------------------------------------------------------------
    attributes::attributes(const global_settings& settings) 
		:	m_stack()
		,	m_settings(&settings)
		,   m_values()
    {}
    //--------------------------------------------------------------------
    struct affine_serializer_adaptor
    {
        transformer& m_trans;
        affine_serializer_adaptor(transformer& trans) : m_trans(trans) {}
        unsigned byte_size() const { return m_trans.byte_size_affine(); }
        void serialize(agg::int8u* ptr) const { m_trans.serialize_affine(ptr); }
        void deserialize(const agg::int8u* ptr) { m_trans.deserialize_affine(ptr); }
    };
    //--------------------------------------------------------------------
    void attributes::end_session()
    {
        // Restore saved attributes
        //-------------------------
        int i = int(m_stack.num_attributes()) - 1;
        for(; i >= 0; --i)
        {
            const attr_stack::attr* a = m_stack.attribute(i);
            if(a)
            {
                switch(a->attr_type())
                {
                case attr_transform: { affine_serializer_adaptor  (m_values.transformer).deserialize(a->data); } break;
                case attr_viewBox:           restore_attribute(m_values.transformer,        a->data); break;
                case attr_color:             restore_attribute(m_values.color,              a->data); break;
                case attr_opacity:           restore_attribute(m_values.opacity,            a->data); break;
                case attr_fill:              restore_attribute(m_values.fill_type,          a->data); break;
                case attr_fill_color:        restore_attribute(m_values.fill_color,         a->data); break;
                case attr_fill_gradient:     restore_serializable(m_values.fill_gradient_id,a->data, a->size); break;
                case attr_fill_opacity:      restore_attribute(m_values.fill_opacity,       a->data); break;

                case attr_stroke:            restore_attribute(m_values.stroke_type,        a->data); break;
                case attr_stroke_color:      restore_attribute(m_values.stroke_color,       a->data); break;
                case attr_stroke_gradient:   restore_serializable(m_values.stroke_gradient_id, a->data, a->size); break;
                case attr_stroke_opacity:    restore_attribute(m_values.stroke_opacity,     a->data); break;
                case attr_fill_rule:         restore_attribute(m_values.fill_rule,          a->data); break;
                case attr_stroke_linejoin:   restore_attribute(m_values.stroke_linejoin,    a->data); break;
                case attr_stroke_linecap:    restore_attribute(m_values.stroke_linecap,     a->data); break;
                case attr_stroke_miterlimit: restore_attribute(m_values.stroke_miterlimit,  a->data); break;
                case attr_stroke_width:      restore_attribute(m_values.stroke_width,       a->data); break;
                }
            }
        }
        // Pop all attributes from stack
        //--------------------------
        m_stack.end_session();
    }
    //--------------------------------------------------------------------
    void attributes::reset_all() 
    { 
        m_values = values(); 
    }
    //--------------------------------------------------------------------
    void attributes::window(int x1, int y1, int x2, int y2)
    {
        m_values.transformer.window(x1, y1, x2, y2);
        update_zoom();
    }
    //--------------------------------------------------------------------
    void attributes::set_zoom(double x1, double y1, double x2, double y2)
    {
        zoom_data zoom;
        zoom.sx1 = window_x1();
        zoom.sy1 = window_y1();
        zoom.sx2 = window_x2();
        zoom.sy2 = window_y2();
        zoom.wx1 = x1;
        zoom.wy1 = y1;
        zoom.wx2 = x2;
        zoom.wy2 = y2;
        m_zoom_stack.add(zoom);
        update_zoom();
    }
    //--------------------------------------------------------------------
    void attributes::initial_zoom()
    {
        m_zoom_stack.remove_all();
        update_zoom();
    }
    //--------------------------------------------------------------------
    void attributes::update_zoom()
    {
        unsigned i;
        m_values.transformer.zoom(window_x1(), window_y1(), window_x2(), window_y2(),
                                  window_x1(), window_y1(), window_x2(), window_y2());
        for(i = 0; i < m_zoom_stack.size(); i++)
        {
            const zoom_data& zoom = m_zoom_stack[i];
            double zoom_wx1 = zoom.wx1;
            double zoom_wy1 = zoom.wy1;
            double zoom_wx2 = zoom.wx2;
            double zoom_wy2 = zoom.wy2;
            m_values.transformer.zoom().inverse_transform(&zoom_wx1, &zoom_wy1);
            m_values.transformer.zoom().inverse_transform(&zoom_wx2, &zoom_wy2);
            m_values.transformer.zoom(zoom_wx1, zoom_wy1, zoom_wx2, zoom_wy2,
                                      zoom.sx1, zoom.sy1, zoom.sx2, zoom.sy2);

        }
    }
    //--------------------------------------------------------------------
    void attributes::viewBox(double scx, double scy, double scw, double sch,
                             double vbx, double vby, double vbw, double vbh, 
                             uniform_scaling_e align_type,
                             window_fit_logic_e meet_or_slice,
                             bool separate_window)
    {
        save_attribute(attr_viewBox, m_values.transformer);
        m_values.transformer.viewBox(scx, scy, scw, sch,
                                     vbx, vby, vbw, vbh,
                                     align_type, meet_or_slice, separate_window);
    }
    //--------------------------------------------------------------------
    void attributes::opacity(double op)
    {
        save_attribute(attr_opacity, m_values.opacity);
        m_values.opacity = op;
    }
    //--------------------------------------------------------------------
    void attributes::color(const color_type& c)
    {
        save_attribute(attr_color, m_values.color);
        m_values.color = gamma_color(c);
    }
    //--------------------------------------------------------------------
    void attributes::color(unsigned c)
    {
        color(agg::rgb8_packed(c));
    }
    //--------------------------------------------------------------------
    void attributes::fill_none()
    {
        save_attribute(attr_fill, m_values.fill_type);
        m_values.fill_type = paint_none;
    }
    //--------------------------------------------------------------------
    void attributes::fill_currentColor()
    {
        save_attribute(attr_fill, m_values.fill_type);
        m_values.fill_type = paint_currentColor;
    }
    //--------------------------------------------------------------------
    void attributes::fill(const color_type& c)
    {
        save_attribute(attr_fill,       m_values.fill_type);
        save_attribute(attr_fill_color, m_values.fill_color);
        m_values.fill_type = paint_color;
        m_values.fill_color = gamma_color(c);
    }
    //--------------------------------------------------------------------
    void attributes::fill(unsigned c)
    {
        fill(agg::rgb8_packed(c));
    }
    //--------------------------------------------------------------------
    color_type attributes::fill_color() const
    {
        color_type c = (m_values.fill_type == paint_color) ? m_values.fill_color : m_values.color;
        c.a = (color_type::value_type)(c.a * fill_opacity() + 0.5);
        return c;
    }
    //--------------------------------------------------------------------
    void attributes::fill_opacity(double op)
    {
        save_attribute(attr_fill_opacity, m_values.fill_opacity);
        m_values.fill_opacity = op;
    }
    //--------------------------------------------------------------------
    void attributes::stroke_none()
    {
        save_attribute(attr_stroke, m_values.stroke_type);
        m_values.stroke_type = paint_none;
    }
    //--------------------------------------------------------------------
    void attributes::stroke_currentColor()
    {
        save_attribute(attr_stroke, m_values.stroke_type);
        m_values.stroke_type = paint_currentColor;
    }
    //--------------------------------------------------------------------
    void attributes::stroke(const color_type& c)
    {
        save_attribute(attr_stroke,       m_values.stroke_type);
        save_attribute(attr_stroke_color, m_values.stroke_color);
        m_values.stroke_type = paint_color;
        m_values.stroke_color = gamma_color(c);
    }
    //--------------------------------------------------------------------
    void attributes::stroke(unsigned c)
    {
        stroke(agg::rgb8_packed(c));
    }
    //--------------------------------------------------------------------
    color_type attributes::stroke_color() const
    {
        color_type c = (m_values.stroke_type == paint_color) ? m_values.stroke_color : m_values.color;
        c.a = (color_type::value_type)(c.a * stroke_opacity() + 0.5);
        return c;
    }
    //--------------------------------------------------------------------
    void attributes::stroke_opacity(double op)
    {
        save_attribute(attr_stroke_opacity, m_values.stroke_opacity);
        m_values.stroke_opacity = op;
    }
    //--------------------------------------------------------------------
    void attributes::fill_rule(fill_rule_e fe)
    {
        save_attribute(attr_fill_rule, m_values.fill_rule);
        m_values.fill_rule = fe;
    }
    //--------------------------------------------------------------------
    void attributes::stroke_linejoin(stroke_linejoin_e lj)
    {
        save_attribute(attr_stroke_linejoin, m_values.stroke_linejoin);
        m_values.stroke_linejoin = lj;
    }
    //--------------------------------------------------------------------
    void attributes::stroke_linecap(stroke_linecap_e lc)
    {
        save_attribute(attr_stroke_linecap, m_values.stroke_linecap);
        m_values.stroke_linecap = lc;
    }
    //--------------------------------------------------------------------
    void attributes::stroke_miterlimit(double ml)
    {
        save_attribute(attr_stroke_miterlimit, m_values.stroke_miterlimit);
        m_values.stroke_miterlimit = ml;
    }
    //--------------------------------------------------------------------
    void attributes::stroke_width(double sw)
    {
        save_attribute(attr_stroke_width, m_values.stroke_width);
        m_values.stroke_width = sw;
        if(sw <= 1e-30)
        {
            stroke_none();
        }
    }
    //--------------------------------------------------------------------
    void attributes::object_bbox(double x1, double y1, double x2, double y2)
    {
        save_attribute(attr_object_bbox, m_values.object_bbox);
        m_values.object_bbox[0] = x1;
        m_values.object_bbox[1] = y1;
        m_values.object_bbox[2] = x2;
        m_values.object_bbox[3] = y2;
    }
    //--------------------------------------------------------------------
    void attributes::object_bbox(const rectangle& bbox)
    {
        object_bbox(bbox.x1, bbox.y1, bbox.x2, bbox.y2);
    }
    //--------------------------------------------------------------------
    void attributes::transform(const agg::trans_affine& mtx)
    {
        save_serializable(attr_transform, affine_serializer_adaptor(m_values.transformer));
        m_values.transformer.transform(mtx);
    }
    //--------------------------------------------------------------------
    void attributes::transform(double a0, double a1, double a2,
                               double a3, double a4, double a5)
    {
        save_serializable(attr_transform, affine_serializer_adaptor(m_values.transformer));
        m_values.transformer.transform(a0, a1, a2, a3, a4, a5);
    }
    //--------------------------------------------------------------------
    void attributes::translate(double dx, double dy)
    {
        save_serializable(attr_transform, affine_serializer_adaptor(m_values.transformer));
        m_values.transformer.translate(dx, dy);
    }
    //--------------------------------------------------------------------
    void attributes::rotate(double angle)
    {
        save_serializable(attr_transform, affine_serializer_adaptor(m_values.transformer));
        m_values.transformer.rotate(angle);
    }
    //--------------------------------------------------------------------
    void attributes::rotate(double angle, double cx, double cy)
    {
        save_serializable(attr_transform, affine_serializer_adaptor(m_values.transformer));
        m_values.transformer.rotate(angle, cx, cy);
    }
    //--------------------------------------------------------------------
    void attributes::scale(double s)
    {
        save_serializable(attr_transform, affine_serializer_adaptor(m_values.transformer));
        m_values.transformer.scale(s);
    }
    //--------------------------------------------------------------------
    void attributes::scale(double sx, double sy)
    {
        save_serializable(attr_transform, affine_serializer_adaptor(m_values.transformer));
        m_values.transformer.scale(sx, sy);
    }
    //--------------------------------------------------------------------
    void attributes::skew(double sx, double sy)
    {
        save_serializable(attr_transform, affine_serializer_adaptor(m_values.transformer));
        m_values.transformer.skew(sx, sy);
    }
    //--------------------------------------------------------------------
    void attributes::skew_x(double s)
    {
        save_serializable(attr_transform, affine_serializer_adaptor(m_values.transformer));
        m_values.transformer.skew_x(s);
    }
    //--------------------------------------------------------------------
    void attributes::skew_y(double s)
    {
        save_serializable(attr_transform, affine_serializer_adaptor(m_values.transformer));
        m_values.transformer.skew_y(s);
    }
    //---------------------------------------------------------------------------------
    double attributes::percent2pix(double percent) const
    {
        double w = window_x2() - window_x1();
        double h = window_y2() - window_y1();
        if(m_values.transformer.viewBox_level())
        {
            double x1, y1, x2, y2;
            m_values.transformer.world_viewport(&x1, &y1, &x2, &y2);
            w = x2 - x1;
            h = y2 - y1;
        }
        return percent * sqrt(w*w + h*h) * 0.0070710678118654752440084436210485;
    }
    //---------------------------------------------------------------------------------
    double attributes::percent2pix_x(double percent) const
    {
        double w = window_x2() - window_x1();
        if(m_values.transformer.viewBox_level())
        {
            double x1, y1, x2, y2;
            m_values.transformer.world_viewport(&x1, &y1, &x2, &y2);
            w = x2 - x1;
        }
        return percent * 0.01 * w;
    }
    //---------------------------------------------------------------------------------
    double attributes::percent2pix_y(double percent) const
    {
        double h = window_y2() - window_y1();
        if(m_values.transformer.viewBox_level())
        {
            double x1, y1, x2, y2;
            m_values.transformer.world_viewport(&x1, &y1, &x2, &y2);
            h = y2 - y1;
        }
        return percent * 0.01 * h;
    }
    //---------------------------------------------------------------------------------
    double attributes::conv_units(double v, units2_e u) const
    {
        switch(u)
        {
            default:              break;
            case units_em:        return v; // don't supported
            case units_ex:        return v; // don't supported

			case units_pt:		  
            case units_pt_x:      return m_settings->pt2pix_x(v);
            case units_pt_y:      return m_settings->pt2pix_y(v);

			case units_pc:		  
            case units_pc_x:      return m_settings->pc2pix_x(v);
            case units_pc_y:      return m_settings->pc2pix_y(v);

			case units_cm:		  
            case units_cm_x:      return m_settings->cm2pix_x(v);
            case units_cm_y:      return m_settings->cm2pix_y(v);

			case units_mm:
            case units_mm_x:      return m_settings->mm2pix_x(v);
            case units_mm_y:      return m_settings->mm2pix_y(v);

			case units_in:
            case units_in_x:      return m_settings->in2pix_x(v);
            case units_in_y:      return m_settings->in2pix_y(v);

            case units_percent:   return percent2pix(v);
            case units_percent_x: return percent2pix_x(v);
            case units_percent_y: return percent2pix_y(v);

            case units_deg:       return v * agg::pi / 180.0;
            case units_grad:      return v * agg::pi / 200.0;
            case units_rad:       return v;
        }
        return v;
    }

}
}






