#ifndef AGG_SVG_PATH_INTERPRETER_INCLUDED
#define AGG_SVG_PATH_INTERPRETER_INCLUDED

#include "agg_bezier_arc.h"
#include "agg_math.h"
#include "agg_svg_basics.h"

namespace agg
{
namespace svg
{

    //-------------------------------------------------------------------------
    // Use this class to interprete the serialized path. 
    //
    //---------------------------------------------------------------------------
    template<class DataAccessor> class path_interpreter
    {
    public:
        typedef DataAccessor data_accessor_type;
        typedef typename data_accessor_type::value_type value_type;

        path_interpreter();
        path_interpreter(const data_accessor_type& data);

        void init(const data_accessor_type& data);

        void     rewind(unsigned);
        unsigned vertex(double* x, double* y);
        
        unsigned num_vertices() const { return m_num_vertices; }

    private:
        void rel_to_abs(double *x, double *y) const
        {
            if(m_num_vertices > 1)
            {
                *x += m_vertices[m_num_vertices - 2];
                *y += m_vertices[m_num_vertices - 1];
            }
        }

        unsigned last_command() const
        {
            if(m_last_command >= 'A' && m_last_command <= 'z')
            {
                return m_commands[m_last_command - 'A'];
            }
            return agg::path_cmd_stop;
        }

        void move_to(unsigned cmd);
        void line_to(unsigned cmd);
        void hline_to(unsigned cmd);
        void vline_to(unsigned cmd);
        void curve3_to(unsigned cmd);
        void curve3_to_ref(unsigned cmd);
        void curve4_to(unsigned cmd);
        void curve4_to_ref(unsigned cmd);
        void arc_to(unsigned cmd);
        void close_subpath(unsigned cmd);

        typedef void (path_interpreter<DataAccessor>::*command_handler)(unsigned cmd);

        static const agg::int8u      m_commands[58];
        static const command_handler m_command_handlers[58];

        data_accessor_type m_accessor;
        data_accessor_type m_data;
        double             m_vertices[26];
        unsigned           m_num_vertices;
        unsigned           m_vertex;
        double             m_start_x;
        double             m_start_y;
        unsigned           m_last_command;
    };



    //------------------------------------------------------------------------
    template<class DA>
    const agg::int8u path_interpreter<DA>::m_commands[58] = 
    {
         agg::path_cmd_curve4,  //065 A
         0,                     //066 B
         agg::path_cmd_curve4,  //067 C
         0,                     //068 D
         0,                     //069 E
         0,                     //070 F
         0,                     //071 G
         agg::path_cmd_line_to, //072 H
         0,                     //073 I
         0,                     //074 J
         0,                     //075 K
         agg::path_cmd_line_to, //076 L
         agg::path_cmd_move_to, //077 M
         0,                     //078 N
         0,                     //079 O
         0,                     //080 P
         agg::path_cmd_curve3,  //081 Q
         0,                     //082 R
         agg::path_cmd_curve4,  //083 S
         agg::path_cmd_curve3,  //084 T
         0,                     //085 U
         agg::path_cmd_line_to, //086 V
         0,                     //087 W
         0,                     //088 X
         0,                     //089 Y
         agg::path_cmd_end_poly | agg::path_flags_close,  //090 Z
         0,                     //091 [
         0,                     //092 backslash
         0,                     //093 ]
         0,                     //094 ^
         0,                     //095 _
         0,                     //096 '
         agg::path_cmd_curve4,  //097 a
         0,                     //098 b
         agg::path_cmd_curve4,  //099 c
         0,                     //100 d
         0,                     //101 e
         0,                     //102 f
         0,                     //103 g
         agg::path_cmd_line_to, //104 h
         0,                     //105 i
         0,                     //106 j
         0,                     //107 k
         agg::path_cmd_line_to, //108 l
         agg::path_cmd_move_to, //109 m
         0,                     //110 n
         0,                     //111 o
         0,                     //112 p
         agg::path_cmd_curve3,  //113 q
         0,                     //114 r
         agg::path_cmd_curve4,  //115 s
         agg::path_cmd_curve3,  //116 t
         0,                     //117 u
         agg::path_cmd_line_to, //118 v
         0,                     //119 w
         0,                     //120 x
         0,                     //121 y
         agg::path_cmd_end_poly | agg::path_flags_close  //122 z
    };

    //------------------------------------------------------------------------
    template<class DA>
    path_interpreter<DA>::path_interpreter() : 
        m_accessor(),
        m_data(),
        m_num_vertices(0),
        m_vertex(0),
        m_start_x(0.0),
        m_start_y(0.0),
        m_last_command(0)
    {
    }

