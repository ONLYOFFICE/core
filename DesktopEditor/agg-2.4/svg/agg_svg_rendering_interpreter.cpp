//-----------------------------------------------------------------------------
#include "agg_svg_rendering_interpreter.h"
#include "agg_svg_pipeline.h"
#include "agg_svg_frame_buffer_rgba.h"
#include "agg_svg_renderer_rgba.h"
#include "agg_svg_shape_adaptors.h"
#include "agg_svg_gradient_lut_cache.h"
#include "agg_bounding_rect.h"
#include "agg_svg_percent.h"
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    //-------------------------------------------------------------------------
    rendering_interpreter::el_interpreter const
    rendering_interpreter::element_interpreters[end_of_elements] = {
        &render_circle,
		&render_clipPath,
        &render_defs,    
        &render_ellipse,
        &render_g,
        &render_line,
        &render_linearGradient,
        &render_path,
        &render_polygon,
        &render_polyline,
        &render_radialGradient,
        &render_rect,
        &render_stop,
        &render_svg,
        &set_title_element,
        &render_use,    
    };
    //-------------------------------------------------------------------------
    rendering_interpreter::rendering_interpreter(pipeline&              p, 
                                                 attributes&            attr,
                                                 renderer_rgba_type&    ren,
                                                 dom_storage::map_type& map,
                                                 gradient_lut_cache&    g_lut_cache,
                                                 bool render_paint_servers)

            :	m_storage(0)
			,	m_pipeline(p)
			,	m_attributes(attr)
			,	m_renderer(ren)
			,	m_it2element_map(map)
			,	m_gradient_lut_cache(g_lut_cache)
			,	m_current_element(end_of_elements)
			,	m_vertex_source(0)
			,	m_defs_counter(0)
			,	m_paint_server_counter(0)
			,	m_attr_setter(m_attributes)
			,	m_render_paint_servers(render_paint_servers)
			,	m_clipPath(false)
			,   m_gradient(0) 
			,   m_ignore_view_box(false)
	{
		m_attr_setter.set_interpreter(this);
    }        
    //-------------------------------------------------------------------------
    rendering_interpreter::rendering_interpreter(rendering_interpreter const& r,
                                                 attributes& attr,
                                                 renderer_rgba_type& ren,
                                                 bool render_paint_servers)
            :	m_storage(r.m_storage)
			,	m_pipeline(r.m_pipeline)
			,	m_attributes(attr)
			,	m_renderer(ren)
			,	m_it2element_map(r.m_it2element_map)
			,	m_current_element(end_of_elements)
			,	m_vertex_source(r.m_vertex_source)
			,	m_defs_counter(0)
			,	m_paint_server_counter(render_paint_servers ? -1 : 0)
			,	m_attr_setter(attr)
			,	m_render_paint_servers(render_paint_servers)
			,	m_gradient(0)
			,	m_gradient_lut_cache(r.m_gradient_lut_cache)
			,	m_clipPath(false)
			,   m_ignore_view_box(false)
    {
		m_attr_setter.set_interpreter(this);
    }
    //-------------------------------------------------------------------------
    bool rendering_interpreter::element_is_paint_server(element_e code) const
    {
        return (code == elem_linearGradient || code == elem_radialGradient);
    }
    //-------------------------------------------------------------------------
    bool rendering_interpreter::start_element(const dom_storage& st, 
                                              const element_data& el)
    {
        m_storage				= &st;
        m_current_element		= el.code;

        if (m_defs_counter)
        {
            if (el.code == elem_defs)
            {
                ++m_defs_counter;
         
                m_attributes.begin_session();
                render_defs(el);
            }
            return true;
        }
        else if (el.code == elem_defs)
            ++m_defs_counter;


        if (element_is_paint_server(el.code))
        {
            ++m_paint_server_counter;
        }

        if (m_paint_server_counter > 0)
            return true;

		if (m_clipPath)
			return true;

        m_attributes.begin_session();

        if (el.code != end_of_elements)
        {
            (this->*element_interpreters[el.code])(el);
        }
        
        return true;
    }
    //-------------------------------------------------------------------------
    bool rendering_interpreter::end_element(const dom_storage& st, 
                                            const element_data& el)
    {
        m_current_element = el.code;

        if (el.code == elem_defs)
        {
            --m_defs_counter;
            m_attributes.end_session();
           return true;
        }

        if (m_defs_counter)
        {
           return true;
        }

        if (element_is_paint_server(el.code))
        {
            if (m_paint_server_counter)
            {
                --m_paint_server_counter;
                return true;
            }
        }

		if (m_clipPath)
			m_clipPath = false;

        else if (m_paint_server_counter == 0)
            m_attributes.end_session();

        return true;
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::prepare_gradient_fill(gradient& g)
    {
        if (m_attributes.fill_type() == paint_gradient) 
        { 
            create_gradient(g, locate(m_attributes.fill_gradient_id())); 
            m_renderer.fill_gradient(g); 
        } 
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::prepare_gradient_stroke(gradient& g)
    {
        if (m_attributes.stroke_type() == paint_gradient) 
        { 
            create_gradient(g, locate(m_attributes.stroke_gradient_id()));
            m_renderer.stroke_gradient(g); 
        }
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::calculate_and_set_object_bbox(object_bbox& bbox)
    {
        assert(m_vertex_source);
            
        bbox.calculate(m_attributes, *m_vertex_source);
        m_attributes.object_bbox(bbox.bbox());
    }
    //-------------------------------------------------------------------------
    bool rendering_interpreter::is_visible(coord_type(&bounds)[4]) const
    {
        coord_type stroke_width = (coord_type)m_attributes.stroke_width();
        bounds[0] -= stroke_width;
        bounds[1] -= stroke_width;
        bounds[2] += stroke_width;
        bounds[3] += stroke_width;
        double x1 = bounds[0];
        double y1 = bounds[1];
        double x2 = bounds[2];
        double y2 = bounds[1];
        double x3 = bounds[2];
        double y3 = bounds[3];
        double x4 = bounds[0];
        double y4 = bounds[3];

        m_attributes.transform().transform(&x1, &y1);
        m_attributes.transform().transform(&x2, &y2);
        m_attributes.transform().transform(&x3, &y3);
        m_attributes.transform().transform(&x4, &y4);
        
        double cx1 = x1;
        double cy1 = y1;
        double cx2 = x1;
        double cy2 = y1;
        if(x2 < cx1) cx1 = x2;
        if(y2 < cy1) cy1 = y2;
        if(x3 < cx1) cx1 = x3;
        if(y3 < cy1) cy1 = y3;
        if(x4 < cx1) cx1 = x4;
        if(y4 < cy1) cy1 = y4;
        if(x2 > cx2) cx2 = x2;
        if(y2 > cy2) cy2 = y2;
        if(x3 > cx2) cx2 = x3;
        if(y3 > cy2) cy2 = y3;
        if(x4 > cx2) cx2 = x4;
        if(y4 > cy2) cy2 = y4;

        // Check for clipping
        //----------------------
        if(!m_attributes.is_visible(cx1, cy1, cx2, cy2))
        {
            return false;
        }

        // If not clipped check if the bounding box area
        // is more than 0.01 of a pixel.
        //----------------------
        return (cx2 - cx1) * (cy2 - cy1) > 0.01;
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::set_common_gradient_attributes(attr_data* attr, 
                                                        unsigned num_attr)
    {
        spreadMethod_e spreadMethod;

        for (unsigned i = 0; i < num_attr; ++i)
        {
            if (!attr[i].processed) switch (attr[i].code)
            {
            case attr_spreadMethod:
                assert(attr[i].data.size() == 1);
                spreadMethod = (spreadMethod_e)attr[i].data.int8u_at(0);
                assert(spreadMethod == spreadMethod_pad     ||
                       spreadMethod == spreadMethod_reflect ||
                       spreadMethod == spreadMethod_repeat);

                m_gradient->spreadMethod(spreadMethod);
                attr[i].processed = true;
                break;

            case attr_gradientUnits:
                m_gradient->gradientUnits(read_objectUnits(attr[i].data));
                attr[i].processed = true;
                break;

            case attr_transform:
                m_attr_setter.set_transform(*m_gradient, attr[i].data);
                attr[i].processed = true;
                break;
            }
        }
    }
    //-------------------------------------------------------------------------
    double rendering_interpreter::read_length(
                                          const data_accessor_type& data) const
    {
        assert(data.size() == sizeof(coord_type) + 1);
        
        double value = data.units_value(m_attributes);
        return value;
    }
    //-------------------------------------------------------------------------
    objectUnits_e rendering_interpreter::read_objectUnits(
                                          const data_accessor_type& data) const
    {
        assert(data.size() == 1);

        objectUnits_e objectUnits = (objectUnits_e)data.int8u_at(0);

        assert(objectUnits == objectUnits_userSpaceOnUse    ||
               objectUnits == objectUnits_objectBoundingBox ||
               objectUnits == objectUnits_strokeWidth);

        return objectUnits;
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::read_viewBox(const data_accessor_type& data,
                                             double* x, double* y, 
                                             double* w, double* h) const
    {
        assert(data.size() == 4 * sizeof(coord_type));

        *x = data.coord_at(sizeof(coord_type) * 0);
        *y = data.coord_at(sizeof(coord_type) * 1);
        *w = data.coord_at(sizeof(coord_type) * 2);
        *h = data.coord_at(sizeof(coord_type) * 3);
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::read_preserveAspectRatio(
                                  const data_accessor_type& data,
                                  uniform_scaling_e*        usc,
                                  window_fit_logic_e*       wfl) const
    {
        assert(data.size() == 2);

        *usc = uniform_scaling_e (data.int8u_at(0));
        *wfl = window_fit_logic_e(data.int8u_at(sizeof(agg::int8u)));

    }
    //-------------------------------------------------------------------------
    // render basic shapes
    //-------------------------------------------------------------------------
    void rendering_interpreter::render_rect(const element_data& el)
    {
        double x  =  0.;
        double y  =  0.;
        double w  =  0.;
        double h  =  0.;
        double rx = -1.;
        double ry = -1.;

        for (unsigned i = 0; i < el.num_attr; ++i)
        {
            if (!el.attr[i].processed) switch (el.attr[i].code)
            {
            case attr_x:      x  = read_length(el.attr[i].data); break;
            case attr_y:      y  = read_length(el.attr[i].data); break;
            case attr_width:  w  = read_length(el.attr[i].data); break;
            case attr_height: h  = read_length(el.attr[i].data); break;
            case attr_rx:     rx = read_length(el.attr[i].data); break;
            case attr_ry:     rx = read_length(el.attr[i].data); break;
            }
        }

        if (rx < 0. && ry < 0.) 
        {
            rx = 0.;
            rx = 0.;
        }
        if (rx < 0.) rx = ry;
        if (ry < 0.) ry = rx;

        if (rx > w / 2.) rx = w / 2.;
        if (ry > h / 2.) ry = h / 2.;

        m_attr_setter.set_transform_attributes(el.attr, el.num_attr);

        coord_type bounds[4] = {x, y, x + w, y + h};

        if (is_visible(bounds))
        {
            rectangle_adaptor source(x, y, w, h, rx, ry);
            general_work_for_basic_shapes(el, source);
        }
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::render_circle(const element_data& el)
    {
        double cx = 0.;
        double cy = 0.;
        double  r = 0.;

        for (unsigned i = 0; i < el.num_attr; ++i)
        {
            if (!el.attr[i].processed) switch (el.attr[i].code)
            {
            case attr_cx: cx = read_length(el.attr[i].data); break;
            case attr_cy: cy = read_length(el.attr[i].data); break;
            case attr_r:   r = read_length(el.attr[i].data); break;
            }
        }

        m_attr_setter.set_transform_attributes(el.attr, el.num_attr);

        coord_type bounds[4] = {cx - r, cy - r, cx + r, cy + r};

        if (is_visible(bounds))
        {
            circle_adaptor source(cx, cy, r);
            general_work_for_basic_shapes(el, source);
        }
    }
    //-------------------------------------------------------------------------
	void rendering_interpreter::render_clipPath(const element_data&)
	{
		m_clipPath = true;
	}
    //-------------------------------------------------------------------------
    void rendering_interpreter::render_ellipse(const element_data& el)
    {
        double cx = 0.;
        double cy = 0.;
        double rx = 0.;
        double ry = 0.;

        for (unsigned i = 0; i < el.num_attr; ++i)
        {
            if (!el.attr[i].processed) switch (el.attr[i].code)
            {
            case attr_cx: cx = read_length(el.attr[i].data); break;
            case attr_cy: cy = read_length(el.attr[i].data); break;
            case attr_rx: rx = read_length(el.attr[i].data); break;
            case attr_ry: ry = read_length(el.attr[i].data); break;
            }
        }

        m_attr_setter.set_transform_attributes(el.attr, el.num_attr);

        coord_type bounds[4] = {cx - rx, cy - ry, cx + rx, cy + ry};

        if (is_visible(bounds))
        {
            ellipse_adaptor source(cx, cy, rx, ry);
            general_work_for_basic_shapes(el, source);
        }
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::render_line(const element_data& el)
    {
        double x1 = 0.;
        double y1 = 0.;
        double x2 = 0.;
        double y2 = 0.;

        for (unsigned i = 0; i < el.num_attr; ++i)
        {
            if (!el.attr[i].processed) switch (el.attr[i].code)
            {
            case attr_x1: x1 = read_length(el.attr[i].data); break;
            case attr_y1: y1 = read_length(el.attr[i].data); break;
            case attr_x2: x2 = read_length(el.attr[i].data); break;
            case attr_y2: y2 = read_length(el.attr[i].data); break;
            }
        }

        m_attr_setter.set_transform_attributes(el.attr, el.num_attr);

        coord_type bounds[4] = {x1, y1, x2, y2};

        if (is_visible(bounds))
        {
            line_adaptor source(x1, y1, x2, y2);
            general_work_for_basic_shapes(el, source);
        }
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::render_polygon(const element_data& el)
    {
        assert(el.shape_data.size() > 0);

        m_attr_setter.set_transform_attributes(el.attr, el.num_attr);

        if (el.bounds)
        {
            coord_type bounds[4];
            memcpy(bounds, el.bounds, sizeof(bounds));

            if (is_visible(bounds))
            {        
                poly_interpreter<data_accessor_type> source(el.shape_data, true);
                general_work_for_basic_shapes(el, source);
            }
        }
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::render_polyline(const element_data& el)
    {
        assert(el.shape_data.size() > 0);

        m_attr_setter.set_transform_attributes(el.attr, el.num_attr);

        if (el.bounds)
        {
            coord_type bounds[4];
            memcpy(bounds, el.bounds, sizeof(bounds));

            if (is_visible(bounds))
            {
                poly_interpreter<data_accessor_type> source(el.shape_data, false);
                general_work_for_basic_shapes(el, source);
            }
        }
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::render_path(const element_data& el)
    {
        assert(el.shape_data.size() > 0);

        m_attr_setter.set_transform_attributes(el.attr, el.num_attr);
        
        if (el.bounds)
        {
            coord_type bounds[4];
            memcpy(bounds, el.bounds, sizeof(bounds));

            if (is_visible(bounds))
            {
                path_interpreter<data_accessor_type> source(el.shape_data);
                general_work_for_basic_shapes(el, source);
            }
        }
    }  
    //-------------------------------------------------------------------------
    void rendering_interpreter::render_radialGradient(const element_data& el)
    {
        m_attr_setter.set_color_attributes           (el.attr, el.num_attr);
        m_attr_setter.set_xlink_attributes           (el.attr, el.num_attr);

        set_common_gradient_attributes (el.attr, el.num_attr);

        // set default values
        double cx = m_attributes.conv_units(50., units_percent);
        double cy = m_attributes.conv_units(50., units_percent);
        double  r = m_attributes.conv_units(50., units_percent  );
        
        // read attributes from DOM storage
        for (unsigned i = 0; i < el.num_attr; ++i)
        {
            if (!el.attr[i].processed) switch (el.attr[i].code)
            {
            case attr_cx:
                assert(el.attr[i].data.size() == sizeof(coord_type) + 1);
                cx = el.attr[i].data.units_value(m_attributes);
                break;
            case attr_cy:
                assert(el.attr[i].data.size() == sizeof(coord_type) + 1);
                cy = el.attr[i].data.units_value(m_attributes);
                break;
            case attr_r:
                assert(el.attr[i].data.size() == sizeof(coord_type) + 1);
                r = el.attr[i].data.units_value(m_attributes);
                break;
            case attr_fx:
                assert(el.attr[i].data.size() == sizeof(coord_type) + 1);
                m_gradient->fx(el.attr[i].data.units_value(m_attributes));
                break;
            case attr_fy:
                assert(el.attr[i].data.size() == sizeof(coord_type) + 1);
                m_gradient->fy(el.attr[i].data.units_value(m_attributes));
                break;
            }
        }

        m_gradient->radial(cx, cy, r);
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::render_linearGradient(const element_data& el)
    {
        m_attr_setter.set_color_attributes           (el.attr, el.num_attr);
        m_attr_setter.set_xlink_attributes           (el.attr, el.num_attr);

        set_common_gradient_attributes (el.attr, el.num_attr);

        double x1 = 0.;
        double y1 = 0.;
        double x2 = 1.;
        double y2 = 0.;

        for (unsigned i = 0; i < el.num_attr; ++i)
        {
            if (!el.attr[i].processed) switch (el.attr[i].code)
            {
            case attr_x1:
                assert(el.attr[i].data.size() == sizeof(coord_type) + 1);
                x1 =  el.attr[i].data.units_value(m_attributes);
                break;
            case attr_y1:
                assert(el.attr[i].data.size() == sizeof(coord_type) + 1);
                y1 =  el.attr[i].data.units_value(m_attributes);
                break;
            case attr_x2:
                assert(el.attr[i].data.size() == sizeof(coord_type) + 1);
                x2 =  el.attr[i].data.units_value(m_attributes);
                break;
            case attr_y2:
                assert(el.attr[i].data.size() == sizeof(coord_type) + 1);
                y2 =  el.attr[i].data.units_value(m_attributes);
                break;
            }
        }

        m_gradient->linear(x1, y1, x2, y2);
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::render_defs(const element_data& el)
    {
        m_vertex_source   = 0; 

        m_attr_setter.set_presentation_attributes    (el.attr, el.num_attr);
        m_attr_setter.set_transform_attributes       (el.attr, el.num_attr);
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::create_gradient(gradient& g, unsigned idx)
    {
        object_bbox bbox;

        bool is_necessary_set_bbox = 
            units_is_objectBoundingBox(idx, attr_gradientUnits);

        if (is_necessary_set_bbox)
        {
            calculate_and_set_object_bbox(bbox);
        }

        attributes attr(m_attributes.settings()); 

		attr.begin_session();

        if (is_necessary_set_bbox)
        {
            attr.object_bbox(bbox.bbox());
        }

        rendering_interpreter rin(*this, attr, m_renderer,true);
        rin.set_gradient(&g);

        m_storage->traverse(rin, idx);
        
        g.create_gradient_lut(attr, m_gradient_lut_cache);

		attr.end_session();
    }
    //-------------------------------------------------------------------------
    bool rendering_interpreter::units_is_objectBoundingBox(unsigned idx, 
                                                           attr_e   code) const
    {
        data_accessor_type data = m_storage->get_attr_specified_value(idx, code);

        if (data.size() == 1)
        {
            objectUnits_e units = read_objectUnits(data);
            
            return units == objectUnits_objectBoundingBox;
        }
        else if (data.size() == 0)
        {
            return true;
        }
        return false;
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::render_g(const element_data& el)
    {
        m_vertex_source   = 0; 

        m_attr_setter.set_presentation_attributes    (el.attr, el.num_attr);
        m_attr_setter.set_transform_attributes       (el.attr, el.num_attr);
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::process_xlink_href(data_accessor_type data)
    {
        unsigned index = locate(data);

		if (index == 0) return;

        if (m_current_element == elem_linearGradient || 
            m_current_element == elem_radialGradient)
        {
            rendering_interpreter rin(*this, m_attributes, m_renderer, true);
            rin.set_gradient(m_gradient);
            m_storage->traverse(rin, index);
        }
        else
        {
            assert(false);
        }
    }
    //-------------------------------------------------------------------------
    unsigned rendering_interpreter::locate(const char* id)
    {
        using namespace std;

        typedef agg::pod_array_adaptor<char const> adaptor_type;

        adaptor_type id_adaptor(id, (unsigned)strlen(id));

        data_accessor<adaptor_type> data(id_adaptor);

        return locate(data);
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::render_stop(const element_data& el)
    {
        m_vertex_source   = 0; 

        m_attr_setter.set_color_attributes(el.attr, el.num_attr);

        double     stop_offset = 0.;
        color_type stop_color   = color_type(0, 0, 0);
        double     stop_opacity = 1.;

        for (unsigned i = 0; i < el.num_attr; ++i)
        {
            if (!el.attr[i].processed) switch (el.attr[i].code)
            {
            case attr_offset:
                assert(el.attr[i].data.size() == sizeof(coord_type));
                stop_offset = percent::decrypt(el.attr[i].data.coord_at(0));
                break;

            case attr_stop_color:
                assert(el.attr[i].data.size() == sizeof(color_type));
                stop_color = el.attr[i].data.color_at(0);
                break;
    
            case attr_stop_opacity:
                assert(el.attr[i].data.size() == sizeof(coord_type));
                stop_opacity = el.attr[i].data.coord_at(0);
                break;

            }
        }
        m_gradient->add_stop(stop_offset, stop_color, stop_opacity);
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::render_svg(const element_data& el)
    {
        m_vertex_source = 0; 

        m_attr_setter.set_presentation_attributes(el.attr, el.num_attr);

        // set default values
        coordinate x = {  0, units_px};
        coordinate y = {  0, units_px};
        length     w = {100, units_percent_x};
        length     h = {100, units_percent_y};

        double vb_x =  0.;
        double vb_y =  0.;
        double vb_w =  0.;
        double vb_h =  0.;

        uniform_scaling_e  usc = usc_xMidYMid;
		if (m_ignore_view_box)
			usc = usc_none;
        window_fit_logic_e wfl = window_meet;

        bool is_viewBox_present = false;

        // read attributes
        for (unsigned i = 0; i < el.num_attr; ++i)
        {
            if (!el.attr[i].processed) switch (el.attr[i].code)
            {
            case attr_x:
                assert(el.attr[i].data.size() == sizeof(coord_type) + 1);
                x.value = el.attr[i].data.coord_at(0);
                x.unit = units2_e(el.attr[i].data.int8u_at(sizeof(coord_type)));
                break;

            case attr_y:
                assert(el.attr[i].data.size() == sizeof(coord_type) + 1);
                y.value = el.attr[i].data.coord_at(0);
                y.unit = units2_e(el.attr[i].data.int8u_at(sizeof(coord_type)));
                break;
            case attr_width:
                assert(el.attr[i].data.size() == sizeof(coord_type) + 1);
                w.value = el.attr[i].data.coord_at(0);
                w.unit = units2_e(el.attr[i].data.int8u_at(sizeof(coord_type)));
                break;
            case attr_height:
                assert(el.attr[i].data.size() == sizeof(coord_type) + 1);
                h.value = el.attr[i].data.coord_at(0);
                h.unit = units2_e(el.attr[i].data.int8u_at(sizeof(coord_type)));
                break;

            case attr_viewBox:
                read_viewBox(el.attr[i].data, &vb_x, &vb_y, &vb_w, &vb_h);
                is_viewBox_present = true;
                break;

            case attr_preserveAspectRatio:
                read_preserveAspectRatio(el.attr[i].data, &usc, &wfl);
                break;
            }
        }

        // Normalize values
        x.value = m_attributes.conv_units(x.value, x.unit);
        y.value = m_attributes.conv_units(y.value, y.unit);
        w.value = m_attributes.conv_units(w.value, w.unit);
        h.value = m_attributes.conv_units(h.value, h.unit);

        if (!is_viewBox_present)
        {
            vb_w = w.value;
            vb_h = h.value;
        }

		if (m_ignore_view_box)
		{
			double scrW = (m_attributes.window_x2()<0)?w.value:(m_attributes.window_x2()/2-m_attributes.window_x1());
			double scrH = (m_attributes.window_y2()<0)?h.value:(m_attributes.window_y2()/2-m_attributes.window_y1());
		   m_attributes.viewBox(m_attributes.window_x1(), m_attributes.window_y1(), 
			                 scrW, 
							 scrH,
							 vb_x, vb_y, vb_w, vb_h, usc, wfl, false);
		}
		else
		{
			m_attributes.viewBox(x.value, y.value, w.value, h.value,
				vb_x, vb_y, vb_w, vb_h, usc, wfl, false);
		}
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::set_title_element(const element_data& el)
    {
        m_vertex_source   = 0; 
    }
    //-------------------------------------------------------------------------
    void rendering_interpreter::render_use(const element_data& el)
    {
		m_attr_setter.set_paint_attributes           (el.attr, el.num_attr);
		m_attr_setter.set_color_attributes           (el.attr, el.num_attr);   
		m_attr_setter.set_opacity_attributes         (el.attr, el.num_attr);

        m_attr_setter.set_transform_attributes       (el.attr, el.num_attr);

        double x = 0.;
        double y = 0.;
        data_accessor_type href;
    
        for (unsigned i = 0; i < el.num_attr; ++i)
        {
            if (!el.attr[i].processed) switch (el.attr[i].code)
            {
                case attr_x: x = read_length(el.attr[i].data); break;
                case attr_y: y = read_length(el.attr[i].data); break;
                case attr_xlink_href: href = el.attr[i].data;  break;
            }
        }

        if (href.size() > 0)
        {
            m_attributes.translate(x, y);

			unsigned index = locate(href, 0);

			if (index != 0)
			{
				rendering_interpreter rin(*this, m_attributes, m_renderer);
				m_storage->traverse(rin, index);
			}
        }
    }

}
}

