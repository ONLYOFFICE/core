#ifndef AGG_SVG_PATH_SERIALIZER_INCLUDED
#define AGG_SVG_PATH_SERIALIZER_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "agg_array.h"
#include "agg_svg_basics.h"
#include "agg_svg_parse_real.h"


namespace agg
{
namespace svg
{

    //-------------------------------------------------------------------------
    // The tokenizer does all the routine job of parsing the SVG paths.
    //----------------------------------------------------------------------------
    template<class CharT=char> class path_tokenizer
    {
     public:
        typedef CharT value_type;

        path_tokenizer();

        void path_str(const value_type* str);
        bool rewind();
        bool token();

        double token(value_type cmd);

        value_type last_command() const { return m_last_command; }
        double     last_number()  const { return m_last_number;  }
        bool       multiple_coordinate_pair() const
        {
            return (m_num_count > 2) && (m_num_count & 1) == 0;
        }

    private:
        static void init_mask(agg::int8u* mask, const agg::int8u* char_set);

        bool contains(const agg::int8u* mask, unsigned c) const
        {
            return (mask[(c >> 3) & (256/8-1)] & (1 << (c & 7))) != 0;
        }

        bool is_command(unsigned c) const
        {
            return contains(m_commands_mask, c);
        }

        bool is_numeric(unsigned c) const
        {
            return contains(m_numeric_mask, c);
        }

        bool is_separator(unsigned c) const
        {
            return contains(m_separators_mask, c);
        }

        bool parse_number();

        agg::int8u m_separators_mask[256/8];
        agg::int8u m_commands_mask[256/8];
        agg::int8u m_numeric_mask[256/8];

        const value_type* m_str;
        const value_type* m_ptr;
        double            m_last_number;
        unsigned          m_num_count;
        value_type        m_last_command;


        static const agg::int8u s_commands[];
        static const agg::int8u s_numeric[];
        static const agg::int8u s_separators[];
    };


    //------------------------------------------------------------------------
    template<class CharT> 
    const agg::int8u path_tokenizer<CharT>::s_commands[] = "+-MmZzLlHhVvCcSsQqTtAa";

    template<class CharT> 
    const agg::int8u path_tokenizer<CharT>::s_numeric[] = ".0123456789";

    template<class CharT> 
    const agg::int8u path_tokenizer<CharT>::s_separators[] = " ,\t\n\r";

    //------------------------------------------------------------------------
    template<class CharT> 
    path_tokenizer<CharT>::path_tokenizer() : 
        m_str(0), m_ptr(0), m_last_number(0.0), m_num_count(0), m_last_command(0)
    {
        init_mask(m_commands_mask,   s_commands);
        init_mask(m_numeric_mask,    s_numeric);
        init_mask(m_separators_mask, s_separators);
    }

    //------------------------------------------------------------------------
    template<class CharT> 
    void path_tokenizer<CharT>::path_str(const CharT* str)
    {
        m_ptr = m_str = str;
        m_last_command = 0;
        m_num_count = 0;
        m_last_number = 0.0;
    }

    //------------------------------------------------------------------------
    template<class CharT> 
    void path_tokenizer<CharT>::init_mask(agg::int8u* mask, const agg::int8u* char_set)
    {
        memset(mask, 0, 256/8);
        while(*char_set) 
        {
            unsigned c = unsigned(*char_set++) & 0xFF;
            mask[c >> 3] |= 1 << (c & 7);
        }
    }

    //------------------------------------------------------------------------
    template<class CharT> 
    bool path_tokenizer<CharT>::rewind()
    {
        m_ptr          = m_str;
        m_last_command = 0;
        m_num_count    = 0;
        m_last_number  = 0.0;
        return m_str != 0;
    }

    //------------------------------------------------------------------------
    template<class CharT> 
    bool path_tokenizer<CharT>::token()
    {
        if(m_ptr == 0) return false;

        // Skip all white spaces and other garbage
        while(*m_ptr && !is_command(*m_ptr) && !is_numeric(*m_ptr)) 
        {
            if(!is_separator(*m_ptr))
            {
				throw exception("Parsing Path Structure: Invalid Character %c", *m_ptr);
            }
            m_ptr++;
        }

        if(*m_ptr == 0) return false;

        if(is_command(*m_ptr))
        {
            // Check if the command is a numeric sign character
            if(*m_ptr == '-' || *m_ptr == '+')
            {
                return parse_number();
            }
            m_last_command = *m_ptr++;
            m_num_count = 0;
            while(*m_ptr && is_separator(*m_ptr)) m_ptr++;
            if(*m_ptr == 0) return true;
        }
        return parse_number();
    }

