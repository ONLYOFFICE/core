/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "draw_base.h"

#include "../../DataTypes/common_attlists.h"
#include "../../DataTypes/targetframename.h"

namespace cpdoccore {
namespace odf_writer {

class draw_shape : public draw_base
{  
public:
    static const wchar_t * ns;
    static const wchar_t * name;
   
	static const ElementType type = typeDrawShape;


    virtual void serialize(std::wostream & _Wostream);
    virtual void serialize_attlist(CP_ATTR_NODE);
	
	odf_types::common_xlink_attlist		common_xlink_attlist_;
	
	_CP_OPT(std::wstring)		draw_id_;//используется для анимашек
  

	

	int sub_type_;

};
//----------------------------------------------------------------------------------------------
class draw_rect_attlist
{
public:

	_CP_OPT(odf_types::length)	draw_corner_radius_;
	_CP_OPT(std::wstring)		draw_filter_name_;
	
	void serialize(CP_ATTR_NODE);   
};

class draw_rect : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
   
	static const ElementType type = typeDrawRect;


    virtual void serialize(std::wostream & _Wostream);

    draw_rect_attlist draw_rect_attlist_;    

};

CP_REGISTER_OFFICE_ELEMENT2(draw_rect);
//----------------------------------------------------------------------------------------------
class draw_ellipse_attlist
{
public:
    _CP_OPT(std::wstring) draw_filter_name_;
	void serialize(CP_ATTR_NODE);   

};

class draw_ellipse : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawEllipse;


    virtual void serialize(std::wostream & _Wostream);

	draw_ellipse_attlist draw_ellipse_attlist_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_ellipse);
//----------------------------------------------------------------------------------------------
class draw_circle : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawCircle;


    virtual void serialize(std::wostream & _Wostream);
};

CP_REGISTER_OFFICE_ELEMENT2(draw_circle);
//----------------------------------------------------------------------------------------------
class draw_line_attlist
{
public:
	_CP_OPT(odf_types::length) svg_x1_;
    _CP_OPT(odf_types::length) svg_y1_;

	_CP_OPT(odf_types::length) svg_x2_;
    _CP_OPT(odf_types::length) svg_y2_;
	
	void serialize(CP_ATTR_NODE);   
};
//----------------------------------------------------------------------------------------------
class draw_line : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawLine;


    virtual void serialize(std::wostream & _Wostream);

    draw_line_attlist draw_line_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(draw_line);
//----------------------------------------------------------------------------------------------
class draw_path_attlist
{
public:
	_CP_OPT(std::wstring) svg_d_;
	_CP_OPT(std::wstring) svg_viewbox_;
	
	void serialize(CP_ATTR_NODE);   
};
//----------------------------------------------------------------------------------------------
class draw_path : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawPath;


    virtual void serialize(std::wostream & _Wostream);
    
	draw_path_attlist draw_path_attlist_;

	void reset_svg_path();
};

CP_REGISTER_OFFICE_ELEMENT2(draw_path);
//----------------------------------------------------------------------------------------------
class draw_polygon_attlist
{
public:
	_CP_OPT(std::wstring) draw_points_;
	_CP_OPT(std::wstring) svg_viewbox_;
	
	void serialize(CP_ATTR_NODE);   
};
//----------------------------------------------------------------------------------------------
class draw_polygon : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawPolygon;


    virtual void serialize(std::wostream & _Wostream);

    draw_polygon_attlist draw_polygon_attlist_;

	void reset_polygon_path();//частный случай svg - все точки соединены прямыми
};

CP_REGISTER_OFFICE_ELEMENT2(draw_polygon);
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
class draw_custom_shape : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawCustomShape;


    virtual void serialize(std::wostream & _Wostream);
    virtual void add_child_element( const office_element_ptr & child_element);

	office_element_ptr draw_enhanced_geometry_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_custom_shape);
//----------------------------------------------------------------------------------------------
//draw:enhanced-geometry
//----------------------------------------------------------------------------------------------
class draw_equation_attlist//убрать стринги ... сделать парсинг см стр 378 оазис !!!!!!!!!!!
{
public:
	_CP_OPT(std::wstring) draw_name_;
	_CP_OPT(std::wstring) draw_formula_;
	
	void serialize(CP_ATTR_NODE);   
};
//----------------------------------------------------------------------------------------------
class draw_equation : public office_element_impl<draw_equation>
{
public:

    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawEquation;

    
	
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    draw_equation_attlist draw_equation_attlist_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_equation);
//----------------------------------------------------------------------------------------------
struct draw_handle_geometry
{
	int min;
	int max;
};
//----------------------------------------------------------------------------------------------
class draw_handle_attlist//убрать стринги ... сделать парсинг см стр 378 оазис !!!!!!!!!!!
{
public:
	_CP_OPT(std::wstring) draw_handle_position_;

	_CP_OPT(std::wstring) draw_handle_range_y_maximum_;
	_CP_OPT(std::wstring) draw_handle_range_y_minimum_;

	_CP_OPT(std::wstring) draw_handle_range_x_maximum_;
	_CP_OPT(std::wstring) draw_handle_range_x_minimum_;
	
	_CP_OPT(std::wstring) draw_handle_radius_range_maximum_;
	_CP_OPT(std::wstring) draw_handle_radius_range_minimum_;
	
	_CP_OPT(std::wstring) draw_handle_polar_;
	
	void serialize(CP_ATTR_NODE);   
};
//----------------------------------------------------------------------------------------------
class draw_handle : public office_element_impl<draw_handle>
{
public:

    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawHandle;

	
		
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    draw_handle_attlist draw_handle_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(draw_handle);
//----------------------------------------------------------------------------------------------
class draw_enhanced_geometry_attlist
{
public:
	_CP_OPT(std::wstring)		draw_type_;
	_CP_OPT(std::wstring)		draw_modifiers_;
	_CP_OPT(std::wstring)		draw_enhanced_path_;
	_CP_OPT(std::wstring)		draw_text_areas_;
	_CP_OPT(std::wstring)		draw_glue_points_;
	
