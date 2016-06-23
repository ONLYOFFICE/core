#include <stdio.h>
#include <stdlib.h>

#include "agg_scanline_p.h"
#include "platform/agg_platform_support.h"
#include "ctrl/agg_slider_ctrl.h"
#include "agg_svg_exception.h"
#include "agg_svg_parser.h"
#include "agg_svg_indexation_interpreter.h"
#include "agg_svg_rendering_interpreter.h"
#include "agg_svg_attributes_map.h"
#include "agg_svg_rasterizer.h"

enum { flip_y_e = false };

class the_application : public agg::platform_support
{
    agg::svg::dom_storage             m_storage;
    agg::svg::dom_storage::map_type   m_id2elem_map;
    agg::svg::attributes_map          m_attr_map;
    agg::svg::global_settings         m_settings;
	agg::svg::attributes              m_attributes;
    agg::svg::frame_buffer_rgba       m_frame_buffer;
    agg::svg::rasterizer              m_rasterizer;
    agg::svg::renderer_rgba           m_renderer;
    agg::svg::pipeline                m_pipeline;
    agg::svg::gradient_lut_cache      m_gradient_lut_cache;

    //agg::slider_ctrl<agg::rgba8> m_expand;
    agg::slider_ctrl<agg::rgba8> m_gamma;
    //agg::slider_ctrl<agg::rgba8> m_scale;
    //agg::slider_ctrl<agg::rgba8> m_rotate;

	enum tool_type_e {tool_none, tool_zoom, tool_pan};
	
	tool_type_e m_current_tool;

	int       m_x1;
	int       m_y1;
	int       m_x2;
	int       m_y2;
	bool      m_drag_flag;

public:

    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        //m_expand(5,     5,    256-5, 11,    !flip_y),
        m_gamma (5,     5, 256-5, 20, !flip_y),
        //m_scale (256+5, 5,    512-5, 11,    !flip_y),
        //m_rotate(256+5, 5+15, 512-5, 11+15, !flip_y),
		m_x1(0),
		m_y1(0),
		m_x2(0),
		m_y2(0),
        m_drag_flag(false),
		m_attributes(m_settings),
		m_renderer(m_rasterizer, m_frame_buffer),
		m_current_tool(tool_none)
    {
        //add_ctrl(m_expand);
        add_ctrl(m_gamma);
        //add_ctrl(m_scale);
        //add_ctrl(m_rotate);

        //m_expand.label("Expand=%3.2f");
        //m_expand.range(-1, 1.2);
        //m_expand.value(0.0);

        m_gamma.label("Gamma=%3.2f");
        m_gamma.range(0.0, 3.0);
        m_gamma.value(1.0);

        //m_scale.label("Scale=%3.2f");
        //m_scale.range(0.2, 10.0);
        //m_scale.value(1.0);

        //m_rotate.label("Rotate=%3.2f");
        //m_rotate.range(-180.0, 180.0);
        //m_rotate.value(0.0);
    }

	void swap_int(int* n1, int* n2)
	{
		int temp;

		temp = *n1;
		*n1  = *n2;
		*n2  = temp; 
	}

	void normalize_rect(int* x1, int* y1, int* x2, int* y2)
	{
		if (*x1 > *x2)
		{
			swap_int(x1, x2);
		}
		if (*y1 > *y2)
		{
			swap_int(y1, y2);
		}
	}

	void update_zoom()
	{
		if (m_current_tool == tool_zoom)
		{
			int x1 = m_x1;
			int y1 = m_y1;
			int x2 = m_x2;
			int y2 = m_y2;

			normalize_rect(&x1, &y1, &x2, &y2);

			m_attributes.set_zoom(x1, y1, x2, y2);
		}
		else if (m_current_tool == tool_pan)
		{
			int offset_x = m_x2 - m_x1;
			int offset_y = m_y2 - m_y1;

			int x1 = -offset_x;
			int y1 = -offset_y;
			int x2 = rbuf_window().width()  - offset_x;
			int y2 = rbuf_window().height() - offset_y;
	        
			m_attributes.set_zoom(x1, y1, x2, y2);
		}
	}

    void indexation()
    {
		agg::svg::indexation_interpreter consumer(m_id2elem_map);

		m_storage.traverse(consumer);

		m_id2elem_map.sort_state(agg::svg::on);	
    }

    void reset()
    {
        m_id2elem_map.clear();
        m_attributes.clear();
        m_renderer.reset();
    }

    void parse_svg(const char* fname)
    {
		if(!fname)
			return;

        m_attributes.initial_zoom(); 
        m_storage.clear();
        reset();

		agg::svg::parser p(m_storage, m_attr_map);
		p.parse(fname);
        indexation();
    }

	void on_resize(int sx, int sy)
	{
		m_frame_buffer.create(sx, sy, flip_y_e);
	}

