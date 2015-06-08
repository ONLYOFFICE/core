#pragma once

#include <iosfwd>
#include <vector>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPWeakPtr.h>
#include <cpdoccore/xml/nodetype.h>

#include "paragraph_content.h"
#include "office_elements_create.h"

#include "datatypes/style_ref.h"
#include "datatypes/targetframename.h"
#include "datatypes/noteclass.h"

#include "../docx/docx_conversion_context.h"

#include "datatypes/common_attlists.h"

namespace cpdoccore { 
namespace odf_reader {


namespace text {

template <class ElementT>
class paragraph_content_element :  public paragraph_content_impl<ElementT>
{
};

// TODO:
// 5.6 Индексные метки
// 5.7 Метки изменений
// 5.8 Внедренные графические объекты и текстовые блоки

// simple text
//////////////////////////////////////////////////////////////////////////////////////////////////
class text : public paragraph_content_element<text>
{
public:
    static office_element_ptr create(const std::wstring & Text);
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeText;
    static const ElementType type = typeTextText;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

public:
    text(const ::std::wstring & Text) :  text_(Text) {};
    text() {};

    std::wstring & attr_text() { return text_; };

private:
    
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ) 
    {};

    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
    {};

    virtual void add_text(const std::wstring & Text);

private:
    ::std::wstring text_;

};

// text:s
//////////////////////////////////////////////////////////////////////////////////////////////////
class s : public paragraph_content_element<s>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextS;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
    

public:
    s(unsigned int c) : text_c_(c) {};
    s() {};

    _CP_OPT(unsigned int) attr_c() const { return text_c_; }

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ) ;
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text) {}

private:
    _CP_OPT(unsigned int) text_c_;
};

CP_REGISTER_OFFICE_ELEMENT2(s);

// text:tab
//////////////////////////////////////////////////////////////////////////////////////////////////
class tab : public paragraph_content_element<tab>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextTab;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual void docx_convert(oox::docx_conversion_context & Context) ;
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context) ;
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;
    
public:
    virtual std::wostream & text_to_stream(::std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text) {}

public:
    tab(unsigned int ab_ref) : text_tab_ref_(ab_ref) {};
    tab() {};

    _CP_OPT(unsigned int) attr_tab_ref() const { return text_tab_ref_; }

private:
    _CP_OPT(unsigned int) text_tab_ref_;
};

CP_REGISTER_OFFICE_ELEMENT2(tab);

// text:line-break
//////////////////////////////////////////////////////////////////////////////////////////////////
class line_break : public paragraph_content_element<line_break>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextLineBreak;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

public:
    line_break() {};

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ) {}
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text) {}

};

CP_REGISTER_OFFICE_ELEMENT2(line_break);

// text:bookmark
//////////////////////////////////////////////////////////////////////////////////////////////////
class bookmark : public paragraph_content_element<bookmark>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextBookmark;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual std::wostream & text_to_stream(::std::wostream & _Wostream) const;
public:
    bookmark() {}
    bookmark(const ::std::wstring & Name) : text_name_(Name){};

    const ::std::wstring & attr_name() const { return text_name_; }

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text) {}

private:
    std::wstring text_name_;
};

CP_REGISTER_OFFICE_ELEMENT2(bookmark);

// text:bookmark-start
//////////////////////////////////////////////////////////////////////////////////////////////////
class bookmark_start : public paragraph_content_element<bookmark_start>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextBookmarkStart;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual std::wostream & text_to_stream(::std::wostream & _Wostream) const;
public:
    bookmark_start() {}
    bookmark_start(const ::std::wstring & Name) : text_name_(Name){};

    const std::wstring & attr_name() const { return text_name_; }

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text) {}

private:
    std::wstring text_name_;
};

CP_REGISTER_OFFICE_ELEMENT2(bookmark_start);

// text:bookmark-end
//////////////////////////////////////////////////////////////////////////////////////////////////
class bookmark_end : public paragraph_content_element<bookmark_end>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextBookmarkEnd;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
public:
    bookmark_end() {} ;
    bookmark_end(const ::std::wstring & Name) : text_name_(Name){};

    const ::std::wstring & attr_name() const { return text_name_; }

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text) {}

private:
    std::wstring text_name_;
};

CP_REGISTER_OFFICE_ELEMENT2(bookmark_end);

// text:reference-mark
//////////////////////////////////////////////////////////////////////////////////////////////////
class reference_mark : public paragraph_content_element<reference_mark>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextReferenceMark;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
public:
    reference_mark() {};
    reference_mark(const ::std::wstring & Name) : text_name_(Name){};
    
    const std::wstring & attr_name() const { return text_name_; }

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text) {}

private:
    std::wstring text_name_;
};

CP_REGISTER_OFFICE_ELEMENT2(reference_mark);

// text:reference-mark-start
//////////////////////////////////////////////////////////////////////////////////////////////////
class reference_mark_start : public paragraph_content_element<reference_mark_start>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextReferenceMarkStart;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual std::wostream & text_to_stream(::std::wostream & _Wostream) const;
public:
    reference_mark_start() {}
    reference_mark_start(const ::std::wstring & Name) : text_name_(Name){};

    const ::std::wstring & attr_name() const { return text_name_; }

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text) {}

private:
    std::wstring text_name_;
};

CP_REGISTER_OFFICE_ELEMENT2(reference_mark_start);

