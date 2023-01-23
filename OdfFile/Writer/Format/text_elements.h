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

#include <iosfwd>

#include <CPOptional.h>
#include <xml/xmlelement.h>
#include <xml/nodetype.h>

#include "office_elements_create.h"
#include "../../DataTypes/common_attlists.h"

#include "../../DataTypes/bool.h"
#include "../../DataTypes/textdisplay.h"

namespace cpdoccore { 
namespace odf_writer {

class odf_text_context;

class paragraph_attrs
{
public:

    _CP_OPT(std::wstring)		text_style_name_;
    _CP_OPT(std::wstring)		text_cond_style_name_;
	std::vector<std::wstring>	text_class_names_;

	void serialize(CP_ATTR_NODE);   

};
//---------------------------------------------------------------------------------------------------
class text_h : public office_element_impl<text_h>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextH;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    text_h();

    virtual void add_text(const std::wstring & Text);

    unsigned int text_outline_level_;
    bool text_restart_numbering_; // default false
    _CP_OPT(unsigned int) text_start_value_;
    bool text_is_list_header_; // default false
    _CP_OPT(std::wstring) text_number_;
    
	paragraph_attrs paragraph_attrs_;
	office_element_ptr_array paragraph_content_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_h);

//---------------------------------------------------------------------------------------------------
class text_p : public office_element_impl<text_p>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextP;    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	text_p(){};
	
	paragraph_attrs paragraph_attrs_;
	office_element_ptr_array paragraph_content_;

    virtual void add_text(const std::wstring & Text);
};
CP_REGISTER_OFFICE_ELEMENT2(text_p);
//---------------------------------------------------------------------------------------------------
class text_list : public office_element_impl<text_list>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextList;
    

public:

    text_list(){};

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)			text_style_name_;
    _CP_OPT(odf_types::Bool)				text_continue_numbering_;

    office_element_ptr          text_list_header_;
    office_element_ptr_array    text_list_items_;

};

CP_REGISTER_OFFICE_ELEMENT2(text_list);

//---------------------------------------------------------------------------------------------------
class text_soft_page_break : public office_element_impl<text_soft_page_break>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextSoftPageBreak;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);
};

CP_REGISTER_OFFICE_ELEMENT2(text_soft_page_break);

class text_section_attr
{
public:
    _CP_OPT(std::wstring)			text_style_name_;
    std::wstring					text_name_;
    _CP_OPT(bool)					text_protected_;
    _CP_OPT(std::wstring)			text_protection_key_;
    _CP_OPT(odf_types::text_display)text_display_;
    _CP_OPT(std::wstring)			text_condition_;
	
	void serialize(CP_ATTR_NODE);   
};

//---------------------------------------------------------------------------------------------------
class text_section : public office_element_impl<text_section>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextSection;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    text_section_attr text_section_attr_;
    
	office_element_ptr			text_section_source_;
    office_element_ptr_array	content_;          

};

CP_REGISTER_OFFICE_ELEMENT2(text_section);

class text_section_source_attr
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

    odf_types::common_xlink_attlist	common_xlink_attlist_;

    _CP_OPT(std::wstring)	text_section_name_;
    _CP_OPT(std::wstring)	text_filter_name_;
    
	void serialize(CP_ATTR_NODE);   
};

//---------------------------------------------------------------------------------------------------
class text_section_source : public office_element_impl<text_section_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextSectionSource;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    text_section_source_attr text_section_source_attr_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_section_source);

//---------------------------------------------------------------------------------------------------
class text_table_of_content : public office_element_impl<text_table_of_content>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextTableOfContent;
    
	
    
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    text_section_attr	text_section_attr_;
    
	office_element_ptr	table_of_content_source_;
    office_element_ptr	index_body_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_table_of_content);

//---------------------------------------------------------------------------------------------------
class text_index_body : public office_element_impl<text_index_body>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextIndexBody;
    
    
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array index_content_main_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_index_body);

//---------------------------------------------------------------------------------------------------
class text_index_title : public office_element_impl<text_index_title>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextIndexTitle;
    
  	
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	text_section_attr			text_section_attr_;
    office_element_ptr_array	index_content_main_;
    
};
CP_REGISTER_OFFICE_ELEMENT2(text_index_title);

//---------------------------------------------------------------------------------------------------
class text_unknown_change : public office_element_impl<text_unknown_change>
{
public:
	static const wchar_t * ns;
    static const wchar_t * name;   	

    static const ElementType type		= typeTextUnknownChange;

    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    office_element_ptr			office_change_info_;
    office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_unknown_change);

//---------------------------------------------------------------------------------------------------
class text_insertion : public text_unknown_change
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextInsertion;

    virtual void serialize(std::wostream & _Wostream);
};

