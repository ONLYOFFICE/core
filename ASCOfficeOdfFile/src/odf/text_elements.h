#pragma once

#include <iosfwd>

#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "text_content.h"
#include "office_elements_create.h"

#include "datatypes/textdisplay.h"

#include "datatypes/common_attlists.h"
#include "datatypes/style_ref.h"

namespace cpdoccore { 
namespace odf {
namespace text {

class paragraph
{
public:
    paragraph() : next_par_(NULL), next_section_(false), next_end_section_(false) {}

public:
    ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name, document_context * Context);
    void add_text(const std::wstring & Text);

    paragraph * get_next() { return next_par_; }
    void set_next(paragraph * next) {next_par_ = next;}
    void set_next_section(bool Val) 
    {
        next_section_ = Val;
    }

    void set_next_end_section(bool Val) 
    {
        next_end_section_ = Val;
    }

    void afterCreate(document_context * ctx);
   
	void docx_convert(oox::docx_conversion_context & Context) ;
    void xlsx_convert(oox::xlsx_conversion_context & Context) ;
    void pptx_convert(oox::pptx_conversion_context & Context) ;

	void drop_cap_docx_convert(oox::docx_conversion_context & Context);

private:
	void drop_cap_text_docx_convert(office_element_ptr first_text_paragraph,oox::docx_conversion_context & Context);
    paragraph_attrs paragraph_attrs_;

    office_element_ptr_array paragraph_content_;
    paragraph * next_par_;
    bool next_section_;
    bool next_end_section_;
    friend class par_docx_convert_class;   
    
};

// text:h
//////////////////////////////////////////////////////////////////////////////////////////////////
class h : public text_content_impl<h>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextH;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    void docx_convert(oox::docx_conversion_context & Context) ;
    void xlsx_convert(oox::xlsx_conversion_context & Context) ;
    void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual void afterCreate();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

public:
    h();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    // heading-attrs
    unsigned int text_outline_level_;
    bool text_restart_numbering_; // default false
    _CP_OPT(unsigned int) text_start_value_;
    bool text_is_list_header_; // default false
    _CP_OPT(std::wstring) text_number_;
    
    paragraph paragraph_;

    friend class par_docx_convert_class;

};

CP_REGISTER_OFFICE_ELEMENT2(h);

// text:p
//////////////////////////////////////////////////////////////////////////////////////////////////
class p : public text_content_impl<p>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextP;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    void docx_convert(oox::docx_conversion_context & Context) ;
    void xlsx_convert(oox::xlsx_conversion_context & Context) ;
	void pptx_convert(oox::pptx_conversion_context & Context) ;

	virtual void afterCreate();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

public:
    p(){};
	paragraph paragraph_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);
private:

    friend class par_docx_convert_class;   
};

CP_REGISTER_OFFICE_ELEMENT2(p);

// text:list
//////////////////////////////////////////////////////////////////////////////////////////////////
class list : public text_content_impl<list>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextList;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

public:
    list(){};

public:
    void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    style_ref text_style_name_;
    _CP_OPT(bool) text_continue_numbering_;

    office_element_ptr          text_list_header_;
    office_element_ptr_array    text_list_items_;

};

CP_REGISTER_OFFICE_ELEMENT2(list);

// text:soft-page-break
//////////////////////////////////////////////////////////////////////////////////////////////////
class soft_page_break : public text_content_impl<soft_page_break>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSoftPageBreak;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

};

CP_REGISTER_OFFICE_ELEMENT2(soft_page_break);

class text_section_attr
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(style_ref) text_style_name_;
    std::wstring text_name_;
    _CP_OPT(bool) text_protected_;
    _CP_OPT(std::wstring) text_protection_key_;
    _CP_OPT(text_display) text_display_;
    _CP_OPT(std::wstring) text_condition_;

};

// text:section
//////////////////////////////////////////////////////////////////////////////////////////////////
class text_section : public text_content_impl<text_section>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSection;
    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context);
	//void pptx_convert(oox::pptx_conversion_context & Context) ;

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
    virtual void afterCreate();
    virtual void afterReadContent();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

private:
    text_section_attr text_section_attr_;
    office_element_ptr text_section_source_;
    office_element_ptr_array text_content_;          

};

CP_REGISTER_OFFICE_ELEMENT2(text_section);

// text-section-source-attr
class text_section_source_attr
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

private:
    common_xlink_attlist	common_xlink_attlist_;

    _CP_OPT(std::wstring)	text_section_name_;
    _CP_OPT(std::wstring)	text_filter_name_;
    
};

// text:section-source
// text-section-source
//////////////////////////////////////////////////////////////////////////////////////////////////
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

private:
    text_section_source_attr text_section_source_attr_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_section_source);

// text:table-of-content
// text-table-of-content
//////////////////////////////////////////////////////////////////////////////////////////////////
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

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    text_section_attr text_section_attr_;
    office_element_ptr text_table_of_content_source_;
    office_element_ptr text_index_body_;

};

CP_REGISTER_OFFICE_ELEMENT2(text_table_of_content);

// text:index-body
// text-index-body
//////////////////////////////////////////////////////////////////////////////////////////////////
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

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    office_element_ptr_array index_content_main_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_index_body);

// text:index-title
// text-index-title
//////////////////////////////////////////////////////////////////////////////////////////////////
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

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    text_section_attr text_section_attr_;
    office_element_ptr_array index_content_main_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_index_title);

}
}
}
