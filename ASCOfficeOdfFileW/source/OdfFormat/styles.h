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

#include "stylefamily.h"
#include "textalign.h"
#include "length.h"
#include "styleverticalrel.h"
#include "styleverticalpos.h"
#include "percent.h"
#include "verticalalign.h"
#include "pageusage.h"
#include "tablecentering.h"
#include "layoutgridmode.h"
#include "direction.h"
#include "linestyle.h"
#include "styletype.h"
#include "noteclass.h"
#include "gradientstyle.h"
#include "hatchstyle.h"

#include "common_attlists.h"

#include "header_footer.h"

#include "styles_list.h"
#include "style_map.h"


namespace cpdoccore { 
namespace odf { 

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
	style_content(odf_conversion_context * _context){Context = _context;}

	void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    void add_child_element(office_element_ptr & child);
	void serialize(std::wostream & strm);
  
    style_text_properties *			get_style_text_properties();
    style_paragraph_properties *	get_style_paragraph_properties() ;
    style_graphic_properties *		get_style_graphic_properties() ;
    style_table_properties *		get_style_table_properties();
    style_section_properties *		get_style_section_properties();
    style_table_cell_properties *	get_style_table_cell_properties();
    style_table_row_properties *	get_style_table_row_properties() ;
    style_table_column_properties * get_style_table_column_properties() ;
    style_chart_properties *		get_style_chart_properties() ;
	//style_drawing_page_properties*	get_style_drawing_page_properties();

private:
	odf_conversion_context * Context;
    style_family style_family_;

    office_element_ptr style_text_properties_;
    office_element_ptr style_paragraph_properties_;
    office_element_ptr style_section_properties_;
    office_element_ptr style_ruby_properties_;
    office_element_ptr style_table_properties_;
    office_element_ptr style_table_column_properties_;
    office_element_ptr style_table_row_properties_;
    office_element_ptr style_chart_properties_;
    office_element_ptr style_graphic_properties_;
    office_element_ptr style_table_cell_properties_;
	office_element_ptr style_drawing_page_properties_;

};

/// \class  default_style
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
	default_style() : style_content_(getContext()) {}

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    style_family style_family_;

    style_content style_content_;

};

CP_REGISTER_OFFICE_ELEMENT2(default_style);


/////////////////////////////////////////////////////////////////////////////////////////////////
/// \class  style_draw_gradient
class draw_gradient : public office_element_impl<draw_gradient>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleDrawGradient;

    CPDOCCORE_DEFINE_VISITABLE();

	std::wstring				get_style_name(){return draw_name_.get_value_or(L"");}

	_CP_OPT(color)				draw_start_color_;
	_CP_OPT(color)				draw_end_color_;

	_CP_OPT(percent)			draw_end_intensity_; 
	_CP_OPT(percent)			draw_start_intensity_;

	_CP_OPT(percent)			draw_cy_;//%
	_CP_OPT(percent)			draw_cx_;
	
	_CP_OPT(percent)			draw_border_;
	_CP_OPT(int)				draw_angle_;
	_CP_OPT(gradient_style)		draw_style_;

 	_CP_OPT(std::wstring)		draw_name_;
	_CP_OPT(std::wstring)		draw_display_name_;
	
    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child)
		{std::wstringstream str; str <<L"Non add child in "; str << ns; str << L":"; str <<name; _CP_LOG(error) << str.str();}
 	virtual void serialize(std::wostream & strm);

};
CP_REGISTER_OFFICE_ELEMENT2(draw_gradient);
/////////////////////////////////////////////////////////////////////////////////////////////////
/// \class  style_draw_hatch
class draw_hatch : public office_element_impl<draw_hatch>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleDrawHatch;

    CPDOCCORE_DEFINE_VISITABLE();

	std::wstring	get_style_name(){return draw_name_.get_value_or(L"");}
	
	_CP_OPT(hatch_style)	draw_style_;
	_CP_OPT(int)			draw_rotation_;
	_CP_OPT(length)			draw_distance_;	
	_CP_OPT(color)			draw_color_;

 	_CP_OPT(std::wstring)	draw_name_;
	_CP_OPT(std::wstring)	draw_display_name_;

    virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child)
		{std::wstringstream str; str <<L"Non add child in "; str << ns; str << L":"; str <<name; _CP_LOG(error) << str.str();}
	virtual void serialize(std::wostream & strm);
 
};
CP_REGISTER_OFFICE_ELEMENT2(draw_hatch);
/////////////////////////////////////////////////////////////////////////////////////////////////
/// \class  style_draw_gradient
class draw_opacity : public office_element_impl<draw_opacity>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleDrawOpacity;

    CPDOCCORE_DEFINE_VISITABLE();

	std::wstring	get_style_name(){return draw_name_.get_value_or(L"");}

	_CP_OPT(gradient_style)	draw_style_;//linear, radial, ..
	_CP_OPT(int)			draw_angle_;

	_CP_OPT(percent)		draw_start_;
	_CP_OPT(percent)		draw_end_;
	
	_CP_OPT(percent)		draw_border_;

 	_CP_OPT(std::wstring)	draw_name_;
	_CP_OPT(std::wstring)	draw_display_name_;
	
    virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
     virtual void add_child_element(office_element_ptr & child)
		{std::wstringstream str; str <<L"Non add child in "; str << ns; str << L":"; str <<name; _CP_LOG(error) << str.str();}
	virtual void serialize(std::wostream & strm);

};
CP_REGISTER_OFFICE_ELEMENT2(draw_opacity);
/// \class  style_draw_fill_image
class draw_fill_image : public office_element_impl<draw_fill_image>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleDrawFillImage;

    CPDOCCORE_DEFINE_VISITABLE();

	std::wstring get_style_name(){return draw_name_.get_value_or(L"");}
	
	common_xlink_attlist		xlink_attlist_;

 	_CP_OPT(std::wstring)		draw_name_;
	_CP_OPT(std::wstring)		draw_display_name_;

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child)
		{std::wstringstream str; str <<L"Non add child in "; str << ns; str << L":"; str <<name; _CP_LOG(error) << str.str();}
	virtual void serialize(std::wostream & strm);


};
CP_REGISTER_OFFICE_ELEMENT2(draw_fill_image);
/////////////////////////////////////////////////////////////////////////////////////////////////
class style;
typedef shared_ptr<style>::Type style_ptr;
typedef weak_ptr<style>::Type style_weak_ptr;
typedef ::std::vector< style_weak_ptr > style_weak_ptr_array;
typedef ::std::vector< style_ptr > style_ptr_array;

