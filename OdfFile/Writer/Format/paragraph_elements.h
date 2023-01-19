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
#include <vector>
#include <CPOptional.h>
#include <CPWeakPtr.h>
#include <xml/nodetype.h>

#include "office_elements_create.h"

#include "../../DataTypes/targetframename.h"
#include "../../DataTypes/noteclass.h"
#include "../../DataTypes/bibliography.h"

#include "../../DataTypes/common_attlists.h"

namespace cpdoccore { 
namespace odf_writer {

// simple text
//---------------------------------------------------------------------------------------------------
class text_text : public office_element_impl<text_text>
{
public:
    static office_element_ptr create(const std::wstring & Text);

public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const ElementType type = typeTextText;    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

	virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
	virtual void serialize(std::wostream & _Wostream);
  
    text_text(const std::wstring & Text) :  text_(Text) {}
    text_text() {}

    std::wstring & attr_text() { return text_; }

    virtual void add_text(const std::wstring & Text);

    std::wstring text_;

};

// text:s
//---------------------------------------------------------------------------------------------------
class text_s : public office_element_impl<text_s>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextS;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    text_s(unsigned int c) : text_c_(c) {};
    text_s() {};

    _CP_OPT(unsigned int) text_c_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_s);

// text:tab
//---------------------------------------------------------------------------------------------------
class text_tab : public office_element_impl<text_tab>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextTab;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    text_tab(unsigned int ab_ref) : text_tab_ref_(ab_ref) {};
    text_tab() {};

    _CP_OPT(unsigned int) attr_tab_ref() const { return text_tab_ref_; }

    _CP_OPT(unsigned int) text_tab_ref_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_tab);

// text:line-break
//---------------------------------------------------------------------------------------------------
class text_line_break : public office_element_impl<text_line_break>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextLineBreak;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    text_line_break() {};

};
CP_REGISTER_OFFICE_ELEMENT2(text_line_break);

// text:bookmark
//---------------------------------------------------------------------------------------------------
class text_bookmark : public office_element_impl<text_bookmark>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextBookmark;
    
	text_bookmark() {}
    text_bookmark(const std::wstring & Name) : text_name_(Name){};

 	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    std::wstring text_name_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_bookmark);

// text:bookmark-start
//---------------------------------------------------------------------------------------------------
class text_bookmark_start : public office_element_impl<text_bookmark_start>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextBookmarkStart;
    
    text_bookmark_start() {}
    text_bookmark_start(const std::wstring & Name) : text_name_(Name){};

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    std::wstring text_name_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_bookmark_start);

// text:bookmark-end
//---------------------------------------------------------------------------------------------------
class text_bookmark_end : public office_element_impl<text_bookmark_end>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextBookmarkEnd;

    text_bookmark_end() {} ;
    text_bookmark_end(const std::wstring & Name) : text_name_(Name){};

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    std::wstring text_name_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_bookmark_end);

// text:reference-mark
//---------------------------------------------------------------------------------------------------
class text_reference_mark : public office_element_impl<text_reference_mark>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextReferenceMark;

    text_reference_mark() {};
    text_reference_mark(const std::wstring & Name) : text_name_(Name){};
    
 	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    std::wstring text_name_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_reference_mark);

// text:reference-mark-start
//---------------------------------------------------------------------------------------------------
class text_reference_mark_start : public office_element_impl<text_reference_mark_start>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextReferenceMarkStart;
    
    text_reference_mark_start() {}
    text_reference_mark_start(const std::wstring & Name) : text_name_(Name){}

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    std::wstring text_name_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_reference_mark_start);

// text:reference-mark-end
//---------------------------------------------------------------------------------------------------
class text_reference_mark_end : public office_element_impl<text_reference_mark_end>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextReferenceMarkEnd;
    
    text_reference_mark_end() {};
    text_reference_mark_end(const std::wstring & Name) : text_name_(Name){};

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	std::wstring text_name_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_reference_mark_end);

// text:span
//---------------------------------------------------------------------------------------------------
class text_span : public office_element_impl<text_span>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextSpan;    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);
	virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

    text_span() {}
   
	office_element_ptr_array content_;
    
	_CP_OPT(std::wstring)		text_style_name_;//ваще то это такой элемент где стиль должОн быть всегда
    std::vector<std::wstring>	text_class_names_;

    virtual void add_text(const std::wstring & Text);
};
CP_REGISTER_OFFICE_ELEMENT2(text_span);