CP_REGISTER_OFFICE_ELEMENT2(text_insertion);

//---------------------------------------------------------------------------------------------------
class text_deletion : public text_unknown_change
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextDeletion;
  
	virtual void serialize(std::wostream & _Wostream);
};

CP_REGISTER_OFFICE_ELEMENT2(text_deletion);

//---------------------------------------------------------------------------------------------------
class text_format_change : public text_unknown_change
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextFormatChange;

	virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)	text_style_name_; //не по снецификации ... но КАК сохранить то что было изменено в формате?????

};
CP_REGISTER_OFFICE_ELEMENT2(text_format_change);

//---------------------------------------------------------------------------------------------------
class text_tracked_changes : public office_element_impl<text_tracked_changes>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextTrackedChanges;
    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

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

    static const ElementType type		= typeTextChangedRegion;
    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)	text_id_; //== xml:id
    office_element_ptr		element_;    
};
CP_REGISTER_OFFICE_ELEMENT2(text_changed_region);

//---------------------------------------------------------------------------------------------------
class text_add_change : public office_element_impl<text_add_change>
{
public:
 	static const wchar_t * ns;
    static const wchar_t * name;   	


    static const ElementType type		= typeTextChange;
   
	
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element)			{}

    virtual void serialize(std::wostream & _Wostream) = 0;

	_CP_OPT(std::wstring)	text_change_id_;    
};

//---------------------------------------------------------------------------------------------------
class text_change : public text_add_change
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextChange;  
   
	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(text_change);

//---------------------------------------------------------------------------------------------------
class text_change_start : public text_add_change
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextChangeStart;

	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(text_change_start);

//---------------------------------------------------------------------------------------------------
class text_change_end : public text_add_change
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextChangeEnd;
  
	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(text_change_end);
//---------------------------------------------------------------------------------------------------
// text:table-index
//---------------------------------------------------------------------------------------------------
class text_table_index : public office_element_impl<text_table_index>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType	type		= typeTextTableIndex;
    
	
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	odf_types::section_attlists	section_attr_;
    office_element_ptr			table_index_source_;
    office_element_ptr			index_body_;

};
CP_REGISTER_OFFICE_ELEMENT2(text_table_index);
//---------------------------------------------------------------------------------------------------
// text:illustration-index
//---------------------------------------------------------------------------------------------------
class text_illustration_index : public office_element_impl<text_illustration_index>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType	type		= typeTextIllustrationIndex;
    
	
  
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	odf_types::section_attlists	section_attr_;
    office_element_ptr			illustration_index_source_;
    office_element_ptr			index_body_;

};
CP_REGISTER_OFFICE_ELEMENT2(text_illustration_index);
//---------------------------------------------------------------------------------------------------
// text:alphabetical-index
//---------------------------------------------------------------------------------------------------
class text_alphabetical_index : public office_element_impl<text_alphabetical_index>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType	type		= typeTextAlphabeticalIndex;
    
	
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	odf_types::section_attlists	section_attr_;
    office_element_ptr			alphabetical_index_source_;
    office_element_ptr			index_body_;

};
CP_REGISTER_OFFICE_ELEMENT2(text_alphabetical_index);
//---------------------------------------------------------------------------------------------------
// text:user-index
//---------------------------------------------------------------------------------------------------
class text_user_index : public office_element_impl<text_user_index>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType	type		= typeTextUserIndex;
    
	
  
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	odf_types::section_attlists	section_attr_;
    office_element_ptr			user_index_source_;
    office_element_ptr			index_body_;

};
CP_REGISTER_OFFICE_ELEMENT2(text_user_index);
//------------------------------------------------------------------------------------------------------------
// text:bibliography
//------------------------------------------------------------------------------------------------------------
class text_bibliography : public office_element_impl<text_bibliography>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType	type		= typeTextBibliography;
    
	
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    odf_types::section_attlists	section_attr_;
    office_element_ptr			bibliography_source_;
    office_element_ptr			index_body_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_bibliography);
//------------------------------------------------------------------------------------------------------------
// text:bibliography-source
//------------------------------------------------------------------------------------------------------------
class text_bibliography_source: public office_element_impl<text_bibliography_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextBibliographySource;
    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	office_element_ptr			index_title_template_;
	office_element_ptr_array	entry_templates_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_bibliography_source);
