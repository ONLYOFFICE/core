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

#include <string>
#include <vector>
#include <iosfwd>

#include <CPNoncopyable.h>
#include <xml/attributes.h>

#include <CPSharedPtr.h>
#include <CPWeakPtr.h>
#include <CPOptional.h>

#include "office_elements_create.h"

#include "../../DataTypes/stylefamily.h"
#include "../../DataTypes/textalign.h"
#include "../../DataTypes/length.h"
#include "../../DataTypes/styleverticalrel.h"
#include "../../DataTypes/styleverticalpos.h"
#include "../../DataTypes/percent.h"
#include "../../DataTypes/verticalalign.h"
#include "../../DataTypes/pageusage.h"
#include "../../DataTypes/tablecentering.h"
#include "../../DataTypes/layoutgridmode.h"
#include "../../DataTypes/direction.h"
#include "../../DataTypes/linestyle.h"
#include "../../DataTypes/styletype.h"
#include "../../DataTypes/noteclass.h"
#include "../../DataTypes/gradientstyle.h"
#include "../../DataTypes/hatchstyle.h"
#include "../../DataTypes/fontstyle.h"
#include "../../DataTypes/fontvariant.h"
#include "../../DataTypes/fontweight.h"
#include "../../DataTypes/fontstretch.h"
#include "../../DataTypes/fontfamilygeneric.h"
#include "../../DataTypes/fontpitch.h"
#include "../../DataTypes/drawangle.h"
#include "../../DataTypes/common_attlists.h"

#include "header_footer.h"

#include "styles_list.h"
#include "style_map.h"


namespace cpdoccore { 
namespace odf_writer { 

class graphic_format_properties;

class style_text_properties;
class style_paragraph_properties;
class style_section_properties;
class style_table_cell_properties;
class style_table_row_properties;
class style_table_column_properties;
class style_chart_properties;
class style_table_properties;
class style_drawing_page_properties;

class style_content : noncopyable
{
public:
	style_content(odf_conversion_context * _context){Context = _context;}

	void create_child_element	( const std::wstring & Ns, const std::wstring & Name);
    void add_child_element		( const office_element_ptr & child);
	void serialize				(std::wostream & strm);
  
    graphic_format_properties *		get_graphic_properties() ;
    
	style_text_properties *			get_style_text_properties();
    style_paragraph_properties *	get_style_paragraph_properties() ;
    style_table_properties *		get_style_table_properties();
    style_section_properties *		get_style_section_properties();
    style_table_cell_properties *	get_style_table_cell_properties();
    style_table_row_properties *	get_style_table_row_properties() ;
    style_table_column_properties * get_style_table_column_properties() ;
    style_chart_properties *		get_style_chart_properties() ;
	style_drawing_page_properties*	get_style_drawing_page_properties();

    odf_types::style_family style_family_;
private:
	odf_conversion_context * Context;

    office_element_ptr		style_text_properties_;
    office_element_ptr		style_paragraph_properties_;
    office_element_ptr		style_section_properties_;
    office_element_ptr		style_ruby_properties_;
    office_element_ptr		style_table_properties_;
    office_element_ptr		style_table_column_properties_;
    office_element_ptr		style_table_row_properties_;
    office_element_ptr		style_chart_properties_;
    office_element_ptr		style_graphic_properties_;
    office_element_ptr		style_table_cell_properties_;
	office_element_ptr		style_drawing_page_properties_;

};

class default_style : public office_element_impl<default_style>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleDefaultStyle;
    

	default_style() : content_(getContext()) {}

    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    odf_types::style_family style_family_;
    style_content			content_;

};

CP_REGISTER_OFFICE_ELEMENT2(default_style)


//----------------------------------------------------------------------------------------------------
class draw_gradient : public office_element_impl<draw_gradient>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleDrawGradient;

    

	std::wstring				get_style_name(){return draw_name_.get_value_or(L"");}

	_CP_OPT(odf_types::color)			draw_start_color_;
	_CP_OPT(odf_types::color)			draw_end_color_;

	_CP_OPT(odf_types::percent)			draw_end_intensity_; 
	_CP_OPT(odf_types::percent)			draw_start_intensity_;

