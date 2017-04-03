/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "bool.h"
#include "lengthorpercent.h"
#include "fobreak.h"
#include "backgroundcolor.h"
#include "shadowtype.h"
#include "keeptogether.h"
#include "writingmode.h"
#include "borderwidths.h"
#include "textalign.h"
#include "styleverticalrel.h"
#include "styleverticalpos.h"
#include "stylehorizontalpos.h"
#include "stylehorizontalrel.h"
#include "percentorscale.h"
#include "anchortype.h"
#include "style_ref.h"
#include "linewidth.h"
#include "presentationclass.h"
#include "xlink.h"
#include "drawfill.h"
#include "clockvalue.h"
#include "stylerepeat.h"
#include "officevaluetype.h"
#include "fillimagerefpoint.h"
#include "borderstyle.h"
#include "mathvariant.h"

#define _CP_APPLY_PROP(A, B) \
    if (B) \
        A = B;

#define _CP_APPLY_PROP2(A) \
    if (Other.A) \
        A = Other.A;

namespace cpdoccore { 
namespace odf_types {

void apply_line_width(_CP_OPT(line_width) & A, const _CP_OPT(line_width) & B);
void apply_length_or_percent(_CP_OPT(length_or_percent) & Value, const _CP_OPT(length_or_percent) & Other);

// common_draw_fill_attlist
class common_draw_fill_attlist	
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_draw_fill_attlist & Other);
	void serialize(CP_ATTR_NODE);

	_CP_OPT(percent)			draw_opacity_;	
	_CP_OPT(percent)			draw_image_opacity_;

	_CP_OPT(draw_fill)			draw_fill_;

	_CP_OPT(color)				draw_fill_color_;
	
	_CP_OPT(std::wstring)		draw_fill_image_name_;
	_CP_OPT(std::wstring)		draw_fill_gradient_name_;
	_CP_OPT(std::wstring)		draw_fill_hatch_name_;
	_CP_OPT(std::wstring)		draw_opacity_name_;
	
	_CP_OPT(Bool)				draw_fill_hatch_solid_;
	_CP_OPT(style_repeat)		style_repeat_;

	_CP_OPT(fill_image_ref_point)draw_fill_image_ref_point_;
	_CP_OPT(percent)			draw_fill_image_ref_point_y_;
	_CP_OPT(percent)			draw_fill_image_ref_point_x_;

	_CP_OPT(length_or_percent)	draw_fill_image_width_;
	_CP_OPT(length_or_percent)	draw_fill_image_height_;
};

// common-horizontal-margin-attlist
class common_horizontal_margin_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_horizontal_margin_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(length_or_percent) fo_margin_left_;
    _CP_OPT(length_or_percent) fo_margin_right_;
};

// common-vertical-margin-attlist
class common_vertical_margin_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );   
    void apply_from(const common_vertical_margin_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(length_or_percent) fo_margin_top_;
    _CP_OPT(length_or_percent) fo_margin_bottom_;
};

//  common-margin-attlist
class common_margin_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_margin_attlist & Other);

	void serialize(CP_ATTR_NODE);

    _CP_OPT(length_or_percent) fo_margin_;
};

//  common-math_style_attlist
class common_math_style_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_math_style_attlist & Other);

	void serialize(CP_ATTR_NODE);

    _CP_OPT(math_variant) mathvariant_;
};

// common-break-attlist
class common_break_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_break_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(fo_break) fo_break_before_;
    _CP_OPT(fo_break) fo_break_after_;

};

// common-page-number-attlist
class common_page_number_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_page_number_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(unsigned int) style_page_number_;

};

// common-background-color-attlist
class common_background_color_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_background_color_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(background_color) fo_background_color_;

};

// common-shadow-attlist
class common_shadow_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_shadow_attlist & Other);
	void serialize(CP_ATTR_NODE);

	_CP_OPT(shadow_type) style_shadow_;

};

// common-keep-with-next-attlist
class common_keep_with_next_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_keep_with_next_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(keep_together) keep_together_;

};

// common-writing-mode-attlist
class common_writing_mode_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_writing_mode_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(writing_mode) style_writing_mode_;
};

/// common-draw-data-attlist
class common_xlink_attlist
{
public:
    bool add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_xlink_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(std::wstring)	href_;
    _CP_OPT(xlink_type)		type_;
    _CP_OPT(xlink_show)		show_;    
    _CP_OPT(xlink_actuate)	actuate_;    

};

