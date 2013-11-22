#ifndef __SVGParser__
#define __SVGParser__

#include <vector>
#include <utility>
#include <algorithm>
#include <cassert>
#include <cstring>

namespace  SVG
{
	class Exception
	{
	public:
		exception() : m_msg(0) {}
		exception(const char* fmt, ...) :	m_msg(0)
		{
			if(fmt) 
			{
				m_msg = new char [4096];
				va_list arg;
				va_start(arg, fmt);
				vsprintf(m_msg, fmt, arg);
				va_end(arg);
			}
		}
		exception(const exception& exc) : m_msg(exc.m_msg ? new char[strlen(exc.m_msg) + 1] : 0)
		{
			if(m_msg) strcpy(m_msg, exc.m_msg);
		}

		~exception()		{		delete [] m_msg;		}
		const char* msg() const { return m_msg; }

	private:
		char* m_msg;
	};

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

	enum window_fit_logic_e
	{
		window_meet,
		window_slice
	};

	enum paint_type_e
	{
		paint_none,
		paint_color,
		paint_currentColor,
		paint_gradient,
		paint_id
	};

	enum uri_type_e
	{
		uri_none,       
		uri_id
	};
	
	enum gradient_e
	{
		gradient_linear,
		gradient_radial
	};

	enum objectUnits_e
	{
		objectUnits_userSpaceOnUse,
		objectUnits_objectBoundingBox,
		objectUnits_strokeWidth
	};


	enum spreadMethod_e
	{
		spreadMethod_pad,
		spreadMethod_reflect,
		spreadMethod_repeat
	};

	enum transform_e
	{
		transform_matrix,
		transform_translate,
		transform_rotate,
		transform_scale,
		transform_skew_x,
		transform_skew_y
	};

	enum stroke_linejoin_e
	{
		stroke_linejoin_miter = 0,
		stroke_linejoin_round = 1,
		stroke_linejoin_bevel = 2
	};

	enum stroke_linecap_e
	{
		stroke_linecap_butt   = 0,
		stroke_linecap_round  = 1,
		stroke_linecap_square = 2
	};

	enum fill_rule_e
	{
		fill_rule_nonzero = 0,
		fill_rule_evenodd = 1
	};

	enum units2_e
	{
		units_em,        // The font size of the default font, 
	
		units_ex,       

		units_px,        

		units_pt,
		units_pt_x,     
		units_pt_y,

		units_pc,
		units_pc_x,      
		units_pc_y,

		units_cm,
		units_cm_x,      
		units_cm_y,

		units_mm,
		units_mm_x,     
		units_mm_y,

		units_in,
		units_in_x,      
		units_in_y,

		units_percent,   
		units_percent_x,
		units_percent_y,

		units_deg,       
		units_grad,
		units_rad,
	};

	class AttributesMap
	{
		typedef std::vector<std::pair<const char*, attr_e> > map_type;
		map_type m_map;
		unsigned m_names_positions[end_of_attr];
	public:
		//---------------------------------------------------------------------
		AttributesMap()
		{

#define ADD_ATTR(name, value) \
	m_map.push_back(map_type::value_type(name, value));

			memset(m_names_positions, 0, sizeof(unsigned)*end_of_attr);
			m_map.reserve(end_of_attr);
			int index = 0;

			ADD_ATTR( "color",                        attr_color );
			ADD_ATTR( "cx",                           attr_cx );
			ADD_ATTR( "cy",                           attr_cy );
			ADD_ATTR( "d",                            attr_d );
			ADD_ATTR( "fill",                         attr_fill );
			ADD_ATTR( "fill-opacity",                 attr_fill_opacity );
			ADD_ATTR( "fill-rule",                    attr_fill_rule );
			ADD_ATTR( "fx",                           attr_fx );
			ADD_ATTR( "fy",                           attr_fy );
			ADD_ATTR( "gradientTransform",            attr_gradientTransform );
			ADD_ATTR( "gradientUnits",                attr_gradientUnits );
			ADD_ATTR( "height",                       attr_height );
			ADD_ATTR( "id",                           attr_id );
			ADD_ATTR( "offset",                       attr_offset );
			ADD_ATTR( "opacity",                      attr_opacity );
			ADD_ATTR( "points",                       attr_points );
			ADD_ATTR( "preserveAspectRatio",          attr_preserveAspectRatio );
			ADD_ATTR( "r",                            attr_r );
			ADD_ATTR( "rx",                           attr_rx );
			ADD_ATTR( "ry",                           attr_ry );
			ADD_ATTR( "spreadMethod",                 attr_spreadMethod );
			ADD_ATTR( "stop-color",                   attr_stop_color );
			ADD_ATTR( "stop-opacity",                 attr_stop_opacity );
			ADD_ATTR( "stroke",                       attr_stroke );
			ADD_ATTR( "stroke-linecap",               attr_stroke_linecap );
			ADD_ATTR( "stroke-linejoin",              attr_stroke_linejoin );
			ADD_ATTR( "stroke-miterlimit",            attr_stroke_miterlimit );
			ADD_ATTR( "stroke-opacity",               attr_stroke_opacity );
			ADD_ATTR( "stroke-width",                 attr_stroke_width );
			ADD_ATTR( "style",                        attr_style );
			ADD_ATTR( "transform",                    attr_transform );
			ADD_ATTR( "viewBox",                      attr_viewBox );
			ADD_ATTR( "width",                        attr_width );
			ADD_ATTR( "x",                            attr_x );
			ADD_ATTR( "x1",                           attr_x1 );
			ADD_ATTR( "x2",                           attr_x2 );
			ADD_ATTR( "xlink:href",                   attr_xlink_href );
			ADD_ATTR( "y",                            attr_y );
			ADD_ATTR( "y1",                           attr_y1 );
			ADD_ATTR( "y2",                           attr_y2 );

			//--------------------------------------
			std::sort(m_map.begin(), 
								m_map.end(),
								make_comparer
								(
								&map_type::value_type::first, 
								string_comparer()
								)
								);

			map_type::iterator it  = m_map.begin();
			map_type::iterator end = m_map.end(); 
			for (unsigned i = 0; it != end; ++it)
				m_names_positions[it->second] = i++;
		}
		//---------------------------------------------------------------------
		attr_e       get_type(const char * name)
		{
			typedef std::pair<const char*, attr_e>* iterator;
			std::pair<map_type::iterator,map_type::iterator> range;

			range = std::equal_range(m_map.begin(), m_map.end(), name, 
									make_comparer
									(
									&std::pair<const char*, attr_e>::first, 
									string_comparer()
									));

			return range.first != range.second ? range.first->second : end_of_attr;
		}
		//---------------------------------------------------------------------
		const char * get_name(unsigned attr)
		{
			assert(attr >= 0);
			assert(attr < end_of_attr);

				return m_map[m_names_positions[attr]].first;
		}
	};

}

#endif