	_CP_OPT(odf_types::percent)			draw_cy_;//%
	_CP_OPT(odf_types::percent)			draw_cx_;
	
	_CP_OPT(odf_types::percent)			draw_border_;
	_CP_OPT(odf_types::draw_angle)		draw_angle_;
	_CP_OPT(odf_types::gradient_style)	draw_style_;

 	_CP_OPT(std::wstring)				draw_name_;
	_CP_OPT(std::wstring)				draw_display_name_;
	
    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
    virtual void serialize(std::wostream & strm);

};
CP_REGISTER_OFFICE_ELEMENT2(draw_gradient)

//----------------------------------------------------------------------------------------------------
class draw_hatch : public office_element_impl<draw_hatch>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeStyleDrawHatch;

    

	std::wstring	get_style_name(){return draw_name_.get_value_or(L"");}
	
	_CP_OPT(odf_types::hatch_style)	draw_style_;
	_CP_OPT(int)					draw_rotation_;
	_CP_OPT(odf_types::length)		draw_distance_;	
	_CP_OPT(odf_types::color)		draw_color_;

 	_CP_OPT(std::wstring)			draw_name_;
	_CP_OPT(std::wstring)			draw_display_name_;

    virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void serialize(std::wostream & strm);
 
};
CP_REGISTER_OFFICE_ELEMENT2(draw_hatch)

//----------------------------------------------------------------------------------------------------
class draw_opacity : public office_element_impl<draw_opacity>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeStyleDrawOpacity;

    

	std::wstring	get_style_name(){return draw_name_.get_value_or(L"");}

	_CP_OPT(odf_types::gradient_style)	draw_style_;//linear, radial, ..
	_CP_OPT(odf_types::draw_angle)		draw_angle_;

	_CP_OPT(odf_types::percent)		draw_cy_;//%
	_CP_OPT(odf_types::percent)		draw_cx_;

	_CP_OPT(odf_types::percent)		draw_start_;
	_CP_OPT(odf_types::percent)		draw_end_;
	
	_CP_OPT(odf_types::percent)		draw_border_;

 	_CP_OPT(std::wstring)			draw_name_;
	_CP_OPT(std::wstring)			draw_display_name_;
	
    virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void serialize(std::wostream & strm);

};
CP_REGISTER_OFFICE_ELEMENT2(draw_opacity)

//----------------------------------------------------------------------------------------------------
class draw_layer : public office_element_impl<draw_layer>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeStyleDrawLayer;

    

 	_CP_OPT(std::wstring)			draw_name_;
	
    virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void serialize(std::wostream & strm);

};
CP_REGISTER_OFFICE_ELEMENT2(draw_layer)
//----------------------------------------------------------------------------------------------------
class draw_layer_set : public office_element_impl<draw_layer_set>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeStyleDrawLayerSet;

    

    office_element_ptr_array	content_;
	
    virtual void add_child_element( const office_element_ptr & child);
    virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void serialize(std::wostream & strm);

};
CP_REGISTER_OFFICE_ELEMENT2(draw_layer_set)

//----------------------------------------------------------------------------------------------------
class draw_fill_image : public office_element_impl<draw_fill_image>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeStyleDrawFillImage;

    

	std::wstring get_style_name(){return draw_name_.get_value_or(L"");}
	
	odf_types::common_xlink_attlist		xlink_attlist_;

 	_CP_OPT(std::wstring)				draw_name_;
	_CP_OPT(std::wstring)				draw_display_name_;

    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
	virtual void serialize(std::wostream & strm);
};
CP_REGISTER_OFFICE_ELEMENT2(draw_fill_image)

//----------------------------------------------------------------------------------------------------
class draw_marker : public office_element_impl<draw_marker>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeStyleDrawMarker;

    

	std::wstring get_style_name(){return draw_name_.get_value_or(L"");}
	
	_CP_OPT(std::wstring)		svg_viewBox_;
	_CP_OPT(std::wstring)		svg_d_;

 	_CP_OPT(std::wstring)		draw_name_;
	_CP_OPT(std::wstring)		draw_display_name_;

    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
	virtual void serialize(std::wostream & strm);
};
CP_REGISTER_OFFICE_ELEMENT2(draw_marker)

