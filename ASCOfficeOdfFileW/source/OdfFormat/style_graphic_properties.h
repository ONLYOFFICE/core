#pragma once

#include <iosfwd>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements_create.h"

#include "common_attlists.h"
#include "lengthorpercent.h"
#include "stylehorizontalpos.h"
#include "stylehorizontalrel.h"
#include "styleverticalrel.h"
#include "styleverticalpos.h"
#include "anchortype.h"
#include "stylewrap.h"
#include "wrapoption.h"
#include "hyphenationladdercount.h"
#include "stylewrapcontourmode.h"
#include "runthrough.h"
#include "linestyle.h"
#include "markerstyle.h"
#include "verticalalign.h"
#include "gradientstyle.h"


namespace cpdoccore { 
namespace odf_writer {

class graphic_format_properties
{
public:
	graphic_format_properties(); //for defaults set
    void apply_from(const graphic_format_properties & Other);

	void serialize(std::wostream & strm, const wchar_t * ns, const wchar_t * name );

public:    
	_CP_OPT(odf_types::length_or_percent)			fo_min_width_;
    _CP_OPT(odf_types::length_or_percent)			fo_min_height_;    
    _CP_OPT(odf_types::length_or_percent)			fo_max_width_;
    _CP_OPT(odf_types::length_or_percent)			fo_max_height_;
	_CP_OPT(odf_types::wrap_option)					fo_wrap_option_;

	_CP_OPT(odf_types::color)						svg_stroke_color_;
	_CP_OPT(odf_types::length_or_percent)			svg_stroke_opacity_;
	_CP_OPT(odf_types::line_style)					draw_stroke_;
	_CP_OPT(std::wstring)							draw_stroke_dash_;
	_CP_OPT(odf_types::length_or_percent)			svg_stroke_width_;

	_CP_OPT(odf_types::marker_style)				draw_marker_end_;
	_CP_OPT(odf_types::marker_style)				draw_marker_start_;

	_CP_OPT(odf_types::length)						draw_marker_start_width_;
	_CP_OPT(odf_types::length)						draw_marker_end_width_;

	_CP_OPT(odf_types::text_align)					draw_textarea_horizontal_align_;
	_CP_OPT(odf_types::vertical_align)				draw_textarea_vertical_align_;

	_CP_OPT(odf_types::Bool)						draw_auto_grow_height_;
	_CP_OPT(odf_types::Bool)						draw_auto_grow_width_;
    
	_CP_OPT(odf_types::Bool)						draw_fit_to_size_;
	_CP_OPT(odf_types::Bool)						draw_fit_to_contour_;
    _CP_OPT(std::wstring)							draw_wrap_influence_on_position_;

    //odf_types::common_shadow_attlist				common_shadow_attlist_;
	_CP_OPT(odf_types::shadow_type1)				draw_shadow_;
	_CP_OPT(odf_types::percent)						draw_shadow_opacity_;
	_CP_OPT(odf_types::color)						draw_shadow_color_;
	_CP_OPT(odf_types::length)						draw_shadow_offset_y_;
	_CP_OPT(odf_types::length)						draw_shadow_offset_x_;
	
	odf_types::common_draw_fill_attlist				common_draw_fill_attlist_;
	odf_types::common_draw_rel_size_attlist			common_draw_rel_size_attlist_;

    odf_types::common_horizontal_pos_attlist		common_horizontal_pos_attlist_;
	odf_types::common_horizontal_rel_attlist		common_horizontal_rel_attlist_;    
	odf_types::common_horizontal_margin_attlist		common_horizontal_margin_attlist_;
		
    odf_types::common_vertical_pos_attlist			common_vertical_pos_attlist_;
    odf_types::common_vertical_rel_attlist			common_vertical_rel_attlist_;
    odf_types::common_vertical_margin_attlist		common_vertical_margin_attlist_;

    odf_types::common_text_anchor_attlist			common_text_anchor_attlist_;
    odf_types::common_margin_attlist				common_margin_attlist_;

	odf_types::common_text_animation_attlist		common_text_animation_attlist_;

	odf_types::common_border_attlist				common_border_attlist_;
    odf_types::common_border_line_width_attlist	common_border_line_width_attlist_;

	odf_types::common_shadow_attlist				common_shadow_attlist_;

    odf_types::common_padding_attlist				common_padding_attlist_;
    
    odf_types::common_background_color_attlist		common_background_color_attlist_;    
 
	_CP_OPT(odf_types::style_wrap)			style_wrap_;
    _CP_OPT(odf_types::Bool)				style_print_content_;
    _CP_OPT(std::wstring)					style_protect_;
	_CP_OPT(odf_types::Bool)				style_editable_;
    _CP_OPT(unsigned int)					style_wrap_dynamic_treshold_;
    _CP_OPT(odf_types::integer_or_nolimit)	style_number_wrapped_paragraphs_;
    _CP_OPT(odf_types::Bool)				style_wrap_contour_;
    _CP_OPT(odf_types::wrap_contour_mode)	style_wrap_contour_mode_;
    _CP_OPT(odf_types::run_through)			style_run_through_; 
    _CP_OPT(odf_types::Bool)				style_flow_with_text_;
    _CP_OPT(std::wstring)					style_overflow_behavior_;
	_CP_OPT(std::wstring)					style_mirror_;

    _CP_OPT(std::wstring)					fo_clip_;
////////////////////////////////////////////////////////////////////////    
    office_element_ptr						style_background_image_;        
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

    CPDOCCORE_DEFINE_VISITABLE()

	graphic_format_properties & content(){ return graphic_format_properties_; }

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child){}

	virtual void serialize(std::wostream & strm);
 
private:
    graphic_format_properties graphic_format_properties_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_graphic_properties);


}
}