// text:a
//---------------------------------------------------------------------------------------------------
class text_a : public office_element_impl<text_a>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextA;
    
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    text_a() {}

    virtual void add_text(const std::wstring & Text);

    odf_types::common_xlink_attlist common_xlink_attlist_;

    _CP_OPT(std::wstring)					office_name_;
    _CP_OPT(odf_types::target_frame_name)	office_target_frame_name_;

    _CP_OPT(std::wstring)					text_style_name_;
    _CP_OPT(std::wstring)					text_visited_style_name_;

    office_element_ptr_array				paragraph_content_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_a);

// text:note-citation
//---------------------------------------------------------------------------------------------------
class text_note_citation : public office_element_impl<text_note_citation>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextNoteCitation;
    
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    virtual void add_text(const std::wstring & Text);

    text_note_citation() {} 

    _CP_OPT(std::wstring)		text_label_;
    office_element_ptr_array	content_;

};
CP_REGISTER_OFFICE_ELEMENT2(text_note_citation);

// text:note-body
//---------------------------------------------------------------------------------------------------
class text_note_body : public office_element_impl<text_note_body>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextNoteBody;
    
	
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	virtual void add_text(const std::wstring & Text){}

    text_note_body() {} 

	office_element_ptr_array    content_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_note_body);


// text:note
//---------------------------------------------------------------------------------------------------
class text_note : public office_element_impl<text_note>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextNote;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    virtual void add_text(const std::wstring & Text);

    text_note();

    std::wstring			text_id_;
    odf_types::noteclass	text_note_class_;

    office_element_ptr		text_note_citation_;
    office_element_ptr		text_note_body_;
  
};
CP_REGISTER_OFFICE_ELEMENT2(text_note);

// text:ruby
//---------------------------------------------------------------------------------------------------
class text_ruby : public office_element_impl<text_ruby>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextRuby;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    virtual void add_text(const std::wstring & Text);

	std::wstring		text_style_name_;

    office_element_ptr	text_ruby_base_;
    office_element_ptr	text_ruby_text_;
  
};
CP_REGISTER_OFFICE_ELEMENT2(text_ruby);

class common_field_fixed_attlist
{
public:

    _CP_OPT(odf_types::Bool) text_fixed_;

	void serialize(CP_ATTR_NODE);   
    
};

// text:title
//---------------------------------------------------------------------------------------------------
class text_title : public office_element_impl<text_title>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextTitle;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    virtual void add_text(const std::wstring & Text);

    common_field_fixed_attlist common_field_fixed_attlist_;
    office_element_ptr_array content_;

};
CP_REGISTER_OFFICE_ELEMENT2(text_title);

// text:placeholder
//---------------------------------------------------------------------------------------------------
class text_placeholder : public office_element_impl<text_placeholder>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextPlaceholder;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	virtual void add_text(const std::wstring & Text);

    office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_placeholder);

// text:page-number
//---------------------------------------------------------------------------------------------------
class text_page_number: public office_element_impl<text_page_number>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextPageNumber;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	odf_types::common_num_format_attlist	common_num_format_attlist_;
	common_field_fixed_attlist				common_field_fixed_attlist_;

	_CP_OPT(int)			text_page_adjust_;
	_CP_OPT(std::wstring)	text_select_page_;

    virtual void add_text(const std::wstring & Text);

    office_element_ptr_array text_;    
};
CP_REGISTER_OFFICE_ELEMENT2(text_page_number);

// text:page-count
//---------------------------------------------------------------------------------------------------
class text_page_count : public office_element_impl<text_page_count>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextPageCount;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);
    virtual void add_text(const std::wstring & Text);

	common_field_fixed_attlist common_field_fixed_attlist_;

    office_element_ptr_array text_;    
};
CP_REGISTER_OFFICE_ELEMENT2(text_page_count);

// text:date 
//---------------------------------------------------------------------------------------------------
class text_date : public office_element_impl<text_date>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextDate;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);
    virtual void add_text(const std::wstring & Text);

	common_field_fixed_attlist	common_field_fixed_attlist_;

	_CP_OPT(std::wstring)		style_data_style_name_;
	_CP_OPT(std::wstring)		text_date_value_;//with format

    office_element_ptr_array	text_;    
};
CP_REGISTER_OFFICE_ELEMENT2(text_date);