//----------------------------------------------------------------------------------------------------
class style;
typedef shared_ptr<style>::Type style_ptr;
typedef weak_ptr<style>::Type style_weak_ptr;
typedef std::vector< style_weak_ptr > style_weak_ptr_array;
typedef std::vector< style_ptr > style_ptr_array;

class style : public office_element_impl<style>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeStyleStyle;
    
 
    style() : content_(getContext()) {} 
    
	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    std::wstring			style_name_;
    _CP_OPT( std::wstring ) style_display_name_; 
    odf_types::style_family	style_family_;

    _CP_OPT( std::wstring ) style_parent_style_name_; 
    _CP_OPT( std::wstring ) style_next_style_name_; 
    _CP_OPT( std::wstring ) style_list_style_name_; 
    _CP_OPT( std::wstring ) style_master_page_name_;
	_CP_OPT( bool)			style_auto_update_; // default = false
    _CP_OPT( std::wstring ) style_data_style_name_; 
    _CP_OPT( std::wstring ) style_class_;
    _CP_OPT(std::wstring)	style_default_outline_level_; 

    style_content				content_;
    office_element_ptr_array	style_map_;

};

CP_REGISTER_OFFICE_ELEMENT2(style)

//----------------------------------------------------------------------------------------------------
class styles
{
public:
    void create_child_element(const std::wstring & Ns, const std::wstring & Name, odf_conversion_context * Context);
	void add_child_element( const office_element_ptr & child, odf_conversion_context * Context);

	void serialize(std::wostream & strm);

    office_element_ptr_array style_style_;      ///< style-style
    office_element_ptr_array text_list_style_;  ///< text-list-style

    // number-number-style
    // number-currency-style
    // number-percentage-style
    // number-date-style
    // number-time-style
    // number-boolean-style
    office_element_ptr_array number_styles_; 

};

class draw_styles
{
public:
    void create_child_element(const std::wstring & Ns, const std::wstring & Name, odf_conversion_context * Context);
	void add_child_element( const office_element_ptr & child, odf_conversion_context * Context);

	void serialize(std::wostream & strm);

	office_element_ptr_array draw_gradient_;    
	office_element_ptr_array draw_fill_image_; 
    office_element_ptr_array draw_opacity_;     
	office_element_ptr_array draw_hatch_;    
    
	office_element_ptr_array draw_marker_; 
    office_element_ptr_array draw_stroke_dash_; 
	
	office_element_ptr_array svg_linearGradient_; 
    office_element_ptr_array svg_radialGradient_; 

};
class templates
{
public:
    void create_child_element(const std::wstring & Ns, const std::wstring & Name, odf_conversion_context * Context);
    void add_child_element( const office_element_ptr & child);

	void serialize(std::wostream & strm);

	office_element_ptr_array table_templates_;    

};

class office_automatic_styles : public office_element_impl<office_automatic_styles>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeOfficeAutomaticStyles;
    

    virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    styles styles_; ///< styles
    office_element_ptr_array style_page_layout_; 
};

CP_REGISTER_OFFICE_ELEMENT2(office_automatic_styles)

//----------------------------------------------------------------------------------------------------
class office_master_styles : public office_element_impl<office_master_styles>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeOfficeMasterStyles;
    

    virtual void create_child_element(  const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    office_element_ptr_array	style_master_page_;		// разметки тем
    office_element_ptr			style_handout_master_;	// разметки для принтера - .. второстепенно
    office_element_ptr			draw_layer_set_;		// необязательно .. так как слои все равно не поддерживаются в мс.
													// то есть не будут объекты объеденены по признаку слоя
													// зы. не путать с обычной группировкой

};

CP_REGISTER_OFFICE_ELEMENT2(office_master_styles)

//----------------------------------------------------------------------------------------------------
class style_master_page_attlist
{
public:
	void serialize(CP_ATTR_NODE);

    _CP_OPT(std::wstring)	style_name_;
    _CP_OPT(std::wstring)	style_display_name_;
    _CP_OPT(std::wstring)	style_page_layout_name_;
    
