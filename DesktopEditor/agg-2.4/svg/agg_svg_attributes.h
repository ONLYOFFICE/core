#ifndef AGG_SVG_ATTRIBUTES_INCLUDED
#define AGG_SVG_ATTRIBUTES_INCLUDED

#include "agg_conv_stroke.h"
#include "agg_gamma_lut.h"
#include "agg_svg_basics.h"
#include "agg_svg_defines.h"
#include "agg_svg_transformer.h"
#include "agg_svg_frame_buffer_rgba.h"
#include "agg_svg_gradient_lut_cache.h"
#include <string>


namespace agg
{
namespace svg
{
    extern agg::line_join_e    g_stroke_linejoin_lut[];
    extern agg::line_cap_e     g_stroke_linecap_lut[];
    extern agg::filling_rule_e g_fill_rule_lut[];

    class global_settings
    {
    public:
        typedef agg::gamma_lut<color_type::value_type, 
                               color_type::value_type, 
                               color_type::base_shift, 
                               color_type::base_shift> gamma_lut_type;

        global_settings() 
			:	m_gamma()
			,	m_dpi_x(96.0)
			,	m_dpi_y(96.0)
        {}

        // Metrics
        //-----------------------------------------------------------------------
        void   dpi(double v)   { m_dpi_x = m_dpi_y = v; }
        void   dpi_x(double v) { m_dpi_x = v; }
        void   dpi_y(double v) { m_dpi_y = v; }
        double dpi() const     { return (m_dpi_x + m_dpi_y) / 2; }

        static double pt2pix(double pt, double dpi) { return pt * dpi / 72.0; }
        static double pc2pix(double pc, double dpi) { return pc * dpi / 6.0;  }
        static double cm2pix(double cm, double dpi) { return cm * dpi / 2.54; }
        static double mm2pix(double mm, double dpi) { return mm * dpi / 25.4; }
        static double in2pix(double in, double dpi) { return in * dpi; }

        double pt2pix_x(double pt) const { return pt2pix(pt, m_dpi_x); }
        double pt2pix_y(double pt) const { return pt2pix(pt, m_dpi_y); }
        double pc2pix_x(double pc) const { return pc2pix(pc, m_dpi_x); }
        double pc2pix_y(double pc) const { return pc2pix(pc, m_dpi_y); }
        double cm2pix_x(double cm) const { return cm2pix(cm, m_dpi_x); }
        double cm2pix_y(double cm) const { return cm2pix(cm, m_dpi_y); }
        double mm2pix_x(double mm) const { return mm2pix(mm, m_dpi_x); }
        double mm2pix_y(double mm) const { return mm2pix(mm, m_dpi_y); }
        double in2pix_x(double in) const { return in2pix(in, m_dpi_x); }
        double in2pix_y(double in) const { return in2pix(in, m_dpi_y); }


        double pt2pix(double pt) const { return pt2pix(pt, m_dpi_x); }
        double pc2pix(double pc) const { return pc2pix(pc, m_dpi_x); }
        double cm2pix(double cm) const { return cm2pix(cm, m_dpi_x); }
        double mm2pix(double mm) const { return mm2pix(mm, m_dpi_x); }
        double in2pix(double in) const { return in2pix(in, m_dpi_x); }


        // Full color gamma correction
        //-----------------------------------------------------------------------
        void gamma(double g) { m_gamma.gamma(g); }
        double gamma() const { return m_gamma.gamma(); }

        const gamma_lut_type& gamma_lut() const { return m_gamma; }

        unsigned gamma_dir(unsigned v) const { return m_gamma.dir(v); }
        unsigned gamma_inv(unsigned v) const { return m_gamma.inv(v); }
  
    private:
        gamma_lut_type        m_gamma;
        double                m_dpi_x;
        double                m_dpi_y;
    };

    //-------------------------------------------------------------------------
    // Class attr_stack is used to save and restore different attributes.
    // 
    // USAGE:
    // 
    // 1. When the SVG command comes (startElement event), call
    //    begin_session();
    // 
    // 2. When the attribute comes, you do the following:
    //    if(!stack.attribute_saved(attr.type()))
    //    {
    //        int8u* ptr = stack.push_attribute(attr.type(), attr.size());
    //        if(ptr) attr.serialize(ptr);
    //    }
    //    attr.set_new_value(attr_value);
    // 
    // 3. When processing of the command is done (endElement event)
    //    you restore all saved attributes and call end_session():
    // 
    //    for(i = 0; i < stack.num_attributes(); ++i)
    //    {
    //        const attr_stack::attr* a = stack.attribute(i);
    //        if(a)
    //        {
    //            attr.deserialize(a->data, a->size);
    //        }
    //    }
    //    end_session();
    // 
    //---------------------------------------------------------------------------
    class attr_stack
    {
    public:
        //-----------------------------------------------------------------------
        enum 
        { 
            attr_heap_block_shift = 16,
            attr_heap_block_size  = 1 << attr_heap_block_shift,
            attr_heap_block_mask  = attr_heap_block_size - 1,