//---------------------------------------------------------------------------------------------------
class common_entry_template : public office_element_impl<common_entry_template>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextCommonEntryTemplate;
    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)		style_name_;
	_CP_OPT(int)				outline_level_;
	office_element_ptr_array	content_;
};
//------------------------------------------------------------------------------------------------------------
// text:bibliography-entry-template
//------------------------------------------------------------------------------------------------------------
class text_bibliography_entry_template: public common_entry_template
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextBibliographyEntryTemplate;
    
    
    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)		bibliography_type_;//article, book, email, ...
};
CP_REGISTER_OFFICE_ELEMENT2(text_bibliography_entry_template);
//---------------------------------------------------------------------------------------------------
class text_index_title_template : public office_element_impl<text_index_title_template>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextIndexTitleTemplate;
    
  	
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)	style_name_;
	_CP_OPT(std::wstring)	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_index_title_template);
//---------------------------------------------------------------------------------------------------
//text:sequence-decl
//---------------------------------------------------------------------------------------------------
class text_sequence_decl : public office_element_impl<text_sequence_decl>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextSequenceDecl;
    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)	separation_character_; //one char
	_CP_OPT(unsigned int)	display_outline_level_;
	_CP_OPT(std::wstring)	name_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_sequence_decl);
//---------------------------------------------------------------------------------------------------
//text:sequence-decls
//---------------------------------------------------------------------------------------------------
class text_sequence_decls : public office_element_impl<text_sequence_decls>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextSequenceDecls;
    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_sequence_decls);
//---------------------------------------------------------------------------------------------------
//text:table-of-content-source
//---------------------------------------------------------------------------------------------------
class text_table_of_content_source: public office_element_impl<text_table_of_content_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextTableOfContentSource;
    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(int)				outline_level_;
	_CP_OPT(odf_types::Bool)	use_index_marks_;
	_CP_OPT(odf_types::Bool)	use_index_source_styles_;
	_CP_OPT(odf_types::Bool)	use_outline_level_;
	_CP_OPT(odf_types::Bool)	relative_tab_stop_position_;
	_CP_OPT(std::wstring)		index_scope_; // chapter or document:

	office_element_ptr			index_title_template_;
	office_element_ptr_array	entry_templates_;
	office_element_ptr_array	index_source_styles_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_table_of_content_source);

//---------------------------------------------------------------------------------------------------
//text:table-of-content-entry-template
//---------------------------------------------------------------------------------------------------
class text_table_of_content_entry_template : public common_entry_template
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextTableOfContentEntryTemplate;
    
};
CP_REGISTER_OFFICE_ELEMENT2(text_table_of_content_entry_template);
//---------------------------------------------------------------------------------------------------
//text:index-entry-bibliography
//---------------------------------------------------------------------------------------------------
class text_index_entry_bibliography: public office_element_impl<text_index_entry_bibliography>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextIndexEntryBibliography;
    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)		style_name_;
	_CP_OPT(std::wstring)		bibliography_data_field_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_index_entry_bibliography);
//---------------------------------------------------------------------------------------------------
//text:index-entry-chapter
//---------------------------------------------------------------------------------------------------
class text_index_entry_chapter: public office_element_impl<text_index_entry_chapter>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextIndexEntryChapter;
    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(text_index_entry_chapter);
//---------------------------------------------------------------------------------------------------
//text:index-entry-link-end
//---------------------------------------------------------------------------------------------------
class text_index_entry_link_end: public office_element_impl<text_index_entry_link_end>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextIndexEntryLinkEnd;
    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(text_index_entry_link_end);
//---------------------------------------------------------------------------------------------------
//text:index-entry-link-start
//---------------------------------------------------------------------------------------------------
class text_index_entry_link_start: public office_element_impl<text_index_entry_link_start>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextIndexEntryLinkStart;
    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(text_index_entry_link_start);
//---------------------------------------------------------------------------------------------------
//text:index-entry-page-number
//---------------------------------------------------------------------------------------------------
class text_index_entry_page_number: public office_element_impl<text_index_entry_page_number>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextIndexEntryPageNumber;
    

	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(text_index_entry_page_number);
//---------------------------------------------------------------------------------------------------
//text:index-entry-span
//---------------------------------------------------------------------------------------------------
class text_index_entry_span: public office_element_impl<text_index_entry_span>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextIndexEntrySpan;
    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(text_index_entry_span);
//---------------------------------------------------------------------------------------------------
//text:index-entry-tab-stop
//---------------------------------------------------------------------------------------------------
class text_index_entry_tab_stop: public office_element_impl<text_index_entry_tab_stop>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextIndexEntryTabStop;
    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(text_index_entry_tab_stop);
//---------------------------------------------------------------------------------------------------
//text:index-entry-text
//---------------------------------------------------------------------------------------------------
class text_index_entry_text: public office_element_impl<text_index_entry_text>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextIndexEntryText;
    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(text_index_entry_text);
