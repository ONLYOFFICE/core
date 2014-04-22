#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements.h"
#include "office_elements_create.h"

#include "common_attlists.h"


namespace cpdoccore 
{

namespace odf {


class draw_shape : public office_element_impl<draw_shape>
{  
public:
    static const wchar_t * ns;
    static const wchar_t * name;
   
	static const ElementType type = typeDrawShape;
	static const xml::NodeType xml_type = xml::typeElement;

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);
    virtual void serialize_attlist(CP_ATTR_NODE);
	
	common_xlink_attlist		common_xlink_attlist_;
	common_presentation_attlist common_presentation_attlist_;
	union_common_draw_attlists  common_draw_attlists_;
	
	_CP_OPT(std::wstring)		draw_id_;//используется для анимашек
  
	office_element_ptr_array content_;

	CPDOCCORE_DEFINE_VISITABLE();

	int sub_type_;
	std::vector<odf::_property> additional_;


};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_rect_attlist
{
public:

   _CP_OPT(std::wstring) draw_filter_name_;
	void serialize(CP_ATTR_NODE);   


};

class draw_rect : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
   
	static const ElementType type = typeDrawRect;
	static const xml::NodeType xml_type = xml::typeElement;

    virtual void serialize(std::wostream & _Wostream);

    draw_rect_attlist draw_rect_attlist_;    

};

CP_REGISTER_OFFICE_ELEMENT2(draw_rect);

//////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	static const xml::NodeType xml_type = xml::typeElement;

    virtual void serialize(std::wostream & _Wostream);

	draw_ellipse_attlist draw_ellipse_attlist_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_ellipse);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_circle : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawCircle;
	static const xml::NodeType xml_type = xml::typeElement;

    virtual void serialize(std::wostream & _Wostream);
};

CP_REGISTER_OFFICE_ELEMENT2(draw_circle);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_line_attlist
{
public:
	_CP_OPT(length) svg_x1_;
    _CP_OPT(length) svg_y1_;

	_CP_OPT(length) svg_x2_;
    _CP_OPT(length) svg_y2_;
	
	void serialize(CP_ATTR_NODE);   
};
/////////////////////////////////////////////////////////////////////////
class draw_line : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawLine;
	static const xml::NodeType xml_type = xml::typeElement;

    virtual void serialize(std::wostream & _Wostream);

    draw_line_attlist draw_line_attlist_;

	void reset_svg_attributes();
};

CP_REGISTER_OFFICE_ELEMENT2(draw_line);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_path_attlist
{
public:
	_CP_OPT(std::wstring) svg_d_;
	_CP_OPT(std::wstring) svg_viewbox_;
	
	void serialize(CP_ATTR_NODE);   
};
/////////////////////////////////////////////////////////////////////////
class draw_path : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawPath;
	static const xml::NodeType xml_type = xml::typeElement;

    virtual void serialize(std::wostream & _Wostream);
    
	draw_path_attlist draw_path_attlist_;

	void reset_svg_path();
};

CP_REGISTER_OFFICE_ELEMENT2(draw_path);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_polygon_attlist
{
public:
	_CP_OPT(std::wstring) draw_points_;
	_CP_OPT(std::wstring) svg_viewbox_;
	
	void serialize(CP_ATTR_NODE);   
};
/////////////////////////////////////////////////////////////////////////
class draw_polygon : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawPolygon;
	static const xml::NodeType xml_type = xml::typeElement;

    virtual void serialize(std::wostream & _Wostream);

    draw_polygon_attlist draw_polygon_attlist_;

	void reset_polygon_path();//частный случай svg - все точки соединены прямыми
};

CP_REGISTER_OFFICE_ELEMENT2(draw_polygon);
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
class draw_custom_shape : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawCustomShape;
	static const xml::NodeType xml_type = xml::typeElement;

    virtual void serialize(std::wostream & _Wostream);
};

CP_REGISTER_OFFICE_ELEMENT2(draw_custom_shape);
//draw:enhanced-geometry
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////
class draw_equation_attlist//убрать стринги ... сделать парсинг см стр 378 оазис !!!!!!!!!!!
{
public:
	_CP_OPT(std::wstring) draw_name_;
	_CP_OPT(std::wstring) draw_formula_;
	
	void serialize(CP_ATTR_NODE);   
};
/////////////////////////////////////////////////////////////////
class draw_equation : public office_element_impl<draw_equation>
{
public:

    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawEquation;
	static const xml::NodeType xml_type = xml::typeElement;
    CPDOCCORE_DEFINE_VISITABLE();
	
	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    draw_equation_attlist draw_equation_attlist_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_equation);
////////////////
struct draw_handle_geometry
{
	int min;
	int max;
};
//////////////////////////////////////////////////////
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

class draw_handle : public office_element_impl<draw_handle>
{
public:

    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawHandle;
	static const xml::NodeType xml_type = xml::typeElement;
	CPDOCCORE_DEFINE_VISITABLE();
		
	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    draw_handle_attlist draw_handle_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(draw_handle);
/////////////////
class draw_enhanced_geometry_attlist
{
public:
	_CP_OPT(std::wstring) draw_type_;
	_CP_OPT(std::wstring) draw_modifiers_;
	_CP_OPT(std::wstring) draw_enhanced_path_;
	
	void serialize(CP_ATTR_NODE);   
};
/////////////////////////////////////////////////////////////////////////
class draw_enhanced_geometry : public office_element_impl<draw_enhanced_geometry>
{
public:

    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawCustomShape;
	static const xml::NodeType xml_type = xml::typeElement;
	
	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    draw_enhanced_geometry_attlist draw_enhanced_geometry_attlist_;

	_CP_OPT(std::wstring) svg_viewbox_;

	typedef std::pair<std::wstring,std::wstring> pair_string_value;

	std::vector<draw_handle_geometry> draw_handle_geometry_; //параметры в удобноваримом виде
	std::vector<pair_string_value>draw_equation_array_;
	
	office_element_ptr_array draw_handle_;
	office_element_ptr_array draw_equation_;//некоторые заданные параметры отрисовки которые используются в draw_handle - автозамена общих частей
	
	static int parsing(_CP_OPT(std::wstring) val);//todoooo наоборот

	CPDOCCORE_DEFINE_VISITABLE();
};

CP_REGISTER_OFFICE_ELEMENT2(draw_enhanced_geometry);

///////////////////////////////////////////////////////////////////////////////////////////////////////////

class draw_caption : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawCaption;
	static const xml::NodeType xml_type = xml::typeElement;

    virtual void serialize(std::wostream & _Wostream);
};

CP_REGISTER_OFFICE_ELEMENT2(draw_caption);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_connector_attlist
{
public:
	_CP_OPT(std::wstring) svg_d_;
	_CP_OPT(std::wstring) svg_viewbox_;
	_CP_OPT(std::wstring) draw_type_;
	
	void serialize(CP_ATTR_NODE);   
};
/////////////////////////////////////////////////////////////////////////
class draw_connector : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawConnector;
	static const xml::NodeType xml_type = xml::typeElement;

	virtual void serialize(std::wostream & _Wostream);

    draw_connector_attlist draw_connector_attlist_;
    draw_line_attlist draw_line_attlist_;

	void reset_svg_path();
	void reset_svg_attributes();
};

CP_REGISTER_OFFICE_ELEMENT2(draw_connector);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}