	_CP_OPT(std::wstring)	draw_style_name_;
    _CP_OPT(std::wstring)	style_next_style_name_;
};


//----------------------------------------------------------------------------------------------------
class style_master_page;
typedef boost::shared_ptr<style_master_page> style_master_page_ptr;

class style_master_page: public office_element_impl<style_master_page>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleMasterPage;
    

    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

	int find_placeHolderIndex(odf_types::presentation_class::type placeHolder,int & last_idx);
   
	style_master_page_attlist attlist_;

    office_element_ptr style_header_;  
    office_element_ptr style_header_left_; 
    office_element_ptr style_header_first_; 

	office_element_ptr style_footer_;      
    office_element_ptr style_footer_left_; 
    office_element_ptr style_footer_first_; 

	office_element_ptr office_forms_;       // TODO
    office_element_ptr_array style_style_;
    office_element_ptr presentation_notes_; // TODO           
    
	office_element_ptr_array content_;        // shapes, frames, text ...
};

CP_REGISTER_OFFICE_ELEMENT2(style_master_page)

//----------------------------------------------------------------------------------------------------
class office_styles : public office_element_impl<office_styles>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeOfficeStyles;
    

    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    styles		styles_;
	draw_styles draw_styles_;	
	templates	templates_;	
	
	office_element_ptr_array	style_default_style_;
	office_element_ptr_array	style_presentation_page_layout_;
    
	office_element_ptr			text_outline_style_; 
    office_element_ptr_array	text_notes_configuration_;
    office_element_ptr			text_bibliography_configuration_; // < TODO
	office_element_ptr_array	text_linenumbering_configuration_;

};
CP_REGISTER_OFFICE_ELEMENT2(office_styles)

//----------------------------------------------------------------------------------------------------
class header_footer_content_common
{
public:
    common_style_header_footer_attlist	common_style_header_footer_attlist_;
    header_footer_content				header_footer_content_;
};

class header_footer_common
{
public:
    header_footer_content_common & content() { return content_; }
    const header_footer_content_common & content() const { return content_; }
    virtual ~header_footer_common() {}

private:
    header_footer_content_common content_;

};

//----------------------------------------------------------------------------------------------------
class style_header : public office_element_impl<style_header>, public header_footer_common
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleHeader;
    

    virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    header_footer_content_common content_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_header)

//----------------------------------------------------------------------------------------------------
class style_footer : public office_element_impl<style_footer>, public header_footer_common
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleFooter;
    

	virtual void serialize(std::wostream & strm);

    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

    header_footer_content_common content_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_footer)

//----------------------------------------------------------------------------------------------------
class style_header_first : public office_element_impl<style_header_first>, public header_footer_common
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleHeaderFirst;
    

	virtual void serialize(std::wostream & strm);

    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

    header_footer_content_common content_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_header_first)

//----------------------------------------------------------------------------------------------------
class style_footer_first : public office_element_impl<style_footer_first>, public header_footer_common
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleFooterFirst;
    

    virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    header_footer_content_common content_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_footer_first)

//----------------------------------------------------------------------------------------------------
class style_header_left : public office_element_impl<style_header_left>, public header_footer_common
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleHeaderLeft;
    

    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);
  
	header_footer_content_common content_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_header_left)

//----------------------------------------------------------------------------------------------------
class style_footer_left : public office_element_impl<style_footer_left>, public header_footer_common
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleFooterLeft;
    

    virtual void create_child_element(  const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

	header_footer_content_common content_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_footer_left)

//----------------------------------------------------------------------------------------------------
class style_page_layout_attlist
{
public:
	void serialize(CP_ATTR_NODE);

	_CP_OPT(std::wstring)			style_name_;
    _CP_OPT(odf_types::page_usage)	style_page_usage_; // default All
        
};

//----------------------------------------------------------------------------------------------------
class style_header_style : public office_element_impl<style_header_style>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleHeaderStyle;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

	office_element_ptr style_header_footer_properties_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_header_style)

//----------------------------------------------------------------------------------------------------
class style_footer_style : public office_element_impl<style_footer_style>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleFooterStyle;
    

    virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

	office_element_ptr style_header_footer_properties_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_footer_style)

