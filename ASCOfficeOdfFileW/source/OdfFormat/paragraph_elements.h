#pragma once

#include <iosfwd>
#include <vector>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPWeakPtr.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements_create.h"
#include "style_ref.h"
#include "targetframename.h"
#include "noteclass.h"

#include "common_attlists.h"

namespace cpdoccore { 
namespace odf {

// simple text
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_text : public office_element_impl<text_text>
{
public:
    static office_element_ptr create(const std::wstring & Text);

public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeText;
    static const ElementType type = typeTextText;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);
  
    text_text(const ::std::wstring & Text) :  text_(Text) {};
    text_text() {};

    std::wstring & attr_text() { return text_; };

    virtual void add_text(const std::wstring & Text);

    ::std::wstring text_;

};

// text:s
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_s : public office_element_impl<text_s>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextS;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    text_s(unsigned int c) : text_c_(c) {};
    text_s() {};

    _CP_OPT(unsigned int) text_c_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_s);

// text:tab
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_tab : public office_element_impl<text_tab>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextTab;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    text_tab(unsigned int ab_ref) : text_tab_ref_(ab_ref) {};
    text_tab() {};

    _CP_OPT(unsigned int) attr_tab_ref() const { return text_tab_ref_; }

    _CP_OPT(unsigned int) text_tab_ref_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_tab);

// text:line-break
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_line_break : public office_element_impl<text_line_break>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextLineBreak;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    text_line_break() {};

};

CP_REGISTER_OFFICE_ELEMENT2(text_line_break);

// text:bookmark
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_bookmark : public office_element_impl<text_bookmark>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextBookmark;
    CPDOCCORE_DEFINE_VISITABLE();

public:

	text_bookmark() {}
    text_bookmark(const ::std::wstring & Name) : text_name_(Name){};

 	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    ::std::wstring text_name_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_bookmark);

// text:bookmark-start
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_bookmark_start : public office_element_impl<text_bookmark_start>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextBookmarkStart;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    text_bookmark_start() {}
    text_bookmark_start(const ::std::wstring & Name) : text_name_(Name){};

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    ::std::wstring text_name_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_bookmark_start);

// text:bookmark-end
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_bookmark_end : public office_element_impl<text_bookmark_end>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextBookmarkEnd;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    text_bookmark_end() {} ;
    text_bookmark_end(const ::std::wstring & Name) : text_name_(Name){};

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    ::std::wstring text_name_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_bookmark_end);

// text:reference-mark
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_reference_mark : public office_element_impl<text_reference_mark>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextReferenceMark;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    text_reference_mark() {};
    text_reference_mark(const ::std::wstring & Name) : text_name_(Name){};
    
 	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    ::std::wstring text_name_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_reference_mark);

// text:reference-mark-start
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_reference_mark_start : public office_element_impl<text_reference_mark_start>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextReferenceMarkStart;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    text_reference_mark_start() {}
    text_reference_mark_start(const ::std::wstring & Name) : text_name_(Name){};

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    ::std::wstring text_name_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_reference_mark_start);

// text:reference-mark-end
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_reference_mark_end : public office_element_impl<text_reference_mark_end>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextReferenceMarkEnd;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    text_reference_mark_end() {};
    text_reference_mark_end(const ::std::wstring & Name) : text_name_(Name){};

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	::std::wstring text_name_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_reference_mark_end);

// text:span
//////////////////////////////////////////////////////////////////////////////////////////////////

class text_span : public office_element_impl<text_span>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSpan;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    text_span() {}
   
	office_element_ptr_array paragraph_content_;
    
	_CP_OPT(style_ref)	text_style_name_;//ваще то это такой элемент где стиль должќн быть всегда
    style_ref_array		text_class_names_;

    virtual void add_text(const std::wstring & Text);
};

CP_REGISTER_OFFICE_ELEMENT2(text_span);

// text:a
//////////////////////////////////////////////////////////////////////////////////////////////////

class text_a : public office_element_impl<text_a>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextA;
    CPDOCCORE_DEFINE_VISITABLE();

public:
	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    text_a() {}

    virtual void add_text(const std::wstring & Text);

    common_xlink_attlist common_xlink_attlist_;

    ::std::wstring office_name_;
    _CP_OPT(target_frame_name) office_target_frame_name_;

    style_ref text_style_name_;
    style_ref text_visited_style_name_;

    office_element_ptr_array paragraph_content_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_a);


