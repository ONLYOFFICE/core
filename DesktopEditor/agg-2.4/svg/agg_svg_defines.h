#ifndef AGG_SVG_DEFINES_INCLUDED
#define AGG_SVG_DEFINES_INCLUDED

namespace agg
{
namespace svg
{
    // Element types
    //-------------------------------------------------------------------------
    enum element_e
    {
        elem_circle,         
		elem_clipPath,
        elem_defs,       
        elem_ellipse,
        elem_g,
        elem_line,
        elem_linearGradient,
        elem_path,
        elem_polygon,
        elem_polyline,
        elem_radialGradient,
        elem_rect,
        elem_stop,
        elem_svg,
        elem_title,
        elem_use,            
    
        end_of_elements
    };

    //-------------------------------------------------------------------------
    enum attr_e
    {
        attr_color = 0, 
        attr_cx, 
        attr_cy, 
        attr_d, 
        attr_fill,
        attr_fill_color,
        attr_fill_gradient,
        attr_fill_opacity, 
        attr_fill_rule, 
        attr_fx, 
        attr_fy, 
        attr_gradientTransform, 
        attr_gradientUnits, 
        attr_height, 
        attr_id,
        attr_object_bbox,
        attr_offset, 
        attr_opacity, 
        attr_points, 
        attr_preserveAspectRatio, 
        attr_r, 
        attr_rx, 
        attr_ry, 
        attr_spreadMethod, 
        attr_stop_color, 
        attr_stop_opacity, 
        attr_stroke, 
        attr_stroke_color, 
        attr_stroke_gradient, 
        attr_stroke_linecap, 
        attr_stroke_linejoin, 
        attr_stroke_miterlimit, 
        attr_stroke_opacity, 
        attr_stroke_width, 
        attr_style, 
        attr_transform, 
        attr_viewBox, 
        attr_width, 
        attr_x, 
        attr_x1, 
        attr_x2, 
        attr_xlink_href, 
        attr_y, 
        attr_y1, 
        attr_y2, 

        attr_bounds,
 
        end_of_attr
    };

    //-----------------------------------------------------------uniform_scaling_e
    enum uniform_scaling_e
    {
        usc_none,   
        usc_xMinYMin,
        usc_xMidYMin,
        usc_xMaxYMin,
        usc_xMinYMid,
        usc_xMidYMid,
        usc_xMaxYMid,
        usc_xMinYMax,
        usc_xMidYMax,
        usc_xMaxYMax
    };

    //---------------------------------------------------------window_fit_logic_e
    enum window_fit_logic_e
    {
        window_meet,
        window_slice
    };

    //---------------------------------------------------------------paint_type_e
    enum paint_type_e
    {
        paint_none,
        paint_color,
        paint_currentColor,
        paint_gradient,
        paint_id
    };

	//---------------------------------------------------------------uri_type_e
    enum uri_type_e
    {
        uri_none,       
        uri_id
    };
    //----------------------------------------------------------------gradient_e
    enum gradient_e
    {
        gradient_linear,
        gradient_radial
    };

    //-------------------------------------------------------------objectUnits_e
    enum objectUnits_e
    {
        objectUnits_userSpaceOnUse,
        objectUnits_objectBoundingBox,
        objectUnits_strokeWidth
    };


    //------------------------------------------------------------spreadMethod_e
    enum spreadMethod_e
    {
        spreadMethod_pad,
        spreadMethod_reflect,
        spreadMethod_repeat
    };

    //---------------------------------------------------------------transform_e
    enum transform_e
    {
        transform_matrix,
        transform_translate,
        transform_rotate,
        transform_scale,
        transform_skew_x,
        transform_skew_y
    };

    //----------------------------------------------------------stroke_linejoin_e
    enum stroke_linejoin_e
    {
        stroke_linejoin_miter = 0,
        stroke_linejoin_round = 1,
        stroke_linejoin_bevel = 2
    };


    //-----------------------------------------------------------stroke_linecap_e
    enum stroke_linecap_e
    {
        stroke_linecap_butt   = 0,
        stroke_linecap_round  = 1,
        stroke_linecap_square = 2
    };
    

    //----------------------------------------------------------------fill_rule_e
    enum fill_rule_e
    {
        fill_rule_nonzero = 0,
        fill_rule_evenodd = 1
    };

    //-----------------------------------------------------------------------------
    enum units2_e
    {
        units_em,        // The font size of the default font, 
                          // usually equivalent to the height of a character

        units_ex,        // The height of the letter x

        units_px,        // Pixels

        units_pt,
        units_pt_x,      // Points (1/72 of an inch)
        units_pt_y,

        units_pc,
        units_pc_x,      // Picas  (1/6 of an inch)
        units_pc_y,

        units_cm,
        units_cm_x,      // Centimeters
        units_cm_y,

        units_mm,
        units_mm_x,      // Millimeters
        units_mm_y,

        units_in,
        units_in_x,      // Inches
        units_in_y,

        units_percent,   // Percent
        units_percent_x,
        units_percent_y,

        units_deg,       // Angle units
        units_grad,
        units_rad,
    };
   

} // namespace svg
} // namespace agg


#endif // ifndef AGG_SVG_DEFINES_INCLUDED

