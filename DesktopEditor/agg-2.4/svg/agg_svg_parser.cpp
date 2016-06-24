//-----------------------------------------------------------------------------
#include "agg_svg_parser.h"
#include "agg_svg_color_parser.h"
#include "agg_svg_attributes_map.h"
#include "agg_svg_utils.h"
#include "agg_svg_parse_real.h"
#include "agg_svg_exception.h"

#include <cstring>
#include <cctype>
#include <cassert>
#include <cstdlib> // for strtod
#include "expat.h"
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    //-------------------------------------------------------------------------
    class sax_attributes_wrapper
    {
    public:
        //---------------------------------------------------------------------
        void go_to_first_attribute() const 
		{ 
			m_current_index = 0; 
		}
        //---------------------------------------------------------------------
        bool get_next_attribute(const char** name, const char** value) const
		{
			if (!m_attributes[m_current_index])
				return false; 

			*name  = m_attributes[m_current_index++];
			*value = m_attributes[m_current_index++];

			return true;
		}
        //---------------------------------------------------------------------
        const char* get_attribute_value(const char* name)
		{
			for (unsigned i = 0u; m_attributes[i]; i += 2)
			{
				using namespace std;
				if (strcmp(name, m_attributes[i]) == 0)
					return m_attributes[i+1];
			}
			return 0;
		}
        //---------------------------------------------------------------------
        void init(const char** attributes)
        { 
            m_attributes  = attributes;
            m_current_index = 0;
        }
        //---------------------------------------------------------------------
    private:
        const char**            m_attributes;
        mutable unsigned int    m_current_index;
    };
    //-------------------------------------------------------------------------
    #define PARSE_ENUM_ATTR( ATTRIB )\
    {attr_e attrib = ATTRIB; \
     const char* attr_str = extract_attribute( attrib );\
     if( attr_str != 0 ) {     
    //-------------------------------------------------------------------------
    #define NV( NAME, VALUE )\
     if(strcmp(attr_str,NAME)==0) m_storage.attribute_byte(attrib,VALUE); else
    //-------------------------------------------------------------------------
    #define PARSE_END()\
	{throw exception("Parsing element '%s', attribute '%s' - Invalid value: '%s'", m_curr_elem_name, m_attr_map.get_name(attrib), attr_str);\
	}}}
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    parser::unit_descriptor parser::m_units[] = 
    {   {units_em, "em", 2},
        {units_em, "em", 2},
        {units_em, "em", 2},

        {units_ex, "ex", 2},
        {units_ex, "ex", 2},
        {units_ex, "ex", 2},

        {units_px, "px", 2},
        {units_px, "px", 2},
        {units_px, "px", 2},

        {units_pt,  "pt", 2},
        {units_pt_x,"pt", 2},
        {units_pt_y,"pt", 2},

        {units_pc,  "pc", 2},
        {units_pc_x,"pc", 2},
        {units_pc_y,"pc", 2},

        {units_cm,  "cm", 2},
        {units_cm_x,"cm", 2},
        {units_cm_y,"cm", 2},

        {units_mm,  "mm", 2},
        {units_mm_x,"mm", 2},
        {units_mm_y,"mm", 2},

        {units_in,  "in", 2},
        {units_in_x,"in", 2},
        {units_in_y,"in", 2},

        {units_percent,  "%", 1},
        {units_percent_x,"%", 1},
        {units_percent_y,"%", 1}
    };
    //-------------------------------------------------------------------------
    bool parser::length_parser(length& len, const char* begin, const char * end)
    {
        assert(begin && *begin);

        const char * endptr = 0;

        len.unit  = units_px;
        double value = 0.;
        parse_real pd;
        if (!pd.parse(begin, end, &value, &endptr))
        {
            return false;
        }

        len.value = value;

        begin = endptr;

        if (*begin == 0)
            return true;

        for (unsigned i(0); i < sizeof(m_units) / sizeof(*m_units); i += 3)
        {
            if (strncmp(end - m_units[i].length, m_units[i].literal, m_units[i].length) == 0)
            {
                len.unit = m_units[i].unit;
                return true;
            }
        }

        return true;
    }
    //-------------------------------------------------------------------------
    attr_e parser::find_known_attribute(const char* attr_name)
    { 
        return m_attr_map.get_type(attr_name);
    }
    //-------------------------------------------------------------------------
    const char* parser::get_attr_name( unsigned attr )
    {
        return m_attr_map.get_name(attr);
    }
    //-------------------------------------------------------------------------
    parser::parser(dom_storage& storage, attributes_map& na_map)
        :	m_storage(storage)
		,	m_path_tokenizer()
		,	m_buffer(64-8)
		,	m_path_serializer(m_buffer)
		,	m_buf(new char[buf_size])
		,	m_style_buffer(new char[1024])
		,	m_style_buffer_len(1023)
		,   m_attr_map(na_map)
    {    
        memset(m_known_attr, 0, sizeof(m_known_attr));
    }
    //-------------------------------------------------------------------------
    parser::~parser()
    {
		delete [] m_buf;
        delete [] m_style_buffer;
    }
    //-------------------------------------------------------------------------
    void parser::parse(const wchar_t* fname)
    {
        m_storage.clear();

        char msg[512];
	    XML_Parser p = XML_ParserCreate(NULL);
	    if(p == 0) 
	    {
		    throw exception("Couldn't allocate memory for parser");
	    }

        XML_SetUserData(p, this);
	    XML_SetElementHandler(p, &parser::on_start_element, &parser::on_end_element);
	    XML_SetCharacterDataHandler(p, &parser::on_content);

        FILE* fd = _wfopen(fname, L"r");
        if(fd == 0)
        {
			XML_ParserFree(p);
            sprintf(msg, "Couldn't open file");
		    throw exception(msg);
        }

        bool done = false;
        do
        {
            size_t len = fread(m_buf, 1, buf_size, fd);
            done = len < buf_size;
            if(!XML_Parse(p, m_buf, (int)len, done))
            {
                sprintf(msg,
                    "%s at line %d\n",
                    XML_ErrorString(XML_GetErrorCode(p)),
                    XML_GetCurrentLineNumber(p));
				fclose(fd);
				XML_ParserFree(p);
                throw exception(msg);
            }
        }
        while(!done);
        fclose(fd);
        XML_ParserFree(p);
    }
    //-------------------------------------------------------------------------
	void parser::on_start_element(void* data, const char* el, const char** attr)
    {
		parser& self = *(parser*)data;

		assert(el && *el);

        self.m_curr_elem_name = el;
		self.prepare_attributes(attr);

		bool unknown_element = false;

             if (strcmp(el, "svg")            == 0) self.parse_svg();
        else if (strcmp(el, "title")          == 0) self.parse_title();
        else if (strcmp(el, "g")              == 0) self.parse_g();
        else if (strcmp(el, "path")           == 0) self.parse_path();
        else if (strcmp(el, "rect")           == 0) self.parse_rect();
        else if (strcmp(el, "line")           == 0) self.parse_line();
        else if (strcmp(el, "circle")         == 0) self.parse_circle();
        else if (strcmp(el, "ellipse")        == 0) self.parse_ellipse();
        else if (strcmp(el, "polyline")       == 0) self.parse_polyline();
        else if (strcmp(el, "polygon")        == 0) self.parse_polygon();
        else if (strcmp(el, "linearGradient") == 0) self.parse_linearGradient();
        else if (strcmp(el, "radialGradient") == 0) self.parse_radialGradient();
        else if (strcmp(el, "stop")           == 0) self.parse_stop();
        else if (strcmp(el, "defs")           == 0) self.parse_defs();
        else if (strcmp(el, "use")            == 0) self.parse_use();
        else if (strcmp(el, "clipPath")       == 0) self.parse_clipPath();
		else
			unknown_element = true;

		self.m_unknownElement.push(unknown_element);

        self.start_element_cleanup();
    }
    //-------------------------------------------------------------------------
	void parser::on_end_element(void* data, const char* el)
    {
		parser& self = *(parser*)data;

		bool unknown_element = self.m_unknownElement.top();
		self.m_unknownElement.pop();

		if (!unknown_element)
	        self.m_storage.end_element();
    }
    //-------------------------------------------------------------------------
	void parser::on_content(void* data, const char* s, int len)
    {
		parser& self = *(parser*)data;

		bool unknown_element = self.m_unknownElement.top();
		if (!unknown_element)
		{
			// TODO: parse_content(s, len);
		}
    }
    //-------------------------------------------------------------------------
    void parser::prepare_attributes(const char** attrs_str)
    {
		sax_attributes_wrapper attr;
		attr.init(attrs_str);

        const char* p_attr_name;
        const char* p_attr_value;

        while (attr.get_next_attribute( &p_attr_name, &p_attr_value ))        
        {
            attr_e a = find_known_attribute( p_attr_name );
            if (a != end_of_attr)
            {
                m_known_attr[a] = p_attr_value;
            }
        }
    }
    //-------------------------------------------------------------------------
    void parser::start_element_cleanup()
    {
        unsigned i;
        for(i = 0; i < end_of_attr; i++)
        {
            if(m_known_attr[i])
            {
                m_known_attr[i] = 0;
            }
        }
    }
    //-------------------------------------------------------------------------
    void parser::parse_name_value(char* nv_start, char* nv_end)
    {
        using namespace std;
        char* str = nv_start;

        // Find ':'
        while (str < nv_end && *str != ':') ++str;

        char* val = str + (str < nv_end);

        // Right Trim
        unsigned trim = 0;
        while (str > nv_start && (*str == ':' || isspace(*str))) 
        {
            ++trim;
            --str;
        }
        
        if (trim) ++str;
        *str = 0;

        while (val < nv_end && (*val == ':' || isspace(*val))) ++val;
        *nv_end = 0;

        attr_e a = find_known_attribute(nv_start);

        if(a != end_of_attr)
        {
			m_known_attr[a] = val;
        }
    }
    //-------------------------------------------------------------------------
    const char* parser::extract_attribute(attr_e attr)
    {
        const char* a = m_known_attr[attr];
        if (a) m_known_attr[attr] = 0;
        return a;
    }
    //-------------------------------------------------------------------------
    void parser::not_negative(double val)
    {
        if (val < 0.)
		{	
			throw exception( "Parsing element %s - Negative value: %f", m_curr_elem_name, val);
		}
    }
     //-------------------------------------------------------------------------
    void parser::parse_coordinate_attr(attr_e attr, axis_e axis, 
                                       necessity_e necessity, 
                                       error_checker_type error_check)
    {
        if (const char* value = extract_attribute(attr))
        {
            using namespace std;

            coordinate coor = {0., units_px};
            const char * endptr = 0;
        
            double val;
            parse_real pd;
            if (!pd.parse(value, value + strlen(value), &val, &endptr))
                return;

            coor.value = val;

            if (*endptr)
            {
                unsigned i = 0;
                switch (axis)
                {
                case no_axis:i = 0; break;
                case axis_x: i = 1; break;
                case axis_y: i = 2; break;
                default:
                    assert(false);
                }

                for (; i < sizeof(m_units) / sizeof(*m_units); i += 3)
                {
                    value += strlen(value);
                    value -= m_units[i].length;

                    const char * unit = m_units[i].literal; 

                    if (strncmp(value, unit, m_units[i].length) == 0)
                    {
                        coor.unit = m_units[i].unit;
                        break;
                    }
                }
            }
            if (error_check) (this->*error_check)(coor.value);
            m_storage.attribute(attr, coor);
        }
        else if (necessity == required)
        {
			throw exception("Element %s must have '%s' attribute", m_curr_elem_name, get_attr_name(attr));
        }
    }
    //-------------------------------------------------------------------------
    void parser::parse_length_attr(attr_e attr, necessity_e necessity, 
                                   error_checker_type error_check)
    {
        if (const char* value = extract_attribute(attr))
        {
            using namespace std;

            length len = {0., units_px};
            const char * endptr = 0;
        
            double val;
            parse_real pd;
            if (!pd.parse(value, value + strlen(value), &val, &endptr))
                return;

            len.value = val;

            if (*endptr)
            {
                unsigned i = 0;

                for (; i < sizeof(m_units) / sizeof(*m_units); i += 3)
                {
                    value += strlen(value);
                    value -= m_units[i].length;

                    const char * unit = m_units[i].literal; 

                    if (strncmp(value, unit, m_units[i].length) == 0)
                    {
                        len.unit = m_units[i].unit;
                        break;
                    }
                }
            }

            if (error_check) (this->*error_check)(len.value);
            m_storage.attribute(attr, len);
        }
        else if (necessity == required)
        {
			throw exception("Element %s must have '%s' attribute", m_curr_elem_name, get_attr_name(attr));
        }
    }
    //-------------------------------------------------------------------------
    void parser::parse_number(attr_e attr, necessity_e necessity)
    {
        if (const char* value = extract_attribute(attr))
        {
            double val = to_double(value);

            m_storage.attribute_numeric(attr, val);   
        }
        else if (necessity == required)
        {
			throw exception("Element %s must have '%s' attribute", m_curr_elem_name, get_attr_name(attr));
        }
    }
    //-------------------------------------------------------------------------
    void parser::parse_string(attr_e attr)
    {
        if (const char* value = extract_attribute(attr))
            m_storage.attribute_string(attr, value);
    }
    //-------------------------------------------------------------------------
    void parser::parse_color(attr_e attr, necessity_e necessity)
    {
        if (const char* value = extract_attribute(attr))
        {
            m_storage.attribute_datatype(attr, color_parser::parse_str(value));
        }
        else if (necessity == required)
        {
			throw exception("Element %s must have '%s' attribute", m_curr_elem_name, get_attr_name(attr));
        }
    }
    //-------------------------------------------------------------------------
    void parser::add_double_to_buffer(double val)
    {
        coord_type v = coord_type(val);
        m_buffer.add_array((const agg::int8u*)&v, sizeof(v));
    }
    //-------------------------------------------------------------------------
    void parser::parse_object_units_attr( attr_e attr )
    {
        PARSE_ENUM_ATTR( attr )        
            NV( "userSpaceOnUse"    ,    objectUnits_userSpaceOnUse      )
            NV( "objectBoundingBox" ,    objectUnits_objectBoundingBox   )     
        PARSE_END()
    }
    //-------------------------------------------------------------------------
    void parser::parse_paint_attr( attr_e attr )
    {
        if (const char* value = extract_attribute( attr ))
        {
			if (strcmp(value, "none") == 0)
				m_storage.attribute_byte(attr, paint_none);
			else if (strcmp(value, "currentColor") == 0)
				m_storage.attribute_byte(attr, paint_currentColor);        
			else if (strncmp(value, "url(#", 5) == 0)
				m_storage.attribute(attr, paint_id, value+5, static_cast<agg::int32u>(strlen(value+5)-1));          
			else
				m_storage.attribute_datatype(attr, paint_color, color_parser::parse_str(value)); 
        }
    }
	//-------------------------------------------------------------------------
    void parser::parse_uri_attr( attr_e attr )
    {
        if (const char* value = extract_attribute( attr ))
        {
			if (strcmp(value, "none") == 0)
				m_storage.attribute_byte(attr, uri_none);  
			else if (strncmp(value, "url(#", 5) == 0)
				m_storage.attribute(attr, uri_id, value+5, (unsigned)strlen(value+5)-1); 
			else
				throw exception("Parsing element '%s', attribute '%s' - Invalid value: '%s'", m_curr_elem_name, m_attr_map.get_name(attr), value);
        }
    }

    //-------------------------------------------------------------------------
    void parser::parse_attr_viewBox()
    {
        if (const char* value = extract_attribute( attr_viewBox ))
        {
            coord_type xy_wh[4] = {0.,0.,-1., -1.}; // x, y, w, h

            xy_wh[0] = to_double(value);
            xy_wh[1] = to_double(value = next_number(value));
            xy_wh[2] = to_double(value = next_number(value));
            xy_wh[3] = to_double(value = next_number(value));

            if (xy_wh[2] < 0.0) 
			{
				throw exception("Parsing element %s - Negative viewBox width: %f", m_curr_elem_name, xy_wh[2]);
			}

            if (xy_wh[3] < 0.0) 
			{
				throw exception("Parsing element %s - Negative viewBox height: %f",m_curr_elem_name, xy_wh[3]);
            }
            m_storage.attribute(attr_viewBox, xy_wh, sizeof(xy_wh));
        }
    }
    //-------------------------------------------------------------------------
    void parser::parse_attr_preserveAspectRatio()
    {
        if (const char* value = extract_attribute(attr_preserveAspectRatio))
        {    
            agg::int8u data[2] = {agg::int8u(usc_xMidYMid), agg::int8u(window_meet)}; // uniform_scaling_e, window_fit_logic_e

            char s1[32];
            char s2[32];
            char s3[32];

            value = get_token(value, s1, 31);
            value = get_token(value, s2, 31);
            value = get_token(value, s3, 31);

            const char* align         = s1;
            const char* meet_or_slice = s2;

            if (strcmp(s1, "defer") == 0)
            {
                align         = s2;
                meet_or_slice = s3;
            }

			if (align)
			{
					 if (strcmp(align, "none")    == 0) data[0] = usc_none;     
				else if (strcmp(align, "xMinYMin")== 0) data[0] = usc_xMinYMin; 
				else if (strcmp(align, "xMidYMin")== 0) data[0] = usc_xMidYMin; 
				else if (strcmp(align, "xMaxYMin")== 0) data[0] = usc_xMaxYMin; 
				else if (strcmp(align, "xMinYMid")== 0) data[0] = usc_xMinYMid; 
				else if (strcmp(align, "xMidYMid")== 0) data[0] = usc_xMidYMid; 
				else if (strcmp(align, "xMaxYMid")== 0) data[0] = usc_xMaxYMid; 
				else if (strcmp(align, "xMinYMax")== 0) data[0] = usc_xMinYMax; 
				else if (strcmp(align, "xMidYMax")== 0) data[0] = usc_xMidYMax; 
				else if (strcmp(align, "xMaxYMax")== 0) data[0] = usc_xMaxYMax; 
				else 
					throw exception("Parsing element %s - preserveAspectRatio: Invalid alignment: %s", m_curr_elem_name, align);
			}
            if (uniform_scaling_e(data[0]) != usc_none && meet_or_slice && *meet_or_slice)
            {
				     if (strcmp(meet_or_slice, "meet") == 0)  data[1] = window_meet;
				else if (strcmp(meet_or_slice, "slice") == 0) data[1] = window_slice;
				else
					throw exception("Parsing element %s - preserveAspectRatio: Invalid meet_or_slice: %s", m_curr_elem_name, meet_or_slice);
            } 
            m_storage.attribute(attr_preserveAspectRatio, data, sizeof(data));
        }     
    }
    //-------------------------------------------------------------------------
    void parser::parse_svg()
    {
        m_storage.start_element(elem_svg);

        parse_core_attributes();
        parse_style_attributes();
        parse_presentation_attributes();

        parse_coordinate_attr(attr_x, axis_x, optional);
        parse_coordinate_attr(attr_y, axis_y, optional);
        parse_coordinate_attr(attr_width,  axis_x, optional, &parser::not_negative);
        parse_coordinate_attr(attr_height, axis_y, optional, &parser::not_negative);

        parse_attr_viewBox();
        parse_attr_preserveAspectRatio();
    }
    //-------------------------------------------------------------------------
    void parser::parse_path()
    {
        m_storage.start_element(elem_path);

        parse_basic_shapes_common_attributes();

        if (const char* value = extract_attribute(attr_d))
        {
            m_buffer.remove_all(); 
            m_path_tokenizer.path_str(value);
            m_path_serializer.parse(m_path_tokenizer);
            m_storage.attribute_d_with_bounds(data_accessor_type(m_buffer));   
        }
    }
    //-------------------------------------------------------------------------
    void parser::parse_polygon()
    {
        m_storage.start_element(elem_polygon);

        parse_basic_shapes_common_attributes();

        if (const char* value = extract_attribute(attr_points))
        {
            m_buffer.remove_all();
            m_path_tokenizer.path_str(value);

            while (m_path_tokenizer.token())
            {
                double x = m_path_tokenizer.last_number();

                if (!m_path_tokenizer.token())
                {
					return;
                }

                double y = m_path_tokenizer.last_number();

                add_double_to_buffer(x);
                add_double_to_buffer(y);
            }

            if (m_buffer.size() < sizeof(coord_type) * 6)
            {
				return;
            }

            m_storage.attribute_points_with_bounds(m_buffer);     
        }
    }
    //-------------------------------------------------------------------------
    void parser::parse_polyline()
    {
        m_storage.start_element(elem_polyline);

        parse_basic_shapes_common_attributes();

        if (const char* value = extract_attribute(attr_points))
        {
            m_buffer.remove_all();
            m_path_tokenizer.path_str(value);

            while (m_path_tokenizer.token())
            {
                double x = m_path_tokenizer.last_number();

                if (!m_path_tokenizer.token())
                {
					return;
                }

                double y = m_path_tokenizer.last_number();

                add_double_to_buffer(x);
                add_double_to_buffer(y);
            }

            if (m_buffer.size() < sizeof(coord_type) * 4)
			{
				return;
			}

            m_storage.attribute_points_with_bounds(m_buffer);     
        }
    }
    //-------------------------------------------------------------------------
    void parser::parse_rect()
    {
        m_storage.start_element(elem_rect);

        parse_basic_shapes_common_attributes();

        parse_coordinate_attr(attr_x,     axis_x, optional);
        parse_coordinate_attr(attr_y,     axis_y, optional);
        parse_coordinate_attr(attr_width, axis_x, required, &parser::not_negative);
        parse_coordinate_attr(attr_height,axis_y, required, &parser::not_negative);
    
        parse_coordinate_attr(attr_rx,    axis_x, optional, &parser::not_negative);
        parse_coordinate_attr(attr_ry,    axis_y, optional, &parser::not_negative);
    }
    //-------------------------------------------------------------------------
    void parser::parse_line()
    {
        m_storage.start_element(elem_line);

        parse_basic_shapes_common_attributes();

        parse_coordinate_attr(attr_x1, axis_x, optional);
        parse_coordinate_attr(attr_y1, axis_y, optional);
        parse_coordinate_attr(attr_x2, axis_x, optional);
        parse_coordinate_attr(attr_y2, axis_y, optional);
    }
    //-------------------------------------------------------------------------
    void parser::parse_circle()
    {
        m_storage.start_element(elem_circle);

        parse_basic_shapes_common_attributes();

        parse_coordinate_attr(attr_cx, axis_x, optional);
        parse_coordinate_attr(attr_cy, axis_y, optional);
        parse_length_attr(attr_r, required, &parser::not_negative);
    }
    //-------------------------------------------------------------------------
    void parser::parse_ellipse()
    {
        m_storage.start_element(elem_ellipse);

        parse_basic_shapes_common_attributes();

        parse_coordinate_attr(attr_cx, axis_x, optional);
        parse_coordinate_attr(attr_cy, axis_y, optional);
        parse_length_attr(attr_rx, required, &parser::not_negative);
        parse_length_attr(attr_ry, required, &parser::not_negative);
    }
    //-------------------------------------------------------------------------
    void parser::parse_linearGradient()
    {
        m_storage.start_element(elem_linearGradient);

        parse_core_attributes            ();
        parse_style_attributes           ();
        parse_color_attributes           ();
        parse_xlink_attributes           ();
        parse_common_gradient_attributes ();

        // x1, y1, x2, y2 
        //----------------------------------------------------
        parse_coordinate_attr( attr_x1, no_axis, optional);
        parse_coordinate_attr( attr_y1, no_axis, optional);
        parse_coordinate_attr( attr_x2, no_axis, optional);
        parse_coordinate_attr( attr_y2, no_axis, optional);
    }
    //-------------------------------------------------------------------------
    void parser::parse_radialGradient()
    {
        m_storage.start_element(elem_radialGradient);

        parse_core_attributes            ();
        parse_style_attributes           ();
        parse_color_attributes           ();
        parse_xlink_attributes           ();
        parse_common_gradient_attributes ();

        // cx, cy, r, fx, fy 
        //----------------------------------------------------
        parse_coordinate_attr(attr_cx, axis_x, optional);
        parse_coordinate_attr(attr_cy, axis_y, optional);
        parse_length_attr(attr_r, optional, &parser::not_negative);
    
        parse_length_attr(attr_fx, optional);
        parse_length_attr(attr_fy, optional);
    }
    //-------------------------------------------------------------------------
    void parser::parse_stop()
    {
        m_storage.start_element(elem_stop);

        parse_core_attributes ();
        parse_style_attributes();
        parse_color_attributes();

        parse_color (attr_stop_color  , optional);
        parse_number(attr_stop_opacity, optional);

        if (const char* value = extract_attribute(attr_offset))
        {
            double offset = to_double( value );

            if (str_ends(value,'%')) 
                m_storage.attribute_percent(attr_offset, offset);
            else
                m_storage.attribute_numeric(attr_offset, offset);    
        }
    }
    //-------------------------------------------------------------------------
    void parser::parse_defs()
    {
        m_storage.start_element(elem_defs);

        parse_core_attributes            ();
        parse_style_attributes           ();
        parse_presentation_attributes    ();
        parse_transform_attributes       (attr_transform);
    }
    //-------------------------------------------------------------------------
    void parser::parse_title()
    {
        m_storage.start_element(elem_title);

        parse_core_attributes ();
        parse_style_attributes();
    }
    //-------------------------------------------------------------------------
    void parser::parse_g()
    {
        m_storage.start_element(elem_g);

        parse_core_attributes            ();
        parse_style_attributes           ();
        parse_presentation_attributes    ();
        parse_transform_attributes       (attr_transform);
    }
    //-------------------------------------------------------------------------
	void parser::parse_use()
	{
        m_storage.start_element(elem_use);

        parse_core_attributes            ();
        parse_style_attributes           ();
        parse_presentation_attributes    ();
		parse_xlink_attributes           ();
        parse_transform_attributes       (attr_transform);

        parse_coordinate_attr(attr_x, axis_x, optional);
        parse_coordinate_attr(attr_y, axis_y, optional);
        parse_coordinate_attr(attr_width,  axis_x, optional, &parser::not_negative);
        parse_coordinate_attr(attr_height, axis_y, optional, &parser::not_negative);
	}
    //-------------------------------------------------------------------------
    void parser::parse_clipPath()
    {
		m_storage.start_element(elem_clipPath);
    }
    //-------------------------------------------------------------------------
    void parser::parse_core_attributes()
    {
        if (const char* value = extract_attribute(attr_id))
        {
            m_storage.attribute(attr_id, value, (unsigned)strlen(value) );
        }    
    }
    //-------------------------------------------------------------------------
    void parser::parse_presentation_attributes()
    {
        parse_core_attributes            ();
        parse_style_attributes           (); 
        parse_paint_attributes           ();
        parse_color_attributes           ();   
        parse_opacity_attributes         ();
        parse_common_gradient_attributes (); 
        parse_xlink_attributes           ();
    }
    //-------------------------------------------------------------------------
    void parser::parse_style_attributes()
    {
        if(m_known_attr[attr_style])
        {
            // Process and parse "style" attribute 
            unsigned len = (unsigned)strlen( m_known_attr[attr_style] );
            if( len > m_style_buffer_len )
            {
                delete [] m_style_buffer;
                m_style_buffer = new char[ len + 256 ];
                m_style_buffer_len = len + 256 - 1;
            }
         
            memcpy( m_style_buffer, m_known_attr[attr_style], len );
            m_style_buffer[len] = 0;
            m_known_attr[attr_style] = 0;
 
            char* str = m_style_buffer;
            char* end = m_style_buffer + strlen( m_style_buffer );
            while(str < end)
            {
                left_trim( str, end );
                char* nv_start = str;

                // find separator i.e. ';'
                while(str < end && *str != ';') ++str;
                char* nv_end = str;

                if(nv_end > nv_start)
                {
                    right_trim( nv_start, nv_end, ';' );
                    parse_name_value( nv_start, nv_end );
                }
                ++str;
            }
        }
    }
    //-------------------------------------------------------------------------
    void parser::parse_paint_attributes()
    {
        parse_color_attributes();

        parse_paint_attr(attr_fill);

        PARSE_ENUM_ATTR( attr_fill_rule )
            NV(  "nonzero", fill_rule_nonzero )
            NV(  "evenodd", fill_rule_evenodd )    
        PARSE_END()


        parse_paint_attr(attr_stroke);

        PARSE_ENUM_ATTR( attr_stroke_linecap )
            NV( "butt"  , stroke_linecap_butt   )
            NV( "round" , stroke_linecap_round  )
            NV( "square", stroke_linecap_square )      
        PARSE_END()     

        PARSE_ENUM_ATTR( attr_stroke_linejoin )
            NV(  "miter", stroke_linejoin_miter )
            NV(  "round", stroke_linejoin_round )
            NV(  "bevel", stroke_linejoin_bevel )      
        PARSE_END()     

        parse_number(attr_stroke_miterlimit, optional);   

        parse_length_attr(attr_stroke_width, optional);
    }
    //-------------------------------------------------------------------------
    void parser::parse_color_attributes()
    {
        parse_color(attr_color, optional); 
	}
    //-------------------------------------------------------------------------
    void parser::parse_opacity_attributes()
    {
        parse_number(attr_opacity,        optional);
        parse_number(attr_stroke_opacity, optional);    
        parse_number(attr_fill_opacity,   optional);
    }
    //-------------------------------------------------------------------------
    void parser::parse_xlink_attributes()
    {
        if (const char* value = extract_attribute(attr_xlink_href))
        {
            if (*value == '#')
                m_storage.attribute_string(attr_xlink_href, value + 1);
            else
                m_storage.attribute_string(attr_xlink_href, value);
        }
    }
    //-------------------------------------------------------------------------
    void parser::parse_common_gradient_attributes()
    {
        PARSE_ENUM_ATTR( attr_spreadMethod )  
            NV( "pad"     ,    spreadMethod_pad       )
            NV( "reflect" ,    spreadMethod_reflect   )     
            NV( "repeat"  ,    spreadMethod_repeat    )
        PARSE_END()
        
        parse_object_units_attr(attr_gradientUnits);
        
        parse_transform_attributes(attr_gradientTransform);
    }
    //-------------------------------------------------------------------------
    void parser::parse_basic_shapes_common_attributes()
    {
        parse_core_attributes           ();
        parse_style_attributes          ();
        parse_paint_attributes          ();
        parse_color_attributes          ();
        parse_opacity_attributes        ();
        parse_transform_attributes      (attr_transform);
    }
    //-------------------------------------------------------------------------
    void parser::parse_transform_attributes(attr_e attr)
    {
        if (const char* value = extract_attribute( attr ))
        {
            while (*value)
            {
                using namespace std;
                if (!islower(*value))
                {
                    ++value;
                    continue;
                }

                     if (strncmp(value, "matrix",    6) == 0) value += parse_matrix(value);     
                else if (strncmp(value, "translate", 9) == 0) value += parse_translate(value);  
                else if (strncmp(value, "rotate",    6) == 0) value += parse_rotate(value);     
                else if (strncmp(value, "scale",     5) == 0) value += parse_scale(value);      
                else if (strncmp(value, "skewX",     5) == 0) value += parse_skew_x(value);     
                else if (strncmp(value, "skewY",     5) == 0) value += parse_skew_y(value);    
                else
					++value;
            }
        }
    }
    //-------------------------------------------------------------------------
    unsigned parser::parse_transform_args(const char* str, double* args, 
                                          unsigned max_na, unsigned* na)
    {
        *na = 0;
        const char* ptr = str;

        while (*ptr && *ptr != '(') ++ptr;

        if(*ptr == 0)
        {
			return 0;
        }

        const char* end = ptr;

        while(*end && *end != ')') ++end;

        if (*end == 0)
        {
			return 0;
        }

        while(ptr < end)
        {
            if(is_numeric(*ptr))
            {
                if(*na >= max_na)
                {
					return 0;
                }
                double val;
                parse_real pd;
                const char* endptr;
                if (!pd.parse(ptr, end, &val, &endptr))
                {
					return 0;
                }
                args[(*na)++] = val;
                ptr = endptr;

                while(ptr < end && is_numeric(*ptr)) ++ptr;
            }
            else
            {
                ++ptr;
            }
        }
        return unsigned(end - str);
    }
    //-------------------------------------------------------------------------
    unsigned parser::parse_matrix(const char* str)
    {
        double args[6];
        unsigned na = 0;
        unsigned len = parse_transform_args(str, args, 6, &na);

        m_storage.attribute_transform_matrix(args[0], args[1], args[2], 
                                             args[3], args[4], args[5]);
    
        return len;
    }
    //-------------------------------------------------------------------------
    unsigned parser::parse_translate(const char* str)
    {
        double args[2];
        unsigned na = 0;
        unsigned len = parse_transform_args(str, args, 2, &na);
        if (na == 1) args[1] = 0.0;
        m_storage.attribute_transform_translate(args[0], args[1]);
        return len;
    }
    //-------------------------------------------------------------------------
    unsigned parser::parse_rotate(const char* str)
    {
        double args[3];
        unsigned na = 0;
        unsigned len = parse_transform_args(str, args, 3, &na);
        if(na == 1) 
        {
            m_storage.attribute_transform_rotate(agg::deg2rad(args[0])); 
        }
        else if(na == 3)
        {
            m_storage.attribute_transform_rotate(agg::deg2rad(args[0]), 
                                                 args[1], args[2]); 
        }
        return len;
    }
    //-------------------------------------------------------------------------
    unsigned parser::parse_scale(const char* str)
    {
        double args[2];
        unsigned na = 0;
        unsigned len = parse_transform_args(str, args, 2, &na);
        if(na == 1) m_storage.attribute_transform_scale(args[0]); 
        else        m_storage.attribute_transform_scale(args[0], args[1]); 
        return len;
    }
    //-------------------------------------------------------------------------
    unsigned parser::parse_skew_x(const char* str)
    {
        double arg;
        unsigned na = 0;
        unsigned len = parse_transform_args(str, &arg, 1, &na);
        m_storage.attribute_transform_skew_x(agg::deg2rad(arg)); 
        return len;
    }
    //-------------------------------------------------------------------------
    unsigned parser::parse_skew_y(const char* str)
    {
        double arg;
        unsigned na = 0;
        unsigned len = parse_transform_args(str, &arg, 1, &na);
        m_storage.attribute_transform_skew_y(agg::deg2rad(arg)); 
        return len;
    }

}
}