// text:reference-mark-end
//////////////////////////////////////////////////////////////////////////////////////////////////
class reference_mark_end : public paragraph_content_element<reference_mark_end>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextReferenceMarkEnd;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual std::wostream & text_to_stream(::std::wostream & _Wostream) const;
public:
    reference_mark_end() {};
    reference_mark_end(const ::std::wstring & Name) : text_name_(Name){};

    const ::std::wstring & attr_name() const { return text_name_; }

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text) {}

private:
    std::wstring text_name_;
};

CP_REGISTER_OFFICE_ELEMENT2(reference_mark_end);

// text:span
//////////////////////////////////////////////////////////////////////////////////////////////////

class span : public paragraph_content_element<span>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSpan;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
public:
    span() {}
   
	office_element_ptr_array paragraph_content_;
    
	odf_types::style_ref			text_style_name_;
    odf_types::style_ref_array		text_class_names_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);
};

CP_REGISTER_OFFICE_ELEMENT2(span);

// text:a
//////////////////////////////////////////////////////////////////////////////////////////////////

class a : public paragraph_content_element<a>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextA;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
public:
    a() {}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
	odf_types::common_xlink_attlist common_xlink_attlist_;

    std::wstring office_name_;
    _CP_OPT(odf_types::target_frame_name) office_target_frame_name_;

    odf_types::style_ref text_style_name_;
    odf_types::style_ref text_visited_style_name_;

    office_element_ptr_array paragraph_content_;
};

CP_REGISTER_OFFICE_ELEMENT2(a);


// text:note
//////////////////////////////////////////////////////////////////////////////////////////////////

class note : public paragraph_content_element<note>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextNote;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    void docx_convert(oox::docx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context);

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
public:
    note();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
	std::wstring text_id_;
	odf_types::noteclass text_note_class_;
    office_element_ptr text_note_citation_;
    office_element_ptr text_note_body_;
  
};

CP_REGISTER_OFFICE_ELEMENT2(note);

// text:ruby
//////////////////////////////////////////////////////////////////////////////////////////////////

class ruby : public paragraph_content_element<ruby>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextRuby;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    odf_types::style_ref text_style_name_;
    office_element_ptr text_ruby_base_;
    office_element_ptr text_ruby_text_;
  
};

CP_REGISTER_OFFICE_ELEMENT2(ruby);

class common_field_fixed_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    
private:
    _CP_OPT(bool) text_fixed_;
    
};

// text:title
//////////////////////////////////////////////////////////////////////////////////////////////////

class title : public paragraph_content_element<title>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextTitle;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    common_field_fixed_attlist common_field_fixed_attlist_;
    office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(title);

// text:placeholder
// text-placeholder
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_placeholder : public paragraph_content_element<text_placeholder>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextPlaceholder;
    CPDOCCORE_DEFINE_VISITABLE();
   
	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    office_element_ptr_array content_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_placeholder);

// text:page-number
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_page_number: public paragraph_content_element<text_page_number>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextPageNumber;
    CPDOCCORE_DEFINE_VISITABLE();
  
	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    // TODO: attributes
    office_element_ptr_array text_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_page_number);

// text:page-count
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_page_count : public paragraph_content_element<text_page_count>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextPageCount;
    CPDOCCORE_DEFINE_VISITABLE();
   
	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    // TODO: attributes
    office_element_ptr_array text_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_page_count);

// text:date 
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_date : public paragraph_content_element<text_date>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextDate;
    CPDOCCORE_DEFINE_VISITABLE();
    
	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
	_CP_OPT(std::wstring)		style_data_style_name_;

	_CP_OPT(bool)				text_fixed_;
	_CP_OPT(std::wstring)		text_date_value_;//with format

    office_element_ptr_array text_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_date);

// text:time 
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_time : public paragraph_content_element<text_time>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextTime;
    CPDOCCORE_DEFINE_VISITABLE();
   
	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
	_CP_OPT(std::wstring)		style_data_style_name_;
	_CP_OPT(bool)				text_fixed_;
	_CP_OPT(std::wstring)		text_time_value_;//with format

    office_element_ptr_array	text_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_time);

// text:file-name 
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_file_name : public paragraph_content_element<text_file_name>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextFileName;
    CPDOCCORE_DEFINE_VISITABLE();
   
	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    // TODO: attributes
    office_element_ptr_array text_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_file_name);

// text:sequence
//////////////////////////////////////////////////////////////////////////////////////////////////
class sequence : public paragraph_content_element<sequence>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSequence;
    CPDOCCORE_DEFINE_VISITABLE();
 
	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

public:
    virtual std::wostream & text_to_stream(::std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    // TODO: attributes
    office_element_ptr_array text_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(sequence);

// text:sheet-name
//////////////////////////////////////////////////////////////////////////////////////////////////
class sheet_name : public paragraph_content_element<sheet_name>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSheetName;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    office_element_ptr_array text_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(sheet_name);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//presentation:footer
class presentation_footer : public paragraph_content_element<presentation_footer>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typePresentationFooter;
    CPDOCCORE_DEFINE_VISITABLE();
    
	//void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
	virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name){}
    
};
CP_REGISTER_OFFICE_ELEMENT2(presentation_footer);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//presentation:date-time
class presentation_date_time: public paragraph_content_element<presentation_date_time>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typePresentationDateTime;
    CPDOCCORE_DEFINE_VISITABLE();
    
	//void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
	virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name){}
    
};
CP_REGISTER_OFFICE_ELEMENT2(presentation_date_time);
} // namespace text
} // namespace odf_reader
} // namespace cpdoccore