    //------------------------------------------------------------------------
    template<class DA>
    path_interpreter<DA>::path_interpreter(const data_accessor_type& data) :
        m_accessor(data),
        m_data(data),
        m_num_vertices(0),
        m_vertex(0),
        m_start_x(0.0),
        m_start_y(0.0),
        m_last_command(0)
    {
    }

    //------------------------------------------------------------------------
    template<class DA>
    void path_interpreter<DA>::init(const data_accessor_type& data)
    {
        m_accessor = data;
        m_data = data;
        m_num_vertices = 0;
        m_vertex = 0;
        m_start_x = 0.0;
        m_start_y = 0.0;
        m_last_command = 0;
    }

    //------------------------------------------------------------------------
    template<class DA>
    void path_interpreter<DA>::move_to(unsigned cmd)
    {
        double x = m_data.read_coord();
        double y = m_data.read_coord();
        if(cmd == 'm')
        {
            rel_to_abs(&x, &y);
        }
        m_start_x = x;
        m_start_y = y;
        m_vertices[0]  = x;
        m_vertices[1]  = y;
        m_num_vertices = 2;
        m_last_command = cmd;
    }

    //------------------------------------------------------------------------
    template<class DA>
    void path_interpreter<DA>::line_to(unsigned cmd)
    {
        double x = m_data.read_coord();
        double y = m_data.read_coord();
        if(cmd == 'l')
        {
            rel_to_abs(&x, &y);
        }
        m_vertices[0]  = x;
        m_vertices[1]  = y;
        m_num_vertices = 2;
        m_last_command = cmd;
    }

    //------------------------------------------------------------------------
    template<class DA>
    void path_interpreter<DA>::hline_to(unsigned cmd)
    {
        double x = m_data.read_coord();
        double y = 0.0;
        if(m_num_vertices > 1)
        {
            if(cmd == 'h')
            {
                x += m_vertices[m_num_vertices - 2];
            }
            y = m_vertices[m_num_vertices - 1];
        }
        m_vertices[0]  = x;
        m_vertices[1]  = y;
        m_num_vertices = 2;
        m_last_command = cmd;
    }

    //------------------------------------------------------------------------
    template<class DA>
    void path_interpreter<DA>::vline_to(unsigned cmd)
    {
        double x = 0.0;
        double y = m_data.read_coord();
        if(m_num_vertices > 1)
        {
            x = m_vertices[m_num_vertices - 2];
            if(cmd == 'v')
            {
                y += m_vertices[m_num_vertices - 1];
            }
        }
        m_vertices[0]  = x;
        m_vertices[1]  = y;
        m_num_vertices = 2;
        m_last_command = cmd;
    }

    //------------------------------------------------------------------------
    template<class DA>
    void path_interpreter<DA>::curve3_to(unsigned cmd)
    {
        double x1 = m_data.read_coord();
        double y1 = m_data.read_coord();
        double x  = m_data.read_coord();
        double y  = m_data.read_coord();
        if(cmd == 'q')
        {
            rel_to_abs(&x1, &y1);
            rel_to_abs(&x,  &y);
        }
        m_vertices[0]  = x1;
        m_vertices[1]  = y1;
        m_vertices[2]  = x;
        m_vertices[3]  = y;
        m_num_vertices = 4;
        m_last_command = cmd;
    }

    //------------------------------------------------------------------------
    template<class DA>
    void path_interpreter<DA>::curve3_to_ref(unsigned cmd)
    {
        double x1 = 0.0;
        double y1 = 0.0;
        double x  = m_data.read_coord();
        double y  = m_data.read_coord();
        rel_to_abs(&x1, &y1); 

        if(cmd == 't')
        {
            rel_to_abs(&x, &y);
        }
        if(agg::is_curve(last_command()) && m_num_vertices > 3)
        {
            x1 += x1 - m_vertices[m_num_vertices - 4];
            y1 += y1 - m_vertices[m_num_vertices - 3];
        }
        m_vertices[0]  = x1;
        m_vertices[1]  = y1;
        m_vertices[2]  = x;
        m_vertices[3]  = y;
        m_num_vertices = 4;
        m_last_command = cmd;
    }

