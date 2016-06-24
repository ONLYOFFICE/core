/*
 * (c) Copyright Ascensio System SIA 2010-2016
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

#include <string>
#include <vector>
#include <iosfwd>

#include <cpdoccore/CPNoncopyable.h>
#include <cpdoccore/xml/attributes.h>

#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPWeakPtr.h>
#include <cpdoccore/CPOptional.h>

#include "office_elements.h"
#include "office_elements_create.h"

#include "datatypes/stylefamily.h"
#include "datatypes/style_ref.h"
#include "datatypes/textalign.h"
#include "datatypes/lengthorpercent.h"
#include "datatypes/styleverticalrel.h"
#include "datatypes/styleverticalpos.h"
#include "datatypes/verticalalign.h"
#include "datatypes/pageusage.h"
#include "datatypes/tablecentering.h"
#include "datatypes/layoutgridmode.h"
#include "datatypes/direction.h"
#include "datatypes/linestyle.h"
#include "datatypes/styletype.h"
#include "datatypes/common_attlists.h"
#include "datatypes/noteclass.h"
#include "datatypes/gradientstyle.h"
#include "datatypes/hatchstyle.h"

#include "header_footer.h"

#include "styles_list.h"
#include "style_map.h"


namespace cpdoccore { namespace odf_reader { 

class style_text_properties;
class style_paragraph_properties;
class style_graphic_properties;
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
    void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name, document_context * Context);
    
	void docx_convert(oox::docx_conversion_context & Context, bool in_styles = false);
    void xlsx_convert(oox::xlsx_conversion_context & Context);
   
    style_text_properties *			get_style_text_properties() const;
    style_paragraph_properties *	get_style_paragraph_properties() const;
    style_graphic_properties *		get_style_graphic_properties() const;
    style_table_properties *		get_style_table_properties() const;
    style_section_properties *		get_style_section_properties() const;
    style_table_cell_properties *	get_style_table_cell_properties() const;
    style_table_row_properties *	get_style_table_row_properties() const;
    style_table_column_properties * get_style_table_column_properties() const;
    style_chart_properties *		get_style_chart_properties() const;
	style_drawing_page_properties*	get_style_drawing_page_properties() const;

private:
	odf_types::style_family style_family_;

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

//  default_style
///         style:default-style
class default_style : public office_element_impl<default_style>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleDefaultStyle;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

public:
    default_style() {};

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    odf_types::style_family style_family_;

    style_content style_content_;

    friend class odf_document;

};

CP_REGISTER_OFFICE_ELEMENT2(default_style);


/////////////////////////////////////////////////////////////////////////////////////////////////
//  style_draw_gradient
class draw_gradient : public office_element_impl<draw_gradient>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleDrawGradient;

    CPDOCCORE_DEFINE_VISITABLE();

	std::wstring	get_style_name(){return draw_name_.get_value_or(L"");}

	_CP_OPT(odf_types::color)		draw_start_color_;
	_CP_OPT(odf_types::color)		draw_end_color_;

	_CP_OPT(odf_types::percent)	draw_end_intensity_; 
	_CP_OPT(odf_types::percent)	draw_start_intensity_;

	_CP_OPT(odf_types::percent)	draw_cy_;//%
	_CP_OPT(odf_types::percent)	draw_cx_;
	
	_CP_OPT(odf_types::percent)	draw_border_;
	_CP_OPT(int)		draw_angle_;
	_CP_OPT(odf_types::gradient_style)		draw_style_;


private:
 	_CP_OPT(std::wstring)		draw_name_;
	_CP_OPT(std::wstring)		draw_display_name_;
	
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
 
    friend class odf_document;
};
CP_REGISTER_OFFICE_ELEMENT2(draw_gradient);
/////////////////////////////////////////////////////////////////////////////////////////////////
//  style_draw_hatch
class draw_hatch : public office_element_impl<draw_hatch>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleDrawHatch;

    CPDOCCORE_DEFINE_VISITABLE();

	std::wstring	get_style_name(){return draw_name_.get_value_or(L"");}
	
	_CP_OPT(odf_types::hatch_style)	draw_style_;
	_CP_OPT(int)			draw_rotation_;
	_CP_OPT(odf_types::length)			draw_distance_;	
	_CP_OPT(odf_types::color)			draw_color_;

private:
 	_CP_OPT(std::wstring)	draw_name_;
	_CP_OPT(std::wstring)	draw_display_name_;
	
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
 
    friend class odf_document;
};
CP_REGISTER_OFFICE_ELEMENT2(draw_hatch);
/////////////////////////////////////////////////////////////////////////////////////////////////
//  style_draw_gradient
class draw_opacity : public office_element_impl<draw_opacity>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleDrawGradient;

    CPDOCCORE_DEFINE_VISITABLE();

	std::wstring	get_style_name(){return draw_name_.get_value_or(L"");}

	_CP_OPT(odf_types::gradient_style)	draw_style_;//linear, radial, ..
	_CP_OPT(int)			draw_angle_;

	_CP_OPT(odf_types::percent)		draw_start_;
	_CP_OPT(odf_types::percent)		draw_end_;
	
	_CP_OPT(odf_types::percent)		draw_border_;

private:
 	_CP_OPT(std::wstring)	draw_name_;
	_CP_OPT(std::wstring)	draw_display_name_;
	
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
 
    friend class odf_document;
};
CP_REGISTER_OFFICE_ELEMENT2(draw_opacity);
//  style_draw_fill_image
class draw_fill_image : public office_element_impl<draw_fill_image>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleDrawFillImage;

    CPDOCCORE_DEFINE_VISITABLE();

	std::wstring get_style_name(){return draw_name_.get_value_or(L"");}
	
	odf_types::common_xlink_attlist		xlink_attlist_;

private:
 	_CP_OPT(std::wstring)		draw_name_;

    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    friend class odf_document;

};
CP_REGISTER_OFFICE_ELEMENT2(draw_fill_image);
/////////////////////////////////////////////////////////////////////////////////////////////////
class style;
typedef shared_ptr<style>::Type style_ptr;
typedef weak_ptr<style>::Type style_weak_ptr;
typedef ::std::vector< style_weak_ptr > style_weak_ptr_array;
typedef ::std::vector< style_ptr > style_ptr_array;

//  style
///         style:style
class style : public office_element_impl<style>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleStyle;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

public:
    style() : style_auto_update_(false) { }

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);
    
private:
    // attr
    std::wstring	style_name_;
    bool			style_auto_update_;				// default = false
    
	_CP_OPT( std::wstring ) style_display_name_; 
    odf_types::style_family style_family_;

    _CP_OPT( std::wstring ) style_parent_style_name_; 
    _CP_OPT( std::wstring ) style_next_style_name_;	 
    _CP_OPT( std::wstring ) style_list_style_name_;	
    _CP_OPT( std::wstring ) style_master_page_name_;
    _CP_OPT( std::wstring ) style_data_style_name_;	
    _CP_OPT( std::wstring ) style_class_;
    _CP_OPT( std::wstring ) style_default_outline_level_;  
private:
    style_content				style_content_;
    office_element_ptr_array	style_map_;

    friend class odf_document;

};

CP_REGISTER_OFFICE_ELEMENT2(style);

//  styles
class styles
{
public:
    void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name, document_context * Context);

private:
    office_element_ptr_array style_style_;      ///< style-style
    office_element_ptr_array text_list_style_;  ///< text-list-style

    // number-number-style
    // number-currency-style
    // number-percentage-style
    // number-date-style
    // number-time-style
    // number-boolean-style
    office_element_ptr_array number_styles_; 

    friend class odf_document;
};

class draw_styles
{
public:
    void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name, document_context * Context);

private:
	office_element_ptr_array draw_gradient_;    
	office_element_ptr_array draw_fill_image_; 
    office_element_ptr_array draw_opacity_;     
	office_element_ptr_array draw_hatch_;    
    
	office_element_ptr_array draw_marker_; // < TODO
    office_element_ptr_array draw_stroke_dash_; // < TODO
	
	office_element_ptr_array svg_linearGradient_; // < TODO
    office_element_ptr_array svg_radialGradient_; // < TODO

    friend class odf_document;
};
class templates
{
public:
    void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name, document_context * Context);

private:
	office_element_ptr_array table_templates_;    

    friend class odf_document;
};

//  office:automatic-styles
class office_automatic_styles : public office_element_impl<office_automatic_styles>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeAutomaticStyles;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    styles styles_; ///< styles
    office_element_ptr_array style_page_layout_; 
    friend class odf_document;
};

CP_REGISTER_OFFICE_ELEMENT2(office_automatic_styles);

//  office:master-styles
class office_master_styles : public office_element_impl<office_master_styles>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeMasterStyles;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    office_element_ptr_array style_master_page_;	// разметки тем
    office_element_ptr style_handout_master_;		// разметки для принтера - .. второстепенно
    office_element_ptr draw_layer_set_;				// необязательно .. так как слои все равно не поддерживаются в мс.
													// то есть не будут объекты объеденены по признаку слоя
													// зы. не путать с обычной группировкой

    friend class odf_document;
};

CP_REGISTER_OFFICE_ELEMENT2(office_master_styles);

// style-master-page-attlist
class style_master_page_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    
public:
    _CP_OPT(odf_types::style_ref)		style_name_;
    _CP_OPT(std::wstring)				style_display_name_;
    _CP_OPT(odf_types::style_ref)		style_page_layout_name_;
    
	_CP_OPT(std::wstring)				draw_style_name_;
    _CP_OPT(odf_types::style_ref)		style_next_style_name_;
};

//  style:master-page
class style_master_page;
typedef boost::shared_ptr<style_master_page> style_master_page_ptr;

class style_master_page: public office_element_impl<style_master_page>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleMasterPage;
    CPDOCCORE_DEFINE_VISITABLE();

public:
	virtual void pptx_convert(oox::pptx_conversion_context & Context);
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
	int find_placeHolderIndex(odf_types::presentation_class::type placeHolder,int & last_idx);
   
	style_master_page_attlist style_master_page_attlist_;

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

CP_REGISTER_OFFICE_ELEMENT2(style_master_page);

//  office:styles
class office_styles : public office_element_impl<office_styles>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeStyles;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

private:
    styles		styles_;
	draw_styles draw_styles_;	
	templates	templates_;	
	
	office_element_ptr_array style_default_style_;
	office_element_ptr_array style_presentation_page_layout_;
    
	office_element_ptr			text_outline_style_; // < TODO
    office_element_ptr_array	text_notes_configuration_; // < TODO
    office_element_ptr			text_bibliography_configuration_; // < TODO
    office_element_ptr			text_linenumbering_configuration_; // < TODO

    friend class odf_document;
    
   
};

CP_REGISTER_OFFICE_ELEMENT2(office_styles);

class header_footer_content_impl
{
public:
    common_style_header_footer_attlist common_style_header_footer_attlist_;
    header_footer_content header_footer_content_;
};

class header_footer_impl
{
public:
    header_footer_content_impl & content() { return content_; }
    const header_footer_content_impl & content() const { return content_; }
    virtual ~header_footer_impl() {}
    void docx_convert(oox::docx_conversion_context & Context);
private:
    header_footer_content_impl content_;

};

//         style:header
class style_header : public office_element_impl<style_header>, public header_footer_impl
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleHeader;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    //header_footer_content_impl content_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_header);

///         style:footer
class style_footer : public office_element_impl<style_footer>, public header_footer_impl
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleFooter;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
   // header_footer_content_impl content_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_footer);

///         style:header-first
class style_header_first : public office_element_impl<style_header_first>, public header_footer_impl
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleHeaderFirst;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    //header_footer_content_impl content_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_header_first);

///         style:footer_first
class style_footer_first : public office_element_impl<style_footer_first>, public header_footer_impl
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleFooterFirst;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
   // header_footer_content_impl content_;

};

///         style:header-left
class style_header_left : public office_element_impl<style_header_left>, public header_footer_impl
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleHeaderLeft;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
   // header_footer_content_impl content_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_header_left);

///         style:footer-left
class style_footer_left : public office_element_impl<style_footer_left>, public header_footer_impl
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleFooterLeft;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
   // header_footer_content_impl content_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_footer_left);

/// style_columns
/// style-columns
/// style:columns
class style_columns : public office_element_impl<style_columns>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleColumns;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    _CP_OPT(unsigned int)		fo_column_count_;
	_CP_OPT(odf_types::length)	fo_column_gap_;

    office_element_ptr			style_column_sep_;
    office_element_ptr_array	style_column_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_columns);

/// style_column
/// style-column
/// style:column
class style_column : public office_element_impl<style_column>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleColumn;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

private:
    _CP_OPT(odf_types::length) style_rel_width_;
    odf_types::length fo_start_indent_;
    odf_types::length fo_end_indent_;
    odf_types::length fo_space_before_;
    odf_types::length fo_space_after_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(style_column);

/// style_column_sep
/// style-column-sep
/// style:column-sep
class style_column_sep : public office_element_impl<style_column_sep>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleColumnSep;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
	std::wstring				style_style_; // default solid
    _CP_OPT(odf_types::length)	style_width_;
    odf_types::percent			style_height_; // default 100
    odf_types::vertical_align	style_vertical_align_; //default top
    odf_types::color			style_color_; // default #000000
};

CP_REGISTER_OFFICE_ELEMENT2(style_column_sep);


///         style:section-properties
class style_section_properties : public office_element_impl<style_section_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleSectionProperties;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
	bool style_protect_; // default false

    odf_types::common_background_color_attlist	common_background_color_attlist_;
    odf_types::common_horizontal_margin_attlist common_horizontal_margin_attlist_;
    odf_types::common_writing_mode_attlist		common_writing_mode_attlist_;

    office_element_ptr							style_background_image_;  

    office_element_ptr							style_columns_;
    _CP_OPT(bool)								text_dont_balance_text_columns_;
    
    // 15.7.9
    // TODO text-notes-configuration

};

CP_REGISTER_OFFICE_ELEMENT2(style_section_properties);

///         style-page-layout-attlist
class style_page_layout_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    const std::wstring & get_style_name() const { return style_name_; }

private:
    std::wstring style_name_;
    odf_types::page_usage style_page_usage_; // default All
        
};

/// style:header-style
class style_header_style : public office_element_impl<style_header_style>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleHeaderStyle;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
	office_element_ptr style_header_footer_properties_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_header_style);

/// style:footer-style
class style_footer_style : public office_element_impl<style_header_style>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleFooterStyle;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
	office_element_ptr style_header_footer_properties_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_footer_style);

/// style:page-layout
class style_page_layout : public office_element_impl<style_page_layout>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStylePageLayout;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
   
	style_page_layout_attlist	style_page_layout_attlist_;
    office_element_ptr			style_page_layout_properties_;
    office_element_ptr			style_header_style_;
    office_element_ptr			style_footer_style_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    
};

CP_REGISTER_OFFICE_ELEMENT2(style_page_layout);

// style-page-layout-properties-attlist
class style_page_layout_properties_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

	void docx_convert_serialize(std::wostream & strm, oox::docx_conversion_context & Context, _CP_OPT(odf_types::length_or_percent) margin_left, 
																								_CP_OPT(odf_types::length_or_percent) margin_right);
    void pptx_convert(oox::pptx_conversion_context & Context);

public:
    _CP_OPT(odf_types::length) fo_page_width_;         // +
    _CP_OPT(odf_types::length) fo_page_height_;        // +
    odf_types::common_num_format_attlist common_num_format_attlist_;
    odf_types::common_num_format_prefix_suffix_attlist common_num_format_prefix_suffix_attlist_;
    _CP_OPT(std::wstring) style_paper_tray_name_;
    _CP_OPT(std::wstring) style_print_orientation_; // +
    //15.2.5
    odf_types::common_horizontal_margin_attlist	common_horizontal_margin_attlist_;
    odf_types::common_vertical_margin_attlist	common_vertical_margin_attlist_;
    odf_types::common_margin_attlist			common_margin_attlist_;
    
    odf_types::common_border_attlist			common_border_attlist_;
    odf_types::common_border_line_width_attlist common_border_line_width_attlist_;
    odf_types::common_padding_attlist			common_padding_attlist_;
    odf_types::common_shadow_attlist			common_shadow_attlist_;
    
    // 15.2.10 
    odf_types::common_background_color_attlist common_background_color_attlist_;
    // 15.2.12
    _CP_OPT(odf_types::style_ref) style_register_truth_ref_style_name_;
    // 15.2.13 TODO
    _CP_OPT(std::wstring) style_print_;
        
    // 15.2.14
    _CP_OPT(odf_types::direction) style_print_page_order_;

    // 15.2.15 TODO
    _CP_OPT(std::wstring) style_first_page_number_;

    // 15.2.16
    _CP_OPT(odf_types::percent) style_scale_to_;
    _CP_OPT(unsigned int) style_scale_to_pages_;

    // 15.2.17
    _CP_OPT(odf_types::table_centering) style_table_centering_;
    // 15.2.18
    _CP_OPT(odf_types::length) style_footnote_max_height_;

    // 15.2.19
    odf_types::common_writing_mode_attlist common_writing_mode_attlist_;

    // 15.2.21
    _CP_OPT(odf_types::layout_grid_mode) style_layout_grid_mode_;

    // 15.2.22
    _CP_OPT(odf_types::length) style_layout_grid_base_height_;

    // 15.2.23
    _CP_OPT(odf_types::length) style_layout_grid_ruby_height_;

    // 15.2.24
    _CP_OPT(unsigned int) style_layout_grid_lines_;

    // 15.2.25
    _CP_OPT(odf_types::color) style_layout_grid_color_;

    // 15.2.26
    _CP_OPT(bool) style_layout_grid_ruby_below_;  

    // 15.2.27
    _CP_OPT(bool) style_layout_grid_print_;

    // 15.2.28
    _CP_OPT(bool) style_layout_grid_display_;

};

// style-footnote-sep-attlist
class style_footnote_sep_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

private:
    _CP_OPT(odf_types::length) style_width_;
    _CP_OPT(odf_types::percent) style_rel_width_;
    _CP_OPT(odf_types::color) style_color_;
    _CP_OPT(odf_types::line_style) style_line_style_;
    odf_types::style_type style_adjustment_; // default Left
    _CP_OPT(odf_types::length) style_distance_before_sep_;
    _CP_OPT(odf_types::length) style_distance_after_sep_;
                
};

//          style:footnote-sep
class style_footnote_sep : public office_element_impl<style_footnote_sep>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleFootnoteSep;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

private:
    style_footnote_sep_attlist style_footnote_sep_attlist_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(style_footnote_sep);

// style-page-layout-properties-elements
class style_page_layout_properties_elements
{
public:
    void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name, document_context * Context);

public:
    office_element_ptr	style_background_image_;
    office_element_ptr	style_columns_;
    
    // 15.2.20
    office_element_ptr  style_footnote_sep_;

};

//          style:page-layout-properties
class style_page_layout_properties : public office_element_impl<style_page_layout_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStylePageLayout;
    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert_serialize(std::wostream & strm, oox::docx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context);
    
	bool docx_back_serialize(std::wostream & strm, oox::docx_conversion_context & Context);


public:
    style_page_layout_properties() { }

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    style_page_layout_properties_attlist	style_page_layout_properties_attlist_;
    style_page_layout_properties_elements	style_page_layout_properties_elements_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(style_page_layout_properties);

//          text:notes-configuration
class text_notes_configuration : public office_element_impl<text_notes_configuration>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextNotesConfiguration;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

public:
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

CP_REGISTER_OFFICE_ELEMENT2(text_notes_configuration);

/// style:presentation-page-layout

class style_presentation_page_layout;
typedef boost::shared_ptr<style_presentation_page_layout> style_presentation_page_layout_ptr;

class style_presentation_page_layout : public office_element_impl<style_presentation_page_layout>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStylePresentationPageLayout;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void pptx_convert(oox::pptx_conversion_context & Context);

    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
   
    _CP_OPT(std::wstring) style_name_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

	office_element_ptr_array content_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(style_presentation_page_layout);
} // namespace odf_reader
} // namespace cpdoccore