// text:time 
//---------------------------------------------------------------------------------------------------
class text_time : public office_element_impl<text_time>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextTime;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);
    virtual void add_text(const std::wstring & Text);

	common_field_fixed_attlist		common_field_fixed_attlist_;

	_CP_OPT(std::wstring)			style_data_style_name_;
	_CP_OPT(std::wstring)			text_time_value_;//with format

    office_element_ptr_array		text_;    
};
CP_REGISTER_OFFICE_ELEMENT2(text_time);

// text:file-name 
//---------------------------------------------------------------------------------------------------
class text_file_name : public office_element_impl<text_file_name>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextFileName;
    
   
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element		( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);
    virtual void add_text(const std::wstring & Text);

	common_field_fixed_attlist		common_field_fixed_attlist_;

    office_element_ptr_array text_;    
};
CP_REGISTER_OFFICE_ELEMENT2(text_file_name);

// text:sequence
//---------------------------------------------------------------------------------------------------
class text_sequence : public office_element_impl<text_sequence>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextSequence;
    
 
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(odf_types::style_numformat)	style_num_format_;
	_CP_OPT(std::wstring)				style_num_letter_sync_;
	_CP_OPT(std::wstring)				formula_;
	_CP_OPT(std::wstring)				name_;
	_CP_OPT(std::wstring)				ref_name_;

	_CP_OPT(std::wstring)		template_;
    office_element_ptr_array	text_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_sequence);

// text:text-input
//---------------------------------------------------------------------------------------------------
class text_text_input : public office_element_impl<text_text_input>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextTextInput;
    
 
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(std::wstring)		text_description_;
    office_element_ptr_array	text_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_text_input);

// text:sheet-name
//---------------------------------------------------------------------------------------------------
class text_sheet_name : public office_element_impl<text_sheet_name>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextSheetName;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);
    virtual void add_text(const std::wstring & Text);

    office_element_ptr_array text_;    
};
CP_REGISTER_OFFICE_ELEMENT2(text_sheet_name);

//---------------------------------------------------------------------------------------------------
//presentation:footer
class presentation_footer : public office_element_impl<presentation_footer>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typePresentationFooter;
    
    
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);    
};
CP_REGISTER_OFFICE_ELEMENT2(presentation_footer);

//---------------------------------------------------------------------------------------------------
//presentation:date-time
class presentation_date_time: public office_element_impl<presentation_date_time>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typePresentationDateTime;
    
    
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);    
};
CP_REGISTER_OFFICE_ELEMENT2(presentation_date_time);
//---------------------------------------------------------------------------------------------------
//text:toc-mark-start
//---------------------------------------------------------------------------------------------------
class text_toc_mark_start : public office_element_impl<text_toc_mark_start>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType	type		= typeTextTocMarkStart;    
	

	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    _CP_OPT(int)	outline_level_;
	std::wstring	id_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_toc_mark_start);
//---------------------------------------------------------------------------------------------------
//text:toc-mark-end
//---------------------------------------------------------------------------------------------------
class text_toc_mark_end : public office_element_impl<text_toc_mark_end>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType	type		= typeTextTocMarkEnd;    
	

	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	std::wstring	id_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_toc_mark_end);
//---------------------------------------------------------------------------------------------------
//text:toc-mark
//---------------------------------------------------------------------------------------------------
class text_toc_mark : public office_element_impl<text_toc_mark>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType	type		= typeTextTocMark;    
	

	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)	string_value_;
    _CP_OPT(int)			outline_level_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_toc_mark);
//---------------------------------------------------------------------------------------------------
//text:alphabetical-index-mark-start
//---------------------------------------------------------------------------------------------------
class text_alphabetical_index_mark_start : public office_element_impl<text_alphabetical_index_mark_start>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType	type		= typeTextAlphabeticalIndexMarkStart;    
	

	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	std::wstring				id_;
	_CP_OPT(std::wstring)		key1_;
	_CP_OPT(std::wstring)		key1_phonetic_;
	_CP_OPT(std::wstring)		key2_;
	_CP_OPT(std::wstring)		key2_phonetic_;
	_CP_OPT(odf_types::Bool)	main_entry_;
	_CP_OPT(std::wstring)		string_value_phonetic_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_alphabetical_index_mark_start);