///         style:style
class style : public office_element_impl<style>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleStyle;
    CPDOCCORE_DEFINE_VISITABLE();
 
    style() : style_content_(getContext()) {} 
    
	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    // attr
    std::wstring			style_name_;
    _CP_OPT( std::wstring ) style_display_name_; 
    style_family			style_family_;

    _CP_OPT( std::wstring ) style_parent_style_name_; 
    _CP_OPT( std::wstring ) style_next_style_name_; 
    _CP_OPT( std::wstring ) style_list_style_name_; //  may be empty
    _CP_OPT( std::wstring ) style_master_page_name_;
	_CP_OPT( bool)			style_auto_update_; // default = false
    _CP_OPT( std::wstring ) style_data_style_name_; 
    _CP_OPT( std::wstring ) style_class_;
    _CP_OPT(std::wstring)	style_default_outline_level_; 


    style_content				style_content_;
    office_element_ptr_array	style_map_;

};

CP_REGISTER_OFFICE_ELEMENT2(style);

/// \class  styles
class styles
{
public:
    void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context);
	void add_child_element(office_element_ptr & child, odf_conversion_context * Context);

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
    void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context);
	void add_child_element(office_element_ptr & child, odf_conversion_context * Context);

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
    void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context);
    void add_child_element(office_element_ptr & child);

	void serialize(std::wostream & strm);

	office_element_ptr_array table_templates_;    

};

/// \class  office_automatic_styles
/// \brief  office:automatic-styles
class office_automatic_styles : public office_element_impl<office_automatic_styles>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeAutomaticStyles;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    styles styles_; ///< styles
    office_element_ptr_array style_page_layout_; 
};

CP_REGISTER_OFFICE_ELEMENT2(office_automatic_styles);

/// \class  office_master_styles
/// \brief  office:master-styles
class office_master_styles : public office_element_impl<office_master_styles>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeMasterStyles;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    office_element_ptr_array style_master_page_;	// разметки тем
    office_element_ptr style_handout_master_;		// разметки для принтера - .. второстепенно
    office_element_ptr draw_layer_set_;				// необязательно .. так как слои все равно не поддерживаются в мс.
													// то есть не будут объекты объеденены по признаку слоя
													// зы. не путать с обычной группировкой

};

CP_REGISTER_OFFICE_ELEMENT2(office_master_styles);

/// \class class style_master_page_attlist
/// \brief style-master-page-attlist
class style_master_page_attlist
{
public:
	void serialize(CP_ATTR_NODE);