//----------------------------------------------------------------------------------------------------
class style_page_layout : public office_element_impl<style_page_layout>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStylePageLayout;
    
   
	style_page_layout_attlist style_page_layout_attlist_;
   
	office_element_ptr style_page_layout_properties_;
    office_element_ptr style_header_style_;
    office_element_ptr style_footer_style_;

    virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);  
};

CP_REGISTER_OFFICE_ELEMENT2(style_page_layout)

//----------------------------------------------------------------------------------------------------
class style_footnote_sep_attlist
{
public:
	void serialize(CP_ATTR_NODE);

    _CP_OPT(odf_types::length)	style_width_;
    _CP_OPT(odf_types::percent) style_rel_width_;
    _CP_OPT(odf_types::color)	style_color_;
    _CP_OPT(odf_types::line_style) style_line_style_;
	_CP_OPT(odf_types::style_type) style_adjustment_; // default Left
	_CP_OPT(odf_types::length)	style_distance_before_sep_;
    _CP_OPT(odf_types::length)	style_distance_after_sep_;
                
};

//----------------------------------------------------------------------------------------------------
class style_footnote_sep : public office_element_impl<style_footnote_sep>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleFootnoteSep;
    

    virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void serialize(std::wostream & strm);

    style_footnote_sep_attlist style_footnote_sep_attlist_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(style_footnote_sep)

//----------------------------------------------------------------------------------------------------
class text_notes_configuration : public office_element_impl<text_notes_configuration>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextNotesConfiguration;
    

    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
	virtual void serialize(std::wostream & strm){}

    odf_types::noteclass noteclass_;
    _CP_OPT(std::wstring) text_citation_style_name_;
    _CP_OPT(std::wstring) text_citation_body_style_name_;
    _CP_OPT(std::wstring) text_default_style_name_;
    _CP_OPT(std::wstring) text_master_page_name_;
    _CP_OPT(unsigned int) text_start_value_;
    odf_types::common_num_format_prefix_suffix_attlist common_num_format_prefix_suffix_attlist_;
    odf_types::common_num_format_attlist common_num_format_attlist_;
    _CP_OPT(std::wstring) text_start_numbering_at_;   
    _CP_OPT(std::wstring) text_footnotes_position_;

    office_element_ptr text_note_continuation_notice_forward_;
    office_element_ptr text_note_continuation_notice_backward_;
    
};
CP_REGISTER_OFFICE_ELEMENT2(text_notes_configuration)
//-------------------------------------------------------------------------------------------------------------------------
class text_linenumbering_configuration : public office_element_impl<text_linenumbering_configuration>
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;

	static const ElementType type = typeTextLinenumberingConfiguration;
	

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void serialize(std::wostream & strm);

	bool								text_number_lines_;
	_CP_OPT(std::wstring)				text_style_name_;
	_CP_OPT(odf_types::style_numformat)	style_num_format_;
	_CP_OPT(std::wstring)				style_num_letter_sync_;
	_CP_OPT(odf_types::Bool)			text_count_empty_lines_;
	_CP_OPT(odf_types::Bool)			text_count_in_text_boxes_;
	_CP_OPT(unsigned int)				text_increment_;
	_CP_OPT(std::wstring)				text_number_position_; //inner, left, outer, right
	_CP_OPT(odf_types::length)			text_offset_;
	_CP_OPT(odf_types::Bool)			text_restart_on_page_;

	office_element_ptr text_linenumbering_separator_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_linenumbering_configuration);
//-------------------------------------------------------------------------------------------------------------------------
class text_linenumbering_separator : public office_element_impl<text_linenumbering_separator>
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;

	static const ElementType type = typeTextLinenumberingSeparator;
	

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name) {}
	virtual void serialize(std::wostream & strm);

	_CP_OPT(unsigned int)	text_increment_;
	_CP_OPT(std::wstring)	text_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_linenumbering_separator);
//----------------------------------------------------------------------------------------------------
class style_presentation_page_layout;
typedef boost::shared_ptr<style_presentation_page_layout> style_presentation_page_layout_ptr;

