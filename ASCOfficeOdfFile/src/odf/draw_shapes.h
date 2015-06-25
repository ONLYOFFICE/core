#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"
#include "datatypes/common_attlists.h"
#include "../docx/xlsxconversioncontext.h"

#include "svg_parser.h"

namespace cpdoccore 
{

namespace odf_reader {

class draw_shape_attlist
{
public:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:

};

class draw_shape : public office_element_impl<draw_shape>
{  
public:
    static const wchar_t * ns;
    static const wchar_t * name;
   
	static const ElementType type = typeDrawShape;
	static const xml::NodeType xml_type = xml::typeElement;

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

	virtual void docx_convert(oox::docx_conversion_context & Context) ;
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context) {}
	virtual void pptx_convert(oox::pptx_conversion_context & Context) {}
	
	void common_xlsx_convert(oox::xlsx_conversion_context & Context); 
	void common_docx_convert(oox::docx_conversion_context & Context) ;
	void common_pptx_convert(oox::pptx_conversion_context & Context) ;

	odf_types::common_xlink_attlist			common_xlink_attlist_;
	odf_types::common_presentation_attlist	common_presentation_attlist_;
	odf_types::union_common_draw_attlists	common_draw_attlists_;
	draw_shape_attlist						draw_shape_attlist_;
	_CP_OPT(std::wstring)					draw_id_;//используется для анимашек
  
	office_element_ptr_array content_;

	CPDOCCORE_DEFINE_VISITABLE();
	friend class odf_document;
//----------------------------------------------------------------------------------------------
	bool								word_art_;
	int									sub_type_;
	std::vector<odf_reader::_property>	additional_;


};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_rect_attlist
{
public:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(std::wstring) draw_filter_name_;

};

class draw_rect : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
   
	static const ElementType type = typeDrawRect;
	static const xml::NodeType xml_type = xml::typeElement;

    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

private:
    draw_rect_attlist draw_rect_attlist_;    

};

CP_REGISTER_OFFICE_ELEMENT2(draw_rect);

//////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_ellipse_attlist
{
public:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(std::wstring) draw_filter_name_;

};

class draw_ellipse : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawEllipse;
	static const xml::NodeType xml_type = xml::typeElement;

    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

    draw_ellipse_attlist draw_ellipse_attlist_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_ellipse);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_circle_attlist
{
public:
 	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );

};
/////////////////////////////////////////////////////////////////////////
class draw_circle : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawCircle;
	static const xml::NodeType xml_type = xml::typeElement;

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void pptx_convert(oox::pptx_conversion_context & Context);

    draw_circle_attlist draw_circle_attlist_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_circle);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_line_attlist
{
public:
 	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
 
	_CP_OPT(odf_types::length) svg_x1_;
    _CP_OPT(odf_types::length) svg_y1_;

	_CP_OPT(odf_types::length) svg_x2_;
    _CP_OPT(odf_types::length) svg_y2_;

};
/////////////////////////////////////////////////////////////////////////
class draw_line : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawLine;
	static const xml::NodeType xml_type = xml::typeElement;

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

    draw_line_attlist draw_line_attlist_;
private:
	void reset_svg_attributes();
};

CP_REGISTER_OFFICE_ELEMENT2(draw_line);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_path_attlist
{
public:
 	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	_CP_OPT(std::wstring) svg_d_;
	_CP_OPT(std::wstring) svg_viewbox_;

};
/////////////////////////////////////////////////////////////////////////
class draw_path : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawPath;
	static const xml::NodeType xml_type = xml::typeElement;

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

    draw_path_attlist draw_path_attlist_;

	void reset_svg_path();
};

CP_REGISTER_OFFICE_ELEMENT2(draw_path);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_polygon_attlist
{
public:
 	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	_CP_OPT(std::wstring) draw_points_;
	_CP_OPT(std::wstring) svg_viewbox_;

};
/////////////////////////////////////////////////////////////////////////
class draw_polygon : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawPolygon;
	static const xml::NodeType xml_type = xml::typeElement;

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

    draw_polygon_attlist draw_polygon_attlist_;

	void reset_polygon_path();//частный случай svg - все точки соединены прямыми
};

