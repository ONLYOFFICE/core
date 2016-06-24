//-----------------------------------------------------------------------------
#include "agg_svg_attributes_setter.h"
#include "agg_svg_attributes.h"
#include "agg_svg_rendering_interpreter.h"
#include <string>
//-----------------------------------------------------------------------------
namespace agg
{
	namespace svg
{

    //-------------------------------------------------------------------------
    attributes_setter::attributes_setter(attributes& at)
        :	m_attributes(at)
		,	m_interpreter(0)
    {
    }
    //-------------------------------------------------------------------------
	void attributes_setter::set_interpreter(rendering_interpreter* rin)
	{
		 m_interpreter = rin;
	}
    //-------------------------------------------------------------------------
    void attributes_setter::set_xlink_attributes(attr_data* attr, 
                                                     unsigned num_attr)
    {
        element_e element;

        for (unsigned i = 0; i < num_attr; ++i)
        {
            if (!attr[i].processed) switch (attr[i].code)
            {
            case attr_xlink_href:
                assert(attr[i].data.size() > 0);
                element = m_interpreter->current_element();

                if (element == elem_linearGradient || 
                    element == elem_radialGradient )
                {
                    m_interpreter->process_xlink_href(attr[i].data);
                }

                attr[i].processed = true;
                break;
            }
        }
    }
    //-------------------------------------------------------------------------
    void attributes_setter::set_paint_attributes(attr_data* attr, unsigned num_attr)
    {
        for (unsigned i = 0; i < num_attr; ++i)
        {
            if (!attr[i].processed) switch (attr[i].code)
            {
            case attr_color:
                assert(attr[i].data.size() == sizeof(agg::rgba8));
                m_attributes.color(attr[i].data.color_at(0)); 
                attr[i].processed = true;
                break;
            case attr_fill:
                set_fill(attr[i].data); 
                attr[i].processed = true;
                break;
            case attr_fill_rule:
                assert(attr[i].data.size() == 1);
                m_attributes.fill_rule(fill_rule_e(*attr[i].data));
                attr[i].processed = true;
                break;
            case attr_stroke:
                set_stroke(attr[i].data);
                attr[i].processed = true;
                break;
            case attr_stroke_linecap:
                assert(attr[i].data.size() == 1);
                m_attributes.stroke_linecap(stroke_linecap_e(*attr[i].data));
                attr[i].processed = true;
                break;
            case attr_stroke_linejoin:
                assert(attr[i].data.size() == 1);
                m_attributes.stroke_linejoin(stroke_linejoin_e(*attr[i].data));
                attr[i].processed = true;
                break;
            case attr_stroke_miterlimit:
                assert(attr[i].data.size() == sizeof(coord_type));
                m_attributes.stroke_miterlimit(attr[i].data.coord_at(0));
                attr[i].processed = true;
                break;
            case attr_stroke_width:
                assert(attr[i].data.size() == sizeof(coord_type) + 1);
                m_attributes.stroke_width(attr[i].data.units_value(m_attributes));
                attr[i].processed = true;
                break;
            }
        }
    }
    //-------------------------------------------------------------------------
    void attributes_setter::set_color_attributes(attr_data* attr, 
                                                     unsigned num_attr)
    {
        for (unsigned i = 0; i < num_attr; ++i)
        {
            if (!attr[i].processed && attr[i].code == attr_color)
            {
                assert(attr[i].data.size() == sizeof(agg::rgba8));
                m_attributes.color(attr[i].data.color_at(0)); 
                attr[i].processed = true;
            }
        }
    }
    //-------------------------------------------------------------------------
    void attributes_setter::set_opacity_attributes(attr_data* attr, 
                                                       unsigned num_attr)
    {
        for (unsigned i = 0; i < num_attr; ++i)
        {
            if (!attr[i].processed) switch (attr[i].code)
            {
            case attr_opacity:
                assert(attr[i].data.size() == sizeof(coord_type));
                m_attributes.opacity(attr[i].data.coord_at(0) * m_attributes.opacity());
                attr[i].processed = true;
                break;
            case attr_stroke_opacity:
                assert(attr[i].data.size() == sizeof(coord_type));
                m_attributes.stroke_opacity(attr[i].data.coord_at(0));
                attr[i].processed = true;
                break;
            case attr_fill_opacity:
                assert(attr[i].data.size() == sizeof(coord_type));
                m_attributes.fill_opacity(attr[i].data.coord_at(0));
                attr[i].processed = true;
                break;
            }
        }
    }
    //-------------------------------------------------------------------------
    void attributes_setter::set_transform_attributes(attr_data* attr, 
                                                         unsigned num_attr)
    {
        for (unsigned i = 0; i < num_attr; ++i)
        {
            if (!attr[i].processed && attr[i].code == attr_transform)
            {
                set_transform(m_attributes, attr[i].data);
                attr[i].processed = true;
            }
        }
    }
    //-------------------------------------------------------------------------
    void attributes_setter::set_presentation_attributes(attr_data* attr, 
                                                            unsigned num_attr)
    {
        set_paint_attributes   (attr, num_attr);
        set_color_attributes   (attr, num_attr);   
        set_opacity_attributes (attr, num_attr);
        set_xlink_attributes   (attr, num_attr);
    }
    //-------------------------------------------------------------------------
    void attributes_setter::set_basic_shapes_common_attributes(
                                                             attr_data* attr, 
                                                             unsigned num_attr)
    {
        set_paint_attributes     (attr, num_attr);
        set_color_attributes     (attr, num_attr);
        set_opacity_attributes   (attr, num_attr);
        set_transform_attributes (attr, num_attr);
    }
    //-------------------------------------------------------------------------
    void attributes_setter::set_stroke_for_id(data_accessor_type data)
    {

        ++data; // skip paint type
 
        switch (m_interpreter->interpret_element(data))
        {
        case elem_linearGradient:
        case elem_radialGradient:
            m_attributes.stroke_gradient_id(data);
            break;
        default:
            m_attributes.stroke_none(); 
            assert(false);
        }
    }
    //-------------------------------------------------------------------------
    void attributes_setter::set_fill_for_id(data_accessor_type data)
    {
        ++data; // skip paint type

        switch (m_interpreter->interpret_element(data))
        {
        case elem_linearGradient:
        case elem_radialGradient:
            m_attributes.fill_gradient_id(data);
            break;
        default:
            m_attributes.fill_none(); 
            assert(false);
        }
    }
    //-------------------------------------------------------------------------
    void attributes_setter::set_fill(const data_accessor_type& data)
    {
        assert(data.size() >= 1);

		paint_type_e p = paint_type_e(*data);
        switch (p)
        {
        //---------------------------------------------------------------------
        case paint_none:         
            assert(data.size() == 1);
            m_attributes.fill_none(); 
            break;
        //---------------------------------------------------------------------
        case paint_currentColor: 
            assert(data.size() == 1);
            m_attributes.fill_currentColor(); 
            break;
        //---------------------------------------------------------------------
        case paint_color:        
            assert(data.size() == 1 + sizeof(agg::rgba8));
            m_attributes.fill(data.color_at(1)); 
            break;
        //---------------------------------------------------------------------
        case paint_id:     
            set_fill_for_id(data);
            break; 
        //---------------------------------------------------------------------
        case paint_gradient:     
            assert(false);
        //---------------------------------------------------------------------
        default:
            assert(false);
        }
    }
    //-------------------------------------------------------------------------
    void attributes_setter::set_stroke(const data_accessor_type& data)
    {
        assert(data.size() >= 1);

        paint_type_e p = paint_type_e(*data);
        switch (p)
        {
        //---------------------------------------------------------------------
        case paint_none:         
            assert(data.size() == 1);
            m_attributes.stroke_none(); 
            break;
        //---------------------------------------------------------------------
        case paint_currentColor: 
            assert(data.size() == 1);
            m_attributes.stroke_currentColor(); 
            break;
        //---------------------------------------------------------------------
        case paint_color:        
            assert(data.size() == sizeof(agg::rgba8) + 1);
            m_attributes.stroke(data.color_at(1)); 
            break;
        //---------------------------------------------------------------------
        case paint_id:           
            set_stroke_for_id(data);
            break; 
        //---------------------------------------------------------------------
        case paint_gradient:     
            assert(false);
        //---------------------------------------------------------------------
        default:
            assert(false);
        }
    }
    //-------------------------------------------------------------------------

} 
}