	_CP_OPT(std::wstring)		draw_sub_view_size_;

	_CP_OPT(odf_types::Bool)	draw_mirror_vertical_;
	_CP_OPT(odf_types::Bool)	draw_mirror_horizontal_;
	
	_CP_OPT(odf_types::Bool)	draw_text_path_;
	_CP_OPT(odf_types::Bool)	draw_text_path_same_letter_heights_;
	_CP_OPT(std::wstring)		draw_text_path_mode_;
	_CP_OPT(std::wstring)		draw_text_path_scale_;

	_CP_OPT(int)				draw_text_rotate_angle_;
	
	void serialize(CP_ATTR_NODE);   
};
//----------------------------------------------------------------------------------------------
class draw_enhanced_geometry : public office_element_impl<draw_enhanced_geometry>
{
public:

    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawEnhancedGeometry;

	
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    draw_enhanced_geometry_attlist			attlist_;

	_CP_OPT(std::wstring)					svg_viewbox_;

	typedef std::pair<std::wstring,std::wstring> pair_string_value;

	std::vector<draw_handle_geometry>	draw_handle_geometry_; //параметры в удобноваримом виде
	std::vector<pair_string_value>		draw_equation_array_;
	
	office_element_ptr_array			draw_handle_;
	office_element_ptr_array			draw_equation_;//некоторые заданные параметры отрисовки которые используются в draw_handle - автозамена общих частей
	
	static int parsing(_CP_OPT(std::wstring) val);//todoooo наоборот

	
};

CP_REGISTER_OFFICE_ELEMENT2(draw_enhanced_geometry);

//----------------------------------------------------------------------------------------------

class draw_caption : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawCaption;


	virtual void serialize(std::wostream & _Wostream);
};

CP_REGISTER_OFFICE_ELEMENT2(draw_caption);
//----------------------------------------------------------------------------------------------
class draw_connector_attlist
{
public:
	_CP_OPT(std::wstring) svg_d_;
	_CP_OPT(std::wstring) svg_viewbox_;
	_CP_OPT(std::wstring) draw_type_;
	
	void serialize(CP_ATTR_NODE);   
};
//----------------------------------------------------------------------------------------------
class draw_connector : public draw_line
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawConnector;


	virtual void serialize(std::wostream & _Wostream);

    draw_connector_attlist draw_connector_attlist_;

	void reset_svg_path();
	void reset_svg_attributes();
};

CP_REGISTER_OFFICE_ELEMENT2(draw_connector);
//----------------------------------------------------------------------------------------------
class draw_control : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawControl;


	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)	xml_id_;
	_CP_OPT(std::wstring)	caption_id_;
	_CP_OPT(std::wstring)	control_id_;

	office_element_ptr		draw_glue_point_;
//<svg:desc>
//<svg:title>
};
CP_REGISTER_OFFICE_ELEMENT2(draw_control);
//----------------------------------------------------------------------------------------------
class dr3d_scene : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDr3dScene;


	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

 	odf_types::common_dr3d_attlist dr3d_attlist_;

};
CP_REGISTER_OFFICE_ELEMENT2(dr3d_scene);
//------------------------------------------------------------------------------------------------------------

class dr3d_extrude : public draw_path
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDr3dExtrude;


	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(dr3d_extrude);
//------------------------------------------------------------------------------------------------------------

class dr3d_rotate : public draw_path
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDr3dRotate;


	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(dr3d_rotate);

//------------------------------------------------------------------------------------------------------------
class dr3d_light : public office_element_impl<dr3d_light>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeDr3dLight;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)		dr3d_diffuse_color_;
	_CP_OPT(std::wstring)		dr3d_direction_;
	_CP_OPT(odf_types::Bool)	dr3d_specular_;
	_CP_OPT(odf_types::Bool)	dr3d_enabled_;

};
CP_REGISTER_OFFICE_ELEMENT2(dr3d_light);

//------------------------------------------------------------------------------------------------------------
class dr3d_cube : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDr3dCube;


	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)	dr3d_max_edge_;
	_CP_OPT(std::wstring)	dr3d_min_edge_;
	_CP_OPT(std::wstring)	dr3d_transform_;

};
CP_REGISTER_OFFICE_ELEMENT2(dr3d_cube);

//------------------------------------------------------------------------------------------------------------
class dr3d_sphere : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDr3dSphere;


	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(odf_types::vector3D)	dr3d_size_; 
	_CP_OPT(odf_types::vector3D)	dr3d_center_;
	_CP_OPT(std::wstring)			dr3d_transform_;

};
CP_REGISTER_OFFICE_ELEMENT2(dr3d_sphere);
//----------------------------------------------------------------------------------
// draw:a
//----------------------------------------------------------------------------------
class draw_a : public office_element_impl<draw_a>
{  
public:
    static const wchar_t * ns;
    static const wchar_t * name;
   
	static const ElementType type = typeDrawA;

	

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);
  
	office_element_ptr_array content_;

	friend class odf_document;

	odf_types::common_xlink_attlist xlink_attlist_;

    _CP_OPT(std::wstring)					office_name_;
    _CP_OPT(odf_types::target_frame_name)	office_target_frame_name_;

    _CP_OPT(std::wstring)					text_style_name_;
    _CP_OPT(std::wstring)					text_visited_style_name_;

};
CP_REGISTER_OFFICE_ELEMENT2(draw_a);
}
}