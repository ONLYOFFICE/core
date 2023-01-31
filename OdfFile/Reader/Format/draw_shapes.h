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

#include <iosfwd>

#include "office_elements.h"
#include "office_elements_create.h"
#include "../../DataTypes/common_attlists.h"
#include "../../Reader/Converter/xlsxconversioncontext.h"

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

	draw_shape() : bad_shape_(false), word_art_(false), idx_in_owner(-1), lined_shape_(false), connector_(false){}

	CPDOCCORE_DEFINE_VISITABLE();
	friend class odf_document;
//----------------------------------------------------------------------------------------------
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

	virtual void docx_convert(oox::docx_conversion_context & Context) ;
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context) {}
	virtual void pptx_convert(oox::pptx_conversion_context & Context) {}
	
	void common_xlsx_convert(oox::xlsx_conversion_context & Context); 
	void common_docx_convert(oox::docx_conversion_context & Context) ;
	void common_pptx_convert(oox::pptx_conversion_context & Context) ;
	
	odf_types::union_common_draw_attlists	common_draw_attlists_;

	draw_shape_attlist						draw_shape_attlist_;
	odf_types::common_xlink_attlist			xlink_attlist_;
	_CP_OPT(std::wstring)					draw_id_;				//используется для анимашек
  
	office_element_ptr_array			content_;
	office_element_ptr					enhanced_geometry_;

	bool								bad_shape_;
	bool								word_art_;
	int									sub_type_;
	std::vector<odf_reader::_property>	additional_;
	int									idx_in_owner;
	bool								lined_shape_;
	bool								connector_;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_rect_attlist
{
public:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(std::wstring)		draw_filter_name_;
	_CP_OPT(odf_types::length)	draw_corner_radius_;

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
//draw:polygon
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
//-----------------------------------------------------------------------------------------------------
//draw:contour-polygon
class draw_contour_polygon : public draw_polygon
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawContourPolygon;
	static const xml::NodeType xml_type = xml::typeElement;

};
CP_REGISTER_OFFICE_ELEMENT2(draw_contour_polygon);
//-----------------------------------------------------------------------------------------------------
//draw:contour-path
class draw_contour_path : public draw_path
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawContourPath;
	static const xml::NodeType xml_type = xml::typeElement;

};
CP_REGISTER_OFFICE_ELEMENT2(draw_contour_path);
//-----------------------------------------------------------------------------------------------------
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
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

    draw_equation_attlist attlist_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_equation);
////////////////
//struct draw_handle_geometry
//{
//	int min;
//	int max;
//};
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
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

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
	_CP_OPT(std::wstring)	draw_enhanced_path_;
	_CP_OPT(std::wstring)	drawooo_enhanced_path_;
	_CP_OPT(std::wstring)	drawooo_sub_view_size_;
	_CP_OPT(bool)			draw_mirror_horizontal_;
	_CP_OPT(bool)			draw_mirror_vertical_;
};
/////////////////////////////////////////////////////////////////////////
class draw_enhanced_geometry : public office_element_impl<draw_enhanced_geometry>
{
public:

    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDrawCustomShape;
	static const xml::NodeType xml_type = xml::typeElement;
	
	CPDOCCORE_DEFINE_VISITABLE();

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
	
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

    draw_enhanced_geometry_attlist		attlist_;

	_CP_OPT(std::wstring)				svg_viewbox_;
		
	office_element_ptr_array			draw_handle_;
	office_element_ptr_array			draw_equation_;

//-------------------------------------------------
	static int parsing(_CP_OPT(std::wstring) val);

	void find_draw_type_oox();
	
	std::map<std::wstring, std::wstring> map_draw_equation_;
	//std::vector<draw_handle_geometry>	draw_handle_geometry_; 
	
	_CP_OPT(int)			sub_type_;
	_CP_OPT(int)			draw_type_oox_index_;

	bool					word_art_;
	bool					bOoxType_;
	std::wstring			odf_path_;
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

    draw_connector_attlist	draw_connector_attlist_;
    draw_line_attlist		draw_line_attlist_;

	void reset_svg_path();
	void reset_svg_attributes();
};

CP_REGISTER_OFFICE_ELEMENT2(draw_connector);
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class dr3d_scene : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDr3dScene;
	static const xml::NodeType xml_type = xml::typeElement;

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

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
	static const xml::NodeType xml_type = xml::typeElement;

	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	
	//virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	//virtual void docx_convert(oox::docx_conversion_context & Context);
	//virtual void pptx_convert(oox::pptx_conversion_context & Context);
};
CP_REGISTER_OFFICE_ELEMENT2(dr3d_extrude);
//------------------------------------------------------------------------------------------------------------

class dr3d_rotate : public draw_path
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDr3dRotate;
	static const xml::NodeType xml_type = xml::typeElement;

	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	
	//virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	//virtual void docx_convert(oox::docx_conversion_context & Context);
    //virtual void pptx_convert(oox::pptx_conversion_context & Context);
};
CP_REGISTER_OFFICE_ELEMENT2(dr3d_rotate);
//------------------------------------------------------------------------------------------------------------
class dr3d_light : public office_element_impl<dr3d_light>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
	
	static const ElementType type = typeDr3dLight;
	static const xml::NodeType xml_type = xml::typeElement;

	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

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
	static const xml::NodeType xml_type = xml::typeElement;

	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

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
	static const xml::NodeType xml_type = xml::typeElement;

	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

	_CP_OPT(odf_types::vector3D)	dr3d_size_;
	_CP_OPT(odf_types::vector3D)	dr3d_center_; 
	_CP_OPT(std::wstring)			dr3d_transform_;

};
CP_REGISTER_OFFICE_ELEMENT2(dr3d_sphere);
//----------------------------------------------------------------------------------------------
class draw_control : public draw_shape
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeDrawControl;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context){}

	_CP_OPT(std::wstring)	xml_id_;
	_CP_OPT(std::wstring)	caption_id_;
	_CP_OPT(std::wstring)	control_id_;

	office_element_ptr		draw_glue_point_;
//<svg:desc>
//<svg:title>
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
};
CP_REGISTER_OFFICE_ELEMENT2(draw_control);
}
}