            large_block_flag = (1 << 16)
        };

        //-----------------------------------------------------------------------
        struct attr
        {
            unsigned    type;
            unsigned    size;
            agg::int8u* data;

            attr() {}
            attr(unsigned t, agg::int8u* d, unsigned s) : 
                type(t), size(s), data(d)
            {}
				unsigned attr_type() const { return type & (~large_block_flag); }
        };


        //-----------------------------------------------------------------------
        attr_stack() : 
            m_heap(256-4),
            m_attributes(),
            m_sessions()
        {}

        //-----------------------------------------------------------------------
        unsigned num_sessions() const 
        { 
            return m_sessions.size(); 
        }

        //-----------------------------------------------------------------------
        void begin_session()
        {
            m_sessions.add(session(m_attributes.size(), m_heap.size()));
        }

        //-----------------------------------------------------------------------
        bool attribute_saved(unsigned type) const
        {
            return m_sessions.last().attribute_saved(type) != 0;
        }

        //-----------------------------------------------------------------------
        agg::int8u* push_attribute(unsigned type, unsigned size);


        //-----------------------------------------------------------------------
        unsigned num_attributes() const
        {
            return m_sessions.last().num_attributes;
        }

        //-----------------------------------------------------------------------
        const attr* attribute(unsigned idx) const
        {
            return &m_attributes[m_sessions.last().start_attr_index + idx];
        }

        //-----------------------------------------------------------------------
        void end_session();
        //-----------------------------------------------------------------------
        void clear()
        {
            m_heap.free_all();
            m_attributes.free_all();
            m_sessions.free_all();
        }

    private:
        //-----------------------------------------------------------------------
        struct session
        {
            enum
            {
                num_flag_bytes = 512/8
            };

            unsigned   start_attr_index;
            unsigned   num_attributes;
            unsigned   start_attr_heap;
            agg::int8u saved_attributes[num_flag_bytes];

            session() {}
            session(unsigned start_attr, unsigned start_heap) : 
                start_attr_index(start_attr),
                num_attributes(0),
                start_attr_heap(start_heap)
            {
                memset(saved_attributes, 0, sizeof(saved_attributes));
            }

            void set_saved_attribute(unsigned attr_type)
            {
                attr_type &= ~large_block_flag;
                saved_attributes[attr_type >> 3] |= 1 << (attr_type & 7);
            }

            unsigned attribute_saved(unsigned attr_type) const
            {
                attr_type &= ~large_block_flag;
                return saved_attributes[attr_type >> 3] & (1 << (attr_type & 7));
            }
        };

        //-----------------------------------------------------------------------
        agg::pod_bvector<agg::int8u, attr_heap_block_shift> m_heap;
        agg::pod_bvector<attr, 8>                           m_attributes;
        agg::pod_bvector<session, 6>                        m_sessions;
    };


    //-------------------------------------------------------------------------
	class attr_string
	{
		std::string m_value;

	public:
		const char_type* data()const { return m_value.c_str(); }

		unsigned byte_size()const {return (unsigned)m_value.size(); }

        template <class DataAccessor>
        const attr_string& assign_from(DataAccessor str)
        {
            unsigned len = str.size();

			m_value.clear();
			m_value.reserve(len + 1);

            for(unsigned i = 0; str.size();) 
            { 
                m_value += str.read_value(); 
            }
            return *this;
        }

        void serialize(agg::int8u* ptr)const
		{
			memcpy(ptr, m_value.data(), m_value.size());
		}

		void deserialize(const agg::int8u* data, unsigned size)
		{
			m_value.assign(data, data + size);
		}
	};

    //-------------------------------------------------------------------------
    // This class is a container for all known SVG attributes. It uses 
    // attr_stack inside. 
    // 
    //---------------------------------------------------------------------------
    class attributes
    {
	private:
		struct zoom_data
        {
            double wx1, wy1, wx2, wy2;
            double sx1, sy1, sx2, sy2;
        };

        typedef global_settings::gamma_lut_type gamma_lut_type;

		// prevent copy
        attributes(const attributes& attr);
        const attributes& operator = (const attributes& attr);

        struct values
        {
            double               opacity;
            color_type           color;
            paint_type_e         fill_type;
            color_type           fill_color;
			attr_string          fill_gradient_id;
            double               fill_opacity;
            fill_rule_e          fill_rule;
            paint_type_e         stroke_type;
            color_type           stroke_color;
			attr_string          stroke_gradient_id;
            double               stroke_opacity;
            double               stroke_width;
            stroke_linejoin_e    stroke_linejoin;
            stroke_linecap_e     stroke_linecap;
            double               stroke_miterlimit;

