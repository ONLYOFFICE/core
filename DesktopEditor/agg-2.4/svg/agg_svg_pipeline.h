#ifndef AGG_SVG_PIPELINE_INCLUDED
#define AGG_SVG_PIPELINE_INCLUDED

#include "agg_svg_attributes.h"
#include "agg_svg_transformer.h"
#include "agg_svg_renderer_rgba.h"

#include "agg_conv_curve.h"
#include "agg_conv_transform.h"
#include "agg_conv_stroke.h"
#include "agg_conv_contour.h"


namespace agg
{
namespace svg
{

    //-------------------------------------------------------------------------
    class conv_polymorphic_base
    {
    public:
        virtual ~conv_polymorphic_base() {}
        virtual void     rewind(unsigned path_id) = 0;
        virtual unsigned vertex(double* x, double* y) = 0;
    };
    //-------------------------------------------------------------------------
    template<class VertexSource> 
    class conv_polymorphic_wrapper : public conv_polymorphic_base
    {
    public:
        conv_polymorphic_wrapper(VertexSource& source) : m_source(&source) {}

        virtual void rewind(unsigned path_id)
        {
            m_source->rewind(path_id);
        }

        virtual unsigned vertex(double* x, double* y)
        {
            return m_source->vertex(x, y);
        }

    private:
        VertexSource* m_source;
    };
    //-------------------------------------------------------------------------
    struct fake_vertex_source : public conv_polymorphic_base
    {
    public:
        virtual void rewind(unsigned) {}
        virtual unsigned vertex(double*, double*)
        {
            return agg::path_cmd_stop;
        }
    };

	//-------------------------------------------------------------------------
    class pipeline
    {
    public:
        //-----------------------------------------------------------------------
        typedef agg::conv_curve<conv_polymorphic_base>              conv_curve;
        typedef agg::conv_transform<conv_curve, transformer>        conv_curve_trans;
        typedef agg::conv_stroke<conv_curve>                        conv_curve_stroke;
        typedef agg::conv_transform<conv_curve_stroke, transformer> conv_curve_stroke_trans;
		typedef agg::conv_contour<conv_curve_trans>                 conv_curve_trans_contour;
        //-----------------------------------------------------------------------
        pipeline() 
			:	m_fake_source()
			,	m_identity_transformer()
			,	m_conv_curve(m_fake_source)
			,	m_conv_curve_trans(m_conv_curve, m_identity_transformer)
			,	m_conv_curve_stroke(m_conv_curve)
			,	m_conv_curve_stroke_trans(m_conv_curve_stroke, m_identity_transformer)
			,	m_conv_curve_trans_contour(m_conv_curve_trans)
			,	m_approximation_scale(4.0)
        {
            m_conv_curve_stroke.inner_join(agg::inner_round);
			m_conv_curve_trans_contour.auto_detect_orientation(false);

			m_conv_curve_trans_contour.width(0);
        }
        
        //-----------------------------------------------------------------------
        void approximation_scale(double s) { m_approximation_scale = s; }
        double approximation_scale() const { return m_approximation_scale; }