CP_REGISTER_OFFICE_ELEMENT2(draw_polygon);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_polyline_attlist
{
public:
 	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	_CP_OPT(std::wstring) draw_points_;
	_CP_OPT(std::wstring) svg_viewbox_;

};
/////////////////////////////////////////////////////////////////////////
class draw_polyline : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawPolyline;
	static const xml::NodeType xml_type = xml::typeElement;

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

    draw_polyline_attlist draw_polyline_attlist_;

	void reset_polyline_path();//частный случай svg - все точки соединены прямыми
};

CP_REGISTER_OFFICE_ELEMENT2(draw_polyline);
/////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_custom_shape_attlist
{
public:
 	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );

};
/////////////////////////////////////////////////////////////////////////
class draw_custom_shape : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawCustomShape;
	static const xml::NodeType xml_type = xml::typeElement;

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

    draw_custom_shape_attlist draw_custom_shape_attlist_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_custom_shape);
//draw:enhanced-geometry
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////
class draw_equation_attlist//убрать стринги ... сделать парсинг см стр 378 оазис !!!!!!!!!!!
{
public:
 	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	
	_CP_OPT(std::wstring) draw_name_;
	_CP_OPT(std::wstring) draw_formula_;
};
/////////////////////////////////////////////////////////////////
class draw_equation : public office_element_impl<draw_equation>
{
public:

    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawEquation;
	static const xml::NodeType xml_type = xml::typeElement;
	
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name){}

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
 	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	
	_CP_OPT(std::wstring) draw_handle_position_;

	_CP_OPT(std::wstring) draw_handle_range_y_maximum_;
	_CP_OPT(std::wstring) draw_handle_range_y_minimum_;

	_CP_OPT(std::wstring) draw_handle_range_x_maximum_;
	_CP_OPT(std::wstring) draw_handle_range_x_minimum_;
	
	_CP_OPT(std::wstring) draw_handle_radius_range_maximum_;
	_CP_OPT(std::wstring) draw_handle_radius_range_minimum_;
	
	_CP_OPT(std::wstring) draw_handle_polar_;
};

class draw_handle : public office_element_impl<draw_handle>
{
public:

    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawHandle;
	static const xml::NodeType xml_type = xml::typeElement;
	
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name){}

    draw_handle_attlist draw_handle_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(draw_handle);
/////////////////
class draw_enhanced_geometry_attlist
{
public:
 	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	_CP_OPT(std::wstring)	draw_type_;
	_CP_OPT(std::wstring)	draw_modifiers_;
	_CP_OPT(bool)			draw_text_path_;
};
/////////////////////////////////////////////////////////////////////////
class draw_enhanced_geometry : public office_element_impl<draw_enhanced_geometry>
{
public:

    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawCustomShape;
	static const xml::NodeType xml_type = xml::typeElement;
	
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
	
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

	void find_draw_type_oox();

    draw_enhanced_geometry_attlist draw_enhanced_geometry_attlist_;

	_CP_OPT(int)			sub_type_;
	_CP_OPT(int)			draw_type_oox_index_;
	_CP_OPT(std::wstring)	svg_viewbox_;
	bool					word_art_;

	typedef std::pair<std::wstring,std::wstring> pair_string_value;

	std::vector<draw_handle_geometry> draw_handle_geometry_; //параметры в удобноваримом виде
	std::vector<pair_string_value>draw_equation_array_;
	
	office_element_ptr_array draw_handle_;
	office_element_ptr_array draw_equation_;//некоторые заданные параметры отрисовки которые используются в draw_handle - автозамена общих частей
	
	static int parsing(_CP_OPT(std::wstring) val);

	CPDOCCORE_DEFINE_VISITABLE();
	friend class odf_document;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_enhanced_geometry);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// draw-caption-attlist
class draw_caption_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:

};

class draw_caption : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawCaption;
	static const xml::NodeType xml_type = xml::typeElement;

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

	draw_caption_attlist draw_caption_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(draw_caption);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_connector_attlist
{
public:
 	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	_CP_OPT(std::wstring) svg_d_;
	_CP_OPT(std::wstring) svg_viewbox_;
	_CP_OPT(std::wstring) draw_type_;

};
/////////////////////////////////////////////////////////////////////////
class draw_connector : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawConnector;
	static const xml::NodeType xml_type = xml::typeElement;

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

    draw_connector_attlist draw_connector_attlist_;
    draw_line_attlist draw_line_attlist_;

	void reset_svg_path();
	void reset_svg_attributes();
};

CP_REGISTER_OFFICE_ELEMENT2(draw_connector);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}