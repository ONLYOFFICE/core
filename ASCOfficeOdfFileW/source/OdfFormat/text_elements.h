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

#include <iosfwd>

#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements_create.h"
#include "common_attlists.h"

#include "style_ref.h"
#include "textdisplay.h"

namespace cpdoccore { 
namespace odf_writer {

class odf_text_context;

class paragraph_attrs
{
public:

    _CP_OPT(odf_types::style_ref)		text_style_name_;
    _CP_OPT(odf_types::style_ref)		text_cond_style_name_;
    odf_types::style_ref_array			text_class_names_;

	void serialize(CP_ATTR_NODE);   

};

class paragraph
{
public:
    paragraph() {}

public:

    void add_text(const std::wstring & Text);
   
	void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name,odf_conversion_context * context);
    void add_child_element( const office_element_ptr & child_element);

	void serialize		(std::wostream & _Wostream);
	void serialize_attr	(CP_ATTR_NODE);

    paragraph_attrs paragraph_attrs_;

    office_element_ptr_array paragraph_content_;
   
};

// text:h
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_h : public office_element_impl<text_h>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextH;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    text_h();

    virtual void add_text(const std::wstring & Text);

    // heading-attrs
    unsigned int text_outline_level_;
    bool text_restart_numbering_; // default false
    _CP_OPT(unsigned int) text_start_value_;
    bool text_is_list_header_; // default false
    _CP_OPT(std::wstring) text_number_;
    
    paragraph paragraph_;


};

CP_REGISTER_OFFICE_ELEMENT2(text_h);

// text:p
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_p : public office_element_impl<text_p>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextP;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	text_p(){};
	paragraph paragraph_;

    virtual void add_text(const std::wstring & Text);
};

CP_REGISTER_OFFICE_ELEMENT2(text_p);

// text:list
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_list : public office_element_impl<text_list>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextList;
    CPDOCCORE_DEFINE_VISITABLE()

public:

    text_list(){};

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(odf_types::style_ref)			text_style_name_;
    _CP_OPT(odf_types::Bool)				text_continue_numbering_;

    office_element_ptr          text_list_header_;
    office_element_ptr_array    text_list_items_;

};

CP_REGISTER_OFFICE_ELEMENT2(text_list);

// text:soft-page-break
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_soft_page_break : public office_element_impl<text_soft_page_break>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSoftPageBreak;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);
};

CP_REGISTER_OFFICE_ELEMENT2(text_soft_page_break);

class text_section_attr
{
public:
    _CP_OPT(odf_types::style_ref)	text_style_name_;
    std::wstring					text_name_;
    _CP_OPT(bool)					text_protected_;
    _CP_OPT(std::wstring)			text_protection_key_;
    _CP_OPT(odf_types::text_display)			text_display_;
    _CP_OPT(std::wstring)			text_condition_;
	
	void serialize(CP_ATTR_NODE);   
};

// text:section
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_section : public office_element_impl<text_section>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSection;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    text_section_attr text_section_attr_;
    
	office_element_ptr			text_section_source_;
    office_element_ptr_array	text_content_;          

};

CP_REGISTER_OFFICE_ELEMENT2(text_section);

// text-section-source-attr
class text_section_source_attr
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

    odf_types::common_xlink_attlist	common_xlink_attlist_;

    _CP_OPT(std::wstring)	text_section_name_;
    _CP_OPT(std::wstring)	text_filter_name_;
    
	void serialize(CP_ATTR_NODE);   
};

// text:section-source
// text-section-source
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_section_source : public office_element_impl<text_section_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSectionSource;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    text_section_source_attr text_section_source_attr_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_section_source);

// text:table-of-content
// text-table-of-content
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_table_of_content : public office_element_impl<text_table_of_content>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextTableOfContent;
    
	CPDOCCORE_DEFINE_VISITABLE()
    
	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    text_section_attr text_section_attr_;
    
	office_element_ptr text_table_of_content_source_;
    office_element_ptr text_index_body_;

};

CP_REGISTER_OFFICE_ELEMENT2(text_table_of_content);

// text:index-body
// text-index-body
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_index_body : public office_element_impl<text_index_body>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextIndexBody;
    CPDOCCORE_DEFINE_VISITABLE()
    
	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array index_content_main_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_index_body);

// text:index-title
// text-index-title
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_index_title : public office_element_impl<text_index_title>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextIndexTitle;
    CPDOCCORE_DEFINE_VISITABLE()
  	
	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	text_section_attr text_section_attr_;
    office_element_ptr_array index_content_main_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_index_title);


}
}
