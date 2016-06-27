//-----------------------------------------------------------------------------
//
// SVG parser.
//
//-----------------------------------------------------------------------------
#ifndef AGG_SVG_PARSER_INCLUDED
#define AGG_SVG_PARSER_INCLUDED
//-----------------------------------------------------------------------------
#include "agg_svg_basics.h"
#include "agg_svg_path_serializer.h"
#include "agg_svg_path_interpreter.h"
#include "agg_svg_dom_storage.h"
#include <stack>
//-----------------------------------------------------------------------------

namespace agg
{
namespace svg
{
    class attributes_map;
    //-------------------------------------------------------------------------
    class parser
    {
		enum buf_size_e { buf_size = 8 * BUFSIZ };
    public:
        //---------------------------------------------------------------------
        parser(dom_storage&, attributes_map& attr_map);
        //---------------------------------------------------------------------
        ~parser();
        //---------------------------------------------------------------------
        void parse(const wchar_t* fname);
        //---------------------------------------------------------------------
    private:
        
        // XML event handlers
        static void on_start_element(void* data, const char* el, const char** attr);
        static void on_end_element(void* data, const char* el);
        static void on_content(void* data, const char* s, int len);
		//---------------------------------------------------------------------
        typedef path_tokenizer<char>            path_tokenizer_type;
        typedef path_serializer<data_container> path_serializer_type;
         //---------------------------------------------------------------------
        // utility
        //
        //---------------------------------------------------------------------
        void prepare_attributes( const char**);
        //---------------------------------------------------------------------
        void start_element_cleanup();
        //---------------------------------------------------------------------
        const char* extract_attribute(attr_e attr);
        //---------------------------------------------------------------------
        attr_e find_known_attribute(const char* attr_name);   
        //---------------------------------------------------------------------
        void parse_name_value(char* nv_start, char* nv_end);
        //---------------------------------------------------------------------
        void add_double_to_buffer(double val);
        //---------------------------------------------------------------------
        const char* get_attr_name( unsigned attr );
        //---------------------------------------------------------------------
        // Error processing
        //
        //---------------------------------------------------------------------
        typedef void(parser::*error_checker_type)(double);
        //---------------------------------------------------------------------
        void not_negative(double);
        //---------------------------------------------------------------------
        // parse basic data types
        //
        //---------------------------------------------------------------------
        enum axis_e       {no_axis, axis_x, axis_y};
        //---------------------------------------------------------------------
        enum necessity_e  {optional, required};
        //---------------------------------------------------------------------
        void parse_number(attr_e, necessity_e necessity);
        //---------------------------------------------------------------------
        void parse_color (attr_e, necessity_e necessity);
        //---------------------------------------------------------------------
        void parse_string(attr_e);
        //---------------------------------------------------------------------
        void parse_coordinate_attr(attr_e attr, axis_e axis, 
                                necessity_e necessity, error_checker_type = 0);
        //---------------------------------------------------------------------
        bool length_parser(length& len, const char* begin, const char * end);
        //---------------------------------------------------------------------
        void parse_length_attr(attr_e attr, necessity_e necessity, 
                                error_checker_type = 0);
        //---------------------------------------------------------------------
        // parse elements
        //
        //---------------------------------------------------------------------
        void parse_polygon();
        void parse_polyline();
        void parse_svg();
        void parse_path();
        void parse_rect();
        void parse_line();
        void parse_circle();
        void parse_ellipse();
        void parse_linearGradient();
        void parse_radialGradient();
        void parse_stop();
        void parse_defs();
        void parse_title();
        void parse_g();
        void parse_use();
		void parse_clipPath();
        //---------------------------------------------------------------------
        // parse attributes
        //
        void parse_attr_viewBox();
        void parse_attr_preserveAspectRatio();
        void parse_object_units_attr(attr_e);
        void parse_paint_attr(attr_e);
        void parse_uri_attr(attr_e);


        unsigned parse_transform_args(const char* str, double* args, 
                                      unsigned max_na, unsigned* na);
 
        unsigned parse_matrix   (const char* str);
        unsigned parse_translate(const char* str);
        unsigned parse_rotate   (const char* str);
        unsigned parse_scale    (const char* str);
        unsigned parse_skew_x   (const char* str);
        unsigned parse_skew_y   (const char* str);	    
        //---------------------------------------------------------------------
        // parse attribute sets
        //
        void parse_core_attributes               ();
        void parse_presentation_attributes       ();
        void parse_style_attributes              ();
        void parse_paint_attributes              ();
        void parse_color_attributes              (); 
        void parse_opacity_attributes            ();
        void parse_xlink_attributes              ();
        void parse_common_gradient_attributes    ();
        void parse_basic_shapes_common_attributes();
        void parse_transform_attributes          (attr_e);
        //---------------------------------------------------------------------
   
    private:
        //---------------------------------------------------------------------
        struct extra_attr
        {
            const char* name;
            const char* value;
            extra_attr() {}
            extra_attr(const char* n, const char* v) : name(n), value(v) {}
        };
        //---------------------------------------------------------------------
        struct unit_descriptor
        {
            units2_e      unit;
            const char *  literal;
            unsigned char length;     
        };
        //---------------------------------------------------------------------
        dom_storage&                  m_storage;
        path_tokenizer_type           m_path_tokenizer;
        data_container                m_buffer;
        path_serializer_type          m_path_serializer;
        const char*                   m_curr_elem_name;

        const char*                   m_known_attr[end_of_attr];
        char*                         m_style_buffer;
        unsigned                      m_style_buffer_len;
        static unit_descriptor        m_units[];
        attributes_map&               m_attr_map;

		std::stack<bool> m_unknownElement;

		char*          m_buf;
    }; 

} //namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_PARSER_INCLUDED



