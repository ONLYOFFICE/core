#pragma once

#include "common_attlists.h"

#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements.h"
#include "office_elements_create.h"

#include "presentationclass.h"
#include "drawfill.h"

namespace cpdoccore { 
namespace odf {


// presentation:placeholder 
class presentation_placeholder : public office_element_impl<presentation_placeholder>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStylePresentationPlaceholder;

    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);
 
public:
	_CP_OPT(length) svg_x_;
    _CP_OPT(length) svg_y_;
    _CP_OPT(length) svg_width_;
    _CP_OPT(length) svg_height_;

	_CP_OPT(presentation_class) presentation_object_;

};

CP_REGISTER_OFFICE_ELEMENT2(presentation_placeholder);

//////////////////////////////////////////////
class drawing_page_properties
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const drawing_page_properties & Other);

////////////

	_CP_OPT(draw_fill)		draw_fill_;
	_CP_OPT(std::wstring)	draw_fill_image_name_;
	_CP_OPT(color)			draw_fill_color_;

	//draw:fill-hatch-name
	//draw:fill-gradient-name
	
	//_CP_OPT(std::wstring) presentation_transition_type_;
	//presentation:transition-style
	//presentation:transition-speed
	//presentation:display-footer
	//presentation:display-page-number
	//presentation:display-date-time
	//presentation:display-header

	//presentation:background-objects-visible
	//presentation:background-visible
	//style:repeat
	//presentation:page-duration
	//presentation:visibility.
	//presentation:sound.
	//draw:background-size

};
//style:drawing-page-properties
class style_drawing_page_properties : public office_element_impl<style_drawing_page_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleDrawingPageProperties;

    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    //virtual void pptx_convert(oox::pptx_conversion_context & Context);

	const drawing_page_properties & content() const { return drawing_page_properties_; }

 
public:
	drawing_page_properties drawing_page_properties_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_drawing_page_properties);

}
}
