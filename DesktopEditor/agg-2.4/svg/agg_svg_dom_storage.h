#ifndef AGG_SVG_DOM_STORAGE_INCLUDE
#define AGG_SVG_DOM_STORAGE_INCLUDE
//-----------------------------------------------------------------------------
#include "agg_svg_defines.h"
#include "agg_svg_basics.h"
#include "agg_svg_tags.h"
#include "agg_svg_dom_read_protocol.h"
#include "agg_svg_assoc_pod_array.h"
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    //---------------------------------------------------------------------
    // Attribute data. If the attribute is known, it has valid "code"
    // (see enum attr_e). If it's unknown, its "code" has value 
	// "end_of_attributes".
    // Fields "data" represent raw attribute data;
    // its interpretation depends on the attribute.
	//
    //----------------------------------------------------------------------
    struct attr_data
    {
        bool processed;

        attr_e             code;
        data_accessor_type data;
    };
    //-------------------------------------------------------------------------
    // Element data. Field "index" defines the starting index in the array
    // where the element begins.
    // If the attribute is known, it has valid "code"
    // (see enum element_e). If it's unknown, 
    // its code has value "end_of_elements".
	//
    //-------------------------------------------------------------------------
    struct element_data
    {
       agg::int32u       index;
       element_e         code;
       const agg::int8u* bounds;
       attr_data*        attr;
       unsigned          num_attr;

	   data_accessor_type shape_data;

       void clear()
       {
           index           = 0u;
           code            = end_of_elements;
           bounds          = 0;
           attr            = 0;
           num_attr        = 0u;
           shape_data.reset();
       }
    };
    //-------------------------------------------------------------------------
    template <class DataAccessor>
    struct data_accessor_compare
    {
        //---------------------------------------------------------------------
        typedef data_accessor<agg::pod_array_adaptor<char const> > string_type;
        //---------------------------------------------------------------------
        bool operator()(const DataAccessor& lhs, const DataAccessor& rhs)const
        {
            return lhs < rhs;
        }
        //---------------------------------------------------------------------
        bool operator()(const DataAccessor& lhs, const string_type& rhs)const
        {
            return lhs < rhs;
        }
        //---------------------------------------------------------------------
        bool operator()(const string_type& lhs, const DataAccessor& rhs)const
        {
            return lhs < rhs;
        }
    };
    //-------------------------------------------------------------------------
    class dom_storage
    {
    public:
        //---------------------------------------------------------------------
		struct element_location
		{
			agg::int32u idx;
			element_e   code;
		};

        typedef assoc_pod_array
                <
                  data_accessor_type, 
                  element_location,
                  data_accessor_compare<data_accessor_type> 
                > map_type;
        //---------------------------------------------------------------------
        void clear()
        {
            m_storage.clear();
			m_width = m_height = 0;
        }
        //---------------------------------------------------------------------
        void start_element(element_e);
        //---------------------------------------------------------------------
        void attribute(attr_e, const void* data, agg::int32u size);
        //---------------------------------------------------------------------
        void attribute(attr_e, agg::int8u type, const void* data, agg::int32u);
        //---------------------------------------------------------------------
        template<class T> 
        void attribute_datatype(attr_e attr, const T& data)
        {
            attribute(attr, &data, sizeof(data));
        }
        //---------------------------------------------------------------------
        template<class T> 
        void attribute_datatype(attr_e attr, agg::int8u type, const T& data)
        {
            attribute(attr, type, &data, sizeof(data));
        }
        //---------------------------------------------------------------------
        void attribute(attr_e, const data_accessor_type &);
        //---------------------------------------------------------------------
        void attribute_byte(attr_e, agg::int8u val);
        //---------------------------------------------------------------------
        void attribute_string(attr_e attr, const char* val);
        //---------------------------------------------------------------------
        void attribute(attr_e attr, const length & val);
        //---------------------------------------------------------------------
        void attribute_numeric(attr_e, double val);
        //---------------------------------------------------------------------
        void attribute_unsigned(attr_e, unsigned val);
        //---------------------------------------------------------------------
        void attribute_percent(attr_e attr, double val);
        //---------------------------------------------------------------------
        void attribute_transform_matrix(double m0, double m1, double m2, 
            double m3, double m4, double m5);
        //---------------------------------------------------------------------
        void attribute_transform_translate(double dx, double dy);
        //---------------------------------------------------------------------
        void attribute_transform_rotate(double angle);
        //---------------------------------------------------------------------
        void attribute_transform_rotate(double angle, double cx, double cy);
        //---------------------------------------------------------------------
        void attribute_transform_scale(double s);
        //---------------------------------------------------------------------
        void attribute_transform_scale(double sx, double sy);
        //---------------------------------------------------------------------
        void attribute_transform_skew_x(double sx);
        //---------------------------------------------------------------------
        void attribute_transform_skew_y(double sy);
        //---------------------------------------------------------------------

        //---------------------------------------------------------------------
        void attribute_d_with_bounds(data_accessor_type data);
        //---------------------------------------------------------------------
        void attribute_points_with_bounds(data_accessor_type data);
        //---------------------------------------------------------------------

        //---------------------------------------------------------------------
        void end_element();
        //---------------------------------------------------------------------
        const data_container & storage()const { return m_storage.buffer(); }
		      data_container & storage()      { return m_storage.buffer(); }
        //---------------------------------------------------------------------
		data_accessor_type get_attr_specified_value(unsigned offset, attr_e code)const;
        //---------------------------------------------------------------------
    private:
        //---------------------------------------------------------------------
        void attribute_short(attr_e, const void* data, agg::int8u size);
        //---------------------------------------------------------------------
        void attribute_short(attr_e, agg::int8u type,
            const void* data, agg::int8u size);
        //---------------------------------------------------------------------

    private:
        //---------------------------------------------------------------------
        struct element_stack_data
        {
            unsigned  index;
            element_e code;
        };
        //---------------------------------------------------------------------
        typedef agg::pod_bvector<element_stack_data, 5> element_stack_type;
        //---------------------------------------------------------------------
        void push_element(const element_data&, element_stack_type&) const;
        //---------------------------------------------------------------------
        void pop_element(element_stack_type&) const;
        //---------------------------------------------------------------------

        //---------------------------------------------------------------------
        typedef agg::pod_vector<attr_data>         attributes_type;
        typedef agg::pod_bvector<agg::int8u, 10>   traverse_buffer_type;
        //---------------------------------------------------------------------
        void collect_attributes(read_protocol const&  rp, 
                                element_data&         el, 
                                attributes_type&      attributes, 
                                traverse_buffer_type& traverse_buffer) const;
        //---------------------------------------------------------------------
        void proceed_attribute(attributes_type&       attributes, 
                               traverse_buffer_type&  traverse_buffer,
                               element_data&          el, 
                               attr_data&             attr, 
                               data_container const&  cont,
                               agg::int32u            data_idx,
                               agg::int32u            data_size) const;
        //---------------------------------------------------------------------

   public:
        //---------------------------------------------------------------------
        template <class Consumer> 
        agg::int32u traverse(Consumer& cons, agg::int32u idx = 0) const
        {
            traverse_buffer_type  traverse_buffer(32);
            element_stack_type    element_stack(32);
            attributes_type       attributes(256);
            element_data          el;
            const data_container* cont = 0;
            agg::int32u           data_idx  = 0u;
            agg::int32u           data_size = 0u;
            bool                  not_done  = true;
            read_protocol         rp(*this);

            rp.rewind(idx);

            while (not_done && !rp.end())
            {
                tag_e tag = rp.read_tag();
                switch (tag)
                {
                case tag_end_element:           
                    el.clear();

					rp.end_element();

                    if (element_stack.size())
                    {
                        const element_stack_data& esd = element_stack.last();
                        el.index = esd.index;
                        el.code  = esd.code;
                    }
                    not_done = cons.end_element(*this, el);
                    pop_element(element_stack);
                    if (element_stack.size() == 0) not_done = false; 
                    break;

                case tag_element_bin:           
                    el.clear();
                    rp.element_bin(&el.index, &el.code);
                    collect_attributes(rp, el, attributes, traverse_buffer);
                    push_element(el, element_stack);
                    not_done = cons.start_element(*this, el);
                    break;

                default:
                    assert(false);
                }
            }

            return rp.current_location();
        }

    private:
        iomemstream m_storage;

		/***************************************************************************/
	private:
		double      m_width;
		double      m_height;
	public:
		double      get_width() const {return m_width;};
		double      get_height() const  {return m_height;};
    };

} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_DOM_STORAGE_INCLUDE