            agg::svg::transformer          transformer;
            double               object_bbox[4];

            values() : 
                opacity(1.0),
                color(color_type(0,0,0)),
                fill_type(paint_color),
                fill_color(color_type(0, 0, 0)),
                fill_gradient_id(),
                fill_opacity(1.0),
                fill_rule(fill_rule_nonzero),
                stroke_type(paint_none),
                stroke_color(color_type(0,0,0)),

                stroke_gradient_id(),

                stroke_opacity(1.0),
                stroke_width(1.0),
                stroke_linejoin(stroke_linejoin_miter),
                stroke_linecap(stroke_linecap_butt),
                stroke_miterlimit(4.0),

                transformer()
            {
                object_bbox[0] = 0;
                object_bbox[1] = 0;
                object_bbox[2] = 1;
                object_bbox[3] = 1;
            }
        };

	public:
        explicit attributes(const global_settings& settings);

        // Global Settings
        //-----------------------------------------------------------------------
        const global_settings& settings() const { return *m_settings; }

        double                 gamma()     const { return m_settings->gamma(); }
        const gamma_lut_type&  gamma_lut() const { return m_settings->gamma_lut(); }

        color_type gamma_color(color_type c) const 
        { 
            c.r = m_settings->gamma_dir(c.r);
            c.g = m_settings->gamma_dir(c.g);
            c.b = m_settings->gamma_dir(c.b);
            return c;
        }

        // Sessions
        //-----------------------------------------------------------------------
        void begin_session() { m_stack.begin_session(); }
        void end_session();
        unsigned num_sessions() const { return m_stack.num_sessions(); }

        //-----------------------------------------------------------------------
        void reset_all();

        void clear()
        {
            m_stack.clear();
            reset_all();
        }
        //-----------------------------------------------------------------------
        void window(int x1, int y1, int x2, int y2);
        void set_zoom(double x1, double y1, double x2, double y2);
        void initial_zoom();

        // General attributes and properties
        //-----------------------------------------------------------------------

        void viewBox(double scx, double scy, double scw, double sch,
                     double vbx, double vby, double vbw, double vbh, 
                     uniform_scaling_e align_type,
                     window_fit_logic_e meet_or_slice,
                     bool separate_window);
       
        void opacity(double op);
        void color(const color_type& c);
        void color(unsigned c);

        void fill_none();
        void fill_currentColor();
        void fill(const color_type& c);
        void fill(unsigned c);

        template<class DataAccessor> 
        void fill_gradient_id(DataAccessor gradient_id)
        {
            save_attribute(attr_fill, m_values.fill_type);
            save_serializable(attr_fill_gradient, m_values.fill_gradient_id);
            m_values.fill_type = paint_gradient;
            m_values.fill_gradient_id.assign_from(gradient_id);
        }

        void fill_opacity(double op);

        void stroke_none();
        void stroke_currentColor();
        void stroke(const color_type& c);
        void stroke(unsigned c);

        template<class DataAccessor> 
        void stroke_gradient_id(DataAccessor gradient_id)
        {
            save_attribute(attr_stroke, m_values.stroke_type);
            save_serializable(attr_stroke_gradient, m_values.stroke_gradient_id);
            m_values.stroke_type = paint_gradient;
            m_values.stroke_gradient_id.assign_from(gradient_id);
        }

		void stroke_opacity(double op);
        void fill_rule(fill_rule_e fe);
        void stroke_linejoin(stroke_linejoin_e lj);
        void stroke_linecap(stroke_linecap_e lc);
        void stroke_miterlimit(double ml);
        void stroke_width(double sw);

        void object_bbox(double x1, double y1, double x2, double y2);
        void object_bbox(const rectangle& bbox);

        void transform(const agg::trans_affine& mtx);
        void transform(double a0, double a1, double a2,
                       double a3, double a4, double a5);
        void translate(double dx, double dy);
        void rotate(double angle);
        void rotate(double angle, double cx, double cy);
        void scale(double s);
        void scale(double sx, double sy);
        void skew(double sx, double sy);
        void skew_x(double s);
        void skew_y(double s);

        // Accessors
        //-----------------------------------------------------------------------
        double             opacity()            const { return m_values.opacity; }
        paint_type_e       fill_type()          const { return m_values.fill_type; }
        color_type         fill_color()         const;
        double             fill_opacity()       const { return m_values.fill_opacity * opacity(); }
        const char_type*   fill_gradient_id()   const { return m_values.fill_gradient_id.data(); }