    //------------------------------------------------------------------------
    template<class CharT> 
    double path_tokenizer<CharT>::token(CharT cmd)
    {
        if(!token()) 
		{
			throw exception("Parsing Path Structure: Unexpected end of Path");
        }
		if(last_command() != cmd)
        {
			throw exception("Parsing Path Structure: Command %c is bad or missing parameters", cmd);
        }
        return last_number();
    }
    //------------------------------------------------------------------------
    template<class CharT> 
    bool path_tokenizer<CharT>::parse_number()
    {
        char buf[256]; // Should be enough for any number
        char* buf_ptr = buf;

        // Copy all sign characters
        while(buf_ptr < buf+255 && *m_ptr == '-' || *m_ptr == '+')
        {
            *buf_ptr++ = char(*m_ptr++);
        }

        // Copy all numeric characters
        while(buf_ptr < buf+255 && is_numeric(*m_ptr))
        {
            *buf_ptr++ = char(*m_ptr++);
        }

		// check for exponent
		if (*m_ptr == 'e' || *m_ptr == 'E')
		{
			*buf_ptr++ = char(*m_ptr++);
			// Copy all sign characters
			while(buf_ptr < buf+255 && *m_ptr == '-' || *m_ptr == '+')
			{
				*buf_ptr++ = char(*m_ptr++);
			}

			// Copy all numeric characters
			while(buf_ptr < buf+255 && is_numeric(*m_ptr))
			{
				*buf_ptr++ = char(*m_ptr++);
			}
		}

        *buf_ptr = 0;

        const char*  endptr = 0;
        parse_real   pd;

        pd.parse(buf, buf + strlen(buf), &m_last_number, &endptr);
        ++m_num_count;
        return true;
    }

    //-------------------------------------------------------------------------
    // Ths class is used to parse, validate, and serialize paths into binary format.
	//
    //----------------------------------------------------------------------------
    template<class Container> class path_serializer
    {
    public:
        typedef Container container_type;

        path_serializer(container_type& buffer);

        // Make path
        //------------------------------------------------------------------------
        void reset();
        void move_to(double x, double y, bool rel=false);
        void line_to(double x, double y, bool rel=false);
        void hline_to(double x, bool rel=false);
        void vline_to(double y, bool rel=false);
        void curve3_to(double x1, double y1, double x, double y, bool rel=false);
        void curve3_to(double x,  double y, bool rel=false);

        void curve4_to(double x1, double y1, double x2, double y2, 
                       double x,  double y,  bool rel=false);

        void curve4_to(double x2, double y2, double x, double y, bool rel=false);

        void arc_to(double rx, double ry, double x_axis_rotation,
                    bool large_arc_flag, bool sweep_flag,
                    double x, double y, bool rel=false);
        void close_subpath();

        // Parse path string.
        //------------------------------------------------------------------------
        template<class Tokenizer> void parse(Tokenizer& tok)
        {
            reset();
            if(!tok.rewind()) return;
            while(tok.token())
            {
                double arg[10];
                unsigned cmd = tok.last_command();
                unsigned i;
                switch(cmd)
                {
                    case 'M': case 'm':
                        arg[0] = tok.last_number();
                        arg[1] = tok.token(cmd);
                        if(tok.multiple_coordinate_pair())
                        {
                            line_to(arg[0], arg[1], cmd == 'm');
                        }
                        else
                        {
                            move_to(arg[0], arg[1], cmd == 'm');
                        }
                        break;

                    case 'L': case 'l':
                        arg[0] = tok.last_number();
                        arg[1] = tok.token(cmd);
                        line_to(arg[0], arg[1], cmd == 'l');
                        break;

                    case 'V': case 'v':
                        vline_to(tok.last_number(), cmd == 'v');
                        break;

                    case 'H': case 'h':
                        hline_to(tok.last_number(), cmd == 'h');
                        break;
                
                    case 'Q': case 'q':
                        arg[0] = tok.last_number();
                        for(i = 1; i < 4; i++)
                        {
                            arg[i] = tok.token(cmd);
                        }
                        curve3_to(arg[0], arg[1], arg[2], arg[3], cmd == 'q');
                        break;

                    case 'T': case 't':
                        arg[0] = tok.last_number();
                        arg[1] = tok.token(cmd);
                        curve3_to(arg[0], arg[1], cmd == 't');
                        break;

                    case 'C': case 'c':
                        arg[0] = tok.last_number();
                        for(i = 1; i < 6; i++)
                        {
                            arg[i] = tok.token(cmd);
                        }
                        curve4_to(arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], cmd == 'c');
                        break;

                    case 'S': case 's':
                        arg[0] = tok.last_number();
                        for(i = 1; i < 4; i++)
                        {
                            arg[i] = tok.token(cmd);
                        }
                        curve4_to(arg[0], arg[1], arg[2], arg[3], cmd == 's');
                        break;

                    case 'A': case 'a':
                        arg[0] = tok.last_number();
                        for(i = 1; i < 7; i++)
                        {
                            arg[i] = tok.token(cmd);
                        }
                        arc_to(arg[0], arg[1], agg::deg2rad(arg[2]), 
                               arg[3] > 0.1, arg[4] > 0.1, 
                               arg[5], arg[6], cmd == 'a');
                        break;

                    case 'Z': case 'z':
                        close_subpath();
                        break;

                    default:
                    {
						throw exception("Parsing Path Structure: Invalid Command %c", cmd);
                    }
                }
            }
        }