// common-value-and-type-attlist
class common_value_and_type_attlist
{
public:
    bool add_attributes( const xml::attributes_wc_ptr & Attributes );    
    void apply_from(const common_value_and_type_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(office_value_type)	office_value_type_;
    _CP_OPT(std::wstring)		office_value_;
    _CP_OPT(std::wstring)		office_currency_;
    _CP_OPT(std::wstring)		office_date_value_;
    _CP_OPT(std::wstring)		office_time_value_;
    _CP_OPT(std::wstring)		office_boolean_value_;
    _CP_OPT(std::wstring)		office_string_value_;
};

// common-border-attlist
class common_border_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );    
    void apply_from(const common_border_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(border_style) fo_border_;
    _CP_OPT(border_style) fo_border_top_;
    _CP_OPT(border_style) fo_border_bottom_;
	_CP_OPT(border_style) fo_border_left_;
	_CP_OPT(border_style) fo_border_right_;
    
};

// common-border-line-width-attlist
class common_border_line_width_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );    
    void apply_from(const common_border_line_width_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(border_widths) style_border_line_width_;
    _CP_OPT(border_widths) style_border_line_width_top_;
    _CP_OPT(border_widths) style_border_line_width_bottom_;
    _CP_OPT(border_widths) style_border_line_width_left_;
    _CP_OPT(border_widths) style_border_line_width_right_;

};

// common-padding-attlist
class common_padding_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );  
    void apply_from(const common_padding_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(length) fo_padding_;
    
    _CP_OPT(length) fo_padding_top_;
    _CP_OPT(length) fo_padding_bottom_;
    _CP_OPT(length) fo_padding_left_;
    _CP_OPT(length) fo_padding_right_;
};

// common-rotation-angle-attlist
class common_rotation_angle_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );  
    void apply_from(const common_rotation_angle_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(unsigned int) style_rotation_angle_;

};

// common-num-format-attlist
class common_num_format_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_num_format_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(std::wstring)	style_num_format_;
    _CP_OPT(Bool)			style_num_letter_sync_;

};

// common-num-format-prefix-suffix-attlist
class common_num_format_prefix_suffix_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_num_format_prefix_suffix_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(std::wstring) style_num_prefix_;
    _CP_OPT(std::wstring) style_num_suffix_;
};


// common-text-align
class common_text_align
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_text_align & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(text_align) fo_text_align_;
};

// common-draw-size-attlist
class common_draw_size_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_draw_size_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(length) svg_width_;
    _CP_OPT(length) svg_height_;

};

/// common-text-anchor-attlist
class common_text_anchor_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_text_anchor_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(anchor_type)	type_;
    _CP_OPT(unsigned int)	page_number_;
};

/// common-text-animation-attlist
class common_text_animation_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_text_anchor_attlist & Other)
    {
        //_CP_APPLY_PROP2(type_);
        //_CP_APPLY_PROP2(direction_);
    }
	void serialize(CP_ATTR_NODE){}

 //   _CP_OPT(animation_type)			type_;
	//_CP_OPT(animation_direction)	direction_;
	//text:animation
	//text:animation-direction
	//text:animation-start-inside
	//text:animation-stop-inside
	//text:animation-repeat
};


// common-draw-rel-size-attlist
class common_draw_rel_size_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_draw_rel_size_attlist & Other);
	void serialize(CP_ATTR_NODE);


    common_draw_size_attlist	common_draw_size_attlist_;
    _CP_OPT(percent_or_scale)	style_rel_width_;
    _CP_OPT(percent_or_scale)	style_rel_height_;
};

// common-vertical-rel-attlist
class common_vertical_rel_attlist 
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_vertical_rel_attlist & Other);
	void serialize(CP_ATTR_NODE);
	
    _CP_OPT(vertical_rel) style_vertical_rel_; 

};

// common-horizontal-rel-attlist
class common_horizontal_rel_attlist 
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_horizontal_rel_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(horizontal_rel) style_horizontal_rel_;

};

// common-vertical-pos-attlist
class common_vertical_pos_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_vertical_pos_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(vertical_pos) style_vertical_pos_;
    _CP_OPT(length) svg_y_;

};

// common-horizontal-pos-attlist
class common_horizontal_pos_attlist
{
public:

    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_horizontal_pos_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(horizontal_pos) style_horizontal_pos_;
    _CP_OPT(length)			svg_x_;

};

// common-draw-style-name-attlist
class common_draw_style_name_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_draw_style_name_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(style_ref)		draw_style_name_;
    _CP_OPT(std::wstring)	draw_class_names_;
    _CP_OPT(style_ref)		presentation_style_name_;
    _CP_OPT(std::wstring)	presentation_class_names_;

};