    _CP_OPT(style_ref)		style_name_;
    _CP_OPT(std::wstring)	style_display_name_;
    _CP_OPT(style_ref)		style_page_layout_name_;
    
	_CP_OPT(std::wstring)	draw_style_name_;
    _CP_OPT(style_ref)		style_next_style_name_;
};

/// \class  style_master_page
/// \brief  style:master-page
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

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

	int find_placeHolderIndex(presentation_class::type placeHolder,int & last_idx);
   
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

/// \class  office_styles
/// \brief  office:styles
class office_styles : public office_element_impl<office_styles>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeStyles;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    styles		styles_;
	draw_styles draw_styles_;	
	templates	templates_;	
	
	office_element_ptr_array style_default_style_;
	office_element_ptr_array style_presentation_page_layout_;
    
	office_element_ptr			text_outline_style_; // < TODO
    office_element_ptr_array	text_notes_configuration_; // < TODO
    office_element_ptr			text_bibliography_configuration_; // < TODO
    office_element_ptr			text_linenumbering_configuration_; // < TODO

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

private:
    header_footer_content_impl content_;

};

///         style:header
class style_header : public office_element_impl<style_header>, public header_footer_impl
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleHeader;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    header_footer_content_impl content_;
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

	virtual void serialize(std::wostream & strm);

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child);

    header_footer_content_impl content_;

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

	virtual void serialize(std::wostream & strm);

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child);

    header_footer_content_impl content_;
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

    virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    header_footer_content_impl content_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_footer_first);

///         style:header-left
class style_header_left : public office_element_impl<style_header_left>, public header_footer_impl
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleHeaderLeft;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child);

	virtual void serialize(std::wostream & strm);
  
	header_footer_content_impl content_;
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

    virtual void create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

	header_footer_content_impl content_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_footer_left);

///         style:page-layout-attlist
class style_page_layout_attlist
{
public:
	void serialize(CP_ATTR_NODE);

	_CP_OPT(std::wstring)	style_name_;
    _CP_OPT(page_usage)		style_page_usage_; // default All
        
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

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element(office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

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

    virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

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
   
	style_page_layout_attlist style_page_layout_attlist_;
   
	office_element_ptr style_page_layout_properties_;
    office_element_ptr style_header_style_;
    office_element_ptr style_footer_style_;

    virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child);

	virtual void serialize(std::wostream & strm);  
};

CP_REGISTER_OFFICE_ELEMENT2(style_page_layout);

// style:footnote-sep-attlist
class style_footnote_sep_attlist
{
public:
	void serialize(CP_ATTR_NODE);

    _CP_OPT(length) style_width_;
    _CP_OPT(percent) style_rel_width_;
    _CP_OPT(color) style_color_;
    _CP_OPT(line_style) style_line_style_;
   _CP_OPT(style_type) style_adjustment_; // default Left
    _CP_OPT(length) style_distance_before_sep_;
    _CP_OPT(length) style_distance_after_sep_;
                
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

    virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child)
		{std::wstringstream str; str <<L"Non add child in "; str << ns; str << L":"; str <<name; _CP_LOG(error) << str.str();}
	virtual void serialize(std::wostream & strm);

    style_footnote_sep_attlist style_footnote_sep_attlist_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(style_footnote_sep);


//          text:notes-configuration
class text_notes_configuration : public office_element_impl<text_notes_configuration>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextNotesConfiguration;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child)
		{std::wstringstream str; str <<L"Non add child in "; str << ns; str << L":"; str <<name; _CP_LOG(error) << str.str();}
	virtual void serialize(std::wostream & strm){}

    noteclass noteclass_;
    _CP_OPT(std::wstring) text_citation_style_name_;
    _CP_OPT(std::wstring) text_citation_body_style_name_;
    _CP_OPT(std::wstring) text_default_style_name_;
    _CP_OPT(std::wstring) text_master_page_name_;
    _CP_OPT(unsigned int) text_start_value_;
    common_num_format_prefix_suffix_attlist common_num_format_prefix_suffix_attlist_;
    common_num_format_attlist common_num_format_attlist_;
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

	virtual void serialize(std::wostream & strm){}
    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child)
		{std::wstringstream str; str <<L"Non add child in "; str << ns; str << L":"; str <<name; _CP_LOG(error) << str.str();}

	_CP_OPT(std::wstring) style_name_;

	office_element_ptr_array content_;
    };

CP_REGISTER_OFFICE_ELEMENT2(style_presentation_page_layout);
} // namespace odf
} // namespace cpdoccore