// text:note
//////////////////////////////////////////////////////////////////////////////////////////////////

class text_note : public office_element_impl<text_note>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextNote;
    CPDOCCORE_DEFINE_VISITABLE();

public:
	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    text_note();

    virtual void add_text(const std::wstring & Text);

    ::std::wstring text_id_;
    noteclass text_note_class_;

    office_element_ptr text_note_citation_;
    office_element_ptr text_note_body_;
  
};

CP_REGISTER_OFFICE_ELEMENT2(text_note);

// text:ruby
//////////////////////////////////////////////////////////////////////////////////////////////////

class text_ruby : public office_element_impl<text_ruby>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextRuby;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    virtual void add_text(const std::wstring & Text);

	style_ref text_style_name_;

    office_element_ptr text_ruby_base_;
    office_element_ptr text_ruby_text_;
  
};

CP_REGISTER_OFFICE_ELEMENT2(text_ruby);

class common_field_fixed_attlist
{
public:

    _CP_OPT(bool) text_fixed_;

	void serialize(CP_ATTR_NODE);   
    
};

// text:title
//////////////////////////////////////////////////////////////////////////////////////////////////

class text_title : public office_element_impl<text_title>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextTitle;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    virtual void add_text(const std::wstring & Text);

    common_field_fixed_attlist common_field_fixed_attlist_;
    office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(text_title);

// text:placeholder
// text-placeholder
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_placeholder : public office_element_impl<text_placeholder>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextPlaceholder;
    CPDOCCORE_DEFINE_VISITABLE();
   
	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	virtual void add_text(const std::wstring & Text);

    office_element_ptr_array content_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_placeholder);

// text:page-number
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_page_number: public office_element_impl<text_page_number>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextPageNumber;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);


    virtual void add_text(const std::wstring & Text);

    office_element_ptr_array text_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_page_number);

// text:page-count
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_page_count : public office_element_impl<text_page_count>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextPageCount;
    CPDOCCORE_DEFINE_VISITABLE();
   
	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    virtual void add_text(const std::wstring & Text);

    office_element_ptr_array text_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_page_count);

// text:date 
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_date : public office_element_impl<text_date>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextDate;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    virtual void add_text(const std::wstring & Text);

	_CP_OPT(std::wstring)		style_data_style_name_;

	_CP_OPT(bool)				text_fixed_;
	_CP_OPT(std::wstring)		text_date_value_;//with format

    office_element_ptr_array text_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_date);

// text:time 
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_time : public office_element_impl<text_time>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextTime;
    CPDOCCORE_DEFINE_VISITABLE();
   
	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);


	virtual void add_text(const std::wstring & Text);

	_CP_OPT(std::wstring)		style_data_style_name_;
	_CP_OPT(bool)				text_fixed_;
	_CP_OPT(std::wstring)		text_time_value_;//with format

    office_element_ptr_array	text_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_time);

// text:file-name 
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_file_name : public office_element_impl<text_file_name>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextFileName;
    CPDOCCORE_DEFINE_VISITABLE();
   
	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    virtual void add_text(const std::wstring & Text);

private:
    // TODO: attributes
    office_element_ptr_array text_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_file_name);

// text:sequence
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_sequence : public office_element_impl<text_sequence>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSequence;
    CPDOCCORE_DEFINE_VISITABLE();
 
	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);


    virtual void add_text(const std::wstring & Text);

    office_element_ptr_array text_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_sequence);

// text:sheet-name
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_sheet_name : public office_element_impl<text_sheet_name>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSheetName;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);


    virtual void add_text(const std::wstring & Text);

    office_element_ptr_array text_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_sheet_name);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//presentation:footer
class presentation_footer : public office_element_impl<presentation_footer>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typePresentationFooter;
    CPDOCCORE_DEFINE_VISITABLE();
    
	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    
};
CP_REGISTER_OFFICE_ELEMENT2(presentation_footer);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//presentation:date-time
class presentation_date_time: public office_element_impl<presentation_date_time>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typePresentationDateTime;
    CPDOCCORE_DEFINE_VISITABLE();
    
	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    
};
CP_REGISTER_OFFICE_ELEMENT2(presentation_date_time);
} // namespace odf
} // namespace cpdoccore
