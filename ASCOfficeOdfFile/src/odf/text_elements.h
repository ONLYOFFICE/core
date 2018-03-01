/*
 * (c) Copyright Ascensio System SIA 2010-2018
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

#include "text_content.h"
#include "office_elements_create.h"

#include "datatypes/textdisplay.h"

#include "datatypes/common_attlists.h"

namespace cpdoccore { 
namespace odf_reader {
namespace text {

class paragraph
{
public:
    paragraph() : next_par_(NULL), next_section_(false), next_end_section_(false), is_header_(false) {}

    std::wostream & text_to_stream(std::wostream & _Wostream) const;
   
	void add_attributes		( const xml::attributes_wc_ptr & Attributes );
    void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context);
    void add_text			(const std::wstring & Text);

    paragraph * get_next()					{ return next_par_; }
    void		set_next(paragraph * next)	{next_par_ = next;}
    
	void set_next_section(bool Val) 
    {
        next_section_ = Val;
    }

    void set_next_end_section(bool Val) 
    {
        next_end_section_ = Val;
    }

    void afterCreate(document_context * ctx);
 	void afterReadContent(document_context * ctx);
  
	void docx_convert (oox::docx_conversion_context & Context) ;
    void xlsx_convert (oox::xlsx_conversion_context & Context) ;
    void pptx_convert (oox::pptx_conversion_context & Context) ;

	size_t drop_cap_docx_convert(oox::docx_conversion_context & Context);

    office_element_ptr_array	content_;
private:
	void drop_cap_text_docx_convert(office_element_ptr first_text_paragraph,oox::docx_conversion_context & Context);
   
	paragraph_attrs			attrs_;
   
	paragraph				*next_par_;
    
	bool					next_section_;
    bool					next_end_section_;
	
	bool					is_header_;

    friend class par_docx_convert_class;   
    friend class p;   
	friend class h;   
   
};

//---------------------------------------------------------------------------------------------------
class h : public text_content_impl<h>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextH;
    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context) ;
    void xlsx_convert(oox::xlsx_conversion_context & Context) ;
    void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual void afterCreate();
    virtual void afterReadContent();
    
	virtual std::wostream & text_to_stream(std::wostream & _Wostream) const;

    paragraph paragraph_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

    // heading-attrs
    _CP_OPT(unsigned int)	text_outline_level_;
    _CP_OPT(bool)			text_restart_numbering_;
    _CP_OPT(unsigned int)	text_start_value_;
    _CP_OPT(bool)			text_is_list_header_;
    _CP_OPT(std::wstring)	text_number_;
    
    friend class par_docx_convert_class;

};

CP_REGISTER_OFFICE_ELEMENT2(h);

//---------------------------------------------------------------------------------------------------
class p : public text_content_impl<p>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextP;
    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context) ;
    void xlsx_convert(oox::xlsx_conversion_context & Context) ;
	void pptx_convert(oox::pptx_conversion_context & Context) ;

	virtual void afterCreate();
	virtual void afterReadContent();
    
	virtual std::wostream & text_to_stream(std::wostream & _Wostream) const;

    p(){};
	paragraph paragraph_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

	friend class par_docx_convert_class;   
};
CP_REGISTER_OFFICE_ELEMENT2(p);

//---------------------------------------------------------------------------------------------------
class list : public text_content_impl<list>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextList;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual std::wostream & text_to_stream(std::wostream & _Wostream) const;

    list(){};

    void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

    std::wstring				text_style_name_;
    _CP_OPT(bool)				text_continue_numbering_;

    office_element_ptr          text_list_header_;
    office_element_ptr_array    text_list_items_;

};
CP_REGISTER_OFFICE_ELEMENT2(list);

//---------------------------------------------------------------------------------------------------
class soft_page_break : public text_content_impl<soft_page_break>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSoftPageBreak;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual std::wostream & text_to_stream(std::wostream & _Wostream) const;
    void docx_convert(oox::docx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

};
CP_REGISTER_OFFICE_ELEMENT2(soft_page_break);

//---------------------------------------------------------------------------------------------------

class text_section_attr
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

    _CP_OPT(std::wstring)			text_style_name_;
    std::wstring					text_name_;
    _CP_OPT(bool)					text_protected_;
    _CP_OPT(std::wstring)			text_protection_key_;
    _CP_OPT(odf_types::text_display) text_display_;
    _CP_OPT(std::wstring)			text_condition_;

};

class text_section : public text_content_impl<text_section>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSection;
    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context);

    virtual std::wostream & text_to_stream(std::wostream & _Wostream) const;
    
	virtual void afterCreate();
    virtual void afterReadContent();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    text_section_attr			text_section_attr_;
    office_element_ptr			text_section_source_;
    office_element_ptr_array	text_content_;          

};

CP_REGISTER_OFFICE_ELEMENT2(text_section);

class text_section_source_attr
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

private:
    odf_types::common_xlink_attlist	common_xlink_attlist_;

    _CP_OPT(std::wstring)			text_section_name_;
    _CP_OPT(std::wstring)			text_filter_name_;
    
};

//---------------------------------------------------------------------------------------------------
class text_section_source : public text_content_impl<text_section_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSectionSource;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    text_section_source_attr text_section_source_attr_;
    
};
CP_REGISTER_OFFICE_ELEMENT2(text_section_source);

//---------------------------------------------------------------------------------------------------
class text_table_of_content : public text_content_impl<text_table_of_content>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextTableOfContent;
    
	CPDOCCORE_DEFINE_VISITABLE();
    
	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    text_section_attr	text_section_attr_;
    office_element_ptr	text_table_of_content_source_;
    office_element_ptr	text_index_body_;

};
CP_REGISTER_OFFICE_ELEMENT2(text_table_of_content);

//---------------------------------------------------------------------------------------------------
class text_table_index : public text_content_impl<text_table_index>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextTableIndex;
    
	CPDOCCORE_DEFINE_VISITABLE();
    
	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    text_section_attr	text_section_attr_;
    office_element_ptr	text_table_index_source_;
    office_element_ptr	text_index_body_;

};
CP_REGISTER_OFFICE_ELEMENT2(text_table_index);

//---------------------------------------------------------------------------------------------------
class text_illustration_index : public text_content_impl<text_illustration_index>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextIllustrationIndex;
    
	CPDOCCORE_DEFINE_VISITABLE();
  
	virtual void afterCreate();
    virtual void afterReadContent();
    
	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    text_section_attr	text_section_attr_;
    office_element_ptr	text_illustration_index_source_;
    office_element_ptr	text_index_body_;

};
CP_REGISTER_OFFICE_ELEMENT2(text_illustration_index);
//---------------------------------------------------------------------------------------------------
class text_alphabetical_index : public text_content_impl<text_alphabetical_index>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextAlphabeticalIndex;
    
	CPDOCCORE_DEFINE_VISITABLE();
    
    virtual void afterCreate();
    virtual void afterReadContent();

	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    text_section_attr	text_section_attr_;
    office_element_ptr	text_alphabetical_index_source_;
    office_element_ptr	text_index_body_;

};
CP_REGISTER_OFFICE_ELEMENT2(text_alphabetical_index);
//------------------------------------------------------------------------------------------------------------
class text_bibliography : public text_content_impl<text_bibliography>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextBibliography;
    
	CPDOCCORE_DEFINE_VISITABLE();
    
    virtual void afterCreate();
    virtual void afterReadContent();

	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    text_section_attr	text_section_attr_;
    office_element_ptr	text_bibliography_source_;
    office_element_ptr	text_index_body_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_bibliography);
//---------------------------------------------------------------------------------------------------
//typeTextTableIndex,
//typeTextObjectIndex,
//typeTextUserIndex,

//---------------------------------------------------------------------------------------------------
//text:bibliography-mark

class text_bibliography_attr
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

    std::wstring			text_identifier_;
    std::wstring			text_bibliography_type_; // todoooo in datatype
    
	_CP_OPT(std::wstring)	text_url_;
    _CP_OPT(std::wstring)	text_author_;
    _CP_OPT(std::wstring)	text_title_;
    _CP_OPT(std::wstring)	text_year_;
    _CP_OPT(std::wstring)	text_isbn_;
	_CP_OPT(std::wstring)	text_chapter_;
	//todooo to map or list
};

class text_bibliography_mark : public text_content_impl<text_bibliography_mark>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextBibliographyMark;
    
	CPDOCCORE_DEFINE_VISITABLE();

	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream) const;

    text_bibliography_attr	text_bibliography_attr_;
    office_element_ptr		content_;

private:
    virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text			(const std::wstring & Text);
};
CP_REGISTER_OFFICE_ELEMENT2(text_bibliography_mark);
//---------------------------------------------------------------------------------------------------
class text_index_body : public text_content_impl<text_index_body>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextIndexBody;
    CPDOCCORE_DEFINE_VISITABLE();
    
	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream) const;

    office_element_ptr_array index_content_main_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
};
CP_REGISTER_OFFICE_ELEMENT2(text_index_body);

//---------------------------------------------------------------------------------------------------
class text_index_title : public text_content_impl<text_index_title>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextIndexTitle;
    CPDOCCORE_DEFINE_VISITABLE();
  	
	void pptx_convert(oox::pptx_conversion_context & Context) ;
	void docx_convert(oox::docx_conversion_context & Context);

    virtual std::wostream & text_to_stream(std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    text_section_attr			text_section_attr_;
    office_element_ptr_array	index_content_main_;
    
};
CP_REGISTER_OFFICE_ELEMENT2(text_index_title);

//---------------------------------------------------------------------------------------------------
class text_unknown_base_change : public office_element_impl<text_unknown_base_change>
{
public:
	static const wchar_t * ns;
    static const wchar_t * name;   	
	static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeNone;

    CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context);

    office_element_ptr			office_change_info_;
    office_element_ptr_array	content_;

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

};

//---------------------------------------------------------------------------------------------------
class text_unknown_change : public text_unknown_base_change
{
public:
	static const wchar_t * ns;
    static const wchar_t * name;   	
	static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextUnknownChange;
    
	virtual void docx_convert(oox::docx_conversion_context & Context);
};
CP_REGISTER_OFFICE_ELEMENT2(text_unknown_change);

//---------------------------------------------------------------------------------------------------
class text_insertion : public text_unknown_base_change
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextInsertion;

    virtual void docx_convert(oox::docx_conversion_context & Context);
};

CP_REGISTER_OFFICE_ELEMENT2(text_insertion);

//---------------------------------------------------------------------------------------------------
class text_deletion : public text_unknown_base_change
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextDeletion;

	virtual void docx_convert(oox::docx_conversion_context & Context);
};

CP_REGISTER_OFFICE_ELEMENT2(text_deletion);

//---------------------------------------------------------------------------------------------------
class text_format_change : public text_unknown_base_change
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextFormatChange;

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void docx_convert(oox::docx_conversion_context & Context);
	
	_CP_OPT(std::wstring)	text_style_name_; //не по снецификации ... но КАК сохранить то что было изменено в формате?????
};
CP_REGISTER_OFFICE_ELEMENT2(text_format_change);

//---------------------------------------------------------------------------------------------------
class text_tracked_changes : public office_element_impl<text_tracked_changes>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextTrackedChanges;
    CPDOCCORE_DEFINE_VISITABLE()
    
    void docx_convert(oox::docx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

	_CP_OPT(odf_types::Bool)	text_track_changes_;
    office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_tracked_changes);

//---------------------------------------------------------------------------------------------------
class text_changed_region : public office_element_impl<text_changed_region>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextChangedRegion;
    CPDOCCORE_DEFINE_VISITABLE()
    
    void docx_convert(oox::docx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

	_CP_OPT(std::wstring)		text_id_; //== xml:id
    office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_changed_region);

//---------------------------------------------------------------------------------------------------
class text_add_change : public office_element_impl<text_add_change>
{
public:
 	static const wchar_t * ns;
    static const wchar_t * name;   	

	static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextChange;
   
	CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context) = 0;

	_CP_OPT(std::wstring)	text_change_id_;    
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

};

//---------------------------------------------------------------------------------------------------
class text_change : public text_add_change
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextChange;  
   
	virtual void docx_convert(oox::docx_conversion_context & Context);
};
CP_REGISTER_OFFICE_ELEMENT2(text_change);

//---------------------------------------------------------------------------------------------------
class text_change_start : public text_add_change
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextChangeStart;
	
	virtual void docx_convert(oox::docx_conversion_context & Context);
};
CP_REGISTER_OFFICE_ELEMENT2(text_change_start);

//---------------------------------------------------------------------------------------------------
class text_change_end : public text_add_change
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextChangeEnd;

	virtual void docx_convert(oox::docx_conversion_context & Context);
};
CP_REGISTER_OFFICE_ELEMENT2(text_change_end);
//-------------------------------------------------------------------------------------------------------------------
//text:variable-input
//---------------------------------------------------------------------------------------------------
class text_variable_input : public office_element_impl<text_variable_input>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextVariableInput;
    CPDOCCORE_DEFINE_VISITABLE()
    
    void docx_convert(oox::docx_conversion_context & Context);

    virtual std::wostream & text_to_stream(std::wostream & _Wostream) const;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(odf_types::office_value_type)	office_value_type_;
	_CP_OPT(std::wstring)	style_data_style_name_;
	_CP_OPT(std::wstring)	text_description_;
	_CP_OPT(std::wstring)	text_display_;
	_CP_OPT(std::wstring)	text_name_;

    std::wstring			text_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_variable_input);
//-------------------------------------------------------------------------------------------------------------------
//text:variable-get
//---------------------------------------------------------------------------------------------------
class text_variable_get : public office_element_impl<text_variable_get>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextVariableGet;
    CPDOCCORE_DEFINE_VISITABLE()
    
    void docx_convert(oox::docx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

	_CP_OPT(std::wstring)	style_data_style_name_;
	_CP_OPT(std::wstring)	text_display_;
	_CP_OPT(std::wstring)	text_name_;

};
CP_REGISTER_OFFICE_ELEMENT2(text_variable_get);
//-------------------------------------------------------------------------------------------------------------------
//text:variable-set
//---------------------------------------------------------------------------------------------------
class text_variable_set : public office_element_impl<text_variable_set>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextVariableSet;
    CPDOCCORE_DEFINE_VISITABLE()
    
    void docx_convert(oox::docx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

	_CP_OPT(odf_types::office_value_type)	office_value_type_;

	_CP_OPT(std::wstring)	office_boolean_value_;
	_CP_OPT(std::wstring)	office_date_value_;
	_CP_OPT(std::wstring)	office_time_value_;
	_CP_OPT(std::wstring)	office_string_value_;
	_CP_OPT(std::wstring)	office_value_;
	_CP_OPT(std::wstring)	office_currency_;
	_CP_OPT(std::wstring)	office_formula_;
	_CP_OPT(std::wstring)	style_data_style_name_;
	_CP_OPT(std::wstring)	text_display_;
	_CP_OPT(std::wstring)	text_name_;

};
CP_REGISTER_OFFICE_ELEMENT2(text_variable_set);
//---------------------------------------------------------------------------------------------------
//text:variable-decl
//---------------------------------------------------------------------------------------------------
class text_variable_decl : public office_element_impl<text_variable_decl>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextVariableDecl;
    CPDOCCORE_DEFINE_VISITABLE()
    
    void docx_convert(oox::docx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

	_CP_OPT(odf_types::office_value_type)	office_value_type_;
	_CP_OPT(std::wstring)	text_display_;
	_CP_OPT(std::wstring)	text_name_;

};
CP_REGISTER_OFFICE_ELEMENT2(text_variable_decl);
//---------------------------------------------------------------------------------------------------
//text:variable-decls
//---------------------------------------------------------------------------------------------------
class text_variable_decls : public office_element_impl<text_variable_decls>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextVariableDecls;
    CPDOCCORE_DEFINE_VISITABLE()
    
    void docx_convert(oox::docx_conversion_context & Context);

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_variable_decls);
}
}
}
