//-----------------------------------------------------------------------------
#ifndef AGG_SVG_RENDERING_INTERPRETER_INCLUDED
#define AGG_SVG_RENDERING_INTERPRETER_INCLUDED
//-----------------------------------------------------------------------------
#include "agg_array.h"
#include "agg_svg_basics.h"
#include "agg_svg_attributes_setter.h"
#include "agg_svg_attributes.h"
#include "agg_svg_dom_storage.h"
#include "agg_svg_assoc_pod_array.h"
#include "agg_svg_pipeline.h"
#include "agg_svg_exception.h"
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    //-------------------------------------------------------------------------
    // forward declarations
    //
    class pipeline;
    class gradient_lut_cache;
    class frame_buffer_rgba;
	class renderer_rgba;
    //-------------------------------------------------------------------------
    class rendering_interpreter
    {
    public:
        //---------------------------------------------------------------------
        typedef renderer_rgba renderer_rgba_type;
        //---------------------------------------------------------------------
        rendering_interpreter(rendering_interpreter const&, 
                              attributes&, 
                              renderer_rgba_type&,
                              bool render_paint_servers = false);
        //---------------------------------------------------------------------
        rendering_interpreter(pipeline&, 
                              attributes&,
                              renderer_rgba_type&, 
                              dom_storage::map_type&,
                              gradient_lut_cache&,
                              bool render_paint_servers = false);
		//---------------------------------------------------------------------
		void ignore_viewBox(bool ignVB)
		{
			m_ignore_view_box = ignVB;
		}
        //---------------------------------------------------------------------
        bool start_element(const dom_storage& st, 
                           const element_data& el);
        //---------------------------------------------------------------------
        bool end_element(const dom_storage& st, const element_data& el);
        //---------------------------------------------------------------------
#ifdef EXPAND_PATHS
		void expand(double value)
		{
			m_pipeline.expand(value);
		}
#endif
        //---------------------------------------------------------------------
        element_e current_element() {return m_current_element;}
        //---------------------------------------------------------------------
        template <class DataAccessor>
        element_e interpret_element(const DataAccessor& data)
        {
            dom_storage::map_type::iterator it = m_it2element_map.find(data);
            if (it == m_it2element_map.end())
            {
                return end_of_elements;
            }
            m_current_element = end_of_elements;

            return (*it).second.code;
        }
        //---------------------------------------------------------------------
        void process_xlink_href(data_accessor_type data);
        //---------------------------------------------------------------------
    private:
        //---------------------------------------------------------------------
        typedef rendering_interpreter this_type;
        typedef void (this_type::*el_interpreter)(const element_data&);
        static el_interpreter const element_interpreters[];
        //---------------------------------------------------------------------
        void render_circle              (const element_data&);
        void render_clipPath            (const element_data&);
        void render_defs                (const element_data&);
        void render_ellipse             (const element_data&);
        void render_g                   (const element_data&);
        void render_line                (const element_data&);
        void render_linearGradient      (const element_data&);
        void render_path                (const element_data&);
        void render_polygon             (const element_data&);
        void render_polyline            (const element_data&);
        void render_radialGradient      (const element_data&);
        void render_rect                (const element_data&);
        void render_stop                (const element_data&);
        void render_svg                 (const element_data&);
        void set_title_element          (const element_data&);
        void render_use                 (const element_data&);
        //---------------------------------------------------------------------
        double read_length(const data_accessor_type& data) const;
        //---------------------------------------------------------------------
        objectUnits_e read_objectUnits(const data_accessor_type& data) const;
        //---------------------------------------------------------------------
        void read_viewBox(const data_accessor_type& data,
                          double* x, double* y, double* w, double* h) const;
        //---------------------------------------------------------------------
        void read_preserveAspectRatio(const data_accessor_type& data,
                                      uniform_scaling_e*        usc,
                                      window_fit_logic_e*       wfl) const;
        //---------------------------------------------------------------------
        template <class VertexSource>
        void general_work_for_basic_shapes(const element_data& el, 
                                                 VertexSource&      src)
        {
            conv_polymorphic_wrapper<VertexSource> source(src);
            m_vertex_source = &source;    

            m_attr_setter.set_basic_shapes_common_attributes(el.attr, el.num_attr);

            gradient fill_gradient;
            gradient stroke_gradient;

            prepare_gradient_fill   (fill_gradient);
            prepare_gradient_stroke (stroke_gradient);

            m_pipeline.render(m_attributes, src, m_renderer);
        }
        //---------------------------------------------------------------------
        bool element_is_paint_server(element_e code) const;
        //---------------------------------------------------------------------
        void calculate_and_set_object_bbox(object_bbox& bbox);
        //---------------------------------------------------------------------
        bool units_is_objectBoundingBox(unsigned idx, attr_e code) const;
        //---------------------------------------------------------------------
        void create_gradient(gradient&, unsigned);
        //---------------------------------------------------------------------
        void prepare_gradient_fill  (gradient&);
        //---------------------------------------------------------------------
        void prepare_gradient_stroke(gradient&);
        //---------------------------------------------------------------------
        bool is_visible(coord_type(&)[4]) const;
        //---------------------------------------------------------------------
        void set_common_gradient_attributes(attr_data*, unsigned);
        //---------------------------------------------------------------------
        void set_gradient (gradient* g)   {m_gradient = g;   } 
	    //---------------------------------------------------------------------
        unsigned locate(const char* str);
        //---------------------------------------------------------------------
        template <class DataAccessor>
        unsigned locate(DataAccessor id, int* = 0)
        {
            dom_storage::map_type::iterator it;
        
            it = m_it2element_map.find(id);
    
            if (it == m_it2element_map.end())
            {
				return 0;
            }
    
            return it->second.idx;
        }
        //---------------------------------------------------------------------
        pipeline&              m_pipeline; 
        attributes&            m_attributes; 
        gradient_lut_cache&    m_gradient_lut_cache;
        renderer_rgba_type&    m_renderer;
        dom_storage const *    m_storage;
        dom_storage::map_type& m_it2element_map;
        element_e              m_current_element;
        gradient*              m_gradient;
        attributes_setter      m_attr_setter;
        int                    m_defs_counter;
        int                    m_paint_server_counter;

        conv_polymorphic_base* m_vertex_source;        

        bool m_render_paint_servers;
		bool m_clipPath;
		bool m_ignore_view_box;
    };
   
} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_RENDERING_INTERPRETER_INCLUDED