//---------------------------------------------------------------------------------------------------
//text:alphabetical-index-mark-end
//---------------------------------------------------------------------------------------------------
class text_alphabetical_index_mark_end : public office_element_impl<text_alphabetical_index_mark_end>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType	type		= typeTextAlphabeticalIndexMarkEnd;    
	

	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	std::wstring id_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_alphabetical_index_mark_end);
//---------------------------------------------------------------------------------------------------
//text:alphabetical-index-mark
//---------------------------------------------------------------------------------------------------
class text_alphabetical_index_mark : public office_element_impl<text_alphabetical_index_mark>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType	type		= typeTextAlphabeticalIndexMark;    
	

	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)		key1_;
	_CP_OPT(std::wstring)		key1_phonetic_;
	_CP_OPT(std::wstring)		key2_;
	_CP_OPT(std::wstring)		key2_phonetic_;
	_CP_OPT(odf_types::Bool)	main_entry_;
	_CP_OPT(std::wstring)		string_value_;
	_CP_OPT(std::wstring)		string_value_phonetic_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_alphabetical_index_mark);
//---------------------------------------------------------------------------------------------------
//text:bibliography-mark
//---------------------------------------------------------------------------------------------------
class text_bibliography_mark : public office_element_impl<text_bibliography_mark>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType	type		= typeTextBibliographyMark;

	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    std::wstring			identifier_;
	odf_types::bibliography	bibliography_type_; 
    
	_CP_OPT(std::wstring)	url_;
    _CP_OPT(std::wstring)	author_;
    _CP_OPT(std::wstring)	title_;
    _CP_OPT(std::wstring)	year_;
    _CP_OPT(std::wstring)	isbn_;
	_CP_OPT(std::wstring)	chapter_;
	_CP_OPT(std::wstring)	address_;
	_CP_OPT(std::wstring)	annote_;
	_CP_OPT(std::wstring)	booktitle_;
	_CP_OPT(std::wstring)	edition_;
	_CP_OPT(std::wstring)	editor_;
	_CP_OPT(std::wstring)	howpublished_;
	_CP_OPT(std::wstring)	institution_;
	_CP_OPT(std::wstring)	issn_;
	_CP_OPT(std::wstring)	journal_;
	_CP_OPT(std::wstring)	month_;
	_CP_OPT(std::wstring)	note_;
	_CP_OPT(std::wstring)	number_;
	_CP_OPT(std::wstring)	organizations_;
	_CP_OPT(std::wstring)	pages_;
	_CP_OPT(std::wstring)	publisher_;
	_CP_OPT(std::wstring)	report_type_;
	_CP_OPT(std::wstring)	school_;
	_CP_OPT(std::wstring)	series_;
	_CP_OPT(std::wstring)	volume_;

	_CP_OPT(std::wstring)	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_bibliography_mark);
//-------------------------------------------------------------------------------------------------------------------
// text:sequence-ref
//-------------------------------------------------------------------------------------------------------------------
class text_sequence_ref : public office_element_impl<text_sequence_ref>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextSequenceRef;
    
 
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)	reference_format_;//caption, category-and-value, value, chapter, direction, page, text, number, number-all-superior, number-no-superior
	_CP_OPT(std::wstring)	ref_name_;
   
	std::wstring			content_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_sequence_ref);
//-------------------------------------------------------------------------------------------------------------------
//text:drop-down
//-------------------------------------------------------------------------------------------------------------------
class text_drop_down : public office_element_impl<text_drop_down>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextDropDown;
    
 
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element		( const office_element_ptr & child_element);

    virtual void add_text(const std::wstring & Text);
    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)		text_name_;
    office_element_ptr_array	content_;
    
    office_element_ptr			text_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_drop_down);
//-------------------------------------------------------------------------------------------------------------------
//text:label
//-------------------------------------------------------------------------------------------------------------------
class text_label : public office_element_impl<text_label>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextLabel;
    
 
	virtual void create_child_element	(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element		( const office_element_ptr & child_element){}

    virtual void add_text(const std::wstring & Text);
    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(odf_types::Bool)text_current_selected_;
	_CP_OPT(std::wstring)	text_value_;
    _CP_OPT(std::wstring)	text_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_label);
} // namespace odf_writer
} // namespace cpdoccore