/// common-draw-z-index-attlist
class common_draw_z_index_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_draw_z_index_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(int) draw_z_index_;
};

/// common-draw-id-attlist
class common_draw_id_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_draw_id_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(std::wstring) draw_id_;
};

/// common-draw-layer-name-attlist
class common_draw_layer_name_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_draw_layer_name_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(std::wstring) draw_layer_;
};

/// common-draw-transform-attlist
class common_draw_transform_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_draw_transform_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(std::wstring) draw_transform_;
};

/// common-draw-name-attlist
class common_draw_name_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_draw_name_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(std::wstring) draw_name_;
};

/// common-text-spreadsheet-shape-attlist
class common_text_spreadsheet_shape_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_text_spreadsheet_shape_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(std::wstring)		table_end_cell_address_;
    _CP_OPT(length)				table_end_x_;
    _CP_OPT(length)				table_end_y_;
    _CP_OPT(Bool)				table_table_background_;
    common_text_anchor_attlist	common_text_anchor_attlist_;

};


/// common-draw-shape-with-styles-attlist
class common_draw_shape_with_styles_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_draw_shape_with_styles_attlist & Other);
	void serialize(CP_ATTR_NODE);

    common_draw_z_index_attlist             common_draw_z_index_attlist_;
    common_draw_id_attlist                  common_draw_id_attlist_;
    common_draw_layer_name_attlist          common_draw_layer_name_attlist_;
    common_draw_style_name_attlist          common_draw_style_name_attlist_;
    common_draw_transform_attlist           common_draw_transform_attlist_;
    common_draw_name_attlist                common_draw_name_attlist_;
    common_text_spreadsheet_shape_attlist   common_text_spreadsheet_shape_attlist_;

};

/// common-draw-text-style-name-attlist
class common_draw_text_style_name_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_draw_text_style_name_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(style_ref) draw_text_style_name_;
};

/// common-draw-position-attlist
class common_draw_position_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_draw_position_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(length) svg_x_;
    _CP_OPT(length) svg_y_;

};

/// common-draw-shape-with-text-and-styles-attlist
class common_draw_shape_with_text_and_styles_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_draw_shape_with_text_and_styles_attlist & Other);
	void serialize(CP_ATTR_NODE);

    common_draw_shape_with_styles_attlist	common_draw_shape_with_styles_attlist_;
    common_draw_text_style_name_attlist		common_draw_text_style_name_attlist_;
};

/// presentation-shape-attlist
class presentation_shape_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const presentation_shape_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(std::wstring)	presentation_class_;
    _CP_OPT(Bool)			presentation_placeholder_;
    _CP_OPT(Bool)			presentation_user_transformed_;
    
};

///  common-data-style-attlist
class common_data_style_attlist 
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
	void serialize(CP_ATTR_NODE);

    _CP_OPT(style_ref)		style_name_;
    _CP_OPT(std::wstring) number_language_;
    _CP_OPT(std::wstring) number_country_;
    _CP_OPT(std::wstring) number_title_;
    _CP_OPT(Bool)			number_volatile_;
    _CP_OPT(std::wstring) number_transliteration_format_;
    _CP_OPT(std::wstring) number_transliteration_language_;
    _CP_OPT(std::wstring) number_transliteration_country_;
    _CP_OPT(std::wstring) number_transliteration_style_;

};


struct union_common_draw_attlists
{
    common_draw_shape_with_text_and_styles_attlist	shape_with_text_and_styles_;
    common_draw_position_attlist					position_;
    common_draw_rel_size_attlist					rel_size_;
	
	void serialize(CP_ATTR_NODE);
};


// common-presentation-attlist
class common_presentation_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_presentation_attlist & Other);
	void serialize(CP_ATTR_NODE);

    _CP_OPT(presentation_class) presentation_class_;
	_CP_OPT(std::wstring) style_name_;
	_CP_OPT(Bool) presentation_placeholder_;

};


// common-anim-smil-attlist
class common_anim_smil_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const common_anim_smil_attlist & Other);
	void serialize(CP_ATTR_NODE){}

	//_CP_OPT(std::wstring) smil_direction_;
	//_CP_OPT(std::wstring) smil_subtype_; 
	//_CP_OPT(std::wstring) smil_type_;
	//_CP_OPT(std::wstring) smil_dur_;

	//_CP_OPT(color)	smil_fadeColor;

};

}
}