	void draw(agg::rendering_buffer& rbuf, double expand, double gamma)
	{
		agg::svg::renderer_rgba::pixfmt_type        pixfmt(rbuf);
		agg::svg::renderer_rgba::renderer_base_type rbase(pixfmt);

		m_rasterizer.gamma(gamma);
		m_settings.gamma(gamma);

		m_attributes.window(0, 0, rbuf.width(), rbuf.height());

		m_frame_buffer.clear(agg::svg::color_type(255, 255, 255));

		m_renderer.attach(m_frame_buffer);

		agg::svg::rendering_interpreter rin(  m_pipeline, 
											  m_attributes, 
											  m_renderer, 
											  m_id2elem_map,
											  m_gradient_lut_cache
											  );
#ifdef EXPAND_PATHS
		rin.expand(expand);
#endif
 
        m_storage.traverse(rin);
        
        assert(0 == m_attributes.num_sessions());

        if (m_settings.gamma() != 1.0)
        {
            m_frame_buffer.pixfmt().apply_gamma_inv(m_settings.gamma_lut());
        }

        if(m_frame_buffer.is_valid())
        {
            rbase.copy_from(m_frame_buffer.ren_buf());

			if (m_current_tool == tool_zoom)
			{
				agg::rgba8 color(0, 0, 0, 255);

				rbase.blend_hline(m_x1, m_y1, m_x2, color, agg::cover_full);
				rbase.blend_hline(m_x1, m_y2, m_x2, color, agg::cover_full);
				rbase.blend_vline(m_x1, m_y1, m_y2, color, agg::cover_full);
				rbase.blend_vline(m_x2, m_y1, m_y2, color, agg::cover_full);
			}
        }
        else
        {
            rbase.clear(agg::svg::color_type(255, 255, 255));
        }
	}

    virtual void on_draw()
    {
		try
		{
			typedef agg::pixfmt_bgra32 pixfmt;
			typedef agg::renderer_base<pixfmt> renderer_base;
			typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;

			pixfmt pixf(rbuf_window());
			renderer_base rb(pixf);
			renderer_solid ren(rb);

			rb.clear(agg::rgba(1,1,1));

			agg::rasterizer_scanline_aa<> ras;
			agg::scanline_p8 sl;

			//ras.gamma(agg::gamma_power(m_gamma.value()));

			start_timer();
			draw(rbuf_window(), 0.0/*m_expand.value()*/, m_gamma.value());
			double tm = elapsed_time();

			ras.gamma(agg::gamma_none());
			//agg::render_ctrl(ras, sl, rb, m_expand);
			agg::render_ctrl(ras, sl, rb, m_gamma);
			//agg::render_ctrl(ras, sl, rb, m_scale);
			//agg::render_ctrl(ras, sl, rb, m_rotate);

			char buf[128]; 
			agg::gsv_text t;
			t.size(10.0);
			t.flip(true);

			agg::conv_stroke<agg::gsv_text> pt(t);
			pt.width(1.5);

			//sprintf(buf, "Vertices=%d Time=%.3f ms", vertex_count, tm);
			sprintf(buf, "Time=%.3f ms", tm);

			t.start_point(4.0, 40.0);
			t.text(buf);

			ras.add_path(pt);
			ren.color(agg::rgba(0,0,0));
			agg::render_scanlines(ras, sl, ren);
		}
		catch (agg::svg::exception&)
		{
		}
    }

    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
		if (flags & agg::mouse_left)
			m_current_tool = tool_zoom;

		if (flags & agg::mouse_right) 
			m_current_tool = tool_pan;

		m_x1 = x;
		m_y1 = y;        
		m_x2 = x;
		m_y2 = y;
		m_drag_flag = true;
    }

    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
        if(flags == 0)
        {
            m_drag_flag = false;
        }

		if (m_drag_flag)
		{
			m_x2 = x;
			m_y2 = y;

			if (m_current_tool == tool_zoom)
			{
				force_redraw();
			}
			else if (m_current_tool == tool_pan)
			{
				update_zoom();
				m_x1 = x;
				m_y1 = y;     
				force_redraw();
			}
		}
    }

	void zoom_out(int x, int y)
	{
		int d = 100;

		int x1 = -d;
		int y1 = -d;
		int x2 = rbuf_window().width()  + d;
		int y2 = rbuf_window().height() + d;
	    
		int w = x2 - x1;
		int h = y2 - y1;

		int cx = x1 + w/2;
		int cy = y1 + h/2;

		int dx = x - cx;
		int dy = y - cy;

		m_attributes.set_zoom(x1 + dx, y1 + dy, x2 + dx, y2 + dy);
		force_redraw();
	}

    virtual void on_mouse_button_up(int x, int y, unsigned flags)
    {
		if ((flags & agg::kbd_shift) && m_x1 == x && m_y1 == y)
		{
			zoom_out(x,y);
			m_drag_flag = false;
			m_current_tool = tool_none;
		}

		if (m_drag_flag)
		{
			if (m_current_tool == tool_zoom)
			{
				if(x != m_x1 && y != m_y1)
				{
					m_x2 = x;
					m_y2 = y;
					update_zoom();
				}
				force_redraw();
			}
			m_drag_flag = false;
		}
		m_current_tool = tool_none;
    }

    virtual void on_key(int x, int y, unsigned key, unsigned flags)
    {
		if(key == agg::key_home)
        {
			m_attributes.initial_zoom();			
			force_redraw();
        }
    }
};

int agg_main(int argc, char* argv[])
{
	the_application app(agg::svg::pix_format, flip_y_e);

    const char* fname = "tiger.svg";
    if(argc <= 1)
    {
        FILE* fd = fopen(app.full_file_name(fname), "r");
        if(fd == 0)
        {
            app.message("Usage: svg_test <svg_file>\n"
                        "Download http://antigrain.com/svg/tiger.svg");
            return 1;
        }
        fclose(fd);
    }
    else
    {
        fname = argv[1];
    }

    try
    {
        app.parse_svg(app.full_file_name(fname));
    }
    catch (agg::svg::exception& e)
    {
        app.message(e.msg());
        return 1;
    }

    if(app.init(512, 600, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}



