#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"
#include "common_attlists.h"

#include "draw_base.h"

namespace cpdoccore { 
namespace odf {

/// draw-image-attlist
class draw_image_attlist
{
public:
    _CP_OPT(std::wstring) draw_filter_name_;
	
	void serialize(CP_ATTR_NODE);   
};
 //draw-frame-attlist
class draw_frame_attlist
{
public:
    _CP_OPT(std::wstring) draw_copy_of_;
	
	void serialize(CP_ATTR_NODE);   
};
 //draw-chart-attlist
class draw_chart_attlist
{
public:

    //_CP_OPT(std::wstring) draw_filter_name_;
	void serialize(CP_ATTR_NODE);   

};
/////////////////////////////////////////////////////////////////////////////////////////
class draw_image : public office_element_impl<draw_image>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeDrawImage;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	draw_image_attlist draw_image_attlist_;
    common_xlink_attlist common_xlink_attlist_;
    
	office_element_ptr office_binary_data_;
    office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(draw_image);
//////////////////////////////////////////////////////////////////////////////////////////////////////////

 //объект рисования не нужен .. нужно только место для фрэйма - сам чарт в другом месте
class draw_chart : public office_element_impl<draw_chart>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeDrawChart;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	draw_chart_attlist draw_chart_attlist_;
    common_xlink_attlist common_xlink_attlist_;
   
	//office_element_ptr title_;
    office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(draw_chart);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

class draw_frame : public draw_base
{    //для  draw-text-box, draw-image, draw-object, draw-object-ole, draw-applet, draw-floating-frame, draw-plugin
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeDrawFrame;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	int idx_in_owner ;

  	presentation_shape_attlist presentation_shape_attlist_;
    draw_frame_attlist draw_frame_attlist_;

    office_element_ptr office_event_listeners_; // в content перенести нельзя - иначе событи будет добавляться не к этому объекту а следующему
    office_element_ptr draw_glue_point_;
    office_element_ptr draw_image_map_;
    //office_element_ptr draw_chart_map_;

    office_element_ptr draw_contour_; // draw-contour-polygon or draw-contour-path
};

CP_REGISTER_OFFICE_ELEMENT2(draw_frame);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_g : public office_element_impl<draw_g>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeDrawG;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	union_common_draw_attlists  common_draw_attlists_;  

    office_element_ptr_array content_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_g);
// draw-text-box-attlist
class draw_text_box_attlist
{
public:
    _CP_OPT(std::wstring) draw_chain_next_name_;
    _CP_OPT(length) draw_corner_radius_;
    _CP_OPT(length_or_percent) fo_min_width_;
    _CP_OPT(length_or_percent) fo_min_height_;
    _CP_OPT(length_or_percent) fo_max_width_;
    _CP_OPT(length_or_percent) fo_max_height_;        

	void serialize(CP_ATTR_NODE);   
};


class draw_text_box : public office_element_impl<draw_text_box>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeDrawTextBox;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	draw_text_box_attlist draw_text_box_attlist_;
    office_element_ptr_array content_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_text_box);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// draw:object
class draw_object : public office_element_impl<draw_object>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeDrawObject;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    common_xlink_attlist common_xlink_attlist_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_object);

// draw:object-ole
class draw_object_ole : public office_element_impl<draw_object>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeDrawObjectOle;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	common_xlink_attlist	common_xlink_attlist_;
	_CP_OPT(std::wstring)	draw_class_id_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_object_ole);

}
}