    //------------------------------------------------------------------------
    template<class DA>
    void path_interpreter<DA>::curve4_to(unsigned cmd)
    {
        double x1 = m_data.read_coord();
        double y1 = m_data.read_coord();
        double x2 = m_data.read_coord();
        double y2 = m_data.read_coord();
        double x  = m_data.read_coord();
        double y  = m_data.read_coord();
        if(cmd == 'c')
        {
            rel_to_abs(&x1, &y1);
            rel_to_abs(&x2, &y2);
            rel_to_abs(&x,  &y);
        }
        m_vertices[0]  = x1;
        m_vertices[1]  = y1;
        m_vertices[2]  = x2;
        m_vertices[3]  = y2;
        m_vertices[4]  = x;
        m_vertices[5]  = y;
        m_num_vertices = 6;
        m_last_command = cmd;
    }

    //------------------------------------------------------------------------
    template<class DA>
    void path_interpreter<DA>::curve4_to_ref(unsigned cmd)
    {
        double x1 = 0.0;
        double y1 = 0.0;
        double x2 = m_data.read_coord();
        double y2 = m_data.read_coord();
        double x  = m_data.read_coord();
        double y  = m_data.read_coord();
        rel_to_abs(&x1, &y1); 

        if(cmd == 's')
        {
            rel_to_abs(&x,  &y);
            rel_to_abs(&x2, &y2);
        }
        if(agg::is_curve(last_command()) && m_num_vertices > 3)
        {
            x1 += x1 - m_vertices[m_num_vertices - 4];
            y1 += y1 - m_vertices[m_num_vertices - 3];
        }
        m_vertices[0]  = x1;
        m_vertices[1]  = y1;
        m_vertices[2]  = x2;
        m_vertices[3]  = y2;
        m_vertices[4]  = x;
        m_vertices[5]  = y;
        m_num_vertices = 6;
        m_last_command = cmd;
    }

    //------------------------------------------------------------------------
    template<class DA>
    void path_interpreter<DA>::arc_to(unsigned cmd)
    {
        const double epsilon = 1e-30;

        double x0              = 0.0;
        double y0              = 0.0;
        double rx              = m_data.read_coord();
        double ry              = m_data.read_coord();
        double x_axis_rotation = m_data.read_coord();
        bool   large_arc_flag  = m_data.read_int8u() != '0';
        bool   sweep_flag      = m_data.read_int8u() != '0';
        double x               = m_data.read_coord();
        double y               = m_data.read_coord();

        if(m_num_vertices > 1)
        {
            x0 = m_vertices[m_num_vertices - 2];
            y0 = m_vertices[m_num_vertices - 1];
        }
        if(cmd == 'a')
        {
            rel_to_abs(&x, &y);
        }

        rx = fabs(rx);
        ry = fabs(ry);

        if(rx < epsilon || ry < epsilon) 
        {
            m_vertices[0]  = x;
            m_vertices[1]  = y;
            m_num_vertices = 2;
            m_last_command = (cmd == 'a') ? 'l' : 'L';
            return;
        }

        if(agg::calc_distance(x0, y0, x, y) < epsilon)
        {
            return;
        }
        agg::bezier_arc_svg a(x0, y0, 
                              rx, ry, 
                              x_axis_rotation, 
                              large_arc_flag, sweep_flag, 
                              x, y);
        if(a.radii_ok())
        {
            for(unsigned i = 2; i < a.num_vertices(); i += 2)
            {
                m_vertices[i - 2] = a.vertices()[i];
                m_vertices[i - 1] = a.vertices()[i + 1];
            }
            m_num_vertices = a.num_vertices() - 2;
            m_last_command = cmd;
        }
        else
        {
            m_vertices[0]  = x;
            m_vertices[1]  = y;
            m_num_vertices = 2;
            m_last_command = (cmd == 'a') ? 'l' : 'L';
        }
    }

    //------------------------------------------------------------------------
    template<class DA>
    void path_interpreter<DA>::close_subpath(unsigned cmd)
    {
        m_vertices[0]  = m_start_x;
        m_vertices[1]  = m_start_y;
        m_num_vertices = 2;
        m_vertex       = 0;
        m_last_command = cmd;
    }

