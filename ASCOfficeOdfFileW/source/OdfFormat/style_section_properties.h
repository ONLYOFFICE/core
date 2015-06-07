#pragma once

#include <iosfwd>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements_create.h"

#include "common_attlists.h"
#include "verticalalign.h"

namespace cpdoccore { 
namespace odf {

/// style:columns
class style_columns : public office_element_impl<style_columns>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleColumns;
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element(office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    _CP_OPT(unsigned int) fo_column_count_;
    _CP_OPT(length) fo_column_gap_;

    office_element_ptr style_column_sep_;
    office_element_ptr_array style_column_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_columns);

///		style:column
class style_column : public office_element_impl<style_column>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleColumn;
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element(office_element_ptr & child){}
	virtual void serialize(std::wostream & strm);

    _CP_OPT(length) style_rel_width_;
	_CP_OPT(length) fo_start_indent_;
    _CP_OPT(length) fo_end_indent_;
    _CP_OPT(length) fo_space_before_;
    _CP_OPT(length) fo_space_after_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(style_column);

/// style:column-sep
class style_column_sep : public office_element_impl<style_column_sep>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleColumnSep;
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element(office_element_ptr & child){}
	
	virtual void serialize(std::wostream & strm);

    _CP_OPT(std::wstring)	style_style_;			 // default solid
    _CP_OPT(length)			style_width_;
	_CP_OPT(percent)		style_height_;			// default 100
    _CP_OPT(vertical_align) style_vertical_align_;	// default top
    _CP_OPT(color)			style_color_;			// default #000000
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
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    common_background_color_attlist		common_background_color_attlist_;
    common_horizontal_margin_attlist	common_horizontal_margin_attlist_;
    common_writing_mode_attlist			common_writing_mode_attlist_;

    _CP_OPT(Bool) style_protect_; // default false
    _CP_OPT(Bool) text_dont_balance_text_columns_;
 
    office_element_ptr style_columns_;
    office_element_ptr style_background_image_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_section_properties);
}
}