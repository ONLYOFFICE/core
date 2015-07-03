#pragma once

#include <iosfwd>

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/CPOptional.h>

#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements.h"
#include "office_elements_create.h"

#include "datatypes/common_attlists.h"
#include "datatypes/lengthorpercent.h"
#include "datatypes/stylehorizontalpos.h"
#include "datatypes/stylehorizontalrel.h"
#include "datatypes/styleverticalrel.h"
#include "datatypes/styleverticalpos.h"
#include "datatypes/anchortype.h"
#include "datatypes/stylewrap.h"
#include "datatypes/hyphenationladdercount.h"
#include "datatypes/stylewrapcontourmode.h"
#include "datatypes/runthrough.h"
#include "datatypes/linestyle.h"
#include "datatypes/markerstyle.h"
#include "datatypes/verticalalign.h"
#include "datatypes/gradientstyle.h"


namespace cpdoccore { 
namespace odf_reader {

class graphic_format_properties
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

    void apply_from(const graphic_format_properties & Other);
	void apply_to(std::vector<_property> & properties);

public:
    
	_CP_OPT(odf_types::length_or_percent) fo_min_width_;
    _CP_OPT(odf_types::length_or_percent) fo_min_height_;
    
    _CP_OPT(odf_types::length_or_percent) fo_max_width_;
    _CP_OPT(odf_types::length_or_percent) fo_max_height_;

///////////////////////////////
	_CP_OPT(odf_types::color)				svg_stroke_color_;
	_CP_OPT(odf_types::length_or_percent)	svg_stroke_opacity_;
	_CP_OPT(odf_types::line_style)			draw_stroke_;
	_CP_OPT(std::wstring)					draw_stroke_dash_;
	_CP_OPT(odf_types::length_or_percent)	svg_stroke_width_;

	_CP_OPT(odf_types::marker_style)		draw_marker_end_;
	_CP_OPT(odf_types::marker_style)		draw_marker_start_;

	_CP_OPT(odf_types::text_align)			draw_textarea_horizontal_align_;
	_CP_OPT(odf_types::vertical_align)		draw_textarea_vertical_align_;
	
	_CP_OPT(bool)							draw_auto_grow_height_;
	_CP_OPT(bool)							draw_auto_grow_width_;

	_CP_OPT(bool)							draw_fit_to_size_;
	_CP_OPT(bool)							draw_fit_to_contour_;
	_CP_OPT(std::wstring)					draw_wrap_influence_on_position_;

	odf_types::common_draw_rel_size_attlist			common_draw_rel_size_attlist_;
	odf_types::common_draw_fill_attlist				common_draw_fill_attlist_;  
	odf_types::common_horizontal_margin_attlist		common_horizontal_margin_attlist_;
	odf_types::common_vertical_margin_attlist		common_vertical_margin_attlist_;
    odf_types::common_margin_attlist				common_margin_attlist_;

    odf_types::common_horizontal_pos_attlist		common_horizontal_pos_attlist_;
    odf_types::common_horizontal_rel_attlist		common_horizontal_rel_attlist_;   
    odf_types::common_vertical_pos_attlist			common_vertical_pos_attlist_;
    odf_types::common_vertical_rel_attlist			common_vertical_rel_attlist_;
    odf_types::common_text_anchor_attlist			common_text_anchor_attlist_;
	odf_types::common_text_animation_attlist		common_text_animation_attlist_;
	odf_types::common_border_attlist				common_border_attlist_;
    odf_types::common_border_line_width_attlist		common_border_line_width_attlist_;
    odf_types::common_padding_attlist				common_padding_attlist_;    
    odf_types::common_shadow_attlist				common_shadow_attlist_;    
    odf_types::common_background_color_attlist		common_background_color_attlist_;    
 
    _CP_OPT(bool)							style_print_content_;
    _CP_OPT(std::wstring)					style_protect_;
	_CP_OPT(bool)							style_editable_;
    _CP_OPT(odf_types::style_wrap)			style_wrap_;
    _CP_OPT(unsigned int)					style_wrap_dynamic_treshold_;
    _CP_OPT(odf_types::integer_or_nolimit)	style_number_wrapped_paragraphs_;
    _CP_OPT(bool)							style_wrap_contour_;
    _CP_OPT(odf_types::wrap_contour_mode)	style_wrap_contour_mode_;
    _CP_OPT(odf_types::run_through)			style_run_through_; 
    _CP_OPT(bool)							style_flow_with_text_;
    _CP_OPT(std::wstring)					style_overflow_behavior_;
    _CP_OPT(std::wstring)					style_mirror_;

    _CP_OPT(std::wstring)	fo_clip_;

    
    office_element_ptr		style_background_image_;
        
};

/// \class  style_graphic_properties
///         style:graphic-properties
class style_graphic_properties : public office_element_impl<style_graphic_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleGraphicPropertis;

    CPDOCCORE_DEFINE_VISITABLE();

   const graphic_format_properties & content() const { return graphic_format_properties_; }


private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
 
private:
    graphic_format_properties graphic_format_properties_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_graphic_properties);


}
}

