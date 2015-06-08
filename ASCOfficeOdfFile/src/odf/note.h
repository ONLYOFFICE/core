#pragma once

#include <cpdoccore/CPOptional.h>
#include "office_elements.h"
#include "office_elements_create.h"

namespace cpdoccore { 
namespace odf_reader {

namespace text {

// text:note-citation
//////////////////////////////////////////////////////////////////////////////////////////////////
class note_citation : public office_element_impl<note_citation>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextNoteCitation;
    CPDOCCORE_DEFINE_VISITABLE();
    virtual void docx_convert(oox::docx_conversion_context & Context) ;

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

public:
    note_citation() {} 

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    ::std::wstring text_label_;
    office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(note_citation);

// text:note-body
//////////////////////////////////////////////////////////////////////////////////////////////////
class note_body : public office_element_impl<note_body>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextNoteBody;
    CPDOCCORE_DEFINE_VISITABLE();
    virtual void docx_convert(oox::docx_conversion_context & Context) ;

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
public:
    note_body() {} 

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    office_element_ptr_array    content_;

};

CP_REGISTER_OFFICE_ELEMENT2(note_body);


}
}
}