//---------------------------------------------------------------------------------------------------
//text:illustration-index-source
//---------------------------------------------------------------------------------------------------
class text_illustration_index_source: public office_element_impl<text_illustration_index_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextIllustrationIndexSource;
    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);
	
	_CP_OPT(std::wstring)		caption_sequence_name_;
	_CP_OPT(std::wstring)		caption_sequence_format_;

	_CP_OPT(odf_types::Bool)	relative_tab_stop_position_;
	_CP_OPT(odf_types::Bool)	use_caption_;
	_CP_OPT(std::wstring)		index_scope_; // chapter or document:

	office_element_ptr			index_title_template_;
	office_element_ptr_array	entry_templates_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_illustration_index_source);
//---------------------------------------------------------------------------------------------------
//text:illustration-index-entry-template
//---------------------------------------------------------------------------------------------------
class text_illustration_index_entry_template : public common_entry_template
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextIllustrationIndexEntryTemplate;
    
};
CP_REGISTER_OFFICE_ELEMENT2(text_illustration_index_entry_template);
//---------------------------------------------------------------------------------------------------
//text:user-index-source
//---------------------------------------------------------------------------------------------------
class text_user_index_source: public office_element_impl<text_user_index_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextUserIndexSource;
    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);
	
	_CP_OPT(std::wstring)		index_name_;
	_CP_OPT(odf_types::Bool)	copy_outline_levels_;
	_CP_OPT(odf_types::Bool)	relative_tab_stop_position_;
	_CP_OPT(odf_types::Bool)	use_chart_objects_;
	_CP_OPT(odf_types::Bool)	use_draw_objects_;
	_CP_OPT(odf_types::Bool)	use_graphics_;
	_CP_OPT(odf_types::Bool)	use_floating_frames_;
	_CP_OPT(odf_types::Bool)	use_index_marks_;
	_CP_OPT(odf_types::Bool)	use_index_source_styles_;
	_CP_OPT(odf_types::Bool)	use_objects_;
	_CP_OPT(odf_types::Bool)	use_tables_;
	_CP_OPT(std::wstring)		index_scope_; // chapter or document:

	office_element_ptr			index_title_template_;
	office_element_ptr_array	entry_templates_;
	office_element_ptr_array	index_source_styles_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_user_index_source);
//---------------------------------------------------------------------------------------------------
//text:user-index-entry-template
//---------------------------------------------------------------------------------------------------
class text_user_index_entry_template : public common_entry_template
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextUserIndexEntryTemplate;
    
};
CP_REGISTER_OFFICE_ELEMENT2(text_user_index_entry_template);
//---------------------------------------------------------------------------------------------------
//text:alphabetical-index-source
//---------------------------------------------------------------------------------------------------
class alphabetical_index_source : public office_element_impl<alphabetical_index_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextAlphabeticalIndexSource;
    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);
	
	_CP_OPT(std::wstring)	index_scope_; // chapter or document:

//fo:country
//fo:language 
//fo:script
//style:rfclanguage-tag
//text:alphabetical-separators 
//text:capitalizeentries
//text:combine-entries 
//text:combine-entries-with-dash
//text:combine-entries-with-pp
//text:comma-separated
//text:ignore-case
//text:main-entry-style-name
//text:relative-tab-stop-position
//text:sort-algorithm
//text:use-keys-as-entries

	office_element_ptr			index_title_template_;
	office_element_ptr_array	entry_templates_;
};
CP_REGISTER_OFFICE_ELEMENT2(alphabetical_index_source);
//---------------------------------------------------------------------------------------------------
//text:alphabetical-index-entry-template
//---------------------------------------------------------------------------------------------------
class text_alphabetical_index_entry_template : public common_entry_template
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextAlphabeticalIndexEntryTemplate;
    
};
CP_REGISTER_OFFICE_ELEMENT2(text_alphabetical_index_entry_template);
//---------------------------------------------------------------------------------------------------
//text:table-index-source
//---------------------------------------------------------------------------------------------------
class text_table_index_source: public office_element_impl<text_table_index_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextTableIndexSource;
    
    
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);
	
	_CP_OPT(std::wstring)		caption_sequence_name_;
	_CP_OPT(std::wstring)		caption_sequence_format_;

	_CP_OPT(odf_types::Bool)	relative_tab_stop_position_;
	_CP_OPT(odf_types::Bool)	use_caption_;
	_CP_OPT(std::wstring)		index_scope_; // chapter or document:

	office_element_ptr			index_title_template_;
	office_element_ptr_array	entry_templates_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_table_index_source);
//---------------------------------------------------------------------------------------------------
//text:table-index-entry-template
//---------------------------------------------------------------------------------------------------
class text_table_index_entry_template : public common_entry_template
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeTextTableIndexEntryTemplate;
    
};
CP_REGISTER_OFFICE_ELEMENT2(text_table_index_entry_template);
}
}
