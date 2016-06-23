#ifndef AGG_SVG_SHAPE_ADAPTORS_INCLUDED
#define AGG_SVG_SHAPE_ADAPTORS_INCLUDED

#include "agg_bezier_arc.h"
#include "agg_svg_basics.h"
#include "agg_svg_path_serializer.h"
#include "agg_svg_path_interpreter.h"

namespace agg
{
namespace svg
{

    //-------------------------------------------------------------------------
    template<class CoordT, unsigned MaxVertices=32> class simple_path_container
    {
    public:
        typedef CoordT coord_type;
        enum { max_vertices = MaxVertices };

        //-----------------------------------------------------------------------
        simple_path_container() :
            m_end_of_buffer(m_buffer + max_vertices * (sizeof(agg::int8u) + sizeof(CoordT) * 2)),
            m_end_of_data(m_buffer),
            m_vertex(m_buffer)
        {}

        //-----------------------------------------------------------------------
        void remove_all()
        {
            m_end_of_data = m_buffer;
            m_vertex      = m_buffer;
        }

        //-----------------------------------------------------------------------
        bool add_vertex(double x, double y, unsigned cmd)
        {
            if((m_end_of_data + sizeof(agg::int8u) + sizeof(CoordT) * 2) <= m_end_of_buffer)
            {
                CoordT tx = (CoordT)x;
                CoordT ty = (CoordT)y;
                *m_end_of_data++ = (agg::int8u)cmd;
                memcpy(m_end_of_data, &tx, sizeof(CoordT));
                m_end_of_data += sizeof(CoordT);
                memcpy(m_end_of_data, &ty, sizeof(CoordT));
                m_end_of_data += sizeof(CoordT);
                return true;
            }
            return false;
        }

        //-----------------------------------------------------------------------
        void move_to(double x, double y)
        {
            add_vertex(x, y, agg::path_cmd_move_to);
        }

        //-----------------------------------------------------------------------
        void line_to(double x, double y)
        {
            add_vertex(x, y, agg::path_cmd_line_to);
        }

        //-----------------------------------------------------------------------
        void curve3_to(double x1, double y1, double x, double y)
        {
            add_vertex(x1, y1, agg::path_cmd_curve3);
            add_vertex(x , y,  agg::path_cmd_curve3);
        }

        //-----------------------------------------------------------------------
        void curve4_to(double x1, double y1, 
                       double x2, double y2, 
                       double x,  double y)
        {
            add_vertex(x1, y1, agg::path_cmd_curve4);
            add_vertex(x2, y2, agg::path_cmd_curve4);
            add_vertex(x , y,  agg::path_cmd_curve4);
        }

        //-----------------------------------------------------------------------
        void close_subpath()
        {
            add_vertex(0.0, 0.0, agg::path_cmd_end_poly | agg::path_flags_close);
        }

        //-----------------------------------------------------------------------
        template<class VertexSource> void add_path(VertexSource& vs, unsigned path_id=0)
        {
            double x;
            double y;
            unsigned cmd;
            vs.rewind(path_id);
            while(!agg::is_stop(cmd = vs.vertex(&x, &y)))
            {
                if(!add_vertex(x, y, cmd)) break;
            }
        }

        //-----------------------------------------------------------------------
        template<class VertexSource> void add_path_solid(VertexSource& vs, unsigned path_id=0)
        {
            double x;
            double y;
            unsigned cmd;
            vs.rewind(path_id);
            while(agg::is_move_to(cmd = vs.vertex(&x, &y)));
            if(!agg::is_stop(cmd))
            {
                if(add_vertex(x, y, cmd))
                {
                    while(!agg::is_stop(cmd = vs.vertex(&x, &y)))
                    {
                        if(!add_vertex(x, y, cmd)) break;
                    }
                }
            }
        }

        //-----------------------------------------------------------------------
        void rewind(unsigned)
        {
            m_vertex = m_buffer;
        }

        //-----------------------------------------------------------------------
        unsigned vertex(double* x, double* y)
        {
            if(m_vertex >= m_end_of_data) return agg::path_cmd_stop;
            CoordT tx;
            CoordT ty;
            unsigned cmd = *m_vertex++;
            memcpy(&tx, m_vertex, sizeof(CoordT));
            m_vertex += sizeof(CoordT);
            memcpy(&ty, m_vertex, sizeof(CoordT));
            m_vertex += sizeof(CoordT);
            *x = (double)tx;
            *y = (double)ty;
            return cmd;
        }

    private:
        agg::int8u        m_buffer[max_vertices * (sizeof(agg::int8u) + sizeof(CoordT) * 2)];
        agg::int8u*       m_end_of_buffer;
        agg::int8u*       m_end_of_data;
        const agg::int8u* m_vertex;
    };