        //-----------------------------------------------------------------------
#ifdef EXPAND_PATHS
        // Expand all polygons 
        void expand(double value)
        {
            m_conv_curve_trans_contour.width(value);
        }
#endif
        //-----------------------------------------------------------------------
        template<class Source> 
        void render(const attributes& attr, Source& src, renderer_rgba& ren)
        {
            if(!ren.is_valid()) return;
            conv_polymorphic_wrapper<Source> psrc(src);
            m_conv_curve.attach(psrc);
            unsigned cmd;
            double x, y;

            double scale = attr.scale() * m_approximation_scale;
            m_conv_curve.approximation_scale(scale);
            m_conv_curve.angle_tolerance(0.0);
            m_conv_curve_stroke.approximation_scale(scale);

            ren.clip_box(attr.clip_x1(), attr.clip_y1(), attr.clip_x2(), attr.clip_y2());

            if(attr.fill_type() != paint_none)
            {
                ren.reset();
                m_conv_curve_trans.transformer(attr.transform());

                if(fabs(m_conv_curve_trans_contour.width()) < 0.0001)
                {
		            m_conv_curve_trans.rewind(0);
					while(!agg::is_stop(cmd = m_conv_curve_trans.vertex(&x, &y)))
					{
						ren.add_vertex(x, y, cmd);
					}
				}
                else
                {
                    m_conv_curve_trans_contour.miter_limit(attr.stroke_miterlimit());
					m_conv_curve_trans_contour.rewind(0);
					while(!agg::is_stop(cmd = m_conv_curve_trans_contour.vertex(&x, &y)))
					{
						ren.add_vertex(x, y, cmd);
					}
				}

                ren.fill(attr);
            }

            if(attr.stroke_type() != paint_none)
            {
                ren.reset();
                m_conv_curve_stroke_trans.transformer(attr.transform());
                m_conv_curve_stroke.width(attr.stroke_width());
                m_conv_curve_stroke.line_join(g_stroke_linejoin_lut[attr.stroke_linejoin()]);
                m_conv_curve_stroke.line_cap(g_stroke_linecap_lut[attr.stroke_linecap()]);
                m_conv_curve_stroke.miter_limit(attr.stroke_miterlimit());

                if(attr.stroke_width() * scale > 1.0)
                {
                    m_conv_curve.angle_tolerance(agg::deg2rad(10.0));
                }

                m_conv_curve_stroke_trans.rewind(0);
                while(!agg::is_stop(cmd = m_conv_curve_stroke_trans.vertex(&x, &y)))
                {
                    ren.add_vertex(x, y, cmd);
                }
                ren.stroke(attr);
            }
        }


    private:
        //-----------------------------------------------------------------------
        transformer                   m_identity_transformer;
        fake_vertex_source            m_fake_source;
        conv_curve                    m_conv_curve;
        conv_curve_trans              m_conv_curve_trans;
        conv_curve_stroke             m_conv_curve_stroke;
        conv_curve_stroke_trans       m_conv_curve_stroke_trans;
		conv_curve_trans_contour      m_conv_curve_trans_contour;
        double                        m_approximation_scale;
    };


    //---------------------------------------------------------------------------
    class object_bbox
    {
    public:
        //-----------------------------------------------------------------------
        typedef agg::conv_curve<conv_polymorphic_base> conv_curve;


        //-----------------------------------------------------------------------
        object_bbox() : 
            m_fake_source(),
            m_conv_curve(m_fake_source),
            m_minx(1e100),
            m_miny(1e100),
            m_maxx(-1e100),
            m_maxy(-1e100)
        {}

        //-----------------------------------------------------------------------
        void reset()
        {
            m_minx = 1e100;
            m_miny = 1e100;
            m_maxx = -1e100;
            m_maxy = -1e100;
        }

        //-----------------------------------------------------------------------
        template<class Source> void calculate(const attributes& attr, 
                                              Source& src, 
                                              bool reset_flag=true)
        {
            if(reset_flag) reset();

            conv_polymorphic_wrapper<Source> psrc(src);
            m_conv_curve.attach(psrc);
            unsigned cmd;
            double x, y;

            m_conv_curve.approximation_scale(attr.scale());
            m_conv_curve.rewind(0);
            while(!agg::is_stop(cmd = m_conv_curve.vertex(&x, &y)))
            {
                if(agg::is_vertex(cmd))
                {
                    if(x < m_minx) m_minx = x;
                    if(y < m_miny) m_miny = y;
                    if(x > m_maxx) m_maxx = x;
                    if(y > m_maxy) m_maxy = y;
                }
            }
            m_conv_curve.attach(m_fake_source);
        }

        double minx() const { return m_minx; }
        double miny() const { return m_miny; }
        double maxx() const { return m_maxx; }
        double maxy() const { return m_maxy; }
        rectangle bbox() const { return rectangle(m_minx, m_miny, m_maxx, m_maxy); }

    private:
        fake_vertex_source m_fake_source;
        conv_curve         m_conv_curve;
        double m_minx;
        double m_miny;
        double m_maxx;
        double m_maxy;
    };

}
}

#endif


