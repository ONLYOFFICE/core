#include "agg_svg_dom_storage.h"
#include "agg_svg_ptr_size_pair.h"
#include "agg_svg_basics.h"
#include "agg_svg_path_interpreter.h"
#include "agg_bounding_rect.h"
#include "agg_svg_rendering_interpreter.h"
#include "agg_svg_attribute_source.h"
#include "agg_svg_percent.h"
//-----------------------------------------------------------------------------
#include <cstring> // for std::strlen, std::memcpy
//-----------------------------------------------------------------------------
namespace
{
	bool find_attribute(agg::svg::read_protocol const& rp, agg::svg::attr_data& attr) 
    {
		using namespace agg::svg;

        attr_e       attr_type = end_of_attr;
        agg::int32u  data_size = 0u;
        bool         find      = false;

        const data_container* cont = 0;
        agg::int32u           data_idx = 0;

        while (!rp.end())
        {
            tag_e tag = rp.read_tag();

            switch (tag)
            {
            case tag_attribute_bin_byte: 
                rp.attribute_bin_byte(&cont, &attr_type, &data_idx);
                if (attr.code == attr_type)
                {
                    attr.data.init(*cont, data_idx, 1);
                    return true;
                }
                break;
            case tag_attribute_bin:
                rp.attribute_bin(&cont, &attr_type, &data_idx, &data_size);
                if (attr.code == attr_type)
                {
                    assert(!find);
                    attr.data.init(*cont, data_idx, data_size);
                    find = true;
                }
                break;
            case tag_attribute_bin_short: 
                rp.attribute_bin_short(&cont, &attr_type, &data_idx, &data_size);
                if (attr.code == attr_type)
                {
                    assert(!find);
                    attr.data.init(*cont, data_idx, data_size);
                    find = true;
                }
                break;

            default:
                rp.unread(1);
                return find;
            }
        }
        assert(false);
        return find;
    }
}
//-----------------------------------------------------------------------------
namespace agg
{
	namespace svg
{