    //-------------------------------------------------------------------------
    class line_adaptor
    {
    public:
        line_adaptor() {}
        line_adaptor(double x1, double y1, double x2, double y2)
        {
            init(x1, y1, x2, y2);
        }
        
        void init(double x1, double y1, double x2, double y2)
        {
            m_path.remove_all();
            m_path.move_to(x1, y1);
            m_path.line_to(x2, y2);
        }

        //-----------------------------------------------------------------------
        void rewind(unsigned) { m_path.rewind(0); }
        unsigned vertex(double* x, double* y) { return m_path.vertex(x, y); }

    private:
        simple_path_container<coord_type, 2> m_path;
    };

    //-------------------------------------------------------------------------
    class rectangle_adaptor
    {
    public:
        rectangle_adaptor() {}
        rectangle_adaptor(double x, double y, double w, double h, double rx=0, double ry=0)
        {
            init(x, y, w, h, rx, ry);
        }

        void init(double x, double y, double w, double h, double rx=0, double ry=0)
        {
            m_path.remove_all();
            if(w < 0.0) return;
            if(h < 0.0) return;
            double x1 = x;
            double y1 = y;
            double x2 = x + w;
            double y2 = y + h;
            if(rx == 0.0) rx = ry;
            if(ry == 0.0) ry = rx;
            if(rx == 0.0 || ry == 0.0)
            {
                m_path.move_to(x1, y1);
                m_path.line_to(x2, y1);
                m_path.line_to(x2, y2);
                m_path.line_to(x1, y2);
                m_path.close_subpath();
            }
            else
            {
                double dx = fabs(y2 - y1);
                double dy = fabs(x2 - x1);

                double k = 1.0;
                double t;
                t = dx / (rx * 2.0); if(t < k) k = t; 
                t = dy / (ry * 2.0); if(t < k) k = t; 

                if(k < 1.0)
                {
                    rx *= k; 
                    ry *= k;
                }
                agg::bezier_arc a;
                m_path.move_to(x1 + rx, y1);
                m_path.line_to(x2 - rx, y1);
                a.init(x2 - rx, y1 + ry, rx, ry, -agg::pi/2.0, agg::pi/2.0);
                m_path.add_path_solid(a);
                m_path.line_to(x2, y2 - ry);
                a.init(x2 - rx, y2 - ry, rx, ry, 0.0, agg::pi/2.0);
                m_path.add_path_solid(a);
                m_path.line_to(x1 + rx, y2);
                a.init(x1 + rx, y2 - ry, rx, ry, agg::pi/2.0, agg::pi/2.0);
                m_path.add_path_solid(a);
                m_path.line_to(x1, y1 + ry);
                a.init(x1 + rx, y1 + ry, rx, ry, agg::pi, agg::pi/2.0);
                m_path.add_path_solid(a);
                m_path.close_subpath();
            }
        }

        //------------------------------------------------------------------------
        void rewind(unsigned) { m_path.rewind(0); }
        unsigned vertex(double* x, double* y) { return m_path.vertex(x, y); }

    private:
        simple_path_container<coord_type, 30> m_path;
    };

    //-------------------------------------------------------------------------
    class circle_adaptor
    {
    public:
        circle_adaptor() {}
        circle_adaptor(double cx, double cy, double r) 
        {
            init(cx, cy, r);
        }

        void init(double cx, double cy, double r)
        {
            m_path.remove_all();
            if(r > 0.0)
            {
                agg::bezier_arc a(cx, cy, r, r, 0, 2.0*agg::pi);
                m_path.add_path(a);
                m_path.close_subpath();
            }
        }

        //------------------------------------------------------------------------
        void rewind(unsigned) { m_path.rewind(0); }
        unsigned vertex(double* x, double* y) { return m_path.vertex(x, y); }

    private:
        simple_path_container<coord_type, 20> m_path;
    };

    //-------------------------------------------------------------------------
    class ellipse_adaptor
    {
    public:
        ellipse_adaptor() {}
        ellipse_adaptor(double cx, double cy, double rx, double ry) 
        {
            init(cx, cy, rx, ry);
        }

        void init(double cx, double cy, double rx, double ry)
        {
            m_path.remove_all();
            if(rx > 0.0 && ry > 0.0)
            {
                agg::bezier_arc a(cx, cy, rx, ry, 0, 2.0*agg::pi);
                m_path.add_path(a);
                m_path.close_subpath();
            }
        }

        //------------------------------------------------------------------------
        void rewind(unsigned) { m_path.rewind(0); }
        unsigned vertex(double* x, double* y) { return m_path.vertex(x, y); }

    private:
        simple_path_container<coord_type, 20> m_path;
    };

}
}

#endif