        fill_rule_e        fill_rule()          const { return m_values.fill_rule; }
        paint_type_e       stroke_type()        const { return m_values.stroke_type; }
        color_type         stroke_color()       const; 
        double             stroke_opacity()     const { return m_values.stroke_opacity * opacity(); }
        const char_type*   stroke_gradient_id() const { return m_values.stroke_gradient_id.data(); }
        double             stroke_width()       const { return m_values.stroke_width; }
        stroke_linejoin_e  stroke_linejoin()    const { return m_values.stroke_linejoin; }
        stroke_linecap_e   stroke_linecap()     const { return m_values.stroke_linecap; }
        double             stroke_miterlimit()  const { return m_values.stroke_miterlimit; }


        const transformer& transform()          const { return m_values.transformer; }
        transformer&       transform()                { return m_values.transformer; }
        double             scale()              const { return m_values.transformer.scale(); }
        double             object_x1()          const { return m_values.object_bbox[0]; }
        double             object_y1()          const { return m_values.object_bbox[1]; }
        double             object_x2()          const { return m_values.object_bbox[2]; }
        double             object_y2()          const { return m_values.object_bbox[3]; }

        int                window_x1()          const { return m_values.transformer.window_x1(); }
        int                window_y1()          const { return m_values.transformer.window_y1(); }
        int                window_x2()          const { return m_values.transformer.window_x2(); }
        int                window_y2()          const { return m_values.transformer.window_y2(); }

        double             clip_x1()            const { return m_values.transformer.clip_x1(); }
        double             clip_y1()            const { return m_values.transformer.clip_y1(); }
        double             clip_x2()            const { return m_values.transformer.clip_x2(); }
        double             clip_y2()            const { return m_values.transformer.clip_y2(); }

        int                buffer_x1()          const { return m_values.transformer.buffer_x1(); }
        int                buffer_y1()          const { return m_values.transformer.buffer_y1(); }
        int                buffer_x2()          const { return m_values.transformer.buffer_x2(); }
        int                buffer_y2()          const { return m_values.transformer.buffer_y2(); }
        bool               viewBox_is_valid()   const { return m_values.transformer.is_valid();  }
        bool               is_visible(double x1, double y1, double x2, double y2) const
        {
            return m_values.transformer.is_visible(x1, y1, x2, y2);
        }


        // Metrics
        //-----------------------------------------------------------------------
        double percent2pix(double percent) const;
        double percent2pix_x(double percent) const;
        double percent2pix_y(double percent) const;

        double conv_units(double v, units2_e u) const;

    private:
        template<class Serializer> 
        void save_serializable(unsigned attr, const Serializer& serializer)
        {
            if(!m_stack.attribute_saved(attr))
            {
                agg::int8u* ptr = m_stack.push_attribute(attr, serializer.byte_size());
                serializer.serialize(ptr);
            }
        }

        template<class Serializer> 
        void restore_serializable(Serializer& serializer, 
                                  const agg::int8u* data, 
                                  unsigned size)
        {
            serializer.deserialize(data, size);
        }


        template<class T>
        void save_attribute(attr_e attr, const T& data)
        {
            if(!m_stack.attribute_saved(attr))
            {
                agg::int8u* ptr = m_stack.push_attribute(attr, sizeof(T));
                memcpy(ptr, &data, sizeof(T));
            }
        }

        template<class T1, class T2>
        void save_attribute(unsigned attr, const T1& data1, const T2& data2)
        {
            if(!m_stack.attribute_saved(attr))
            {
                agg::int8u* ptr = m_stack.push_attribute(attr, sizeof(T1) + sizeof(T2));
                memcpy(ptr,              &data1, sizeof(T1));
                memcpy(ptr + sizeof(T1), &data2, sizeof(T2));
            }
        }

        template<class T>
        void restore_attribute(T& attr, const agg::int8u* data)
        {
            memcpy(&attr, data, sizeof(T));
        }

        template<class T1, class T2>
        void restore_attribute(T1& attr1, T2& attr2, const agg::int8u* data)
        {
            memcpy(&attr1, data,              sizeof(T1));
            memcpy(&attr2, data + sizeof(T1), sizeof(T2));
        }

        template<class ArrayType, class UnitsArrayAccessor>
        void deserialize_units_array(unsigned start, 
                                     ArrayType& array, 
                                     UnitsArrayAccessor data)
        {
            if(start == 0) array.remove_all();
            while(array.size() < start)
            {
                array.add(0.0);
            }

            for(unsigned i = 0; data.size(); i++)
            {
                double v = data.read_units_value(*this);
                if(start + i < array.size())
                {
                    array.at(start + i) = v;
                }
                else
                {
                    array.add(v);
                }
            }
        }

		void update_zoom();

        attr_stack                  m_stack;
        const global_settings*      m_settings;
        agg::pod_bvector<zoom_data> m_zoom_stack;
        values                      m_values;
    };

} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_ATTRIBUTES_INCLUDED