    //-------------------------------------------------------------------------
    namespace 
    {
        void read_raw_data(data_container const & cont, agg::int8u* ptr, 
                           unsigned size, unsigned idx)
        {
            for (unsigned i = 0u; i < size; ++i)
            {
                *ptr++ = cont[idx++];
            }
        }
    }
    //-------------------------------------------------------------------------
    iomemstream& operator<<(iomemstream& s, tag_e tag) 
    { 
        return s << (agg::int8u)tag;
    }
    //-------------------------------------------------------------------------
    iomemstream& operator<<(iomemstream& s, attr_e attr) 
    { 
        return s << (agg::int8u)attr;
    }
    //-------------------------------------------------------------------------
    void dom_storage::start_element(element_e el)
    {
        m_storage << tag_element_bin << (agg::int8u)el;
    }
    //-------------------------------------------------------------------------
    void dom_storage::end_element() 
    { 
        m_storage << tag_end_element; 
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute(attr_e attr, const void * data, 
                                agg::int32u size)
    {
        assert(data && size);

        if (size <= 255)
        {
            attribute_short(attr, data, size);
        }
        else
        {
			agg::svg::attribute_bin src(attr, data, size);
            m_storage.add(src);
        }
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute_short(attr_e attr, const void* data, 
                                      agg::int8u size)
    {
		agg::svg::attribute_bin_short src(attr, data, size);
        m_storage.add(src);
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute(attr_e attr, agg::int8u type, 
                                const void* data, agg::int32u size)
    {
        if (size <= 254)
        {
            attribute_short(attr, type, data, size);
        }
        else
        {
			agg::svg::attribute_bin src(attr, type, data, size);
            m_storage.add(src);
        }
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute_short(attr_e attr, agg::int8u type, 
                                      const void* data, agg::int8u size)
    {
		agg::svg::attribute_bin_short src(attr, type, data, size);
        m_storage.add(src);
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute_byte(attr_e attr, agg::int8u val)
    {
		agg::svg::attribute_byte src(attr, val);
        m_storage.add(src);
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute_string(attr_e attr, const char* val)
    {
        using namespace std;
        attribute(attr, val, static_cast<agg::int32u>(strlen(val)));
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute(attr_e attr, const length & val)
    {
        m_storage << tag_attribute_bin_short 
                 << attr 
                 << agg::int8u(sizeof(coord_type) + 1);

        m_storage.write(&val.value, sizeof(coord_type));
        m_storage << (agg::int8u)val.unit;

		if (attr==attr_width)
			m_width = val.value;
		if (attr==attr_height)
			m_height = val.value;
    }  
    //-------------------------------------------------------------------------
    void dom_storage::attribute_numeric(attr_e attr, double val)
    {
        coord_type data = static_cast<coord_type>(val);
        attribute_short(attr, &data, sizeof(data));
    }  
    //-------------------------------------------------------------------------
    void dom_storage::attribute_unsigned(attr_e attr, unsigned val)
    {
        attribute_short(attr, &val, sizeof(unsigned));
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute_percent(attr_e attr, double val)
    {
        attribute_numeric(attr, percent::encrypt(val));
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute(attr_e attr, const data_accessor_type & data)
    {
        if (data.size() <= 255u)
        {
            m_storage << tag_attribute_bin_short 
                     << attr
                     << static_cast<agg::int8u>(data.size())
                     << data;
        }
        else
        {
            m_storage << tag_attribute_bin << attr << data.size() << data;
        }
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute_transform_matrix(double m0, double m1, 
                                                 double m2, double m3,
                                                 double m4, double m5)
    {
        coord_type data[] = {coord_type(m0), coord_type(m1),
                             coord_type(m2), coord_type(m3),
                             coord_type(m4), coord_type(m5)};

        attribute_short(attr_transform, transform_matrix, data, sizeof(data));
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute_transform_translate(double dx, double dy)
    {
        coord_type data[] = {coord_type(dx), coord_type(dy)};

        attribute_short(attr_transform, transform_translate, data, sizeof(data));
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute_transform_rotate(double angle)
    {
        coord_type data = coord_type(angle);

        attribute_short(attr_transform, transform_rotate, &data, sizeof(data));
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute_transform_rotate(double angle, 
                                                 double cx, double cy)
    {
        coord_type data[] = {coord_type(angle), 
                             coord_type(cx), 
                             coord_type(cy)}; 

        attribute_short(attr_transform, transform_rotate, &data, sizeof(data));
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute_transform_scale(double s)
    {
        coord_type data = coord_type(s);

        attribute_short(attr_transform, transform_scale, &data, sizeof(data));
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute_transform_scale(double sx, double sy)
    {
        coord_type data[] = {coord_type(sx), coord_type(sy)};

        attribute_short(attr_transform, transform_scale, &data, sizeof(data));
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute_transform_skew_x(double sx)
    {
        coord_type data = coord_type(sx);

        attribute_short(attr_transform, transform_skew_x, &data, sizeof(data));
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute_transform_skew_y(double sy)
    {
        coord_type data = coord_type(sy);

        attribute_short(attr_transform, transform_skew_y, &data, sizeof(data));
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute_d_with_bounds(data_accessor_type data)
    {
        coord_type bounds[4];
        path_interpreter<data_accessor_type> p(data);
        agg::bounding_rect_single(p, 0, 
                                  &bounds[0], &bounds[1], 
                                  &bounds[2], &bounds[3]);
        attribute(attr_bounds, bounds, sizeof(bounds));
        attribute(attr_d, data);
    }
    //-------------------------------------------------------------------------
    void dom_storage::attribute_points_with_bounds(data_accessor_type data)
    {
        coord_type bounds[4];
        poly_interpreter<data_accessor_type> p(data, false);
        agg::bounding_rect_single(p, 0, 
                                  &bounds[0], &bounds[1], 
                                  &bounds[2], &bounds[3]);
        attribute(attr_bounds, bounds, sizeof(bounds));
        attribute(attr_points, data);
    }
    //-------------------------------------------------------------------------
    data_accessor_type dom_storage::get_attr_specified_value(unsigned offset, attr_e code)const
    {
        attr_data attr;
        attr.code = code;

        read_protocol rp(*this);
        rp.rewind(offset);

        if (rp.read_tag() == tag_element_bin) 
        {
            element_data el;
            el.clear();

            rp.element_bin(&el.index, &el.code);

            find_attribute(rp, attr);
        }
        return attr.data;
    }
    //-------------------------------------------------------------------------
    void dom_storage::push_element(const element_data&  el, 
                                   element_stack_type&  element_stack) const
    {
        element_stack_data esd;
        
        esd.index = el.index;
        esd.code  = el.code;  

        element_stack.add(esd);
    }
    //-------------------------------------------------------------------------
    void dom_storage::pop_element(element_stack_type& element_stack) const
    {
        if (element_stack.size())
        {
            element_stack.remove_last();
        }
    }
    //-------------------------------------------------------------------------
    void dom_storage::proceed_attribute(attributes_type&      attributes, 
                                        traverse_buffer_type& traverse_buffer,
                                        element_data&         el,
                                        attr_data&            attr, 
                                        data_container const& cont,
                                        agg::int32u           data_idx,
                                        agg::int32u           data_size) const
    {
        int i = -1;

        switch (attr.code)
        {
        case attr_points:    
        case attr_d:
            el.shape_data.init(cont, data_idx, data_size);
            break;

        case attr_bounds:
            i = traverse_buffer.allocate_continuous_block(data_size);
            if (i < -1)
            {
				el.bounds = 0;
            }
			else
			{
				read_raw_data(cont, &traverse_buffer[i], data_size, data_idx);
				el.bounds = &traverse_buffer[i];
			}
            break;

        default:
            attr.processed = false;
            attr.data.init(cont, data_idx, data_size);
            if (attributes.size() >= attributes.capacity())
            {
                attributes.resize(attributes.size() + 256);
            }
            attributes.add(attr);
        }
    }
    //-------------------------------------------------------------------------
    void dom_storage::collect_attributes(read_protocol const& rp, 
                                 element_data& el, attributes_type& attributes,
                                 traverse_buffer_type& traverse_buffer) const
    {
        attr_data  attr;
        unsigned   data_idx;
        unsigned   data_size;
        int        i = -1;

        const data_container* cont = 0;

        attributes.remove_all();
        traverse_buffer.remove_all();

        while (!rp.end())
        {
            tag_e tag = rp.read_tag();

            switch (tag)
            {
            default:
                el.attr     = &attributes[0];
                el.num_attr = attributes.size();
                
                rp.unread(1);
                return;
            case tag_attribute_bin:
                rp.attribute_bin(&cont, &attr.code, &data_idx, &data_size);
                proceed_attribute(attributes, traverse_buffer, el, attr, *cont, data_idx, data_size);
                break;

            case tag_attribute_bin_short: 
                rp.attribute_bin_short(&cont, &attr.code, &data_idx, &data_size);
                proceed_attribute(attributes, traverse_buffer, el, attr, *cont, data_idx, data_size);
                break;

            case tag_attribute_bin_byte:
                rp.attribute_bin_byte(&cont, &attr.code, &data_idx);
                proceed_attribute(attributes, traverse_buffer, el, attr, *cont, data_idx, 1);
                break;
            }
        }
    }
}
}