    private:
        void check_last_vertex(unsigned cmd);
        void check_curve_reflection(unsigned cmd);

        //------------------------------------------------------------------------
        void append_c(unsigned chr)
        {
            m_buffer->add((agg::int8u)chr);
        }

        void append_d(double val)
        {
            coord_type f = (coord_type)val;
            const agg::int8u* p = (const agg::int8u*)&f;
            unsigned i;
            for(i = 0; i < sizeof(coord_type); i++)
            {
                m_buffer->add(*p++);
            }
        }

        //------------------------------------------------------------------------
        container_type* m_buffer;
        unsigned        m_last_command;
        unsigned        m_num_vertices;
    };



    //------------------------------------------------------------------------
    template<class Container>
    path_serializer<Container>::path_serializer(container_type& buffer) : 
        m_buffer(&buffer),
        m_last_command(0),
        m_num_vertices(0)
    {}

    //------------------------------------------------------------------------
    template<class Container>
    void path_serializer<Container>::check_last_vertex(unsigned cmd)
    {
        if(m_num_vertices == 0)
        {
			throw exception("Parsing Path Structure: Invalid use of command '%c' - Path must begin with 'M' or 'm'", cmd);
        }
    }

    //------------------------------------------------------------------------
    template<class Container>
    void path_serializer<Container>::check_curve_reflection(unsigned cmd)
    {
        if(m_last_command == 'C' || m_last_command == 'c' ||
           m_last_command == 'S' || m_last_command == 's' ||
           m_last_command == 'Q' || m_last_command == 'q' ||
           m_last_command == 'T' || m_last_command == 't')
        {
            return;
        }

		throw exception("Parsing Path Structure: Invalid use of reflection in command '%c' - The command must be followed by a curve command", cmd);
    }

    //------------------------------------------------------------------------
    template<class Container>
    void path_serializer<Container>::reset()
    {
        m_last_command = 0;
        m_num_vertices = 0;
    }

    //------------------------------------------------------------------------
    template<class Container>
    void path_serializer<Container>::move_to(double x, double y, bool rel)
    {
        append_c(m_last_command = (rel ? 'm' : 'M'));
        append_d(x);
        append_d(y);
        ++m_num_vertices;
    }

    //------------------------------------------------------------------------
    template<class Container>
    void path_serializer<Container>::line_to(double x, double y, bool rel)
    {
        check_last_vertex(rel ? 'l' : 'L');
        append_c(m_last_command = (rel ? 'l' : 'L'));
        append_d(x);
        append_d(y);
        ++m_num_vertices;
    }

    //------------------------------------------------------------------------
    template<class Container>
    void path_serializer<Container>::hline_to(double x, bool rel)
    {
        check_last_vertex(rel ? 'h' : 'H');
        append_c(m_last_command = (rel ? 'h' : 'H'));
        append_d(x);
        ++m_num_vertices;
    }