class style_presentation_page_layout : public office_element_impl<style_presentation_page_layout>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStylePresentationPageLayout;
    

	virtual void serialize(std::wostream & strm);
    virtual void create_child_element	( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child);


	_CP_OPT(std::wstring) style_name_;

	office_element_ptr_array content_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_presentation_page_layout)

//----------------------------------------------------------------------------------------------------

//  style:font-face
class style_font_face : public office_element_impl<style_font_face>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleFontFace;

    

	virtual void serialize(std::wostream & strm);
	virtual void create_child_element	( const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child){}
	//todooo <svg:definition-src> and <svg:font-face-src>
    
    _CP_OPT(std::wstring)						style_name_;
    _CP_OPT(std::wstring)						style_font_adornments_;
    _CP_OPT(odf_types::font_family_generic)		style_font_family_generic_;
    _CP_OPT(odf_types::font_pitch)				style_font_pitch_;
    _CP_OPT(std::wstring)						style_font_charset_;
    
    _CP_OPT(std::wstring)				svg_font_family_;
    _CP_OPT(odf_types::font_style)		svg_font_style_;
    _CP_OPT(odf_types::font_variant)	svg_font_variant_;
    _CP_OPT(odf_types::font_weight)		svg_font_weight_;
    _CP_OPT(odf_types::font_stretch)	svg_font_stretch_;
    _CP_OPT(odf_types::length)		svg_font_size_;
    _CP_OPT(std::wstring)			svg_unicode_range_;
    _CP_OPT(int)					svg_units_per_em_;
    _CP_OPT(std::wstring)			svg_panose_1_;
    _CP_OPT(int)					svg_stemv_;
    _CP_OPT(int)					svg_stemh_;
    _CP_OPT(int)					svg_slope_;
    _CP_OPT(int)					svg_cap_height_;
    _CP_OPT(int)					svg_x_height_;
    _CP_OPT(int)					svg_accent_height_;
    _CP_OPT(int)					svg_ascent_;
    _CP_OPT(int)					svg_descent_;
    _CP_OPT(std::wstring)			svg_widths_;
    _CP_OPT(std::wstring)			svg_bbox_;
    _CP_OPT(int)					svg_ideographic_;
    _CP_OPT(int)					svg_alphabetic_;
    _CP_OPT(int)					svg_mathematical_;
    _CP_OPT(int)					svg_hanging_;
    _CP_OPT(int)					svg_v_ideographic_;
    _CP_OPT(int)					svg_v_alphabetic_;
    _CP_OPT(int)					svg_v_mathematical_;
    _CP_OPT(int)					svg_v_hanging_;
    _CP_OPT(int)					svg_underline_position_;
    _CP_OPT(int)					svg_underline_thickness_;
    _CP_OPT(int)					svg_strikethrough_position_;
    _CP_OPT(int)					svg_strikethrough_thickness_;
    _CP_OPT(int)					svg_overline_position_;
    _CP_OPT(int)					svg_overline_thickness_;

    office_element_ptr_array		svg_font_face_uri_; 
    office_element_ptr_array		svg_font_face_name_;   
    office_element_ptr				svg_definition_src_;

};
CP_REGISTER_OFFICE_ELEMENT2(style_font_face);

//  office-font-face-decls
class office_font_face_decls : public office_element_impl<office_font_face_decls>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeOfficeFontFaceDecls;

    

	virtual void serialize(std::wostream & strm);
    virtual void create_child_element	( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child);

    office_element_ptr_array content_;

};
CP_REGISTER_OFFICE_ELEMENT2(office_font_face_decls);

//  style:region-left
class style_region_left : public office_element_impl<style_region_left>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleRegionLeft;
    

	virtual void serialize(std::wostream & strm);
    virtual void create_child_element	( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child);

	office_element_ptr_array content_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_region_left);

//  style:region-right
class style_region_right : public office_element_impl<style_region_right>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleRegionRight;
    

	virtual void serialize(std::wostream & strm);
    virtual void create_child_element	( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child);

    office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_region_right);


//  style:region-center
class style_region_center : public office_element_impl<style_region_center>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleRegionCenter;
    

	virtual void serialize(std::wostream & strm);
    virtual void create_child_element	( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child);

    office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_region_center);
}
} 
