#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"
#include "common_attlists.h"
#include "../docx/xlsxconversioncontext.h"

namespace cpdoccore { 
namespace odf {

/// draw-image-attlist
class draw_image_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(std::wstring) draw_filter_name_;

};


class draw_image : public office_element_impl<draw_image>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeDrawImage;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

private:
    draw_image_attlist draw_image_attlist_;
    common_xlink_attlist common_xlink_attlist_;
    office_element_ptr office_binary_data_;
    office_element_ptr_array content_;

    friend class odf_document;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_image);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
 //draw-chart-attlist
class draw_chart_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    //_CP_OPT(std::wstring) draw_filter_name_;

};

 //объект рисования не нужен .. нужно только место для фрэйма - сам чарт в другом месте
class draw_chart : public office_element_impl<draw_chart>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeDrawChart;
    CPDOCCORE_DEFINE_VISITABLE();

    //virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

private:
    draw_chart_attlist draw_chart_attlist_;
    common_xlink_attlist common_xlink_attlist_;
   
	//office_element_ptr title_;
    office_element_ptr_array content_;

    friend class odf_document;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_chart);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //draw-frame-attlist
class draw_frame_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(std::wstring) draw_copy_of_;

};


class draw_frame : public office_element_impl<draw_frame>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeDrawFrame;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
	int idx_in_owner ;
	common_presentation_attlist common_presentation_attlist_;
	union_common_draw_attlists  common_draw_attlists_;

  
	presentation_shape_attlist presentation_shape_attlist_;
    draw_frame_attlist draw_frame_attlist_;

    // draw-text-box, draw-image, draw-object, draw-object-ole, draw-applet, draw-floating-frame, draw-plugin
    office_element_ptr_array content_;

    office_element_ptr office_event_listeners_; // в content перенести нельзя - иначе событи будет добавляться не к этому объекту а следующему
    office_element_ptr draw_glue_point_;
    office_element_ptr draw_image_map_;
    //office_element_ptr draw_chart_map_;

    office_element_ptr draw_contour_; // draw-contour-polygon or draw-contour-path

    friend class odf_document;
    friend class draw_image;
	friend class draw_chart;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_frame);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //draw-frame-attlist
class draw_g_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:

};


class draw_g : public office_element_impl<draw_g>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeDrawG;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

public:
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
	union_common_draw_attlists  common_draw_attlists_;  
    draw_g_attlist draw_g_attlist_;

    office_element_ptr_array content_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_g);
// draw-text-box-attlist
class draw_text_box_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(std::wstring) draw_chain_next_name_;
    _CP_OPT(length) draw_corner_radius_;
    _CP_OPT(length_or_percent) fo_min_width_;
    _CP_OPT(length_or_percent) fo_min_height_;
    _CP_OPT(length_or_percent) fo_max_width_;
    _CP_OPT(length_or_percent) fo_max_height_;        

};


class draw_text_box : public office_element_impl<draw_text_box>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeDrawTextBox;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

public:
    draw_text_box_attlist draw_text_box_attlist_;
    office_element_ptr_array content_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_text_box);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// draw-object-attlist
class draw_object_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes )
    {
        // TODO
    }

public:
    

};


/// \class  draw_object
///         draw:object
///         draw-object
class draw_object : public office_element_impl<draw_object>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeDrawObject;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    draw_object_attlist draw_object_attlist_;
    common_xlink_attlist common_xlink_attlist_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_object);


}
}
