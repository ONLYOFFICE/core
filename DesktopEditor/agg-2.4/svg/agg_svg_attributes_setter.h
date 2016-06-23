#ifndef AGG_SVG_ATTRIBUTES_SETTER_INCLUDE
#define AGG_SVG_ATTRIBUTES_SETTER_INCLUDE
//-----------------------------------------------------------------------------
#include "agg_svg_dom_storage.h"
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    //-------------------------------------------------------------------------
    class attributes;
    class rendering_interpreter;
    //-------------------------------------------------------------------------
    class attributes_setter
    {
    public:
        //---------------------------------------------------------------------
        attributes_setter(attributes&);
        //---------------------------------------------------------------------
        void set_interpreter(rendering_interpreter*);
        //---------------------------------------------------------------------
        void set_paint_attributes              (attr_data*, unsigned);
        void set_color_attributes              (attr_data*, unsigned);
        void set_opacity_attributes            (attr_data*, unsigned);
        void set_transform_attributes          (attr_data*, unsigned);
        void set_basic_shapes_common_attributes(attr_data*, unsigned);
        void set_presentation_attributes       (attr_data*, unsigned);
        void set_xlink_attributes              (attr_data*, unsigned);
        //---------------------------------------------------------------------
        template <class Consumer>
        void set_transform(Consumer& cons, const data_accessor_type& data)
        {
            assert(data.size() >= 1);

            transform_e t = transform_e(*data);

            switch (t)
            {
            //-----------------------------------------------------------------
            case transform_matrix:
                assert(data.size() == 1 + 6 * sizeof(coord_type));
                cons.transform(data.coord_at(1),
                               data.coord_at(1 + sizeof(coord_type)),
                               data.coord_at(1 + sizeof(coord_type) * 2),
                               data.coord_at(1 + sizeof(coord_type) * 3),
                               data.coord_at(1 + sizeof(coord_type) * 4),
                               data.coord_at(1 + sizeof(coord_type) * 5));
                break;
            //-----------------------------------------------------------------
            case transform_translate:
                assert(data.size() == 1 + 2 * sizeof(coord_type));
                cons.translate(data.coord_at(1), 
                               data.coord_at(1 + sizeof(coord_type)));
                break;
            //-----------------------------------------------------------------
            case transform_rotate:
                assert(data.size() >= 1 + sizeof(coord_type));
                if (data.size() - 1 == sizeof(coord_type))
                {
                    cons.rotate(data.coord_at(1));
                }
                else
                {
                    assert(data.size() == 1 + 3 * sizeof(coord_type));
                    cons.rotate(data.coord_at(1),
                                data.coord_at(1 + sizeof(coord_type)),
                                data.coord_at(1 + sizeof(coord_type) * 2));
                }
                break;
            //-----------------------------------------------------------------
            case transform_scale:
                assert(data.size() >= 1 + sizeof(coord_type));
                if (data.size() - 1 == sizeof(coord_type))
                {
                    cons.scale(data.coord_at(1));
                }
                else
                {
                    assert(data.size() == 1 + 2 * sizeof(coord_type));
                    cons.scale(data.coord_at(1),
                               data.coord_at(1 + sizeof(coord_type)));
                }
                break;
            //-----------------------------------------------------------------
            case transform_skew_x:
                assert(data.size() == 1 + sizeof(coord_type));
                cons.skew_x(data.coord_at(1));
                break;
            //-----------------------------------------------------------------
            case transform_skew_y:
                assert(data.size() == 1 + sizeof(coord_type));
                cons.skew_y(data.coord_at(1));
                break;
            //-----------------------------------------------------------------
            default: 
                assert(false);
            }
        }
        //---------------------------------------------------------------------

    private:
        //---------------------------------------------------------------------
        void set_fill  (const data_accessor_type&);
        //---------------------------------------------------------------------
        void set_stroke(const data_accessor_type&);
        //---------------------------------------------------------------------
        void set_fill_for_id  (data_accessor_type);
        void set_stroke_for_id(data_accessor_type);
        //---------------------------------------------------------------------
    
    private:
        attributes&              m_attributes;
        rendering_interpreter*   m_interpreter;
    };

} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_ATTRIBUTES_SETTER_INCLUDE