    //------------------------------------------------------------------------
    template<class Container>
    void path_serializer<Container>::vline_to(double y, bool rel)
    {
        check_last_vertex(rel ? 'v' : 'V');
        append_c(m_last_command = (rel ? 'v' : 'V'));
        append_d(y);
        ++m_num_vertices;
    }

    //------------------------------------------------------------------------
    template<class Container>
    void path_serializer<Container>::curve3_to(double x1, double y1, 
                                               double x, double y, 
                                               bool rel)
    {
        check_last_vertex(rel ? 'q' : 'Q');
        append_c(m_last_command = (rel ? 'q' : 'Q'));
        append_d(x1);
        append_d(y1);
        append_d(x);
        append_d(y);
        m_num_vertices += 2;
    }

    //------------------------------------------------------------------------
    template<class Container>
    void path_serializer<Container>::curve3_to(double x, double y, bool rel)
    {
        check_last_vertex(rel ? 't' : 'T');
        check_curve_reflection(rel ? 't' : 'T');
        append_c(m_last_command = (rel ? 't' : 'T'));
        append_d(x);
        append_d(y);
        ++m_num_vertices;
    }

    //------------------------------------------------------------------------
    template<class Container>
    void path_serializer<Container>::curve4_to(double x1, double y1, 
                                               double x2, double y2, 
                                               double x,  double y, bool rel)
    {
        check_last_vertex(rel ? 'c' : 'C');
        append_c(m_last_command = (rel ? 'c' : 'C'));
        append_d(x1);
        append_d(y1);
        append_d(x2);
        append_d(y2);
        append_d(x);
        append_d(y);
        m_num_vertices += 3;
    }

    //------------------------------------------------------------------------
    template<class Container>
    void path_serializer<Container>::curve4_to(double x2, double y2, 
                                               double x,  double y, bool rel)
    {
        check_last_vertex(rel ? 's' : 'S');
        check_curve_reflection(rel ? 's' : 'S');
        append_c(m_last_command = (rel ? 's' : 'S'));
        append_d(x2);
        append_d(y2);
        append_d(x);
        append_d(y);
        m_num_vertices += 2;
    }

    //------------------------------------------------------------------------
    template<class Container>
    void path_serializer<Container>::arc_to(double rx, double ry, 
                                            double x_axis_rotation,
                                            bool large_arc_flag, 
                                            bool sweep_flag,
                                            double x, double y, 
                                            bool rel)
    {
        check_last_vertex(rel ? 'a' : 'A');
        append_c(m_last_command = (rel ? 'a' : 'A'));
        append_d(rx);
        append_d(ry);
        append_d(x_axis_rotation);
        append_c(large_arc_flag ? '1' : '0');
        append_c(sweep_flag     ? '1' : '0');
        append_d(x);
        append_d(y);
        m_num_vertices += 3;
    }

    //------------------------------------------------------------------------
    template<class Container>
    void path_serializer<Container>::close_subpath()
    {
        append_c('z');
    }


    //-------------------------------------------------------------------------
    // Ths class is used to parse, validate, and serialize polygons and polylines
    // into binary format.
    //----------------------------------------------------------------------------
    template<class Container> class poly_serializer
    {
    public:
        typedef Container container_type;

        poly_serializer(container_type& buffer) : m_buffer(&buffer) {}

        //------------------------------------------------------------------------
        template<class Tokenizer> void parse(Tokenizer& tok, bool closed)
        {
            unsigned np = 0;
            while(tok.token())
            {
                double x = tok.last_number();
                if(!tok.token())
                {
					throw exception("Parsing Polygon Structure: Odd number of coordinates");
                }
                double y = tok.last_number();
                append_d(x);
                append_d(y);
                ++np;
            }
            if(closed)
            {
                if(np < 3)
                {
					throw exception("Parsing Polygon Structure: Too few coordinates for polygon/polyline");
                }
            }
            else
            {
                if(np < 2)
                {
					throw exception("Parsing Polygon Structure: Too few coordinates for polygon/polyline");
                }
            }
        }

    private:
        void append_d(double val)
        {
            coord_type f = (coord_type)val;
            const agg::int8u* p = (const agg::int8u*)&f;
            unsigned i;
            for(i = 0; i < sizeof(coord_type); i++)
            {
                m_buffer->add(*p++);
            }
        }

        container_type* m_buffer;
    };

}
}

#endif