    //------------------------------------------------------------------------
    template<class DA>
    const typename path_interpreter<DA>::command_handler 
    path_interpreter<DA>::m_command_handlers[58] = 
    {
         &path_interpreter<DA>::arc_to,        //065 A
         0,                                    //066 B
         &path_interpreter<DA>::curve4_to,     //067 C
         0,                                    //068 D
         0,                                    //069 E
         0,                                    //070 F
         0,                                    //071 G
         &path_interpreter<DA>::hline_to,      //072 H
         0,                                    //073 I
         0,                                    //074 J
         0,                                    //075 K
         &path_interpreter<DA>::line_to,       //076 L
         &path_interpreter<DA>::move_to,       //077 M
         0,                                    //078 N
         0,                                    //079 O
         0,                                    //080 P
         &path_interpreter<DA>::curve3_to,     //081 Q
         0,                                    //082 R
         &path_interpreter<DA>::curve4_to_ref, //083 S
         &path_interpreter<DA>::curve3_to_ref, //084 T
         0,                                    //085 U
         &path_interpreter<DA>::vline_to,      //086 V
         0,                                    //087 W
         0,                                    //088 X
         0,                                    //089 Y
         &path_interpreter<DA>::close_subpath, //090 Z
         0,                                    //091 [
         0,                                    //092 backslash
         0,                                    //093 ]
         0,                                    //094 ^
         0,                                    //095 _
         0,                                    //096 '
         &path_interpreter<DA>::arc_to,        //097 a
         0,                                    //098 b
         &path_interpreter<DA>::curve4_to,     //099 c
         0,                                    //100 d
         0,                                    //101 e
         0,                                    //102 f
         0,                                    //103 g
         &path_interpreter<DA>::hline_to,      //104 h
         0,                                    //105 i
         0,                                    //106 j
         0,                                    //107 k
         &path_interpreter<DA>::line_to,       //108 l
         &path_interpreter<DA>::move_to,       //109 m
         0,                                    //110 n
         0,                                    //111 o
         0,                                    //112 p
         &path_interpreter<DA>::curve3_to,     //113 q
         0,                                    //114 r
         &path_interpreter<DA>::curve4_to_ref, //115 s
         &path_interpreter<DA>::curve3_to_ref, //116 t
         0,                                    //117 u
         &path_interpreter<DA>::vline_to,      //118 v
         0,                                    //119 w
         0,                                    //120 x
         0,                                    //121 y
         &path_interpreter<DA>::close_subpath  //122 z
    };


    // AGG Vertex Source interface functions
    //------------------------------------------------------------------------
    template<class DA>
    void path_interpreter<DA>::rewind(unsigned)
    {
        m_data = m_accessor;
        m_num_vertices = 0;
        m_vertex = 0;
        m_start_x = 0.0;
        m_start_y = 0.0;
        m_last_command = 0;
    }

    //------------------------------------------------------------------------
    template<class DA>
    unsigned path_interpreter<DA>::vertex(double* x, double* y)
    {
        for(;;)
        {
            if(m_vertex < m_num_vertices)
            {
                *x = m_vertices[m_vertex++];
                *y = m_vertices[m_vertex++];
                break;
            }

            if(m_data.size() == 0)
            {
                *x = *y = 0.0;
                return agg::path_cmd_stop;
            }

            unsigned cmd = m_data.read_int8u();

            //----------------------------
            command_handler ch;
            if(cmd >= 'A' && cmd <= 'z' && 
              (ch = m_command_handlers[cmd - 'A']) != 0)
            {
                (this->*ch)(cmd);
            }
            else
            {
                m_last_command = 0;
                return agg::path_cmd_stop;
            }
            m_vertex = 0;
        }
        return last_command();
    }





    //-------------------------------------------------------------------------
    // Similar to path_interpreter.
    // 
    // Use flag "closed=false" for polylines and "closed=true" for polygons.
    //
    //-------------------------------------------------------------------------
    template<class DataAccessor> class poly_interpreter
    {
    public:
        typedef DataAccessor data_accessor_type;
        typedef typename data_accessor_type::value_type value_type;

        poly_interpreter() : 
            m_accessor(), 
            m_data(), 
            m_closed(false), 
            m_stop(false)
        {}

        poly_interpreter(const data_accessor_type& data, bool closed) :
            m_accessor(data),
            m_data(data),
            m_closed(closed),
            m_stop(false)
        {}

        void init(const data_accessor_type& data, bool closed)
        {
            m_accessor = data;
            m_data = data;
            m_closed = closed;
            m_stop = false;
        }

        void rewind(unsigned)
        {
            m_data = m_accessor;
            m_stop = false;
        }

        unsigned vertex(double* x, double* y)
        {
            if(m_data.size())
            {
                bool first = m_data.size() == m_accessor.size();
                *x = m_data.read_coord();
                *y = m_data.read_coord();
                return first ? agg::path_cmd_move_to : agg::path_cmd_line_to;
            }
            *x = *y = 0.0;
            if(m_closed && !m_stop)
            {
                m_stop = true;
                return agg::path_cmd_end_poly | agg::path_flags_close;
            }
            return agg::path_cmd_stop;
        }

    private:
        data_accessor_type m_accessor;
        data_accessor_type m_data;
        bool               m_closed;
        bool               m_stop;
    };

}
}


#endif
